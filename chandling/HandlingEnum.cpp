#include "main.h"
#include "game/game.h"
#include "HandlingEnum.h"

CHandlingAttribType GetHandlingAttribType(CHandlingAttrib attribute)
{
	switch (attribute)
	{
	case HANDL_FMASS:
	case HANDL_FTURNMASS:
	case HANDL_FDRAGMULTIPLIER:
	case HANDL_CENTREOFMASS_X:
	case HANDL_CENTREOFMASS_Y:
	case HANDL_CENTREOFMASS_Z:
	case HANDL_FTRACTIONMULTIPLIER:
	case HANDL_FTRACTIONLOSS:
	case HANDL_FTRACTIONBIAS:
	case HANDL_TR_FMAXVELOCITY:
	case HANDL_TR_FENGINEACCELERATION:
	case HANDL_TR_FENGINEINERTIA:
	case HANDL_FBRAKEDECELERATION:
	case HANDL_FBRAKEBIAS:
	case HANDL_FSTEERINGLOCK:
	case HANDL_FSUSPENSIONFORCELEVEL:
	case HANDL_FSUSPENSIONDAMPINGLEVEL:
	case HANDL_FSUSPENSIONHIGHSPDCOMDAMP:
	case HANDL_FSUSPENSIONUPPERLIMIT:
	case HANDL_FSUSPENSIONLOWERLIMIT:
	case HANDL_FSUSPENSIONBIAS:
	case HANDL_FSUSPENSIONANTIDIVEMULT:
	case HANDL_FSEATOFFSETDISTANCE:
	case HANDL_FCOLLISIONDAMAGEMULT:
		return TYPE_FLOAT;

	case HANDL_NPERCENTSUBMERGED:
	case HANDL_ANIMGROUP:
	case HANDL_FRONTLIGHTS:
	case HANDL_REARLIGHTS:
	case HANDL_TR_NDRIVETYPE:
	case HANDL_TR_NENGINETYPE:
	case HANDL_TR_NNUMBEROFGEARS:
	case HANDL_BABS:
		return TYPE_BYTE;

	case HANDL_UIDENTIFIER:
	case HANDL_UIMONETARYVALUE:
		return TYPE_UINT;

	case HANDL_HANDLINGFLAGS:
	case HANDL_MODELFLAGS:
		return TYPE_FLAG;
	}
	return TYPE_NONE;
}

bool CanSetHandlingAttrib(CHandlingAttrib attribute)
{
	switch (attribute)
	{
	case HANDL_UIDENTIFIER:
	case HANDL_ANIMGROUP:
	case HANDL_UIMONETARYVALUE:
		return false;

	}
	return true;
}

void* GetHandlingAttribPtr(struct tHandlingData *handling, CHandlingAttrib attrib)
{
	switch (attrib)
	{
	case HANDL_FMASS:
		return &handling->m_fMass;
	case HANDL_FTURNMASS:
		return &handling->m_fTurnMass;
	case HANDL_FDRAGMULTIPLIER:
		return &handling->m_fDragMult;
	case HANDL_CENTREOFMASS_X:
		return &handling->m_vecCentreOfMass.fX;
	case HANDL_CENTREOFMASS_Y:
		return &handling->m_vecCentreOfMass.fY;
	case HANDL_CENTREOFMASS_Z:
		return &handling->m_vecCentreOfMass.fZ;
	case HANDL_FTRACTIONMULTIPLIER:
		return &handling->m_fTractionMultiplier;
	case HANDL_FTRACTIONLOSS:
		return &handling->m_fTractionLoss;
	case HANDL_FTRACTIONBIAS:
		return &handling->m_fTractionBias;
	case HANDL_TR_FMAXVELOCITY:
		return &handling->m_transmissionData.m_fMaxGearVelocity;
	case HANDL_TR_FENGINEACCELERATION:
		return &handling->m_transmissionData.m_fEngineAcceleration;
	case HANDL_TR_FENGINEINERTIA:
		return &handling->m_transmissionData.m_fEngineInertia;
	case HANDL_FBRAKEDECELERATION:
		return &handling->m_fBrakeDeceleration;
	case HANDL_FBRAKEBIAS:
		return &handling->m_fBrakeBias;
	case HANDL_FSTEERINGLOCK:
		return &handling->m_fSteeringLock;
	case HANDL_FSUSPENSIONFORCELEVEL:
		return &handling->m_fSuspensionForceLevel;
	case HANDL_FSUSPENSIONDAMPINGLEVEL:
		return &handling->m_fSuspensionDampingLevel;
	case HANDL_FSUSPENSIONHIGHSPDCOMDAMP:
		return &handling->m_fSuspensionHighSpdComDamp;
	case HANDL_FSUSPENSIONUPPERLIMIT:
		return &handling->m_fSuspensionUpperLimit;
	case HANDL_FSUSPENSIONLOWERLIMIT:
		return &handling->m_fSuspensionLowerLimit;
	case HANDL_FSUSPENSIONBIAS:
		return &handling->m_fSuspensionBiasBetweenFrontAndRear;
	case HANDL_FSUSPENSIONANTIDIVEMULT:
		return &handling->m_fSuspensionAntiDiveMultiplier;
	case HANDL_FSEATOFFSETDISTANCE:
		return &handling->m_fSeatOffsetDistance;
	case HANDL_FCOLLISIONDAMAGEMULT:
		return &handling->m_fCollisionDamageMultiplier;

	case HANDL_NPERCENTSUBMERGED:
		return &handling->m_nPercentSubmerged;
	case HANDL_ANIMGROUP:
		return &handling->m_nAnimGroup;
	case HANDL_FRONTLIGHTS:
		return &handling->m_nFrontLights;
	case HANDL_REARLIGHTS:
		return &handling->m_nRearLights;
	case HANDL_TR_NDRIVETYPE:
		return &handling->m_transmissionData.m_nDriveType;
	case HANDL_TR_NENGINETYPE:
		return &handling->m_transmissionData.m_nEngineType;
	case HANDL_TR_NNUMBEROFGEARS:
		return &handling->m_transmissionData.m_nNumberOfGears;
	case HANDL_BABS:
		return &handling->m_bABS;

	case HANDL_UIDENTIFIER:
		return &handling->m_iIndex;
	case HANDL_UIMONETARYVALUE:
		return &handling->m_nMonetaryValue;

	case HANDL_HANDLINGFLAGS:
		return &handling->m_nHandlingFlags;
	case HANDL_MODELFLAGS:
		return &handling->m_nModelFlags;

	}
	return  nullptr;
}