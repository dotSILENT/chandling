#pragma once
#include <cstdint>
#include <windef.h>

#include "CVector.h"
#include "CSomeClassess.h"

/*
Handling shite we need
*/

struct  tTransmissionGear
{
	float m_fMaxVelocity;
	float m_fChangeUpVelocity;
	float m_fChangeDownVelocity;
};

class cTransmission {
public:
	tTransmissionGear m_aGears[6];
	unsigned char m_nDriveType; // F/R/4
	unsigned char m_nEngineType; // P/D/E
	unsigned char m_nNumberOfGears; // 1 to 6
	char field_4B;
	unsigned int  m_nHandlingFlags;
	float         m_fEngineAcceleration; // 0.1 to 10.0
	float         m_fEngineInertia; // 0.0 to 50.0
	float         m_fMaxGearVelocity; // 5.0 to 150.0
	int field_5C;
	float         m_fMinGearVelocity;
	float         m_fCurrentSpeed;

	/*cTransmission();
	void InitGearRatios();
	void CalculateGearForSimpleCar(float velocity, unsigned char& currrentGear);
	// it uses printf, so you won't see it actually...
	void DisplayGearRatios();
	float CalculateDriveAcceleration(float& gasPedal, unsigned char& currrentGear, float& gearChangeCount, float& speed, float& unk1, float& unk2, bool allWheelsOnGround, unsigned char handlingType);
	*/
};

enum eVehicleLightsSize : unsigned char {
	LIGHTS_LONG,
	LIGHTS_SMALL,
	LIGHTS_BIG,
	LIGHTS_TALL
};

enum eVehicleHandlingFlags : unsigned int {
	VEHICLE_HANDLING_1G_BOOST = 0x1,
	VEHICLE_HANDLING_2G_BOOST = 0x2,
	VEHICLE_HANDLING_NPC_ANTI_ROLL = 0x4,
	VEHICLE_HANDLING_NPC_NEUTRAL_HANDL = 0x8,
	VEHICLE_HANDLING_NO_HANDBRAKE = 0x10,
	VEHICLE_HANDLING_STEER_REARWHEELS = 0x20,
	VEHICLE_HANDLING_HB_REARWHEEL_STEER = 0x40,
	VEHICLE_HANDLING_ALT_STEER_OPT = 0x80,
	VEHICLE_HANDLING_WHEEL_F_NARROW2 = 0x100,
	VEHICLE_HANDLING_WHEEL_F_NARROW = 0x200,
	VEHICLE_HANDLING_WHEEL_F_WIDE = 0x400,
	VEHICLE_HANDLING_WHEEL_F_WIDE2 = 0x800,
	VEHICLE_HANDLING_WHEEL_R_NARROW2 = 0x1000,
	VEHICLE_HANDLING_WHEEL_R_NARROW = 0x2000,
	VEHICLE_HANDLING_WHEEL_R_WIDE = 0x4000,
	VEHICLE_HANDLING_WHEEL_R_WIDE2 = 0x8000,
	VEHICLE_HANDLING_HYDRAULIC_GEOM = 0x10000,
	VEHICLE_HANDLING_HYDRAULIC_INST = 0x20000,
	VEHICLE_HANDLING_HYDRAULIC_NONE = 0x40000,
	VEHICLE_HANDLING_NOS_INST = 0x80000,
	VEHICLE_HANDLING_OFFROAD_ABILITY = 0x100000,
	VEHICLE_HANDLING_OFFROAD_ABILITY2 = 0x200000,
	VEHICLE_HANDLING_HALOGEN_LIGHTS = 0x400000,
	VEHICLE_HANDLING_PROC_REARWHEEL_1ST = 0x800000,
	VEHICLE_HANDLING_USE_MAXSP_LIMIT = 0x1000000,
	VEHICLE_HANDLING_LOW_RIDER = 0x2000000,
	VEHICLE_HANDLING_STREET_RACER = 0x4000000,
	VEHICLE_HANDLING_SWINGING_CHASSIS = 0x10000000
};

enum eVehicleHandlingModelFlags : unsigned int {
	VEHICLE_HANDLING_MODEL_IS_VAN = 0x1,
	VEHICLE_HANDLING_MODEL_IS_BUS = 0x2,
	VEHICLE_HANDLING_MODEL_IS_LOW = 0x4,
	VEHICLE_HANDLING_MODEL_IS_BIG = 0x8,
	VEHICLE_HANDLING_MODEL_REVERSE_BONNET = 0x10,
	VEHICLE_HANDLING_MODEL_HANGING_BOOT = 0x20,
	VEHICLE_HANDLING_MODEL_TAILGATE_BOOT = 0x40,
	VEHICLE_HANDLING_MODEL_NOSWING_BOOT = 0x80,
	VEHICLE_HANDLING_MODEL_NO_DOORS = 0x100,
	VEHICLE_HANDLING_MODEL_TANDEM_SEATS = 0x200,
	VEHICLE_HANDLING_MODEL_SIT_IN_BOAT = 0x400,
	VEHICLE_HANDLING_MODEL_CONVERTIBLE = 0x800,
	VEHICLE_HANDLING_MODEL_NO_EXHAUST = 0x1000,
	VEHICLE_HANDLING_MODEL_DOUBLE_EXHAUST = 0x2000,
	VEHICLE_HANDLING_MODEL_NO1FPS_LOOK_BEHIND = 0x4000,
	VEHICLE_HANDLING_MODEL_FORCE_DOOR_CHECK = 0x8000,
	VEHICLE_HANDLING_MODEL_AXLE_F_NOTILT = 0x10000,
	VEHICLE_HANDLING_MODEL_AXLE_F_SOLID = 0x20000,
	VEHICLE_HANDLING_MODEL_AXLE_F_MCPHERSON = 0x40000,
	VEHICLE_HANDLING_MODEL_AXLE_F_REVERSE = 0x80000,
	VEHICLE_HANDLING_MODEL_AXLE_R_NOTILT = 0x100000,
	VEHICLE_HANDLING_MODEL_AXLE_R_SOLID = 0x200000,
	VEHICLE_HANDLING_MODEL_AXLE_R_MCPHERSON = 0x400000,
	VEHICLE_HANDLING_MODEL_AXLE_R_REVERSE = 0x800000,
	VEHICLE_HANDLING_MODEL_IS_BIKE = 0x1000000,
	VEHICLE_HANDLING_MODEL_IS_HELI = 0x2000000,
	VEHICLE_HANDLING_MODEL_IS_PLANE = 0x4000000,
	VEHICLE_HANDLING_MODEL_IS_BOAT = 0x8000000,
	VEHICLE_HANDLING_MODEL_BOUNCE_PANELS = 0x10000000,
	VEHICLE_HANDLING_MODEL_DOUBLE_RWHEELS = 0x20000000,
	VEHICLE_HANDLING_MODEL_FORCE_GROUND_CLEARANCE = 0x40000000,
	VEHICLE_HANDLING_MODEL_IS_HATCHBACK = 0x80000000
};

