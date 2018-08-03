#define _CRT_SECURE_NO_WARNIGNS
#include <Windows.h>
#include <windef.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <Psapi.h>
#include "raknet\RakClientInterface.h"
#include "hookfunctions.h"
#include "main.h"

#include "samp.h"
#include "game\game.h"
#include "Manager.h"

HMODULE hDllModule = NULL;
HMODULE hSampDLL = NULL;
uint32_t sampAddr = NULL;
struct stSAMP *gSamp;

SAMP::samp* pSampCore = new SAMP::samp();
#define SAMP_INFO_OFFSET	0x21A0F8

Manager handlingMgr;



static signed char hex_to_dec(signed char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	if (ch >= 'a' && ch <= 'f')
		return ch - 'A' + 10;

	return -1;
}

uint8_t *hex_to_bin(const char *str)
{
	int		len = (int)strlen(str);
	uint8_t *buf, *sbuf;

	if (len == 0 || len % 2 != 0)
		return NULL;

	sbuf = buf = (uint8_t *)malloc(len / 2);

	while (*str)
	{
		signed char bh = hex_to_dec(*str++);
		signed char bl = hex_to_dec(*str++);

		if (bl == -1 || bh == -1)
		{
			free(sbuf);
			return NULL;
		}

		*buf++ = (uint8_t)(bl | (bh << 4));
	}

	return sbuf;
}

static int __page_size_get(void)
{
	static int	page_size = -1;
	SYSTEM_INFO si;

	if (page_size == -1)
	{
		GetSystemInfo(&si);
		page_size = (int)si.dwPageSize;
	}

	return page_size;
}

bool isBadPtr_handlerAny(void *pointer, ULONG size, DWORD dwFlags)
{
	DWORD						dwSize;
	MEMORY_BASIC_INFORMATION	meminfo;

	if (NULL == pointer)
		return true;

	memset(&meminfo, 0x00, sizeof(meminfo));
	dwSize = VirtualQuery(pointer, &meminfo, sizeof(meminfo));

	if (0 == dwSize)
		return true;

	if (MEM_COMMIT != meminfo.State)
		return true;

	if (0 == (meminfo.Protect & dwFlags))
		return true;

	if (size > meminfo.RegionSize)
		return true;

	if ((unsigned)((char *)pointer - (char *)meminfo.BaseAddress) > (unsigned)(meminfo.RegionSize - size))
		return true;

	return false;
}

