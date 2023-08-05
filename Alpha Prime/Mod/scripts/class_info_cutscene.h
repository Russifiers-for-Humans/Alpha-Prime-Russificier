
//SCENE_ROOT = 0
const int B_LW_Camera1 = 1
const int B_LW_Camera1_zoom = 2
const int B_LW_Camera2 = 3
const int B_LW_Camera2_zoom = 4
const int B_LW_Camera3 = 5
const int B_LW_Camera3_zoom = 6
const int B_LW_Camera4 = 7
const int B_LW_Camera4_zoom = 8
const int B_LW_Camera5 = 9
const int B_LW_Camera5_zoom = 10
const int B_LW_Camera6 = 11
const int B_LW_Camera6_zoom = 12
const int B_LW_Camera7 = 13
const int B_LW_Camera7_zoom = 14
const int B_LW_Camera8 = 15
const int B_LW_Camera8_zoom = 16
const int B_LW_Camera9 = 17
const int B_LW_Camera9_zoom = 18
const int B_LW_Camera10 = 19
const int B_LW_Camera10_zoom = 20
const int B_LW_Camera11 = 21
const int B_LW_Camera11_zoom = 22
const int B_LW_Camera12 = 23
const int B_LW_Camera12_zoom = 24
const int B_LW_Camera13 = 25
const int B_LW_Camera13_zoom = 26
const int B_LW_Camera14 = 27
const int B_LW_Camera14_zoom = 28
const int B_LW_Camera15 = 29
const int B_LW_Camera15_zoom = 30

vector g_AnimCameraPos, g_AnimCameraAngs

float UpdateAnimCamera(MinClass owner, int CamBone, vector offset)
{
	float atime = BumpFrame(owner, ftime, 0)
	UpdateEntity(owner)
	vector ang
	vector mat[4]
	GetBoneMatrix(owner, CamBone - 1, mat)

	//convert from LW
	ang = MatrixAngles(mat)

	vector pos
	pos[0] = mat[3][1]
	float ff = mat[3][0], fg
	pos[1] = -ff
	pos[2] = mat[3][2]

	ff = ang[0]
	fg = ang[2]
	ang[0] = -fg
	ang[2] = -ff

	g_AnimCameraPos = pos + offset
	g_AnimCameraAngs = ang
	
	//FOV je na druhe kosti
	GetBoneMatrix(owner, CamBone, mat)
	SetCameraVerticalFOV(0, mat[3][2] * 0.03125 * RAD2DEG)
	
	return atime
}

//-----------------------------------------------------------------
bool FromDirector
bool GrabVideo
int LastVideoFrame


widget DProgressBar,DProgressBar2,DTime
widget CTopBand,CBottomBand,CFlash
float SLength
bool DirPause
int InCutscene

//-----------------------------------------------------------------
class info_cutscene extends ExtClass
{
string OnStart, OnEnd
vector angles
string name
int flags

tparse tp
int line
float scenetime, lasttime
int ntokens
vector coords

MinClass target
int lpath,tpath
float band
int timescale

float yaw
int fcol
float fadetime, fadetimer

string tokens[100]
vobject CameraAnim, CameraHierarchy

int mode
bool disabled

vector CameraAngles, CameraPos

string HoldID
bool IsRunning


void LoadFlashWidget(int sort)
{
	if(CFlash)
		DestroyWidget(CFlash)
	
	CFlash = NULL
	CFlash = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, sort)
	LoadWidgetImage(CFlash, 0, "gfx/white.tga")
}

//-----------------------------------------------------------------
int NextLine()
{
	string token
	
	ntokens = ParseLine(tp, line, tokens)
	if(ntokens < 2)
		return false

	token = tokens[0]
	lasttime = atof(token)

	token = tokens[1]
	if(token != ":")
	{
		DPrint("Expected ':' in cutscene")
		return false
	}
	return true
}

//-----------------------------------------------------------------
void CameraRun(string num)
{
	mode = atoi(num)
}

//-----------------------------------------------------------------
void CameraFPS(string fps)
{
	SetAnimSlot(this, 0, CameraAnim, 0, 1, NULL, atoi(fps), AF_FORCEFPS)
}

