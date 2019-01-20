#include "CAddresses.h"
#include "main.h"
#include "SimpleIni.h"
#include <cstdlib>
#include <cstdio>

#pragma warning(disable : 4996) // fopen may be unsafe

#include "curl/curl.h"
#pragma comment(lib, "curl/lib/libcurl.lib")

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

bool CAddresses::parseFile(uint32_t dwSAMP)
{
	CSimpleIniA ini;
	if (ini.LoadFile(CHANDLING_OFFSETS_FILE) < 0)
		return false;

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	for (CSimpleIniA::TNamesDepend::const_iterator i = sections.begin(); i != sections.end(); i++)
	{
		if (CAddresses::compareMemory((void*)(dwSAMP + 0xBABE), i->pItem, strlen(i->pItem)))
		{
			const char* pszVersion = ini.GetValue(i->pItem, "name", "unknown");
			DebugPrint("Detected SA:MP version %s", pszVersion);

			// Load offsets from an ini file
			this->OFFSET_SampInfo = ini.GetLongValue(i->pItem, "OFFSET_SampInfo");
			this->OFFSET_SampInfo_pPools = ini.GetLongValue(i->pItem, "OFFSET_SampInfo_pPools");
			this->OFFSET_SampInfo_pPools_pVehiclePool = ini.GetLongValue(i->pItem, "OFFSET_SampInfo_pPools_pVehiclePool");
			this->OFFSET_VehiclePool_pGtaVehicles = ini.GetLongValue(i->pItem, "OFFSET_VehiclePool_pGtaVehicles");
			this->FUNC_IDFromGtaPtr = ini.GetLongValue(i->pItem, "FUNC_IDFromGtaPtr");

			/*DebugPrint("sampinfo 0x%x", this->OFFSET_SampInfo);
			DebugPrint("pools 0x%x", this->OFFSET_SampInfo_pPools);
			DebugPrint("vehiclepool 0x%x", this->OFFSET_SampInfo_pPools_pVehiclePool);
			DebugPrint("gtavehicles 0x%x", this->OFFSET_VehiclePool_pGtaVehicles);
			DebugPrint("idfromgtaptr 0x%x", this->FUNC_IDFromGtaPtr);*/

			if (!strncmp(pszVersion, "unknown", strlen(pszVersion))
				|| this->OFFSET_SampInfo == 0
				|| this->OFFSET_SampInfo_pPools == 0
				|| this->OFFSET_SampInfo_pPools_pVehiclePool == 0
				|| this->OFFSET_VehiclePool_pGtaVehicles == 0
				|| this->FUNC_IDFromGtaPtr == 0)
			{
				return false;
			}

			this->loaded = true;
			return true;
		}
	}

	LogError("No version matching, 0xBABE: %X%X", *(uint8_t*)(dwSAMP + 0xBABE), *(uint8_t*)(dwSAMP + 0xBABE + 1));
	return false;
}

bool CAddresses::detectVersionAndLoadOffsets(uint32_t dwSAMP)
{
	if (!CAddresses::parseFile(dwSAMP))
	{
		// Fetch the newest offsets file from GitHub repo and try again
#define OFFSETSURL "https://github.com/" CHANDLING_GITHUB_REPO "/raw/master/" CHANDLING_OFFSETS_FILE

		DebugPrint("Fetching "  CHANDLING_OFFSETS_FILE " from " OFFSETSURL);
		CURL *curl;
		CURLcode res;
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		if (!curl)
		{
			curl_easy_cleanup(curl);
			return false;
		}

		FILE *fp = fopen(CHANDLING_OFFSETS_FILE, "wb");
		if (!fp)
		{
			LogError("Couldn't open " CHANDLING_OFFSETS_FILE " for writing");
			curl_easy_cleanup(curl);
			return false;
		}
		curl_easy_setopt(curl, CURLOPT_URL, OFFSETSURL);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_CRLF, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		if (CAddresses::parseFile(dwSAMP))
			return true;
	}
	else return true;

#undef OFFSETSURL
	return false;
}

