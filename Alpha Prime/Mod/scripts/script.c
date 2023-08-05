//prototypes
#include "scripts/proto.h"
#include "scripts/definitions/globals.h"

bool r_bloom = false
float r_gbias = -0.01 //gamma texture bias
float r_blurscale = 0.614 //bloom image scale
float r_imgscale = 0.814 //original image scale
float r_gauss = 0.404 //gaussian blur scale

#include "scripts/soundset.h"
#include "scripts/definitions/sounds.h"
#include "scripts/definitions/keys.h"
#include "scripts/definitions/items.h"
#include "scripts/definitions/strings.h"
#include "scripts/definitions/shoteffects.h"

#include "scripts/shaders.h"

#include "scripts/animset.h"
#include "scripts/definitions/anim.h"
#include "scripts/definitions/gameplay.h"

bool g_bShowAI
bool g_bDoShowAI
bool g_bFreezeAI

float g_FlySpeed
	
#ifdef DEVELOPER
	bool g_bDebugFSM
	string ComputerName
#else
	string ComputerName = "fido"
#endif

float g_FPS

//superclasses
#include "scripts/hud.h"
#include "scripts/super.h"
#include "scripts/music_player.h"
#include "scripts/definitions/animset.h"
#include "scripts/definitions/speechset.h"
#include "scripts/func_display.h"

#include "scripts/class_particle.h"   // must be here, for footsteps in super_actor
#include "scripts/super_actor.h"
#include "scripts/super2.h"

int g_iVMode = -1

float Flash = 0

hsound MenuMusic
int MenuMusicID

hsound GameOverSnd
int GameOverSound

int globalmap
int MenuMap
bool g_bEndLevel
bool g_bStartLevel
int g_EndStatus
bool g_EscKey
bool g_RunMode
bool g_FirstRun
bool g_FromSavePos
bool g_bRMap = false
bool g_bImmortal
bool g_Joypad
int g_iMInGame
bool DebugLevel

widget EndGameMessage
float EndGameMessageLeft
float EndGameMessageTop
float EndGameMessageHeight
float EndGameMessageWidth
bool	NeedAutosave
int g_SaveSelected

const int RUNMODE_DEVELOPER = 0		//mapa bola spustena sposobom Loader.exe NazovMapy	(develop ucely)
const int RUNMODE_NORMAL = 1			//normalna hra cez NewGame, ContinueGame alebo po LoadGame
const int RUNMODE_TUTORIAL = 2		//bezi mapa ktora je tutorial

const int DIFFICULTY_EASY = 0
const int DIFFICULTY_NORMAL = 1
const int DIFFICULTY_HARD = 2
int g_DifficultySelected


string g_sNextLevel, g_sThisLevel
int g_iNumLevels = 12
const string g_sLevelNames[14] = {"l0i", "l00", "l01", "l02", "l03",
		"l04", "l05", "l06", "l07", "l08",
		"l09", "l10", "finale"}

widget g_IMBackground
widget g_wBackground, g_wProgress, g_wBlack, g_wLoading, g_wGameOver1, g_wGameOver2
widget g_StartScreen, g_StartVideo
widget g_DemoEndVideo
hsound g_VideoSnd
int g_VideoSound

string ActButton

vector g_vPlayerPos
vector g_vPlayerAngs

vector EarthquakeOff, EarthquakeAng
float ETime, ETime2, ETimeMax, ESpeed, EPower, EFadeInTime, EFadeOutTime, EPriority
string g_FromConsoleCall

int NOISE_NOSHOT_MASK

//=============== FIXME no optional place for this ===============================
bool HealthFlicker
float HealthBarAlpha
float HFSpeed


//-----------------------------------------------------------------
#ifdef _XBOX
int GetTickCount()
{
	return 0
}
#endif

const int SAVETYPE_NONE = 0
const int SAVETYPE_EMPTY = 1
const int SAVETYPE_AUTO = 2
const int SAVETYPE_NORMAL = 3
const int SAVETYPE_QUICK = 4

string SaveFileNames[64]
string SaveMapsNames[64]
string SaveMapsTimes[64]
int	 SaveTypes[64]
int 	 SaveIdent

//-----------------------------------------------------------------
int GetNewtestSaveSlot(int slot1, int slot2)
{
	string tokens1[32]
	string tokens2[32]
	int NumTok = ParseString(SaveMapsTimes[slot1], tokens1)
					 ParseString(SaveMapsTimes[slot2], tokens2)

	for(int n = 0; n < NumTok; n++)
	{
		if(tokens1[n] == "/" || tokens1[n] == ":")
			continue

		int num1 = atoi(tokens1[n])
		int num2 = atoi(tokens2[n])
		
		if(num1 == num2)
			continue

		if(num1 > num2)
			return slot1
		
		return slot2
	}
	return slot2
}

//-----------------------------------------------------------------
int GetLastSavedSlot()	//podla datumu
{
	int BestTimeSlot = -1
	for(int n = 0; n < sizeof(SaveTypes); n++)	
	{
		if(SaveTypes[n] > SAVETYPE_EMPTY)
		{
			if(BestTimeSlot == -1)
				BestTimeSlot = n
			else
				BestTimeSlot = GetNewtestSaveSlot(n, BestTimeSlot)
		}
	}
	return BestTimeSlot
}

//-----------------------------------------------------------------
int GetLastAutoSavedSlotOfLevel(string level)	//podla datumu
{
	int BestTimeSlot = -1
	for(int n = 0; n < sizeof(SaveTypes); n++)	
	{
		if(SaveTypes[n] == SAVETYPE_AUTO && SaveMapsNames[n] == level)
		{
			if(BestTimeSlot == -1)
				BestTimeSlot = n
			else
				BestTimeSlot = GetNewtestSaveSlot(n, BestTimeSlot)
		}
	}
	return BestTimeSlot
}

//-----------------------------------------------------------------
int GetEmptySaveSlot()
{
	int EmptySlot = -1
	int n
	for(n = 0; n < sizeof(SaveTypes); n++)	
	{
		if(SaveTypes[n] == SAVETYPE_EMPTY)
		{
			EmptySlot = n
			break
		}
	}
	return EmptySlot
}

//-----------------------------------------------------------------
void RemoveSaveEmptySlot()
{
	int EmptySlot = GetEmptySaveSlot()
	
	if(EmptySlot == -1)
		return
	
	int LastIndex = sizeof(SaveTypes) - 1
	for(int n = EmptySlot; n < LastIndex; n++)	
	{
		int next = n + 1
		SaveFileNames[n] = SaveFileNames[next]
		SaveMapsNames[n] = SaveMapsNames[next]
		SaveMapsTimes[n] = SaveMapsTimes[next]
		SaveTypes[n] = SaveTypes[next]
	}
}

//-----------------------------------------------------------------
void AddSaveEmptySlot()
{
	int EmptySlot = GetEmptySaveSlot()
	
	if(EmptySlot != -1)
		return

	EmptySlot = 0
	int n

	for(n = 0; n < sizeof(SaveTypes); n++)	
	{
		if(SaveTypes[n] != SAVETYPE_QUICK)
		{
			EmptySlot = n
			break
		}
	}

	int LastIndex = sizeof(SaveTypes) - 1
	for(n = LastIndex; n > EmptySlot; n--)	
	{
		int prev = n - 1
		SaveFileNames[n] = SaveFileNames[prev]
		SaveMapsNames[n] = SaveMapsNames[prev]
		SaveMapsTimes[n] = SaveMapsTimes[prev]
		SaveTypes[n] = SaveTypes[prev]
	}

	SaveFileNames[EmptySlot] = ""
	SaveMapsNames[EmptySlot] = ""
	SaveMapsTimes[EmptySlot] = ""
	SaveTypes[EmptySlot] = SAVETYPE_EMPTY
}

//-----------------------------------------------------------------
void LoadSaveList()	//vracia empty slot do ktoreho je mozno ukladat
{
	string tokens[10]
	int n = 0
	tparse tp = BeginParse("save/save.lst")
	int line = 0
	int num = sizeof(SaveFileNames)
	SaveIdent = 1

	if(tp)
	{
		while(ParseLine(tp, n++, tokens) && line < num)
		{
			if(tokens[0] == "SAVE" || tokens[0] == "SOFTSAVE" || tokens[0] == "QUICKSAVE")
			{
				SaveFileNames[line] = tokens[1]
				SaveMapsNames[line] = tokens[2]
				SaveMapsTimes[line] = tokens[3]
	
				if(tokens[0] == "SOFTSAVE")	//poznamenename si ktore sejvy su iba soft
					SaveTypes[line] = SAVETYPE_AUTO
					
				if(tokens[0] == "SAVE")
					SaveTypes[line] = SAVETYPE_NORMAL
					
				if(tokens[0] == "QUICKSAVE")
					SaveTypes[line] = SAVETYPE_QUICK
	
				if(tokens[0] != "QUICKSAVE")
				{
					string id = substr(tokens[1], 3, 4)
					int ident = atoi(id)
					if(ident > SaveIdent)	//hladame navacsie unikatne ID
						SaveIdent = ident
				}
				line++
			}
		}
		EndParse(tp)
	}

	if(g_SaveSelected > line)
		g_SaveSelected = line

	for(n = line; n < num; n++)
	{
		SaveFileNames[n] = ""
		SaveMapsNames[n] = ""
		SaveMapsTimes[n] = ""
		SaveTypes[n] = SAVETYPE_NONE
	}

	AddSaveEmptySlot()	//ked je vsetko natiahnute tak tam vlozime empty slot na zaklade toho ake sejvy uz mame v slotoch
}

//-----------------------------------------------------------------
// Level transition storage
//-----------------------------------------------------------------
class PlayerStorage
{
	int weapons[10]
	int NumWeapons
	int ammo[10]	//ammo mimo zasobniky
	int MagazinesAmmo[10]
	int CurrentWeapon
	float BulletTime
	float health
	float oxygen
	float flashlight
	int DifficultyLevel
}
PlayerStorage g_PlayerStorage					//na prenos z levelu do levelu a pod

//------------------------------------------------------------------
void BeforeNewGame(int DifficultyLevel)		//pred nahravanim levelu ked zacina uplne nova hra
{
	if(!g_PlayerStorage)
		g_PlayerStorage = new PlayerStorage
	
	PlayerStorage st = g_PlayerStorage
	st.NumWeapons = 0

	st.weapons[st.NumWeapons] = CDummyWeapon		//ziadna zbran (default)
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

/*	st.weapons[st.NumWeapons] = CJackHammer
	st.ammo[st.NumWeapons] = -1						//player dostane startovne ammo nadefinovane v gameplay.h
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CPistol
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CShotgun
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CMachinegun
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
	
	st.weapons[st.NumWeapons] = CSniperRifle
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CRocketLauncher
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CFlameThrower
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
	
	st.weapons[st.NumWeapons] = CGrenade
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
*/
	st.CurrentWeapon = CDummyWeapon
	st.BulletTime = 0
	st.health = PlayerMaxHealth
	st.flashlight = FlashLightMaxTime
	st.oxygen = PlayerMaxOxygen
	st.DifficultyLevel = DifficultyLevel
}

//------------------------------------------------------------------
void BeforeRunFromCommandLine(int DifficultyLevel)		//pred nahravanim levelu z command lajny
{
	LoadSaveList()

	if(!g_PlayerStorage)
		g_PlayerStorage = new PlayerStorage
	
	PlayerStorage st = g_PlayerStorage
	st.NumWeapons = 0

	st.weapons[st.NumWeapons] = CDummyWeapon		//ziadna zbran (default)
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CJackHammer
	st.ammo[st.NumWeapons] = -1						//player dostane startovne ammo nadefinovane v gameplay.h
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
/*
	st.weapons[st.NumWeapons] = CPistol
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CShotgun
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CMachinegun
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
	
	st.weapons[st.NumWeapons] = CSniperRifle
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CRocketLauncher
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++

	st.weapons[st.NumWeapons] = CFlameThrower
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
	
	st.weapons[st.NumWeapons] = CGrenade
	st.ammo[st.NumWeapons] = -1
	st.MagazinesAmmo[st.NumWeapons] = -1
	st.NumWeapons++
*/
	st.CurrentWeapon = CJackHammer
	st.BulletTime = 0
	st.health = PlayerMaxHealth
	st.flashlight = FlashLightMaxTime
	st.oxygen = PlayerMaxOxygen
	st.DifficultyLevel = DifficultyLevel
}

#ifdef DEMO
//------------------------------------------------------------------
void BeforeCustomLevel(int DifficultyLevel, string level)
{
	if(!g_PlayerStorage)
		g_PlayerStorage = new PlayerStorage
	
	PlayerStorage st = g_PlayerStorage
	st.NumWeapons = 0

	if(level == "l03")
	{
		st.weapons[st.NumWeapons] = CDummyWeapon		//ziadna zbran (default)
		st.ammo[st.NumWeapons] = -1
		st.MagazinesAmmo[st.NumWeapons] = -1
		st.NumWeapons++
	
		st.weapons[st.NumWeapons] = CJackHammer
		st.ammo[st.NumWeapons] = -1						//player dostane startovne ammo nadefinovane v gameplay.h
		st.MagazinesAmmo[st.NumWeapons] = -1
		st.NumWeapons++
	
		st.weapons[st.NumWeapons] = CPistol
		st.ammo[st.NumWeapons] = 17
		st.MagazinesAmmo[st.NumWeapons] = 8
		st.NumWeapons++
	
		st.weapons[st.NumWeapons] = CShotgun
		st.ammo[st.NumWeapons] = 15
		st.MagazinesAmmo[st.NumWeapons] = 8
		st.NumWeapons++
	
		st.weapons[st.NumWeapons] = CMachinegun
		st.ammo[st.NumWeapons] = 32
		st.MagazinesAmmo[st.NumWeapons] = 28
		st.NumWeapons++

		st.weapons[st.NumWeapons] = CSniperRifle
		st.ammo[st.NumWeapons] = 0
		st.MagazinesAmmo[st.NumWeapons] = 5
		st.NumWeapons++

		st.weapons[st.NumWeapons] = CRocketLauncher
		st.ammo[st.NumWeapons] = 1
		st.MagazinesAmmo[st.NumWeapons] = 1
		st.NumWeapons++
/*	
		st.weapons[st.NumWeapons] = CFlameThrower
		st.ammo[st.NumWeapons] = -1
		st.MagazinesAmmo[st.NumWeapons] = -1
		st.NumWeapons++
*/
		st.weapons[st.NumWeapons] = CGrenade
		st.ammo[st.NumWeapons] = 4
		st.MagazinesAmmo[st.NumWeapons] = 1
		st.NumWeapons++

		st.CurrentWeapon = CDummyWeapon
		st.BulletTime = 0
		st.health = PlayerMaxHealth
		st.flashlight = FlashLightMaxTime
		st.oxygen = PlayerMaxOxygen
		st.DifficultyLevel = DifficultyLevel		
	}
	else
	{
		st.weapons[st.NumWeapons] = CDummyWeapon		//ziadna zbran (default)
		st.ammo[st.NumWeapons] = -1
		st.MagazinesAmmo[st.NumWeapons] = -1
		st.NumWeapons++

		st.weapons[st.NumWeapons] = CJackHammer
		st.ammo[st.NumWeapons] = -1
		st.MagazinesAmmo[st.NumWeapons] = -1
		st.NumWeapons++

		st.CurrentWeapon = CJackHammer
		st.BulletTime = 0
		st.health = PlayerMaxHealth
		st.flashlight = FlashLightMaxTime
		st.oxygen = PlayerMaxOxygen
		st.DifficultyLevel = DifficultyLevel
	}
}
#endif

//-----------------------------------------------------------------
float g_FadeCounter
bool  g_WFadeStat
float DelayCounter

bool Delay(float WaitTime)		//odfaduje/nafaduje globalny widget
{
	float ftm = ftime
	if(ftm == 0)
		ftm = 0.01

	DelayCounter += ftm * 100

	if(DelayCounter >= WaitTime)
	{
		DelayCounter = 0
		return true
	}

	return false
}

const int FADE_TEXT = 1
const int FADE_BACK = 2

bool FadeWidget(widget w, float speed, int flags)		//odfaduje/nafaduje globalny widget
{
	if(!g_WFadeStat)
	{
		g_WFadeStat = true
		if(w)
			ShowWidget(w, true)

		if(speed < 0)
			g_FadeCounter = 255
		else
			g_FadeCounter = 0
	}

	float ftm = ftime
	if(ftm == 0)
		ftm = 0.01

	g_FadeCounter += ftm * speed

	if(g_FadeCounter >= 255)
		g_FadeCounter = 255

	if(g_FadeCounter <= 0)
		g_FadeCounter = 0

	if(w)
	{
		if(flags & FADE_BACK)
			SetWidgetColor(w, ARGB(g_FadeCounter, g_FadeCounter, g_FadeCounter, g_FadeCounter))
			
		if(flags & FADE_TEXT)
			SetWidgetTextColor(w, ARGB(g_FadeCounter, g_FadeCounter, g_FadeCounter, g_FadeCounter))
	}

	if(g_FadeCounter == 255 || g_FadeCounter == 0)
	{
		g_WFadeStat = false
		return true
	}

	return false
}
//-----------------------------------------------------------------
void LoadBlackWidget()
{
	if(!g_wBlack)
	{
		g_wBlack = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA|WF_BLEND, 0, 125)
		LoadWidgetImage(g_wBlack, 0, "gfx/black.tga")
	}
	ShowWidget(g_wBlack, true)
	SetWidgetColor(g_wBlack, 0xffffffff)
}

//-----------------------------------------------------------------
void change_level(string targ, bool NoFade)
{
	g_bEndLevel = true

#ifdef DEMO
	if(targ != "Escape=Menu LMB=Restart")
		targ = "demoend"
#endif

	g_sNextLevel = targ

	if(NoFade == false)
	{
		LoadBlackWidget()
		SetWidgetColor(g_wBlack, 0x00ffffff)
		g_EndStatus = 1
	}
	else
		g_EndStatus = 10	//vynechame fade
}

//=================================================================================
bool AnyKey()
{
#ifdef _XBOX
	if(JoyButtonState(JB_A) || JoyButtonState(JB_START))
	{
		ClearJoyButton(JB_A)
		ClearJoyButton(JB_START)
		return true
	}
	
#else
	int n
	for(n = 0; n < 256; n++)
	{
		if(KeyState(n))
		{
			ClearKey(n)
			return true
		}
	}

	for(n = 0; n < 3; n++)
	{
		if(MousePress(n))
		{
			ClearMousePress(n)
			return true
		}
	}		

#endif

	return false
}

//=================================================================================
int CreateFontEx(string name)
{
	return CreateFont("gfx/fonts/1024x768/" + name, 1024, 768)
}

//entity classes
#include "scripts/class_info_cutscene.h"
#include "scripts/class_trigger.h"
#include "scripts/class_light.h"

#include "scripts/class_info.h"
#include "scripts/class_item_weapon.h"
#include "scripts/class_shot.h"
#include "scripts/class_item.h"
#include "scripts/class_func.h"
#include "scripts/class_misc_physics_model.h"
#include "scripts/class_misc_physics_pack.h"

#include "scripts/class_misc_model.h"