//-----------------------------------------------------------------
void CameraSeek(string frame)
{
	scenetime = atof(frame)
	lasttime = scenetime
	SetFrame(this, 0, scenetime)
}

//-----------------------------------------------------------------
void LoadCamera(string name)
{
	vobject cam = GetObject(name)

	if(cam)
	{
		if(CameraAnim)
			ReleaseObject(CameraAnim, 0)

		CameraAnim = cam

		if(!CameraHierarchy)
		{
			CameraHierarchy = GetObject("obj/camera.xob")
			SelectObject(this, CameraHierarchy)
			SetEventMask(this, EV_ANIMEND)
		}
		SetAnimSlot(this, 0, CameraAnim, 0, 1, NULL, 30, AF_ONCE|AF_RESET)
		
		//HACK: supress transformation of object skeleton
		SetFlags(this, TFL_NOBILLBOARD)
		 
		 //co s tou svini je???
		SetOrigin(this, "1 1 1")
		SetOrigin(this, coords)
		BumpFrame(this,0,0)
		UpdateEntity(this)
	}
}

//-----------------------------------------------------------------
bool Loop

int Run()
{
	//HACK: something very bad happened to our hero
	if(World.Player.status == STATE_DEAD)
		return false


	if(band < 100)
	{
		band = ftime * 100 + band
		clamp band<0,100>

		SetWidgetPos(CTopBand, 0, 0, 800, band)

		SetWidgetPos(CBottomBand, 0, 600 - band, 800, band)
	}

#ifdef _XBOX
	if(JoyButtonState(JB_START))
	{
		ClearJoyButton(JB_START)
#else
	if(KeyState(KC_ESCAPE))
	{
		ClearKey(KC_ESCAPE)
#endif
		if(!flags & 1)
		{
			World.CutSceneSkip = true
			World.OnCutSceneSkip()
		}
		HoldID = ""
	}
#ifdef DEVELOPER
//update progress bar
	if(FromDirector)
	{
		if(GrabVideo && LastVideoFrame != scenetime)
		{
			LastVideoFrame = scenetime
			if(scenetime > 0)
			{
				//AppendVideoFrame()
				MakeScreenshot(String("video/" + _name + "_" + itoal(scenetime - 1, 5) + ".tga"))
			}
		}

		float w = scenetime * 800 / SLength
		clamp w<0,800>

		SetWidgetPos(DProgressBar2, 0,580,w,20)
		SetWidgetText(DTime, 0, itoa(scenetime))

		w = scenetime * 800 / SLength
		clamp w<0,760>
		SetWidgetPos(DTime, w, 580, 40, 20)

		if(KeyState(KC_UP))
		{
			timescale = timescale + 10
			clamp timescale<10,1000>
		}

		if(KeyState(KC_DOWN))
		{
			timescale = timescale - 10
			clamp timescale<10,1000>
		}

		if(KeyState(KC_RIGHT))
			timescale = 100

		if(KeyState(KC_SPACE))
		{
			ClearKey(KC_SPACE)
			DirPause = 1 - DirPause
			return true
		}

		if(KeyState(KC_RETURN))
		{
			ClearKey(KC_RETURN)
			line = 0
			scenetime = 0
			target = NULL
			NextLine()
			return true
		}

		if(DirPause != false)
			return true
	}
#endif

	if(!World.CutSceneSkip && HoldID)
	{
		if(World.IsDialogActive(HoldID))
			return true
		HoldID = ""
	}
	
	//when skipping, don't move camera
	if(!World.CutSceneSkip)
	{
		if(mode == 0)	//static camera
		{
			scenetime = ftime * timescale + scenetime
	
			if(link)
			{
				MinClass cl
				cl = FindClass(link)
				if(cl != NULL)
				{
					vector v = GetMatrix(cl, 3) - CameraPos
					float f = v[2]
					v[2] = -f
					CameraAngles = Vector2Angles(v)
				}
			}
		}
		else	//lightwave camera
		{
			scenetime = UpdateAnimCamera(this, mode * 2, coords)
			CameraPos = g_AnimCameraPos
			CameraAngles = g_AnimCameraAngs
		}
		
		World.CameraPos = CameraPos + EarthquakeOff
		World.CameraAngs = CameraAngles + EarthquakeAng
		World.CameraNorm = Angles2Vector(World.CameraAngs)
		SetCamera(World.CameraPos, World.CameraAngs)
	}
	
	if(fcol > 0)
	{
		fcol -= ftime * 600
		if(fcol <= 0)
		{
			fcol = 0
			ShowWidget(CFlash, false)
		}
		else
			SetWidgetColor(CFlash, ARGB(fcol,fcol,fcol,fcol))
	}

	if(fadetime != 0)
	{
		if(fadetime > 0 && fadetimer < fadetime)
		{ //fadein			
			fadetimer += ftime * 100
			clamp fadetimer<0, fadetime>
			float alp = fadetimer / fadetime * 255.0
			SetWidgetColor(CFlash, ARGB(alp, 0, 0, 0))
			if(fadetimer == fadetime)
				fadetime = 0
		}
		else
		{ //fadeout			
			fadetimer -= ftime * 100
			alp = fadetimer / fadetime * -255.0 + 255.0
			SetWidgetColor(CFlash, ARGB(alp, 0, 0, 0))
			
			if(fadetimer < fadetime)
			{
				fadetime = 0
				ShowWidget(CFlash, false)
			}
		}
	}
		
	if(Loop && !World.CutSceneSkip)
		return true

	//time to execute line
	while(World.CutSceneSkip || scenetime >= lasttime)
	{
		int t, st, lt
		string token,token1,token2,token3,token4
		bool fskip = false

		t = 2
		while(t < ntokens)
		{
			token = tokens[t++]

			//method call
			if(t < ntokens && tokens[t] == ".")
			{
				string uv = tostr(34)
				
				while(t < ntokens && tokens[t] != ";")
				{
					token2 = tokens[t++]
					if(substr(token2, 0, 1) == uv)
						token += substr(token2, 1, strlen(token2) - 2)
					else
						token += token2
				}
				
				if(World.CutSceneSkip && strstr(token, ".Say(") != -1)
					goto next

				if(!fskip)
					CallMethods(this, token)
				goto next
			}

			switch(token)
			{
			case "debug":
				debug
			break
			
			case "precache":
				if(t >= ntokens)
					goto err
				GetObject(tokens[t++])
			break

			case "disable_enemies":
				DisableEnemies(false)
			break

			case "enable_enemies":
				EnableEnemies()
			break

			case "fskip":
				fskip = World.CutSceneSkip
			break

			case "fadein":
				if(t >= ntokens)
					goto err
					
				token = tokens[t++]

				int sort = 118
				
				if(tokens[t] == "onback")
				{
					sort = 11
					t++
				}

				if(!World.CutSceneSkip)
				{
					LoadFlashWidget(sort)
					ShowWidget(CFlash, true)
					SetWidgetColor(CFlash, 0x00000000)
					fadetime = atof(token)
					fadetimer = 0
				}				
			break
			
			case "fadeout":
				if(t >= ntokens)
					goto err
					
				token = tokens[t++]

				sort = 118
				
				if(tokens[t] == "onback")
				{
					sort = 11
					t++
				}

				if(!World.CutSceneSkip)
				{
					LoadFlashWidget(sort)
					ShowWidget(CFlash, true)
					SetWidgetColor(CFlash, 0xff000000)
					fadetime = -atof(token)
					fadetimer = 0
				}				
			break
			
			case "flash":
				if(!World.CutSceneSkip)
				{
					LoadFlashWidget(118)
					ShowWidget(CFlash, true)
					SetWidgetColor(CFlash, 0xffffffff)
					fcol = 255
				}
			break

			case "end":
				return false
			break

#ifdef DEVELOPER
			case "pause":
				DirPause = 1 - DirPause
			break
#endif
			case "change_level":
				if(t >= ntokens)
					goto err
				t++
				DPrint("Cutscene: change_level is obsolete command. Use worldspawn.ChangeLevel instead")
			break

			case "camera":
				if(t >= ntokens)
					goto err

				token = tokens[t++]
				if(!World.CutSceneSkip)
					LoadCamera(token)
			break

			case "camera_seek":
				if(t >= ntokens)
					goto err

				token = tokens[t++]
				if(!World.CutSceneSkip)
					CameraSeek(token)
			break

			case "camera_fps":
				if(t >= ntokens)
					goto err

				token = tokens[t++]
				if(!World.CutSceneSkip)
					CameraFPS(token)
			break

			case "camera_play":
				if(t >= ntokens)
					goto err

				token = tokens[t++]
				if(!World.CutSceneSkip)
					CameraRun(token)
			break

			case "camera_stop":
				mode = 0
				scenetime = 0
			break

			case "hold":
				if(t >= ntokens)
					goto err

				token = tokens[t++]
				if(!World.CutSceneSkip)
					HoldID = itoal(atoi(token), 4)
			break
			
			case "loop":
				Loop = true
			break
			
			case "say":
				if(t >= ntokens)
					goto err

				token = tokens[t++]

				if(!World.CutSceneSkip)
					World.Dialog(World, token, SF_AMBIENT)
			break

			case "shutup":
				World.CloseCutSceneDialog()
			break

			default:
				DPrint("Unknown cutscene command " + token)
			}
next:;
			if(token != "fskip")
				fskip = false

			if(t < ntokens)
			{
				token = tokens[t]
				if(token != ";")
					goto err

				t++
			}
		}

		line++

		if(!NextLine())
			return false

		if(HoldID)
			return true
	}

	return true

err:
	DPrint("Error in cutscene on line" + itoa(line))
	return false
}

//-----------------------------------------------------------------
void CleanupScene()
{
	if(tp)
	{
		EndParse(tp)
		tp = NULL
	}

	if(lpath)
	{
		RemoveBezier(lpath)
		lpath = NULL
	}

	if(tpath)
	{
		RemoveBezier(tpath)
		tpath = NULL
	}
}

hsound CachedSounds[128]
int NumCachedSounds

CreatureClass CachedPuppets[16]
int NumCachedPuppets

//-----------------------------------------------------------------
void Precache(tparse t)
{
	int line = 0

	NumCachedSounds = 0
	NumCachedPuppets = 0

	while(true)
	{
		int ntokens = ParseLine(t, line, tokens)

		if(ntokens <= 0)
			return

		int n = 2
		while(n < ntokens)
		{
			string token = tokens[n++]

			//skip method call
			if(n < ntokens && tokens[n] == ".")
			{
				if(n > 0)
				{
					CreatureClass ec = FindClass(tokens[n - 1])
					if(ec && ClassName(ec) == "creature_puppet")
					{
						for(int p = 0; p < NumCachedPuppets; p++)
						{
							if(CachedPuppets[p] == ec)
								break
						}

						if(p == NumCachedPuppets)
						{
							CachedPuppets[NumCachedPuppets++] = ec
							LoadSoundSet(ec.SoundSetName, ec.CSounds)
						}
					}
				}

				if(tokens[n + 1] == "Say")
				{
					CachedSounds[NumCachedSounds] = GetSoundEx(tokens[n + 3], 0, SF_NOERROR)
					if(CachedSounds[NumCachedSounds])
						NumCachedSounds++
				}

				while(n < ntokens && tokens[n] != ";")
					n++
			}		

			if(n >= ntokens)
				break

			switch(token)
			{
			case "say":
				CachedSounds[NumCachedSounds] = GetSoundEx(RemoveQuotes(tokens[n]), 0, SF_NOERROR)
				if(CachedSounds[NumCachedSounds])
					NumCachedSounds++
				n++
				break

			case "camera":
			//	Print(String("camera " + tokens[n]))
				n++
				break

			default:
				//skip command
				while(n < ntokens && tokens[n] != ";")
					n++
				break
			}
		}
		line++
	}
}

widget TempMessage
bool IsFinal

//-----------------------------------------------------------------
void StartScene()
{
	InCutscene++
	IsRunning = true
	
	string sname = "cutscene/" + name + ".scn"
	if(!name)
		sname ="cutscene/default.scn"

	tp = BeginParse(sname)
	if(tp == NULL)
	{
		DPrint("Error loading cutscene " + sname)
		ClearEventMask(this,EV_ALL)
		return
	}
	
	Precache(tp)

	disabled = true

	line = 0
	scenetime = 0
	mode = 0
	Loop = false
	
	target = NULL
	NextLine()

	World.CutScene = true
	World.CutSceneSkip = false
	World.ShowCursor = false

	for(ExtClass ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		ec.BeginCutscene()

	band = 0

	//World.pfov = r_fovy
	//fov = World.pfov * 2
	yaw = 0

	fadetime = 0
	fadetimer = 0

	SetWidgetPos(CTopBand,0,0,0,0)
	SetWidgetPos(CBottomBand,0,0,0,0)
	ShowWidget(CTopBand,true)
	ShowWidget(CBottomBand,true)

	World.CloseCutSceneDialog()
	World.StopDialogs()

	HoldID = ""

	if(!IsFinal)
	{
		if(!TempMessage)
		{
			TempMessage = CreateWidget(WTYPE_TEXT, 400, 5, 380, 40, WF_SOURCEALPHA|WF_BLEND|WF_RALIGN, ARGB(255,255,202,0), 10000)
			SetFont(TempMessage, MainFont)
			SetWidgetText(TempMessage, 0, "Temporal cutscene placeholder")
			SetWidgetText(TempMessage, 1, "Please, don't report anything about it")
			ShowWidget(TempMessage, true)
		}	
	}

#ifndef DEVELOPER
	timescale = 100
#endif
}

//-----------------------------------------------------------------
void M_Start(ExtClass other)
{
#ifdef EDITOR
	if(World.EditorActive)
		return
#endif

	ExtClass player
	
	//HACK: something very bad happened to our hero
	if(World.Player.status == STATE_DEAD)
		return

	if(disabled)
		return

	SetEventMask(this, EV_FRAME)
	StartScene()
	CallMethods(this, OnStart)
}

//-----------------------------------------------------------------
void EndScene()
{
	InCutscene--
	clamp InCutscene<0, InCutscene>
	IsRunning = false

	World.ShowCursor = true
	World.CutScene = false
	World.CutSceneSkip = false
	SetCameraVerticalFOV(0, DefaultFOV)

	if(FromDirector)
	{
		ShowWidget(DProgressBar,false)
		ShowWidget(DProgressBar2,false)
		ShowWidget(DTime,false)
	}

	World.StopDialogs()
	World.CloseCutSceneDialog()

	ShowWidget(CTopBand,false)
	ShowWidget(CBottomBand,false)
	ShowWidget(CFlash,false)
	FromDirector = false

	for(ExtClass ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		ec.EndCutscene()

	for(int n = 0; n < NumCachedSounds; n++)
	{
		if(CachedSounds[n])
		{
			ReleaseSound(CachedSounds[n])
			CachedSounds[n] = HNULL
		}
	}

	for(n = 0; n < NumCachedPuppets; n++)
	{
		CreatureClass cc = CachedPuppets[n]

		for(int s = 0; s < sizeof(cc.CSounds); s++)
		{		
			if(cc.CSounds[s])
			{
				ReleaseSound(cc.CSounds[s])
			}
		}
	}
	
	if(TempMessage)
	{
		DestroyWidget(TempMessage)
		TempMessage = NULL
	}
}

//-----------------------------------------------------------------
void EOnFrame(MinClass other, int extra)
{
#ifdef EDITOR
	if(World.EditorActive)
		return
#endif

	if(Run() == false)
	{
#ifdef DEVELOPER
		if(GrabVideo)
		{
			//CloseVideoStream()
			GrabVideo = false
		}
#endif
		ClearEventMask(this, EV_FRAME)
		CleanupScene()
		EndScene()
		CallMethods(this, OnEnd)
	}
}

void LoadWidgets()
{
	if(CTopBand == NULL)
	{
		CTopBand = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 85, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 12)
		LoadWidgetImage(CTopBand, 0, "gfx/black.tga")
		CBottomBand = CreateWidget(WTYPE_IMAGE, 0, 515, 800, 85, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 12)
		LoadWidgetImage(CBottomBand, 0, "gfx/black.tga")
	}
	
	LoadFlashWidget(118)
}

void EOnLoad(MinClass other, int extra)
{
	LoadWidgets()

	if(tp > 0)	//ak pred sejvom bezala tak ju pustime znova
	{
		disabled = false
		M_Start(this)
	}
}

//-----------------------------------------------------------------
void EOnLoaded(MinClass other, int extra)
{
	CallMethods(this, OnLoad)
}

void EOnInit(MinClass other, int extra)
{
	ClearEventMask(this, EV_FRAME)
	SetEventMask(this, EV_SOUNDEND)
	SetFlags(this, TFL_NOBILLBOARD)
	timescale = 100
	SetOrigin(this, coords)

	LoadWidgets()

	CameraPos = coords
	if(angles != ZeroVec)
		CameraAngles = angles

}

//-----------------------------------------------------------------
void info_cutscene()
{
	SetEventMask(this, EV_INIT|EV_ONLOAD)
}

//-----------------------------------------------------------------
void ~info_cutscene()
{
	if(IsRunning)
		EndScene()

	if(CTopBand)
		ShowWidget(CTopBand,false)
	
	if(CBottomBand)
		ShowWidget(CBottomBand,false)

	if(CFlash)
		ShowWidget(CFlash,false)
	
	if(DProgressBar)
		ShowWidget(DProgressBar,false)
	if(DProgressBar2)
		ShowWidget(DProgressBar2,false)
	if(DTime)
		ShowWidget(DTime,false)

	if(TempMessage)
	{
		DestroyWidget(TempMessage)
		TempMessage = NULL
	}

	CleanupScene()
}

//-----------------------------------------------------------------
int Length()
{
int length = 0

	tparse t = BeginParse("cutscene/" + name + ".scn")
	if(t)
	{
		int line = 0

		while(true)
		{
			if(ParseLine(t, line, tokens) == 0)
				break

			int l = atoi(tokens[0])
			if(l > length)
				length = l

			line++
		}

		EndParse(t)
	}

	return length
}

}

//-----------------------------------------------------------------
// DIRECTOR
//-----------------------------------------------------------------
#ifdef DEVELOPER

int EScenes[20]
string Scenes[20]
widget WScenes[20]
int NumScenes
int DirectorActive
int DirCursor

void CleanupDirector()
{
	int n = 0

	while(n < NumScenes)
	{
		DestroyWidget(WScenes[n])
		n = n + 1
	}
}

//-----------------------------------------------------------------
void ShowDirector(WorldClass wc)
{
	int n = 0

	while(n < NumScenes)
	{
		ShowWidget(WScenes[n],true)
		n = n + 1
	}

	DirectorActive = true
}

//-----------------------------------------------------------------
void HideDirector(WorldClass wc)
{
	int n = 0

	while(n < NumScenes)
	{
		ShowWidget(WScenes[n],false)
		n = n + 1
	}

	DirectorActive = false
}

//-----------------------------------------------------------------
void ProcessDirector(WorldClass wc)
{
int p
info_cutscene cs

	p = KeyState(KC_ESCAPE)
	p = p | KeyState(KC_F11)

	if(p)
	{
		ClearKey(KC_ESCAPE)
		ClearKey(KC_F11)
		HideDirector(wc)
		return
	}


	if(KeyState(KC_UP))
	{
		ClearKey(KC_UP)

		p = DirCursor

		DirCursor = DirCursor - 1
		if(DirCursor < 0)
			DirCursor = NumScenes - 1

		SetWidgetImage(WScenes[p], 0)
		SetWidgetImage(WScenes[DirCursor], 1)
	}

	if(KeyState(KC_DOWN))
	{
		ClearKey(KC_DOWN)

		p = DirCursor

		DirCursor = DirCursor + 1
		if(DirCursor >= NumScenes)
			DirCursor = 0

		SetWidgetImage(WScenes[p], 0)
		SetWidgetImage(WScenes[DirCursor], 1)
	}

	if(KeyState(KC_E))
	{
		ClearKey(KC_E)

		cs = EScenes[DirCursor]
		Edit("cutscene/" + cs.name + ".scn",0)
	}

	if(KeyState(KC_RETURN))
	{
		ClearKey(KC_RETURN)
		HideDirector(wc)
		cs = EScenes[DirCursor]
		cs.disabled = false

		SLength = cs.Length()
		if(SLength <= 0)
			return

		ShowWidget(DProgressBar,true)
		ShowWidget(DProgressBar2,true)
		ShowWidget(DTime,true)

		FromDirector = true
		GrabVideo = false
		
		if(KeyState(KC_LSHIFT))
		{
			//OpenVideoStream()
			GrabVideo = true
			LastVideoFrame = -1
			//d_fixedtimeslice = 1000.0 / 30
		}
		cs.M_Start(cs)
	}
}

//-----------------------------------------------------------------
void InitDirector(WorldClass wc)
{
int y = 32

	NumScenes = 0

	ctype ct = Type("info_cutscene")
	for(info_cutscene cl = GetNextEntity(NULL, ct); cl != NULL; cl = GetNextEntity(cl, ct))
	{
		Scenes[NumScenes] = cl.name

		widget w = CreateWidget(WTYPE_TEXT, 64, y, 672, 20, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 120)
		SetWidgetColor(w, 0xffffffff)
		LoadWidgetImage(w, 0, "gfx/black.tga")
		LoadWidgetImage(w, 1, "gfx/gray.tga")

		EScenes[NumScenes] = (int)cl
		WScenes[NumScenes] = w
		SetWidgetText(w, 0, cl.name)

		y = y + 24
		NumScenes++
	}

	if(NumScenes == 0)
		return

	DProgressBar = CreateWidget(WTYPE_IMAGE, 0, 580, 800, 20, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 120)
	LoadWidgetImage(DProgressBar, 0, "gfx/black.tga")
	DProgressBar2 = CreateWidget(WTYPE_IMAGE, 0, 580, 800, 20, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 121)
	LoadWidgetImage(DProgressBar2, 0, "gfx/gray.tga")

	DTime = CreateWidget(WTYPE_TEXT, 0, 580, 40, 20, WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 122)
	LoadWidgetImage(DTime, 0, "gfx/black.tga")
	SetWidgetText(DTime, 0, "000")

	DirCursor = 0
	SetWidgetImage(WScenes[DirCursor], 1)
}
#endif











class misc_camera_player extends ExtClass
{
	vector coords
	string anim
	vobject CameraHierarchy, CameraAnim
		
	void M_Start(ExtClass other)
	{
		SetEventMask(this, EV_FRAME)
	}

	void M_Stop(ExtClass other)
	{
		ClearEventMask(this, EV_FRAME)		
	}
	
	void EOnFrame(MinClass other, int extra)
	{
		UpdateAnimCamera(this, 2, ZeroVec)
		World.CameraPos = g_AnimCameraPos
		World.CameraAngs = g_AnimCameraAngs
		World.CameraNorm = Angles2Vector(World.CameraAngs)
		SetCamera(World.CameraPos, World.CameraAngs)
		
		vector mat[4]
		RollPitchYawMatrix(World.CameraAngs, mat);
		mat[3] = World.CameraPos
		SetCameraEx(1, mat)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

	void EOnInit(MinClass other, int extra)
	{
		CameraAnim = GetObject(anim)
		CameraHierarchy = GetObject("obj/camera.xob")
		SelectObject(this, CameraHierarchy)
//		SetEventMask(this, EV_ANIMEND)
		if(CameraAnim)
			SetAnimSlot(this, 0, CameraAnim, 0, 1, NULL, 18, 0)

		 //co s tou svini je???
		SetOrigin(this, "1 1 1")
		SetOrigin(this, ZeroVec/*coords*/)
		BumpFrame(this, 0, 0)
		UpdateEntity(this)
	}
	
	void ~misc_camera_player()
	{
		if(CameraAnim)
			ReleaseObject(CameraAnim, 0)		
	}
}
