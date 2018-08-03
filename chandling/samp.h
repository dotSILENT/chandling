#pragma once
#include <cstdint>
#include <windef.h>
#include <string>
#include "game\game.h"

enum Limits
{
	SAMP_MAX_ACTORS = 1000,
	SAMP_MAX_PLAYERS = 1004,
	SAMP_MAX_VEHICLES = 2000,
	SAMP_MAX_PICKUPS = 4096,
	SAMP_MAX_OBJECTS = 1000,
	SAMP_MAX_GANGZONES = 1024,
	SAMP_MAX_3DTEXTS = 2048,
	SAMP_MAX_TEXTDRAWS = 2048,
	SAMP_MAX_PLAYERTEXTDRAWS = 256,
	SAMP_MAX_CLIENTCMDS = 144,
	SAMP_MAX_MENUS = 128,
	SAMP_MAX_PLAYER_NAME = 24,
	SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20,
};

#pragma pack(push, 1)
struct stSAMPPools
{
	struct stActorPool		*pActor;
	struct stObjectPool		*pObject;
	struct stGangzonePool	*pGangzone;
	struct stTextLabelPool	*pText3D;
	struct stTextdrawPool	*pTextdraw;
	void					*pPlayerLabels;
	struct stPlayerPool		*pPlayer;
	struct stVehiclePool	*pVehicle;
	struct stPickupPool		*pPickup;
};

struct stSAMP
{
	void					*pUnk0;
	struct stServerInfo			*pServerInfo;
	uint8_t					byteSpace[24+12];
	char					szIP[257];
	char					szHostname[259];
	bool					bNametagStatus; // changes by /nametagstatus
	uint32_t				ulPort;
	uint32_t				ulMapIcons[100];
	int						iLanMode;
	int						iGameState;
	uint32_t				ulConnectTick; // it's good up to this place
	struct stServerPresets	*pSettings;
	void					*pRakClientInterface;
	uint8_t					pad[7];
	struct stSAMPPools		*pPools;
};


struct stServerInfo {
	uint32_t 			uiIP;
	uint16_t 			usPort;

	// ...

};

struct stServerPresets
{
	uint8_t byteCJWalk;
	uint8_t byteUnk0[4];
	float	fWorldBoundaries[4];
	uint8_t byteUnk1;
	float	fGravity;
	uint8_t byteDisableInteriorEnterExits;
	uint32_t ulVehicleFriendlyFire;
	uint8_t byteUnk2[4];
	int		iClassesAvailable;
	float	fNameTagsDistance;
	uint8_t byteUnk3;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	uint8_t bytePlayerMarkersMode;
	uint8_t byteUnk4[3];
	float	fGlobalChatRadiusLimit;
	uint8_t byteShowNameTags;
};


/* POOLS */

template <typename T>
struct stSAMPEntity
{
	void		*pVTBL;
	uint8_t		byteUnk0[60]; // game CEntity object maybe. always empty.
	T			*pGTAEntity;
	uint32_t	ulGTAEntityHandle;
};

struct stPickup
{
	int		iModelID;
	int		iType;
	float	fPosition[3];
};

struct stPickupPool
{
	int				iPickupsCount;
	uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
	int				iPickupID[SAMP_MAX_PICKUPS];
	int				iTimePickup[SAMP_MAX_PICKUPS];
	uint8_t			unk[SAMP_MAX_PICKUPS * 3];
	struct stPickup pickup[SAMP_MAX_PICKUPS];
};

struct stPlayerPool
{
	uint32_t				ulMaxPlayerID;
	uint16_t				sLocalPlayerID;
	void					*pVTBL_txtHandler;
	std::string				strLocalPlayerName;
	struct stLocalPlayer	*pLocalPlayer;
	int						iLocalPlayerPing;
	int						iLocalPlayerScore;
	struct stRemotePlayer	*pRemotePlayer[SAMP_MAX_PLAYERS];
	int						iIsListed[SAMP_MAX_PLAYERS];
	DWORD						dwPlayerIP[SAMP_MAX_PLAYERS]; // always 0
};

struct stVehiclePool
{
	int						iVehicleCount;
	void					*pUnk0;
	uint8_t					byteSpace1[0x112C];
	struct stSAMPVehicle	*pSAMP_Vehicle[SAMP_MAX_VEHICLES];
	int						iIsListed[SAMP_MAX_VEHICLES];
	struct vehicle_info		*pGTA_Vehicle[SAMP_MAX_VEHICLES];
	uint8_t					byteSpace2[SAMP_MAX_VEHICLES * 6];
	uint32_t				ulShit[SAMP_MAX_VEHICLES];
	int						iIsListed2[SAMP_MAX_VEHICLES];
	uint32_t				byteSpace3[SAMP_MAX_VEHICLES * 2];
	float					fSpawnPos[SAMP_MAX_VEHICLES][3];
	int						iInitiated;
};

struct stSAMPVehicle : public stSAMPEntity < vehicle_info >
{
	uint32_t			bUnk0;
	struct vehicle_info *pGTA_Vehicle;
	uint8_t				byteUnk1[8];
	int					bIsMotorOn;
	int					iIsLightsOn;
	int					iIsLocked;
	uint8_t				byteIsObjective;
	int					iObjectiveBlipCreated;
	uint8_t				byteUnk2[16];
	uint8_t				byteColor[2];
	int					iColorSync;
	int					iColor_something;
};

struct stObject : public stSAMPEntity < object_info >
{
	uint8_t				byteUnk0[2];
	uint32_t			ulUnk1;
	int					iModel;
	uint16_t			byteUnk2;
	float				fDrawDistance;
	float				fUnk;
	float				fPos[3];
	uint8_t				byteUnk3[68];
	uint8_t				byteUnk4;
	float				fRot[3];
	// ...
};

struct stObjectPool
{
	int				iObjectCount;
	int				iIsListed[SAMP_MAX_OBJECTS];
	struct stObject *object[SAMP_MAX_OBJECTS];
};

struct stGangzone
{
	float	fPosition[4];
	DWORD	dwColor;
	DWORD	dwAltColor;
};

struct stGangzonePool
{
	struct stGangzone	*pGangzone[SAMP_MAX_GANGZONES];
	int					iIsListed[SAMP_MAX_GANGZONES];
};

struct stTextLabel
{
	char		*pText;
	DWORD		color;
	float		fPosition[3];
	float		fMaxViewDistance;
	uint8_t		byteShowBehindWalls;
	uint16_t	sAttachedToPlayerID;
	uint16_t	sAttachedToVehicleID;
};

struct stTextLabelPool
{
	struct stTextLabel	textLabel[SAMP_MAX_3DTEXTS];
	int					iIsListed[SAMP_MAX_3DTEXTS];
};

#pragma pack(pop)