#include "scripts/class_creature/ai2.h"
#include "scripts/class_creature/robots.h"
#include "scripts/class_creature/npc.h"
#include "scripts/class_creature/boss.h"

bool IsTrue(int n)
{
	if(n == 0)
		return false
	
	return true
}

//-----------------------------------------------------------------
void ExtClass::EOnLoaded(MinClass other, int extra)
{	
	if(parent)
	{
		ExtClass MyParent = FindClass(parent)
		
		if(MyParent)
		{
			UpdateEntity(this)
			UpdateEntity(MyParent)
			vector ChildPos = GetMatrix(this, 3)
			vector mat[3]
			GetMatrix4(MyParent, mat)
			vector ParentOff = ChildPos - GetMatrix(MyParent, 3)
			ParentOff = VectorMatrixInvMultiply3(mat, ParentOff)

			if(AddChild(MyParent, this, 5))
			{
				SetOrigin(this, ParentOff)
				UpdateEntity(MyParent)
				OnParent(MyParent)
#ifdef EDITOR
				bool ChildFound = false
				for(int c = 0; c < MyParent.ChildsNamesNum; c++)
				{
					if(MyParent.ChildsNames[c] == _name)
						ChildFound = true
				}
				
				if(!ChildFound)
					MyParent.ChildsNames[MyParent.ChildsNamesNum++] = _name

				ParentName = MyParent._name
#endif	
			}
			else
				DPrint("Cannot parent " + _name + " on " + parent + ". Unknown error")
		}
		else
			DPrint("Cannot parent " + _name + " on " + parent + ". " + parent + " not found!")
	}
}

//-----------------------------------------------------------------
void ExtClass::M_SetCamera(ExtClass other)
{
	misc_physics_camera cam = FindClass(other._value)
	
	if(cam == NULL || !IsInherited(cam, Type("misc_physics_camera")))
	{
		DPrint(other._value + " is not camera")
		return
	}

	SetRenderView(this, cam.CameraIndex, 0)
	cam.IsConnected = _name
}

#ifdef DEVELOPER
void showfilter()
{
	if(globalmap)
	{
		int prevmap = SetCurrentMap(globalmap)
		
		UIMenu FilterMenu = new UIMenu

		FilterMenu.AddCheckItem("Planar", 0x6000)
		FilterMenu.SetCheckItem(0x6000, IsTrue(g_iShowFilter & 1))

		FilterMenu.AddCheckItem("Patch", 0x6002)
		FilterMenu.SetCheckItem(0x6002, IsTrue(g_iShowFilter & 4))

		FilterMenu.AddCheckItem("Sorted faces", 0x6004)
		FilterMenu.SetCheckItem(0x6004, IsTrue(g_iShowFilter & 16))

		FilterMenu.AddCheckItem("All entities", 0x6005)
		FilterMenu.SetCheckItem(0x6005, IsTrue(g_iShowFilter & 32))

		FilterMenu.AddCheckItem("Sprites", 0x6006)
		FilterMenu.SetCheckItem(0x6006, IsTrue(g_iShowFilter & 64))

		FilterMenu.AddCheckItem("Meshes", 0x6008)
		FilterMenu.SetCheckItem(0x6008, IsTrue(g_iShowFilter & 256))

		FilterMenu.AddCheckItem("Brush models", 0x6009)
		FilterMenu.SetCheckItem(0x6009, IsTrue(g_iShowFilter & 512))

		FilterMenu.AddCheckItem("Particles", 0x600a)
		FilterMenu.SetCheckItem(0x600a, IsTrue(g_iShowFilter & 1024))

		FilterMenu.AddSeparator()

		FilterMenu.AddTextItem("Show all", 0x8000)
		FilterMenu.AddTextItem("Hide all", 0x8001)
		
		ShowCursor(true)
		SetMousePos(40, 40)

		int res = FilterMenu.IPopup(20, 20)
		
		switch(res)
		{
		case 0x8000:
			g_iShowFilter = 0
			break
		
		case 0x8001:
			g_iShowFilter = 0xffffffff
			break
		
		default:
			if(res & 0x6000)
			{
				res = res & 0xff
				
				int bit = 1 << res
				
				if(g_iShowFilter & bit)
					g_iShowFilter &= -1 - bit
				else
					g_iShowFilter |= bit
			}
		}

		delete FilterMenu

		SetCurrentMap(prevmap)
		ShowCursor(false)
		ResetCursor()
	}
		
}
#endif

#ifdef EDITOR
int g_EditorRuns
#endif

int AddToCheckSum(int chksm, int val, int shift)
{
	int mask = val << shift
	chksm = chksm ^ mask
	return chksm
}

//=================================================================================
class ExplosionManager extends MinClass
{
	ShotStorage shot
	ExtClass Bodies[64]
	float Dists[64]
	ExtClass rb
	int BodiesNum
	int ToApply
	float ExploRange

//---------------------------------------------------------------------------------
	void Update()
	{
		bool RigidBody
		bool Creature
		vector ForceOrigin
		float extime

		ExploRange += ftime * 700

		clamp ExploRange<0, shot.radius>

		for(int n = 0; n < BodiesNum; n++)
		{
			rb = Bodies[n]

			if(rb)
			{
				if(ExploRange > Dists[n])
				{
					rb.Shoot(shot)
					rb.Dependencies--		//davame vediet ze entitu uz nepotrebujeme takze sa moze mazat ked ma Dependencies == 0
					Bodies[n] = NULL
					ToApply--
				}
			}
		}		

		if(ToApply <= 0)
			delete this
	}

//---------------------------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		Update()
	}

//---------------------------------------------------------------------------------
	void ExplosionManager()
	{
		SetEventMask(this, EV_FRAME)
		shot = new ShotStorage
		shot.HurtType = HT_EXPLOSION
	}
	
//---------------------------------------------------------------------------------
	void ~ExplosionManager()
	{
		if(shot)
		{
			delete shot
			shot = NULL
		}
	}
}

//=================================================================================
const float FADE_DEBRIS_TIME = 2

class DebrisHeap extends MinClass
{
	ExtClass Debrises[128]
	int		NumDebrises
	float 	Lifetime
	vobject	Objects[16]
	int		NumObjects

	void AddObject(string name)
	{
		Objects[NumObjects++] = GetObject(name)
	}
	
	void EOnTick(ExtClass other, int extra)
	{
		Lifetime -= TICK_PERIOD * 0.6

		if(Lifetime > FADE_DEBRIS_TIME)
			return

		if(Lifetime <= 0)
		{
			for(int n = 0; n < NumDebrises; n++)
			{
				ExtClass ec = Debrises[n]
				NxActorDestroy(ec)
				delete ec
			}
			
			delete this
		}

		float scale = Lifetime * 255 / FADE_DEBRIS_TIME

		clamp scale<0, 255>
		
		int c = AWHITE(scale)
		
		for(n = 0; n < NumDebrises; n++)
		{
			ec = Debrises[n]
			ec.Color = c
		}
	}

	void ~DebrisHeap()
	{
		for(int n = 0; n < NumObjects; n++)
		{
			ReleaseObject(Objects[n], 0)
		}
	}

	void DebrisHeap(float lifetime)
	{
		SetEventMask(this, EV_TICK)
		Lifetime = lifetime + FADE_DEBRIS_TIME
	}
}

//-----------------------------------------------------------------
void WorldClass::M_ShowScreenDisplay(ExtClass other)
{
	string display = other._value
//	display = "displays/camera00_overlay.dpl"
	if(!ScreenDisplay)
	{
		ScreenDisplay = new func_display(this, display, true)
		ClearEventMask(ScreenDisplay, EV_FRAME)	//zastavime cas displeja
	}
}

//-----------------------------------------------------------------
void WorldClass::M_HideScreenDisplay()
{
	if(ScreenDisplay)
	{
		delete ScreenDisplay
		ScreenDisplay = NULL
	}
}

//=================================================================================
class world extends WorldClass
{
	int 		_noshadersun
	vector 	_color
	int 		_minlight
	int 		_minvertexlight
	float		_skyspecular
	vector	gridsize
	int		AtmosphereToxicity

	string MusicAmbient	//cesty k hudbam
	string MusicAtmosphere
	string MusicFight
	string MusicFight2
	string MusicDialog
	string MusicDialog2

	string MAmbientVol	//volumes k hudbam
	string MAtmosphereVol
	string MFightVol
	string MFight2Vol
	string MDialogVol
	string MDialog2Vol