bool isBadPtr_readAny(void *pointer, ULONG size)
{
	return isBadPtr_handlerAny(pointer, size, PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
		PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
}

bool isBadPtr_writeAny(void *pointer, ULONG size)
{
	return isBadPtr_handlerAny(pointer, size,
		PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
}

static int __page_write(void *_dest, const void *_src, uint32_t len)
{
	static int		page_size = __page_size_get();
	uint8_t			*dest = (uint8_t *)_dest;
	const uint8_t	*src = (const uint8_t *)_src;
	DWORD			prot_prev = 0;
	int				prot_changed = 0;
	/*SIZE_T			write_len;*/
	int				ret = 1;

	while (len > 0)
	{
		int page_offset = (int)((UINT_PTR)dest % page_size);
		int page_remain = page_size - page_offset;
		int this_len = len;

		if (this_len > page_remain)
			this_len = page_remain;

		if (isBadPtr_writeAny(dest, this_len))
		{
			if (!VirtualProtect((void *)dest, this_len, PAGE_EXECUTE_READWRITE, &prot_prev))
				ret = 0;
			else
				prot_changed = 1;
		}

		/*if ( !WriteProcessMemory(GetCurrentProcess(), dest, (void *)src, this_len, &write_len) )
		write_len = 0;*/
		memcpy(dest, src, this_len);

		if (prot_changed)
		{
			
			//if (!VirtualProtect((void *)dest, this_len, prot_prev, &dummy))
				//Log("__page_write() could not restore original permissions for ptr %p", dest);
		}

		/*if ( (int)write_len != this_len )
		ret = 0;*/

		dest += this_len;
		src += this_len;
		len -= this_len;
	}

	return ret;
}

static int __page_read(void *_dest, const void *_src, uint32_t len)
{
	static int	page_size = __page_size_get();
	uint8_t		*dest = (uint8_t *)_dest;
	uint8_t		*src = (uint8_t *)_src;
	DWORD		prot_prev = 0;
	int			prot_changed = 0;
	/*SIZE_T		read_len;*/
	int			ret = 1;

	while (len > 0)
	{
		int page_offset = (int)((UINT_PTR)src % page_size);
		int page_remain = page_size - page_offset;
		int this_len = len;

		if (this_len > page_remain)
			this_len = page_remain;

		if (isBadPtr_readAny(src, this_len))
		{
			if (!VirtualProtect((void *)src, this_len, PAGE_EXECUTE_READWRITE, &prot_prev))
				ret = 0;
			else
				prot_changed = 1;
		}

		/*if ( !ReadProcessMemory(GetCurrentProcess(), src, dest, this_len, &read_len) )
		read_len = 0;*/
		memcpy(dest, src, this_len);

		if (prot_changed)
		{
			//DWORD	dummy;
			//if (!VirtualProtect((void *)src, this_len, prot_prev, &dummy))
				
		}

		/*if ( (int)read_len != this_len )
		{
		memset( dest + read_len, 0, this_len - read_len );
		ret = 0;
		}*/

		dest += this_len;
		src += this_len;
		len -= this_len;
	}

	return ret;
}

int	memcpy_safe(void *dest, const void *src, uint32_t len, int check = NULL, const void *checkdata = NULL);

int memcmp_safe(const void *_s1, const void *_s2, uint32_t len)
{
	const uint8_t	*s1 = (const uint8_t *)_s1;
	const uint8_t	*s2 = (const uint8_t *)_s2;
	uint8_t			buf[4096];

	for (;; )
	{
		if (len > 4096)
		{
			if (!memcpy_safe(buf, s1, 4096))
				return 0;
			if (memcmp(buf, s2, 4096))
				return 0;
			s1 += 4096;
			s2 += 4096;
			len -= 4096;
		}
		else
		{
			if (!memcpy_safe(buf, s1, len))
				return 0;
			if (memcmp(buf, s2, len))
				return 0;
			break;
		}
	}

	return 1;
}

int memcpy_safe(void *_dest, const void *_src, uint32_t len, int check, const void *checkdata)
{
	static int		page_size = __page_size_get();
	static int		recurse_ok = 1;
	uint8_t			buf[4096];
	uint8_t			*dest = (uint8_t *)_dest;
	const uint8_t	*src = (const uint8_t *)_src;
	int				ret = 1;

	if (check && checkdata)
	{
		if (!memcmp_safe(checkdata, _dest, len))
			return 0;
	}

	while (len > 0)
	{
		uint32_t	this_len = sizeof(buf);

		if (this_len > len)
			this_len = len;

		if (!__page_read(buf, src, this_len))
			ret = 0;

		if (!__page_write(dest, buf, this_len))
			ret = 0;

		len -= this_len;
		src += this_len;
		dest += this_len;
	}

	return ret;
}



#define SAMP_CMP		"F8036A004050518D4C24"

DWORD WINAPI waitForSamp()
{
	while (true)
	{
		static int counter = 0;

		counter++;
		if ((hSampDLL = GetModuleHandleA("samp.dll")) != NULL)
		{
			sampAddr = (uint32_t)hSampDLL;

			if (memcmp_safe((uint8_t *)sampAddr + 0xBABE, hex_to_bin(SAMP_CMP), 10))
			{
				SAMP::samp::AddToLog("version valid");
			}
			else
			{
				SAMP::samp::AddToLog("invalid version");
			}

			pSampCore->SetUp();
			ExitThread(EXIT_SUCCESS);
			return 1;
		}
		else if (counter > 200)
			ExitThread(EXIT_SUCCESS);

		Sleep(50);
	}
	return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			hDllModule = hModule;
			
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)waitForSamp, NULL, NULL, NULL);
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
		
		}
	}
	return 1;
}

