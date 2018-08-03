#pragma once
#include "game\game.h"
#include "HandlingEnum.h"
#include <list>

#define MAX_HANDLINGS 212
#define MAX_VEHICLES 2000


void FUNC_CalcHandling(struct tHandlingData* handling)
{
	((void(__stdcall *)(struct tHandlingData *))0x6F5080)(handling);
}



class HandlingModifiers
{
private:
	struct modifiers
	{
		handlingAttributes attrib;
		char modifier; // -+*
		char value[16];

		void ApplyModifier(float &handlval)
		{
			if (modifier == '-')
				handlval -= (float)atof(value);
			else if (modifier == '+')
				handlval += (float)atof(value);
			else if (modifier == '*')
				handlval *= (float)atof(value);
			else handlval = (float)atof(value);
		}

		void ApplyModifier(int &handlval)
		{
			if (modifier == '-')
				handlval -= atoi(value);
			else if (modifier == '+')
				handlval += atoi(value);
			else if (modifier == '*')
				handlval *= atoi(value);
			else handlval = atoi(value);
		}

		void ApplyModifier(unsigned int &handlval)
		{
			if (modifier == '-')
				handlval -= atoi(value);
			else if (modifier == '+')
				handlval += atoi(value);
			else if (modifier == '*')
				handlval *= atoi(value);
			else handlval = atoi(value);
		}

		void ApplyModifier(uint8_t &handlval)
		{
			if (modifier == '-')
				handlval -= (uint8_t)atoi(value);
			else if (modifier == '+')
				handlval += (uint8_t)atoi(value);
			else if (modifier == '*')
				handlval *= (uint8_t)atoi(value);
			else handlval = (uint8_t)atoi(value);
		}

		void ApplyFlagModifier(uint32_t &handlval)
		{
			uint32_t flag = (uint32_t)atoi(value);
			if (modifier == '-')
			{
				if(handlval & flag)
					handlval &= ~flag;
			}
			else if (modifier == '+')
			{
				if(!(handlval & flag))
					handlval |= flag;
			}
			else handlval = flag;

		}
	};

	std::list<struct modifiers> lModifiers;

public:
	void AddModifier(handlingAttributes attrib, char*  value)
	{
		struct modifiers x;
		x.attrib = attrib;
		if (value[0] == '-' || value[0] == '+' || value[0] == '*')
		{
			x.modifier = value[0];
			strncpy(x.value, value+1, sizeof(modifiers::value));
		}
		else
			strncpy(x.value, value, sizeof(modifiers::value));
		lModifiers.push_back(x);
	}

	void AddModifier(handlingAttributes attrib, int value, char modifier = NULL)
	{
		struct modifiers x;
		x.attrib = attrib;
		x.modifier = modifier;
		_itoa(value, x.value, 10);
		lModifiers.push_back(x);
	}

	bool RemoveModifierByValue(handlingAttributes attrib, char* value) // after removing a modifier, ApplyModifiersToHandling needs to be used again to apply every modifier but this one
	{
		std::list<struct modifiers>::iterator i = lModifiers.begin();

		while (i != lModifiers.end())
		{
			if (i->attrib == attrib && !strncmp(i->value, value, sizeof(modifiers::value)))
			{
				lModifiers.erase(i);
				return true; // there may be more modifiers of the same value & attribute, but we consider only one (since modifiers can be applied many times)
			}
			i++;
		}
		return false;
	}

	void ClearModifiers()
	{
		lModifiers.clear();
	}

