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
		{
			OFFSET_SampInfo = 0x21A100;
			OFFSET_SampInfo_pPools = 0x3C5;
			OFFSET_SampInfo_pPools_pVehiclePool = 0xC;
			OFFSET_SampInfo_pPools_pVehiclePool_pGtaVehicles = 0x4FB4;
			OFFSET_SampInfo_Hostname = 0x11D;
			FUNC_IDFromGtaPtr = 0x1b180;
			FUNC_CVehiclePool_CreateVehicle = 0x1B670;
		}
		}
	}

	DWORD	OFFSET_SampInfo = 0;
	DWORD	OFFSET_SampInfo_pPools = 0;
	DWORD	OFFSET_SampInfo_pPools_pVehiclePool = 0;
	DWORD	OFFSET_SampInfo_pPools_pVehiclePool_pGtaVehicles = 0;
	DWORD	OFFSET_SampInfo_Hostname = 0;

	DWORD	FUNC_IDFromGtaPtr = 0;
	DWORD	FUNC_CVehiclePool_CreateVehicle = 0;
};