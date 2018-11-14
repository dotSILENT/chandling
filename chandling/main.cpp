#include "main.h"
#include "sampVersions.h"
#include "Hooks.h"

// globals
DWORD dwSampDLL = NULL;
CAddresses Addr;
bool gInited = false;


DWORD WINAPI waitForSamp()
{
	int startTime = GetTickCount();
	while (true)
	{
		if (!gInited && GetTickCount() - startTime >= 35 * 1000)
		{
			DebugPrint("SAMP Module not found, exiting...\n");
			break;
		}

		if (dwSampDLL == NULL && (dwSampDLL = (DWORD)GetModuleHandleA("samp.dll")) != NULL)
		{
			DebugPrint("SAMP Module loaded at 0x%x\n", (DWORD)dwSampDLL);
		}
		else if (!gInited)
		{
			// TODO: Version detection
			Addr.Init(SAMP_037_R2);

			DWORD * info = (DWORD*)(dwSampDLL + Addr.OFFSET_SampInfo);
			if (*(DWORD**)info == nullptr)
				continue;

			gInited = true;
			DebugPrint("Setting up SAMP Hooks\n");
			SetupSampHooks();
			DebugPrint("SAMP Initialized, host: %s\n", (char*)((*(DWORD*)info) + Addr.OFFSET_SampInfo_Hostname));
		}
		
		Sleep(100);
	}
	ExitThread(EXIT_SUCCESS);
	return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)waitForSamp, NULL, NULL, NULL);

			SetupGtaHooks();
#ifdef DEBUG
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
#endif
		}
	}
	return 1;
}