template<typename T>
T GetSAMPPtrInfo(uint32_t offset)
{
	if (sampAddr == NULL)
		return NULL;
	return *(T *)((uint32_t)sampAddr + offset);
}

struct stSAMP *stGetSampInfo(void)
{
	return GetSAMPPtrInfo<struct stSAMP *>(SAMP_INFO_OFFSET);
}



DWORD dwCalcJMPBack = NULL;
char originalCalcBytes[11];
#define MAX_HANDLINGS 210

#define FUNC_CALC_HANDLING 0x6f5080


int count;
void __declspec(naked) Hook_CalculateHandling()
{
	DWORD dwHandlingData;
	struct tHandlingData* pdata;
	_asm
	{
		mov eax, [esp+4]
		mov dwHandlingData, eax
	}

	// copy the un-calculated handling data
	pdata = (struct tHandlingData*)(dwHandlingData);
	
	handlingMgr.StoreRawModeHandling(count, pdata);

	count++;
	printf("\nstored %d handlings fmax %f %f cur index %d", count, pdata->m_transmissionData.m_fMaxGearVelocity, handlingMgr.modelHandlings[count-1].rawHandling.m_transmissionData.m_fMaxGearVelocity, pdata->m_iIndex);


	if (count >= MAX_HANDLINGS)
	{
		// restore the original bytes in this function (destroy our hook)
		DWORD dwOldProtect, dwBkup;

		VirtualProtect((BYTE*)FUNC_CALC_HANDLING, sizeof(originalCalcBytes), PAGE_EXECUTE_READWRITE, &dwOldProtect);
		memcpy((BYTE*)FUNC_CALC_HANDLING, &originalCalcBytes, sizeof(originalCalcBytes));
		VirtualProtect((BYTE*)FUNC_CALC_HANDLING, sizeof(originalCalcBytes), dwOldProtect, &dwBkup);
		printf("\nhandlings stored");

		handlingMgr.RecalculateModelHandlings();
	}

	_asm
	{
		ret 4;
	}
}



DWORD dwJMPback;
int created = 0;
DWORD dwVehPtr = NULL;

struct sampVeh
{
	uint8_t pad[0x4C];
	CVehicle *gtaVehicle;
};

struct sampVeh *sampvehptr;
int createdVehicleID;

