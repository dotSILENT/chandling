#include "Hooks.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "raknet\RakClientInterface.h"
#include "hook_utils.h"
#include "PacketEnum.h"
#include "Actions.h"
#include "ActionCallbacks.h"
#include "HandlingDefault.h"
#include "HandlingManager.h"

#include <queue>
#include <unordered_map>

/** Hook Typedefs */
typedef CVehicle*(__cdecl *tCreateCar)(DWORD model, CVector pos, BYTE mission); // CCarCtrlCreateCar
typedef int(__thiscall *tSampIDFromGtaPtr)(DWORD* thisptr, int gtaptr); // SAMP's VehicleIDFromGtaPtr func
typedef int(__thiscall *tSampCreateVehicle)(DWORD* thisptr, int vehiclesmth); // SAMP's CreateVehicle func
typedef Packet*(__thiscall *tReceive)(RakClientInterface* thisptr);

tCreateCar originalCCarCtrlCreateCar = nullptr;
tSampCreateVehicle originalSampCreateVehicle = nullptr;

tReceive originalReceive = nullptr;

tSampIDFromGtaPtr SampIDFromGtaPtr = nullptr; // Get samp ID from gta struct pointer

// Queue used for getting samp ID from pointer, as we can't do it straight away
std::queue<CVehicle*> qCreatedVehiclePtrs;

// They aren't actual pointers but the addresses that real pointers point to are kept as values
DWORD dwSampInfo = NULL;
DWORD dwSampPools = NULL;
DWORD dwSampVehPool = NULL;

RakClientInterface* pRakClientInterface = nullptr;

/*
	This GTA func is used to create vehicles (excluding trains) for scripts, SAMP uses it to create vehicles
	It's called everytime a vehicle streams in, giving us it's direct CVehicle pointer
	but in order to find out what's the SAMP ID of this vehicle, we need to check it later
	using SampIDFromGtaPtr, because at this point there is no ID assigned to it.
*/
CVehicle* __cdecl hookedCCarCtrlCreateCar(DWORD model, CVector pos, BYTE isMission)
{
	CVehicle* ptr = originalCCarCtrlCreateCar(model, pos, isMission);
	if (!gInited)
		return ptr;
	//DebugPrint("Created vehicle ptr[0x%x] model %d at %f %f %f", (int)ptr, model, pos.fX, pos.fY, pos.fZ);
	qCreatedVehiclePtrs.push(ptr);
	return ptr;
}

/*
 * To find this, search for "vehicle %u was not deleted"
 * thisptr should be the pointer to CVehiclePool
 * we use fastcall because thiscall cannot be used in some compilers
 * This is where we apply custom handlings to newly created vehicles
 */

int __fastcall hookedSampCreateVehicle(DWORD *thisptr, DWORD EDX, int vehiclesmth)
{
	int ret = originalSampCreateVehicle(thisptr, vehiclesmth);
	if (!gInited)
		return ret;
	// original func calls CCarCtrl::CreateCar.. so now we should have a pointer to the created vehicle stored in qCreatedVehiclePtrs

	while (!qCreatedVehiclePtrs.empty())
	{
		CVehicle* ptr = qCreatedVehiclePtrs.front();

		int id = SampIDFromGtaPtr((DWORD*)dwSampVehPool, (int)ptr);
		DebugPrint("SAMP ID FROM 0x%x -> %d", (int)ptr, id);

		DebugPrint("vehpool 0x%x pools 0x%x ptrFromId 0x%x", (int)dwSampVehPool, (int)dwSampPools, (int)pID2PTR[id]);

		qCreatedVehiclePtrs.pop();

		HandlingMgr::InitVehicle(id, ptr->m_nModelID);

		struct tHandlingData* pHandl = HandlingMgr::GetHandlingPtrForVehicle(id, ptr->m_nModelID);

		if (pHandl == nullptr)
		{
			DebugPrint("Invalid handling pointer, id %d model %d", id, ptr->m_nModelID);
			LogError("Invalid handling pointer returned for vehicle ID %d (model %d)", id, ptr->m_nModelID);
			return ret;
		}
		else if (pHandl->m_iIndex != ptr->m_pHandlingData->m_iIndex)
		{
			DebugPrint("Handling index mismatch for vehicle ID %d (model %d, index %d != %d)", id, ptr->m_nModelID, pHandl->m_iIndex, ptr->m_pHandlingData->m_iIndex);
			LogError("Handling index mismatch for vehicle ID %d (model %d, index %d != %d)", id, ptr->m_nModelID, pHandl->m_iIndex, ptr->m_pHandlingData->m_iIndex);
		}

		DebugPrint("pHandl 0x%x index %d=%d dragmult %f=%f", (int)pHandl, pHandl->m_iIndex, ptr->m_pHandlingData->m_iIndex, pHandl->m_fDragMult, ptr->m_pHandlingData->m_fDragMult);
		ptr->m_pHandlingData = pHandl;
	}
	return ret;
}

