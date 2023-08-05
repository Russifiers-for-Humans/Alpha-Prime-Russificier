
#define DEVELOPER
#define EDITOR

proto native void OnCameraMove()
proto native void RestetGUIInput()
proto native void ClipCursorInsideWindow(int clip)
proto native void ClampCursorInsideWindow()
proto native void SetGUICursorType(int type)
proto native int GetWindowsCurrentTime()
proto native int IsAppActive()
proto native void SetFocusToRenderWindow()
proto native void SetLoadProgress(int pos)
proto int TextEntryDialog(string EditText, string Label)
proto int RequestDialog(string Request, string Label, int Type)
proto int SingleChoiceDialog(string Label, string Choices[], int NumChoices, int Select)
proto void ToScreenPos(int X, int Y)
proto native void PrintToStatusBar(int Field, string text)
proto void PrintAreaEditInfo(int line, string text)
proto void UpdateAreaGroupList(string Groups[], int NumGroups)
proto void UpdateGroupSelection(string group)
proto native void SetCurrentAreaAccesibility(int status)
proto native void SetAreaFlagsInGUI(int flags)
proto native int GetAreaFlags()
proto string GetAreaNameInGUI()
proto void SetAreaNameInGUI(string name)
proto native void OnDataChange()
proto native int GetNotebookActivePage()
proto native int GetKeyDefIndex(int XMLElement, string name)	//vracia index konkretneho kluca z XML elementu definujuceho entitu. napriklad coords je na druhej pozicii
proto int GetClassNamesFromXML(string ClassNames[])
proto native void ShowEntityPopupMenu()
proto void SetItemNames(string names[], int NumNames)

proto void GetEntityDefinitionsFromXML(class definition, string ClassName)

proto void ShowSelectionProperties(string ClassName, string Keys[], string Values[], int NumKeys)
proto void FillMethodsViewComponent(string Methods[], int NumMethods, string HelpTexts[])
proto void EntityListAdd(string EntNames[], string EntClassNames[], int NumEntities)
proto void EntityListRemove(string EntNames[], string EntClassNames[], int NumEntities)
proto void FillEntityStates(string States[], int NumStates)
proto void FillFilter(string ClassNames[], int NumClassNames)

proto native void BeginSetupKeyEditComponent()
proto void AddKeyToEditComponent(string Key, string Value, string Choices[], int NumChoices, int VarType, int ControlType, string HelpText)
proto native void EndSetupKeyEditComponent()
proto void ChangeKeyInGUI(string key, string val, string VarType)
proto void OnUpdateSelection(string EntName, string ClName, string PrefabName, int NeedSelect)
proto void UpdatePlayerPointer(class PlayerPtr)


const int wx_BACK			= 8
const int wx_TAB     	= 9
const int wx_RETURN  	= 13
const int wx_ESCAPE  	= 27
const int wx_SPACE   	= 32
const int wx_DELETE  	= 127
                     	

const int wx_START   	= 301
const int wx_LBUTTON 	= 302
const int wx_RBUTTON 	= 303
const int wx_CANCEL		= 304
const int wx_MBUTTON 	= 305
const int wx_CLEAR		= 306
const int wx_SHIFT		= 306		//je to divne. niektore akoby nesedeli
const int wx_CONTROL		= 308
const int wx_MENU			= 309
const int wx_PAUSE		= 310
const int wx_CAPITAL 	= 311
const int wx_PRIOR		= 312
const int wx_NEXT			= 313
const int wx_END			= 314
const int wx_HOME			= 315
const int wx_LEFT			= 316
const int wx_UP			= 317
const int wx_RIGHT		= 318
const int wx_DOWN			= 319
const int wx_SELECT		= 320
const int wx_PRINT		= 321
const int wx_EXECUTE		= 322
const int wx_SNAPSHOT	= 323
const int wx_INSERT		= 324
const int wx_HELP			= 325
const int wx_NUMPAD0 	= 326
const int wx_NUMPAD1 	= 327
const int wx_NUMPAD2 	= 328
const int wx_NUMPAD3 	= 329
const int wx_NUMPAD4 	= 330
const int wx_NUMPAD5 	= 331
const int wx_NUMPAD6 	= 332
const int wx_NUMPAD7 	= 333
const int wx_NUMPAD8 	= 334
const int wx_NUMPAD9 	= 335
const int wx_MULTIPLY	= 336
const int wx_ADD			= 392//337
const int wx_SEPARATOR	= 338
const int wx_SUBTRACT	= 394//339
const int wx_DECIMAL		= 340
const int wx_DIVIDE		= 341
const int wx_F1			= 342
const int wx_F2			= 343
const int wx_F3			= 344
const int wx_F4			= 345
const int wx_F5			= 346
const int wx_F6			= 347
const int wx_F7			= 348
const int wx_F8			= 349
const int wx_F9			= 350
const int wx_F10			= 351
const int wx_F11			= 352
const int wx_F12			= 353
const int wx_F13			= 354
const int wx_F14			= 355
const int wx_F15			= 356
const int wx_F16			= 357
const int wx_F17			= 358
const int wx_F18			= 359
const int wx_F19			= 360
const int wx_F20			= 361
const int wx_F21			= 362
const int wx_F22			= 363
const int wx_F23			= 364
const int wx_F24			= 365
const int wx_NUMLOCK 	= 366
const int wx_SCROLL  	= 367

const int wx_ALT			= 309
const int wx_A				= 65
const int wx_D				= 68
const int wx_W				= 87
const int wx_S				= 83

