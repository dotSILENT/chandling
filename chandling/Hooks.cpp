#include "Hooks.h"
#include "main.h"
#include "raknet/RakClientInterface.h"
#include "game/game.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "detours.h"
#pragma comment(lib, "detours.lib")

#include "hook_utils.h"
#include "PacketEnum.h"
#include "Actions.h"
#include "ActionCallbacks.h"
#include "HandlingDefault.h"
#include "HandlingManager.h"
#include "UpdateChecker.h"
#include "CAddresses.h"

CAddresses Addr;

#include <queue>
#include <unordered_map>


const char szModelHandlingNames[MAX_VEHICLE_MODELS][12] =
{
	{ "LANDSTAL" },
	{ "BRAVURA" },
	{ "BUFFALO" },
	{ "LINERUN" },
	{ "PEREN" },
	{ "SENTINEL" },
	{ "DUMPER" },
	{ "FIRETRUK" },
	{ "TRASH" },
	{ "STRETCH" },
	{ "MANANA" },
	{ "INFERNUS" },
	{ "VOODOO" },
	{ "PONY" },
	{ "MULE" },
	{ "CHEETAH" },
	{ "AMBULAN" },
	{ "LEVIATHN" },
	{ "MOONBEAM" },
	{ "ESPERANT" },
	{ "TAXI" },
	{ "WASHING" },
	{ "BOBCAT" },
	{ "MRWHOOP" },
	{ "BFINJECT" },
	{ "HUNTER" },
	{ "PREMIER" },
	{ "ENFORCER" },
	{ "SECURICA" },
	{ "BANSHEE" },
	{ "PREDATOR" },
	{ "BUS" },
	{ "RHINO" },
	{ "BARRACKS" },
	{ "HOTKNIFE" },
	{ "ARTICT1" },
	{ "PREVION" },
	{ "COACH" },
	{ "CABBIE" },
	{ "STALLION" },
	{ "RUMPO" },
	{ "RCBANDIT" },
	{ "ROMERO" },
	{ "PACKER" },
	{ "MONSTER" },
	{ "ADMIRAL" },
	{ "SQUALO" },
	{ "SEASPAR" },
	{ "MOPED" },
	{ "TRAM" },
	{ "ARTICT2" },
	{ "TURISMO" },
	{ "SPEEDER" },
	{ "REEFER" },
	{ "TROPIC" },
	{ "FLATBED" },
	{ "YANKEE" },
	{ "GOLFCART" },
	{ "SOLAIR" },
	{ "TOPFUN" },
	{ "SEAPLANE" },
	{ "BIKE" },
	{ "MOPED" },
	{ "FREEWAY" },
	{ "RCBARON" },
	{ "RCRAIDER" },
	{ "GLENDALE" },
	{ "OCEANIC" },
	{ "DIRTBIKE" },
	{ "SPARROW" },
	{ "PATRIOT" },
	{ "QUADBIKE" },
	{ "COASTGRD" },
	{ "DINGHY" },
	{ "HERMES" },
	{ "SABRE" },
	{ "RUSTLER" },
	{ "ZR350" },
	{ "WALTON" },
	{ "REGINA" },
	{ "COMET" },
	{ "BMX" },
	{ "BURRITO" },
	{ "CAMPER" },
	{ "MARQUIS" },
	{ "BAGGAGE" },
	{ "DOZER" },
	{ "MAVERICK" },
	{ "COASTMAV" },
	{ "RANCHER" },
	{ "FBIRANCH" },
	{ "VIRGO" },
	{ "GREENWOO" },
	{ "CUPBOAT" },
	{ "HOTRING" },
	{ "SANDKING" },
	{ "BLISTAC" },
	{ "POLMAV" },
	{ "BOXVILLE" },
	{ "BENSON" },
	{ "MESA" },
	{ "RCGOBLIN" },
	{ "HOTRING" },
	{ "HOTRING" },
	{ "BLOODRA" },
	{ "RANCHER" },
	{ "SUPERGT" },
	{ "ELEGANT" },
	{ "JOURNEY" },
	{ "CHOPPERB" },
	{ "MTB" },
	{ "BEAGLE" },
	{ "CROPDUST" },
	{ "STUNT" },
	{ "PETROL" },
	{ "RDTRAIN" },
	{ "NEBULA" },
	{ "MAJESTIC" },
	{ "BUCCANEE" },
	{ "SHAMAL" },
	{ "HYDRA" },
	{ "FCR900" },
	{ "NRG500" },
	{ "HPV1000" },
	{ "CEMENT" },
	{ "TOWTRUCK" },
	{ "FORTUNE" },
	{ "CADRONA" },
	{ "FBITRUCK" },
	{ "WILLARD" },
	{ "FORKLIFT" },
	{ "TRACTOR" },
	{ "COMBINE" },
	{ "FELTZER" },
	{ "REMINGTN" },
	{ "SLAMVAN" },
	{ "BLADE" },
	{ "FREIGHT" },
	{ "STREAK" },
	{ "VORTEX" },
	{ "VINCENT" },
	{ "BULLET" },
	{ "CLOVER" },
	{ "SADLER" },
	{ "FIRETRUK" },
	{ "HUSTLER" },
	{ "INTRUDER" },
	{ "PRIMO" },
	{ "CARGOBOB" },
	{ "TAMPA" },
	{ "SUNRISE" },
	{ "MERIT" },
	{ "UTILITY" },
	{ "NEVADA" },
	{ "YOSEMITE" },
	{ "WINDSOR" },
	{ "MTRUCK_A" },
	{ "MTRUCK_B" },
	{ "URANUS" },
	{ "JESTER" },
	{ "SULTAN" },
	{ "STRATUM" },
	{ "ELEGY" },
	{ "RAINDANC" },
	{ "RCTIGER" },
	{ "FLASH" },
	{ "TAHOMA" },
	{ "SAVANNA" },
	{ "BANDITO" },
	{ "FREIFLAT" },
	{ "CSTREAK" },
	{ "KART" },
	{ "MOWER" },
	{ "DUNE" },
	{ "SWEEPER" },
	{ "BROADWAY" },
	{ "TORNADO" },
	{ "AT400" },
	{ "DFT30" },
	{ "HUNTLEY" },
	{ "STAFFORD" },
	{ "BF400" },
	{ "NEWSVAN" },
	{ "TUG" },
	{ "PETROTR" },
	{ "EMPEROR" },
	{ "WAYFARER" },
	{ "EUROS" },
	{ "HOTDOG" },
	{ "CLUB" },
	{ "FREIFLAT" },
	{ "ARTICT3" },
	{ "ANDROM" },
	{ "DODO" },
	{ "RCCAM" },
	{ "LAUNCH" },
	{ "POLICE_LA" },
	{ "POLICE_SF" },
	{ "POLICE_VG" },
	{ "POLRANGER" },
	{ "PICADOR" },
	{ "SWATVAN" },
	{ "ALPHA" },
	{ "PHOENIX" },
	{ "GLENDALE" },
	{ "SADLER" },
	{ "BAGBOXA" },
	{ "BAGBOXB" },
	{ "STAIRS" },
	{ "BOXBURG" },
	{ "FARM_TR1" },
	{ "UTIL_TR1" }
};