static bool bWelcomeSent = false;
static bool bSendWelcome = false;

typedef int(__thiscall *tConnect)(RakClientInterface* thisptr, char* hostname, int port, uint16_t hostshort, int a5, int a6);
tConnect originalConnect = nullptr;

/* hook RakClient's Connect() for re-setting data */
int __fastcall hookedConnect(RakClientInterface* thisptr, DWORD EDX, char* hostname, int port, uint16_t hostshort, int a5, int a6)
{
	DebugPrint("Connect(%s, %d, %d, %d, %d)", hostname, port, hostshort, a5, a6);
	// Re-send the init packet
	bWelcomeSent = false;
	bSendWelcome = false;
	// Reset stuff to defaults
	HandlingMgr::InitializeModelDefaults();
	return originalConnect(thisptr, hostname, port, hostshort, a5, a6);
}

Packet* __fastcall hookedReceive(RakClientInterface* thisptr)
{
	Packet* pkt = originalReceive(thisptr);
	
	if (bSendWelcome && thisptr->IsConnected())
	{
		bSendWelcome = false;
		bWelcomeSent = true;
		BitStream bs;
		bs.Write((BYTE)ID_CHANDLING);
		bs.Write((BYTE)CHandlingAction::ACTION_INIT);
		bs.Write((int)rand()); // TODO replace to some version number

		thisptr->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);

		DebugPrint("CHandling Init packet sent");
	}

	if (pkt != nullptr)
	{
		if (!bWelcomeSent)
			bSendWelcome = true; // delay the sending to the next call of this function (we send our packet right after the first received one)

		if ((BYTE)pkt->data[0] == ID_CHANDLING) // handle our custom 'action' packet
		{
			if (pkt->length > sizeof(BYTE) * 2)
			{
				BitStream bs(&pkt->data[2], pkt->length - 2, false);
				Actions::Process((CHandlingAction)pkt->data[1], &bs);
			}
			return NULL;
		}
	}
	return pkt;
}

void SetupGtaHooks()
{
	originalCCarCtrlCreateCar = (tCreateCar)DetourFunction((PBYTE)FUNC_CCarCtrl_CreateCarForScript, (PBYTE)hookedCCarCtrlCreateCar);

#ifdef DUMP_DEFAULT_HANDLINGS
	HandlingDefault::originalLoadHandling = (t_ConvertHandlingToGameUnits)DetourFunction((PBYTE)0x6F5080, (PBYTE)HandlingDefault::loadHandlingHook);
#endif
}