	bool bDoRestingTest

//-----------------------------------------------------------------
	void M_SetEffect(ExtClass other)
	{
		string tokens[16]
		int nump = ParseString(other._value, tokens)

		int eff = atoi(tokens[0])
		float parms[4]

		//defaultni parametry
		switch(eff)
		{
		case 1:
			parms[0] = 255.0
			parms[1] = 0.0
			parms[2] = 0.07
			parms[3] = 0.02
			break
		case 2:
			parms[0] = 31.0
			parms[1] = 2000.0
			parms[2] = 0.0
			parms[3] = 0.0
			break
		case 3:
			eff = 1
			parms[0] = 31.0
			parms[1] = 2000.0
			parms[2] = 0.07
			parms[3] = 0.02
			break
		case 4:
			eff = 3
			parms[0] = 255.0
			parms[1] = 0.0
			parms[2] = 1.0
			parms[3] = 0.0
			break
		}

		int p = 0
		for(int n = 1; n < nump && p < 4; n++)
		{
			if(tokens[n] == ",")
				continue

			parms[p++] = atof(tokens[n])
		}

		SetPostEffect(eff, parms[0], parms[1], parms[2], parms[3])
	}

//-----------------------------------------------------------------
	void M_ChangeLevel(ExtClass other);

//-----------------------------------------------------------------
	void M_PlayMusic(ExtClass other)
	{
		string parm = other._value
		int MusicIndex = -1

		if(parm == "0" || parm == "")
			MusicIndex = BS_NONE

		if(parm == "1" || parm == "")
			MusicIndex = BS_AMBIENT

		if(parm == "2")
			MusicIndex = BS_ATMOSPHERE

		if(parm == "3")
			MusicIndex = BS_FIGHT

		if(parm == "4")
			MusicIndex = BS_FIGHT2

		if(parm == "5")
			MusicIndex = BS_DIALOG

		if(parm == "6")
			MusicIndex = BS_DIALOG2

//		if(MusicIndex < 0)
//			return

		if(MusicPlayer)
			MusicPlayer.PlayBackMusic(this, MusicIndex)
	}

//-----------------------------------------------------------------
	void Quake(float TimeMax, float Power, float Speed, float FadeInTime, float FadeOutTime, int Priority)
	{
		if(ETime > 0)	//ak to uz bezi
		{
			if(Priority	< EPriority)
				return
		}
		
		ETimeMax = TimeMax
		EPower = Power
		ESpeed = 1800 * Speed
		EFadeInTime = FadeInTime
		EFadeOutTime = FadeOutTime
		EPriority = Priority
		
		ETime = 0
		ETime2 = 0
	}

//-----------------------------------------------------------------
	void M_Quake(ExtClass other)
	{
		string Parms[10]
		int NumParms = ParseString(other._value, Parms)

		if(NumParms != 9)
		{
			string Err = "wrong num of parameters " + _name + ".Quake"
			Print(Err)
			return
		}

		float p0 = atof(Parms[0])
		float p1 = atof(Parms[2])
		float p2 = atof(Parms[4])
		float p3 = atof(Parms[6])
		float p4 = atof(Parms[8])

		Quake(p0, p1, p2, p3, p4, 100)
	}

//-----------------------------------------------------------------
	void ApplyExplosion(MinClass actor, vector pos, float radius, float power, float damage, int type )
	{
		ExtClass Bodies[64]
		ExtClass rb
		int BodiesNum
		vector RBpos

		ExplosionManager manager = new ExplosionManager()
		manager.shot.shooter = actor
		manager.shot.from = pos
		manager.shot.radius = radius
		manager.shot.power = power
		manager.shot.damage = damage

		BodiesNum = SphereQuery(pos, radius, Bodies, 64, TFL_VISIBLE)
		
		for(int n = 0; n < BodiesNum; n++)
		{
			rb = Bodies[n]

			if(rb == this)
				continue
			
			if(rb == actor && rb != World.Player)	//nezranujeme ownera vybuchu ale playera ano
				continue

			//TODO: tohle je humus. Predelat na flag identifikujici
			//entity, ktere se daji odbouchnout!
			if(!IsInherited(rb, CMiscPhysicsModel))
			{
				if(!IsInherited(rb, CPhysicsPart))
				{
					if(rb.ClassType != ClassTypeCreature)
					{
						if(!IsInherited(rb, CItemClass))
						{
							if(!IsInherited(rb, CItemWeaponMine))
							{
								if(rb.ClassType != ClassTypeMiscPhysicsGlass)
								{
									if(!IsInherited(rb, CGlassDebris))
										continue
								}
							}
						}
					}
				}
			}

			if(IsInherited(rb, CMiscPhysicsGlass))
				RBpos = GetCenterOfEntity(rb)
			else
			{
				if(rb.ClassType == ClassTypeCreature)	//pre ragdoly potrebujeme stred ziskat takto lebo jeho origin stoji na mieste zabitia
				{
					vector mins, maxs
					GetCBoundBox(rb, mins, maxs)
					RBpos = mins + maxs * 0.5
				}
				else
					RBpos = GetMatrix(rb, 3)
			}
			if(P2PVisibilityEx(pos, RBpos, TRACE_VISTEST|TRACE_NOTRACE) == false)
				continue

			if(manager.BodiesNum >= 64)
				break

			manager.Bodies[manager.BodiesNum] = rb
			manager.Dists[manager.BodiesNum++] = VectorLength(pos - RBpos/*GetMatrix(rb, 3)*/)
			rb.Dependencies++		//davame vediet ze tieto entity nesmu byt zmazane
		}
		
		manager.ToApply = manager.BodiesNum

		string PartEff
		bool quake = true

		switch(type)
		{
			case 0:
				PartEff = ""
				quake = false
				break
			case 1:
				PartEff = "particle/explosion1.ptc"
				break
			case 2:
				PartEff = "particle/explosion2.ptc"
				break
			case 3:
				PartEff = "particle/explosion3.ptc"
				break
			case 4:
				PartEff = "particle/explosion4.ptc"
				break
			case 5:
				PartEff = "particle/explosion5.ptc"
				break
			case 6:	//granat
				PartEff = "particle/tachyonfire.ptc"
				break
		}

		if(PartEff)
		{
	      misc_particle_effector eff = new misc_particle_effector
			eff.SetParms(PartEff, pos, true, 2)
		}

		//pridame trochu bordelu
if(g_physx)
		{
			float c, angle, elev, scale

			NxApplyExplosion(pos, 512, 500)
			DebrisHeap heap = new DebrisHeap(15)
			heap.AddObject("obj/physics/debris1.xob")
			heap.AddObject("obj/physics/debris2.xob")
			heap.AddObject("obj/physics/debris3.xob")
			for(n = 0; n < 120; n++)
			{
				angle = frand(0, M_PI2)
				elev = frand(0, M_PI * 0.5)
				c = cos(elev);

				vector v = Vector(c * cos(angle), c * sin(angle), sin(elev))

				vector zp = v * 16 + pos + "0 0 8"

				if(!TestPosition(World, zp, ZeroVec, ZeroVec, TRACE_DETAIL|TRACE_WORLD))
				{
					continue
				}
				ExtClass ec = new ExtClass
				ec.Color = 0xffffffff

				SetOrigin(ec, zp)
				vector ang
				ang[0] = frand(0, 359.0)
				ang[1] = frand(0, 359.0)
				ang[2] = frand(0, 359.0)

				SetAngles(ec, ang)
				UpdateEntity(ec)

				SetFlags(ec, TFL_VISIBLE|TFL_FULLMATRIX|TFL_NOSHADOW)
				ec.SetObject(heap.Objects[n / 40])
				NxActorCreate(ec, ZeroVec, BC_MODEL, true, 1.5, 0, 0)
				NxActorSetCollisionGroup(ec, 1)
				SetVelocity(ec, v * 120)

				heap.Debrises[heap.NumDebrises++] = ec
			}
		}
		
		float PlaDist = VectorLength(pos - GetMatrix(Player, 3))
		float DistRate
		
		if(PlaDist > 512)
			DistRate = 0.1
		else
		{
			DistRate = PlaDist / 512
			DistRate = 1 - DistRate
		}
		
		if(quake)
		{
			float ExpPow = 5 * DistRate
			Quake(20,ExpPow,20,3,17,50)	
		}

//      misc_particle_effector testeff = new misc_particle_effector
//		testeff.SetParms("particle/animfire_test_smoke.ptc", pos, true, 2)
	}

//Explosion(entita, radius, power)
//-----------------------------------------------------------------
	void M_Explosion(ExtClass other)
	{
		string ErrMsg
		string tokens[16]
		int nump = ParseString(other._value, tokens)
		
		if(nump != 9)
		{
			ErrMsg = "Wrong num of parameters. ApplyExplosion(" + other._value + ")"
			DPrint(ErrMsg)
			return
		}
		
		MinClass cl = FindClass(tokens[0])
		
		if(!cl)
		{
			ErrMsg = "Error in parameter. ApplyExplosion(" + other._value + ")"
			DPrint(ErrMsg)
			return
		}
		
		float radius = atof(tokens[2])
		float power = atof(tokens[4])
		float damage = atof(tokens[6])
		int type = atoi(tokens[8])

		ApplyExplosion(cl, GetMatrix(cl, 3), radius, power, damage, type)
	}
	
//---------------------------------------------------------------------------------
	void LoadNAV(string name, bool free)
	{
		AICreatureClass cc
		info_waypoint wp

		if(free)
		{
			for(cc = GetNextEntity(NULL, CExtClass); cc != NULL; cc = GetNextEntity(cc, CExtClass))
			{
				if(cc.WPosition)
				{
					delete cc.WPosition
					cc.WPosition = NULL
				}
				cc.WAFrame = -1
	
				if(ClassName(cc) == "info_portal")
				{
					//TODO: limit this only to dynamicaly created ones!
					delete cc
					continue
				}
	
				if(cc.ClassType == CTypeInfoWaypoint)
				{
					wp = cc
					wp.Init()
					continue
				}
	
				if(IsInherited(cc, CAICreatureClass))
				{
					if(cc.WPath)
						cc.WPath.Destroy()
	
					cc.WPath = NULL
					cc.WPosition2.Invalidate()
				}
			}
		}
	
		World.pPathFinder = NAVLoad(name)

		if(World.pPathFinder)
		{
			World.pPathFinder.LoadEntities()
	
			const int pts0[16] = {
				-6,  10,
				 6,  10,
				 10,  6,
				 10, -6,
				 6, -10,
				-6, -10,
				-10, -6,
				-10,  6}
			World.pPathFinder.SetShape(0, pts0, 8)
 
			const int pts1[16] = {
				-14,  20,
				 14,  20,
				 20,  14,
				 20, -14,
				 14, -20,
				-14, -20,
				-20, -14,
				-20,  14}
			World.pPathFinder.SetShape(1, pts1, 8)

			const int pts2[16] = {
				-20,  26,
				 20,  26,
				 26,  20,
				 26, -20,
				 20, -26,
				-20, -26,
				-26, -20,
				-26,  20}
			World.pPathFinder.SetShape(2, pts2, 8)

			const int pmaxsize = 96, pmidsize = 48
			const int mmaxsize = -96, mmidsize = -48
			const int pshape[16] = {
				pmidsize, pmaxsize, //0
				pmaxsize, pmidsize,
				pmaxsize, mmidsize, //2
				pmidsize, mmaxsize,
				mmidsize, mmaxsize, //4
				mmaxsize, mmidsize,
				mmaxsize, pmidsize, //6
				mmidsize, pmaxsize}
			World.pPathFinder.SetShape(8, pshape, 8)
	
			string pnames[512], aname
	
			//portal - entity
			int nump = World.pPathFinder.GetPortalNames(pnames, sizeof(pnames))
	
			for(int n = 0; n < nump; n++)
			{
				aname = pnames[n]
					
				info_portal pp = FindClass(aname)
				if(pp)
				{
					//already in map
					if(ClassName(pp) != "info_portal")
					{
						DPrint(ClassName(pp) + " '" + pp._name + "' named as potal!")
						ErrorNum++
					}
					continue
				}

				pp = new info_portal
				pp.SetName(aname)
			}
	
			CPosition pos1 = new CPosition
			CPosition pos2 = new CPosition
			
			for(info_connection wc = GetNextEntity(NULL, CInfoConnection); wc != NULL; wc = GetNextEntity(wc, CInfoConnection))
			{
				info_connection nextwc
	
				if(!wc.link) //could be target for another one-way link
				{
					continue
				}
				
				if(wc.type == 0)
				{
					DPrint("info_connection '" + wc._name + "' has no type set! Ignored!")
					ErrorNum++
					continue
				}

				nextwc = FindClass(wc.link)
				if(!nextwc)
				{
					DPrint("info_connection '" + wc._name + "' can't found link '" + wc.link + "'")
					ErrorNum++
					continue
				}
	
				if(!World.pPathFinder.PositionForPoint(GetMatrix(wc, 3) + "0 0 2", pos1))
				{
					DPrint("info_connection '" + wc._name + "' out of area")
					ErrorNum++
					continue
				}
				
				if(!World.pPathFinder.PositionForPoint(GetMatrix(nextwc, 3) + "0 0 2", pos2)
				{
					DPrint("info_connection '" + nextwc._name + "' out of area")
					ErrorNum++
					continue
				}
	
				if(!IsConnected(pos1, pos2))
				{
					DPrint("info_connection '" + wc._name + "' and '" + nextwc._name + "' aren't in the same group")
					ErrorNum++
					continue
				}
	
				World.pPathFinder.AddConnection(wc, pos1, pos2)
			}
			
			delete pos1
			delete pos2
	
			for(wp = GetNextEntity(NULL, CInfoWaypoint); wp != NULL; wp = GetNextEntity(wp, CInfoWaypoint))
			{
				if(!wp._name || substr(wp._name, 0, 1) == "@")
					continue
	
				CPosition cpos = wp.GetWPos()
				if(!cpos)
				{
					DPrint("info_waypoint " + wp._name + " is not on valid area")
	
				//AddDShape(SHAPE_BBOX, 0x7f7f7f7f, 0, GetMatrix(wp, 3) - "2 2 2", GetMatrix(wp, 3) + "2 2 2")
				//AddDShape(SHAPE_BBOX, 0x7f7f7f7f, 0, wp.coords - "2 2 2", wp.coords + "2 2 2")
	
					ErrorNum++
					continue
				}
	
				//link only cover/shoot points
				if(wp.flags & 6)
				{
					wp.node = World.pPathFinder.LinkWaypoint(wp, cpos)
					if(wp.node == 0)
					{
						DPrint("info_waypoint " + wp._name + " can't be linked")
						ErrorNum++
					}
				}
			}
			
			int usedshapes = 0
			int minshape = 9999
			
			for(cc = GetNextEntity(NULL, CAICreatureClass); cc != NULL; cc = GetNextEntity(cc, CAICreatureClass))
			{
				usedshapes = 1 << cc.ShapeIndex | usedshapes
				
				if(cc.ShapeIndex < minshape)
					minshape = cc.ShapeIndex
			}

			World.pPathFinder.Complete(usedshapes)

			if(free && minshape < 31)
			{
				for(wp = GetNextEntity(NULL, CInfoWaypoint); wp != NULL; wp = GetNextEntity(wp, CInfoWaypoint))
				{
					if(!wp._name)
						continue
		
					cpos = wp.GetWPos()
					if(!cpos)
						continue
		
					if(World.pPathFinder.TestPointCollision(minshape, cpos))
					{
						DPrint("info_waypoint " + wp._name + " is obstructed")
						ErrorNum++
					}
				}
			}
		}
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		LoadNAV("worlds/" + g_sThisLevel + ".nav", true)
	}

//-----------------------------------------------------------------
	void InitPhysics()
	{
		dSetInteractionLayer(ET_WORLD, ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_RAGDOLL|ET_DEBRIS|ET_SHELL|ET_GRENADE|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_DEFAULT, ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_RAGDOLL|ET_DEBRIS|ET_DOOR|ET_GRENADE|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_PLAYER, ET_VEHICLE|ET_PLAYERCLIP|ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_DEBRIS|ET_DOOR, true)
		dSetInteractionLayer(ET_NPC, ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_DEBRIS|ET_DOOR|ET_GRENADE|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_RAGDOLL, ET_WORLD|ET_DEFAULT|ET_RAGDOLL|ET_DEBRIS|ET_DOOR|ET_GRENADE|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_DEBRIS, ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_RAGDOLL|ET_DEBRIS|ET_DOOR|ET_GRENADE, true)
		dSetInteractionLayer(ET_SHELL, ET_WORLD|ET_DEFAULT|ET_SHELL|ET_DOOR|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_DOOR, ET_DEFAULT|ET_PLAYER|ET_NPC|ET_RAGDOLL|ET_DEBRIS|ET_SHELL|ET_DOOR|ET_GRENADE|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_GRENADE, ET_WORLD|ET_DEFAULT|ET_NPC|ET_RAGDOLL|ET_DEBRIS|ET_DOOR|ET_GRENADE|ET_MANIPULATING, true)
		dSetInteractionLayer(ET_MANIPULATING, ET_WORLD|ET_DEFAULT|ET_NPC|ET_RAGDOLL|ET_DOOR|ET_GRENADE|ET_MANIPULATING, true)	//nastavuje sa tomu co drzi player
		
		dSetInteractionLayer(ET_NOCOLLISION, ET_NOCOLLISION, false)
		dSetInteractionLayer(ET_DOOR, ET_DOOR, false)

		dSetInteractionLayer(ET_VEHICLE, ET_GRENADE|ET_MANIPULATING|ET_VEHICLE|ET_PLAYERCLIP|ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_DEBRIS|ET_DOOR, true)

		dSetInteractionLayer(ET_PLAYERCAST, ET_VEHICLE|ET_PLAYERCLIP|ET_WORLD|ET_DEFAULT|ET_NPC|ET_DOOR, true)
		
		dSetInteractionLayer(ET_PLAYERCLIP, ET_PLAYERCAST|ET_PLAYER|ET_VEHICLE, true)

		//dSetInteractionLayer(ET_WORLD, ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_RAGDOLL|ET_DEBRIS|ET_SHELL|ET_DOOR|ET_GRENADE, true)

		dSetInteractionLayer(ET_NPC, ET_NPC, false)

//		NxSceneSetActorGroupPairFlags(0, 1, 0)
	//	NxSceneSetGroupCollision(0, 0, false)	//world-world
		NxSceneSetGroupCollision(0, 0, true)	//world-world
		NxSceneSetGroupCollision(0, 1, true)	//world-rigidbody
		NxSceneSetGroupCollision(1, 2, true)	//rigidbody-func
		NxSceneSetGroupCollision(0, 2, false)	//world-func
	}
	
//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{
		LoadNAV("worlds/" + g_sThisLevel + ".nav", false)
		InitPhysics()
		ErrorNum = 0
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
#ifdef DEVELOPER
		InitDirector(this)
#endif
	}

//-----------------------------------------------------------------
	void EOnTimer(MinClass other, int extra)
	{
		if(!DebugLevel && other == Player)
		{
			if(!GameOverSnd)
				GameOverSnd = GetSoundEx("sound/ambiences/end_title.ogg", 1, SF_STREAM)

			GameOverSound = PlaySound(NULL, GameOverSnd, SF_AMBIENT|SF_MUSIC)

			if(MusicPlayer)
				MusicPlayer.PlayBackMusic(this, BS_NONE)
		}
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(ScreenDisplay)	//updatujeme fullscreen display
		{
			ScreenDisplay.UpdateTime()
			ScreenDisplay.UpdateElements()
		}

		if(BulletTimeFade == 0)	//ak nebezi bullettime efekt ani ten v credits
		{
			if(r_bloom)
				SetPostEffect(6, r_gbias, r_blurscale, r_imgscale, r_gauss)
			else
				SetPostEffect(0, 0, 0, 0, 0)
		}

#ifdef DEVELOPER
		if(g_FromConsoleCall != "")	//volanie dynamickych metod z konzole
		{
			int lng = strlen(g_FromConsoleCall)

			if(lng > 2)
				CallMethods(this, g_FromConsoleCall)

			g_FromConsoleCall = ""
		}
#endif

		if(ETime < ETimeMax)
		{
			CalcMatrixFromCamera()

			float QMult, EFade
			ETime += 100 * ftime
			ETime2 += ESpeed * ftime
			EFade = 1

			if(EFadeInTime > 0)								//pokial nastaveny fadeIn/Out
			{
				if(ETime < EFadeInTime)					//rozjazd
					EFade = ETime / EFadeInTime
				else
					EFadeInTime = 0
			}

			if(EFadeOutTime > 0)
			{
				float tmp = ETimeMax - EFadeOutTime
				float ff
				if(ETime >= tmp)						//dojazd
				{
					ff = ETimeMax - ETime
					EFade = ff / EFadeOutTime
				}
			}

			float rr = ETime2 * 1.1
			rr = sin(rr * DEG2RAD)
			QMult = 2 * EPower * EFade
			vector vv = g_CamMat[0] * QMult		//g_CamMat[2] hore - dole
			EarthquakeOff = vv * rr

			rr = sin(ETime2 * DEG2RAD)
			QMult = 2 * EPower * EFade
			EarthquakeAng[2] = rr * QMult

			float QVert = ETime2 * 1.2
			rr = sin(QVert * DEG2RAD)
			QMult = 1 * EPower * EFade
			EarthquakeAng[0] = rr * QMult
		}
		else
		{
			ETime = ETimeMax
			EPriority = 0
			EarthquakeOff = ZeroVec
			EarthquakeAng = ZeroVec
		}
/*
		if(KeyState(KC_B))
		{
			ClearKey(KC_B)
//			_value = "500, 1, 1, 100, 100"
			_value = "1000, 0.4, 1.8, 200, 200"
			M_Quake(this)
		}
/*/
/*
		if(KeyState(KC_B))
		{
			ClearKey(KC_B)
			M_Quake(this)
		}
*/

		if(CutScene == true)
			return

		AICreatureClass cl

#ifdef DEVELOPER
		if(DirectorActive == true)
		{
			ProcessDirector(this)
			return
		}

		if(KeyState(KC_RCONTROL))
		{
			if(KeyState(KC_A))
			{
				ClearKey(KC_A)
				
			
				g_bDoShowAI = true
			}

			if(KeyState(KC_F))
			{
				ClearKey(KC_F)

				g_bFreezeAI = 1 - g_bFreezeAI
				for(cl = GetNextEntity(NULL, CAICreatureClass); cl != NULL; cl = GetNextEntity(cl, CAICreatureClass))
				{
					cl.Stopped = g_bFreezeAI
				}
				
				if(g_bFreezeAI && !g_bShowAI)
				{
					g_bDoShowAI = true
				}
				else
				{
					if(!g_bFreezeAI && g_bShowAI)
						g_bDoShowAI = true
				}
			}

			if(KeyState(KC_X))
			{
				ClearKey(KC_X)
				showfilter()
			}
		}

		if( KeyState(KC_F11) != false)
		{
			ClearKey(KC_F11)
			ShowDirector(this)
		}

		if(KeyState(KC_F2))
		{
			ClearKey(KC_F2)
			bDoRestingTest = true
		}

		if(bDoRestingTest)
		{
			bDoRestingTest = false

			for(cl = GetNextEntity(NULL, CExtClass); cl != NULL; cl = GetNextEntity(cl, CExtClass))
			{
				if(IsInherited(cl, CCreatureClass))
					continue

				if(cl.IsRigidBody() && dBodyIsActive(cl))
				{
					Print(String("Warning: Physical entity is not at rest " + ClassName(cl) + " " + cl._name))
				}
			}
		}

#endif

		if(g_bDoShowAI)
		{
			g_bDoShowAI = false

			for(cl = GetNextEntity(NULL, CCreatureClass); cl != NULL; cl = GetNextEntity(cl, CCreatureClass))
			{
				if(g_bShowAI)
				{
					if(cl.WAI)
						ShowWidget(cl.WAI, false)
					cl.ShowAI = false
				}
				else
				{
					if(cl.status < STATE_DIE)
					{
						if(cl.WAI == NULL)
							cl.WAI = CreateWidget(WTYPE_TEXT, 0, 0, 160, 256, WF_SOURCEALPHA|WF_BLEND|WF_EXACTPOS, 0xe0ffffff, 255)
	
						ShowWidget(cl.WAI, true)
						cl.ShowAI = true
					}
				}
			}

			g_bShowAI = 1 - g_bShowAI
		}
	}

//-----------------------------------------------------------------
	void SetMusicPaths()
	{
		SoundPaths[BS_AMBIENT]	  = MusicAmbient
		SoundPaths[BS_ATMOSPHERE] = MusicAtmosphere
		SoundPaths[BS_FIGHT]		  = MusicFight
		SoundPaths[BS_FIGHT2]	  = MusicFight2
		SoundPaths[BS_DIALOG]	  = MusicDialog
		SoundPaths[BS_DIALOG2]	  = MusicDialog2

		SoundVolumes[BS_AMBIENT]	 = atof(MAmbientVol)
		SoundVolumes[BS_ATMOSPHERE] = atof(MAtmosphereVol)
		SoundVolumes[BS_FIGHT]		 = atof(MFightVol)
		SoundVolumes[BS_FIGHT2]		 = atof(MFight2Vol)
		SoundVolumes[BS_DIALOG]		 = atof(MDialogVol)
		SoundVolumes[BS_DIALOG2]	 = atof(MDialog2Vol)
	}

//-----------------------------------------------------------------
	void world()
	{
		ClassType = ClassTypeWorld
		SetEventMask(this,EV_FRAME|EV_INIT|EV_ONLOAD|EV_TIMER|EV_LOADED)

		name = g_sThisLevel
		
		//bDoRestingTest = true

		ETime = 0
		ETimeMax = 0
		EPriority = 0

		if(!ItemColPtr)
			ItemColPtr = new ItemCollection

		if(!DispColPtr)
			DispColPtr = new DisplayCollection

		InitSpeechSet()

		SetMusicPaths()
		
		if(viewdist == 0)
			viewdist = 16432

		clamp AtmosphereToxicity<0, 1>	//0 = maximum 1 = medium 2 = low

		InitPhysics()
	}

	void ~world()
	{
		if(ItemColPtr)
		{
			delete ItemColPtr
			ItemColPtr = NULL
		}

		if(DispColPtr)
		{
			delete DispColPtr
			DispColPtr = NULL
		}

		if(GameOverSound)
		{
			EndSound(GameOverSound)
			GameOverSound = 0
		}

		ReleaseRicParticles()

#ifdef EDITOR
		g_EditorRuns = -1
#endif

		_eclass ec = GetNextEntity(NULL, CJointClass)	//najprv mazeme jointy
		_eclass current
		while(ec)
		{
			current = ec
			ec = GetNextEntity(ec, CJointClass)
		
			if(IsInherited(current, CJointClass))
				delete current
		}

		//world se smaze jako posledni
		for(ec = GetNextEntity(NULL, NULL); ec != NULL; ec = GetNextEntity(ec, NULL))
		{
			if(this != ec)
				delete ec
		}

		CleanupAnimSets()
	}
}


//=================================================================================

#ifdef EDITOR
#include "scripts/editor_path.h"
#include "scripts/editor.h"
#endif

#include "scripts/console.h"

int	ItemValues[64],ItemValuesBak[64]