#pragma pack(push, 1)
struct stVehicleCreationInfo
{
	WORD id;
	WORD model;
	float x;
	float y;
	float z;
	float angle;
};
#pragma pack(pop)
struct stVehicleCreationInfo *pCreatedVehicleInfo = nullptr; // used for swapping handling data before the actual vehicle creation (hookedCCarCtrlCreateCar)

/** Hook Typedefs */
typedef CVehicle*(__cdecl *tCreateCar)(DWORD model, CVector pos, BYTE mission); // CCarCtrlCreateCar
//typedef int(__thiscall *tSampIDFromGtaPtr)(DWORD* thisptr, int gtaptr); // SAMP's VehicleIDFromGtaPtr func
typedef int(__thiscall *tSampCreateVehicle)(DWORD* thisptr, struct stVehicleCreationInfo &vehinfo); // SAMP's CreateVehicle func
typedef Packet*(__thiscall *tReceive)(RakClientInterface* thisptr);

tCreateCar originalCCarCtrlCreateCar = nullptr;
//tSampIDFromGtaPtr SampIDFromGtaPtr = nullptr; // Get samp ID from gta struct pointer
tSampCreateVehicle originalSampCreateVehicle = nullptr;
tReceive originalReceive = nullptr;

// They aren't actual pointers but the addresses that real pointers point to are kept as values
DWORD dwSampInfo = NULL;
DWORD dwSampPools = NULL;
DWORD dwSampVehPool = NULL;

RakClientInterface* pRakClientInterface = nullptr;


/*
	This GTA func is used to create vehicles (excluding trains) for scripts, SAMP uses it to create vehicles
	It's called everytime a vehicle streams in, giving us it's direct CVehicle pointer
*/

typedef int(__thiscall *t_GetHandlingId)(void* thisptr, const char *vehname);

