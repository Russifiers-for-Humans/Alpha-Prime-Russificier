
float currenttime
const float DefaultFOV = 65
float g_fDecalLifetime = 99

float PlayerCapsuleHeights[2]			//celkove vysky kapsul playera (normal a crouch)
float PlayerCapsuleHalfHeights[2]		//Polovicne vysky kapsul playera (normal a crouch)

const float JOY_DEATH_ZONE = 100
const float JOY_MAX_POW = 500

//physics
const int JOINT_FIXED = 1
const int JOINT_BALL = 2
const int JOINT_HINGE = 3
const int JOINT_SLIDING = 4
const int JOINT_ANGLIMIT = 5
const int JOINT_LINLIMIT = 6
const int JOINT_FIXEDORIENT = 7
const int JOINT_PRISMATIC = 8
const int JOINT_SPRINGDAMPER = 9
const int JOINT_ANGSPRINGDAMPER = 10


const float defaultBreakLimit = 30
const float defaultWeight = 5
const float defaultWeapScale = 1
const float halfWeapScale = 0.5
const float defaultBodyHealth = 100
const float defaultJointHealth = 50
const float ShootImpactScale = 124.671		//WeaponDamage * ShootImpactScale = fyzikalna sila pre zasah
const float ExplosionImpactScale = 41.04     	//ExplosionDamage * ShootImpactScale = fyzikalna sila pre zasah v ohnisku vybuchu
const float MinImpactSoundsDelay = 0.1666	//impact sound jedneho body nesmie zahrat po sebe skor ako je tato hodnota v sekundach

const int GEOMTYPE_NONE = 0	//z modelu
const int GEOMTYPE_BOX = 1
const int GEOMTYPE_SPHERE = 2
const int GEOMTYPE_CAPSULE = 3
const int GEOMTYPE_NULL = 4	//ziadna

#ifdef GERMAN
	const float RAGDOLL_TIMEOUT = 48 //po kolika vterinach se ragdolly deaktivoujou
#else
	const float RAGDOLL_TIMEOUT = 196 //po kolika vterinach se ragdolly deaktivoujou
#endif

const int MaxAvailableWeapons = 8

//=========================================================================================
// Defs & globals
//=========================================================================================

const int ASF_AIMING = 1
const int ASF_MOVEMENT = 2
const int ASF_ALL = 0xffffffff


const int DisableFuncFlag = 0x400 //(bit 10, 11. v poradi)

 //class types
const int ClassTypeWorld = 1

const int ClassTypeFunc = 2
	const int ClassNumDoor = 3
	const int ClassNumRotDoor = 4
	const int ClassNumRotating = 9

const int ClassTypeTrigger = 3

const int ClassTypeLight = 5

const int ClassTypeSound = 7

const int ClassTypeInfo = 8
	const int ClassNumNull = 2
	const int ClassNumImpacter = 3

const int ClassTypeCreature = 9
	const int ClassNumPlayer = 1

const int CTypeInfoWaypoint = 10
const int CTypeInfoWpGroup = 11

const int ClassTypeItem = 13

const int ClassTypeMisc = 14

const int ClassTypeTrapAnim = 17

const int ClassTypeEnterableVehicle = 18

const int ClassTypeMiscPhysicsGlass = 200
	const int ClassNumMiscPhysicsGlass2 = 1

const int ClassTypeMiscShootDetector = 201
const int ClassTypeMiscModel = 202
const int ClassTypePhysicsPart = 203
const int ClassTypeMiscShootingShield = 204
const int ClassTypeRBone = 205
const int ClassTypeFuncTrain = 206

//flags for noise identification
const int NOISE_FOOTSTEP	= 0x00000001
const int NOISE_SHOOT		= 0x00000002
const int NOISE_THUD			= 0x00000004
const int NOISE_PAIN			= 0x00000008
const int NOISE_PHYSICS		= 0x00000010
const int NOISE_ATTACK		= 0x00000020
const int NOISE_FLAME		= 0x00000040

//AI states
//alertable
const int STATE_WALK		= 1
const int STATE_IDLE		= 2
const int STATE_OBSERVE	= 3
const int STATE_SCRIPT		= 4
const int STATE_GUARD		= 5	//guarding
const int STATE_RETURN		= 6	//return to last position
const int STATE_LOST_TARGET = 7

//alerted
const int STATE_1STSIGHT	= 8
const int STATE_CHASE		= 9
const int STATE_LR_ATTACK	= 10
const int STATE_ATTACK	= 11
const int STATE_CLOSECOMBAT = 12

//unalertable
const int STATE_COVER	= 13
const int STATE_ROAMING	= 14
const int STATE_BORN		= 15
const int STATE_COVERFROMDANGER	= 16

const int STATE_DIE			= 17
const int STATE_DEAD			= 18

//flagy u potvor. Promenna flags
const int FL_GUARD			= 0x001
const int FL_COMBAT			= 0x002
const int FL_FRIENDLY		= 0x004
const int FL_WAIT4TRIGGER	= 0x008
const int FL_PUPPET			= 0x010
const int FL_SUMMON			= 0x020
const int FL_NOTEST			= 0x040
const int FL_ROAM				= 0x080 //automaticky nastavuje TACTICS_ROAM pro potvurky
const int FL_NODISABLE		= 0x100

const int FL_SUMMON_TRIGGER = 0x028