void __declspec(naked) CreateVehicle()
{
	// original instructions
	__asm mov edx, [ecx+0x4C]
	__asm mov [edi+eax*4+0x4FB4], edx

	// store some stuff
	__asm mov createdVehicleID, eax
	__asm mov sampvehptr, ecx

	__asm PUSHAD 
	__asm PUSHFD
	// ecx = samp vehicle pointer
	
	printf("\nVehicle id %d created model %d", createdVehicleID, sampvehptr->gtaVehicle->m_nModel);

	sampvehptr->gtaVehicle->m_pHandlingData = &handlingMgr.modelHandlings[sampvehptr->gtaVehicle->m_pHandlingData->m_iIndex].calcHandling;
	

	printf("\nDrive type: %d %c", sampvehptr->gtaVehicle->m_pHandlingData->m_transmissionData.m_nDriveType, sampvehptr->gtaVehicle->m_pHandlingData->m_transmissionData.m_nDriveType);
	
	// test, set up a individual handling for vehicle with id 3
	if (createdVehicleID == 3 && !handlingMgr.vehicleHandlings[createdVehicleID].modified)
	{
		handlingMgr.vehicleHandlings[createdVehicleID].modified = true;

		memcpy(&handlingMgr.vehicleHandlings[createdVehicleID].rawHandling, &handlingMgr.modelHandlings[sampvehptr->gtaVehicle->m_pHandlingData->m_iIndex].rawHandling, sizeof(struct tHandlingData));
		handlingMgr.modelHandlings[sampvehptr->gtaVehicle->m_pHandlingData->m_iIndex].modifiers.ApplyModifiersToHandling(&handlingMgr.vehicleHandlings[createdVehicleID].rawHandling);
		handlingMgr.vehicleHandlings[createdVehicleID].RestoreUncalculated();

		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_FDRAGMULTIPLIER, "*0.97");
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_FMASS, "-400.0");
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_FTRACTIONMULTIPLIER, "*1.02");
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_TRDATA_FENGINEINERTIA, "*1.2");
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_HANDLINGFLAGS, HANDL_HALOGEN_LIGHTS, '-');
		//handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_FSUSPENSIONBIAS, "*0.99");
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_HANDLINGFLAGS, HANDL_WHEEL_R_WIDE2, '+');
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_HANDLINGFLAGS, HANDL_WHEEL_R_WIDE, '-');
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_FRONTLIGHTS, LIGHTS_TALL);
		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_REARLIGHTS, LIGHTS_TALL);

		handlingMgr.vehicleHandlings[createdVehicleID].modifiers.ApplyModifiersToHandling(&handlingMgr.vehicleHandlings[createdVehicleID].calcHandling);

		handlingMgr.vehicleHandlings[createdVehicleID].Recalculate();
		
	}
	
	if(sampvehptr->gtaVehicle->m_pHandlingData && handlingMgr.vehicleHandlings[createdVehicleID].modified)
	{
		sampvehptr->gtaVehicle->m_pHandlingData = &handlingMgr.vehicleHandlings[createdVehicleID].calcHandling;

		printf("\nMax velocity: %f handling flags 0x%x", sampvehptr->gtaVehicle->m_pHandlingData->m_transmissionData.m_fMaxGearVelocity, sampvehptr->gtaVehicle->m_pHandlingData->m_nHandlingFlags);
	}

	__asm POPFD
	__asm POPAD 

	__asm JMP[dwJMPback]
}

void PlaceJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;
	*pAddress = 0xE9;

	*((DWORD *)(pAddress + 0x1)) = dwRelAddr;

	for (DWORD x = 0x5; x < dwLen; x++)
		*(pAddress + x) = 0x90;
	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

}

////////////////////////////////////////////////////////////////////////
// Crashfixes taken from MTA
// Handle CObject::ProcessGarageDoorBehaviour object with no dummy
#define HOOKPOS_CrashFix_Misc28                             0x44A4FD
#define HOOKSIZE_CrashFix_Misc28                            6
DWORD RETURN_CrashFix_Misc28 = 0x44A503;
DWORD RETURN_CrashFix_Misc28B = 0x44A650;
void _declspec(naked) HOOK_CrashFix_Misc28()
{
	_asm
	{
		// Execute replaced code
		mov     eax, [esi + 170h]
	}

#if TEST_CRASH_FIXES
	SIMULATE_ERROR_BEGIN(50)
		_asm
	{
		mov     eax, 0
	}
	SIMULATE_ERROR_END
#endif

		_asm
	{
		// Check if dummy pointer is zero
		test    eax, eax
		jne     cont

			// Skip much code
			jmp     RETURN_CrashFix_Misc28B

			cont :
		// Continue standard path
		jmp     RETURN_CrashFix_Misc28
	}
}

namespace SAMP
{

	Send_t samp::pSend = NULL;
	Receive_t samp::pReceive = NULL;
	//HandleRPC_t samp::pHandleRPC = NULL;
	//AddToChat_t samp::pAddToChat = NULL;
	RakClientInterface* samp::ppRakClient = NULL;

	samp::samp()
	{

	}

	samp::~samp()
	{
		StringCompressor::RemoveReference();
	}