//=================================================================================
void SaveConfig()
{
	int f = OpenFile("settings.cfg",FILEMODE_WRITE)

	if(f)
	{
		FPrintln(f, "//ENGINE CONFIG FILE. DO NOT MODIFY!!!")
		FPrintln(f, " ")

		FPrintln(f, "//video options")

		//parse video mode
		string vmode = ""

		if(g_iVMode >= 0)
			vmode = g_sVideoModes[g_iVMode]
		
		int c = strstr(vmode, "x")
		if(c == -1 || vmode == "")
		{
			FPrint(f, "ScreenWidth ")
			FPrintln(f, itoa(g_iWidth))
	
			FPrint(f, "ScreenHeight ")
			FPrintln(f, itoa(g_iHeight))
		}
		else
		{
			FPrint(f, "ScreenWidth ")
			FPrintln(f, substr(vmode, 0, c))
	
			vmode = substr(vmode, c + 1, strlen(vmode) - c - 1)
			c = strstr(vmode, "x")			
			FPrint(f, "ScreenHeight ")
			if(c == -1)
				FPrintln(f, vmode)
			else			
				FPrintln(f, substr(vmode, 0, c))
		}
	
		FPrint(f, "Fullscreen ")
		FPrintln(f, g_iFullscreen)

		FPrint(f, "FSAA ")
		FPrintln(f, g_iFSAA)

		FPrint(f, "TextureDepth ")
		FPrintln(f, r_texcompress)

		FPrint(f, "TextureDetail ")
		FPrintln(f, r_texdetail)

		FPrint(f, "Bloom ")
		FPrintln(f, r_bloom)

		FPrint(f, "Dither ")
		FPrintln(f, g_iDither)

		FPrint(f, "TextureFilter ")
		FPrintln(f, r_texfilter)

		FPrint(f, "MaxAniso ")
		FPrintln(f, r_maxaniso)

		FPrint(f, "ShaderLevel ")
		FPrintln(f, r_shaderlevel)

		FPrint(f, "GeometricDetail ")
		FPrintln(f, g_iGeomDetail)

		FPrint(f, "Brightness ")
		FPrintln(f, g_iBrightness)

		FPrint(f, "VSync ")
		FPrintln(f, g_iVSync)

		FPrintln(f, "//audio options")

		FPrint(f, "EffectsVolume ")
		FPrintln(f, g_iEffectsVolume)

		FPrint(f, "MusicVolume ")
		FPrintln(f, g_iMusicVolume)

		FPrint(f, "SoundQuality ")
		FPrintln(f, g_iSoundQuality)

		FPrint(f, "OutputRate ")
		FPrintln(f, g_iOutputRate)

		FPrint(f, "Speakers ")
		FPrintln(f, g_iSpeakers)

		FPrint(f, "EnableEAX ")
		FPrintln(f, g_bEAX)

		FPrint(f, "EnableSoundOcclusion ")
		FPrintln(f, g_bSoundOcclusion)

		FPrintln(f, "//advanced options")

		FPrint(f, "ModelShadows ")
		FPrintln(f, r_shadow)

		FPrint(f, "Reflections ")
		FPrintln(f, r_reflection)

		FPrint(f, "Bump ")
		FPrintln(f, r_normal)

		FPrint(f, "Parallax ")
		FPrintln(f, r_parallax)

		FPrint(f, "TerrainQuality ")
		FPrintln(f, r_terrquality)

		FPrint(f, "TerrainSpecular ")
		FPrintln(f, r_terrspecular)

		FPrint(f, "TerrainParallax ")
		FPrintln(f, r_terrparallax)

		FPrint(f, "ScreenQuality ")
		FPrintln(f, r_screenquality)

		FPrint(f, "PhysX ")
		FPrintln(f, g_physx)

		FPrintln(f, "")
		FPrintln(f, "//Defined primary and secondary control keys")

		FPrint(f, "Move_Forward_1 ")
		FPrintln(f, g_KeyDef1[MOVE_F])
		FPrint(f, "Move_Forward_2 ")
		FPrintln(f, g_KeyDef2[MOVE_F])

		FPrint(f, "Move_Backward_1 ")
		FPrintln(f, g_KeyDef1[MOVE_B])
		FPrint(f, "Move_Backward_2 ")
		FPrintln(f, g_KeyDef2[MOVE_B])

		FPrint(f, "Move_Left_1 ")
		FPrintln(f, g_KeyDef1[LEFT])
		FPrint(f, "Move_Left_2 ")
		FPrintln(f, g_KeyDef2[LEFT])

		FPrint(f, "Move_Right_1 ")
		FPrintln(f, g_KeyDef1[RIGHT])
		FPrint(f, "Move_Right_2 ")
		FPrintln(f, g_KeyDef2[RIGHT])

		FPrint(f, "Jump_1 ")
		FPrintln(f, g_KeyDef1[JUMP])
		FPrint(f, "Jump_2 ")
		FPrintln(f, g_KeyDef2[JUMP])

		FPrint(f, "Crouch_1 ")
		FPrintln(f, g_KeyDef1[CROUCH])
		FPrint(f, "Crouch_2 ")
		FPrintln(f, g_KeyDef2[CROUCH])

		FPrint(f, "Walk_1 ")
		FPrintln(f, g_KeyDef1[WALK_MODE])
		FPrint(f, "Walk_2 ")
		FPrintln(f, g_KeyDef2[WALK_MODE])

		FPrint(f, "Prim_Attack_1 ")
		FPrintln(f, g_KeyDef1[P_ATTACK])
		FPrint(f, "Prim_Attack_2 ")
		FPrintln(f, g_KeyDef2[P_ATTACK])

		FPrint(f, "SecAttack_1 ")
		FPrintln(f, g_KeyDef1[S_ATTACK])
		FPrint(f, "SecAttack_2 ")
		FPrintln(f, g_KeyDef2[S_ATTACK])		
		
		FPrint(f, "Prev_Weapon_1 ")
		FPrintln(f, g_KeyDef1[PREV_WEAPON])
		FPrint(f, "Prev_Weapon_2 ")
		FPrintln(f, g_KeyDef2[PREV_WEAPON])
		
		FPrint(f, "Next_Weapon_1 ")
		FPrintln(f, g_KeyDef1[NEXT_WEAPON])
		FPrint(f, "Next_Weapon_2 ")
		FPrintln(f, g_KeyDef2[NEXT_WEAPON])
				
		FPrint(f, "Reload_1 ")
		FPrintln(f, g_KeyDef1[RELOAD])
		FPrint(f, "Reload_2 ")
		FPrintln(f, g_KeyDef2[RELOAD])		
		
		FPrint(f, "Action_(Use)_1 ")
		FPrintln(f, g_KeyDef1[USE])
		FPrint(f, "Action_(Use)_2 ")
		FPrintln(f, g_KeyDef2[USE])

		FPrint(f, "Flashlight_1 ")
		FPrintln(f, g_KeyDef1[FLASHLIGHT])
		FPrint(f, "Flashlight_2 ")
		FPrintln(f, g_KeyDef2[FLASHLIGHT])

		FPrint(f, "RemoteController_1 ")
		FPrintln(f, g_KeyDef1[CONTROLLER])
		FPrint(f, "RemoteController_2 ")
		FPrintln(f, g_KeyDef2[CONTROLLER])

		FPrint(f, "BulletTime_1 ")
		FPrintln(f, g_KeyDef1[BULLET_TIME])
		FPrint(f, "BulletTime_2 ")
		FPrintln(f, g_KeyDef2[BULLET_TIME])

		FPrint(f, "TaskList_1 ")
		FPrintln(f, g_KeyDef1[TASKLIST])
		FPrint(f, "TaskList_2 ")
		FPrintln(f, g_KeyDef2[TASKLIST])

		FPrint(f, "LeanLeft_1 ")
		FPrintln(f, g_KeyDef1[LEAN_LEFT])
		FPrint(f, "LeanLeft_2 ")
		FPrintln(f, g_KeyDef2[LEAN_LEFT])

		FPrint(f, "LeanRight_1 ")
		FPrintln(f, g_KeyDef1[LEAN_RIGHT])
		FPrint(f, "LeanRight_2 ")
		FPrintln(f, g_KeyDef2[LEAN_RIGHT])

		FPrint(f, "Jackhammer_1 ")
		FPrintln(f, g_KeyDef1[WEAP_JACKHAMMER])
		FPrint(f, "Jackhammer_2 ")
		FPrintln(f, g_KeyDef2[WEAP_JACKHAMMER])
		
		FPrint(f, "Pistol_1 ")
		FPrintln(f, g_KeyDef1[WEAP_PISTOL])
		FPrint(f, "Pistol_2 ")
		FPrintln(f, g_KeyDef2[WEAP_PISTOL])
		
		FPrint(f, "Shotgun_1 ")
		FPrintln(f, g_KeyDef1[WEAP_SHOTGUN])
		FPrint(f, "Shotgun_2 ")
		FPrintln(f, g_KeyDef2[WEAP_SHOTGUN])
		
		FPrint(f, "Machinegun_1 ")
		FPrintln(f, g_KeyDef1[WEAP_MACHINEGUN])
		FPrint(f, "Machinegun_2 ")
		FPrintln(f, g_KeyDef2[WEAP_MACHINEGUN])
		
		FPrint(f, "SniperRifle_1 ")
		FPrintln(f, g_KeyDef1[WEAP_SNIPER_RIFLE])
		FPrint(f, "SniperRifle_2 ")
		FPrintln(f, g_KeyDef2[WEAP_SNIPER_RIFLE])
		
		FPrint(f, "RocketLauncher_1 ")
		FPrintln(f, g_KeyDef1[WEAP_ROCKET_LAUNCHER])
		FPrint(f, "RocketLauncher_2 ")
		FPrintln(f, g_KeyDef2[WEAP_ROCKET_LAUNCHER])
		
		FPrint(f, "Flamethrower_1 ")
		FPrintln(f, g_KeyDef1[WEAP_FLAMETHROWER])
		FPrint(f, "Flamethrower_2 ")
		FPrintln(f, g_KeyDef2[WEAP_FLAMETHROWER])
		
		FPrint(f, "Grenade_1 ")
		FPrintln(f, g_KeyDef1[WEAP_GRENADE])
		FPrint(f, "Grenade_2 ")
		FPrintln(f, g_KeyDef2[WEAP_GRENADE])

		FPrint(f, "QuickLoad_1 ")
		FPrintln(f, g_KeyDef1[QUICKLOAD])
		FPrint(f, "QuickLoad_2 ")
		FPrintln(f, g_KeyDef2[QUICKLOAD])

		FPrint(f, "QuickSave_1 ")
		FPrintln(f, g_KeyDef1[QUICKSAVE])
		FPrint(f, "QuickSave_2 ")
		FPrintln(f, g_KeyDef2[QUICKSAVE])

		FPrint(f, "InvertMouse ")
		FPrintln(f, g_InvMouse)

		FPrint(f, "MouseSpeedH ")
		FPrintln(f, g_MSpeedH)

		FPrint(f, "MouseSpeedV ")
		FPrintln(f, g_MSpeedV)

		FPrint(f, "Joypad ")
		FPrintln(f, g_Joypad)

		FPrint(f, "Subtitles ")
		FPrintln(f, g_Subtitles)

		CloseFile(f)
	}
}

//=================================================================================
int WidthFromVMode(int vmod)
{
	if(vmod < 0)
		return 640

	string vmode = g_sVideoModes[vmod]
		
	int c = strstr(vmode, "x")
	
	return atoi(substr(vmode, 0, c))
}

//=================================================================================
int HeightFromVMode(int vmod)
{
	if(vmod < 0)
		return 480

	string vmode = g_sVideoModes[vmod]
		
	int c = strstr(vmode, "x")
	
	vmode = substr(vmode, c + 1, strlen(vmode) - c - 1)
	c = strstr(vmode, "x")			
	if(c == -1)
		return atoi(vmode)

	return atoi(substr(vmode, 0, c))
}

//=================================================================================
void ApplyOptions()
{
	int val

//user just changed resolution. Reset it!
	if(g_iVMode != ItemValues[10] || g_iFSAA != ItemValues[61])
	{
		g_iVMode = ItemValues[10]
		g_iFSAA = ItemValues[61]
		
		g_iWidth = WidthFromVMode(g_iVMode)
		g_iHeight = HeightFromVMode(g_iVMode)
		
		ResetRender(g_iWidth, g_iHeight)
	}

//Video options
	r_shaderlevel = ItemValues[21]
	g_iGeomDetail = ItemValues[22]
	r_texfilter = ItemValues[23]
	r_maxaniso = ItemValues[50] + 1
	val = ItemValues[24]
	r_texcompress = val
	g_iBrightness = ItemValues[25]

//Audio options
	g_iEffectsVolume = ItemValues[26]
	g_iMusicVolume = ItemValues[27]
	g_iSoundQuality = ItemValues[28]
	g_bEAX = ItemValues[11]
	g_iOutputRate = ItemValues[12]
	g_iSpeakers = ItemValues[13]
	g_bSoundOcclusion = ItemValues[14]

//Advanced
	r_shadow = ItemValues[30]
	r_reflection = ItemValues[31]
//	r_normal = ItemValues[32]
	r_specular = ItemValues[33]
	r_texdetail = ItemValues[34]
	r_terrparallax = ItemValues[35]
	r_terrspecular = ItemValues[36]
	r_terrquality = ItemValues[37]

	r_screenquality = ItemValues[45]
	g_physx = ItemValues[46]

//Controls
	for(int n = 0; n < sizeof(g_KeyDef1); n++)
	{
		g_KeyDef1[n] = g_KeyDef1_temp[n]
		g_KeyDef2[n] = g_KeyDef2_temp[n]
	}

	g_InvMouse = ItemValues[51]
	g_MSpeedH = ItemValues[52]
	g_MSpeedV = ItemValues[53]
	g_Subtitles = ItemValues[55]
	g_Joypad = ItemValues[56]

	g_fMouseSpeedH = g_MSpeedH * 0.0005 + 0.005
	if(g_InvMouse)
		g_fMouseSpeedV = -g_MSpeedV * 0.00035 - 0.00125
	else
		g_fMouseSpeedV = g_MSpeedV * 0.00035 + 0.00125
	
	r_bloom = ItemValues[60]
}

//=================================================================================
void GetOptions()
{
//Video options
	ItemValues[10] = g_iVMode
	ItemValues[61] = g_iFSAA
	ItemValues[21] = r_shaderlevel
	ItemValues[22] = g_iGeomDetail
	ItemValues[23] = r_texfilter
	ItemValues[24] = r_texcompress
	ItemValues[25] = g_iBrightness
	clamp r_maxaniso<1, 16>
	ItemValues[50] = r_maxaniso - 1

//Audio options
	ItemValues[11] = g_bEAX
	ItemValues[12] = g_iOutputRate
	ItemValues[13] = g_iSpeakers
	ItemValues[14] = g_bSoundOcclusion

	ItemValues[26] = g_iEffectsVolume
	ItemValues[27] = g_iMusicVolume
	ItemValues[28] = g_iSoundQuality

//Advanced
	ItemValues[30] = r_shadow
	ItemValues[31] = r_reflection
	ItemValues[33] = r_specular
	ItemValues[34] = r_texdetail
	ItemValues[35] = r_terrparallax
	ItemValues[36] = r_terrspecular
	ItemValues[37] = r_terrquality

	ItemValues[45] = r_screenquality
	ItemValues[46] = g_physx

//Controls
	for(int n = 0; n < sizeof(g_KeyDef1); n++)
	{
		g_KeyDef1_temp[n] = g_KeyDef1[n]
		g_KeyDef2_temp[n] = g_KeyDef2[n]
	}

	ItemValues[51] = g_InvMouse
	ItemValues[52] = g_MSpeedH
	ItemValues[53] = g_MSpeedV
	ItemValues[55] = g_Subtitles
	ItemValues[56] = g_Joypad
	ItemValues[60] = r_bloom
}

void LoadMenuMap()
{
	if(MenuMap)
		return

	MenuMap = LoadMap("worlds/menu.wrl", true)

	if(MenuMap != NULL)
	{
		SetWidgetWorld(WView, MenuMap, 0)
		ShowWidget(WView, true)
	}
}

void UnloadMenuMap()
{
	SetWidgetWorld(WView, NULL, 0)
	ShowWidget(WView, false)

	if(MenuMap != NULL)
	{
		FreeMap(MenuMap)
		MenuMap = NULL
	}
}

//-----------------------------------------------------------------
bool CompareMouseGlobal(int l, int t, int r, int b)
{
	r = l + r
	b = t + b
	if(MouseX < l || MouseX > r || MouseY < t || MouseY > b)
		return false

	return true
}


#include "scripts/menu.h"
#include "scripts/player/interface.h"
#include "scripts/player/player.h"

//-----------------------------------------------------------------
void world::M_ChangeLevel(ExtClass other)
{
#ifdef DEVELOPER
	Print(String("Changing Next Level. Caller is " + other._name))
#endif

#ifndef EDITOR
	string Value = other._value
	strlower(Value)
	string Parms[8]
	int NumParms = ParseString(Value, Parms)

	bool NoFade = false

	if(NumParms == 3 && Parms[2] == "nofade")
		NoFade = true

	if(CutScene)
	{
		//if we were playing cutscene, get back our FOV
		SetCameraVerticalFOV(0, pfov)
	}
	
	creature_player pla = World.Player
	pla.WriteCurrentStorageTo(g_PlayerStorage)	//prenasame playerove veci z levelu do levelu. musi to byt tu pretoze jedine to co je volane z ingame scriptu je naozaj dalsi level
	change_level(Parms[0], NoFade)
#endif
}

//=================================================================================
bool LoadMapCallback(string name, int progr)
{

#ifdef EDITOR
	if(GetAsyncKeyState(0x1b) & 0x8000)
	{
		return false
	}
#endif

	if(!g_sThisLevel)
	{
		int pos = strstr(name, ".wrl")
		if(pos >= 0)
			g_sThisLevel = substr(name, 0, pos)

		pos = strstr(g_sThisLevel, "/")
		
		if(pos >= 0)
			g_sThisLevel = substr(name, pos + 1, strlen(g_sThisLevel) - pos - 1)
	}

/*	
	if(LevelNumber)
	{
		SetWidgetText(LevelNumber, 0, g_sThisLevel)
		ShowWidget(LevelNumber, true)
	}
*/

	if(g_wProgress)
		SetWidgetPos(g_wProgress, 14, 573, 774 * progr / 1000, 16)

#ifdef EDITOR
		SetLoadProgress(progr)
#endif

	return true
}

//=================================================================================
void Back2Game(int map)
{
	g_iMInGame = 1
	ShowWidget(WView, true)
	ShowWidget(WMousePointer, false)

	SetWidgetPos(WMousePointer, 400, 300, 63, 63)
	SetWidgetWorld(WView, map, 0)
	EndSound(MenuMusicID)
	MenuMusicID = 0

	MouseX = 0
	MouseY = 0
	MouseDeltaX = 0
	MouseDeltaY = 0

#ifdef _XBOX
	ClearJoyButton(JB_A)
#else
	KEY(P_ATTACK, true)	//vyclearujeme to pretoze po kliknuti na resume player hned nieco robi
#endif
}

//=================================================================================
void Wait4Key()
{
	SetWidgetPos(g_wProgress, 14, 573, 774, 16)
	g_iMInGame = -1

	SetWidgetTextEx(g_wLoading, 0, true, "#LoadWaitingText")
}

//=================================================================================
void BeforeLoading(string name)
{
	Print(String("Loading level : " + name))

	g_wProgress = CreateWidget(WTYPE_IMAGE, 233, 530, 355, 16, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 127)
	LoadWidgetImage(g_wProgress, 0, "gfx/hud_misc/progressbar")

	int wWidth = 800
	int wHeight = 600
	int wflags = WF_VISIBLE
				
	g_wBackground = CreateWidget(WTYPE_IMAGE, 0, 0, wWidth, wHeight, wflags, 0xffffffff, 126)
	string LScreen = "gfx/backgrounds/"

	if(!LoadWidgetImage(g_wBackground, 0, "worlds/" + name))
	{
		LScreen = "gfx/hud_misc/loading_screen_default.dds"
		LoadWidgetImage(g_wBackground, 0, LScreen)
	}

	g_wLoading = CreateWidget(WTYPE_TEXT, 200, 545, 400, 33, WF_BLEND|WF_VISIBLE|WF_CENTER, ARGB(255,130,255,255), 130)
	SetFont(g_wLoading, MainFont)
	SetWidgetTextEx(g_wLoading, 0, true, "#LoadingText")

	g_bEndLevel = false
	g_iMInGame = 0
	g_sThisLevel = name
}

//=================================================================================
void AfterLoading()
{
	if(g_wBackground)
	{
		DestroyWidget(g_wBackground)
		g_wBackground = NULL
	}

	if(g_wProgress)
	{
		DestroyWidget(g_wProgress)
		g_wProgress = NULL
	}

	if(g_wLoading)
	{
		DestroyWidget(g_wLoading)
		g_wLoading = NULL
	}

	//zasejvujeme na zaciatku levelu ale nie ked je to z Loadnutej pozicie alebo ked je mapa pustena cez viewer alebo to je spustene cez tutorial button
	if(g_FromSavePos == false && g_RunMode == RUNMODE_NORMAL && g_sThisLevel != "l0i")
		NeedAutosave = true

	KEY(P_ATTACK, true)
}