CVehicle* __cdecl hookedCCarCtrlCreateCar(DWORD model, CVector pos, BYTE isMission)
{
	/*	
		Overwrite the default handling values now so the vehicle copies them during creation
		 bring back the original ones right after
	*/
	struct tHandlingData orgHandling;
	struct tHandlingData *pGtaHandling = nullptr;
	int handlingIndex = -1;
	bool swapped = false;

	if (gInited && pCreatedVehicleInfo != nullptr && model == pCreatedVehicleInfo->model)
	{
		handlingIndex = ((t_GetHandlingId)0x6F4FD0)(0x0, szModelHandlingNames[VEHICLE_MODEL_INDEX(model)]);
		struct tHandlingData* pCustomHandling = HandlingMgr::GetHandlingPtrForVehicle(pCreatedVehicleInfo->id, model);

		if (handlingIndex >= 0  && handlingIndex < 210 && pCustomHandling != nullptr) // t_GetHandlingId returns 210 for invalid model names
		{
			pGtaHandling = (struct tHandlingData*)(0xC2B9DC + (handlingIndex * sizeof(struct tHandlingData)));
			//DebugPrint("swapping handling (index %d) for vehicleid %d, pCustomHandl 0x%x", handlingIndex, pCreatedVehicleInfo->id, (int)pCustomHandling);
			memcpy(&orgHandling, pGtaHandling, sizeof(struct tHandlingData));
			memcpy(pGtaHandling, pCustomHandling, sizeof(struct tHandlingData));
			swapped = true;
		}
	}

	CVehicle* ptr = originalCCarCtrlCreateCar(model, pos, isMission);
	if (!gInited)
		return ptr;

	// Swap back the original handling
	if (swapped)
		memcpy(pGtaHandling, &orgHandling, sizeof(struct tHandlingData));

	DebugPrint("Created vehicle ptr[0x%x] model %d at %f %f %f", (int)ptr, model, pos.fX, pos.fY, pos.fZ);
	return ptr;
}

/*
 * To find this, search for "vehicle %u was not deleted"
 * thisptr should be the pointer to CVehiclePool
 * we use fastcall because thiscall cannot be used in some compilers
 * This is where we apply custom handlings to newly created vehicles
 */

int __fastcall hookedSampCreateVehicle(DWORD *thisptr, DWORD EDX, struct stVehicleCreationInfo &vehinfo)
{
	pCreatedVehicleInfo = &vehinfo;
	int ret = originalSampCreateVehicle(thisptr, vehinfo); // this calls CCarCtrlCreateCarForScript
	pCreatedVehicleInfo = nullptr;
	if (!gInited)
		return ret;

	//int id = SampIDFromGtaPtr((DWORD*)dwSampVehPool, (int)ptr);
	CVehicle* ptr = pID2PTR[vehinfo.id]; //qCreatedVehiclePtrs.front();
	if (!ptr)
		return ret;

	DebugPrint("CreateVehicle id %d model %d pointer 0x%x", vehinfo.id, vehinfo.model, (int)ptr);

	HandlingMgr::InitVehicle(vehinfo.id, vehinfo.model);

	struct tHandlingData* pHandl = HandlingMgr::GetHandlingPtrForVehicle(vehinfo.id, vehinfo.model);

	if (pHandl == nullptr)
	{
		LogError("Invalid handling pointer returned for vehicle ID %d (model %d = %d)", vehinfo.id, vehinfo.model, ptr->m_nModelID);
		return ret;
	}
	else if (pHandl->m_iIndex != ptr->m_pHandlingData->m_iIndex)
	{
		LogError("Handling index mismatch for vehicle ID %d (model %d, index %d != %d)", vehinfo.id, ptr->m_nModelID, pHandl->m_iIndex, ptr->m_pHandlingData->m_iIndex);
		return ret;
	}

	DebugPrint("pHandl 0x%x index %d=%d dragmult %f=%f", (int)pHandl, pHandl->m_iIndex, ptr->m_pHandlingData->m_iIndex, pHandl->m_fDragMult, ptr->m_pHandlingData->m_fDragMult);
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
		//SampIDFromGtaPtr = (tSampIDFromGtaPtr)(dwSampDLL + Addr.FUNC_IDFromGtaPtr);
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
		DebugPrint("SAMP Module loaded at 0x%x, checking for updates", (DWORD)dwSampDLL);
		// Check for updates
		if (UpdateChecker::CheckForUpdate())
		{
			startTime = GetTickCount(); // reset the waiting time if an update was found
			SetFocus(hWnd);
		}
	}
	else if (!gInited)
	{
		if (!Addr.isLoaded() && !Addr.detectVersionAndLoadOffsets(dwSampDLL))
		{
			LogError("Couldn't detect version or load the offsets file, aborting...");
			UNHOOK();
			return result;
		}

		if (*(DWORD**)(dwSampDLL + Addr.OFFSET_SampInfo) == nullptr)
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