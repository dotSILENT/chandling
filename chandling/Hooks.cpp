#include "Hooks.h"
#include "main.h"
#include <queue>
#include <unordered_map>
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "HandlingManager.h"

CHandlingManager cHandlingMgr;

tCreateCar originalCCarCtrlCreateCar = nullptr;
tSampCreateVehicle originalSampCreateVehicle = nullptr;

tSampIDFromGtaPtr SampIDFromGtaPtr = nullptr; // Get samp ID from gta struct pointer
tSampGetVehicleStruct SampGetVehicleStruct = nullptr; // Get pointer to GTA vehicle struct from internal ID

// Queue used for getting samp ID from pointer, as we can't do it straight away
std::queue<CVehicle*> qCreatedVehiclePtrs;

// Some pointers for easier operations
DWORD* pSampInfo = nullptr;
DWORD* pSampPools = nullptr;
DWORD* pSampVehPool = nullptr;
CVehicle** IdToPtrTranslation = nullptr;

// This GTA func is used to create vehicles (excluding trains) for scripts, SAMP uses it to create vehicles
// It's called everytime a vehicle streams in, giving us it's direct CVehicle pointer
// but in order to find out what's the SAMP ID of this vehicle, we need to check it later
// using SampIDFromGtaPtr, because at this point there is no ID assigned to it.
CVehicle* __cdecl hookedCCarCtrlCreateCar(DWORD model, CVector pos, BYTE isMission)
{
	CVehicle* ptr = originalCCarCtrlCreateCar(model, pos, isMission);
	//DebugPrint("Created vehicle ptr[0x%x] model %d at %f %f %f\n", (int)ptr, model, pos.fX, pos.fY, pos.fZ);
	qCreatedVehiclePtrs.push(ptr);
	return ptr;
}

// To find this, search for "vehicle %u was not deleted"
// thisptr should be the pointer to CVehiclePool
// we use fastcall because thiscall cannot be used in some compilers
int __fastcall hookedSampCreateVehicle(DWORD *thisptr, DWORD EDX, int vehiclesmth)
{
	int ret = originalSampCreateVehicle(thisptr, vehiclesmth);
	// original func calls CCarCtrl::CreateCar.. so now we should have a pointer to the created vehicle stored in qCreatedVehiclePtrs

	while (!qCreatedVehiclePtrs.empty())
	{
		CVehicle* ptr = qCreatedVehiclePtrs.front();

		int id = SampIDFromGtaPtr(pSampVehPool, (int)ptr);
		DebugPrint("SAMP ID FROM 0x%x -> %d\n", (int)ptr, id);

		DebugPrint("vehpool 0x%x pools 0x%x ptrFromId 0x%x\n", (int)pSampVehPool, (int)pSampPools, (int)IdToPtrTranslation[id]);

		qCreatedVehiclePtrs.pop();
	}
	return ret;
}

void SetupGtaHooks()
{
	originalCCarCtrlCreateCar = (tCreateCar)DetourFunction((PBYTE)FUNC_CCarCtrl_CreateCarForScript, (PBYTE)hookedCCarCtrlCreateCar);
}

bool SetupSampHooks()
{
	if (gInited)
	{
		pSampInfo = (DWORD*)(dwSampDLL + Addr.OFFSET_SampInfo);
		if (!pSampInfo)
			return false;
		pSampPools = *(DWORD**)(*pSampInfo + Addr.OFFSET_SampInfo_pPools);
		if (!pSampPools)
			return false;
		pSampVehPool = *(DWORD**)((PBYTE)pSampPools + Addr.OFFSET_SampInfo_pPools_pVehiclePool);
		if (!pSampVehPool)
			return false;

		IdToPtrTranslation = reinterpret_cast<CVehicle**>(((PBYTE)pSampVehPool + Addr.OFFSET_SampInfo_pPools_pVehiclePool_pGtaVehicles));
		SampIDFromGtaPtr = (tSampIDFromGtaPtr)(dwSampDLL + Addr.FUNC_IDFromGtaPtr);
		originalSampCreateVehicle = (tSampCreateVehicle)DetourFunction((PBYTE)(dwSampDLL + Addr.FUNC_CVehiclePool_CreateVehicle), (PBYTE)hookedSampCreateVehicle);
		return true;
	}
	return false;
}