//=================================================================================
int LoadMapEx(string name)
{
	BeforeLoading(name)

#ifdef DEMO
	if(substr(name, 0, 1) == "l")
	{
		if(substr(name, 1, 1) == "0")
		{
			if(substr(name, 2, 1) == "1" || substr(name, 2, 1) == "3")
			{
				goto valid
			}
		}
	}
	return 0
valid:;
#endif

	int map = LoadMap("worlds/" + name + ".wrl", true)

	if(map != NULL)
	{
		Back2Game(map)
		g_bStartLevel = true
		LoadBlackWidget()

		//hod ho do cekaciho kodu
		if(!DebugLevel)
		{
			Wait4Key()
			return map
		}
	}
	AfterLoading()
	return map
}

//=================================================================================
int LoadSaveMapEx(string name, string sf)
{
	BeforeLoading(name)

	g_FromSavePos = true

	SetWidgetWorld(WView, NULL, 0)
	int map = LoadSaveMap(globalmap, sf)

	if(map)
	{
		Back2Game(map)
		g_bStartLevel = true
		LoadBlackWidget()

		//hod ho do cekaciho kodu
		if(!DebugLevel)
		{
			Wait4Key()
			return map
		}
	}

	AfterLoading()
	return map
}

//=================================================================================
void UnloadMap()
{
	SetWidgetWorld(WView, NULL, 0)
	ShowWidget(WView, false)
	if(globalmap != NULL)
	{
		FreeMap(globalmap)
		globalmap = NULL
	}
}


//=================================================================================
void SetConfigValue(string item, string value)
{
	switch(item)
	{
	case "ScreenDepth":
		break

	case "Bloom":
		r_bloom = atoi(value)
		break

	case "FSAA":
		g_iFSAA = atoi(value)
		break

	case "ScreenWidth":
		g_iWidth = atoi(value)
		break

	case "ScreenHeight":
		g_iHeight = atoi(value)
		break

	case "Fullscreen":
		g_iFullscreen = atoi(value)
		break

	case "TextureDepth":
		r_texcompress = atoi(value)
		if(r_texcompress == 32)
			r_texcompress = 1
		break

	case "TextureDetail":
		r_texdetail = atoi(value)
		break

	case "Dither":
		g_iDither = atoi(value)
		break

	case "TextureFilter":
		r_texfilter = atoi(value)
		break

	case "MaxAniso":
		r_maxaniso = atoi(value)
		break

	case "ShaderLevel":
		r_shaderlevel = atoi(value)
		clamp r_shaderlevel<0, r_maxshaderlevel>
		break

	case "GeometricDetail":
		g_iGeomDetail = atoi(value)
		break

	case "DetailMap":
		break

	case "Brightness":
		g_iBrightness = atoi(value)
		break

	case "VSync":
		g_iVSync = atoi(value)
		break

	case "EffectsVolume":
		g_iEffectsVolume = atoi(value)
		break

	case "MusicVolume":
		g_iMusicVolume = atoi(value)
		break

	case "SoundQuality":
		g_iSoundQuality = atoi(value)
		break

	case "Speakers":
		g_iSpeakers = atoi(value)
		break

	case "OutputRate":
		g_iOutputRate = atoi(value)
		break

	case "EnableEAX":
		g_bEAX = atoi(value)
		break

	case "EnableSoundOcclusion":
		g_bSoundOcclusion = atoi(value)
		break

	case "ModelShadows":
		r_shadow = atoi(value)
		clamp r_shadow<0, 1>
		break

	case "Reflections":
		r_reflection = atoi(value)
		break

	case "Bump":
		r_normal = atoi(value)
		break

	case "Parallax":
		r_parallax = atoi(value)
		break

	case "TerrainQuality":
		r_terrquality = atoi(value)
		break

	case "TerrainSpecular":
		r_terrspecular = atoi(value)
		break

	case "TerrainParallax":
		r_terrparallax = atoi(value)
		break

	case "PPQuality":
		r_screenquality = atoi(value)
		break

	case "ScreenQuality":
		r_screenquality = atoi(value)
		break

	case "PhysX":
		g_physx = atoi(value)
		break		

	case "Move_Forward_1":
		g_KeyDef1[MOVE_F] = atoi(value)
		break

	case "Move_Backward_1":
		g_KeyDef1[MOVE_B] = atoi(value)
		break

	case "Move_Left_1":
		g_KeyDef1[LEFT] = atoi(value)
		break

	case "Move_Right_1":
		g_KeyDef1[RIGHT] = atoi(value)
		break
		
	case "Jump_1":
		g_KeyDef1[JUMP] = atoi(value)
		break

	case "Crouch_1":
		g_KeyDef1[CROUCH] = atoi(value)
		break
		
	case "Walk_1":
		g_KeyDef1[WALK_MODE] = atoi(value)
		break
		
	case "Prim_Attack_1":
		g_KeyDef1[P_ATTACK] = atoi(value)
		break

	case "SecAttack_1":
		g_KeyDef1[S_ATTACK] = atoi(value)
		break		

	case "Next_Weapon_1":
		g_KeyDef1[NEXT_WEAPON] = atoi(value)
		break

	case "Prev_Weapon_1":
		g_KeyDef1[PREV_WEAPON] = atoi(value)
		break
		
	case "Reload_1":
		g_KeyDef1[RELOAD] = atoi(value)
		break

	case "Action_(Use)_1":
		g_KeyDef1[USE] = atoi(value)
		break

	case "Flashlight_1":
		g_KeyDef1[FLASHLIGHT] = atoi(value)
		break

	case "RemoteController_1":
		g_KeyDef1[CONTROLLER] = atoi(value)
		break

	case "BulletTime_1":
		g_KeyDef1[BULLET_TIME] = atoi(value)
		break

	case "TaskList_1":
		g_KeyDef1[TASKLIST] = atoi(value)
		break

	case "LeanLeft_1":
		g_KeyDef1[LEAN_LEFT] = atoi(value)
		break

	case "LeanRight_1":
		g_KeyDef1[LEAN_RIGHT] = atoi(value)
		break

	case "Jackhammer_1":
		g_KeyDef1[WEAP_JACKHAMMER] = atoi(value)
		break

	case "Pistol_1":
		g_KeyDef1[WEAP_PISTOL] = atoi(value)
		break

	case "Shotgun_1":
		g_KeyDef1[WEAP_SHOTGUN] = atoi(value)
		break
		
	case "Machinegun_1":
		g_KeyDef1[WEAP_MACHINEGUN] = atoi(value)
		break

	case "SniperRifle_1":
		g_KeyDef1[WEAP_SNIPER_RIFLE] = atoi(value)
		break

	case "RocketLauncher_1":
		g_KeyDef1[WEAP_ROCKET_LAUNCHER] = atoi(value)
		break

	case "Flamethrower_1":
		g_KeyDef1[WEAP_FLAMETHROWER] = atoi(value)
		break
		
	case "Grenade_1":
		g_KeyDef1[WEAP_GRENADE] = atoi(value)
		break

	case "QuickLoad_1":
		g_KeyDef1[QUICKLOAD] = atoi(value)
		break

	case "QuickSave_1":
		g_KeyDef1[QUICKSAVE] = atoi(value)
		break
//#ifndef XBOX
//xbox klavesy ktore su navyse
//#endif

	case "Move_Forward_2":
		g_KeyDef2[MOVE_F] = atoi(value)
		break

	case "Move_Backward_2":
		g_KeyDef2[MOVE_B] = atoi(value)
		break

	case "Move_Left_2":
		g_KeyDef2[LEFT] = atoi(value)
		break

	case "Move_Right_2":
		g_KeyDef2[RIGHT] = atoi(value)
		break
		
	case "Jump_2":
		g_KeyDef2[JUMP] = atoi(value)
		break

	case "Crouch_2":
		g_KeyDef2[CROUCH] = atoi(value)
		break

	case "Walk_2":
		g_KeyDef2[WALK_MODE] = atoi(value)
		break

	case "Prim_Attack_2":
		g_KeyDef2[P_ATTACK] = atoi(value)
		break

	case "SecAttack_2":
		g_KeyDef2[S_ATTACK] = atoi(value)
		break		
		
	case "Prev_Weapon_2":
		g_KeyDef2[PREV_WEAPON] = atoi(value)
		break

	case "Next_Weapon_2":
		g_KeyDef2[NEXT_WEAPON] = atoi(value)
		break		
		
	case "Reload_2":
		g_KeyDef2[RELOAD] = atoi(value)
		break

	case "Action_(Use)_2":
		g_KeyDef2[USE] = atoi(value)
		break

	case "Flashlight_2":
		g_KeyDef2[FLASHLIGHT] = atoi(value)
		break

	case "RemoteController_2":
		g_KeyDef2[CONTROLLER] = atoi(value)
		break

	case "BulletTime_2":
		g_KeyDef2[BULLET_TIME] = atoi(value)
		break

	case "TaskList_2":
		g_KeyDef2[TASKLIST] = atoi(value)
		break

	case "LeanLeft_2":
		g_KeyDef2[LEAN_LEFT] = atoi(value)
		break

	case "LeanRight_2":
		g_KeyDef2[LEAN_RIGHT] = atoi(value)
		break

	case "Jackhammer_2":
		g_KeyDef2[WEAP_JACKHAMMER] = atoi(value)
		break

	case "Pistol_2":
		g_KeyDef2[WEAP_PISTOL] = atoi(value)
		break

	case "Shotgun_2":
		g_KeyDef2[WEAP_SHOTGUN] = atoi(value)
		break
		
	case "Machinegun_2":
		g_KeyDef2[WEAP_MACHINEGUN] = atoi(value)
		break

	case "SniperRifle_2":
		g_KeyDef2[WEAP_SNIPER_RIFLE] = atoi(value)
		break

	case "RocketLauncher_2":
		g_KeyDef2[WEAP_ROCKET_LAUNCHER] = atoi(value)
		break

	case "Flamethrower_2":
		g_KeyDef2[WEAP_FLAMETHROWER] = atoi(value)
		break
		
	case "Grenade_2":
		g_KeyDef2[WEAP_GRENADE] = atoi(value)
		break
		
	case "QuickLoad_2":
		g_KeyDef2[QUICKLOAD] = atoi(value)
		break

	case "QuickSave_2":
		g_KeyDef2[QUICKSAVE] = atoi(value)
		break

//#ifndef XBOX
//xbox klavesy ktore su navyse
//#endif

	case "InvertMouse":
		g_InvMouse = atoi(value)
		break

	case "MouseSpeedH":
		g_MSpeedH = atof(value)
		break

	case "MouseSpeedV":
		g_MSpeedV = atof(value)
		break
		
	case "Subtitles":
		g_Subtitles = atof(value)
		break

	case "Joypad":
		g_Joypad = atoi(value)
		break		

	}
}

void SetDefaultKeys()
{
	g_KeyDef1[MOVE_F] = KC_W
	g_KeyDef2[MOVE_F] = 0
	g_KeyDef1[MOVE_B] = KC_S
	g_KeyDef2[MOVE_B] = 0
	g_KeyDef1[LEFT] = KC_A
	g_KeyDef2[LEFT] = 0
	g_KeyDef1[RIGHT] = KC_D
	g_KeyDef2[RIGHT] = 0	
	g_KeyDef1[JUMP] = KC_SPACE
	g_KeyDef2[JUMP] = 0
	g_KeyDef1[CROUCH] = KC_C
	g_KeyDef2[CROUCH] = 0
	g_KeyDef1[WALK_MODE] = KC_LSHIFT
	g_KeyDef2[WALK_MODE] = 0
	g_KeyDef1[P_ATTACK] = -1
	g_KeyDef2[P_ATTACK] = 0
	g_KeyDef1[S_ATTACK] = -3
	g_KeyDef2[S_ATTACK] = 0
	g_KeyDef1[NEXT_WEAPON] = KC_NEXT
	g_KeyDef2[NEXT_WEAPON] = -5
	g_KeyDef1[PREV_WEAPON] = KC_PRIOR
	g_KeyDef2[PREV_WEAPON] = -4
	g_KeyDef1[RELOAD] = KC_R
	g_KeyDef2[RELOAD] = 0
	g_KeyDef1[USE] = KC_F
	g_KeyDef2[USE] = 0
	g_KeyDef1[FLASHLIGHT] = KC_T
	g_KeyDef2[FLASHLIGHT] = 0
	g_KeyDef1[CONTROLLER] = KC_TAB
	g_KeyDef2[CONTROLLER] = 0
	g_KeyDef1[BULLET_TIME] = KC_LCONTROL
	g_KeyDef2[BULLET_TIME] = 0
	g_KeyDef1[TASKLIST] = KC_Z
	g_KeyDef2[TASKLIST] = 0
	g_KeyDef1[LEAN_LEFT] = KC_Q
	g_KeyDef2[LEAN_LEFT] = 0
	g_KeyDef1[LEAN_RIGHT] = KC_E
	g_KeyDef2[LEAN_RIGHT] = 0
	g_KeyDef1[WEAP_JACKHAMMER] = KC_1
	g_KeyDef2[WEAP_JACKHAMMER] = 0
	g_KeyDef1[WEAP_PISTOL] = KC_2
	g_KeyDef2[WEAP_PISTOL] = 0
	g_KeyDef1[WEAP_SHOTGUN] = KC_3
	g_KeyDef2[WEAP_SHOTGUN] = 0
	g_KeyDef1[WEAP_MACHINEGUN] = KC_4
	g_KeyDef2[WEAP_MACHINEGUN] = 0
	g_KeyDef1[WEAP_SNIPER_RIFLE] = KC_5
	g_KeyDef2[WEAP_SNIPER_RIFLE] = 0
	g_KeyDef1[WEAP_ROCKET_LAUNCHER] = KC_6
	g_KeyDef2[WEAP_ROCKET_LAUNCHER] = 0
	g_KeyDef1[WEAP_FLAMETHROWER] = KC_7
	g_KeyDef2[WEAP_FLAMETHROWER] = 0
	g_KeyDef1[WEAP_GRENADE] = KC_8
	g_KeyDef2[WEAP_GRENADE] = 0
	g_KeyDef1[QUICKLOAD] = KC_F9
	g_KeyDef2[QUICKLOAD] = 0
	g_KeyDef1[QUICKSAVE] = KC_F5
	g_KeyDef2[QUICKSAVE] = 0

//#ifndef XBOX
//xbox klavesy ktore su navyse
//#endif

	g_InvMouse = 1
	g_MSpeedH = 70
	g_MSpeedV = 40
	g_Subtitles = 1
	g_Joypad = 0
}

//=================================================================================
void SetDefaultSettings()
{
	SetDefaultKeys()

	GetOptions()
	SetMenuValue(0)
	SetMenuValue(1)
	SetMenuValue(2)
	SetWidTexts()
}

//=================================================================================
void LoadConfig()
{
	//nastavime defaulty uz tady, kdyby nekdo neco odmazl z cfg fajlu
	g_iFullscreen = 1
	g_iWidth = 1024
	g_iHeight = 768
	g_iFSAA = 0

	r_texcompress = 1
	r_shadow = 1
	r_texdetail = 0
	g_iGeomDetail = 2
	g_iBrightness = 50
	g_iEffectsVolume = 60
	g_iMusicVolume = 60
	r_shaderlevel = r_recommendedshaderlevel
	r_reflection = 1
	r_normal = 1
	
	if(r_recommendedshaderlevel == 0)
	{
		r_terrquality = 0
		r_terrspecular = 0
		r_terrparallax = 0
		r_texfilter = 1
		r_maxaniso = 0
		r_bloom = 0
		r_screenquality = 0
	}
	else
	{
		r_terrquality = 1
		r_terrspecular = 1
		r_terrparallax = 1
		r_texfilter = 2
		r_maxaniso = 2
		r_bloom = 1
		r_screenquality = 1
	}
	g_iDither = 0
	g_bEAX = 1
	g_iSoundQuality = 1
	g_iOutputRate = 1
	g_iSpeakers = -1
	g_bSoundOcclusion = true

	g_iVSync = true

	SetDefaultKeys()

	int f = OpenFile("settings.cfg", FILEMODE_READ)
	if(f == NULL)
	{		
		SaveConfig()
	}
	else
	{
		string str,item,value
	
		while(true)
		{
			int d
	
			if(FGets(f,str) < 0)
				break
	
			if(strstr(str, "//") >= 0)
				continue
	
			d = strstr(str," ")
			if(d <= 0)
				continue
			item = substr(str, 0, d)
			value = substr(str, d + 1, strlen(str) - d - 1)
	
			SetConfigValue(item,value)
		}
		CloseFile(f)
	}

	//validate settings
	if(r_maxshaderlevel == 0)
	{
		r_shaderlevel = 0
		r_bloom = 0
		r_terrquality = 0
		r_terrspecular = 0
		r_terrparallax = 0
	}
	
#ifdef _XBOX
	g_Joypad = true
#endif

	g_fMouseSpeedH = g_MSpeedH * 0.0005 + 0.005

	if(g_InvMouse)
		g_fMouseSpeedV = g_MSpeedV * -0.00035 - 0.00125
	else
		g_fMouseSpeedV = g_MSpeedV * 0.00035 + 0.00125
}

#ifdef DEVELOPER

//---------------------------------------------------------------------------------
void SaveDefaultDeveloperConfig()
{
	int f = OpenFile("DevSettings.cfg",FILEMODE_WRITE)

	if(f)
	{
		FPrintln(f, "//DEVELOPER CONFIG FILE. CAN MODIFY")
		FPrintln(f, "")

		FPrintln(f, "g_FlySpeed 400")
		FPrintln(f, "getweapons")
		FPrintln(f, "getammo")
		FPrintln(f, "getimmortality")
		FPrintln(f, "gethubbardium")
		
		CloseFile(f)
	}
}

//---------------------------------------------------------------------------------
void LoadDeveloperConfig()
{
begin:
	int f = OpenFile("DevSettings.cfg",FILEMODE_READ)
	if(f == NULL)
	{
		SaveDefaultDeveloperConfig()
		goto begin
	}

	string str,item,value

	while(true)
	{
		int d

		if(FGets(f,str) < 0)
			break

		if(strstr(str, "//") >= 0)
			continue

		d = strstr(str," ")
		if(d <= 1)
		{
			if(str)
			{
				item = str
				value = ""
			}
			else
				continue
		}
		else
		{
			item = substr(str, 0, d)
			value = substr(str, d + 1, strlen(str) - d - 1)
		}

		int IntegerVar
		float FloatVar
		vector VectorVar
		string StringVar
		
		if(!GetValue(NULL, item, 0, IntegerVar))
		{
			if(!GetValue(NULL, item, 0, FloatVar))
			{
				if(!GetValue(NULL, item, 0, VectorVar))
				{
					if(!GetValue(NULL, item, 0, StringVar))
					{
						if(!Call(NULL, item, NULL))
						{
							string ErrMsg = "Cannot realize developer command " + item
							DPrint(ErrMsg)
						}
					}
					else
						SetValue(NULL, item, 0, value)
				}
				else
					SetValue(NULL, item, 0, atov(value))
			}
			else
				SetValue(NULL, item, 0, atof(value))
		}
		else
			SetValue(NULL, item, 0, atoi(value))
	}
	CloseFile(f)
}
#endif


//=================================================================================
widget WSplash

void SetSplash()
{
	g_iMInGame = 666
	WSplash = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_VISIBLE|WF_BLEND | WF_SOURCEALPHA, 0xffffffff, 666)
	LoadWidgetImage(WSplash, 0, "gfx/last_screen.jpg")
	AnyKey()	//musime clearovat
}

