#include "Hooks.h"
#include "main.h"
#include <queue>
#include "detours.h"
#pragma comment(lib, "detours.lib")

tCreateCar originalCCarCtrlCreateCar = nullptr;
tSampCreateVehicle originalSampCreateVehicle = nullptr;

tSampIDFromGtaPtr SampIDFromGtaPtr = nullptr; // Get samp ID from gta struct pointer
tSampGtaIDFromID SampGtaIDFromID = nullptr; // Get internal GTA vehicle handle from SAMP vehicle ID
tSampGetVehicleStruct SampGetVehicleStruct = nullptr; // Get pointer to GTA vehicle struct from internal ID

// Queue used for getting samp ID from pointer, as we can't do it straight away
std::queue<CVehicle*> qCreatedVehiclePtrs;

// Some pointers for easier operations
DWORD* Pools = nullptr;
DWORD* VehPool = nullptr;

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

/*CVehicle* __stdcall GetVehicleStruct(int ID)
{
	CVehicle* ret = SampGetVehicleStruct(ID);
	DebugPrint("GetVehicleStruct(%d) = 0x%x\n", ID, (int)ret);
	return ret;
}*/

/* SAMP internal Vehicle Pool, we only need it for m_pGTAVehicles which is an array used for translating the vehicle id to gta PTR*/
struct pVehPool
{
	char pad[0x4FB4];
	CVehicle* m_pGTAVehicles[MAX_VEHICLES+1];
};
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

		int id = SampIDFromGtaPtr(*(DWORD**)(*(char**)(*(char**)(dwSampDLL + Addr.OFFSET_SampInfo) + Addr.OFFSET_SampInfo_pPools) + Addr.OFFSET_SampInfo_pPools_pVehiclePool), (int)ptr);
		DebugPrint("SAMP ID FROM 0x%x -> %d\n", (int)ptr, id);
		int handle = SampGtaIDFromID(*(DWORD**)(*(char**)(*(char**)(dwSampDLL + Addr.OFFSET_SampInfo) + Addr.OFFSET_SampInfo_pPools) + Addr.OFFSET_SampInfo_pPools_pVehiclePool), id);

		struct pVehPool* ppool = *(pVehPool**)(*(char**)(*(char**)(dwSampDLL + Addr.OFFSET_SampInfo) + Addr.OFFSET_SampInfo_pPools) + Addr.OFFSET_SampInfo_pPools_pVehiclePool);
		DebugPrint("ppool 0x%x VehPool 0x%x\n", (int)ppool, (int)VehPool);

		if (handle != 0xFFFF)
		{
			CVehicle *te = SampGetVehicleStruct((DWORD)handle);
			if (!te)
				DebugPrint("bad ptr\n");
			DebugPrint("Pointer frm ID: 0x%x\n", (int)te);
		}
		qCreatedVehiclePtrs.pop();
	}
	return ret;
}

void SetupGtaHooks()
{
	originalCCarCtrlCreateCar = (tCreateCar)DetourFunction((PBYTE)FUNC_CCarCtrl_CreateCarForScript, (PBYTE)hookedCCarCtrlCreateCar);
}

void SetupSampHooks()
{
	if (gInited)
	{
		SampIDFromGtaPtr = (tSampIDFromGtaPtr)(dwSampDLL + Addr.FUNC_IDFromGtaPtr);
		SampGtaIDFromID = (tSampGtaIDFromID)(dwSampDLL + Addr.FUNC_GtaPtrFromID);
		SampGetVehicleStruct = (tSampGetVehicleStruct)(dwSampDLL + Addr.FUNC_GetVehicleStruct); //(tSampGetVehicleStruct)DetourFunction((PBYTE)(dwSampDLL + Addr.FUNC_GetVehicleStruct), (PBYTE)GetVehicleStruct);
		originalSampCreateVehicle = (tSampCreateVehicle)DetourFunction((PBYTE)(dwSampDLL + Addr.FUNC_CVehiclePool_CreateVehicle), (PBYTE)hookedSampCreateVehicle);

		//Pools = *(DWORD**)(*(char**)(dwSampDLL + Addr.OFFSET_SampInfo) + Addr.OFFSET_SampInfo_pPools);
		//VehPool = *(DWORD**)(*(char*)Pools + Addr.OFFSET_SampInfo_pPools_pVehiclePool); // fix this
	}
}