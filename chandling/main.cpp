#define PRINT_DEFAULT_HANDLING

#include "main.h"
#include "sampVersions.h"
#include "Hooks.h"

// globals
DWORD dwSampDLL = NULL;
eSampVersion gSampVer = SAMP_000;
CAddresses Addr;
bool gInited = false;
CVehicle** pID2PTR = nullptr; // CVehicle* m_pGTAVehicles[] array in samp's vehicle pool class, translates samp vehicle ID (index) to a direct CVehicle class pointer

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			
			StartHookingProcedure();
#ifdef DEBUG
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
#endif
		}
	}
	return 1;
}