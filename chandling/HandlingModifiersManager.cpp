#include "HandlingModifiersManager.h"

/* privates */

void CHandlingModifiersMgr::ApplyModifier(struct stHandlingModifier mod, float &handlval)
{
	if (mod.modifier == '-')
		handlval -= (float)atof(mod.value);
	else if (mod.modifier == '+')
		handlval += (float)atof(mod.value);
	else if (mod.modifier == '*')
		handlval *= (float)atof(mod.value);
	else handlval = (float)atof(mod.value);
}

void CHandlingModifiersMgr::ApplyModifier(struct stHandlingModifier mod, int &handlval)
{
	if (mod.modifier == '-')
		handlval -= atoi(mod.value);
	else if (mod.modifier == '+')
		handlval += atoi(mod.value);
	else if (mod.modifier == '*')
		handlval *= atoi(mod.value);
	else handlval = atoi(mod.value);
}

void CHandlingModifiersMgr::ApplyModifier(struct stHandlingModifier mod, unsigned int &handlval)
{
	if (mod.modifier == '-')
		handlval -= atoi(mod.value);
	else if (mod.modifier == '+')
		handlval += atoi(mod.value);
	else if (mod.modifier == '*')
		handlval *= atoi(mod.value);
	else handlval = atoi(mod.value);
}

void CHandlingModifiersMgr::ApplyModifier(struct stHandlingModifier mod, uint8_t &handlval)
{
	if (mod.modifier == '-')
		handlval -= (uint8_t)atoi(mod.value);
	else if (mod.modifier == '+')
		handlval += (uint8_t)atoi(mod.value);
	else if (mod.modifier == '*')
		handlval *= (uint8_t)atoi(mod.value);
	else handlval = (uint8_t)atoi(mod.value);
}

void CHandlingModifiersMgr::ApplyFlagModifier(struct stHandlingModifier mod, uint32_t &handlval)
{
	uint32_t flag = (uint32_t)atoi(mod.value);
	if (mod.modifier == '-')
	{
		if (handlval & flag)
			handlval &= ~flag;
	}
	else if (mod.modifier == '+')
	{
		if (!(handlval & flag))
			handlval |= flag;
	}
	else handlval = flag;
}


/* publics */

void CHandlingModifiersMgr::AddModifier(eHandlingAttribute attrib, char*  value)
{
	struct stHandlingModifier x;
	x.attrib = attrib;
	if (value[0] == '-' || value[0] == '+' || value[0] == '*')
	{
		x.modifier = value[0];
		strncpy(x.value, value + 1, sizeof(stHandlingModifier::value));
	}
	else
		strncpy(x.value, value, sizeof(stHandlingModifier::value));
	this->PushModifierInternal(x);
}

void CHandlingModifiersMgr::AddModifier(eHandlingAttribute attrib, int value, char modifier)
{
	struct stHandlingModifier x;
	x.attrib = attrib;
	x.modifier = modifier;
	_itoa(value, x.value, 10);
	this->PushModifierInternal(x);
}