bool SetupSampHooks()
{
	if (gInited)
	{
		dwSampInfo = *(DWORD*)(dwSampDLL + Addr.OFFSET_SampInfo);
		if ((PDWORD)dwSampInfo == nullptr)
		{
			LogError("Couldn't get OFFSET_SampInfo (0x%x)", Addr.OFFSET_SampInfo);
			return false;
		}
		DebugPrint("dwSampInfo 0x%x @ 0x%x", dwSampInfo, (dwSampDLL + Addr.OFFSET_SampInfo));
		dwSampPools = *(DWORD*)(dwSampInfo + Addr.OFFSET_SampInfo_pPools);
		if ((PDWORD)dwSampPools == nullptr)
		{
			LogError("Couldn't get OFFSET_SampInfo_pPools (0x%x)", Addr.OFFSET_SampInfo_pPools);
			return false;
		}
		DebugPrint("dwSampPools 0x%x", dwSampPools);
		dwSampVehPool = *(DWORD*)(dwSampPools + Addr.OFFSET_SampInfo_pPools_pVehiclePool);
		if ((PDWORD)dwSampVehPool == nullptr)
		{
			LogError("Couldn't get OFFSET_SampInfo_pPools_pVehiclePool (0x%x)", Addr.OFFSET_SampInfo_pPools_pVehiclePool);
			return false;
		}
		DebugPrint("dwSampVehPool 0x%x", dwSampVehPool);

		pID2PTR = reinterpret_cast<CVehicle**>((PDWORD)(dwSampVehPool + Addr.OFFSET_VehiclePool_pGtaVehicles));
		SampIDFromGtaPtr = (tSampIDFromGtaPtr)(dwSampDLL + Addr.FUNC_IDFromGtaPtr);
		DWORD dwAddr = FindPattern(dwSampDLL, "\x56\x8B\x74\x24\x08\x0F\xB7\x06\x57\x8B\xF9", "xxxxxxxxxxx"); // SampCreateVehicle
		if (!dwAddr)
		{
			LogError("Couldn't find SampCreateVehicle");
			return false;
		}
		originalSampCreateVehicle = (tSampCreateVehicle)DetourFunction((PBYTE)dwAddr, (PBYTE)hookedSampCreateVehicle);

		dwAddr = FindPattern(dwSampDLL, "\x53\x55\x56\x57\x8B\xF9\x6A\x00\x8D\x9F\x00\x00\x00\x00\x6A\x64", "xxxxxxxxxx????xx"); // RakClient->Connect()
		if (!dwAddr)
		{
			LogError("Couldn't find RakClient()->Connect()");
			return false;
		}
		originalConnect = (tConnect)DetourFunction((PBYTE)dwAddr, (PBYTE)hookedConnect);

		dwAddr = FindPattern(dwSampDLL, "\x6A\x04\x8B\xCE\xC7\x44\x24\x00\x00\x00\x00\x00", "xxxxxxx?????") - 60; // RakClient->Receive()
		if (!dwAddr)
		{
			LogError("Couldn't find RakClient->Receive()");
			return false;
		}
		originalReceive = (tReceive)DetourFunction((PBYTE)dwAddr, (PBYTE)hookedReceive);
		return true;
	}
	return false;
}



/* 
	Hook Windows' PeekMessageA() which is used in GTA:SA
	We can use this hook to check for samp.dll instead of creating a separate thread
*/

typedef BOOL(WINAPI *tPeekMessageA)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
tPeekMessageA originalPeekMessageA = nullptr;

#define UNHOOK() DetourRemove((PBYTE)originalPeekMessageA, (PBYTE)hookPeekMessageA)

BOOL WINAPI hookPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	static int startTime = GetTickCount();
	BOOL result = originalPeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);

	if (!gInited && GetTickCount() - startTime >= 35 * 1000)
	{
		DebugPrint("SAMP Module not found, exiting...");
		UNHOOK();
		return result;
	}

	if (dwSampDLL == NULL && (dwSampDLL = (DWORD)GetModuleHandleA("samp.dll")) != NULL)
	{
		DebugPrint("SAMP Module loaded at 0x%x", (DWORD)dwSampDLL);
	}
	else if (!gInited)
	{
		if (gSampVer == SAMP_000)
		{
			if ((gSampVer = DetectSampVersion(dwSampDLL)) != SAMP_000)
				Addr.Init(gSampVer);
			else
			{
				DebugPrint("Unsupported SA:MP version, aborting...");
				UNHOOK();
				return result;
			}
		}

		DWORD * info = (DWORD*)(dwSampDLL + Addr.OFFSET_SampInfo);
		if (*(DWORD**)info == nullptr)
			return result;

		gInited = true;

		HandlingDefault::Initialize();
		HandlingMgr::InitializeModelDefaults();

		DebugPrint("Setting up SAMP Hooks");
		if (!SetupSampHooks())
		{
			gInited = false;
			UNHOOK();
			return result;
		}
		else
		{
			// SAMP Hooking successful, now hook GTA functions
			SetupGtaHooks();
		}
		DebugPrint("SAMP Initialized");
		RegisterAllActionCallbacks();
		
		UNHOOK();
	}
	return result;
}

void StartHookingProcedure()
{
	originalPeekMessageA = PeekMessageA;
	originalPeekMessageA = (tPeekMessageA)DetourFunction((PBYTE)originalPeekMessageA, (PBYTE)hookPeekMessageA);
}