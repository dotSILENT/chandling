#pragma once
#include "game/game.h" // handling struct

/*
* Default handling values for each (possible) model
*/
#ifdef DUMP_DEFAULT_HANDLINGS
typedef void(__thiscall *t_ConvertHandlingToGameUnits)(void* thisptr, struct tHandlingData *handling);
#endif

namespace HandlingDefault
{
	bool copyDefaultModelHandling(int modelid, struct tHandlingData* dest);

	void Initialize();

#ifdef DUMP_DEFAULT_HANDLINGS
	extern t_ConvertHandlingToGameUnits originalLoadHandling;

	void __fastcall loadHandlingHook(void* thisptr, DWORD EDX, struct tHandlingData *handling);
#endif
}