struct tHandlingData {
	int           m_iIndex;
	float         m_fMass; // 1.0 to 50000.0
	float field_8;
	float         m_fTurnMass;
	float         m_fDragMult;
	CVector       m_vecCentreOfMass; // x, y, z - 1.0 to 50000.0
	uint8_t m_nPercentSubmerged; // 10 to 120 (> 100% vehicle sinks)
	float         m_fBuoyancyConstant;
	float         m_fTractionMultiplier; // 0.5 to 2.0
	cTransmission m_transmissionData;
	float         m_fBrakeDeceleration; // 0.1 to 10.0
	float         m_fBrakeBias; // 0.0 > x > 1.0
	char          m_bABS; // 0/1
	char field_9D;
	char field_9E;
	char field_9F;
	float         m_fSteeringLock; // 10.0 to 40.0
	float         m_fTractionLoss;
	float         m_fTractionBias;
	float         m_fSuspensionForceLevel; // not [L/M/H]
	float         m_fSuspensionDampingLevel; // not [L/M/H]
	float         m_fSuspensionHighSpdComDamp; // often zero - 200.0 or more for bouncy vehicles
	float         m_fSuspensionUpperLimit;
	float         m_fSuspensionLowerLimit;
	float         m_fSuspensionBiasBetweenFrontAndRear;
	float         m_fSuspensionAntiDiveMultiplier;
	float         m_fCollisionDamageMultiplier; // 0.2 to 5.0
	union {
		eVehicleHandlingModelFlags m_nModelFlags;
		struct {
			unsigned int m_bIsVan : 1;
			unsigned int m_bIsBus : 1;
			unsigned int m_bIsLow : 1;
			unsigned int m_bIsBig : 1;
			unsigned int m_bReverseBonnet : 1;
			unsigned int m_bHangingBoot : 1;
			unsigned int m_bTailgateBoot : 1;
			unsigned int m_bNoswingBoot : 1;
			unsigned int m_bNoDoors : 1;
			unsigned int m_bTandemSeats : 1;
			unsigned int m_bSitInBoat : 1;
			unsigned int m_bConvertible : 1;
			unsigned int m_bNoExhaust : 1;
			unsigned int m_bDoubleExhaust : 1;
			unsigned int m_bNo1fpsLookBehind : 1;
			unsigned int m_bForceDoorCheck : 1;
			unsigned int m_bAxleFNotlit : 1;
			unsigned int m_bAxleFSolid : 1;
			unsigned int m_bAxleFMcpherson : 1;
			unsigned int m_bAxleFReverse : 1;
			unsigned int m_bAxleRNotlit : 1;
			unsigned int m_bAxleRSolid : 1;
			unsigned int m_bAxleRMcpherson : 1;
			unsigned int m_bAxleRReverse : 1;
			unsigned int m_bIsBike : 1;
			unsigned int m_bIsHeli : 1;
			unsigned int m_bIsPlane : 1;
			unsigned int m_bIsBoat : 1;
			unsigned int m_bBouncePanels : 1;
			unsigned int m_bDoubleRwheels : 1;
			unsigned int m_bForceGroundClearance : 1;
			unsigned int m_bIsHatchback : 1;
		};
	};
	union {
		eVehicleHandlingFlags m_nHandlingFlags;
		struct {
			unsigned int m_b1gBoost : 1;
			unsigned int m_b2gBoost : 1;
			unsigned int m_bNpcAntiRoll : 1;
			unsigned int m_bNpcNeutralHandl : 1;
			unsigned int m_bNoHandbrake : 1;
			unsigned int m_bSteerRearwheels : 1;
			unsigned int m_bHbRearwheelSteer : 1;
			unsigned int m_bAltSteerOpt : 1;
			unsigned int m_bWheelFNarrow2 : 1;
			unsigned int m_bWheelFNarrow : 1;
			unsigned int m_bWheelFWide : 1;
			unsigned int m_bWheelFWide2 : 1;
			unsigned int m_bWheelRNarrow2 : 1;
			unsigned int m_bWheelRNarrow : 1;
			unsigned int m_bWheelRWide : 1;
			unsigned int m_bWheelRWide2 : 1;
			unsigned int m_bHydraulicGeom : 1;
			unsigned int m_bHydraulicInst : 1;
			unsigned int m_bHydraulicNone : 1;
			unsigned int m_bNosInst : 1;
			unsigned int m_bOffroadAbility : 1;
			unsigned int m_bOffroadAbility2 : 1;
			unsigned int m_bHalogenLights : 1;
			unsigned int m_bProcRearwheelFirst : 1;
			unsigned int m_bUseMaxspLimit : 1;
			unsigned int m_bLowRider : 1;
			unsigned int m_bStreetRacer : 1;
			unsigned int m_bSwingingChassis : 1;
		};
	};
	float              m_fSeatOffsetDistance; // // ped seat position offset towards centre of car
	unsigned int       m_nMonetaryValue; // 1 to 100000
	eVehicleLightsSize m_nFrontLights;
	eVehicleLightsSize m_nRearLights;
	unsigned char      m_nAnimGroup;
};


