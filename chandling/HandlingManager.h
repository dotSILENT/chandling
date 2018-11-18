#pragma once
#include "game\game.h"
#include "main.h"
#include "HandlingEnum.h"

#include <vector>

#define MAX_HANDLINGS 212


void FUNC_CalcHandling(struct tHandlingData* handling)
{
	((void(__stdcall *)(struct tHandlingData *))0x6F5080)(handling);
}


class CHandlingEntry
{
private:
	bool calculated = false;
public:
	bool stored = false; // set to true when storing any handling for the first time

	struct tHandlingData rawHandling;
	struct tHandlingData calcHandling;

	void RestoreUncalculated() // this restores the uncalculated handling & allows us to calculate it again
	{
		memcpy(&calcHandling, &rawHandling, sizeof(struct tHandlingData));
		calculated = false;
	}

	void Recalculate() // you need to call RestoreUncalculated first
	{
		if (calculated)
			return;

		FUNC_CalcHandling(&calcHandling);
		calculated = true;
	}

	void Clear()
	{
		stored = false;
		calculated = false;
	}
};



class CHandlingManager
{
protected:
	bool handlingsCalculated = false;
	CHandlingEntry modelHandlings[MAX_HANDLINGS]; // our per-model handlings
	CHandlingEntry modelHandlingsOriginal[MAX_HANDLINGS]; // original per-model handlings

	CHandlingEntry vehicleHandlings[MAX_VEHICLES + 1]; // per-vehicle handlings

public:

	tHandlingData* GetHandlingForVehicle(int vehicleID, int model)
	{
		if (vehicleID < 0 || vehicleID > sizeof(vehicleHandlings) || model < 0 || model > sizeof(modelHandlings))
			return nullptr;
		
		if (vehicleHandlings[vehicleID].stored)
			return &vehicleHandlings[vehicleID].calcHandling;
		return &modelHandlings[model].calcHandling;
	}

	void StoreRawModelHandling(unsigned int index, tHandlingData* handlingptr)
	{
		if (index < MAX_HANDLINGS)
		{
			if (modelHandlings[index].stored == false)
			{
				modelHandlings[index].stored = true;
				memcpy(&modelHandlings[index].rawHandling, handlingptr, sizeof(struct tHandlingData));
			}

			if (modelHandlingsOriginal[index].stored == false)
			{
				modelHandlingsOriginal[index].stored = true;
				memcpy(&modelHandlingsOriginal[index].rawHandling, handlingptr, sizeof(struct tHandlingData));
			}
		}
	}

	bool RecalculateModelHandlings() // this gets only called once, after the game is done loading all the handlings
	{
		if (handlingsCalculated)
			return false;

		for (int i = 0; i < MAX_HANDLINGS; i++)
		{
			if (modelHandlings[i].stored)
			{
				modelHandlings[i].RestoreUncalculated();
				modelHandlings[i].Recalculate();
			}
		}
		handlingsCalculated = true;
		return true;
	}

};