	void samp::InstallVTHooks(RakClientInterface* pRakClient)
	{
		static bool bVTHooksInstalled = false;
		if (bVTHooksInstalled == false)
		{
			VTableRedirect(*(VOID***)pRakClient, Send, (VOID**)&pSend, 6);
			bVTHooksInstalled = true;
		}
	}

	void samp::SetUp()
	{
		pSampCore->m_samp = (unsigned)GetModuleHandleA("samp.dll");

		gSamp = stGetSampInfo();

		if (pSampCore->m_samp == NULL)
			return;

		pReceive = (Receive_t)DetourFunc((BYTE*)(FindPattern("\x6A\x04\x8B\xCE\xC7\x44\x24\x00\x00\x00\x00\x00", "xxxxxxx?????") - 60), (BYTE*)Receive, 6);
		//pHandleRPC = (HandleRPC_t)DetourFunc((BYTE*)FindPattern("\x55\x8D\x6C\x24\x98\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50", "xxxxxxxx????xx????x"), (BYTE*)HandleRPC, 5);
		//pAddToChat = (AddToChat_t)FindPattern("\x56\x8B\x74\x24\x0C\x8B\xC6\x57\x8B\xF9\x8D\x50\x01\x8D\x49\x00\x8A\x08", "xxxxxxxxxxxxxxxxxx");
		//DetourFunc((BYTE*)sampAddr + 0xB2525, (BYTE*)CreateVehicle, 6);

		dwJMPback = sampAddr + 0x1B71E + 10; // to find this place look for "vehicle %d was not deleted"
		PlaceJMP((BYTE*)sampAddr + 0x1B71E, (DWORD)CreateVehicle, 10);

		dwCalcJMPBack = FUNC_CALC_HANDLING + 11;
		memcpy(originalCalcBytes, (BYTE*)FUNC_CALC_HANDLING, 11);
		PlaceJMP((BYTE*)FUNC_CALC_HANDLING, (DWORD)Hook_CalculateHandling, 11);
		PlaceJMP((BYTE*)HOOKPOS_CrashFix_Misc28, (DWORD)HOOK_CrashFix_Misc28, HOOKSIZE_CrashFix_Misc28);

	}


	bool __thiscall samp::Send(RakClientInterface* pRakClient, RakNet::BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
	{
		static bool bVTHooksInstalled = false;
		if (bVTHooksInstalled == false)
		{
			pSampCore->InstallVTHooks(pRakClient);
			bVTHooksInstalled = true;
		}

		return pSend(pRakClient, bitStream, priority, reliability, orderingChannel);
	}


	Packet* __thiscall samp::Receive(RakClientInterface* pRakClient)
	{
		if (ppRakClient == NULL)
		{
			ppRakClient = pRakClient;
		}

		static bool bVTHooksInstalled = false;
		if (bVTHooksInstalled == false)
		{
			pSampCore->InstallVTHooks(pRakClient);
			bVTHooksInstalled = true;
		}

		Packet *pkt = pReceive(pRakClient);

		if (pkt)
		{		
			char txt[100];
			sprintf_s(txt, 20, "packet %d", (BYTE)pkt->data[0]);
			samp::AddToLog(txt);

			gSamp = stGetSampInfo();
		}

		return pkt;
	}

	void samp::AddToLog(char *fmt, ...)
	{//z raksampa

		//SYSTEMTIME	time;
		va_list		ap;
		FILE* bLog = NULL;

		if (bLog == NULL)
		{
			fopen_s(&bLog, "chandling.log", "a");
			if (bLog == NULL)
				return;
		}

		//GetLocalTime(&time);
		//fprintf(bLog, "[%02d:%02d:%02d.%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		va_start(ap, fmt);
		vfprintf(bLog, fmt, ap);

		va_end(ap);

		fprintf(bLog, "\n");

		fflush(bLog);
		fclose(bLog);
		bLog = NULL;

	}
};