struct CTrainFlags
{
	unsigned char unknown1 : 3;
	unsigned char bIsTheChainEngine : 1; // Only the first created train on the chain gets this set to true, others get it set to false.
	unsigned char unknown2 : 1; // This is always set to true in mission trains construction.
	unsigned char bIsAtNode : 1;
	unsigned char bDirection : 1;
	unsigned char unknown3 : 1; // If the mission train was placed at the node, this is set to false in construction.

	unsigned char bIsDerailed : 1;
	unsigned char unknown4 : 1;
	unsigned char bIsDrivenByBrownSteak : 1;
	unsigned char unknown5 : 5;

	unsigned char unknown6 : 8;

	unsigned char unknown7 : 8;
};

struct CVehicleFlags
{
	unsigned char bIsLawEnforcer : 1; // Is this guy chasing the player at the moment
	unsigned char bIsAmbulanceOnDuty : 1; // Ambulance trying to get to an accident
	unsigned char bIsFireTruckOnDuty : 1; // Firetruck trying to get to a fire
	unsigned char bIsLocked : 1; // Is this guy locked by the script (cannot be removed)
	unsigned char bEngineOn : 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
	unsigned char bIsHandbrakeOn : 1; // How's the handbrake doing ?
	unsigned char bLightsOn : 1; // Are the lights switched on ?
	unsigned char bFreebies : 1; // Any freebies left in this vehicle ?

	unsigned char bIsVan : 1; // Is this vehicle a van (doors at back of vehicle)
	unsigned char bIsBus : 1; // Is this vehicle a bus
	unsigned char bIsBig : 1; // Is this vehicle a bus
	unsigned char bLowVehicle : 1; // Need this for sporty type cars to use low getting-in/out anims
	unsigned char bComedyControls : 1; // Will make the car hard to control (hopefully in a funny way)
	unsigned char bWarnedPeds : 1; // Has scan and warn peds of danger been processed?
	unsigned char bCraneMessageDone : 1; // A crane message has been printed for this car allready
										 // unsigned char bExtendedRange: 1; // This vehicle needs to be a bit further away to get deleted
	unsigned char bTakeLessDamage : 1; // This vehicle is stronger (takes about 1/4 of damage)

	unsigned char bIsDamaged : 1; // This vehicle has been damaged and is displaying all its components
	unsigned char bHasBeenOwnedByPlayer : 1;// To work out whether stealing it is a crime
	unsigned char bFadeOut : 1; // Fade vehicle out
	unsigned char bIsBeingCarJacked : 1; // Fade vehicle out
	unsigned char bCreateRoadBlockPeds : 1;// If this vehicle gets close enough we will create peds (coppers or gang members) round it
	unsigned char bCanBeDamaged : 1; // Set to FALSE during cut scenes to avoid explosions
									 // unsigned char bUsingSpecialColModel : 1;
									 // Is player vehicle using special collision model, stored in player strucure
	unsigned char bOccupantsHaveBeenGenerated : 1; // Is true if the occupants have already been generated. (Shouldn't happen again)
	unsigned char bGunSwitchedOff : 1; // Level designers can use this to switch off guns on boats

	unsigned char bVehicleColProcessed : 1;// Has ProcessEntityCollision been processed for this car?
	unsigned char bIsCarParkVehicle : 1; // Car has been created using the special CAR_PARK script command
	unsigned char bHasAlreadyBeenRecorded : 1; // Used for replays
	unsigned char bPartOfConvoy : 1;
	unsigned char bHeliMinimumTilt : 1; // This heli should have almost no tilt really
	unsigned char bAudioChangingGear : 1; // sounds like vehicle is changing gear
	unsigned char bIsDrowning : 1; // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
	unsigned char bTyresDontBurst : 1; // If this is set the tyres are invincible

	unsigned char bCreatedAsPoliceVehicle : 1;// True if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
	unsigned char bRestingOnPhysical : 1; // Dont go static cause car is sitting on a physical object that might get removed
	unsigned char      bParking : 1;
	unsigned char      bCanPark : 1;
	unsigned char bFireGun : 1; // Does the ai of this vehicle want to fire it's gun?
	unsigned char bDriverLastFrame : 1; // Was there a driver present last frame ?
	unsigned char bNeverUseSmallerRemovalRange : 1;// Some vehicles (like planes) we don't want to remove just behind the camera.
	unsigned char bIsRCVehicle : 1; // Is this a remote controlled (small) vehicle. True whether the player or AI controls it.

	unsigned char bAlwaysSkidMarks : 1; // This vehicle leaves skidmarks regardless of the wheels' states.
	unsigned char bEngineBroken : 1; // Engine doesn't work. Player can get in but the vehicle won't drive
	unsigned char bVehicleCanBeTargetted : 1;// The ped driving this vehicle can be targetted, (for Torenos plane mission)
	unsigned char bPartOfAttackWave : 1; // This car is used in an attack during a gang war
	unsigned char bWinchCanPickMeUp : 1; // This car cannot be picked up by any ropes.
	unsigned char bImpounded : 1; // Has this vehicle been in a police impounding garage
	unsigned char bVehicleCanBeTargettedByHS : 1;// Heat seeking missiles will not target this vehicle.
	unsigned char bSirenOrAlarm : 1; // Set to TRUE if siren or alarm active, else FALSE

	unsigned char bHasGangLeaningOn : 1;
	unsigned char bGangMembersForRoadBlock : 1;// Will generate gang members if NumPedsForRoadBlock > 0
	unsigned char bDoesProvideCover : 1; // If this is false this particular vehicle can not be used to take cover behind.
	unsigned char bMadDriver : 1; // This vehicle is driving like a lunatic
	unsigned char bUpgradedStereo : 1; // This vehicle has an upgraded stereo
	unsigned char bConsideredByPlayer : 1; // This vehicle is considered by the player to enter
	unsigned char bPetrolTankIsWeakPoint : 1;// If false shootong the petrol tank will NOT Blow up the car
	unsigned char bDisableParticles : 1; // Disable particles from this car. Used in garage.

