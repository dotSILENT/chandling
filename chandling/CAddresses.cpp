#include "CAddresses.h"
#include "main.h"
#include "SimpleIni.h"
#include <cstdlib>

bool CAddresses::compareMemory(const void* ptr, const char* cmp_str, int str_len)
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

bool CAddresses::detectVersionAndLoadOffsets(uint32_t dwSAMP)
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

