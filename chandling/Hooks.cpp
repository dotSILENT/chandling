#include "Hooks.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "raknet\RakClientInterface.h"
#include "hook_utils.h"
#include "PacketEnum.h"
#include "Actions.h"
#include "HandlingDefault.h"
#include "HandlingManager.h"

#include <queue>
#include <unordered_map>

tCreateCar originalCCarCtrlCreateCar = nullptr;
tSampCreateVehicle originalSampCreateVehicle = nullptr;

tReceive originalReceive = nullptr;

tSampIDFromGtaPtr SampIDFromGtaPtr = nullptr; // Get samp ID from gta struct pointer
tSampGetVehicleStruct SampGetVehicleStruct = nullptr; // Get pointer to GTA vehicle struct from internal ID

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

		if (pHandl == nullptr || pHandl->m_iIndex != ptr->m_pHandlingData->m_iIndex)
		{
			DebugPrint("Invalid handling pointer, id %d model %d", id, ptr->m_nModelID);
			return ret;
		}
		DebugPrint("index %d=%d dragmult %f=%f", pHandl->m_iIndex, ptr->m_pHandlingData->m_iIndex, pHandl->m_fDragMult, ptr->m_pHandlingData->m_fDragMult);
		ptr->m_pHandlingData = pHandl;
	}
	return ret;
}

static bool bWelcomeSent = false;
static bool bSendWelcome = false;
Packet* __fastcall hookedReceive(RakClientInterface* thisptr)
{
	Packet* pkt = originalReceive(thisptr);
	
	if (bSendWelcome)
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
			return false;
		DebugPrint("dwSampInfo 0x%x @ 0x%x", dwSampInfo, (dwSampDLL + Addr.OFFSET_SampInfo));
		dwSampPools = *(DWORD*)(dwSampInfo + Addr.OFFSET_SampInfo_pPools);
		if ((PDWORD)dwSampPools == nullptr)
			return false;
		DebugPrint("dwSampPools 0x%x", dwSampPools);
		dwSampVehPool = *(DWORD*)(dwSampPools + Addr.OFFSET_SampInfo_pPools_pVehiclePool);
		if ((PDWORD)dwSampVehPool == nullptr)
			return false;
		DebugPrint("dwSampVehPool 0x%x", dwSampVehPool);

		pID2PTR = reinterpret_cast<CVehicle**>((PDWORD)(dwSampVehPool + Addr.OFFSET_SampInfo_pPools_pVehiclePool_pGtaVehicles));
		SampIDFromGtaPtr = (tSampIDFromGtaPtr)(dwSampDLL + Addr.FUNC_IDFromGtaPtr);
		//originalSampCreateVehicle = (tSampCreateVehicle)DetourFunction((PBYTE)(dwSampDLL + Addr.FUNC_CVehiclePool_CreateVehicle), (PBYTE)hookedSampCreateVehicle);
		DWORD dwSampCrtVeh = FindPattern(dwSampDLL, "\x56\x8B\x74\x24\x08\x0F\xB7\x06\x57\x8B\xF9", "xxxxxxxxxxx");
		if (!dwSampCrtVeh)
			return false;
		originalSampCreateVehicle = (tSampCreateVehicle)DetourFunction((PBYTE)dwSampCrtVeh, (PBYTE)hookedSampCreateVehicle);
		originalReceive = (tReceive)DetourFunction((PBYTE)(FindPattern(dwSampDLL, "\x6A\x04\x8B\xCE\xC7\x44\x24\x00\x00\x00\x00\x00", "xxxxxxx?????") - 60), (PBYTE)hookedReceive);
		return true;
	}
	return false;
}