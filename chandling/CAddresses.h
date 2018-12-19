#pragma once
#include "sampVersions.h"
#include <Windows.h>

// GTA SA func
#define FUNC_CCarCtrl_CreateCarForScript 0x431f80

/*
	I don't use statics for this because it's a pain in the ass (they need to be initialized to be resolved)
	Just don't change any values on runtime as they should all be static
*/
class CAddresses
{
public:
	void Init(eSampVersion ver)
	{
		switch (ver)
		{
		case SAMP_037_R2:
			OFFSET_SampInfo = 0x21A100;
			OFFSET_SampInfo_pPools = 0x3C5;
			OFFSET_SampInfo_pPools_pVehiclePool = 0xC;
			OFFSET_VehiclePool_pGtaVehicles = 0x4FB4;
			FUNC_IDFromGtaPtr = 0x1B180;
			//FUNC_CVehiclePool_CreateVehicle = 0x1B670;
			break;
		case SAMP_037_R3:
			OFFSET_SampInfo = 0x26E8DC;
			OFFSET_SampInfo_pPools = 0x3DE;
			OFFSET_SampInfo_pPools_pVehiclePool = 0xC;
			OFFSET_VehiclePool_pGtaVehicles = 0x4FB4;
			FUNC_IDFromGtaPtr = 0x1E440;
			//FUNC_CVehiclePool_CreateVehicle = 0x1B670;
			break;
		case SAMP_03DL: // R1
			OFFSET_SampInfo = 0x2ACA24;
			OFFSET_SampInfo_pPools = 0x3DE;
			OFFSET_SampInfo_pPools_pVehiclePool = 0xC;
			OFFSET_VehiclePool_pGtaVehicles = 0x4FB4;
			FUNC_IDFromGtaPtr = 0x1E650;
			//FUNC_CVehiclePool_CreateVehicle = 0x1EB40;
			break;
		}
	}

	DWORD	OFFSET_SampInfo = 0;
	DWORD	OFFSET_SampInfo_pPools = 0;
	DWORD	OFFSET_SampInfo_pPools_pVehiclePool = 0;
	DWORD	OFFSET_VehiclePool_pGtaVehicles = 0;

	DWORD	FUNC_IDFromGtaPtr = 0; // can be found by searching for 'Stay within the ~r~world boundaries'
	//DWORD	FUNC_CVehiclePool_CreateVehicle = 0; // FindPattern finds it for us, but it can be found by searching for 'vehicle %u was not deleted'
};