	void ApplyModifiersToHandling(struct tHandlingData* handling) // raw handling, obviously
	{
		std::list<struct modifiers>::iterator i = lModifiers.begin();

		while (i != lModifiers.end())
		{
			switch (i->attrib)
			{
				case HANDL_FMASS:
				{
					i->ApplyModifier(handling->m_fMass);
					break;
				}

				case HANDL_FTURNMASS:
				{
					i->ApplyModifier(handling->m_fTurnMass);
					break;
				}

				case HANDL_FDRAGMULTIPLIER:
				{;
					i->ApplyModifier(handling->m_fDragMult);
					break;
				}

				case HANDL_CENTREOFMASS_X:
				{
					i->ApplyModifier(handling->m_vecCentreOfMass.fX);
					break;
				}

				case HANDL_CENTREOFMASS_Y:
				{
					i->ApplyModifier(handling->m_vecCentreOfMass.fY);
					break;
				}

				case HANDL_CENTREOFMASS_Z:
				{
					i->ApplyModifier(handling->m_vecCentreOfMass.fZ);
					break;
				}

				case HANDL_NPERCENTSUBMERGED:
				{
					i->ApplyModifier(handling->m_nPercentSubmerged);
					break;
				}

				case HANDL_FTRACTIONMULTIPLIER:
				{
					i->ApplyModifier(handling->m_fTractionMultiplier);
					break;
				}

				case HANDL_FTRACTIONLOSS:
				{
					i->ApplyModifier(handling->m_fTractionLoss);
					break;
				}

				case HANDL_FTRACTIONBIAS:
				{
					i->ApplyModifier(handling->m_fTractionBias);
					break;
				}

				case HANDL_TRDATA_NNUMBEROFGEARS:
				{
					i->ApplyModifier(handling->m_transmissionData.m_nNumberOfGears);
					break;
				}

				case HANDL_TRDATA_FMAXVELOCITY:
				{
					i->ApplyModifier(handling->m_transmissionData.m_fMaxGearVelocity);
					break;
				}

				case HANDL_TRDATA_FENGINEACCELERATION:
				{
					i->ApplyModifier(handling->m_transmissionData.m_fEngineAcceleration);
					break;
				}

				case HANDL_TRDATA_FENGINEINERTIA:
				{
					i->ApplyModifier(handling->m_transmissionData.m_fEngineInertia);
					break;
				}

				case HANDL_TRDATA_NDRIVETYPE: // this actually fucks the game up, but whatever
				{
					handling->m_transmissionData.m_nDriveType = i->value[0];
					break;
				}

				case HANDL_TRDATA_NENGINETYPE:
				{
					handling->m_transmissionData.m_nEngineType = i->value[0]; // 4/F/R
					break;
				}

				case HANDL_FBRAKEDECELERATION:
				{
					i->ApplyModifier(handling->m_fBrakeDeceleration);
					break;
				}

				case HANDL_FBRAKEBIAS:
				{
					i->ApplyModifier(handling->m_fBrakeBias);
					break;
				}

				case HANDL_BABS:
				{
					handling->m_bABS = (atoi(i->value)) ? true : false;
					break;
				}

				case HANDL_FSTEERINGLOCK:
				{
					i->ApplyModifier(handling->m_fSteeringLock);
					break;
				}

				case HANDL_FSUSPENSIONFORCELEVEL:
				{
					i->ApplyModifier(handling->m_fSuspensionForceLevel);
					break;
				}

				case HANDL_FSUSPENSIONDAMPINGLEVEL:
				{
					i->ApplyModifier(handling->m_fSuspensionDampingLevel);
					break;
				}

				case HANDL_FSUSPENSIONHIGHSPDCOMDAMP:
				{
					i->ApplyModifier(handling->m_fSuspensionHighSpdComDamp);
					break;
				}

				case HANDL_FSUSPENSIONUPPERLIMIT:
				{
					i->ApplyModifier(handling->m_fSuspensionUpperLimit);
					break;
				}

				case HANDL_FSUSPENSIONLOWERLIMIT:
				{
					i->ApplyModifier(handling->m_fSuspensionLowerLimit);
					break;
				}

				case HANDL_FSUSPENSIONBIAS:
				{
					i->ApplyModifier(handling->m_fSuspensionBiasBetweenFrontAndRear);
					break;
				}

				case HANDL_FSUSPENSIONANTIDIVEMULT:
				{
					i->ApplyModifier(handling->m_fSuspensionAntiDiveMultiplier);
					break;
				}

				case HANDL_FSEATOFFSETDISTANCE:
				{
					i->ApplyModifier(handling->m_fSeatOffsetDistance);
					break;
				}

				case HANDL_FCOLLISIONDAMAGEMULTIPLIER:
				{
					i->ApplyModifier(handling->m_fCollisionDamageMultiplier);
					break;
				}

				case HANDL_NMONETARYVALUE:
				{
					i->ApplyModifier(handling->m_nMonetaryValue);
					break;
				}

				case HANDL_MODELFLAGS:
				{
					uint32_t temp = handling->m_nModelFlags;
					i->ApplyFlagModifier(temp);
					handling->m_nModelFlags = (eVehicleHandlingModelFlags)temp;
					break;
				}

				case HANDL_HANDLINGFLAGS:
				{
					
					uint32_t temp = handling->m_nHandlingFlags;
					i->ApplyFlagModifier(temp);
					handling->m_nHandlingFlags = (eVehicleHandlingFlags)temp;
					break;
				}

				case HANDL_FRONTLIGHTS:
				{
					handling->m_nFrontLights = (eVehicleLightsSize)atoi(i->value);
					break;
				}

				case HANDL_REARLIGHTS:
				{
					handling->m_nRearLights = (eVehicleLightsSize)atoi(i->value);
					break;
				}

				case HANDL_ANIMGROUP:
				{
					handling->m_nAnimGroup = atoi(i->value);
				}
			}

			i++;
		}
	}
};


class HandlingEntry
{
private:
	bool calculated = false;
public:
	bool stored = false; // set to true when storing any handling for the first time
	bool modified = false;

	struct tHandlingData rawHandling;
	HandlingModifiers modifiers;
	struct tHandlingData calcHandling;

	int GetHandlingIndex() { return rawHandling.m_iIndex; };

	void RestoreUncalculated() // this restores the uncalculated handling & allows us to calculate it again
	{
		memcpy(&calcHandling, &rawHandling, sizeof(struct tHandlingData));
		calculated = false;
	}

	void Recalculate() // you need to call RestoreUncalculated first
	{
		if (calculated)
			return;
		
		modifiers.ApplyModifiersToHandling(&calcHandling);
		FUNC_CalcHandling(&calcHandling);
		calculated = true;
	}

	void Clear()
	{
		stored = false;
		modified = false;
		calculated = false;
		modifiers.ClearModifiers();
	}
};



class Manager
{
public:
	bool handlingsCalculated = false;
	HandlingEntry modelHandlings[MAX_HANDLINGS];
	HandlingEntry modelHandlingsOriginal[MAX_HANDLINGS];
	
	HandlingEntry vehicleHandlings[MAX_VEHICLES+1];

public:
	void StoreRawModeHandling(unsigned int index, tHandlingData* handlingptr)
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