	unsigned char bHasBeenResprayed : 1; // Has been resprayed in a respray garage. Reset after it has been checked.
	unsigned char bUseCarCheats : 1; // If this is true will set the car cheat stuff up in ProcessControl()
	unsigned char bDontSetColourWhenRemapping : 1;// If the texture gets remapped we don't want to change the colour with it.
	unsigned char bUsedForReplay : 1; // This car is controlled by replay and should be removed when replay is done.
};

//Only for flying
struct physicalFlyParams
{
#pragma pack( 1 )
	float	__itsNull;	/* 0 */
	float	heightIncreaseRel[2];	/* 4 - Related to (Heli only?) height */
	float	circleAround;			/* 12 - Heli/Plane */
	float	changeDir;		/* 16 - Speed changing Direction (negative = change against the speed vector) */
	float	changeDir_side; /* 20 */
	float	roll_lr;		/* 24 */
	float	__unknown_28;	/* 28 */
	float	pitch;			/* 32 */
	float	__unknown_36;	/* 36 */
	float	_multPitch;		/* 40 - pitch related */
	float	finalPitch_;	/* 44 */
	float	_unknownMultipliers_isOne[2];	/* 48 */
	float	_orientationMultiplier;			/* 56 - somehow orientation related */
	float	_SpeedMultiplier_isOne;			/* 60 */
	float	pitchMultiplier;	/* 64 - keep it >0 */
	float	rollMultiplier;		/* 68 */
	float	circleMultiplier;	/* 72 */
								//following values = global for all vehs (?)
};

struct detachable
{
#pragma pack( 1 )
	float	position[3];
	uint8_t __unknown_12[4];
	float	vector[3];
	uint8_t __unknown_28[16];
};

struct vehicle_info
{
#pragma pack( 1 )
	uint8_t padd[34];
	WORD model;
	uint8_t pad[30];
	uint8_t				flags;			/* 66 - flags */
	uint8_t				__unknown_67;	/* 67 */
	union	/* 68 */
	{
		struct
		{
			CVector m_SpeedVec;
		};
		float	speed[3];
	};
	union	/* 80 */
	{
		struct
		{
			CVector m_SpinVec;
		};
		float	spin[3];
	};
	float						speed_rammed[3];	/* 92 - speed from collision, will be added to speed[3] */
	float						spin_rammed[3];		/* 104 - spin from collision, will be added to spin[3] */

	uint8_t						__unknown_116[20];	/* 116 */

													// handling should start here // actually it's just CPhysical, handling is taken from tHandlingData pointer
	uint8_t						__unknown_136[4];	/* 136 */
	float						mass;		/* 140 - vehicle mass from handling.cfg */
	float						turn_mass;	/* 144 - turn mass */
	float						grip_div;	/* 148 - grip divisor */
	float						mass_to_grip_mult;		/* 152 - mass to grip multiplier */
	float						fTurnMass;				/* 156 - 0.05 or 0.1 */
	float						grip_level_norm;		/* 160 - normalized grip level */
	float						center_of_mass[3];		/* 164 - center of mass */
	void						*__unknown_176;			/* 176 - pointer to a "entry node info" pool item */
	void						*__unknown_180;			/* 180 - pointer to a "ptr node Double" pool item */

														//collision data
	DWORD						collision_flags;		/* 184 - 2nd byte = currently colliding 1/0, or actively
														running against (0x02), being moved/rammed (0x03), 1st byte = 0, if noone inside and colliding
														with actor, else ever 0x0a*/
	void						*last_touched_object;	/* 188 - You can touch roads - those are considered buildings */
	void						*last_collided_object;	/* 192 - pointer to last collided object.. not for ALL collisions?!? */
	uint8_t						__unknown_196[20];		/* 196 */
	float						collision_something;	/* 216 - collision timer?*/
	void						*collision_current_obj; /* 220 - pointer to current thing colliding with */

														// end of handling should be here
	uint8_t						__unknown_224[12];		/* 224 */
	float						collision_position[3];	/* 236 - last collision coordinates */
	uint8_t						__unknown_248[68];		/* 248 */
	void						*__unknown_316;			/* 316 - pointer to this something in this structure */
	uint8_t						__unknown_320[157];		/* 320 */
	uint8_t						in_vehicle;			/* 477 */
	uint8_t						__unknown_478[422]; /* 478 */

	tHandlingData				*pHandlingData;		/* 900 */
	struct physicalFlyParams	*pFlyData;			/* 904 */
	uint8_t						__unknown_908[96];	/* 908 */
	uint32_t					hFlagsLocal;		/* 1004 */
	uint8_t						__unknown_1008[56]; /* 1008 - AutoPilot */
	CVehicleFlags				m_nVehicleFlags;	/* 1064 - 60 bits/flags */
	uint32_t					m_TimeOfCreation;	/* 1072 - GetTimeInMilliseconds when this vehicle was created. */

	uint8_t						color[4];			/* 1076 - as in carcolors.dat - body, stripe, body2, stripe2 */
	uint8_t						modding[2];			/* 1080 - modding data */
	unsigned short				UpgradeModelID[15];	/* 1082 - attached upgrades */
	float						wheel_size;			/* 1112 */
	unsigned short				CarAlarmState;		/* 1116 - time left for car alarm to sound in ms */
	unsigned short				ForcedRandomSeed;	/* 1118 - if this is non-zero the random wander gets deterministic */
	struct actor_info			*passengers[9];		/* 1120 (pointer to the passenger player structs) */

	unsigned char				m_nNumPassengers;	/* 1156 */
	unsigned char				m_nNumGettingIn;	/* 1157 */
	unsigned char				m_nGettingInFlags;	/* 1158 */
	unsigned char				m_nGettingOutFlags; /* 1159 */
	unsigned char				m_nMaxPassengers;	/* 1160 */
	unsigned char				m_windowsOpenFlags; /* 1161 */
	char						m_nNitroBoosts;		/* 1162 */
	char						m_nSpecialColModel; /* 1163 */
	DWORD						*pEntityWeAreOnForVisibilityCheck;	/* 1164 - we need a CEntity */
	DWORD						*m_pFire;			/* 1168 - CFire*/