void CHandlingModifiersMgr::ApplyModifiersToRawHandling(struct tHandlingData* handling) // raw handling, obviously
{
	std::vector<struct stHandlingModifier>::const_iterator i = this->vModifiers.begin();

	while (i != this->vModifiers.end())
	{
		switch (i->attrib)
		{
		case HANDL_FMASS:
		{
			this->ApplyModifier(*i, handling->m_fMass);
			break;
		}

		case HANDL_FTURNMASS:
		{
			this->ApplyModifier(*i, handling->m_fTurnMass);
			break;
		}

		case HANDL_FDRAGMULTIPLIER:
		{
			this->ApplyModifier(*i, handling->m_fDragMult);
			break;
		}

		case HANDL_CENTREOFMASS_X:
		{
			this->ApplyModifier(*i, handling->m_vecCentreOfMass.fX);
			break;
		}

		case HANDL_CENTREOFMASS_Y:
		{
			this->ApplyModifier(*i, handling->m_vecCentreOfMass.fY);
			break;
		}

		case HANDL_CENTREOFMASS_Z:
		{
			this->ApplyModifier(*i, handling->m_vecCentreOfMass.fZ);
			break;
		}

		case HANDL_NPERCENTSUBMERGED:
		{
			this->ApplyModifier(*i, handling->m_nPercentSubmerged);
			break;
		}

		case HANDL_FTRACTIONMULTIPLIER:
		{
			this->ApplyModifier(*i, handling->m_fTractionMultiplier);
			break;
		}

		case HANDL_FTRACTIONLOSS:
		{
			this->ApplyModifier(*i, handling->m_fTractionLoss);
			break;
		}

		case HANDL_FTRACTIONBIAS:
		{
			this->ApplyModifier(*i, handling->m_fTractionBias);
			break;
		}

		case HANDL_TRDATA_NNUMBEROFGEARS:
		{
			this->ApplyModifier(*i, handling->m_transmissionData.m_nNumberOfGears);
			break;
		}

		case HANDL_TRDATA_FMAXVELOCITY:
		{
			this->ApplyModifier(*i, handling->m_transmissionData.m_fMaxGearVelocity);
			break;
		}

		case HANDL_TRDATA_FENGINEACCELERATION:
		{
			this->ApplyModifier(*i, handling->m_transmissionData.m_fEngineAcceleration);
			break;
		}

		case HANDL_TRDATA_FENGINEINERTIA:
		{
			this->ApplyModifier(*i, handling->m_transmissionData.m_fEngineInertia);
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
			this->ApplyModifier(*i, handling->m_fBrakeDeceleration);
			break;
		}

		case HANDL_FBRAKEBIAS:
		{
			this->ApplyModifier(*i, handling->m_fBrakeBias);
			break;
		}

		case HANDL_BABS:
		{
			handling->m_bABS = (atoi(i->value)) ? true : false;
			break;
		}

		case HANDL_FSTEERINGLOCK:
		{
			this->ApplyModifier(*i, handling->m_fSteeringLock);
			break;
		}

		case HANDL_FSUSPENSIONFORCELEVEL:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionForceLevel);
			break;
		}

		case HANDL_FSUSPENSIONDAMPINGLEVEL:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionDampingLevel);
			break;
		}

		case HANDL_FSUSPENSIONHIGHSPDCOMDAMP:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionHighSpdComDamp);
			break;
		}

		case HANDL_FSUSPENSIONUPPERLIMIT:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionUpperLimit);
			break;
		}

		case HANDL_FSUSPENSIONLOWERLIMIT:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionLowerLimit);
			break;
		}

		case HANDL_FSUSPENSIONBIAS:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionBiasBetweenFrontAndRear);
			break;
		}

		case HANDL_FSUSPENSIONANTIDIVEMULT:
		{
			this->ApplyModifier(*i, handling->m_fSuspensionAntiDiveMultiplier);
			break;
		}

		case HANDL_FSEATOFFSETDISTANCE:
		{
			this->ApplyModifier(*i, handling->m_fSeatOffsetDistance);
			break;
		}

		case HANDL_FCOLLISIONDAMAGEMULTIPLIER:
		{
			this->ApplyModifier(*i, handling->m_fCollisionDamageMultiplier);
			break;
		}

		case HANDL_NMONETARYVALUE:
		{
			this->ApplyModifier(*i, handling->m_nMonetaryValue);
			break;
		}

		case HANDL_MODELFLAGS:
		{
			uint32_t temp = handling->m_nModelFlags;
			this->ApplyFlagModifier(*i, temp);
			handling->m_nModelFlags = (eVehicleHandlingModelFlags)temp;
			break;
		}

		case HANDL_HANDLINGFLAGS:
		{

			uint32_t temp = handling->m_nHandlingFlags;
			this->ApplyFlagModifier(*i, temp);
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