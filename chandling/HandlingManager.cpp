#include "main.h"
#include "HandlingManager.h"
#include "HandlingDefault.h"

namespace HandlingMgr
{
	void FUNC_CalcHandling(struct tHandlingData* handling)
	{
		((void(__stdcall *)(struct tHandlingData *))0x6F5080)(handling);
	}

	class CHandlingEntry
	{
	private:
		bool calculated = false;
		uint8_t modsCount = 0;
		void RestoreUncalculated() // this restores the uncalculated handling & allows us to calculate it again
		{
			memcpy(&calcHandling, &rawHandling, sizeof(struct tHandlingData));
			calculated = false;
		}

		void __addMod(const struct stHandlingMod mod)
		{
			void* offs = GetHandlingAttribPtr(&this->rawHandling, mod.attrib);
			/* write the value to the handling data so we can Get it later on */
			DebugPrint("offset 0x%x rawHandl 0x%x attrib %d", (int)offs, (int)&this->rawHandling, (int)mod.attrib);
			if (offs == nullptr)
				return;
			switch (mod.type)
			{
			case TYPE_FLOAT:
				*(float*)offs = mod.fval;
			case TYPE_UINT:
			case TYPE_FLAG:
				*(unsigned int*)offs = mod.uival;
				break;
			case TYPE_BYTE:
				*(uint8_t*)offs = mod.bval;
				break;
			}
		}

	public:
		int		modelID = -1;
		struct tHandlingData	rawHandling;
		struct tHandlingData	calcHandling;

		bool isCalculated() { return calculated; }

		int hasMods() { return modsCount; }

		void Recalculate()
		{
			RestoreUncalculated();
			FUNC_CalcHandling(&calcHandling);
			calculated = true;
		}


		bool AddRawMod(const struct stHandlingMod mod)
		{
			if (!CanSetHandlingAttrib(mod.attrib))
				return false;
			__addMod(mod);
			modsCount++;
			return true;
		}

		void Clear()
		{
			calculated = false;
			modsCount = 0;
		}
	};

	struct CVehicleHandlingEntry : CHandlingEntry
	{
	public:
		bool	usesModelHandling = true;
		struct tHandlingData* pCurrentHandling = nullptr;
	};



	bool modelsInitialized = false;
	CHandlingEntry modelHandlings[MAX_VEHICLE_MODELS]; // our per-model handlings

	CVehicleHandlingEntry vehicleHandlings[MAX_VEHICLES + 1]; // per-vehicle handlings

	bool InitVehicle(int vehicleID, int model, bool ignoreModel /* = false */)
	{
		if (!IS_VALID_VEHICLEID(vehicleID))
			return false;

		int real_model = !ignoreModel ? model : vehicleHandlings[vehicleID].modelID;
		vehicleHandlings[vehicleID].Clear();
		vehicleHandlings[vehicleID].pCurrentHandling = nullptr;
		if (!IS_VALID_VEHICLE_MODEL(real_model))
			return false;
		DebugPrint("Init vehicle %d model %d", vehicleID, real_model);
		vehicleHandlings[vehicleID].pCurrentHandling = &modelHandlings[VEHICLE_MODEL_INDEX(real_model)].calcHandling;
		vehicleHandlings[vehicleID].usesModelHandling = true;
		vehicleHandlings[vehicleID].modelID = real_model;
		// copy the model's handling as our handling
		memcpy(&vehicleHandlings[vehicleID].rawHandling, &modelHandlings[VEHICLE_MODEL_INDEX(real_model)].rawHandling, sizeof(struct tHandlingData));
		vehicleHandlings[vehicleID].Recalculate();
		return true;
	}

	tHandlingData* GetHandlingPtrForVehicle(int vehicleID, int model)
	{
		if (!IS_VALID_VEHICLEID(vehicleID) || !IS_VALID_VEHICLE_MODEL(model))
			return nullptr;

		if (!vehicleHandlings[vehicleID].isCalculated())
			InitVehicle(vehicleID, model);

		return vehicleHandlings[vehicleID].pCurrentHandling;
	}

	bool RecalculateModelHandlings() // this gets only called once, after the game is done loading all the handlings
	{
		for (int i = 0; i < MAX_VEHICLE_MODELS; i++)
		{
			modelHandlings[i].Recalculate();
		}
		return true;
	}

	void InitializeModelDefaults()
	{
		for (int i = 0; i < MAX_VEHICLE_MODELS; i++)
		{
			HandlingDefault::copyDefaultModelHandling(i + 400, &modelHandlings[i].rawHandling);
			modelHandlings[i].modelID = i + 400;
		}

		RecalculateModelHandlings();
		modelsInitialized = true;
	}

	bool AddVehicleMod(int vehicleID, const struct stHandlingMod mod)
	{
		if (!IS_VALID_VEHICLEID(vehicleID) || !CanSetHandlingAttrib(mod.attrib))
			return false;

		// TODO: Add some validation
		vehicleHandlings[vehicleID].AddRawMod(mod);
		return true;
	}

	bool ApplyVehicleMods(int vehicleID)
	{
		if (!IS_VALID_VEHICLEID(vehicleID))
			return false;

		if (vehicleHandlings[vehicleID].hasMods())
		{
			vehicleHandlings[vehicleID].Recalculate();
			vehicleHandlings[vehicleID].usesModelHandling = false;
			vehicleHandlings[vehicleID].pCurrentHandling = &vehicleHandlings[vehicleID].calcHandling;

			CVehicle* ptr = pID2PTR[vehicleID];
			if (ptr != nullptr)
			{
				DebugPrint("ApplyVehicleMods live change on ptr 0x%x", (int)ptr);
				ptr->m_pHandlingData = vehicleHandlings[vehicleID].pCurrentHandling;
			}
		}
		return true;
	}
}