	float						steer_angles[2];	/* 1172 - steer angles */
	float						gas_pedal;			/* 1180 - gas pedal */
	float						break_pedal;		/* 1184 - break pedal */

													// 1188
	unsigned char				VehicleCreatedBy;	// Contains information on whether this vehicle can be deleted or not. Probably only need this or IsLocked.
	short						ExtendedRemovalRange;

	// this padding probably isn't in the right place.
	uint8_t						__unknown_1191;

	//1192
	unsigned char				car_bomb : 3;		// 0 = None. 1 = Timed. 2 = On ignition, 3 = remotely set ? 4 = Timed Bomb has been activated. 5 = On ignition has been activated.
	unsigned char				OverrideLights : 2; // uses enum NO_CAR_LIGHT_OVERRIDE, FORCE_CAR_LIGHTS_OFF, FORCE_CAR_LIGHTS_ON
	unsigned char				WinchType : 2;		// Does this vehicle use a winch?

													// this padding probably isn't in the right place.
	uint8_t						__unknown_1193;

	//1194
	unsigned char				m_GunsCycleIndex : 2;		// Cycle through alternate gun hardpoints on planes/helis
	unsigned char				m_OrdnanceCycleIndex : 2;	// Cycle through alternate ordnance hardpoints on planes/helis

															// 1195
	unsigned char				nUsedForCover;			// Has n number of cops hiding/attempting to hid behind it
	unsigned char				AmmoInClip;				// Used to make the guns on boat do a reload.
	unsigned char				PacMansCollected;
	unsigned char				PedsPositionForRoadBlock;
	unsigned char				NumPedsForRoadBlock;

	//1200
	float						nBodyDirtLevel;			// Dirt level of vehicle body texture: 0.0f=fully clean, 15.0f=maximum dirt visible, it may be altered at any time while vehicle's cycle of lige

														// values used by transmission
	unsigned char				m_nCurrentGear;			/* 1204 */
	float						m_fGearChangeCount;		/* 1205 */
	float						m_fWheelSpinForAudio;	/* 1209 */
	uint8_t						__unknown_1213[3];		/* 1213 */

	float						hitpoints;			/* 1216 */
	void						*pulling_truck;		/* 1220 - vehicle_info - pulling truck */
	struct vehicle_info			*trailer;			/* 1224 - pointer to the currently attached trailer; 0 if theres no trailer */
	uint8_t						__unknown_1228[44]; /* 1228 */
	uint32_t					door_status;		/* 1272 - car door status (1:Opened 2:Locked) */
	uint8_t						__unknown_1276[148];	/* 1276 */
	uint8_t						vehicle_type;			/* 1424 - VEHICLE_TYPE_* */
	uint8_t						__unknown_1425[19];		/* 1425 */

	union
	{
		struct
		{
			float	m_fTrainSpeed;			/* 1444 - Train speed along rails */
			float	m_fTrainRailDistance;	/* 1448 - Distance along rail starting from first rail node (determines train position when on rails) -max 18106 */
			float	m_fDistanceToNextCarriage;		/* 1452 - Distance to Carriage being infront of this */
		};
		struct
		{
			uint8_t boat_sound_level;				/* 1444 */
			uint8_t car_tire_status[4];				/* 1445 - LF, LR, RF, RR, 0 = ok, 1 = flat, 2 = landing gear up */
			uint8_t door_damage_status[6];			/* 1449 - damage status of doors, 0 = ok, 2 = damaged, 3 = open free, 4 = lost */
		};
	};

	// light damage states
	//uint8_t			lights;						/* 1456 - Light status, 0 = no dmg, 1 = front left dmg, 4 = front right dmg, 5 = front dmg, ...+0x40 = ...+back dmg */
	unsigned char		lights_status_frontLeft : 1;
	unsigned char		__unknown_lights_status_2 : 1;
	unsigned char		lights_status_frontRight : 1;
	unsigned char		__unknown_lights_status_4 : 3;
	unsigned char		lights_status_rear : 1;

	// end of light damage states
	uint8_t				__unknown_1457[3];			/* 1457 - 1475 related to doorHood (byte??)*/

													// misc damage states
	unsigned char		car_status_left : 4;		/* 1460 - 0 = ok, 1 = damaged, 2 = free movement, 3 = lost - same status codes for following bumper/windshield */
	unsigned char		car_status_right : 4;
	uint8_t				car_bumper_status;			/* 1461 - unknown */
	unsigned char		car_windshield_status : 4;
	unsigned char		car_bumper_status_front : 4;
	uint8_t				car_bumper_status_rear;
	// end of misc damage states

	CTrainFlags			m_trainFlags;				/* 1464 */
	uint8_t				__unknown_1468[8];			/* 1468 */
	float				doorHood_dist;				/* 1476 - positive = open */
	uint8_t				__unknown_1480[4];			/* 1480 - detachable related*/
	float				doorHood_dist_two;			/* 1484 - positive = open*/
	uint8_t				__unknown_1488[4];			/* 1488 */
	struct vehicle_info *m_train_next_carriage;		/* 1492 */
	uint8_t				__unknown_1496[4];			/* 1496 */
	float				doorTrunk_dist;				/* 1500 - negative = open */
	uint8_t				__unknown_1504[4];			/* 1504 */
	float				doorTrunk_dist_two;			/* 1508 - negative = open */
	uint8_t				__unknown_1512[12];			/* 1512 */
	float				doorOne_dist;				/* 1524 - also for movable part of boats (marquis)*/
	uint8_t				__unknown_1528[4];			/* 1528 */
	union
	{
		struct detachable	detachable_bike1[1];	/* 1532 - bike/motorcycle/car part */
		struct
		{
			float	doorOne_dist_two;			/* 1532 - negative = open */
			uint8_t __unknown_1536[12];			/* 1536 */
			float	doorTwo_dist;				/* 1548 - positive = open */
			uint8_t __unknown_1552[4];			/* 1552 */
			float	doorTwo_dist_two;			/* 1556  - positive = open */
			uint8_t __unknown_1560[12];			/* 1560 */
			float	doorThree_dist;				/* 1572 - negative = open */
			uint8_t __unknown_1576[4];			/* 1576 */
			float	doorThree_dist_two;			/* 1580 - negative = open */
			uint8_t __unknown_1584[12];			/* 1584 */
			float	doorFour_dist;				/* 1596 - positive = open */
			uint8_t __unknown_1600[6];			/* 1600 */
			float	doorFour_dist_two;			/* 1606 - positive = open */
		};
	};
	uint8_t				__unknown_1610[18];		/* 1610 */
	uint8_t				bike_tire_status[2];	/* 1628 Front, Rear, 0 = ok, 1 = flat */
	uint8_t				__unknown_1630[2];		/* 1630 */