const int wx_1 = 49
const int wx_4 = 52
const int wx_5 = 53
const int wx_L = 76
const int wx_X = 88
const int wx_Q = 81
const int wx_E = 69
const int wx_O = 79
const int wx_N = 78
const int wx_H = 72
const int wx_G = 71
const int wx_T = 84




const int GUI_REQUEST_YES = 1
const int GUI_REQUEST_NO = 0

const int GUI_DIALOG_OK = 0
const int GUI_DIALOG_YES_NO = 1

const int CtrlType_EditBox = 0
const int CtrlType_ComboBox = 1
const int CtrlType_CheckBox = 2
const int CtrlType_Slider = 3
const int CtrlType_IncrementBox = 4
const int CtrlType_FileRequester = 5
const int CtrlType_FlagsEdit = 6
const int CtrlType_EventButton = 7

const int VarType_Int = 0
const int VarType_Float = 1
const int VarType_Bool = 2
const int VarType_String = 3
const int VarType_Vector = 4

#include "scripts/script.c"

/*
 Timhle se rekne skriptu, ze tyto metody existuji, jake maji parametry
 a jakym stylem se volaji. Ty, co jsou 'native' maji 1:1 prototypy s
 implemetaci v C++. Ty co nejsou, dostavaji pole script-promennych jako
 parametry. Vyhoda je, ze se z teto reprezentace da zjistit daleko vic,
 nez jen z pouhych dat...
*/

//=================================================================================
void UpdateEditor()
{
	currenttime += 0.02//ftime
	UpdateShaders()
	UpdateMouseButtons()
	UpdateMouseStatus()
	UpdateStatKeys()
}

//=================================================================================
void InitEditor(int hodnota)
{
	LoadConfig()
	g_iWidth = 640
	g_iHeight = 480
	g_iFullscreen = false
	g_iVSync = 0
	g_lod = 0.21
	g_iFly = false
	g_fDopplerFactor = 1.5
}

//-----------------------------------------------------------------------------------
void InitEditor2()
{
	BasicInit()
	CommonInit(true, true)
	BeforeRunFromCommandLine(DIFFICULTY_HARD)
}

//-----------------------------------------------------------------------------------
void OpenScriptEditor()
{
	if(World.EditorActive == true)
		return

	EntityEditor Editor = World.Editor
	creature_player pla = World.Player

	Editor.Activate(true)
	Editor.SetCam(World.CameraPos, World.CameraAngs)
	pla.ShowHUD(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
	World.Cross.Show(false)
	World.EditorActive = true
	World.WorkbenchActive = true
}

//-----------------------------------------------------------------------------------
void CloseScriptEditor()
{
	if(World.EditorActive == false)
		return

	EntityEditor Editor = World.Editor
	creature_player pla = World.Player
	Editor.Activate(false)
	pla.ShowHUD(-1, -1, -1, 1, -1, 1, 1, 0, 0, -1)
	
	if(World.Cross)
		World.Cross.Show(true)

	World.EditorActive = false
}

//-----------------------------------------------------------------------------------
void InitScriptEditor()
{
	if(!World.Editor)
		World.Editor = new EntityEditor
}
//-----------------------------------------------------------------------------------
void OnInitApplication()			//uz je inicializovane MDI okno ale este sa nespustil ziadny modul
{
//	DeleteEntityDefinitions()		//vymazeme definicie z entities.def
//	LoadEntKeysFromRadiantDef()	//nahrajeme nove definicie
}

void OnCloseApplication()	//po tomto bude zmazany engine
{
	if(PrefManager)
	{
		delete PrefManager
		PrefManager = NULL
	}

	if(DefManager)
	{
		delete DefManager
		DefManager = NULL
	}	
}

//-----------------------------------------------------------------------------------
void OnInitEditorInstance()	//initializovalo sa GUI editoru ale este neni nahrana mapa
{
	if(!DefManager)
		DefManager = new EntityDefManager()

	if(!PrefManager)
		PrefManager = new PrefabManager()

	string Current
	string ListArray[256]
	int	 ListArrayNum = 0
	EntityDef CurDef

	for(int n = 0; n < DefManager.EntDefsNum; n++)
	{
		CurDef = DefManager.EntDefs[n]
		Current = CurDef.ClassName

		if(Current == "MinClass" || Current == "RigidBodyClass" || Current == "TriggerClass" || Current == "FuncClass" || Current == "ItemClass" || Current == "DoorClass")
			continue
			
		if(Current == "CreatureClass" || Current == "TrapClass" || Current == "WeaponClass" || Current == "viewer" || Current == "world" || Current == "creature_player")
			continue

		if(Current == "light")
			continue

		ListArray[ListArrayNum++] = Current
	}
	
	FillFilter(ListArray, ListArrayNum)	//naplni filter tej instancii
}

//-----------------------------------------------------------------------------------
void AfterLoadMap(string LoadedMap)	//mapa bola nahrana ale instancia editoru este nebola vytvorena
{
	LoadedMap = ReplaceChars(LoadedMap, "\", "/")

	string level
	int pos = strstr(LoadedMap, ".wrl")

	if(pos >= 0)
		level = substr(LoadedMap, 0, pos)

	pos = strstr(level, "/")
	while(pos >= 0)
	{
		level = substr(level, pos + 1, strlen(level) - pos - 1)
		pos = strstr(level, "/")
	}

	World.ThisLevelName = level
	
	int lng = strlen(level) + 4	//.wrl
	int lng2 = strlen(LoadedMap)

	World.ThisLevelDir = substr(LoadedMap, 0, lng2 - lng)
}

//-----------------------------------------------------------------------------------
WorldClass GetWorldPtr()
{
	return World	
}