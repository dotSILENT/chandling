#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "SimpleIni.h"

// GTA SA func
#define FUNC_CCarCtrl_CreateCarForScript 0x431f80

#define CHANDLING_OFFSETS_FILE "chandling_offsets.ini"

/*
 * I don't use statics for this because it's a pain in the ass (they need to be initialized to be resolved)
 * Just don't change any values on runtime as they should all be static
 */
class CAddresses
{
private:
	/* str_len = strlen() of cmp_str, needs to be even */
	static bool compareMemory(const void* ptr, const char* cmp_str, int str_len)
	{
		char cmpbyte[3] = { 0 };
		for (int i = 0; i < str_len; i += 2)
		{
			if (i + 1 >= str_len)
				return false;
			cmpbyte[0] = cmp_str[i];
			cmpbyte[1] = cmp_str[i + 1];

			if (*(uint8_t*)((uint8_t*)ptr + (i / 2)) != (uint8_t)strtoul(cmpbyte, NULL, 16))
				return false;
		}
		return true;
	}

	bool loaded = false;

public:

	bool	isLoaded() { return this->loaded; }

	bool	detectVersionAndLoadOffsets(DWORD dwSAMP)
	{
		CSimpleIniA ini;
		if (ini.LoadFile(CHANDLING_OFFSETS_FILE) < 0)
		{
			LogError("Couldn't open " CHANDLING_OFFSETS_FILE);
			return false;
		}

		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);

		for (CSimpleIniA::TNamesDepend::const_iterator i = sections.begin(); i != sections.end(); i++)
		{
			if (CAddresses::compareMemory((void*)(dwSAMP + 0xBABE), i->pItem, strlen(i->pItem)))
			{
				DebugPrint("Detected SA:MP version %s", ini.GetValue(i->pItem, "name", "unknown"));

				// Load offsets from an ini file
				this->OFFSET_SampInfo = ini.GetLongValue(i->pItem, "OFFSET_SampInfo");
				this->OFFSET_SampInfo_pPools = ini.GetLongValue(i->pItem, "OFFSET_SampInfo_pPools");
				this->OFFSET_SampInfo_pPools_pVehiclePool = ini.GetLongValue(i->pItem, "OFFSET_SampInfo_pPools_pVehiclePool");
				this->OFFSET_VehiclePool_pGtaVehicles = ini.GetLongValue(i->pItem, "OFFSET_VehiclePool_pGtaVehicles");
				this->FUNC_IDFromGtaPtr = ini.GetLongValue(i->pItem, "FUNC_IDFromGtaPtr");

				/*DebugPrint("sampinfo 0x%x = %d", this->OFFSET_SampInfo, this->OFFSET_SampInfo);
				DebugPrint("pools 0x%x", this->OFFSET_SampInfo_pPools);
				DebugPrint("vehiclepool 0x%x", this->OFFSET_SampInfo_pPools_pVehiclePool);
				DebugPrint("gtavehicles 0x%x", this->OFFSET_VehiclePool_pGtaVehicles);
				DebugPrint("idfromgtaptr 0x%x", this->FUNC_IDFromGtaPtr);*/
				this->loaded = true;
				return true;
			}
		}
		
		LogError("No version matching, 0xBABE: %X%X", *(uint8_t*)(dwSAMP + 0xBABE), *(uint8_t*)(dwSAMP + 0xBABE + 1));
		return false;
	}

	DWORD	OFFSET_SampInfo = 0;
	DWORD	OFFSET_SampInfo_pPools = 0;
	DWORD	OFFSET_SampInfo_pPools_pVehiclePool = 0;
	DWORD	OFFSET_VehiclePool_pGtaVehicles = 0;

	DWORD	FUNC_IDFromGtaPtr = 0; // can be found by searching for 'Stay within the ~r~world boundaries'
	//DWORD	FUNC_CVehiclePool_CreateVehicle = 0; // FindPattern finds it for us, but it can be found by searching for 'vehicle %u was not deleted'
};