//=================================================================================
void Cleanup()
{
	if(g_PlayerStorage)
	{
		delete g_PlayerStorage
		g_PlayerStorage = NULL
	}

	CleanupWidgets()
	CleanupSoundSets()
}

bool LoadPlayerStorageBack(string MapPath)
{
	int n = 0
	int k
	string tokens[30]
	string uv = tostr(34)
	string token
	int WeapClnum

	if(strlen(MapPath) < 4)
		return false

	tparse tp = BeginParse(MapPath)
 
	if(!tp)
	{
		string msg = "No found file " + MapPath
		Print(msg)
		return false
	}

	if(!g_PlayerStorage)
		g_PlayerStorage = new PlayerStorage
	
	PlayerStorage st = g_PlayerStorage		

	bool result = false
	int checksum = -1

	int Tnum = 1
	while(Tnum)
	{
		Tnum = ParseLine(tp, n++, tokens)

		if(Tnum == 0)
			continue

		token = tokens[2]

		switch(tokens[0])
		{
		case "DifficultyLevel":
			st.DifficultyLevel = atoi(token)
			checksum = AddToCheckSum(checksum, st.DifficultyLevel, 1)
			break

		case "CurrentWeapon":
			st.CurrentWeapon = atoi(token)
			checksum = AddToCheckSum(checksum, st.CurrentWeapon, 2)
			break

		case "BulletTime":
			st.BulletTime = atof(token)
			checksum = AddToCheckSum(checksum, st.BulletTime, 3)
			break

		case "Health":
			st.health = atof(token)
			checksum = AddToCheckSum(checksum, st.health, 4)
			break

		case "Oxygen":
			st.oxygen = atof(token)
			checksum = AddToCheckSum(checksum, st.oxygen, 5)
			break

		case "Flashlight":
			st.flashlight = atof(token)
			checksum = AddToCheckSum(checksum, st.flashlight, 6)
			break

		case "Weapons":
			st.NumWeapons = Tnum - 2
			for(k = 0; k < st.NumWeapons; k++)
			{
				st.weapons[k] = atoi(tokens[k + 2])
				checksum = AddToCheckSum(checksum, st.weapons[k], 7)
			}
			break

		case "Ammo":
			int NumAmmos = Tnum - 2
			for(k = 0; k < NumAmmos; k++)
			{
				st.ammo[k] = atoi(tokens[k + 2])
				checksum = AddToCheckSum(checksum, st.ammo[k], 8)
			}
			break

		case "AmmoInMagazines":
			int NumMagazines = Tnum - 2
			for(k = 0; k < NumMagazines; k++)
			{
				st.MagazinesAmmo[k] = atoi(tokens[k + 2])
				checksum = AddToCheckSum(checksum, st.MagazinesAmmo[k], 9)
			}
			break

		case "Checksum":
			int chk = atoi(tokens[2])
			if(chk == checksum)
			{
				result = true
				Print("Bad checksum")
			}
			break
		}
	}
	EndParse(tp)
	return result
}

void LoadSavedPosition(int slot)
{
	if(slot == -1)	//toto sa nesmie nikdy stat
		return

	g_RunMode = RUNMODE_NORMAL
			
	if(SaveTypes[slot] > SAVETYPE_EMPTY)
	{
		string MapFileName = SaveFileNames[slot]	//najprv musime vytiahnut co potrebujeme pretoze PopMenu(false) nam zavrie load menu a tym padom prehadze sloty (doplnuje empty slot)
		string MapName = SaveMapsNames[slot]
		int SaveType = SaveTypes[slot]

		ShowSavesListbox(false)
		
		if(MenuStackOff > 0)
		{
			PopMenu(false)
			FreeMenu()
		}

		UnloadMenuMap()
		g_FromSavePos = true

		if(SaveType == SAVETYPE_AUTO)	//iba nahrajeme mapu a nahadzeme playerovi veci zo soft sejvu
		{
			ShowWidget(WView, false)

			if(LoadPlayerStorageBack("save/" + MapFileName + ".sav") == true)
				globalmap = LoadMapEx(MapName)
			else
				globalmap = LoadMapEx("l01")	//bol podvod na sejvoch tak nech si to zahraje pekne znova
		}
		else
			globalmap = LoadSaveMapEx(MapName, "save/" + MapFileName + ".sav")
	}
}

bool LoadGameDeleteDisabled

void ProcessLoadSavedMap()
{
	if(g_LB_Update)
	{
		int LB_ClickedLine = LB_Update()
		g_SaveSelected = LB_GetSelectedLine()

		if(g_SaveSelected >= 0)
		{
			if(SaveTypes[g_SaveSelected] <= SAVETYPE_AUTO)
			{
				if(!LoadGameDeleteDisabled)
				{
					GUIDisableItem("loadgame_delete")
					LoadGameDeleteDisabled = true
				}
			}
			else
			{
				if(LoadGameDeleteDisabled)
				{
					GUIEnableItem("loadgame_delete")
					LoadGameDeleteDisabled = false
				}
			}
		}

		if(LB_ClickedLine != -1)	//bolo nieco vybrane
		{
			if(SaveTypes[g_SaveSelected] > SAVETYPE_EMPTY)
			{
				if(g_IMBackground)
				{
					DestroyWidget(g_IMBackground)
					g_IMBackground = NULL
				}
			
				UnloadMap()
				LoadSavedPosition(LB_ClickedLine)
			}
		}
	}
}

void SavePlayerStorage(string EntFile)
{
	creature_player pl = World.Player
	if(pl == NULL)
		return
	
	int f = OpenFile(EntFile, FILEMODE_WRITE)

	if(!f)
		return

	string line
	string uv = tostr(34)	//uvodzovky
	int n
	int checksum = -1
		
	pl.WriteCurrentStorageTo(g_PlayerStorage)
	PlayerStorage st = g_PlayerStorage
		
	line = "DifficultyLevel = " + uv + itoa(st.DifficultyLevel) + uv
	checksum = AddToCheckSum(checksum, st.DifficultyLevel, 1)
	FPrintln(f, line)

	line = "CurrentWeapon = " + uv + itoa(st.CurrentWeapon) + uv
	checksum = AddToCheckSum(checksum, st.CurrentWeapon, 2)
	FPrintln(f, line)

	line = "BulletTime = " + uv + ftoa(st.BulletTime) + uv
	checksum = AddToCheckSum(checksum, st.BulletTime, 3)
	FPrintln(f, line)	

	line = "Health = " + uv + ftoa(st.health) + uv
	checksum = AddToCheckSum(checksum, st.health, 4)
	FPrintln(f, line)

	line = "Oxygen = " + uv + ftoa(st.oxygen) + uv
	checksum = AddToCheckSum(checksum, st.oxygen, 5)
	FPrintln(f, line)

	line = "Flashlight = " + uv + ftoa(st.flashlight) + uv
	checksum = AddToCheckSum(checksum, st.flashlight, 6)
	FPrintln(f, line)

	line = "Weapons ="
	for(n = 0; n < st.NumWeapons; n++)
	{
		checksum = AddToCheckSum(checksum, st.weapons[n], 7)
		line += " " + itoa(st.weapons[n])
	}
	FPrintln(f, line)

	line = "Ammo ="
	for(n = 0; n < st.NumWeapons; n++)
	{
		checksum = AddToCheckSum(checksum, st.ammo[n], 8)
		line += " " + itoa(st.ammo[n])
	}
	FPrintln(f, line)
	
	line = "AmmoInMagazines ="
	for(n = 0; n < st.NumWeapons; n++)
	{
		checksum = AddToCheckSum(checksum, st.MagazinesAmmo[n], 9)
		line += " " + itoa(st.MagazinesAmmo[n])
	}
	FPrintln(f, line)

	line = "Checksum = " + itoa(checksum)
	FPrintln(f, line)
	CloseFile(f)
}


void WriteCurrentSavelist()
{
	string SaveList = "save/save.lst"
	int f = OpenFile(SaveList, FILEMODE_WRITE)
	string ln
	string SaveHeader
	string uv = tostr(34)

	for(int n = 0; n < sizeof(SaveFileNames); n++)
	{
		if(SaveTypes[n] > SAVETYPE_EMPTY)
		{
			if(SaveTypes[n] == SAVETYPE_AUTO)	//save na zaciatku levelu bude iba medzilevelovy save
				SaveHeader = "SOFTSAVE"
			
			if(SaveTypes[n] == SAVETYPE_NORMAL)
				SaveHeader = "SAVE"

			if(SaveTypes[n] == SAVETYPE_QUICK)
				SaveHeader = "QUICKSAVE"

			ln = SaveHeader + " " + SaveFileNames[n] + " " + SaveMapsNames[n] + " " + uv +  SaveMapsTimes[n] + uv
			FPrintln(f, ln)
		}
	}
	CloseFile(f)
}

int FreeSlotToSave(int index)
{
	int LastIdex = sizeof(SaveFileNames) - 1
	for(int n = LastIdex; n > index; n--)
	{
		int before = n - 1
		SaveFileNames[n] = SaveFileNames[before]
		SaveMapsNames[n] = SaveMapsNames[before]
		SaveMapsTimes[n] = SaveMapsTimes[before]
		SaveTypes[n] = SaveTypes[before]
	}
}

const int SAVE_DISABLED = 0
const int SAVE_ENABLED = 1
const int SAVE_NEED_REQUEST = 2

int CanSaveToSelectedSlot()
{
	int SelectedType = SaveTypes[g_SaveSelected]
	
	if(SelectedType == SAVETYPE_NONE)
		return SAVE_DISABLED

	if(SelectedType == SAVETYPE_QUICK)	//nemozeme presejvovat quicksave normalnym sejvom
		return SAVE_DISABLED

	if(SelectedType == SAVETYPE_EMPTY)	//free slot. mozeme ukladat bez opytania
		return SAVE_ENABLED

	if(SelectedType == SAVETYPE_NORMAL || SelectedType == SAVETYPE_AUTO)	//v slote je nieco ulozene tak sa musime spytat ci to mozeme prepisat
	{
		//vyhodit request a ked ano tak true
		return SAVE_NEED_REQUEST
	}
}

void SaveActPos(int SaveType)
{
	int SlotToSave = -1
	int n
	
	if(SaveType == SAVETYPE_AUTO)
	{
		for(n = 0; n < sizeof(SaveTypes); n++)	//za normalna sejvujeme na empty slot
		{
			if(SaveTypes[n] == SAVETYPE_EMPTY)	//musi tam byt
			{
				SlotToSave = n + 1
				FreeSlotToSave(SlotToSave)	//empty slot nechame a za neho sejvneme. vsetko shiftneme dolu
				break
			}
		}
	}

	if(SaveType == SAVETYPE_NORMAL)	//jedine z menu
	{
		int SelectedType = SaveTypes[g_SaveSelected]
		if(SelectedType == SAVETYPE_QUICK)	//nemozeme presejvovat quicksave normalnym sejvom
			return

		if(SelectedType == SAVETYPE_EMPTY)	//free slot. mozeme ukladat bez opytania
		{
			SlotToSave = g_SaveSelected + 1
			FreeSlotToSave(SlotToSave)	//musime posunut to co v slote je a vsetko za tym dolu
		}

		if(SelectedType == SAVETYPE_NORMAL || SelectedType == SAVETYPE_AUTO)	//v slote je nieco ulozene tak mazeme stary sejv. prepisanie uz povolene bolo
		{
			SlotToSave = g_SaveSelected
			DeleteFileA(GetFilePath("save/" + SaveFileNames[SlotToSave] + ".sav"))
		}
	}
	
	if(SaveType == SAVETYPE_QUICK)
	{
		SlotToSave = 0		//quicksejvy sejvujeme vzdy na prvy slot
		int NumQuickSaves = 0
		for(n = 0; n < sizeof(SaveTypes); n++)	//zistime kolko quicksejvov mame. mozu byt maximalne 2
		{
			if(SaveTypes[n] == SAVETYPE_QUICK)
				NumQuickSaves++
		}
		
		if(NumQuickSaves < 2)	//este nemame ulozene 2 quicksejvy coz je maximalny pocet takze vsetko shiftneme dolu. posledny sejv vypada zo zoznamu
			FreeSlotToSave(0)
		else	//su tam dva takze ten prvy presunieme na druhe miesto. ten druhy je uz nepotrebny
		{
			DeleteFileA(GetFilePath("save/" + SaveFileNames[1] + ".sav"))	//stary quicksejv uz je nepotrebny takze ho zmazeme
			SaveFileNames[1] = SaveFileNames[0]
			SaveMapsNames[1] = SaveMapsNames[0]
			SaveMapsTimes[1] = SaveMapsTimes[0]
			SaveTypes[1] = SaveTypes[0]
		}
		
		if(NumQuickSaves > 0)	//ak tam bol aspon jeden quicksave tak ten prvy bol presunuty na druhy slot. potrebujeme ho premenovat
		{
			string OldFname = SaveFileNames[1] + ".sav"
			string OldFilename = GetFilePath("save/" + OldFname)	//ten ktory bol doteraz novy premenujeme na stary
			int spos = strlen(OldFilename)
			spos = spos - strlen(OldFname)
			string NewFilename = substr(OldFilename, 0, spos) + "quicksave_previous.sav"
			bool res = MoveFileA(OldFilename, NewFilename)
			SaveFileNames[1] = "quicksave_previous"
		}
	}

	if(SlotToSave == -1)
		return

	if(SaveType == SAVETYPE_QUICK)
		SaveFileNames[SlotToSave] = "quicksave_last"
	else
		SaveFileNames[SlotToSave] = g_sThisLevel + itoal(++SaveIdent, 4)

	SaveMapsNames[SlotToSave] = World.name
	SaveTypes[SlotToSave] = SaveType

	string DateTime = GetDateTime()
	string SDate = substr(DateTime, 0, 10)
	string STime = substr(DateTime, 11, 8)
	DateTime = SDate + "     " + STime
	SaveMapsTimes[SlotToSave] = DateTime

	WriteCurrentSavelist()
	string SaveFile = "save/" + SaveFileNames[SlotToSave] + ".sav"
/*
	string SaveDir = GetFilePath("enforce2.dll")
	if(strstr(SaveDir, "enforce2.dll") != -1)
		SaveDir = substr(SaveDir, 0, strstr(SaveDir, "enforce2.dll"))

	int a = GetFileAttributesA(SaveDir)
	Print(SaveDir)

	if(a == -1 || a & 0x00000010)
	{
		int res2 = CreateDirectoryA(SaveDir, NULL)
		Print(res2)
		Print("create dir")
	}*/

	if(SaveType == SAVETYPE_AUTO)		//falosny sejv na zaciatku levelu
		SavePlayerStorage(SaveFile)
	else
		SaveMap(globalmap, SaveFile)	//normalny sejv alebo quicksejv
		
	g_SaveSelected = SlotToSave		//kurzor tam kam sme ulozili
}

void DeleteSavedPosition()
{
	if(SaveTypes[g_SaveSelected] > SAVETYPE_EMPTY)
	{
#ifndef _XBOX
		bool res = DeleteFileA(GetFilePath("save/" + SaveFileNames[g_SaveSelected] + ".sav"))
#endif

		int LastIndex = sizeof(SaveTypes) - 1
		for(int n = g_SaveSelected; n < LastIndex; n++)
		{
			int next = n + 1
			SaveFileNames[n] = SaveFileNames[next]
			SaveMapsNames[n] = SaveMapsNames[next]
			SaveMapsTimes[n] = SaveMapsTimes[next]
			SaveTypes[n] = SaveTypes[next]
		}
		WriteCurrentSavelist()
		ShowSavesListbox(false)
		ShowSavesListbox(true)
	}
}

void SaveToSelectedSlot()
{
	widget SavingInfo = CreateWidget(WTYPE_TEXT, 350, 500, 100, 40, WF_CENTER|WF_VCENTER|WF_VISIBLE, 0xffffffff, 300)
	SetFont(SavingInfo, MainFont)
	SetWidgetTextEx(SavingInfo, 0, false, "#quicksave")
	ShowWidget(WMousePointer, false)
	Repaint()
	Repaint()
	SaveActPos(SAVETYPE_NORMAL)				//ulozi hru + novy savelist
	ClearMousePress(0)		//inac by sa sejvovalo kazdy frame
	ClearKey(KC_RETURN)
	ShowSavesListbox(false)	//skryjeme stary listbox. to co je v nom uz neni aktualne
	ShowSavesListbox(true)	//zobrazime nanovo. uz bude akceptovat novy savelist
	DestroyWidget(SavingInfo)
	ShowWidget(WMousePointer, true)
}

bool RequestForSave()
{
	int SlotAccesibility = CanSaveToSelectedSlot()

	if(SlotAccesibility == SAVE_DISABLED)	//do selektnuteho slotu sa neda sejvovat
		return false

	if(SlotAccesibility == SAVE_ENABLED)	//mozeme sejvovat
	{
		SaveToSelectedSlot()
		return true
	}

	if(SlotAccesibility == SAVE_NEED_REQUEST)	//spytat sa ci sa moze presejvit existujuci
	{
		if(!ConfDialog)
			ConfDialog = new ConfirmDialog("#save_req_del", "savegame_overwrite_yes", "savegame_cancel")		//bude odstavene ovladanie menu.

		LB_EnableEvents(false)
		return false
	}
	return false
}

void RequestForDelete(string ResultOnYes, string ResultOnNo)
{
	if(SaveTypes[g_SaveSelected] <= SAVETYPE_EMPTY)
		return
	
	if(!ConfDialog)
		ConfDialog = new ConfirmDialog("#save_req_ow", ResultOnYes, ResultOnNo)		//bude odstavene ovladanie menu.

	LB_EnableEvents(false)
}

void ProcessSaveMap()
{
	if(g_LB_Update)
	{
		int LB_ClickedLine = LB_Update()
		g_SaveSelected = LB_GetSelectedLine()

		if(g_SaveSelected >= 0)
		{
			if(SaveTypes[g_SaveSelected] <= SAVETYPE_AUTO)
				GUIDisableItem("savegame_delete")
			else
				GUIEnableItem("savegame_delete")
		}

		if(LB_ClickedLine != -1)	//bolo nieco vybrane
		{
			if(RequestForSave() == true)
				PopMenu(false)	//hra bola ulozena takze skocime o menu vysie
		}
	}
}

void LoadCustomLevel(string lev)
{
	FreeMenu()
	g_FromSavePos = false
	UnloadMenuMap()
	globalmap = LoadMapEx(lev)	
}

bool LoadVideo(widget w, string name, bool sound)
{
	if(!LoadWidgetImage(w, 0, name + ".avi"))
		return false

	if(sound)
	{
		g_VideoSnd = GetSoundEx(name, 1, SF_STREAM)
		g_VideoSound = PlaySound(NULL, g_VideoSnd, SF_AMBIENT|SF_MUSIC|SF_ONCE)
	}
	return true
}

void StopVideoSound()
{
	if(g_VideoSound)
		EndSound(g_VideoSound)
		
	g_VideoSound = 0
}