												//maybe also door positions in here, like in 15xx?
	struct detachable	detachable_bike2[4];	/* 1632 - bike/motorcycle and boat parts */
	uint8_t				__unknown_1808[8];		/* 1808 */
	float				fPos_FireCarLadder[3];	/* 1816 - train ladder positioning (part of detachables?) */
	struct detachable	detachable_car[4];		/* 1828 - car/helicopter/airplane parts */
	uint8_t				__unknown_2004[12];		/* 2004 */
	float				suspension[4];			/* 2016 - FL, RL, FR, RR suspension height */
	uint8_t				__unknown_2032[244];	/* 2032 */
	float				burn_timer;				/* 2276 - burn timer counting up from 0.0f */
	uint8_t				__unknown_2280[100];	/* 2280 */
	float				fCannonAimX_Y[2];		/* 2380 - cannon offset (x/y) */
	uint8_t				__unknown_2388[48];		/* 2388 */
	float				fHeliDistanceToGround;	/* 2436 - (between 0.9 and 1 = touching ground) */
	uint8_t				__unknown_2440[24];		/* 2440 */
	float				fHeliMainRotorDistance;	/* 2464 - distance/rotation */
	float				fHeliSecRotorDistance;	/* 2468 - distance/rotation secondary rotor */
	uint8_t				__unknown_2472[88];		/* 2472 */
	uint8_t				SmokeTrailEnabled;		/* 2560 */
	uint8_t				__unknown_2561[23];		/* 2561 */
												/* 2584 */
};

struct object_info
{
#pragma pack( 1 )
	char padd[66]; //struct object_base	base;
	uint8_t				__unknown_66;			/* 66 */
	uint8_t				flags;	/* 67 - collision and ...? */
	uint8_t				__unknown_68[7];	/* 68 */
	float				physical_height;	/* 75 - normally max. 0.2 (bigger value = more 'ouch' when jumping), different results for different objects? */
	uint8_t				__unknown_79[173];	/* 79 - first value changing when last movement was going up or going down*/
	struct actor_info	*actor; /* 252 - pointer to Actor Object is attached to, 0 when not attached */
	uint8_t				__unknown_256[8];	/* 256 */
	float				height_to_actor;	/* 264 - when attached height difference to actor, else 0*/
	uint8_t				__unknown_268[4];	/* 268 */
	float				pitch;	/* 272 - rotation, pitch (attached) */
	float				roll;	/* 276 - rotation, roll (attached) */
	uint8_t				__unknown_280[68];	/* 280 */
	float				scale;	/* 348 - Scale */
	uint8_t				__unknown_352[59];	/* 352 */
};



/* 
	Classes taken from DK22Pac's plugin sdk 
	https://github.com/DK22Pac/plugin-sdk/
*/

class CVehicle : public CPhysical {
public:
	char pad412421[0x24C]; //CAEVehicleAudioEntity      m_vehicleAudio;
	tHandlingData             *m_pHandlingData;
	int* flying; //tFlyingHandlingData       *m_pFlyingHandlingData;
	union {
		eVehicleHandlingFlags  m_nHandlingFlagsIntValue;
		struct {
			unsigned int b1gBoost : 1;
			unsigned int b2gBoost : 1;
			unsigned int bNpcAntiRoll : 1;
			unsigned int bNpcNeutralHandl : 1;
			unsigned int bNoHandbrake : 1;
			unsigned int bSteerRearwheels : 1;
			unsigned int bHbRearwheelSteer : 1;
			unsigned int bAltSteerOpt : 1;
			unsigned int bWheelFNarrow2 : 1;
			unsigned int bWheelFNarrow : 1;
			unsigned int bWheelFWide : 1;
			unsigned int bWheelFWide2 : 1;
			unsigned int bWheelRNarrow2 : 1;
			unsigned int bWheelRNarrow : 1;
			unsigned int bWheelRWide : 1;
			unsigned int bWheelRWide2 : 1;
			unsigned int bHydraulicGeom : 1;
			unsigned int bHydraulicInst : 1;
			unsigned int bHydraulicNone : 1;
			unsigned int bNosInst : 1;
			unsigned int bOffroadAbility : 1;
			unsigned int bOffroadAbility2 : 1;
			unsigned int bHalogenLights : 1;
			unsigned int bProcRearwheelFirst : 1;
			unsigned int bUseMaxspLimit : 1;
			unsigned int bLowRider : 1;
			unsigned int bStreetRacer : 1;
			unsigned int bSwingingChassis : 1;
		} m_nHandlingFlags;
	};
	CAutoPilot                 m_autoPilot;
	struct {
		unsigned char bIsLawEnforcer : 1; // Is this guy chasing the player at the moment
		unsigned char bIsAmbulanceOnDuty : 1; // Ambulance trying to get to an accident
		unsigned char bIsFireTruckOnDuty : 1; // Firetruck trying to get to a fire
		unsigned char bIsLocked : 1; // Is this guy locked by the script (cannot be removed)
		unsigned char bEngineOn : 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
		unsigned char bIsHandbrakeOn : 1; // How's the handbrake doing ?
		unsigned char bLightsOn : 1; // Are the lights switched on ?
		unsigned char bFreebies : 1; // Any freebies left in this vehicle ?

