#pragma once
#include <cstdint>

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
	static bool compareMemory(const void* ptr, const char* cmp_str, int str_len);

	bool parseFile(uint32_t dwSAMP);

	bool loaded = false;

public:

	bool	isLoaded() { return this->loaded; }

	bool	detectVersionAndLoadOffsets(uint32_t dwSAMP);

	uint32_t	OFFSET_SampInfo = 0;
	uint32_t	OFFSET_SampInfo_pPools = 0;
	uint32_t	OFFSET_SampInfo_pPools_pVehiclePool = 0;
	uint32_t	OFFSET_VehiclePool_pGtaVehicles = 0;

	//uint32_t	FUNC_IDFromGtaPtr = 0; // can be found by searching for 'Stay within the ~r~world boundaries'
	//DWORD	FUNC_CVehiclePool_CreateVehicle = 0; // FindPattern finds it for us, but it can be found by searching for 'vehicle %u was not deleted'
};