//=================================================================================
int GameUpdate()
{
	if(g_EndStatus && g_EscKey == false)	//klavesu potrebujeme zdetekovat a vyclearovat uplne na zaciatku
	{
#ifdef _XBOX
		if(JoyButtonState(JB_START))
		{
			ClearJoyButton(JB_START)
			g_EscKey = true
		}
		
#else
		if(KeyState(KC_ESCAPE))
		{
			ClearKey(KC_ESCAPE)
			g_EscKey = true
		}
#endif
	}


#ifdef DEVELOPER

	if(KeyState(KC_NUMPADENTER))
	{
		ClearKey(KC_NUMPADENTER)

		g_bDebugFSM = 1 - g_bDebugFSM
	}



#endif

	if(NeedAutosave)
	{
		SaveActPos(SAVETYPE_AUTO)
		NeedAutosave = false
	}

	//jestli je nahrana mapa, tak updatuj
	if(globalmap != NULL && g_EndStatus == 0)
		PlayMap(globalmap)

#ifndef EDITOR
	if(InCutscene == 0)
	{
		widget www
	
		if(g_EndStatus == 0)
		{
			if(KEY(QUICKSAVE, true))
			{
				www = CreateWidget(WTYPE_TEXT, 350, 280, 100, 40, WF_CENTER|WF_VCENTER|WF_VISIBLE, 0xffffffff, 300)
				SetFont(www, MainFont)
				SetWidgetTextEx(www, 0, false, "#quicksave")
				LoadWidgetImage(www, 0, "gfx/black.tga")
				Repaint()
				Repaint()
				SaveActPos(SAVETYPE_QUICK)
		
				DestroyWidget(www)
			}
		}
	}

	if(KEY(QUICKLOAD, true))
	{
		if(SaveTypes[0] == SAVETYPE_QUICK)	//ak tam vobec nejaky je
		{
			if(g_wGameOver1)
			{
				DestroyWidget(g_wGameOver1)
				g_wGameOver1 = NULL
			}
			
			if(g_wGameOver2)
			{
				DestroyWidget(g_wGameOver2)
				g_wGameOver2 = NULL
			}

			www = CreateWidget(WTYPE_TEXT, 0, 0, 800, 600, WF_CENTER|WF_VCENTER|WF_VISIBLE, 0xffffffff, 300)
			SetFont(www, MainFont)
			SetWidgetTextEx(www, 0, false, "#quickload")
			LoadWidgetImage(www, 0, "gfx/black.tga")

			Repaint()
			Repaint()
	
			g_sThisLevel = SaveMapsNames[0]
			g_bEndLevel = false
			g_EndStatus = 0
			g_iMInGame = 1
			globalmap = LoadSaveMap(globalmap, "save/" + SaveFileNames[0] + ".sav")	//posledny quicksave
			DestroyWidget(www)
			ShowWidget(g_wBlack, false)
		}
	}
#endif

//cheat next level
	if(CheatNextLevel)
	{
		CheatNextLevel = false

		for(int n = 0; n < g_iNumLevels; n++)
		{
			if(g_sLevelNames[n] == g_sThisLevel)
			{
				n++
				if(n < g_iNumLevels)
				{
					change_level(g_sLevelNames[n], false)
					break
				}
			}
		}
	}

	if(!g_bEndLevel)
	{
#ifdef _XBOX
		
		if(JoyButtonState(JB_START))
		{
			return true //nedovolime ingame menu
			ClearJoyButton(JB_START)
#else
		if(KeyState(KC_ESCAPE))
		{
			ClearKey(KC_ESCAPE)
#endif	
			g_iMInGame = 2
		}
	}

//OnChangeLevel
	if(g_bEndLevel)
	{
		switch(g_EndStatus)
		{
		case 20:
			if(FadeWidget(g_wBlack, 500, FADE_BACK))
			{
				if(!g_wGameOver1)
				{
					g_wGameOver1 = CreateWidget(WTYPE_TEXT, 146, 210, 512, 128, WF_SOURCEALPHA|WF_BLEND|WF_CENTER|WF_VISIBLE, 0xffffffff, 126)
					SetFont(g_wGameOver1, LargeFont)
					SetWidgetTextEx(g_wGameOver1, 0, true, "#game_over_label")
//					LoadWidgetImage(g_wGameOver1, 0, "gfx/gameover.tga")
				}
				
				if(!g_wGameOver2)
				{
					g_wGameOver2 = CreateWidget(WTYPE_TEXT, 146, 270, 512, 128, WF_SOURCEALPHA|WF_BLEND|WF_CENTER|WF_VISIBLE, 0xffffffff, 127)
					SetFont(g_wGameOver2, MainFont)
					SetWidgetTextEx(g_wGameOver2, 0, true, "#game_over_esc")
					SetWidgetTextEx(g_wGameOver2, 1, true, "#game_over_lmb")
					SetWidgetTextEx(g_wGameOver2, 2, true, "#game_over_quickload1 " + KeyNames[g_KeyDef1[QUICKLOAD]] + " #game_over_quickload2")
//					LoadWidgetImage(g_wGameOver1, 0, "gfx/gameover.tga")
				}
				
				g_EndStatus = 21
			}
		break

		case 21:
			if(g_EscKey)
			{
				g_EscKey = false
				g_EndStatus = 22
			}
			else
			{
				if(MousePress(0))
				{
					ClearMousePress(0)
					g_sNextLevel = "restart"
					g_EndStatus = 22
				}
			}
		break

		case 22:
			if(FadeWidget(g_wGameOver1, -500, FADE_TEXT))
			{
				if(g_wGameOver1)
				{
					DestroyWidget(g_wGameOver1)
					g_wGameOver1 = NULL
				}

				if(g_wGameOver2)
				{
					DestroyWidget(g_wGameOver2)
					g_wGameOver2 = NULL
				}

				if(g_sNextLevel == "restart")	//restart alebo hlavne menu
				{
					g_sNextLevel = g_sThisLevel
					g_EndStatus = 0
					UnloadMap()
					LoadSavedPosition(GetLastAutoSavedSlotOfLevel(g_sThisLevel))	//nahravame najnovsi autosave tohto levelu
				}
				else
					g_sNextLevel = "finale"

				g_EndStatus = 10
			}
			else
			{
				if(g_wGameOver2)
					SetWidgetTextColor(g_wGameOver2, ARGB(g_FadeCounter, g_FadeCounter, g_FadeCounter, g_FadeCounter))
			}
		break
			
		case 11:
			if(FadeWidget(g_wBlack, 500, FADE_BACK))
			{
//				g_DemoEndVideo = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA|WF_BLEND|WF_CENTER, 0x00ffffff, 126)
//				LoadWidgetImage(g_DemoEndVideo, 0, "gfx/endscreen_request.tga")
/*				g_DemoEndVideo = CreateWidget(WTYPE_TEXT, 0, 295, 800, 60, WF_SOURCEALPHA|WF_BLEND|WF_CENTER, 0x00ffffff, 126)
				SetFont(g_DemoEndVideo, LargeFont)
				SetWidgetText(g_DemoEndVideo, 0, "End of Demo")
				SetWidgetText(g_DemoEndVideo, 1, "Press any key to main menu...")*/
				g_EndStatus = 12
			}
		break

		case 12:
			if(Delay(100))
			{
				g_sNextLevel = "finale"
				g_EndStatus = 10
			}
		break

		case 13:
			if(FadeWidget(g_DemoEndVideo, 500, FADE_BACK))
			{
				g_EndStatus = 14
			}
		break
		
		case 14:
			if(AnyKey())
				g_EndStatus = 15
		break

		case 15:
			if(FadeWidget(g_DemoEndVideo, -500, FADE_BACK))
			{
				g_sNextLevel = "finale"
				g_EndStatus = 10
			}
		break

		case 1:
			if(g_sNextLevel == "demoend")
				g_EndStatus = 11
			else
			{
				switch(g_sNextLevel)
				{
				case "Escape=Menu LMB=Restart":
					g_EndStatus = 20
				break

				case "finale":
//						ShowWidget(g_wBlack, false)
					EndGameMessageLeft = g_iWidth / 2
					EndGameMessageTop = g_iHeight / 2
					EndGameMessageHeight = 512
					EndGameMessageWidth = 512
					EndGameMessageLeft -= EndGameMessageWidth / 2
					EndGameMessageTop -= EndGameMessageHeight / 2
					
					EndGameMessage = CreateWidget(WTYPE_TEXT, EndGameMessageLeft, EndGameMessageTop, EndGameMessageWidth, EndGameMessageHeight, /*WF_VISIBLE|*/WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTSIZE|WF_EXACTPOS|WF_CENTER|WF_VCENTER, 0x00ffffff, 150)
					SetWidgetTextEx(EndGameMessage, 0, true, "#the_end")
					SetWidgetTextColor(EndGameMessage, ARGB(255, 255, 255, 255))
					SetWidgetColor(EndGameMessage, ARGB(0, 0, 0, 0))
					SetFont(EndGameMessage, LargeFont)
					g_EndStatus = 3					
				break
				
				case "finale_skip":
					if(globalmap)	//mapa sa uz neupdatuje tak musim zrusit zvuk ktory prave hraje
					{
						SetCurrentMap(globalmap)
	
						if(World.MusicPlayer)
							World.MusicPlayer.InterruptPlaying()
					}

					SetWidgetColor(g_wBlack, 0xffffffff)
					ShowWidget(g_wBlack, true)
					g_sNextLevel = "finale"
					g_EndStatus = 1
				break

				default:
					g_EndStatus = 9
				}
			}
		break
/*
		case 2:
			if(FadeWidget(g_wBlack, 200, FADE_BACK))
				g_EndStatus = 3
//			EndGameScreen = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_VISIBLE, 0xffffffff, 10)
//			LoadWidgetImage(EndGameScreen, 0, "gfx/black.tga")
		break*/

		case 3:
			if(FadeWidget(EndGameMessage, 100, FADE_TEXT))
				g_EndStatus = 4
		break

		case 4:
			if(Delay(300))
			{
				g_EndStatus = 5
				InitCredits()
			}
		break

		case 5:
			UpdateCredits()
			
			if(EndGameMessageTop > -EndGameMessageHeight)
			{
				EndGameMessageTop -= ftime * CreditsSpeed
				SetWidgetPos(EndGameMessage, EndGameMessageLeft, EndGameMessageTop, EndGameMessageWidth, EndGameMessageHeight)
			}
			else
				ShowWidget(EndGameMessage, false)
			
			if(g_EscKey)
			{
				g_EscKey = false
				g_EndStatus = 10
				ShowWidget(g_wBlack, true)
			}
		break
/*
		case 6:
			if(FadeWidget(g_wBlack, 1000, FADE_BACK))
				g_EndStatus = 10
		break
*/
		case 9:
			if(FadeWidget(g_wBlack, 300, FADE_BACK))
				g_EndStatus = 10
		break

		case 10:
			g_EndStatus = 0
		//prechod do dalsiho levelu
			SetWidgetWorld(WView, NULL, 0)
				
			if(g_sNextLevel == "demoend")
			{
				g_EndStatus = 11
				return true
			}

			if(g_sNextLevel == "finale" || g_sNextLevel == "gameover" || g_RunMode == RUNMODE_TUTORIAL)
			{
				UnloadMap()
			
				if(g_DemoEndVideo)
					DestroyWidget(g_DemoEndVideo)
					
				g_DemoEndVideo = NULL
				StopVideoSound()

/*				if(EndGameScreen)
				{
					DestroyWidget(EndGameScreen)
					EndGameScreen = NULL
				}*/
			
				if(EndGameMessage)
				{
					DestroyWidget(EndGameMessage)
					EndGameMessage = NULL
				}
					
				DestroyCredits()
							
				ShowWidget(g_wBlack, false)
				InitMenu(true)
				PushMenu("main_menu")
				g_iMInGame = 4
				return true
			}
			else
			{
				UnloadMap()
			
				g_FromSavePos = false
				globalmap = LoadMapEx(g_sNextLevel)

				if(globalmap == NULL)
				{
					Error("Can't load next level " + g_sNextLevel)
					ShowWidget(g_wBlack, false)
					InitMenu(true)
					PushMenu("main_menu")
					g_iMInGame = 4
					return true
				}
			}
		break
		}
	}

	if(g_bStartLevel)
	{
		g_EndStatus = 0

		if(FadeWidget(g_wBlack, -150, FADE_BACK))
		{
			g_bStartLevel = 0
			if(g_wBlack)
				ShowWidget(g_wBlack, false)
		}
	}


	return true
}

//=================================================================================
int StWidStat

void ProcessStartScreens()
{
	if(AnyKey())
	{
		LoadBlackWidget() 
		ShowWidget(g_wBlack, true)
#ifdef DEMO
		if(StWidStat == 14)
			StWidStat = 15
		else
			StWidStat = 12
#else
		StWidStat = 15
#endif
		StopVideoSound()
	}
	
	switch(StWidStat)
	{
	case 0:
		ShowWidget(g_wBlack, false)
		
		if(!g_StartVideo)
			g_StartVideo = CreateWidget(WTYPE_VIDEO, 0, 0, 800, 600, WF_VISIBLE|WF_BLEND|WF_STRETCH, 0xffffffff, 124)

		ShowWidget(g_StartVideo, true)
		LoadVideo(g_StartVideo, "video/logo_bes", true)
		PlayVideo(g_StartVideo, VP_PLAY)
		StWidStat = 1
		StWidStat = 2
	break

	case 1:
		if(PlayVideo(g_StartVideo, VP_ISPLAYING) == false)
		{
			ShowWidget(g_wBlack, true)
			StopVideoSound()
			StWidStat = 2

			if(!g_StartScreen)
			{
				g_StartScreen = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, 0, 0, 124)
				LoadWidgetImage(g_StartScreen, 0, "gfx/bluemedium.tga")
				ShowWidget(g_StartScreen, true)
				SetWidgetColor(g_StartScreen, 0xffffffff)
			}
		}
	break

	case 2:
		if(FadeWidget(g_wBlack, -300, FADE_BACK))
		{
			StWidStat = 3
		}
	break
	
	case 3:
		if(Delay(500))
		{
			StWidStat = 4
		}
	break

	case 4:
		if(FadeWidget(g_wBlack, 300, FADE_BACK))
		{
//			StWidStat = 500
			StWidStat = 5
		}
	break
	/*
	case 500:
		if(!g_StartScreen)
			g_StartScreen = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, 0, 0, 124)
		LoadWidgetImage(g_StartScreen, 0, "gfx/apdemo.tga")
		ShowWidget(g_StartScreen, true)
		SetWidgetColor(g_StartScreen, 0xffffffff)
		StWidStat = 501
		break
		
	case 501:
		if(FadeWidget(g_wBlack, -300, FADE_BACK))
		{
			StWidStat = 502
		}
	break
	
	case 502:
		if(Delay(500))
		{
			StWidStat = 503
		}
	break

	case 503:
		if(FadeWidget(g_wBlack, 300, FADE_BACK))
		{
			StWidStat = 5
		}
	break
*/
	case 5:
		if(g_StartScreen)
			ShowWidget(g_StartScreen, false)
		LoadVideo(g_StartVideo, "video/logo_idea", true)
		PlayVideo(g_StartVideo, VP_PLAY)
		ShowWidget(g_wBlack, false)
		StWidStat = 6
	break

	case 6:
		if(PlayVideo(g_StartVideo, VP_ISPLAYING) == false)
		{
			StWidStat = 13
			ShowWidget(g_wBlack, true)
			ShowWidget(g_StartVideo, false)
			StopVideoSound()
		}
	break
/*

	case 4:
		if(Delay(1))
		{
			ShowWidget(g_wBlack, false)
			LoadVideo(g_StartVideo, "video/local_publisher", true)
			PlayVideo(g_StartVideo, VP_PLAY)
			ShowWidget(g_StartVideo, true)
			StWidStat = 5
		}
	break

	case 5:
		if(PlayVideo(g_StartVideo, VP_ISPLAYING) == false)
		{
			ShowWidget(g_wBlack, true)
			StopVideoSound()
			StWidStat = 6
		}
	break

	case 6:
		if(Delay(1))
		{
			StWidStat = 7
			ShowWidget(g_wBlack, false)
			LoadVideo(g_StartVideo, "video/logo_meqon", true)
			PlayVideo(g_StartVideo, VP_PLAY)
			ShowWidget(g_StartVideo, true)
		}
	break

	case 7:
		if(PlayVideo(g_StartVideo, VP_ISPLAYING) == false)
		{
			ShowWidget(g_wBlack, true)
			StopVideoSound()
			StWidStat = 8
		}
	break

	case 8:
		if(Delay(1))
		{
			ShowWidget(g_wBlack, false)
			LoadVideo(g_StartVideo, "video/nvidia", true)
			PlayVideo(g_StartVideo, VP_PLAY)
			ShowWidget(g_StartVideo, true)
			StWidStat = 9
		}
	break

	case 9:
		if(PlayVideo(g_StartVideo, VP_ISPLAYING) == false)
		{
			ShowWidget(g_wBlack, true)
			StopVideoSound()
			StWidStat = 10
		}
	break

	case 10:
		if(Delay(1))
		{
			ShowWidget(g_wBlack, false)
			LoadVideo(g_StartVideo, "video/soundblaster", true)
			PlayVideo(g_StartVideo, VP_PLAY)
			ShowWidget(g_StartVideo, true)
			StWidStat = 11
		}
	break

	case 11:
		if(PlayVideo(g_StartVideo, VP_ISPLAYING) == false)
		{
			StopVideoSound()
			StWidStat = 14
		}
	break
*/
	case 12:
		if(FadeWidget(g_wBlack, 300, FADE_BACK))
		{
			if(!g_StartScreen)
				g_StartScreen = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, 0, 0, 124)

			LoadWidgetImage(g_StartScreen, 0, "gfx/menu/apdemo.jpg")
			ShowWidget(g_StartScreen, true)
			SetWidgetColor(g_StartScreen, 0xffffffff)
			StWidStat = 13
		}
	break
	
	case 13:
		ShowWidget(g_wBlack, false)
		if(FadeWidget(g_StartScreen, 333, FADE_BACK))
			StWidStat = 14
	break

	case 14:
		//if(Delay(20000))
		//{
			StWidStat = 15
		//}
		break

	case 15:
		if(g_StartScreen)
			DestroyWidget(g_StartScreen)
			
		if(g_StartVideo)
			DestroyWidget(g_StartVideo)
			
		g_StartScreen = NULL
		g_StartVideo = NULL

		g_FirstRun = false
		InitMenu(true)
		PushMenu("main_menu")
//			ShowWidget(WComDemo, true)				
	break
	}
}