		unsigned char bIsVan : 1; // Is this vehicle a van (doors at back of vehicle)
		unsigned char bIsBus : 1; // Is this vehicle a bus
		unsigned char bIsBig : 1; // Is this vehicle a bus
		unsigned char bLowVehicle : 1; // Need this for sporty type cars to use low getting-in/out anims
		unsigned char bComedyControls : 1; // Will make the car hard to control (hopefully in a funny way)
		unsigned char bWarnedPeds : 1; // Has scan and warn peds of danger been processed?
		unsigned char bCraneMessageDone : 1; // A crane message has been printed for this car allready
		unsigned char bTakeLessDamage : 1; // This vehicle is stronger (takes about 1/4 of damage)

		unsigned char bIsDamaged : 1; // This vehicle has been damaged and is displaying all its components
		unsigned char bHasBeenOwnedByPlayer : 1;// To work out whether stealing it is a crime
		unsigned char bFadeOut : 1; // Fade vehicle out
		unsigned char bIsBeingCarJacked : 1; // Fade vehicle out
		unsigned char bCreateRoadBlockPeds : 1;// If this vehicle gets close enough we will create peds (coppers or gang members) round it
		unsigned char bCanBeDamaged : 1; // Set to FALSE during cut scenes to avoid explosions
		unsigned char bOccupantsHaveBeenGenerated : 1; // Is true if the occupants have already been generated. (Shouldn't happen again)
		unsigned char bGunSwitchedOff : 1; // Level designers can use this to switch off guns on boats

		unsigned char bVehicleColProcessed : 1;// Has ProcessEntityCollision been processed for this car?
		unsigned char bIsCarParkVehicle : 1; // Car has been created using the special CAR_PARK script command
		unsigned char bHasAlreadyBeenRecorded : 1; // Used for replays
		unsigned char bPartOfConvoy : 1;
		unsigned char bHeliMinimumTilt : 1; // This heli should have almost no tilt really
		unsigned char bAudioChangingGear : 1; // sounds like vehicle is changing gear
		unsigned char bIsDrowning : 1; // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
		unsigned char bTyresDontBurst : 1; // If this is set the tyres are invincible

		unsigned char bCreatedAsPoliceVehicle : 1;// True if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
		unsigned char bRestingOnPhysical : 1; // Dont go static cause car is sitting on a physical object that might get removed
		unsigned char bParking : 1;
		unsigned char bCanPark : 1;
		unsigned char bFireGun : 1; // Does the ai of this vehicle want to fire it's gun?
		unsigned char bDriverLastFrame : 1; // Was there a driver present last frame ?
		unsigned char bNeverUseSmallerRemovalRange : 1;// Some vehicles (like planes) we don't want to remove just behind the camera.
		unsigned char bIsRCVehicle : 1; // Is this a remote controlled (small) vehicle. True whether the player or AI controls it.