//private
const int FL_SEE				= 0x1000
const int FL_HEAR				= 0x2000
const int FL_FEEL				= 0x4000
const int FL_HURTENEMIES	= 0x8000
const int FL_CANOPENDOORS	= 0x10000
const int FL_NOPAIN			= 0x20000
const int FL_FROZEN			= 0x40000

const int ET_WORLD			= 1
const int ET_NOCOLLISION	= 2
const int ET_DEFAULT			= 4	//veciam pre ktore nemaju nastavene nieco ine, nutno nastavit toto!
const int ET_PLAYER			= 8
const int ET_NPC				= 16

const int ET_RAGDOLL			= 32
const int ET_DEBRIS			= 64
const int ET_GRENADE			= 128
const int ET_SHELL			= 512
const int ET_DOOR				= 1024

const int ET_VEHICLE			= 2048
const int ET_PLAYERCAST		= 4096
const int ET_MANIPULATING	= 8192

const int ET_PLAYERCLIP		= 16384

//typy munice. Nastavit u zbrane v AmmoType
#ifdef EDITOR
const string AmmoTypeEditor[3] = {"AmmoPistol", "AmmoShotgun", "AmmoMachinegun"}
#endif

const int AmmoJackHammer				= 1
const int AmmoPistol						= 2
const int AmmoShotgun					= 3
const int AmmoMachinegun				= 4
const int AmmoSniperRifle				= 5
const int AmmoRocketLauncher			= 6
const int AmmoFlameThrower				= 7
const int AmmoGrenade					= 8

//-----------------------------------------------------------------------------

const string bloodmark_names[4] = {"decals/gore/bloodmark1","decals/gore/bloodmark2","decals/gore/bloodmark3","decals/gore/bloodmark4"}
int	bloodmarks[4]
int	s_BurnShader


int s_hurt[4]

//defaultne materialy. dalsie su v sound/materials.h
const int MATERIAL_METAL	= 1	//plne zelezo
const int MATERIAL_IRON		= 2	//plech
const int MATERIAL_GLASS	= 3	//sklo
const int MATERIAL_PLASTIC	= 4	//umela hmota
const int MATERIAL_LIQUID	= 5	//tekutiny, voda
const int MATERIAL_SLIME	= 6	//sliz, olej apod.
const int MATERIAL_BETON	= 7	//beton
const int MATERIAL_RUBBER	= 8	//guma, linoeum
const int MATERIAL_FLESH	= 9	//maso, humanoidi
const int MATERIAL_STONE	= 10	//kamen, skala
const int MATERIAL_GRAVEL	= 11	//sterk

const int MATERIAL_LAST	= 12 //last one

//-----------------------------------------------------------------------------
int Surf2Mat(int surf)
{
	if(!surf & SURF_MATERIAL_MASK)
		return MATERIAL_METAL
	
	surf = surf & SURF_MATERIAL_MASK >> SURF_MATERIAL_SHIFT
	surf = surf & 255
	return surf
}

//-----------------------------------------------------------------------------
string SoundPaths[6]		//cesty k ambientom pre aktualny level
float  SoundVolumes[6]	//hlasitosti ambientov
const int BS_NONE 		= -2
const int BS_AMBIENT 	= 0
const int BS_ATMOSPHERE = 1
const int BS_FIGHT 		= 2
const int BS_FIGHT2 		= 3
const int BS_DIALOG 		= 4
const int BS_DIALOG2 	= 5


const int DefaultWeight = 40

//-----------------------------------------------------------------------------
//         Global variables
//-----------------------------------------------------------------------------
//player stat widgets
widget WFlash


widget WView, WText, WMousePointer
widget WDialogText

//widget LevelNumber

int SmallFont, LargeFont, MainFont

ctype	CMinClass, CExtClass, CFuncClass, CDoorClass, CItemClass, CMiscPhysicsModel, CItemWeaponSteel, CItemMisc
ctype CSteelWeaponClass, CFireWeaponClass, CWeaponClass
ctype CAICreatureClass, CCreatureClass
ctype CGlassDebris, CBrkDebris, CProjectile
ctype CMiscPhysicsGlass
ctype CItemWeaponMine
ctype CParticleEffector
ctype CTriggerClass, CTriggerDynamic, CTriggerHackDetector
ctype CFuncDoor, CFuncDoorRot
ctype CRigidBodyClass, CMiscPhysicsPack, CPhysicsPart
ctype CInfoPath, CInfoConnection, CInfoWaypoint, CInfoNull
ctype CInfoPathSwitch
ctype CEnterableVehicle
ctype CRBone
ctype CJointClass

const vobject VNULL
const hsound HNULL

int g_iFly

//-----------------------------------------------------------------------------
// Clima globals
//-----------------------------------------------------------------------------
//vector g_climaWind = ZeroVec
//vector g_climaWindDir = ZeroVec
//float  g_WindPower
//float  wind_val

//-----------------------------------------------------------------------------
//mouse parms
int MouseX
int MouseY
float g_fMouseSpeedH
float g_fMouseSpeedV
int g_InvMouse
int g_MSpeedH
int g_MSpeedV
//-----------------------------------------------------------------------------

bool CheatNextLevel
bool CheatWeapons
bool CheatAmmo
bool CheatImmortality
bool CheatHubbardium
bool CheatHealth

bool IsPlayerID(int id)
{
	return true
/*
	for(int n = 0; n < sizeof(g_iPlayerIDs); n++)
	{
		int i = g_iPlayerIDs[n]
		if(i == id)
			return true
		
		if(i < 0)
			break
	}
	
	return false*/
}