//=================================================================================
#ifdef _XBOX
void UpdateStatKeysXBox()
{
	if(JoyButtonState(JB_BACK, false))
	{
		if(JoyButtonState(JB_A, false))
		{
			JoyButtonState(JB_A, true)
			g_iStats++
			if(g_iStats >= 3)
				g_iStats = 0
		}

		if(JoyButtonState(JB_B, false))
		{
			JoyButtonState(JB_B, true)
			if(g_iWire == 0 && g_iDepthComplex == 0)
			{
				g_iWire = 1
			}
			else
			{
				if(g_iWire == 1)
				{
					g_iWire = 0
					g_iDepthComplex = 1
				}
				else
				{
					if(g_iDepthComplex == 1)
					{
						g_iDepthComplex = 0
						g_iWire = 0
					}
				}
			}
		}
	}
}
#endif
//=================================================================================
void UpdateStatKeys()
{
	if(KeyState(KC_RCONTROL) == true)
	{
		if(KeyState(KC_F8) == true)
		{
			ClearKey(KC_F8)

			d_showboxes = d_showboxes + 1 & 3
		}
	}

	if(KeyState(KC_F8) == true)
	{
		ClearKey(KC_F8)
		g_iStats++
		if(g_iStats >= 3)
			g_iStats = 0
	}

	if(KeyState(KC_F9) == true)
	{
		ClearKey(KC_F9)

		if(KeyState(KC_LSHIFT) || KeyState(KC_RSHIFT))
		{	
			r_showrb = 1 - r_showrb
		}
		else
		{
			if(g_iWire == 0 && g_iDepthComplex == 0)
			{
				g_iWire = 1
			}
			else
			{
				if(g_iWire == 1)
				{
					g_iWire = 0
					g_iDepthComplex = 1
				}
				else
				{
					if(g_iDepthComplex == 1)
					{
						g_iDepthComplex = 0
						g_iWire = 0
					}
				}
			}
		}
	}
}


//=================================================================================
int UpdateMenu()
{
	if(g_wBlack)
		ShowWidget(g_wBlack, false)	//v menu nesmie byt vidiet v ziadnom pripade cierny widget

	string res = GUIUpdateMenu()
	
	if(res == "_ESCAPE")	//escape v ktoromkolvek menu
	{
		if(MenuActive == "main_menu")	//quit z hry.
		{
//must not quit!!!
#ifdef _XBOX
			return true
#endif

			UnloadMenuMap()
			FreeMenu()
//#ifdef DEVELOPER
			Cleanup()
			return false
//#endif
			SetSplash()
			return false
		}

		if(ListKeyInput != 0)	//Kdyz se zrovna edituje klavesa v controls menu
			return true

		if(PopMenu(false))
			return true //keep in menu
		else
		{
			FreeMenu()

			//exception = back to game
			if(g_iMInGame == 4)
			{
				Back2Game(globalmap)
				return true
			}

//#ifdef DEVELOPER
			Cleanup()
			return false
//#endif
			SetSplash()
			return true
		}
	}

	if(res == "")
	{
		if(MenuActive == "controls_menu")
			SetItemsPosition(ListboxItemWidgets, ListboxPrimaryWidget, ListboxSecondaryWidget, ListboxBack, Scrollbar, SelectedLine, ListboxCol0Pos, ListboxCol1Pos, ListboxCol2Pos, ItemOffsetV, ListboxItemsNum)

		if(MenuActive == "loadgame_menu")
			ProcessLoadSavedMap()

		if(MenuActive == "savegame_menu")
			ProcessSaveMap()

		if(MenuActive == "credits_menu")
			UpdateCredits()
			
		return true //keep in menu
	}

	switch(MenuActive)
	{
	case "newgame_menu":
		g_RunMode = RUNMODE_NORMAL

		switch(res)
		{
		case "newgame_easy":
			BeforeNewGame(DIFFICULTY_EASY)
			g_DifficultySelected = DIFFICULTY_EASY
			break
		case "newgame_normal":
			BeforeNewGame(DIFFICULTY_NORMAL)
			g_DifficultySelected = DIFFICULTY_NORMAL
			break
		case "newgame_hard":
			BeforeNewGame(DIFFICULTY_HARD)
			g_DifficultySelected = DIFFICULTY_HARD
			break
		default:
			if(strstr(res, "newgame_") == 0)	//z pripdaneho rozstrelu
			{
				BeforeNewGame(DIFFICULTY_NORMAL)
				string levname = substr(res, 8, strlen(res) - 8)
				LoadCustomLevel(levname)
			}
			break
		}

#ifdef DEMO
		PushMenu("newgame_menu2")
#else
		LoadCustomLevel("l0i")
#endif
		break

#ifdef DEMO
	case "newgame_menu2":
		g_RunMode = RUNMODE_NORMAL

		if(strstr(res, "newgame_") == 0)	//z pripdaneho rozstrelu
		{
			string lvl = substr(res, 8, strlen(res) - 8)
			BeforeCustomLevel(g_DifficultySelected, lvl)
			LoadCustomLevel(lvl)
		}
		break
#endif

	case "main_menu":
		switch(res)
		{
			case "main_continue_game":
			g_RunMode = RUNMODE_NORMAL
			g_SaveSelected = 0
			LoadSavedPosition(GetLastSavedSlot())		//nahrajeme poslednu sejvnutu poziciu
			break

		case "main_new_game":	//natvrdo. pre vyber cez ikony je v menu.h GUIAction(GUI_ACTION_MENU, "newgame_menu")
			g_RunMode = RUNMODE_NORMAL
			BeforeNewGame(DIFFICULTY_NORMAL)
			LoadCustomLevel("l0i")
			break

		case "main_tutorial":
			g_RunMode = RUNMODE_TUTORIAL
			BeforeNewGame(DIFFICULTY_NORMAL)
			LoadCustomLevel("l00")
			break

		case "main_load_game":
//  					PushMenu("load_game")
//  					LoadSaveList()
//  					ShowSavesListbox(true)
			return true
		break

		case "main_exit":
			UnloadMenuMap()
			FreeMenu()
//#ifdef DEVELOPER
			Cleanup()
			return false
//#endif
			SetSplash()
			return true
		}
		break

		case "audio_menu":
			g_iEffectsVolume = ItemValues[26]
			g_iMusicVolume = ItemValues[27]

			if(res == "audio_eax")
			{
				if(ItemValues[11])
					GUIDisableItem("audio_quality")
				else
					GUIEnableItem("audio_quality")
			}
			break


		case "ingame_menu":
			switch(res)
			{
			case "ingame_resume_game":
				PopMenu(false)
				FreeMenu()
				Back2Game(globalmap)

				return true

			case "ingame_quit":
				UnloadMap()
				PopMenu(false)
				InitMenu(true)
				PushMenu("main_menu")				
				return true
			}
		break

		case "video_menu":
			if(res == "video_shaders")
			{
				//lower terrain quality
				ItemValues[35] = 0
				ItemValues[36] = 0
				ItemValues[37] = 0
			}

			if(res == "video_filtering")
			{
				if(ItemValues[23] != 2)
					GUIDisableItem("video_aniso")
				else
					GUIEnableItem("video_aniso")
			}
			break

		case "advanced_menu":
			if(res == "advanced_terrquality")
			{
				if(ItemValues[37])
				{
					GUIEnableItem("advanced_terrparallax")
				}
				else
				{
					if(ItemValues[35])
						SwitchMenuItem("advanced_terrparallax", 1)
					GUIDisableItem("advanced_terrparallax")
				}
			}
			break

		case "loadgame_menu":
			switch(res)
			{
			case "loadgame_load":	//Stlacene Load
				if(SaveTypes[g_SaveSelected] > SAVETYPE_EMPTY)
				{
					if(g_IMBackground)
					{
						DestroyWidget(g_IMBackground)
						g_IMBackground = NULL
					}
				
					UnloadMap()
					LoadSavedPosition(g_SaveSelected)
				}
				return true

			case "loadgame_cancel":
				if(ConfDialog)
				{
					delete ConfDialog
					ConfDialog = NULL	
				}
				LB_EnableEvents(true)
				return true

			case "loadgame_delete_yes":
				if(ConfDialog)
				{
					delete ConfDialog
					ConfDialog = NULL	
				}
				DeleteSavedPosition()
				ClearMousePress(0)
				ClearKey(KC_RETURN)
				LB_EnableEvents(true)
/*
				if(SaveTypes[g_SaveSelected] == SAVETYPE_NONE)
				{
					GUIDisableItem("loadgame_load")
					GUIDisableItem("loadgame_delete")
				}*/
				return true

			case "loadgame_delete":
				RequestForDelete("loadgame_delete_yes", "loadgame_cancel")
				return true
			}
			return true

		case "savegame_menu":
			switch(res)
			{
			case "savegame_overwrite_yes":
				if(ConfDialog)
				{
					delete ConfDialog
					ConfDialog = NULL	
				}
				SaveToSelectedSlot()
				LB_EnableEvents(true)
				PopMenu(false)	//hra bola ulozena takze skocime o menu vysie
				return true
			
			case "savegame_cancel":
				if(ConfDialog)
				{
					delete ConfDialog
					ConfDialog = NULL	
				}
				LB_EnableEvents(true)
				return true

			case "savegame_save":
				if(RequestForSave() == true)
					PopMenu(false)	//hra bola ulozena takze skocime o menu vysie
				return true

			case "savegame_delete_yes":
				if(ConfDialog)
				{
					delete ConfDialog
					ConfDialog = NULL	
				}
				DeleteSavedPosition()
				ClearMousePress(0)
				ClearKey(KC_RETURN)
				LB_EnableEvents(true)
/*
				if(SaveTypes[g_SaveSelected] == SAVETYPE_NONE)
				{
					GUIDisableItem("loadgame_load")
					GUIDisableItem("loadgame_delete")
				}*/
				return true

			case "savegame_delete":
				RequestForDelete("savegame_delete_yes", "savegame_cancel")
				return true
			}
			return true
		break
	}

	return true
}

//=================================================================================
int update()
{
	UpdateShaders()

#ifdef _XBOX
	UpdateJoyButtons()
#endif

	if(MenuMap != NULL)
	{
		PlayMap(MenuMap)
	}

	if(g_FirstRun)
	{
		ProcessStartScreens()
		return true
	}

#ifndef _XBOX
	UpdateMouseButtons()
	UpdateMouseStatus()
#endif

#ifdef DEVELOPER
	UpdateStatKeys()
#endif

#ifdef _XBOX
	UpdateStatKeysXBox()	
#endif

	switch(g_iMInGame)
	{
		//exit splash screen
	case 666:
		if(AnyKey())
		{
			LoadWidgetImage(WSplash, 0, "gfx/last_screen2.jpg")
			g_iMInGame = 667
		}
		return true

	case 667:
		if(AnyKey())
		{
			LoadWidgetImage(WSplash, 0, "gfx/last_screen3.jpg")
			g_iMInGame = 668
		}
		return true

	case 668:
		if(AnyKey())
		{
			DestroyWidget(WSplash)
			Cleanup()
			return false
		}
		return true

	//------------------------ IN GAME SECTION ---------------------------
	case -1:
		if(g_wLoading)
		{
			float c = sin(Flash) * 64 + 191
			SetWidgetTextColor(g_wLoading, AWHITE(c))
			Flash += 0.1
		}

		if(AnyKey())
		{
			AfterLoading()
			g_iMInGame = 1
		}
		return true

	case 1:
		return GameUpdate()

	//on game exit
	case 2:
		if(DebugLevel)
		{
			UnloadMap()
			SaveConfig()
			Cleanup()
			return false
		}

		InitMenu(false)
		PushMenu("ingame_menu")
		
		int MapHandle = SetCurrentMap(globalmap)
		DisableEnemies(true)
		World.PlayerControl = 0
		SetCurrentMap(MapHandle)

	   string bitmap
		int wflags = WF_VISIBLE | WF_NOFILTER | WF_SOURCEALPHA|WF_BLEND
		int wWidth, wHeight

		g_IMBackground = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, wflags, 0xaa7f7f7f, 200)
		LoadWidgetImage(g_IMBackground, 0, "gfx/black.tga")

		g_iMInGame = 4
		return true
	//------------------------ IN MENU SECTION ---------------------------
	
	default:
		return UpdateMenu()
	}

	return true
}

//=================================================================================
int init()
{
	//debug
	LoadConfig()

#ifdef DEVELOPER
	LoadDeveloperConfig()
#endif

	g_lod = 0.001
	g_iFly = false
	g_fDopplerFactor = 1.0
	return true
}

#ifdef DEVELOPER
	#include "scripts/viewer.h"
	#include "scripts/pviewer.h"
#endif

//=================================================================================
void InitConfig()
{
	string vmode = itoa(g_iWidth) + "x" + itoa(g_iHeight)
	
	for(g_iVMode = 0; g_iVMode < sizeof(g_sVideoModes); g_iVMode++)
	{
		if(vmode == g_sVideoModes[g_iVMode])
			break

		if(g_sVideoModes[g_iVMode] == "")
		{
			g_sVideoModes[g_iVMode] = vmode
			break
		}		
	}	
}

#ifdef _XBOX
int g_expired
#endif

//=================================================================================
void BasicInit()
{
	WView = CreateWidget(WTYPE_VIEW, 0, 0, 800, 600, WF_VISIBLE, 0, 0)
	WMousePointer = CreateWidget(WTYPE_IMAGE, 0, 0, 63, 63, WF_SOURCEALPHA|WF_BLEND|WF_EXACTSIZE|WF_NOFILTER, 0xffffffff, 999)
	LoadWidgetImage(WMousePointer, 0, "gfx/cursor.tga")

	LargeFont = CreateFontEx("font_large")
	SmallFont = CreateFontEx("font_small")
	MainFont = CreateFontEx("font_main")

	CMinClass = Type("MinClass")
	CExtClass = Type("ExtClass")
	CFuncClass = Type("FuncClass")
	CDoorClass = Type("DoorClass")
	CItemClass = Type("ItemClass")
	CItemMisc = Type("item_misc")

	CFireWeaponClass = Type("FireWeaponClass")
	CSteelWeaponClass = Type("SteelWeaponClass")
	CWeaponClass = Type("WeaponClass")

	CTriggerClass = Type("TriggerClass")
	CTriggerHackDetector = Type("trigger_hack_detector")
	CCreatureClass = Type("CreatureClass")
	CAICreatureClass = Type("AICreatureClass")
	CMiscPhysicsModel = Type("misc_physics_model")
	CPhysicsPart = Type("physics_part")
	CMiscPhysicsGlass = Type("misc_physics_glass")
	CItemWeaponMine = Type("item_weapon_mine")
	CGlassDebris = Type("GlassDebris")
	CItemWeaponSteel = Type("item_weapon_steel")
	CBrkDebris = Type("brk_debris")
	CProjectile = Type("Projectile")
	CTriggerDynamic = Type("trigger_dynamic")
	CFuncDoor = Type("func_door")
	CFuncDoorRot = Type("func_door_rot")
	CRigidBodyClass = Type("RigidBodyClass")
	CMiscPhysicsPack = Type("misc_physics_pack")
	CParticleEffector = Type("misc_particle_effector")
	CInfoWaypoint = Type("info_waypoint")
	CInfoPath = Type("info_path")
	CInfoConnection = Type("info_connection")
	CInfoNull = Type("info_null")
	CInfoPathSwitch = Type("info_pathswitch")
	CEnterableVehicle = Type("EnterableVehicle")
	CRBone = Type("RBone")
	CJointClass = Type("JointClass")

	if(!dMaterialLoad("obj/materials.xml"))
	{
		DPrint("Can't load obj/materials.xml")
	}
}

//=================================================================================
bool CommonInit(bool initanims, bool initsounds)
{
	//pomocne bitove masky
	TRACE_DEFAULT = TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH|TRACE_ENTS
	NOISE_NOSHOT_MASK = NOISE_FOOTSTEP|NOISE_THUD

	if(!CompileAnims("anm/anim.def"))
		return false

	if(LoadStringTable("strings.xml") == 0)
		DPrint("Can't load string table")

	InitSkeletons()

#ifdef DEVELOPER
	ComputerName = "                                                             "
	int len[2] = {33}

	GetComputerNameA(ComputerName, len)
	strlower(ComputerName)
#endif

	if(initsounds)
	{
		LoadSoundSets()
		LoadSounds()
	}
	
	LoadSoundMaterials()

	Init_Keynames()
	InitWidgets()
	CalculateGlassMaskUV()	//predpocita UVcka pre vsetky rozbitelne skla
	
	g_iPhysicsIteration = 20		//pocet iteracii.
	g_fPhysicsErrorReduction = 10	//vysia hodnota viac napravuje chyby.
	g_fPhysicsTolerance = 0.001			//vsetky iteracie sa nevykonaju pokial su chyby mensie nez tato hodnota. default 0
	g_fPhysicsCollisionEpsilon = 0.015 //tolerance detektoru kolizi

	float Extends[2]
	Extends[0] = PlayerCapsuleExtends[0] * 2
	Extends[1] = PlayerCapsuleExtends[1] * 2
	
	for(int n = 0; n < 2; n++)
	{
		PlayerCapsuleHeights[n] = Extends[n] + (PlayerCapsuleRadius * 2)
		PlayerCapsuleHalfHeights[n] = PlayerCapsuleHeights[n] * 0.5
	}

	InitPhysicsPartsDef()
	InitDecalDefs()
	InitHurtFactors()
}

//=================================================================================
int main(string parms)
{
//	parms = "l01"
//d_pathengine = 1
//r_specularbias = 1
//r_diffuse = 0
	
	InitConfig()
	SaveConfig()
	
	BasicInit()

	strlower(parms)

	if(strstr(parms, ".ptc") != -1)
		CommonInit(false, false)
	else
	{
		if(strstr(parms, ".xob") != -1)
		{
			CompileAnims("anm/anim.def")
		}
		else
			CommonInit(true, true)
	}

#ifdef DEVELOPER

	if( parms != "")
	{
		g_iMInGame = 1
		DebugLevel = true

		if(strlen(parms) > 9 && substr(parms, 0, 9) == "creature_")
		{
			gOBJType = 4
			g_Viewer = true
			if(CreatureViewer(parms) == false)
			{
				Cleanup()
				return false
			}
		}
		else
		{
			string name
			int p = strstr(parms, tostr(34))
			if(p >= 0)
			{
				name = substr(parms, p + 1, strlen(parms) - p - 1)

				p = strstr(name, tostr(34))
				if(p >= 0)
				{
					name = substr(name, 0, p)
				}
				name = GetRelativeFilePath(name)
			}
			else
				name = GetRelativeFilePath(parms)
			
			if(strstr(name, ".xob") != -1)
			{
				gOBJType = 1
				g_Viewer = true
				if(TOBViewer(name) == false)
				{
					Cleanup()
					return false
				}
			}
			else
			{
				if(strstr(name, ".dpl") != -1)
				{
					g_Viewer = true
					if(DPLViewer(name) == false)
					{
						Cleanup()
						return false
					}
				}
				else
				{					
					if(strstr(name, ".ptc") != -1)
					{
						gOBJType = 2
						g_Viewer = true
						if(TOBViewer(name) == false)
						{
							Cleanup()
							return false
						}
					}
					else
					{
						if(WRLViewer(name) == false)
						{
							Cleanup()
							return false
						}
						return true
					}
				}
			}
		}

		Back2Game(globalmap)
		return true
	}

#endif

	DebugLevel = false
	SaveConfig()

#ifdef DEVELOPER
	InitMenu(true)
	PushMenu("main_menu")
#else
//	LoadStartScreens()
	LoadBlackWidget()			//prekryjeme uvodne bliknutie
	ShowWidget(g_wBlack, true)
	g_FirstRun = true
#endif

	return true
}