		unsigned char bAlwaysSkidMarks : 1; // This vehicle leaves skidmarks regardless of the wheels' states.
		unsigned char bEngineBroken : 1; // Engine doesn't work. Player can get in but the vehicle won't drive
		unsigned char bVehicleCanBeTargetted : 1;// The ped driving this vehicle can be targetted, (for Torenos plane mission)
		unsigned char bPartOfAttackWave : 1; // This car is used in an attack during a gang war
		unsigned char bWinchCanPickMeUp : 1; // This car cannot be picked up by any ropes.
		unsigned char bImpounded : 1; // Has this vehicle been in a police impounding garage
		unsigned char bVehicleCanBeTargettedByHS : 1;// Heat seeking missiles will not target this vehicle.
		unsigned char bSirenOrAlarm : 1; // Set to TRUE if siren or alarm active, else FALSE
		unsigned char bHasGangLeaningOn : 1;
		unsigned char bGangMembersForRoadBlock : 1;// Will generate gang members if NumPedsForRoadBlock > 0
		unsigned char bDoesProvideCover : 1; // If this is false this particular vehicle can not be used to take cover behind.
		unsigned char bMadDriver : 1; // This vehicle is driving like a lunatic
		unsigned char bUpgradedStereo : 1; // This vehicle has an upgraded stereo
		unsigned char bConsideredByPlayer : 1; // This vehicle is considered by the player to enter
		unsigned char bPetrolTankIsWeakPoint : 1;// If false shootong the petrol tank will NOT Blow up the car
		unsigned char bDisableParticles : 1; // Disable particles from this car. Used in garage.
		unsigned char bHasBeenResprayed : 1; // Has been resprayed in a respray garage. Reset after it has been checked.
		unsigned char bUseCarCheats : 1; // If this is true will set the car cheat stuff up in ProcessControl()
		unsigned char bDontSetColourWhenRemapping : 1;// If the texture gets remapped we don't want to change the colour with it.
		unsigned char bUsedForReplay : 1; // This car is controlled by replay and should be removed when replay is done.
	} m_nFlags;
	unsigned int m_nCreationTime;
	unsigned char  m_nPrimaryColor;
	unsigned char  m_nSecondaryColor;
	unsigned char  m_nTertiaryColor;
	unsigned char  m_nQuaternaryColor;
	char   m_anExtras[2];
	short  m_anUpgrades[15];
	float    m_fWheelScale;
	unsigned short m_nAlarmState;
	short  m_nForcedRandomRouteSeed; // if this is non-zero the random wander gets deterministic
	CPed *m_pDriver;
	CPed *m_apPassengers[8];
	unsigned char  m_nNumPassengers;
	unsigned char  m_nNumGettingIn;
	unsigned char  m_nGettingInFlags;
	unsigned char  m_nGettingOutFlags;
	unsigned char  m_nMaxPassengers;
	unsigned char  m_nWindowsOpenFlags; // initialised, but not used?
	unsigned char  m_nNitroBoosts;
	unsigned char  m_nSpecialColModel;
	CEntity *m_pEntityWeAreOn; // we get it from CWorld::ProcessVerticalLine or ProcessEntityCollision, it's entity under us, 
							   //only static entities (buildings or roads)
	int* padfire; //CFire *m_pFire;
	float  m_fSteerAngle;
	float  m_f2ndSteerAngle; // used for steering 2nd set of wheels or elevators etc..
	float  m_fGasPedal;
	float  m_fBreakPedal;
	unsigned char  m_nCreatedBy; // see eVehicleCreatedBy
	short m_nExtendedRemovalRange; // when game wants to delete a vehicle, it gets min(m_wExtendedRemovalRange, 170.0)
	unsigned char m_nBombOnBoard : 3; // 0 = None
									  // 1 = Timed
									  // 2 = On ignition
									  // 3 = remotely set ?
									  // 4 = Timed Bomb has been activated
									  // 5 = On ignition has been activated
	unsigned char m_nOverrideLights : 2; // uses enum NO_CAR_LIGHT_OVERRIDE, FORCE_CAR_LIGHTS_OFF, FORCE_CAR_LIGHTS_ON
	unsigned char m_nWinchType : 2; // Does this vehicle use a winch?
	unsigned char m_nGunsCycleIndex : 2; // Cycle through alternate gun hardpoints on planes/helis
	unsigned char m_nOrdnanceCycleIndex : 2; // Cycle through alternate ordnance hardpoints on planes/helis
	unsigned char m_nUsedForCover; // Has n number of cops hiding/attempting to hid behind it
	unsigned char m_nAmmoInClip; // Used to make the guns on boat do a reload (20 by default).
	unsigned char m_nPacMansCollected; // initialised, but not used?
	unsigned char m_nPedsPositionForRoadBlock; // 0, 1 or 2
	unsigned char m_nNumCopsForRoadBlock;
	float   m_fDirtLevel; // Dirt level of vehicle body texture: 0.0f=fully clean, 15.0f=maximum dirt visible
	unsigned char m_nCurrentGear;
	float   m_fGearChangeCount; // used as parameter for cTransmission::CalculateDriveAcceleration, but doesn't change
	float   m_fWheelSpinForAudio;
	float   m_fHealth; // 1000.0f = full health. 0 -> explode
	CVehicle *m_pTractor;
	CVehicle *m_pTrailer;
	CPed *m_pWhoInstalledBombOnMe;
	unsigned int m_nTimeTillWeNeedThisCar; // game won't try to delete this car while this time won't reach
	unsigned int m_nGunFiringTime; // last time when gun on vehicle was fired (used on boats)
	unsigned int m_nTimeWhenBlowedUp; // game will delete vehicle when 60 seconds after this time will expire
	short  m_nCopsInCarTimer; // timer for police car (which is following player) occupants to stay in car. If this timer reachs 
							  // some value, they will leave a car. The timer increases each frame if player is stopped in car, 
							  // otherway it resets
	short  m_wBombTimer;     // goes down with each frame
	CPed *m_pWhoDetonatedMe; // if vehicle was detonated, game copies m_pWhoInstalledBombOnMe here
	float  m_fVehicleFrontGroundZ; // we get these values from CCollision::IsStoredPolyStillValidVerticalLine
	float  m_fVehicleRearGroundZ;  // or CWorld::ProcessVerticalLine
	char field_4EC; // initialised, but not used?
	char field_4ED[11]; // possibly non-used data?
	unsigned int m_nDoorLock; // see enum eCarLock
	unsigned int m_nProjectileWeaponFiringTime; // manual-aimed projectiles for hunter, lock-on projectile for hydra
	unsigned int m_nAdditionalProjectileWeaponFiringTime; // manual-aimed projectiles for hydra
	unsigned int m_nTimeForMinigunFiring; // minigun on hunter
	unsigned char m_nLastWeaponDamageType; // see eWeaponType, -1 if no damage
	CEntity *m_pLastDamageEntity;
	char field_510; // not used?
	char field_511; // initialised, but not used?
	char field_512; // initialised, but not used?
	char m_nVehicleWeaponInUse; // see enum eCarWeapon
	unsigned int     m_nHornCounter;
	char field_518; // random id related to siren
	char field_519; // car horn related
	char field_51A;
	char       m_nHasslePosId;
	char frontpollypad[0x2C]; //CStoredCollPoly m_FrontCollPoly; // poly which is under front part of car
	char rearpollypad[0x2C]; //CStoredCollPoly m_RearCollPoly; // poly which is under rear part of car
	unsigned char      m_anCollisionLighting[4]; // left front, left rear, right front, right rear
	int* padfxp[3];//FxSystem_c *m_pOverheatParticle;
	//FxSystem_c *m_pFireParticle;
	//FxSystem_c *m_pDustParticle;
	union {
		unsigned char     m_nRenderLightsFlags;
		struct {
			unsigned char m_bRightFront : 1;
			unsigned char m_bLeftFront : 1;
			unsigned char m_bRightRear : 1;
			unsigned char m_bLeftRear : 1;
		} m_renderLights;
	};
	int* padplatetxt; //RwTexture *m_pCustomCarPlate;
	CVehicle *field_58C;
	unsigned int     m_nVehicleClass; // see enum eVehicleType
	unsigned int     m_nVehicleSubClass; // see enum eVehicleType
	short      m_nPreviousRemapTxd;
	short      m_nRemapTxd;
	int* padremaptxt; //RwTexture *m_pRemapTexture;

	static float &WHEELSPIN_TARGET_RATE; // 1.0
	static float &WHEELSPIN_INAIR_TARGET_RATE; // 10.0
	static float &WHEELSPIN_RISE_RATE; // 0.95
	static float &WHEELSPIN_FALL_RATE; // 0.7
	static float &m_fAirResistanceMult; // 2.5
	static float &ms_fRailTrackResistance; // 0.003
	static float &ms_fRailTrackResistanceDefault; // 0.003
	static bool &bDisableRemoteDetonation;
	static bool &bDisableRemoteDetonationOnContact;
	static bool &m_bEnableMouseSteering;
	static bool &m_bEnableMouseFlying;
	static int &m_nLastControlInput;
	static CColModel **m_aSpecialColVehicle; // CColModel *CVehicle::m_aSpecialColVehicle[4]
	static bool &ms_forceVehicleLightsOff;
	static bool &s_bPlaneGunsEjectShellCasings;
	static CColModel *m_aSpecialColModel; // static CColModel m_aSpecialColModel[4]
};