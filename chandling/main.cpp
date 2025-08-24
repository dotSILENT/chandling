#include "main.h"
#include "Hooks.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// globals
DWORD dwSampDLL = NULL;
bool gInited = false;
bool gUsingCHandling = false;
CVehicle** pID2PTR = nullptr; // CVehicle* m_pGTAVehicles[] array in samp's vehicle pool class, translates samp vehicle ID (index) to a direct CVehicle class pointer

DWORD *pSampChat = nullptr;
tAddChatMessage addChatMsgFn = nullptr;

FILE *g_fLog;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
#ifdef DEBUG
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
#endif

			StartHookingProcedure();
			break;

		case DLL_PROCESS_DETACH:
			if (g_fLog != NULL)
				fclose(g_fLog);
			break;
	}
	return 1;
}

void LogError(const char *fmt, ...)
{
	SYSTEMTIME	time;
	va_list		ap;

	if (g_fLog == NULL)
	{
		g_fLog = fopen(CHANDLING_ERROR_LOG, "w");
		if (g_fLog == NULL)
			return;
	}

	GetLocalTime(&time);
	fprintf(g_fLog, "[%02d:%02d:%02d.%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	va_start(ap, fmt);
	vfprintf(g_fLog, fmt, ap);
	// implementation of DebugPrint() macro
#ifdef DEBUG
	printf("[cHandling] ERROR! ");
	vprintf(fmt, ap);
	printf("\n");
#endif
	va_end(ap);
	fprintf(g_fLog, "\n");
	fflush(g_fLog);
}

void AddChatMessageRaw(const char* fmt, ...)
{
	if (!pSampChat || !addChatMsgFn || !*pSampChat)
		return;

	// format here bc floats get corrupted when forwarding va_list to addChatMsgFn for some reason
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	addChatMsgFn(*pSampChat, buffer);
}