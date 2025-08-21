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

bool getLongValue(CSimpleIniA &ini, const char *section, const char *key, uint32_t &out)
{
	const char* val = ini.GetValue(section, key, nullptr);
	if (!val) {
		LogError("getLongValue error: no such key \"%s\" in section \"%s\"", key, section);
		return false;
	}
	out = ini.GetLongValue(section, key);
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
			DebugPrint("Detected SA:MP version: %s", pszVersion);

			// Load offsets from an ini file
			if (!strncmp(pszVersion, "unknown", strlen(pszVersion)) ||
				!getLongValue(ini, i->pItem, "OFFSET_SampInfo", this->OFFSET_SampInfo) ||
				!getLongValue(ini, i->pItem, "OFFSET_SampInfo_pPools", this->OFFSET_SampInfo_pPools) ||
				!getLongValue(ini, i->pItem, "OFFSET_SampInfo_pPools_pVehiclePool", this->OFFSET_SampInfo_pPools_pVehiclePool) ||
				!getLongValue(ini, i->pItem, "OFFSET_VehiclePool_pGtaVehicles", this->OFFSET_VehiclePool_pGtaVehicles)
			) {
				LogError("Cannot load one or more ini value(s)");
				return false;
			}

			this->loaded = true;
			return true;
		}
	}

	LogError("No version matching, 0xBABE: %X%X%X%X",
		*(uint8_t*)(dwSAMP + 0xBABE + 0),
		*(uint8_t*)(dwSAMP + 0xBABE + 1),
		*(uint8_t*)(dwSAMP + 0xBABE + 2),
		*(uint8_t*)(dwSAMP + 0xBABE + 3)
	);
	return false;
}

bool CAddresses::detectVersionAndLoadOffsets(uint32_t dwSAMP)
{
	if (!CAddresses::parseFile(dwSAMP))
	{
		// Fetch the newest offsets file from GitHub repo and try again
#define OFFSETS_URL "https://github.com/" CHANDLING_GITHUB_REPO "/raw/master/" CHANDLING_OFFSETS_FILE

		DebugPrint("Fetching "  CHANDLING_OFFSETS_FILE " from " OFFSETS_URL);
		CURL *curl;
		CURLcode res;
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		if (!curl)
		{
			curl_easy_cleanup(curl);
			return false;
		}

#define TMP_FILE CHANDLING_OFFSETS_FILE ".tmp"

		FILE *fp = fopen(TMP_FILE, "wb");
		if (!fp)
		{
			LogError("Couldn't open " TMP_FILE " for writing");
			curl_easy_cleanup(curl);
			return false;
		}
		curl_easy_setopt(curl, CURLOPT_URL, OFFSETS_URL);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_CRLF, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 1000);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		if (res == CURLE_OK) {
			long httpCode = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			if (httpCode >= 200 && httpCode < 400) {
				DebugPrint("Saving new " CHANDLING_OFFSETS_FILE " file");
				MoveFileExA(TMP_FILE, CHANDLING_OFFSETS_FILE, MOVEFILE_REPLACE_EXISTING);
			} else {
				DebugPrint("Failed to fetch " CHANDLING_OFFSETS_FILE ", httpCode: %d", httpCode);
			}
		}

		DeleteFileA(TMP_FILE);

		if (CAddresses::parseFile(dwSAMP))
			return true;
	}
	else return true;

#undef OFFSETSURL
	return false;
}

