//Model constants
const int BBoxNone = -1
const int BBoxNormal = 0
const int BBoxCrouch = 1

const float LookLimit = 85

//-----------------------------------------------------------------
class player_dummyobject extends CreatureClass
{
int AnimWeaponOffset

//-----------------------------------------------------------------
	void UpdateAgent(bool set)
	{
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		BumpFrame(this, ftime, 0)
	}
	
//-----------------------------------------------------------------
	void SelectWeapon(WeaponClass wc)
	{
		if(CurrentWeapon)
		{
			delete CurrentWeapon
			CurrentWeapon = NULL
		}
		
		string wclass = ClassName(wc)

		switch(wclass)
		{
		case "item_weapon_jackhammer":
			CurrentWeapon = new item_weapon_jackhammer(this)
			break

		case "item_weapon_pistol":
			CurrentWeapon = new item_weapon_pistol(this)
			break

		case "item_weapon_machinegun":
			CurrentWeapon = new item_weapon_machinegun(this)
			break

		case "item_weapon_shotgun":
			CurrentWeapon = new item_weapon_shotgun(this)
			break

		case "item_weapon_sniperrifle":
			CurrentWeapon = new item_weapon_sniperrifle(this)
			break

		default:
			CurrentWeapon = new item_weapon_dummy
			break
		}

		switch(wclass)
		{
		case "item_weapon_pistol":
			AnimWeaponOffset = 1
			break
		default:
			AnimWeaponOffset = 0
			break
		}

		CurrentWeapon.Pick(this)
		AddChild(this, CurrentWeapon, p_rhand)
		CurrentWeapon.Select()
		CurrentWeapon.Show(true)
		SetRenderViewMask(CurrentWeapon, 0xfffe)
	}

//-----------------------------------------------------------------
	float adisp
//-----------------------------------------------------------------
	void SetOrientation(vector pos, float yaw)
	{
		SetOrigin(this, pos)
		SetAngles(this, Vector(0, yaw - adisp, 0))
	}
	
//-----------------------------------------------------------------
	void Turn(float dir)
	{
		float inc = ftime * 400
		const float min = -75
		const float max = 75
		
		if(dir < 0)
		{
			adisp -= inc
			clamp adisp<min, 0>
		}
		else
		{
			if(dir > 0)
			{
				adisp += inc
				clamp adisp<0, max>
			}
			else
			{
				if(adisp < 0)
				{
					adisp += inc
					clamp adisp<min, 0>
				}
				else
				{
					adisp -= inc
					clamp adisp<0, max>
				}
			}
		}

		SetBone(this, B_Spine, Vector(0, adisp * -0.33, 0), ZeroVec, 1.0)		
		SetBone(this, B_Spine1, Vector(0, adisp * -0.33, 0), ZeroVec, 1.0)		
		SetBone(this, B_Spine2, Vector(0, adisp * -0.33, 0), ZeroVec, 1.0)		
	}
	
//-----------------------------------------------------------------
	void NoMovement()
	{
		Turn(0)
		PlayAnim(s_walk, a_combat_idle + AnimWeaponOffset, 10.0, g_all, 30, 0)
	}
	
//-----------------------------------------------------------------
	void Transmission()
	{
		Turn(0)
		PlayAnim(s_walk, a_relax_idle, 10.0, g_all, 30, 0)
	}
	
//-----------------------------------------------------------------
	void Movement(bool KeyUp, bool KeyDown, bool KeyLeft, bool KeyRight)
	{
		if(KeyLeft)
		{
			Turn(-1)
			PlayAnim(s_walk, a_combat_walk + AnimWeaponOffset, 10.0, g_legs, 30, 0)
			return
		}

		if(KeyRight)
		{
			Turn(1)
			PlayAnim(s_walk, a_combat_walk + AnimWeaponOffset, 10.0, g_legs, 30, 0)
			return
		}
		
		Turn(0)

		if(KeyUp)
		{
			PlayAnim(s_walk, a_combat_walk + AnimWeaponOffset, 10.0, g_legs, 30, 0)
			return
		}
		
		if(KeyDown)
		{
			PlayAnim(s_walk, a_combat_bwalk + AnimWeaponOffset, 10.0, g_legs, 30, 0)
			return
		}
	}
	
//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		ClearFlags(this, TFL_SOLID|TFL_USER6)
		SetFlags(this, TFL_TRANSLUCENT)
	}

//-----------------------------------------------------------------
	void player_dummyobject()
	{
		//ClassNum = ClassNumPlayer
		MainObject = GetObject("obj/creatures/arnold/arnold.xob")
		SetObject(MainObject)
		SetFlags(this, TFL_VISIBLE)
		SetEventMask(this, EV_FRAME)
		SetRenderViewMask(this, 0xfffe)
		astatus = 1
		
		GetAnimSet("Player")

		PlayAnim(s_walk + 1, a_combat_idle, 10.0, g_all, 30, 0)
	}
}

//-----------------------------------------------------------------
class player_hands extends Actor
{
	vobject HandsObject
	player_interface Owner

//---------------------------------------------------------------------------------------------------------
	void PlayIdleAnim()
	{
		WeaponClass wc = Owner.CurrentWeapon
		if(wc.IdleAnim)
			SetAnimSlot(this, s_walk, wc.IdleAnim, 15, 0, NULL, 30, 0)

		Owner.EnableIdleWarAnims(true)
	}

//---------------------------------------------------------------------------------------------------------
	void PlayWalkAnim()
	{
		WeaponClass wc = Owner.CurrentWeapon
		if(wc.WalkAnim)
			SetAnimSlot(this, s_walk, wc.WalkAnim, 15, 1, NULL, 30, 0)

		Owner.EnableIdleWarAnims(false)
	}

//---------------------------------------------------------------------------------------------------------
	bool PlaySelectAnim()
	{
		WeaponClass wc = Owner.CurrentWeapon
		if(wc.SelectAnim)
		{
			SetAnimSlot(this, s_weapon - 1, wc.SelectAnim, 0, 1, NULL, 30, AF_ONCE)	//s_weapon - 1 aby sa v handleri dalo rozoznat ze ide o select
			return true
		}
		else
			return false
	}

//---------------------------------------------------------------------------------------------------------
	void PlayUnselectAnim()
	{
		WeaponClass wc = Owner.CurrentWeapon
		if(wc.UnselectAnim)
			SetAnimSlot(this, s_weapon - 1, wc.UnselectAnim, 0, 1, NULL, 30, AF_ONCE)	//s_weapon - 1 aby sa v handleri dalo rozoznat ze ide o unselect
	}

//---------------------------------------------------------------------------------------------------------
	void PlayIdleVarAnim()
	{
		WeaponClass wc = Owner.CurrentWeapon
		int NumIdleVarAnims = 0
		for(int n = 0; n < sizeof(wc.IdleVarAnims); n++)
		{
			if(wc.IdleVarAnims[n])
				NumIdleVarAnims++
		}

		if(NumIdleVarAnims <= 0)
			return

		int AnmIndex = rand(0, NumIdleVarAnims)
		SetAnimSlot(this, s_walk - 1, wc.IdleVarAnims[AnmIndex], 15, 15, NULL, 30, AF_ONCE)
	}

//-----------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		SetAnimSlot(this, extra, VNULL, 15.0, 0, NULL, 30, 0)	//vsetko co skonci vyhodime. co neskonci uz vyhodene bolo

		if(extra == s_weapon)
			Throw(Owner.CurrentWeapon, this, EV_ANIMEND, extra + 100)

		int ChangeWeapSlot = s_weapon - 1	//select a unselect nesmie byt preruseny. animacka skonci vzdy
		if(extra == ChangeWeapSlot)			//pri prehazdovani inej zbrane sa vola na poslednom frame animhook. keby sa animacka nevyhodila tak ho vola dookola
		{
			if(Owner.WeapSelectStatus == 1)			//zbran mala unselect animacku a ta teraz skoncila
			{
				Owner.UnselectWeapon()						//vyhodime staru zbran. tu sa prepina WeapSelectStatus na 2
				Owner.SelectWeapon(Owner.ChangeWeapon)	//selectneme novu (ChangeWeapon)
			}
			else	//musi tu byt else pretoze WeapSelectStatus sa medzitym zmeni
			{
				if(Owner.WeapSelectStatus == 2)			//zbran mala select animacku takze az teraz skoncil select novej zbrane
				{
					Owner.WeapSelectStatus = 0
					Owner.ChangeWeapon = NULL	//tymto dame vediet ze cely proces vymeny zbrane skoncil

					if(Owner.status == ST_NONE)	//selektli sme novu zbran takze idle/walk musime nahodit podla novej zbrane!
						PlayIdleAnim()
					else
						PlayWalkAnim()

					Owner.UpdateAmmoBar()

/*					if(Owner.NextWeaponToChange)	//tato zbran cakala na dokoncenie vymeny zbrane tak ju hned nahodime
					{
						Owner.WeaponChange(Owner.NextWeaponToChange, true)

						if(Owner.WeaponChange(Owner.NextWeaponToChange, true) == false)	//moze sa stat ze nema naboje
							Owner.WeaponChange(CJackHammer, true)

						Owner.NextWeaponToChange = 0
					}*/
				}
			}
		}
	}

//-----------------------------------------------------------------
	void ReloadAction()	//animhook na moment vlozenia naboja
	{
		if(IsInherited(Owner.CurrentWeapon, CFireWeaponClass)
		{
			FireWeaponClass fw = Owner.CurrentWeapon
			fw.ReloadAction()
		}
	}

//-----------------------------------------------------------------
	void ShootAction()	//animhook. pouzito u granatu
	{
		WeaponClass wc = Owner.CurrentWeapon
		wc.ShootAction()
	}

//-----------------------------------------------------------------
	void ShootAction2()	//animhook. pouzito u granatu
	{
		WeaponClass wc = Owner.CurrentWeapon
		wc.ShootAction2()
	}

//-----------------------------------------------------------------
	void player_hands(player_interface owner)
	{
		Owner = owner
		HandsObject = GetObject("obj/weapons/hands.xob")
		SelectObject(this, HandsObject)
		SetBoundBox(this, "-16 -16 -16", "16 16 16")
		SetFlags(this, TFL_VISIBLE | TFL_FULLMATRIX)
		SetEventMask(this, EV_ANIMEND)
		SetRenderViewMask(this, 0x0001)
	}

//-----------------------------------------------------------------
	void ~player_hands()
	{
		if(HandsObject)
			ReleaseObject(HandsObject, 0)
	}
}

const float DefaultCrossScale = 0.33333	//odchylka presnosti strelby za predpokladu ze sa nepohybujeme, nehybeme kamerou a nestrielame.

const float HackWindowIdleWidth = 254		//rozmery a umiestnenie hack preview okna
const float HackWindowIdleHeight = 192
const float HackWindowIdleLeft = 23
const float HackWindowIdleTop = 48

const int DEATHMODE_NORMAL = 0
const int DEATHMODE_IN_VEHICLE = 1

class creature_player extends player_interface
{
	dGeom geom					//kolizna geometria
	vector coords				//Start Position
//	float angle					//Z axis camera angle (on start)
	bool Controls				//controls are disabled/enabled
	player_dummyobject DummyPlayer		//dummy objekt pro zrcadla a kamery
	
	vector WeaponAngles		//pomocna premenna na opozdeny pohyb zbrane
	float  HandsOffset		//pri suvislej palbe posuvame ruky so sbranou smerom dozadu
	vector CameraAngles		//angles of camera vector
	vector CameraPos			//camera pos (not eqal with player origin)
	vector CameraTargetPos	//target pos for camera smooting system
	vector VirtualBodyCenter//pomyselny stred body (koli naklananiu)
	float LeanAngle
	int flags					//for custom use
	int DifficultyLevel		//dif level for game
	vector MotionVec			//motion vector. at most time means CameraLook vector but Z axis is 0
	float MotionSpeed			//Current motion speed
	float MotionAccel			//zrychlenie pouzite pre pohyb
	float RelMotionAng		//Z axis rotate angle as result from control device. realtive to Camera Look vector
	float MovementSpeedRate			//jaku mame prave rychlost voci maximalnej ktoru mozeme dosiahnut. percentualne
	bool KeyUp, KeyDown, KeyLeft, KeyRight, AnyMoveKey	//movement keystates
	bool IsOnGround
	bool StrongSurfaceBias
	bool Crouch					//substatus for movements ST_NONE and ST_RUN
	float fCrouch				//1-je v crouchi 0-neni		postupne sa blenduje
	float fCrouchPath
	bool Sprint					//substatus for movement ST_RUN
	bool OxygenAtmosphere
	bool OxygenAdding
	float OxygenDownTimer
	int OxygenRespirationSnd
	int OxygenRespSndIndex
	float OxygenPainCounter	//ked sme v nedychatelnej atmosfere a nemame uz oxygen tak raz za cas playerovi uberieme energiu
	bool HackingDisabled
	int Shooting				//ked player stlaci spust = true. ked zbran prestane strielat(pusti spust, dojde zasobnik) = false
	int ShootingMode			//primary alebo secondary suvislej palby danej cez Shooting
	float CameraKickOffset  //Celkovy offset ktory sme nazbierali kopanim zbrane. o ten sa kamera vracia
	float CameraShotingVertOffset		//pocitame o kolko sa pohla mys vertikalne aby sa to dalo potom odcitat zo spatneho pohybu po strelbe
	float WeaponKickTimer
	MinClass misc
	vector WeaponKickOff			//pridavne uhly pre kameru ktore odpovedaju celkovemu posunu kamery pri suvislej strelbe. po skonceni strelby klesaju na nulu
	vector WeaponKickOffSmall	//pridavne uhly pre kameru ktore sluzia na trhnutie kamerou pri vystrele
	float ExactAimPath, ExactAimPathPrev
	bool PrimaryAttack			//stav klavesy
	bool SecondaryAttack			//stav klavesy
	MinClass FlashLightBase		//nosic pre svelo baterky
	bool FlashLightSwitch
	float FlashLight				//energia baterky
	float FlashLightVisTimer
	int FlashLightHandle			//handle na samotne svetlo baterky
	vector MotionForce			//sila ktorou tlacime playera
	vector BrakeForce				//sila ktorou brzdime playera
	float BrakeScale

	float FootstepsTimer
	int	CurrentFootstep
	bool JumpUsingKey
	MinClass WeaponSoundPos

	Constraint ContactConstraint
	Constraint DampingConstraint1
	bool SimulatePlane
	bool MovementDumping
	vector WorldContactNormal			//zpriemerovana normala zo vsetkych kontaktov okrem kontaktov worldu
	int	 NumWorldContactNormals	//ich pocet
	vector LegsContactNormal			//zpriemerovana normala zo vsetkych kontaktov okrem kontaktov worldu
	float	LegsContactDot
	float SurfaceAngle
	int	 NumLegsContactNormals	//ich pocet
	float ManipIdealDist
	float ManipDist
	float ManipMass
	vector ManipOffset
	int WeaponBeforeManip, WeaponBeforeMiniCutscene
	int ManipInterLayer
	float ManipTime
	float ManipThrowPowerScale
	Constraint		ManipPointConstraint1, ManipPointConstraint2, ManipPointConstraint3
	Constraint		ManipAngConstraint1, ManipAngConstraint2, ManipAngConstraint3
	vector			IdealManipPos
	vector			ManipOffMat[3]
	TraceContact TrcContact
	MinClass ShapeCast
	ExtClass DistActivFavorit

	float cincx, cincy
	float MovementSpeedScale
	int Noises
	float DeathCounter
	int	DeathMode			//normalne zabite/vo vehicli
	vector DeathCameraTarget
	vector LastStablePos
	vector LastStayPos

	ExtClass Hacked			//ked sme do niecoho nahackovani
	ExtClass Controlled		//ked nieco ovladame. ak je nieco v Hacked tak to iste musi byt aj tu
	ExtClass HackAdepts[16]
	int HackAdeptsNum
	bool InsideHackTrigger
	int	 VehicleControlStatus
	float	 VehicleControlTimer
	float	 VehicleQuatExt[4]
	float	 VehicleQuatIn[4]
	vector VehicleCameraBlendOff
	dBlock VehiclePlayerColBlock
	float	 VehicleCameraLean
	bool	 HackMode
	float	 HackModeFade
	int	 HackStatus
	float	 HackLoginTimer
	ExtClass SelectedHackAdept
	ExtClass HackCrossSelected
	trigger_hack_detector FirstHackTrigger
	float HackLastCrossX, HackLastCrossY
	float	HackCrossInterpSpeed
	float HackingStripPos
	float HackCrossVisibility
	float HackRandomTextTimer
	float	HackRandomTextPos
	int	HackLoopSound
	float HackNoiseTimer
	float HackPreviewWindowExtScale
	float LoginTime, MaxLoginTime
	float HackNoiseFade
	float HackWindowLeft, HackWindowTop, HackWindowWidth, HackWindowHeight	//aktualne rozmery hack preview okna
	float HackMessageTimer
	float HackingProgress
	float ReconSignal
	float ReconSignalFlash
	int	HackingLoopSound

	float	WeapGUIScroll				//hlavny scroll
	float	WeapGUIActivityTimer
	int	WeapGUIStatus
	int	WeapGUISelected
	float WeapGUIAddScrolls[10]		//pridavne scroly

	float BulletTimeFadeTarget
	float BulletTime
	int	BulletTimeLoopSound
	int	BulletTimeFadeInSound
	int	BulletTimeFadeOutSound
	float LoopTimeCounter

	int	HitZoneStatus[4]
	float	HitZoneFade[4]
	float HealthPumpWait

	bool	HearthSoundNeedStop
	int	HearthSound
	int	HealthPumpUpLoopSound
	int	PainSound
	int	HealthFlashReason
	float HealthFlashRate

	float oxygen
	float MovementCrossScale
	float CameraCrossScale
	float ExactAimCrossScale
	float ShootingCrossScale
	func_train TrainEnt
	int	MiniCutsceneStatus
	float MiniCutsceneCameraScale, MiniCutsceneCameraScaleDyn
	int	WViewWidgetCamera

#ifdef EDITOR
//	EntityEditor Editor

	void EditorOpened()
	{
		player_hands hands = Hands
		hands.Show(false)
		dBodyActive(this, false, false)

		int n
		for(n = 0; n < InteractionAdeptsNum; n++)	//entity sa v editore reloaduju takze nam tu zostavaju neplatne pointre!
			InteractionAdepts[n]	= NULL

		InteractionAdeptsNum = 0

		for(n = 0; n < HackAdeptsNum; n++)
			HackAdepts[n]	= NULL

		HackAdeptsNum = 0
		Hacked = NULL
		Controlled = NULL
		SelectedHackAdept = NULL
		HackCrossSelected = NULL

		coords = atov(GetKeyValue("coords"))	//coords sa mu v OnFrame meni takze musime siahnut po inicializacnej hodnote

		vector mat[4]
		vector Angles = Vector(0, angle, 0)
		RollPitchYawMatrix2(Angles, mat)
		mat[3] = coords
		SetMatrix(this, mat)
		UpdateEntity(this)
	}

	void OnPositionChangingInEditor(vector Pos, vector Angs)	//volane kazdy frame sa meni pozicia pomocou editoru
	{
		coords = Pos
		vector mat[4]
		RollPitchYawMatrix2(Angs, mat)
		mat[3] = Pos
		SetMatrix(this, mat)
		UpdateEntity(this)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()	//musime updatovat zhora

		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}

	void CreateEditorVisuals()
	{
		if(CategoryHidden || Hidden)
			return

		if(!MainVisShape)
		{
			UpdateEntity(this)
			vector mat[4]
			GetMatrix4(this, mat)
			int ShapeColor = StringToColor(DefPtr.Color)

			float HalfHeight = PlayerCapsuleHalfHeights[0]
			float HalfWidth = PlayerCapsuleRadius
			vector dsmin,dsmax
			dsmin = Vector(-HalfWidth, -HalfWidth, -HalfHeight)
			dsmax = Vector(HalfWidth, HalfWidth, HalfHeight)
			dsmin = dsmin - "-0.04 -0.04 -0.04"
			dsmax = dsmax - "0.04 0.04 0.04"

			MainVisShape = AddDShape(SHAPE_BBOX, ShapeColor, SS_NOZUPDATE|SS_TRANSP, dsmin, dsmax)

			if(MainVisShape)
				MainVisShape.SetMatrix(mat)
		}
	}
#endif

//-----------------------------------------------------------------
	int GetDifficultyLevel()
	{
		return DifficultyLevel
	}
	
//-----------------------------------------------------------------
	void SetStatus(int s)
	{
		status = s
	}

//---------------------------------------------------------------------------------------------------------
	void HealthDown(float dmg)
	{
		Health -= dmg
		clamp Health<0, MaxHealth>
		HealthPumpWait = PlayerHealthPumpWait

		float rate = Health / MaxHealth

		if(rate <= 0.25)	//vypiname tlkot srdca a zapiname zvuk naplnania
		{
			if(!HearthSound)
				HearthSound = PlaySound(this, g_sCommonSounds[9], SF_ONCE)

			HearthSoundNeedStop = false
		}

		HealthFlashReason = 1
		if(HealthPumpUpLoopSound)
		{
			EndSound(HealthPumpUpLoopSound)
			HealthPumpUpLoopSound = 0
		}
	}

//-----------------------------------------------------------------
	void M_SetHealth(ExtClass other)
	{
		float rate = atof(other._value)
		clamp rate<0, 100>
		Health = MaxHealth * rate / 100
		HealthDown(0)
		UpdateHealthBar()
	}

//------------------------------------------------------------
	void SetThrowBar(float PosRate, bool show)
	{
		float BarHeight = 86
		float CurHeight = PosRate * BarHeight

		SetWidgetPos(HUD.WThrowBar, 377, 256 + (BarHeight - CurHeight), 86, CurHeight)
		SetWidgetPos(HUD.WThrowBarBack, 377, 256, 86, 86)

		ShowWidget(HUD.WThrowBar, show)
		ShowWidget(HUD.WThrowBarBack, show)
	}

	void ShowHUD(int DialogWnd, int TaskListWnd, int InfoWnd, int HealthBar, int AmmoBar, int BulletTimeBar, int LightBar, int ThrowBar, int OxygenBar, int ReconSignal)
	{
		if(DialogWnd >= 0)
		{
			if(World.Transmitter)
				World.Transmitter.Show(DialogWnd)
		}
	
		if(TaskListWnd >= 0)
		{
			if(World.TaskList)
				World.TaskList.Show(TaskListWnd)
		}
	
		if(InfoWnd >= 0)
		{
			if(World.InfDialog)
				World.InfDialog.Show(InfoWnd)
		}
	
		if(HealthBar >= 0)
		{
			ShowWidget(HUD.WHealthBar, HealthBar)
			ShowWidget(HUD.WHealthBarText, HealthBar)
			ShowWidget(HUD.WHealthBarBack, HealthBar)
			ShowWidget(HUD.WHealthBarFlash, HealthBar)
		}
	
		if(AmmoBar >= 0)
		{
			if(CurrentWeapon.ClassNum == CJackHammer || CurrentWeapon.ClassNum == CDummyWeapon)
				AmmoBar = false

			ShowWidget(HUD.WAmmoBar, AmmoBar)
			ShowWidget(HUD.WAmmoBarBack, AmmoBar)
			ShowWidget(HUD.WAmmoBarText, AmmoBar)
		}
	
		if(BulletTimeBar >= 0)
		{
			ShowWidget(HUD.WTimeBar, BulletTimeBar)
		}
	
		if(LightBar >= 0)
		{
			ShowWidget(HUD.WLightBar, LightBar)
			ShowWidget(HUD.WLightBarBack, LightBar)
		}
		
		if(ThrowBar >= 0)
		{
			ShowWidget(HUD.WThrowBar, ThrowBar)
			ShowWidget(HUD.WThrowBarBack, ThrowBar)
		}

		if(OxygenBar >= 0)
		{
			ShowWidget(HUD.WOxygenBar, OxygenBar)
			ShowWidget(HUD.WOxygenBarBack, OxygenBar)
			ShowWidget(HUD.WOxygenBarText, OxygenBar)
		}
		
		if(ReconSignal >= 0)
		{
			ShowWidget(HUD.WHackingSignal, ReconSignal)
			ShowWidget(HUD.WHackingSignalBack, ReconSignal)
			ShowWidget(HUD.WHackingSignalFlash, ReconSignal)
		}
	}

//------------------------------------------------------------------------------------------------
	void ShowHackingPreview(ExtClass ent)
	{
		string TitleText = ""

		if(ent && ent.HackInterf && ent.HackInterf.SetHackPreviewCamera())
		{
			int prevworld = SetCurrentMap(NULL)	//aby to fungovalo aj v editore
			SetCurrentMap(prevworld)
			SetWidgetWorld(HUD.WHackingPreviewView, prevworld, 1)
			ShowWidget(HUD.WHackingPreviewView, true)
			TitleText = ent.HackInterf.name
		}
		else
		{
			ShowWidget(HUD.WHackingPreviewView, false)
			
			if(ent)
			{
				if(ent.HackInterf)
					TitleText = ent.HackInterf.name
			}
		}
		
		if(TitleText)
		{
			TitleText = "#" + TitleText
			SetWidgetTextEx(HUD.WHackingPreviewTitle, 0, 1, TitleText)
		}
		else
			SetWidgetText(HUD.WHackingPreviewTitle, 0, "")
	}

//------------------------------------------------------------------------------------------------
	void ShowHackGUI(bool stat)	//iba hruba visbilita (renderujeme alebo nie)
	{
		int n
		ExtClass adp

		for(n = 0; n < 4; n++)
			ShowWidget(HUD.WHackingModeCross[n], stat)

		ShowWidget(HUD.WHackingModeGrid, stat)
		ShowWidget(HUD.WHackingModeGrid2, stat)
		ShowWidget(HUD.WHackingStrip, stat)
//		ShowWidget(WHackingRandomText, stat)
//		ShowWidget(WHackingRandomTextBack, stat)

		if(World.HackInfDialog)
			World.HackInfDialog.Show(stat)

		for(n = 0; n < HackAdeptsNum; n++)
		{
			adp = HackAdepts[n]
			
			if(adp.HackInterf)
				adp.HackInterf.Show(stat)
		}

		if(stat)
		{
			if(SelectedHackAdept)		//ak mame nieco selektnute tak renderujeme pohlad z kamery, ak nie tak iba zrnime (iny widget)
				ShowHackingPreview(SelectedHackAdept)
			else
				ShowHackingPreview(NULL)
				
//			ShowWidget(HUD.WHackingPreviewNoise, true)
		}
		else
		{
			ShowWidget(HUD.WHackingPreviewView, false)
		}

		ShowWidget(HUD.WHackingPreviewFrame, stat)
		ShowWidget(HUD.WHackingPreviewNoise, stat)
		ShowWidget(HUD.WHackingPreviewTitle, stat)
	}

//------------------------------------------------------------------------------------------------
	void RemoveHackAdept(ExtClass ent)
	{
		if(Hacked && Hacked == ent)	//napriklad vybuchla mina v ktorej sme boli nahackovani
			InterruptHacking(false)

		for(int n = 0; n < HackAdeptsNum; n++)
		{
			if(HackAdepts[n] == ent)
			{
				int Last = HackAdeptsNum - 1
				if(n < Last)
				{
					for(int k = n; k < Last; k++)
						HackAdepts[k] = HackAdepts[k + 1]
				}
				
				if(SelectedHackAdept == ent)
				{
					SelectedHackAdept = NULL
					HackCrossSelected = NULL
					ShowHackingPreview(NULL)
				}

				HackAdeptsNum--
				return
			}
		}
	}

//------------------------------------------------------------------------------------------------
	void SetHackAdepts(ExtClass adepts[], int NumAdepts)	//vola trigger
	{
		int n
		ExtClass adp

		if(NumAdepts == 0)	//odisli sme z triggeru
		{
			InsideHackTrigger = false		//selectovat/hackovat uz nebude mozne ale vizualizery musime odfadovat a preto si pointre HackAdepts nechame nadalej

			for(n = 0; n < HackAdeptsNum; n++)
			{
				adp = HackAdepts[n]
				adp.HackInterf.FadeOut()
			}

			SelectedHackAdept = NULL
			ShowWidget(HUD.WHackingPreviewView, false)
//			ShowWidget(HUD.WHackingPreviewNoise, false)
			World.HackInfDialog.ShowMessage("// #Connection_lost...")
			return
		}

		for(n = 0; n < NumAdepts; n++)	//mame novych adeptov na hackovanie
		{
			adp = adepts[n]
			HackAdepts[n] = adp

			if(HackModeFade > 0)
				adp.HackInterf.FadeIn()
		}

		HackAdeptsNum = NumAdepts
		InsideHackTrigger = true

		if(HackModeFade != 0)
		{
			ShowHackGUI(true)
			World.HackInfDialog.ShowMessage("// " + itoa(NumAdepts) + " " + "#devices_found...")
		}

		PlaySound(this, g_sCommonSounds[34], SF_ONCE)
	}

//------------------------------------------------------------------------------------------------
	void ProcessHackSelecting()	//zistujeme ktora entita bude selektnuta
	{
		ExtClass adp
		bool CanLog
		ExtClass HAdepts[16]
		int HAdeptsNum = 0
		int n

		for(n = 0; n < HackAdeptsNum; n++)
		{
			adp = HackAdepts[n]
			CanLog = adp.HackInterf.CanHacking()

			if(CanLog)
				HAdepts[HAdeptsNum++] = adp	//spravime zoznam tych do ktroych je mozne sa vobec lognut
			else
			{
				if(adp.HackInterf.IsVisible())
					adp.HackInterf.FadeOut()
			}
		}

		vector AdeptPos, Dir
		float BestDot = -1
		float dot
		ExtClass BestAdept = NULL

		for(n = 0; n < HAdeptsNum; n++)		//najdeme vhodneho adepta na select
		{
			adp = HAdepts[n]
			AdeptPos = adp.HackInterf.GetVisualPos()
			Dir = AdeptPos - World.CameraPos
			VectorNormalize(Dir)
			dot = Dir * World.CameraNorm

			if(dot > 0.99 && dot > BestDot)
			{
				BestDot = dot
				BestAdept = adp
			}
		}

		if(BestAdept)	//mame adepta na select
		{
			if(SelectedHackAdept)	//nieco uz je selektnute
			{
				if(SelectedHackAdept != BestAdept)
				{
					SelectedHackAdept.HackInterf.Select(false)	//stary unselektneme
					BestAdept.HackInterf.Select(true)				//novy selektneme
					HackCrossInterpSpeed = 0
					HackingProgress = 0
					PlaySound(this, g_sCommonSounds[38], SF_ONCE)

					if(HackingLoopSound)	
					{
						EndSound(HackingLoopSound)
						HackingLoopSound = 0
					}

					World.HackInfDialog.ShowMessage("// #Changing_ID_codes [#" + BestAdept.HackInterf.name + "]")
				}
			}
			else					//nebolo selektnute nic
			{
				BestAdept.HackInterf.Select(true)
				HackCrossInterpSpeed = 0
				HackingProgress = 0
				PlaySound(this, g_sCommonSounds[38], SF_ONCE)
				
				World.HackInfDialog.ShowMessage("// #Generating_ID_codes [#" + BestAdept.HackInterf.name + "]")
			}

			SelectedHackAdept = BestAdept
			HackCrossSelected = BestAdept	//cross musi mat vzdy target
			ShowHackingPreview(SelectedHackAdept)
			
			if(SelectedHackAdept)
			{
				float CurHackingTime = SelectedHackAdept.HackInterf.CurHackingTime
				float FullHackingTime = SelectedHackAdept.HackInterf.HackingTime
				CurHackingTime += ftime * 3.03 * sqrt(1 / g_timescale)
				clamp CurHackingTime<0, FullHackingTime>
				SelectedHackAdept.HackInterf.CurHackingTime = CurHackingTime
				
				HackMessageTimer += ftime
				
				if(HackMessageTimer >= 0.2)
				{
					HackMessageTimer = 0
					float HackingProgressBefore = HackingProgress
					HackingProgress = CurHackingTime / FullHackingTime
					float FinHackingProgress = HackingProgress * 100
					
					if(HackingProgressBefore < 1)
					{
						World.HackInfDialog.ShowMessage("#decoding " + itoa(FinHackingProgress) + " % #complete")
						
						if(SelectedHackAdept.HackInterf.CurHackingTime < SelectedHackAdept.HackInterf.HackingTime)
						{
							if(!HackingLoopSound)
								HackingLoopSound = PlaySound(this, g_sCommonSounds[54], SF_AMBIENT)
						}
					}
					else
					{
						if(HackingLoopSound)	
						{
							EndSound(HackingLoopSound)
							HackingLoopSound = 0
							PlaySound(this, g_sCommonSounds[55], SF_ONCE|SF_AMBIENT)
						}
					}
				}
			}
		}
		else						//neni uz nic selektnute
		{
			if(SelectedHackAdept)	//ale nieco bolo takze volame unselect
			{
				SelectedHackAdept.HackInterf.Select(false)
				World.HackInfDialog.ShowMessage("// #Flushing_ID_code_cache...")
			}

			SelectedHackAdept = NULL
			ShowHackingPreview(NULL)
			
			if(HackingLoopSound)	
			{
				EndSound(HackingLoopSound)
				HackingLoopSound = 0
			}
		}
	}

//------------------------------------------------------------------------------------------------
	void UpdateHackGUI()
	{
		if(Hacked)
		{
//			SetPostEffect(0, 0, 0, 0, 0)
			World.HackInfDialog.SetGlobalTransp(0)
		}
		else
		{
//			SetPostEffect(3, 255, 0, HackModeFade, 0.0)
			World.HackInfDialog.SetGlobalTransp(HackModeFade * (1 - HackPreviewWindowExtScale))
		}

		int n
		ExtClass adp
		float GridOp = HackModeFade * (float)255

		SetWidgetColor(HUD.WHackingModeGrid, AWHITE(GridOp))

		if(SelectedHackAdept && InsideHackTrigger)	//zameravac presuvame vzdy na selektnutu entitu
		{
			HackCrossVisibility = LinearBlend(HackCrossVisibility, 1, 8)	//cross chceme viditelny
		}
		else
		{
			HackCrossVisibility = LinearBlend(HackCrossVisibility, 0, 2)	//skryvame cross
		}

		if(HackCrossSelected && HackCrossSelected.HackInterf)
		{
			float scalex = 800 / g_iWidth
			float scaley = 600 / g_iHeight
			float VirtX = HackCrossSelected.HackInterf.Xpos * scalex
			float VirtY = HackCrossSelected.HackInterf.Ypos * scaley
			float CrossOff = 70 * HackCrossSelected.HackInterf.DistanceScale
			HackCrossInterpSpeed += ftime * 1.5
			clamp HackCrossInterpSpeed<0, 1>
	/*					vector vec1 = Vector(VirtX, VirtY, 0)
			vector vec2 = Vector(HackLastCrossX, HackLastCrossY, 0)
			float PosDiff = VectorLength(vec1 - vec2)
	*/
			float MaxX = VirtX - HackLastCrossX
			float MaxY = VirtY - HackLastCrossY
			float StepX = MaxX * HackCrossInterpSpeed
			float StepY = MaxY * HackCrossInterpSpeed
//			if(StepX > )

			HackLastCrossX += StepX
			HackLastCrossY += StepY

			SetWidgetPos(HUD.WHackingModeCross[0], HackLastCrossX - 1024 - CrossOff, HackLastCrossY - 16, 1024, 32)
			SetWidgetPos(HUD.WHackingModeCross[1], HackLastCrossX + CrossOff, HackLastCrossY - 16, 1024, 32)
			SetWidgetPos(HUD.WHackingModeCross[2], HackLastCrossX - 16, HackLastCrossY - 1024 - CrossOff, 32, 1024)
			SetWidgetPos(HUD.WHackingModeCross[3], HackLastCrossX - 16, HackLastCrossY + CrossOff , 32, 1024)
		}

		float CrossOp = HackModeFade * HackCrossVisibility * (float)255

		for(n = 0; n < 4; n++)
			SetWidgetColor(HUD.WHackingModeCross[n], AWHITE(CrossOp))

		HackingStripPos += ftime * 900	//s radar pruhom scrolujeme stale dookola

		if(HackingStripPos > 1556)  // 800+256+delay(500)
			HackingStripPos = -256

		float StripOp = HackModeFade * (float)255
		float RadomTextOp = StripOp * 0.1

		SetWidgetPos(HUD.WHackingStrip, HackingStripPos, 0, 256, 600 )
		SetWidgetColor(HUD.WHackingStrip, AWHITE(StripOp))

/*		HackRandomTextTimer += ftime

		if(HackRandomTextTimer >= 0.25)
		{
			HackRandomTextTimer = 0
			HackRandomTextPos -= 18.7

			if(HackRandomTextPos < -684)
				HackRandomTextPos = 0

		}*/

		for(n = 0; n < HackAdeptsNum; n++)					//updatujeme hack interfaces entit
		{
			adp = HackAdepts[n]
			adp.HackInterf.SetVisibility(HackModeFade)
			adp.HackInterf.UpdateVisualizers(coords)				//volame po celu dobu hacking modu
		}
		
		HackNoiseTimer -= ftime * 50

		if(HackNoiseTimer <= 0)
		{
			HackNoiseTimer = 1
			float UV[8] = {0,0, 0,3, 3,3, 3,0}
			
			float UVoffsetX = frand(0, 1)
			float UVoffsetY = frand(0, 1)
			float UVScale = 1 + (HackWindowWidth / HackWindowIdleWidth)	//UVcko nescalujeme s widgetom

			for(int j = 0; j < 8; j += 2)
			{
				UV[j] = UV[j] * UVScale + UVoffsetX
				UV[j + 1] = UV[j + 1] * UVScale + UVoffsetY
			}
			SetWidgetUV(HUD.WHackingPreviewNoise, UV)
		}

		float alpha1 = HackModeFade * HackNoiseFade * (float)192 + 63	//transp 63 je vzdy
		float alpha2 = HackPreviewWindowExtScale * (float)255
		SetWidgetColor(HUD.WHackingModeGrid2, ARGB(alpha2, 255, 255, 255))
		SetWidgetColor(HUD.WHackingPreviewNoise, ARGB(alpha1, 255, 255, 255))	//ARGB(64, 136, 241, 254)
	}
 		
 //------------------------------------------------------------------------------------------------
 	void SetHackPreviewExtendsSize(float ExtSize)
 	{
 		clamp ExtSize <0, 1>
 		HackPreviewWindowExtScale = ExtSize

		HackWindowLeft = -HackWindowIdleLeft * ExtSize
		HackWindowTop = -HackWindowIdleTop * ExtSize
		float w = 800 - HackWindowIdleWidth
		float h = 600 - HackWindowIdleHeight
		HackWindowWidth = w * ExtSize
		HackWindowHeight = h * ExtSize
		SetWidgetPos(HUD.WHackingPreviewView, HackWindowIdleLeft + HackWindowLeft, HackWindowIdleTop + HackWindowTop, HackWindowIdleWidth + HackWindowWidth, HackWindowIdleHeight + HackWindowHeight)
		SetWidgetPos(HUD.WHackingPreviewNoise, HackWindowIdleLeft + HackWindowLeft, HackWindowIdleTop + HackWindowTop, HackWindowIdleWidth + HackWindowWidth, HackWindowIdleHeight + HackWindowHeight)
		SetWidgetPos(HUD.WHackingModeGrid2, HackWindowIdleLeft + HackWindowLeft, HackWindowIdleTop + HackWindowTop, HackWindowIdleWidth + HackWindowWidth, HackWindowIdleHeight + HackWindowHeight)

		float WidthExt = 32 * ExtSize
		float HeightExt = 32 * ExtSize
		float LeftExt = WidthExt * 0.5
		float TopExt = HeightExt * 0.5

		SetWidgetPos(HUD.WHackingPreviewFrame, HackWindowIdleLeft + HackWindowLeft - 13 - LeftExt, HackWindowIdleTop + HackWindowTop - 12 - TopExt, HackWindowIdleWidth + HackWindowWidth + 26 + WidthExt, HackWindowIdleHeight + HackWindowHeight + 30 + HeightExt)
//		10, 17, 280, 220
 	}

//------------------------------------------------------------------------------------------------
	void OnHackingModeStart()	//bol zapnuty hack mod
	{
		ShowHackGUI(true)
		ShowWidget(HUD.WHackingPreviewNoise, true)
		ProcessHackSelecting()	//okamzite nam nieco selektne
		HackingStripPos = 600
		HackStatus = 0
		SetHackPreviewExtendsSize(0)

		if(InsideHackTrigger)
		{
			ExtClass adp
			for(int n = 0; n < HackAdeptsNum; n++)	//az teraz ked sa uplne skoncil aj update GUI mozeme vynulovat pointre adeptov
			{
				adp = HackAdepts[n]
				adp.HackInterf.FadeIn()
			}
		}

		if(!HackLoopSound)
			HackLoopSound = PlaySound(this, g_sCommonSounds[37], 0)

		PlaySound(this, g_sCommonSounds[35], SF_ONCE)
		World.HackInfDialog.ShowMessage("// #Searching_for_connections...")
	}

//------------------------------------------------------------------------------------------------
	void OnHackingModeEnd()		//cely proces skoncil. Ovladanie bolo vypnute uz skor ale teraz uz skoncil aj update GUI
	{
//		SetPostEffect(0, 0, 0, 0, 0)
		ShowHackGUI(false)

		HackCrossVisibility = 0						//pri najblizsom zapnuti hackingu musi byt vsetko opacitou na nule
		for(int h = 0; h < HackAdeptsNum; h++)	//inac je tam neprijemny preblik resp. neziaduci fade
		{
			ExtClass adp = HackAdepts[h]
			
			if(adp)
			{
				adp.HackInterf.FadeScale = 0
				adp.HackInterf.FadeScaleTarget = 0
			}
		}

		if(!InsideHackTrigger)
		{
			for(int n = 0; n < HackAdeptsNum; n++)	//az teraz ked sa uplne skoncil aj update GUI mozeme vynulovat pointre adeptov
				HackAdepts[n] = NULL

			HackAdeptsNum = 0
			SelectedHackAdept = NULL
		}

		if(HackLoopSound)
		{
			EndSound(HackLoopSound)
			HackLoopSound = 0
		}
		
		if(HackingLoopSound)	
		{
			EndSound(HackingLoopSound)
			HackingLoopSound = 0
		}
		ShowHUD(-1, -1, -1, 1, 1, 1, 1, -1, -1, 1)
	}

//-----------------------------------------------------------------
	void SwitchBulletTimeMode();

//------------------------------------------------------------------------------------------------
	void SwitchHackMode()
	{
		HackMode = 1 - HackMode

		if(HackMode)
		{
			OnHackingModeStart()
			
		}
		else
		{
			PlaySound(this, g_sCommonSounds[36], SF_ONCE)
			World.HackInfDialog.ShowMessage("// #Disable_connections...")
		}
	}

//------------------------------------------------------------------------------------------------
	void InterruptHacking(bool HackModeOff)
	{
		if(!Hacked)
			return
 
		Hacked.HackInterf.LogOut()
		SetListenerCamera(0)
		WViewWidgetCamera = 0
		SetWidgetWorld(WView, globalmap, 0)
		DummyPlayer.CurrentWeapon.Show(true)
		World.HackInfDialog.ShowMessage("// #Logout [#" + Hacked.HackInterf.name + "]")
		Hacked = NULL
		Controlled = NULL
		SetStatus(ST_NONE)
		ShowHackGUI(true)
		MovementDumping = false

		if(HackModeOff)
		{
			HackStatus = 0
			SetHackPreviewExtendsSize(0)
			SwitchHackMode()
		}
		else
			HackStatus = 3	//odlogujeme sa
	}

//---------------------------------------------------------------------------------------------------------
	void HideWeaponSelectGUI()
	{
		WeapGUIStatus = 0
		WeapGUIScroll = 0
		WeapGUIActivityTimer = 0

		for(int n = 0; n < MaxAvailableWeapons; n++)
			ShowWidget(HUD.WeaponList[n], false)
	}

//-----------------------------------------------------------------
	void BeginCutscene()
	{
		if(World.ManipEntity)
			DropManipEntity(false)

		ClearFlags(Hands, TFL_VISIBLE)
		CurrentWeapon.Show(false)
		HideWeaponSelectGUI()

		if(HackMode)						//vypiname recon ked je zapnuty
			SwitchHackMode() 

		KEY(MOVE_F,true)
		KEY(MOVE_B,true)
		KEY(LEFT,true)
		KEY(RIGHT,true)
		KEY(JUMP,true)
		KEY(CROUCH,true)
		SetStatus(ST_NONE)
		dBodyActive(this, false, false)
		
		if(HearthSound)
			HearthSoundNeedStop = true
	}

//-----------------------------------------------------------------
	void EndCutscene()
	{
		if(CurrentWeapon.ClassNum != CDummyWeapon)
		{
			SetFlags(Hands, TFL_VISIBLE)
			CurrentWeapon.Show(true)
		}
		dBodyActive(this, true, true)
	}

//-----------------------------------------------------------------
	void OnEnterVehicle(int stat)	//vola sa 2x
	{
		if(stat == 0)	//zacina nastpovat do auta
		{
			if(HackMode)						//vypiname recon ked je zapnuty
				SwitchHackMode()
		}
		else	//nastupovanie do auta skoncilo. ovladanie prebera auto
		{
			WeaponClass wc = GetWeaponPointer(WeaponBeforeManip)
			if(wc)	//musi tam byt
			{
				ClearFlags(Hands, TFL_VISIBLE)
				wc.Show(false)
			}
	
			FlashLightSwitch = false				//vypneme baterku
			FlashLight = FlashLightMaxTime
			ShowWidget(HUD.WLightBar, false)
			ShowWidget(HUD.WLightBarBack, false) 
	
			if(FlashLightHandle)
			{
				RemoveLight(FlashLightHandle)
				FlashLightHandle = 0
			}
		}
	}

//-----------------------------------------------------------------
	void OnLeaveVehicle(int stat)	//vola sa 2x
	{
		if(stat == 0)	//prva faza. zacina vystupovat z auta. player nemoze robit nic po dobu nez uplne vystupi z auta
		{
			stat = stat
		}
		else	//druha faza. player vystupil z auta a ocita sa v stave kedy je normalne ovladatelny. zacina animacia selektu zbrane WeaponBeforeManip
		{
			WeaponClass wc = GetWeaponPointer(WeaponBeforeManip)
			if(wc)	//musi tam byt
			{
				SetFlags(Hands, TFL_VISIBLE)
				wc.Show(true)
			}
		}
	}

//---------------------------------------------------------------------------------------------------------
	vector GetCenterOfBody()
	{
		if(status == ST_NOCONTROL)
			return coords

		return VirtualBodyCenter	//pomyselny stred akoby nakloneneho body
	}

//---------------------------------------------------------------------------------------------------------
	vector EyesPos()
	{
		return CameraPos

		if(Crouch)
			return GetMatrix(this, 3)
		else
			return GetMatrix(this, 3) + "0 0 22"
	}

	void SetFullOrientation(vector pos, vector Angles)
	{
		vector CameraOffset = Vector(0, 0, PlayerCapsuleExtends[Crouch] + PlayerCameraOffset)
		CameraPos = pos + CameraOffset
		CameraAngles = Angles
		WeaponAngles = Angles

		World.CameraPos = CameraPos
		World.CameraAngs = Angles//CameraAngles
		World.CameraNorm = Angles2Vector(World.CameraAngs)

		SetCamera(CameraPos, CameraAngles)
		SetOrigin(this, pos)
		UpdateEntity(this)
	}

	//-----------------------------------------------------------------
	void M_SetOrientation(ExtClass other)
	{
		ExtClass targ = FindClass(other._value)
	
		if(targ == NULL)
		{
			Print(String(_name + ".SetOrientation() can't found target" + other._value))
			return
		}
	
		vector org = GetOrigin(targ) + HHVec + "0 0 0.5"
	
		SetFullOrientation(org, GetAngles(targ))
	}
//-----------------------------------------------------------------
	void SetOrientationFromCamera()
	{
		CameraTargetPos = CameraPos
		vector CameraOffset = Vector(0, 0, PlayerCapsuleExtends[Crouch] + PlayerCameraOffset)
		coords = CameraPos - CameraOffset
//		SetOrigin(this, coords)

		vector mat[4] = {"1 0 0", "0 1 0", "0 0 1"}
		mat[3] = coords
		SetMatrix(this, mat)
		UpdateEntity(this)

/*		if(KeyState(KC_B))
		{
			ClearKey(KC_B)
			float HalfHeight = PlayerCapsuleHalfHeights[0]		
			vector mins = Vector(-PlayerCapsuleRadius, -PlayerCapsuleRadius, -HalfHeight)
			vector maxs = Vector(PlayerCapsuleRadius, PlayerCapsuleRadius, HalfHeight)
			AddDShape(SHAPE_BBOX, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords + mins, coords + maxs)
		}*/
	}

//---------------------------------------------------------------------------------------------------------
	WeaponClass GetWeaponPointer(int WeaponIndex)	//budu sa lepsie debugovat podivne veci
	{
		if(WeaponIndex < 0 || WeaponIndex >= sizeof(PWeapons))
			return NULL

		return PWeapons[WeaponIndex]
	}

//---------------------------------------------------------------------------------------------------------
	bool SetMyWeaponPointer(int WeaponIndex, WeaponClass ptr)	//budu sa lepsie debugovat podivne veci
	{
		if(WeaponIndex < 0 || WeaponIndex >= sizeof(PWeapons))
			return false

		PWeapons[WeaponIndex] = ptr
		return true
	}

//---------------------------------------------------------------------------------------------------------
	bool PickWeapon(ItemClass item)	//zobrali sme zbran. volane zo samotnej zbrame
	{
		if(GetWeaponPointer(item.ClassNum) != NULL)	//takuto zbran uz mame. v zbrani sa pricitali naboje ale o chvilu bude instancia item zmazana!!!
			return false

		SetMyWeaponPointer(item.ClassNum, item)	//ulozi pointer do PWeapons
		WeaponClass wc
		RealWeaponsNum = 0

		for(int n = 0; n < sizeof(PWeapons); n++)	//spocitame si pocet skutocnych zbrani z herneho hladiska
		{
			wc = PWeapons[n]

			if(wc)
			{
				if(wc.ClassNum == CDummyWeapon)	//tieto zbrane nechceme aby boli dostupne na vyber
					continue

//				SetWidgetImage(WeaponList[wc.ClassNum - 1], 1)	//ikona
				RealWeaponsNum++
			}
		}
		
		if(!ChangeWeapon)	//takuto zbran este nemame a mame iba kladivo takze rovno do ruk
		{
			if(CurrentWeapon.ClassNum == CDummyWeapon || CurrentWeapon.ClassNum == CJackHammer)
			{
				if(item.ClassNum != CDummyWeapon && item.ClassNum != CJackHammer)
					WeaponChange(item.ClassNum, true)
			}
		}

		return true
	}

//-----------------------------------------------------------------
	void UpdateBulletTimeBar()
	{
		float rate = BulletTime / BulletTimeMax
		float BarWidth = rate * 200
		SetWidgetPos(HUD.WTimeBar, 17, 565, BarWidth, 32)

		if(rate <= 0.25)
			SetWidgetImage(HUD.WTimeBar, 1)
		else
			SetWidgetImage(HUD.WTimeBar, 0)


	}

//---------------------------------------------------------------------------------------------------------
	void UpdateHackSignalBar()
	{
		float SignalWidth = ReconSignal * 32
		float ff = SignalWidth / 7
		int ii = ff
		ff = ii - 1
		SignalWidth = ff * 7
		SetWidgetPos(HUD.WHackingSignal, 756, 17, SignalWidth, 32)		
	}

//---------------------------------------------------------------------------------------------------------
	void AddHealth(float quantity)
	{
		Health += quantity
		clamp Health<0, MaxHealth>
		UpdateHealthBar()
	}

//---------------------------------------------------------------------------------------------------------
	void PickAmmo(int type, int quantity)
	{
		if(type == -1)	//vynimka na hubbardium
		{
			BulletTime += BulletTimeMax / 5
			clamp BulletTime<0, BulletTimeMax>
			UpdateBulletTimeBar()
			return
		}

		Ammo[type] = Ammo[type] + quantity
		UpdateAmmoBar()
	}

//---------------------------------------------------------------------------------------------------------
	
		


//---------------------------------------------------------------------------------------------------------
	bool SelectWeapon(WeaponClass wc)	//vrazi mu do ruky zbran. zbrane menime iba cez WeaponChange()!
	{
		DummyPlayer.SelectWeapon(wc)

		CurrentWeapon = wc
		AddChild(Hands, CurrentWeapon, p_rhand)
		CurrentWeapon.Select()
		CurrentWeapon.Show(true)
		player_hands hands = Hands

		if(hands.PlaySelectAnim() == false)	//ak sa nepustila animacka selektu novej zbrane (ziadnu nema)
		{
			WeapSelectStatus = 0					//koncime selektovanie ihned!
			ChangeWeapon = NULL					//proces vymeny zbrane skoncil

			if(status == ST_NONE)	//selektli sme novu zbran takze idle/walk musime nahodit podla novej zbrane!
				hands.PlayIdleAnim()
			else
				hands.PlayWalkAnim()

/*			if(NextWeaponToChange)	//tato zbran cakala na dokoncenie vymeny zbrane tak ju hned nahodime
			{
				if(WeaponChange(NextWeaponToChange, true) == false)	//moze sa stat ze nema naboje
					WeaponChange(CJackHammer, true)

				NextWeaponToChange = 0
			}*/
		}
		else
		{
			CurrentWeapon.BeginSelect()

//			if(wc.WeapSelectAnim)
//				SetAnimSlot(wc, 1, wc.WeapSelectAnim, 8, 0, NULL, 30, 0)
		}

		if(IsInherited(wc, CFireWeaponClass))
		{
			FireWeaponClass fw = wc
			World.Cross.SetMinAndMaxSize(fw.PrecisionFlawMin * 1000, fw.PrecisionFlawMax * 1000)

			if(wc.ClassNum != CDummyWeapon)
				World.Cross.Show(true)
		}
		else
			World.Cross.Show(false)

		if(wc && wc.ClassNum == CDummyWeapon)	//FIXME skor radsej spravit dummy animaciu aby ruky nebolo vidiet
			ClearFlags(Hands, TFL_VISIBLE)
		else
			SetFlags(Hands, TFL_VISIBLE)

		ExactAimPathPrev = -1

		return true
	}

//---------------------------------------------------------------------------------------------------------
	bool UnselectWeapon()	//zbrane menime iba cez WeaponChange()! volane vzdy ked uz je zbran vizualne unselectnuta
	{
		RemoveChild(this, CurrentWeapon)
		CurrentWeapon.Unselect()
		CurrentWeapon.Show(false)
		WeapSelectStatus = 2	//zbran je unselectnuta, zacina select novej zbrane
		return true
	}

//---------------------------------------------------------------------------------------------------------
	bool WeaponChange(int WeaponIndex, bool RequestToWeapon)	//tymto menime zbran
	{
		if(!ChangeWeapon && WeaponIndex == CurrentWeapon.ClassNum && WeaponIndex != CDummyWeapon)
			return false

//		if(CheckItem(WeaponIndex) == false)
//			return false

//		if(CurrentWeapon.Action != 0)
//			return false

/*		if(ChangeWeapon)	//ak uz prebieha vymena tak to nedovolime
		{
			Print("aaaaa")
			return false
		}*/

		player_hands hands = Hands
		ChangeWeapon = GetWeaponPointer(WeaponIndex)

		if(ChangeWeapon == NULL)
			return false

		if(RequestToWeapon)	//na miestach kde sa zbran selektuje cez M_SelectWeapon() to nesmie zlyhat
		{
			if(ChangeWeapon.CanSelect() == false)
			{
				ChangeWeapon = NULL
				return false
			}
		}

		if(CurrentWeapon.Action)		//ak bezi reload, fire atd. tak to musime zrusit pretoze vymena zbrane je dolezitejsia
			CurrentWeapon.InterruptAction()

		EnableIdleWarAnims(false)	//nemozeme prehravat IdleVary pocas vymeny zbrane

		if(CurrentWeapon.Action != 0)
			CurrentWeapon.InterruptAction()

		WeapSelectStatus = 1		//zaciname unselect

		if(CurrentWeapon.UnselectAnim)	//ak mame animacku
		{
			CurrentWeapon.BeginUnselect()	//animhook na konci animacky zavola na Select novu zbran (ChangeWeapon)!!!!!!!!
			hands.PlayUnselectAnim()

//			if(CurrentWeapon.WeapUnselectAnim)
//				SetAnimSlot(CurrentWeapon, 1, CurrentWeapon.WeapUnselectAnim, 8, 0, NULL, 30, 0)
		}
		else
		{
			UnselectWeapon()				//pokial nic nezabranilo skrytiu starej zbrane
			SelectWeapon(ChangeWeapon)	//dame mu novu
		}
		return true
	}

//-----------------------------------------------------------------
	int GetBestAvailableWeapon()
	{
		int CurWeap = CurrentWeapon.ClassNum
		int n, CurChoice, AdeptAmmo
		int BestWeap = -1
		
		for(n = 0; n < 7; n++)	//hladame zbran ktora je definovana ako vyhodna na zaklade co drzime teraz
		{
			CurChoice = BestWeapChoices[CurWeap][n]
			WeaponClass ChangeAdept = PWeapons[CurChoice]
			
			if(!ChangeAdept)
				continue

			AdeptAmmo = ChangeAdept.Ammo + Ammo[ChangeAdept.AmmoType]

			if(AdeptAmmo <= 0 && CurChoice != CJackHammer)
				continue
			
			return CurChoice
		}

		return CDummyWeapon	//tu by sa to nemalo nikdy dostat
	}

//-----------------------------------------------------------------
	void M_AddTask(ExtClass other)
	{
		World.TaskList.AddTask(other._value)
	}

//-----------------------------------------------------------------
	void M_RemoveTask(ExtClass other)
	{
		World.TaskList.CheckTask(other._value)
	}

//-----------------------------------------------------------------
	void M_AddItem(ExtClass other)
	{
		string parm = other._value
		int ClNum = 0

		GetValue(NULL, parm, 0, ClNum)	//ked existuje globalna premenna tak je to zbran
		bool IsWeapon

		if(ClNum == 0)		//neni to zbran ale item
		{
			ClNum = World.ItemColPtr.GetItemIndex(parm)
			IsWeapon = false
		}
		else
			IsWeapon = true

		World.ItemColPtr.GetObjectFromIndex(ClNum)

		if(IsWeapon)	//ked to je weapon
		{
			if(GetWeaponPointer(ClNum) == NULL)
			{
				int MagazineAmmo = 0

				if(other._name == "CheatTempClass")	//volane z cheatu a tam chceme aspon jeden naboj
					MagazineAmmo = 1

				WeaponClass wc = CreateWeapon(ClNum, 0, MagazineAmmo)	//ked ju nema tak zbran pridame ale bez nabojov. pouziva sa to iba pre ucely cutscen na zaciatku levelu a pod
	
				if(wc)
					wc.Pick(this)
			}
		}
	}

//-----------------------------------------------------------------
	void M_SelectWeapon(ExtClass other)
	{
		string parm = other._value
		int ClNum = 0
		GetValue(NULL, parm, 0, ClNum)	//ked existuje globalna premenna tak je to zbran

		if(ClNum > 0)		//neni to zbran ale item
			WeaponChange(ClNum, false)
	}

//---------------------------------------------------------------------------------------------------------
	bool ProcessWeaponSelect()
	{
		if(RealWeaponsNum == 0)
			return false

		if(ChangeWeapon == NULL && status != ST_NOCONTROL && !World.CutScene && !MiniCutsceneStatus)	//pokial uz neprebieha proces vymeny zbrane a nic neovladame
		{
			int SelectFromKey = 0

			if(KEY(WEAP_JACKHAMMER, true))
				SelectFromKey = CJackHammer

			if(KEY(WEAP_PISTOL, true))
				SelectFromKey = CPistol

			if(KEY(WEAP_SHOTGUN, true))
				SelectFromKey = CShotgun

			if(KEY(WEAP_MACHINEGUN, true))
				SelectFromKey = CMachinegun

			if(KEY(WEAP_SNIPER_RIFLE, true))
				SelectFromKey = CSniperRifle

			if(KEY(WEAP_ROCKET_LAUNCHER, true))
				SelectFromKey = CRocketLauncher

			if(KEY(WEAP_FLAMETHROWER, true))
				SelectFromKey = CFlameThrower

			if(KEY(WEAP_GRENADE, true))
				SelectFromKey = CGrenade

			if(SelectFromKey && GetWeaponPointer(SelectFromKey))
			{
				if(World.ManipEntity)
					DropManipEntity(true)
					
				if(WeaponChange(SelectFromKey, true))
				{
					WeapGUISelected = SelectFromKey - 1
	//					WeapGUIStatus = 1
	//					WeapGUIScroll = 0
	//					WeapGUIActivityTimer = 0
				}
			}
		}

		int n
		int WheelUp = g_WHEEL_UP
		int WheelDown = g_WHEEL_DOWN
		bool ReadyToSelect[10]

		WeaponClass CurToSelect

		if(WeapGUIStatus == 0)	//bar je zasunuty
		{
			if(WheelUp || WheelDown)
			{
				if(ChangeWeapon == NULL && status != ST_NOCONTROL && !World.CutScene && !MiniCutsceneStatus)
				{
					WeapGUISelected = CurrentWeapon.ClassNum - 1
					WeapGUIStatus = 1
					WeapGUIScroll = 0
					WeapGUIActivityTimer = 0
//					WheelUp = 0
//					WheelDown = 0
					PlaySound(NULL, g_sCommonSounds[3], SF_ONCE)
				}
				else
					return false
			}
			else
				return false
		}

		if(WeapGUIStatus != 0)	//bar nieje zasunuty
		{
			for(n = 0; n < MaxAvailableWeapons; n++)
			{
				CurToSelect = GetWeaponPointer(n + 1)

				if(CurToSelect && CurToSelect.ClassNum != CDummyWeapon)
					ReadyToSelect[n] = CurToSelect.CanSelect()
				else
					ReadyToSelect[n] = false
			}
		}

		if(WeapGUIStatus == 1)	//bar sa vysuva
		{
			WeapGUIScroll += ftime * 8

			if(WeapGUIScroll >= 1)
			{
				WeapGUIScroll = 1
				WeapGUIActivityTimer = 0	//3 sekundy
				WeapGUIStatus = 2
			}
		}

		if(WeapGUIStatus == 2)	//je vysunuty
		{
			WeapGUIActivityTimer += ftime * 2

			if(WeapGUIActivityTimer >= 3)
			{
				WeapGUIScroll = 1
				WeapGUIActivityTimer = 3
				WeapGUIStatus = 3
				PlaySound(NULL, g_sCommonSounds[4], SF_ONCE)
			}
		}

		if(WeapGUIStatus == 3)	//zasuva sa
		{
			if(WeapGUIActivityTimer != 3)
			{
				WeapGUIStatus = 1
				return true
			}

			WeapGUIScroll -= ftime * 4

			if(WeapGUIScroll <= 0)
			{
				WeapGUIScroll = 0
				WeapGUIStatus = 0
			}
		}

		if(WeapGUIStatus > 0)	//vzdy ked je vidiet
		{
			if(ChangeWeapon == NULL && status != ST_NOCONTROL && !World.CutScene && !MiniCutsceneStatus/* && WeapGUIScroll > 0.5*/)	//pokial uz neprebieha vymena zbrane. pri vysuvani nedovolime selekting aby sa to bolo na aktualnej zbrani
			{
				bool FoundReadyWeapon
				if(WheelUp)
				{
					FoundReadyWeapon = false
					while(!FoundReadyWeapon)
					{
						WeapGUISelected--
						WeapGUIActivityTimer = 0
	
						if(WeapGUISelected < 0)
							WeapGUISelected = MaxAvailableWeapons - 1
						
						if(ReadyToSelect[WeapGUISelected])
							FoundReadyWeapon = true
					}
					PlaySound(NULL, g_sCommonSounds[7], SF_ONCE)
				}

				if(WheelDown)
				{
					FoundReadyWeapon = false
					while(!FoundReadyWeapon)
					{
						WeapGUISelected++
						WeapGUIActivityTimer = 0
	
						if(WeapGUISelected >= MaxAvailableWeapons)
							WeapGUISelected = 0

						if(ReadyToSelect[WeapGUISelected])
							FoundReadyWeapon = true
					}
					PlaySound(NULL, g_sCommonSounds[7], SF_ONCE)
				}

				if(MousePress(0) && ReadyToSelect[WeapGUISelected])
				{
					ClearMousePress(0)

//#ifdef DEVELOPER
					if(WeapGUISelected < MaxAvailableWeapons)
/*#else
					if(WeapGUISelected < RealWeaponsNum)
#endif			*/
					{
						WeapGUIActivityTimer = 3
						WeapGUIStatus = 3

						if(World.ManipEntity)
							DropManipEntity(true)

						if(ChangeWeapon == NULL)	//pokial uz neprebieha proces vymeny zbrane
							WeaponChange(WeapGUISelected + 1, true)

						PlaySound(NULL, g_sCommonSounds[5], SF_ONCE)
					}
					else
						PlaySound(NULL, g_sCommonSounds[6], SF_ONCE)
				}
			}

			float flip = 1 - WeapGUIScroll		//trochu dynamiky z rychla do pomala
			float Scroll = flip * flip
			Scroll = 1 - Scroll

			float IconWidth = 128
			float IconHeight = 51

			float Width
			float Left
			float Top
			float Height = IconHeight
			float ScrollLength = IconWidth + 17			//+ okraj z prava
			int Color
			float Alpha
			float Brithness
			int PosOffset


//			WeaponClass wc
//			n = 0
			int WeapIndex = 0
			int ColorCascade = 13	//prvy sused 75% transp a kazdy dalsi o 5 % menej. to su tie 5 %

			while(WeapIndex < MaxAvailableWeapons)
			{
//				wc = PWeapons[n++]

//				if(!wc)
//					continue

//				ItemDef def = World.ItemColPtr.GetItemFromIndex(wc.ClassNum)
				PosOffset = abs(WeapIndex - WeapGUISelected)

				float FinScroll = Scroll

				for(int r = 0; r < PosOffset; r++)	//kazdy item ide naviac opozdene
					FinScroll *= FinScroll

				if(WeapIndex == WeapGUISelected)		//selektnuty ma alfu aj jas naplno
				{
					Alpha = 255
					Brithness = 255
					WeapGUIAddScrolls[WeapIndex] = WeapGUIAddScrolls[WeapIndex] + (ftime * 8)

					if(WeapGUIAddScrolls[WeapIndex] >= 1)
						WeapGUIAddScrolls[WeapIndex] = 1
				}
				else											//ostatni to maju inak ;-)
				{
					Alpha = 153 - (ColorCascade * (PosOffset - 1))				//prvy sused 75% transp a kazdy dalsi o 5 % menej
					Brithness = 127

					WeapGUIAddScrolls[WeapIndex] = WeapGUIAddScrolls[WeapIndex] - (ftime * 8)

					if(WeapGUIAddScrolls[WeapIndex] <= 0)
						WeapGUIAddScrolls[WeapIndex] = 0
				}

				Left = 800 - (ScrollLength * FinScroll)

				flip = 1 - WeapGUIAddScrolls[WeapIndex]
				float AddScr = flip * flip
				AddScr = 1 - AddScr

				Left -= AddScr * FinScroll * 19
				Width = ScrollLength
				clamp Width<0, IconWidth>

				Top = 80 + (Height * (float)WeapIndex)

				float FinalBrightness = Brithness + (AddScr * (float)127)	//z polovicneho jasu fadujeme do plneho
				clamp FinalBrightness<0, 255>

				int R = FinalBrightness
				int G = FinalBrightness
				int B = FinalBrightness

				if(ReadyToSelect[WeapIndex] == false)
				{
					G = (float)FinalBrightness * 0.3
					B = G
				}

				Color = ARGB(Alpha, R, G, B)

				SetWidgetPos(HUD.WeaponList[WeapIndex], Left, Top, Width, Height)
				SetWidgetColor(HUD.WeaponList[WeapIndex], Color)	////75 %transp

				ShowWidget(HUD.WeaponList[WeapIndex], true)
				WeapIndex++
			}
		}
		else
		{
			for(n = 0; n < MaxAvailableWeapons; n++)
				ShowWidget(HUD.WeaponList[n], false)
		}

		if(WeapGUIStatus > 0)
			return true

		return false
	}

//------------------------------------------------------------------------------------------------
	void UpdateOxygenBar()
	{
		float rate = oxygen / PlayerMaxOxygen
		float BarWidth = rate * 100
		SetWidgetPos(HUD.WOxygenBar, 17, 495, BarWidth, 16)

/*		if(rate <= 0.25)
			SetWidgetImage(HUD.WTimeBar, 1)
		else
			SetWidgetImage(HUD.WTimeBar, 0)*/
	}

//------------------------------------------------------------------------------------------------
	float GetOxygen()
	{
		return oxygen
	}

//------------------------------------------------------------------------------------------------
	void AddOxygen(float quantity)
	{
		oxygen += quantity
		clamp oxygen<0, PlayerMaxOxygen>
		OxygenAdding = true
		OxygenDownTimer = 1	//klesat zacne az po chvili
		UpdateOxygenBar()
	}

//-----------------------------------------------------------------
	void ProcessWeaponVibrations()
	{
		if(IsInherited(CurrentWeapon, CFireWeaponClass) == false)
			return

		FireWeaponClass wc = CurrentWeapon

		WeaponKickOffSmall[0] = WeaponKickOffSmall[0] - (WeaponKickOffSmall[0] * 5 * ftime)

		if(WeaponKickOffSmall[0] < 0)
			WeaponKickOffSmall[0] = 0

		CameraKickOffset += WeaponKickOffSmall[0] * 50 * ftime

		WeaponKickTimer += ftime * 100

		if(WeaponKickTimer > 20)
		{
			CameraKickOffset -= CameraKickOffset * 10 * ftime
		}
		else
		{
			if(wc.Action != 1 && wc.Action != 3)
				CameraKickOffset -= CameraKickOffset * 10 * ftime
		}

		if(CameraKickOffset < 0)
			CameraKickOffset = 0

		WeaponKickOff[0] = CameraKickOffset
	}

//-----------------------------------------------------------------
	vector ProcessWeaponLagMotion()
	{
		vector off = CameraAngles
		float g = off[0]
		off[0] = off[2]
		off[2] = g
		float f = off[2]
		f = -f
		off[2] = f
		off = off - WeaponAngles
		off = off * 0.15
		vector vc = off * ftime * 50//0.2
		WeaponAngles = WeaponAngles + vc
		return off
	}

//-----------------------------------------------------------------
	void StartShooting(int mode)	//vola SA ZA KAZDYM VYSTRELOM
	{
		Shooting++
		ShootingMode = mode
		WeaponKickTimer = 0
		WeaponKickOffSmall[0] = 0
		EnableIdleWarAnims(false)	//Pocas vystrelu IdleVars neprichadzaju do uvahy
		FireWeaponClass wc = CurrentWeapon

		float ExactAimFactor = 1 - ExactAimPath
		ExactAimFactor = 0.24 + (ExactAimFactor * 0.75)

		WeaponKickOffSmall[0] = WeaponKickOffSmall[0] + (wc.KickUp[mode] * ExactAimFactor)

		if(CurrentWeapon.ClassNum != CGrenade)
			Noises = Noises | NOISE_SHOOT
	}

//-----------------------------------------------------------------
	void EndShooting()	//vola sa az ked skonci suvisla palba
	{
		if(!Shooting)	//volane zbytocne
			return

		EnableIdleWarAnims(true)

		if(CameraShotingVertOffset < 0)
			CameraKickOffset += CameraShotingVertOffset	//pocas strelby sme hybali mysou smerom dolu. o tolko menej sa bude zbran narovnavat
		else
			CameraKickOffset -= CameraShotingVertOffset

		if(CameraKickOffset < 0)	//ak sme hybali mysou viac nez je spatny pohyb ktory by mal nasledovat
		{
			if(CameraShotingVertOffset < 0)
				CameraShotingVertOffset -= CameraKickOffset
			else
				CameraShotingVertOffset += CameraKickOffset

			CameraKickOffset = 0
		}

		WeaponKickOff[0] = CameraKickOffset	//o tolko sa bude zbran vyrovnavat
		
		if(CameraShotingVertOffset < 0)		//zbytok prenesieme do kamery
			CameraAngles[0] = CameraAngles[0] - CameraShotingVertOffset
		else
			CameraAngles[0] = CameraAngles[0] + CameraShotingVertOffset
		
		CameraShotingVertOffset = 0
		Shooting = 0					//suvisla palba skoncila
	}

	float HandsAmimTimer
	float HandsAmimTargOffset
	float CurHandsAmimOffset

//-----------------------------------------------------------------
	void ProcessWeapons()
	{
		vector LagOff = ProcessWeaponLagMotion()	//opozdeny pohyb zbrane. vracia uhly v coords systeme kamery
		float miss
		float HandsOffsetTarget = 4	//o kolko sa ma zbran pri suvislej palbe posuvat dozadu

 		if(Shooting)
 			CameraShotingVertOffset += cincy	//pocitame o kolko sa pohla mys vertikalne aby sa to dalo potom odcitat zo spatneho pohybu po strelbe
 		else
 			CameraShotingVertOffset = 0

		if(Shooting)
		{
			ShootingCrossScale += ftime

			if(HandsOffset < HandsOffsetTarget)
			{
				miss = HandsOffsetTarget - HandsOffset
				HandsOffset += miss * 10 * ftime

				if(HandsOffset >= HandsOffsetTarget)
					HandsOffset = HandsOffsetTarget
			}
		}
		else
		{
			ShootingCrossScale -= ftime * 10

			if(HandsOffset > 0)
			{
				miss = HandsOffset * 0.9
				miss = HandsOffset - miss
				HandsOffset -= miss * 100 * ftime

				if(HandsOffset <= 0)
					HandsOffset = 0
			}
		}
		
		clamp ShootingCrossScale<0, 1>

//exact aim
		vector ExactAimOff, ExactAimAngOff

		if(IsInherited(CurrentWeapon, CFireWeaponClass) == true)
		{
			FireWeaponClass wc = CurrentWeapon

			int SAttack = KEY(S_ATTACK, false)
			float afac = 4
			
			//1.3 Automaticke zaliceni pri lean (krome sniperky!)
			if(wc && ClassName(wc) != "item_weapon_sniperrifle")
			{
				if(KEY(LEAN_LEFT, false) || KEY(LEAN_RIGHT, false))
				{
					SAttack = true
					afac = 6
				}
			}
			
			if(SAttack && wc.ExactAimFOV >= 0 && CurrentWeapon.Action != 2)	//ExactAimFOV == -1 v pripade ze nechceme ExactAimMode
			{
				ExactAim += ftime * afac * 1.00004

				if(ExactAim >= 1)
					ExactAim = 1

				EnableIdleWarAnims(false)
			}
			else
			{
				ExactAim -= ftime * afac

				if(ExactAim <= 0)
					ExactAim = 0
			}

			ExactAimPath = ExactAim * ExactAim			
			ExactAimOff = wc.ExactAimOffset * ExactAimPath
			ExactAimAngOff = wc.ExactAimAngOffset * ExactAimPath

			if(ExactAimPathPrev != ExactAimPath)
			{
				ExactAimPathPrev = ExactAimPath

				float AddFov = ExactAimPath * wc.ExactAimFOV
				SetCameraVerticalFOV(0, DefaultFOV - AddFov)
				wc.OnExactAimInterpolate(ExactAimPath)
			
				float ff = ExactAim *= 0.769
				ExactAimCrossScale = 1 - ff
			}
		}

		if(status == ST_RUN)
		{
			float TimeStep = 8
			TimeStep -= ExactAimPath * 6
			HandsAmimTimer += ftime * TimeStep
			HandsAmimTargOffset = sin(HandsAmimTimer) * 2
		}
		else
		{
			if(status == ST_JUMP)
			{
/*				if(cincy > 5)
					HandsAmimTargOffset = 2
				else*/
					HandsAmimTargOffset = 8
			}
			else
				HandsAmimTargOffset = 0
		}
		
		float AnimOffDiff = HandsAmimTargOffset - CurHandsAmimOffset
		CurHandsAmimOffset += AnimOffDiff * ftime * 5

		vector HandsFinalOffset = ExactAimOff
		HandsFinalOffset[2] = HandsFinalOffset[2] + HandsOffset
		HandsFinalOffset[1] = HandsFinalOffset[1] + (CurHandsAmimOffset * -0.1)

		vector HandsFinalAngOffset = ExactAimAngOff + LagOff
		HandsFinalAngOffset[2] = HandsFinalAngOffset[2] + CurHandsAmimOffset

//		HandsFinalAngOffset[0] = HandsFinalAngOffset[0] - (LeanAngle * 1.0)	//ruky naklaname trochu s kamerou

		SetPosInCamAxis(Hands, HandsFinalOffset, HandsFinalAngOffset, 1)	//drzi na kamere zbran !

		DummyPlayer.SetOrientation(coords, CameraAngles[1] + 90)

		if(WeapGUIStatus > 0)	//prebieha vyber zbrane z GUI
			return

		bool ClearPrimary

		if(World.ManipEntity)
			ClearPrimary = false
		else
			ClearPrimary = 1 - CurrentWeapon.Automatic[PRIMARY]

		PrimaryAttack = KEY(P_ATTACK, ClearPrimary)

		if(!PrimaryAttack)
			SecondaryAttack = KEY(S_ATTACK, false/*1 - CurrentWeapon.Automatic[SECONDARY]*/)
		else
			SecondaryAttack = false

		if(Shooting)	//kontrolujeme ci kopanie zbrane netreba zastavit
		{
			if(ShootingMode == PRIMARY)
			{
				if(!PrimaryAttack || ChangeWeapon || CurrentWeapon.Action == 2)
					EndShooting()	//suvisla palba skoncila
			}

			if(ShootingMode == SECONDARY)
			{
				if(!SecondaryAttack || ChangeWeapon || CurrentWeapon.Action == 2)
					EndShooting()	//suvisla palba skoncila
			}
		}

		if(ChangeWeapon)	//ak prave menime zbran tak nedovolime nic ine
			return

		vector tpos

		if(PrimaryAttack && CurrentWeapon.ClassNum != CDummyWeapon)
		{
			tpos = g_CamMat[3]
			tpos = tpos + (World.CameraNorm * (float)8192)

			if(CurrentWeapon.Action(1, tpos) == false)	//vystrel uz prebieha (alebo reload)
			{
				if(CurrentWeapon.Action == 0)
				{
					if(CurrentWeapon.Ammo == 0)
					{
						if(Ammo[CurrentWeapon.AmmoType] == 0)							//nemame ziadne naboje tak berieme predchadzajucu zbran
							WeaponChange(GetBestAvailableWeapon(), true)
						else
						{
							if(CurrentWeapon.Action(2, tpos) == false)				//skusime reload
								WeaponChange(GetBestAvailableWeapon(), true)

							UpdateAmmoBar()
						}
					}
				}
			}
			else
			{
				StartShooting(PRIMARY)
				UpdateAmmoBar()

				if(CurrentWeapon.ClassNum == CFlameThrower)
				{
					if(!CurrentWeapon.ActionSound)
						CurrentWeapon.ActionSound = PlaySound(CurrentWeapon, CurrentWeapon.Sounds[0], 0)
				}
				else
				{
					if(CurrentWeapon.ClassNum == CMachinegun)	//co vystrel, to zvuk. ale musi byt kratky. je nezmysel zvuk usekavat a pustat znovu
						CurrentWeapon.ActionSound = PlaySound(WeaponSoundPos, CurrentWeapon.Sounds[0], SF_ONCE)
					else
					{
						if(CurrentWeapon.ActionSound)
							EndSound(CurrentWeapon.ActionSound)
	
						CurrentWeapon.ActionSound = PlaySound(WeaponSoundPos, CurrentWeapon.Sounds[0], SF_ONCE)
					}
				}
			}
		}

/*		if(SecondaryAttack)
		{
			tpos = g_CamMat[3]
			tpos = tpos + (World.CameraNorm * (float)8192)

			if(CurrentWeapon.Action(3, tpos) == false)	//sekundarny mod zbrane
			{
				if(CurrentWeapon.Action == 0)
				{
					if(CurrentWeapon.Ammo == 0)
					{
						CurrentWeapon.Action(2, tpos)				//skusime reload
						UpdateAmmoBar()
					}
				}
			}
			else
			{
				StartShooting(SECONDARY)
				UpdateAmmoBar()
			}
		}*/
		ProcessWeaponVibrations()

		if(KEY(RELOAD, true))
		{
//			if(CurrentWeapon.Action == 1)	//fire
//				CurrentWeapon.InterruptAction()
			if(ChangeWeapon == NULL)	//select/unselect zbrani nechame na pokoji skoncit za kazdych okolnosti
			{
				if(CurrentWeapon.Action(2, tpos))	//reload
					SetAnimSlot(Hands, s_weapon - 1, VNULL, 0, 1, NULL, 30, AF_ONCE)
			}
		}

/*		if(KEY(NEXT_WEAPON, true))	//dalsia zbran
		{
			if(World.ManipEntity)
				DropManipEntity(true)

			ChangeNextWeapon()
		}

		if(KEY(PREV_WEAPON, true))	//predchadzajuca zbran
		{
			if(World.ManipEntity)
				DropManipEntity(true)

			ChangePreviousWeapon()
		}*/
	}

//-----------------------------------------------------------------
	bool ApplyBackForce()
	{
		if(MotionSpeed == 0)
			return false

//		float ke = dBodyGetKineticEnergy(this)

//		if(ke > 1000)	//koli vybuchom
//			return false

		vector vl = GetVelocity(this)
		
		if(TrainEnt)
		{
			vector TrainVel = dBodyGetVelocityAt(TrainEnt, coords) * 1.10
			vl = vl - TrainVel
		}
		
		vl[2] = 0
		float lng = VectorLength(vl)
		float rate = lng / MotionSpeed
//		clamp rate<0, 1>	//normalne
		clamp rate<0.5, 1>	//s nulovym trenim
		
		float frate = 1 - rate
		vl = -vl

//		float Ftime = ftime * 100
		float b = 20 * frate
		b += 20// * Ftime
		vl = vl * b
		vl = vl * rate
		BrakeForce = vl
		return true
	}

	trigger_hack_detector BestHackTrigger		//najblizsi hack trigger

//------------------------------------------------------------------------------------------------
	float GetDistanceToHackTrigger(trigger_hack_detector trigger)
	{
		vector MyPos = GetMatrix(this, 3)
		vector pos = trigger.CoordToLocal(MyPos)
		pos = trigger.GetNearestPosition(pos)
		pos = trigger.CoordToParent(pos)	//najblizise miesto s triggerom
		return VectorLength(MyPos - pos)
	}

//------------------------------------------------------------------------------------------------
	void ProcessHackSignal()
	{
		float ReconSignalBefore = ReconSignal
		float DistToTrigger
		ReconSignal = 0

		if(InsideHackTrigger)	//sme v hack triggeri takze neni co riesit. mame plny signal
		{
			if(BestHackTrigger)	//toto by malo byt nastavene vzdy ked sme tu
			{
				DistToTrigger = GetDistanceToHackTrigger(BestHackTrigger)
				
				if(DistToTrigger > 0.01)	//uz niesme v triggeri
				{
					BestHackTrigger = NULL
					SetHackAdepts(NULL, 0)							//zahodime starych adeptov (postupne odfaduju)
				}
				
				if(BestHackTrigger && BestHackTrigger.ContainHackableEntities())
					ReconSignal = 1
			}					
		}
		else
		{
			trigger_hack_detector trigger = FirstHackTrigger
			trigger_hack_detector BestTrigger = NULL
			float BestDistToTrigger = ReconSignalDistFromTrigger
			
			while(trigger)	//hladame najblizsi hackovaci trigger
			{
				if(GetEventMask(trigger) & EV_TOUCH)	//vynechame disablovane triggery
				{
					if(trigger.ContainHackableEntities())
					{
						DistToTrigger = GetDistanceToHackTrigger(trigger)
	
						if(DistToTrigger < BestDistToTrigger)
						{
							BestDistToTrigger = DistToTrigger
							BestTrigger = trigger
						}
					}
				}
				trigger = trigger.NextSiblingTrigger
			}

			if(BestTrigger)	//trigger je v dosahu RECONU takze ukazujeme stav signalu
			{
				if(BestDistToTrigger <= 0.01)	//sme v triggeri
				{
					if(BestTrigger != BestHackTrigger)	//a neni to ten posledny
					{
						BestHackTrigger = BestTrigger	//mapetame si najblizsi trigger
						SetHackAdepts(NULL, 0)							//zahodime starych adeptov (postupne odfaduju)
						
						ExtClass NewAdepts[16]
						int NewAdeptsNum = BestHackTrigger.GetHackAdepts(NewAdepts)
						SetHackAdepts(NewAdepts, NewAdeptsNum)		//nahodime novych adeptov
					}
				}

				ReconSignal = 1 - (BestDistToTrigger / ReconSignalDistFromTrigger)
				clamp ReconSignal<0, 1>
//				AddDShape(SHAPE_BBOX, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos - "1 1 1", pos + "1 1 1")
			}
			else
			{
				if(BestHackTrigger)
				{
					BestHackTrigger = NULL
					SetHackAdepts(NULL, 0)							//zahodime starych adeptov (postupne odfaduju)
				}
			}
		}

		if(ReconSignal != ReconSignalBefore)
			UpdateHackSignalBar()

		float FlashTransp

		if(ReconSignal == 1 && !HackMode)	//signal je naplno tak flashujeme
		{
			ReconSignalFlash += ftime * 300

			if(ReconSignalFlash > 360)
				ReconSignalFlash -= 360

			FlashTransp = fabs(sin(ReconSignalFlash * DEG2RAD)) * 255
			SetWidgetColor(HUD.WHackingSignalFlash, AWHITE(FlashTransp))
		}
		else	//signal neni naplno tak len doflashujeme
		{
			if(ReconSignalFlash <= 360)
			{
				ReconSignalFlash += ftime * 300
				
				if(ReconSignalFlash >= 360)
					ReconSignalFlash = 360

				FlashTransp = fabs(sin(ReconSignalFlash * DEG2RAD)) * 255
				SetWidgetColor(HUD.WHackingSignalFlash, AWHITE(FlashTransp))
			}
		}
	}

//------------------------------------------------------------------------------------------------
	bool ProcessHacking()
	{
		if(HackingDisabled)
			return false

		ProcessHackSignal()

		if(Hacked)	//sme nahackovani do nejakej entity
		{
//			dBodyActive(this, false, false)
			/*ApplyBackForce()	//player stoji na mieste, brzime ho aby sa velmi nepohyboval ale uplne tuhy nemoze byt pretoze by mohol branit v pohybe potvoram atd.
			BrakeScale = 10.0
			dBodyApplyForce(this, BrakeForce * BrakeScale)*/

			LastStayPos = GetOrigin(this)
			DummyPlayer.SetOrientation(coords, CameraAngles[1] + 90)

			bool HackModeKey = KEY(CONTROLLER, true)

			if(KEY(USE, true) || KeyState(KC_ESCAPE) || HackModeKey || Hacked.HackInterf.NeedLogOut())	//vraciame sa do playera
			{
				ClearKey(KC_ESCAPE)
				InterruptHacking(HackModeKey)
			}
			else
				Hacked.HackInterf.OnFrame()

			LoginTime += ftime * 100
			
			if(MaxLoginTime != -1)	//pokial to ma obmedzeny cas po ktory je mozne byt lognuti tak ho odpocitame a potom sa odlogujeme
			{
				if(LoginTime >= MaxLoginTime)
				{
					LoginTime = MaxLoginTime
					InterruptHacking(HackModeKey)
				}
				
				float FinishTime = MaxLoginTime - LoginTime
				clamp FinishTime<0, 100>
				
				if(FinishTime < 100)
				{
					HackNoiseFade = 1 - (FinishTime / 100)
					ShowWidget(HUD.WHackingPreviewNoise, true)
				}
				else
					HackNoiseFade = 0
			}

			if(HackModeFade > 0)	//bez ohladu na HackMode musime updatovat GUI koli fadovaniu a pod
				UpdateHackGUI()

			return true
		}
		else			//ak sme v hack triggeri tak informujeme o moznosti zapnut hack mode a pripadne ho managujeme
		{
			if(!World.CutScene && !MiniCutsceneStatus && status != ST_NOCONTROL && KEY(CONTROLLER, true))
				SwitchHackMode()

			if(HackMode)	//je zapnuty hacking mod
			{
				if(HackModeFade < 1)
					HackModeFade = LinearBlend(HackModeFade, 1, 6)

				if(InsideHackTrigger)
				{
					float WindowScale

					if(HackStatus == 0)	//selektujeme
					{
						ProcessHackSelecting()				//selektime

						if(SelectedHackAdept && SelectedHackAdept.HackInterf.CanHacking())
							HackNoiseFade = 1 - (SelectedHackAdept.HackInterf.CurHackingTime / SelectedHackAdept.HackInterf.HackingTime)
						else
							HackNoiseFade = 1

						if(SelectedHackAdept && KEY(USE, true) && SelectedHackAdept.HackInterf.CanLogin())	//nieco sme selektli a USE
						{
							HackLoginTimer = 0

							if(HackingLoopSound)	//poistka
							{
								EndSound(HackingLoopSound)
								HackingLoopSound = 0
							}

							if(SelectedHackAdept.HackInterf.GetMaxLoginTime() == 0)	//neni potreba do toho prepinat kameru
							{
								Hacked = SelectedHackAdept
								Hacked.HackInterf.LogIn()	//logneme sa
								World.HackInfDialog.ShowMessage("// #Login [#" + Hacked.HackInterf.name + "]")
								InterruptHacking(false)		//a hned odlogujeme. Hacked je NULL
							}
							else	//nasizujeme okno na screenu a logneme sa do toho
							{
								HackStatus = 1
								SetStatus(ST_NOCONTROL)
								ShowHUD(-1, -1, -1, 0, 0, 0, 0, 0, -1, 0)								
							}
							return true							//sme nahackovani. vraciame true a to znamena ze player nebude ovladatelny
						}
					}

					if(HackStatus == 1)	//logujeme sa do entity
					{
						if(!SelectedHackAdept || !SelectedHackAdept.HackInterf)	//pocas prezoomovania do entity sa moze stat ledacos
						{
							InterruptHacking(false)
							return true
						}

						HackLoginTimer += ftime * 5

						if(HackLoginTimer >= 1)	//uz sme nalogovani
						{
							HackLoginTimer = 1
							HackStatus = 2
						}

						WindowScale = HackLoginTimer * HackLoginTimer
						SetHackPreviewExtendsSize(WindowScale)
						
						if(HackLoginTimer == 1)
						{
							ShowHackGUI(false)
							ShowWidget(HUD.WHackingPreviewNoise, true)
							ShowWidget(HUD.WHackingModeGrid2, true)
							ShowWidget(HUD.WHackingPreviewTitle, true)
//							ShowWidget(HUD.WHackingModeGrid, true)
							World.Cross.Show(false)
							Hacked = SelectedHackAdept
							Controlled = SelectedHackAdept
//							SetPostEffect(0, 0, 0, 0, 0)
							Hacked.HackInterf.LogIn()
							WViewWidgetCamera = 1
							SetWidgetWorld(WView, globalmap, 1)
							SetListenerCamera(1)
							DummyPlayer.CurrentWeapon.Show(false)
							DummyPlayer.Transmission()
							LoginTime = 0
							MovementDumping = true
							MaxLoginTime = Hacked.HackInterf.GetMaxLoginTime()
							World.HackInfDialog.ShowMessage("// #Login [#" + Hacked.HackInterf.name + "]")
							
							if(HackModeFade > 0)	//bez ohladu na HackMode musime updatovat GUI koli fadovaniu a pod
								UpdateHackGUI()
							
							return true
						}
					}
				} 
				else	//sme mimo hack trigger
				{
					HackNoiseFade = 1	//mimo hack trigger vzdy zrnime naplno
					
					if(HackStatus == 1)	//moze sa stat ze pocas toho co zoomujeme okono do entity opustime trigger takze okno musime zase zmensit do rohu a enablovat pohyb playera
					{
						HackStatus = 3
						
						if(status == ST_NOCONTROL)
							SetStatus(ST_NONE)
					}
				}
				
				if(HackStatus == 3)	//tento satus moze prebiehat aj ked sme uz mimo trigger
				{
					HackLoginTimer -= ftime * 4

					if(HackLoginTimer <= 0)
					{
						HackLoginTimer	= 0
						HackStatus = 0
						ShowHUD(-1, -1, -1, 1, 1, 1, 1, -1, -1, 1)
					}

					WindowScale = HackLoginTimer * HackLoginTimer
					SetHackPreviewExtendsSize(HackLoginTimer)
				}
			}
			else				//je vypnuty hacking mod. ovladanie uz neni mozne ale musime dofadovat takze GUI updatujeme stale
			{
				if(HackModeFade > 0)
				{
					HackModeFade = LinearBlend(HackModeFade, 0, 6)

					if(HackModeFade == 0)
					{
						OnHackingModeEnd()
						return false
					}
				}
			}

			if(HackModeFade > 0)	//bez ohladu na HackMode musime updatovat GUI koli fadovaniu a pod
				UpdateHackGUI()

			World.HackInfDialog.OnFrame()
		}

		return false
	}

//-----------------------------------------------------------------
	vector CheckForPlayerSpaceDown(vector pos)
	{
		vector DirVec = Vector(0, 0, -100)
		dShapeCastSetDir(ShapeCast, DirVec)
		vector mat4[4] = {"1 0 0", "0 1 0", "0 0 1"}
		mat4[3] = pos
		SetMatrix(ShapeCast, mat4)
//		AddDShape(SHAPE_LINE, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos, pos + Vector(0, 0, -lng))

		MinClass clipent
		vector cpos, cnorm
		float dist = dShapeCastUpdate(ShapeCast, 0, clipent, cpos, cnorm)

		if(dist < 1)
		{
			vector off = DirVec * dist
			return pos + off
		}
		return pos
	}

//-----------------------------------------------------------------
	bool ProcessVehicleControl()
	{
		if(!Controlled)
			return false

		EnterableVehicle vehicle = Controlled
		
		if(!IsInherited(vehicle, CEnterableVehicle))
			return false

		if(Hands && CurrentWeapon.CanUpdateOwnerAnim())	//musime updatovat ruky pretoze moze bezat select/unselect
			BumpFrame(Hands, ftime, 0)

		float MatQuat[4]
		vector CameraMat[4]
		float flip
		float RotBlend, TransBlend
		vector CameraPosEx, CameraPosIn
		vector CameraMatExt[3]
		vector CameraMatIn[3]
		vector Off

		if(VehicleControlStatus == 1)	//preblendujeme kameru do auta
		{
			VehicleControlTimer += ftime * 2
			clamp VehicleControlTimer<0, 1>

			flip = 1 - VehicleControlTimer	//ro rychla
			RotBlend = flip * flip// * flip
			RotBlend = 1 - RotBlend

			vehicle.UpdateCameraSystem()
			RollPitchYawMatrix2(EntityAnglesToCameraAngles(vehicle.GetAnglesForCamera()), CameraMatIn)
			MatrixQuat(CameraMatIn, VehicleQuatIn)
			QuatLerp(MatQuat, VehicleQuatExt, VehicleQuatIn, RotBlend)

			CameraPosEx = vehicle.BodyPart.CoordToParent(VehicleCameraBlendOff)
			vehicle.UpdateCameraSystem()
			CameraPosIn = vehicle.GetCoordsForCamera()
			Off = CameraPosEx - CameraPosIn

			TransBlend = 1 - RotBlend
			Off = Off * TransBlend
			QuatMatrix(MatQuat, CameraMat)
			CameraMat[3] = CameraPosIn + Off
			World.CameraPos = CameraMat[3]
			World.CameraNorm = CameraMat[0]
			SetCameraEx(0, CameraMat)

			CameraPos = CameraMat[3]
			SetOrientationFromCamera()

			if(VehicleControlTimer == 1)
			{
				vehicle.LogIn()
				OnEnterVehicle(1)
				VehicleControlStatus = 2
				return true
			}
		}

		if(VehicleControlStatus == 2)	//sme v aute
		{
			CameraPos = vehicle.HeadPosInside
			SetOrientationFromCamera()	//drzime playera v aute

			if(KEY(USE, true))
			{
				vehicle.BeforeLeaving()
				VehicleControlStatus = 4
				return true
			}
			return true
		}

		if(VehicleControlStatus == 4)	//cakame az vehicle premiestni kameru do interieru a auto nezastavi
		{
			if(VectorLength(GetVelocity(vehicle.BodyPart)) < 300)
			{
				vehicle.InExtRateTarget = 0	//nech sa kamera premiestni do first persnu

				if(vehicle.InExtRate == 0)	//ked uz je tam
				{
					int OutSide = vehicle.EnteredSide
					vector IdealOutPos = vehicle.GetIdealOutPos(OutSide)
					vector CastedOutPos = CheckForPlayerSpaceDown(IdealOutPos)
		//				AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, IdealOutPos, CastedOutPos)
		
		//				asi 20 unitov od zeme pre nas neni prekazka a plyera hodime nad to
					if(VectorLength(IdealOutPos - CastedOutPos) < 30)	//ked neni miesto na strane z ktorej sme nastupovali tak skusime este druhu stranu
					{
						OutSide = 1 - vehicle.EnteredSide
						IdealOutPos = vehicle.GetIdealOutPos(OutSide)
						CastedOutPos = CheckForPlayerSpaceDown(IdealOutPos)
		//					AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, IdealOutPos, CastedOutPos)
						
						if(VectorLength(IdealOutPos - CastedOutPos) < 30)	//ak neni miesto ani na druhej strane tak smola. zostavame v aute
							return true
					}
					
					vector FreeCameraPos = CastedOutPos + Vector(0, 0, PlayerCapsuleExtends[Crouch] * 2 + PlayerCameraOffset)
					vehicle.UpdateCameraSystem()
					CameraAngles = EntityAnglesToCameraAngles(vehicle.GetAnglesForCamera())
					CameraAngles = FixAngles(CameraAngles)
					CameraPos = FreeCameraPos//vehicle.GetCoordsForCamera()
					SetOrientationFromCamera()	//drzime playera v aute
		
					RollPitchYawMatrix2(CameraAngles, CameraMatExt)
					RollPitchYawMatrix2(CameraAngles, CameraMatIn)
					MatrixQuat(CameraMatExt, VehicleQuatExt)
					MatrixQuat(CameraMatIn, VehicleQuatIn)
		
					VehicleCameraBlendOff = vehicle.BodyPart.CoordToLocal(FreeCameraPos)	//na tejto pozicii vystupime (pozicia pre kameru)
					VehicleCameraLean = CameraAngles[2]
	
					VehicleControlStatus = 3
					VehicleControlTimer = 1
					vehicle.LogOut()
					OnLeaveVehicle(0)
				}
			}
			return true
		}

		if(VehicleControlStatus == 3)	//vystupujeme z auta
		{
			VehicleControlTimer -= ftime * 2
			clamp VehicleControlTimer<0, 1>
			
			flip = 1 - VehicleControlTimer	//ro rychla
			RotBlend = flip * flip// * flip
			RotBlend = 1 - RotBlend

			float AngOff = GetAngularOffset(0, VehicleCameraLean)
			CameraAngles[2] = AngOff * VehicleControlTimer			//"uklon" kamery postupne vyrovname

			RollPitchYawMatrix2(CameraAngles, CameraMatExt)
			MatrixQuat(CameraMatExt, VehicleQuatExt)

			QuatLerp(MatQuat, VehicleQuatExt, VehicleQuatIn, RotBlend)

			vehicle.UpdateCameraSystem()
			CameraPosEx = vehicle.BodyPart.CoordToParent(VehicleCameraBlendOff)
			CameraPosIn = vehicle.GetCoordsForCamera()
			Off = CameraPosEx - CameraPosIn
			
			TransBlend = 1 - RotBlend
			Off = Off * TransBlend
			QuatMatrix(MatQuat, CameraMat)
			CameraMat[3] = CameraPosIn + Off
			World.CameraPos = CameraMat[3]
			World.CameraNorm = CameraMat[0]
			SetCameraEx(0, CameraMat) 

			CameraPos = vehicle.BodyPart.CoordToParent(VehicleCameraBlendOff)
			SetOrientationFromCamera()

			if(VehicleControlTimer == 0)
			{
				float ang = CameraAngles[0]	//v playerovi s uhlami kamery hybeme aj do minusovych hodnot a v HandleCamera() je limit +- LookLimit. preto musime takto
				if(ang > LookLimit)
					ang = ang - 360
				CameraAngles[0] = ang

				Controlled = NULL
				VehicleControlStatus = 0
				SetStatus(ST_NONE)
				CameraPos = vehicle.BodyPart.CoordToParent(VehicleCameraBlendOff)
				SetOrientationFromCamera()
				WeaponChange(WeaponBeforeManip, true)
				
				if(VehiclePlayerColBlock)
				{
					dBodyRemoveBlock(VehiclePlayerColBlock)
					VehiclePlayerColBlock = 0
				}
				OnLeaveVehicle(1)
			}
		}
		return true
	}

	//-----------------------------------------------------------------
	void Noise(int noises)
	{
		float radius = 0

		//zjisti radius
		if(noises & NOISE_FOOTSTEP)
		{
			if(radius < NOISERANGE_FOOTSTEP)
				radius = NOISERANGE_FOOTSTEP
		}

		if(noises & NOISE_THUD)
		{
			if(radius < NOISERANGE_THUD)
				radius = NOISERANGE_THUD
		}

		if(noises & NOISE_SHOOT)
		{
			if(radius < NOISERANGE_SHOOT)
				radius = NOISERANGE_SHOOT
		}

		if(noises & NOISE_ATTACK)
		{
			if(radius < NOISERANGE_ATTACK)
				radius = NOISERANGE_ATTACK
		}

		ClearFlags(this, TFL_USER6)
		NotifyNoises(this, coords, radius, noises)
		SetFlags(this, TFL_USER6)
	}

//-----------------------------------------------------------------
	bool CanKill()
	{
#ifdef EDITOR	//v editore nebudeme smrtelni
		return false
#endif

		if(g_RunMode == RUNMODE_DEVELOPER)	//mapa bola spustena sposobom Loader.exe NazovMapy	(develop ucely)
			return false

		if(status == ST_DEATH)
			return false
		
		return true
	}

//-----------------------------------------------------------------
	void Kill()
	{
		if(Hacked)
			InterruptHacking(true)

		ShowHackGUI(false)

		if(Controlled && IsInherited(Controlled, CEnterableVehicle))
		{
			DeathMode = DEATHMODE_IN_VEHICLE
			EnterableVehicle vehicle = Controlled
			vehicle.LogOut()
			vehicle.Explosion()
			DeathCameraTarget = World.CameraPos + Vector(0, 0, 2048)
			DeathCounter = 3//0.95
		}
		else
		{
			DeathMode = DEATHMODE_NORMAL
			DeathCameraTarget = GetMatrix(this, 3) - Vector(0, 0, PlayerCapsuleHalfHeights[Crouch] - 5)
			DeathCounter = 1
			Controlled = NULL	//vo vehicli ho potrebujeme nadalej
		}

		SetListenerCamera(0)
		VehicleControlStatus = 0		
		ShowHUD(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
		SetStatus(ST_DEATH)
		PlaySound(this, g_sCommonSounds[16], SF_ONCE)

		HearthSoundNeedStop = true
		if(HealthPumpUpLoopSound)
		{
			EndSound(HealthPumpUpLoopSound)
			HealthPumpUpLoopSound = 0
		}
	}

//-----------------------------------------------------------------
	void ProcessHitZones()
	{
		for(int n = 0; n < 4; n++)
		{
			if(HitZoneStatus[n] == 0)
				continue

			if(HitZoneStatus[n] == 1)	//zviditelnujeme
			{
				HitZoneFade[n] = HitZoneFade[n] + ftime * 8
				ShowWidget(HUD.WHitZones[n], true)

				if(HitZoneFade[n] > 1)
				{
					HitZoneFade[n]	= 1
					HitZoneStatus[n] = 2
				}
			}
			else
			{
				if(HitZoneStatus[n] == 2)	//zhasiname
				{
					HitZoneFade[n] = HitZoneFade[n] - ftime

					if(HitZoneFade[n] < 0)
					{
						HitZoneFade[n]	= 0
						HitZoneStatus[n] = 0
						ShowWidget(HUD.WHitZones[n], false)
					}
				}
			}

			float aplha = HitZoneFade[n] * (float)255
			SetWidgetColor(HUD.WHitZones[n], ARGB(aplha, 255, 255, 255))
		}
	}

//-----------------------------------------------------------------
	void ProcessCrosshair()
	{
	}

//-----------------------------------------------------------------
	void RunHitzoneEffect(vector HitFrom)
	{
		HitFrom[2] = coords[2]
		vector offset = HitFrom - coords
		vector angs = Vector2Angles(offset)
		float PlaAng = FixAngle(CameraAngles[1])
		float ang = FixAngle(PlaAng - angs[1])	//uhol entity voci pohladu playera

		if(ang >= 315 && ang < 360)
			HitZoneStatus[1] = 1
		else
		{
			if(ang >= 0 && ang < 45)
				HitZoneStatus[1] = 1
			else
			{
				if(ang >= 45 && ang < 135)
					HitZoneStatus[2] = 1
				else
				{
					if(ang >= 135 && ang < 225)
						HitZoneStatus[3] = 1
					else
					{
						if(ang >= 225 && ang < 315)
							HitZoneStatus[0] = 1
					}
				}
			}
		}
	}

//-----------------------------------------------------------------
	int Shoot(ShotStorage shot)
	{
		vector Force
		float dmg = shot.damage * ENEMY_DAMAGE_SCALE[DifficultyLevel]

		if(shot.HurtType == HT_SHOT)
		{
//			if(shot.shooter.ClassType == ClassTypeCreature && shot.shooter.ClassNum != ClassNumPlayer)
//				dmg *= World.enemyscale

			if(!Immortality)
				HealthDown(dmg)
	
			if(CheatImmortality)
				Health = MaxHealth
	
			UpdateHealthBar()
//tyhle efekty musi byt videt i po smrti, jinak to vypada
//ze to nefunguje
			RunHitzoneEffect(shot.from)
			World.Quake(30, shot.CameraHitFactor, 10, 15, 15, 1)

			if(Health == 0)
			{
				if(CanKill())
					Kill()

				return SR_HIT
			}
	
			Force = shot.dir * 1000 //shot.Power
			dBodyApplyImpulseAt(this, Force, shot.to)
	
			if(Hacked)
				InterruptHacking(true)
	
			if(!PainSound)
				PainSound = PlaySound(this, g_sCommonSounds[15], SF_ONCE)
	
			return SR_HIT
		}
		
		if(shot.HurtType == HT_EXPLOSION)
		{
//			if(shot.shooter.ClassType == ClassTypeCreature && shot.shooter.ClassNum != ClassNumPlayer)
//				dmg *= World.enemyscale

			float ImpactPower = shot.power * ExplosionImpactScale * 0.005 * sqrt(sqrt(sqrt(1 / g_timescale)))
			vector Org = GetMatrix(this, 3)
			shot.from[2] = Org[2]
			vector ForceDir = Org - shot.from
			float dist = VectorNormalize(ForceDir)
			float scale = dist / shot.radius
			clamp scale<0, 1>
			scale = 1 -scale
			ImpactPower += ImpactPower * scale
			ImpactPower *= 0.3333
	
	/*		if(ImpactPower > 24000)
				ImpactPower = 24000*/
	
			Force = ForceDir * ImpactPower
			Force[2] = 1
			dBodyApplyImpulse(this, Force)
			
//			Print(shot.damage)
			HealthDown(scale * dmg)
			UpdateHealthBar()
			RunHitzoneEffect(shot.from)
	
			if(Health == 0)
			{
				if(CanKill())
					Kill()
				return SR_HIT
			}
			return SR_HIT
		}
		
		if(shot.HurtType == HT_FLAME)
		{
//			if(shot.shooter.ClassType == ClassTypeCreature && shot.shooter.ClassNum != ClassNumPlayer)
//				dmg *= World.enemyscale

			if(!Immortality)
				HealthDown(dmg)
	
			if(CheatImmortality)
				Health = MaxHealth

			UpdateHealthBar()
	
			if(Health == 0)
			{
				if(CanKill())
					Kill()

				return 1
			}
	
			RunHitzoneEffect(shot.from)
			return SR_HIT
		}
		
		if(shot.HurtType == HT_IMPACT)
		{
			if(status < STATE_DEAD)
			{
				if(dmg > 0 && shot.shooter != World.ManipEntity)	//nezranujeme sa predmetom ktry drzime
				{
					if(!Immortality)
						HealthDown(dmg * 10)
	
					if(CheatImmortality)
						Health = MaxHealth

					UpdateHealthBar()
	
					if(Health == 0)
					{
						if(CanKill())
							Kill()

						return SR_HIT
					}
	
					RunHitzoneEffect(shot.from)
	
					if(Hacked)
						InterruptHacking(true)
	
			//		World.Quake(float TimeMax, float Power, float Speed, float FadeInTime, float FadeOutTime, int Priority)
					World.Quake(30, 1, 10, 15, 15, 1)
	
					if(!PainSound)
						PainSound = PlaySound(this, g_sCommonSounds[15], SF_ONCE)
				}
			}
			return 1
		}
		
		if(shot.HurtType == HT_HURT_TRIGGER)
		{
			if(status < STATE_DEAD && dmg > 0)
			{
				if(!Immortality)
					HealthDown(dmg)

				if(CheatImmortality)
					Health = MaxHealth

				UpdateHealthBar()
	
				if(Health == 0)
				{
					if(CanKill())
						Kill()

					return SR_HIT
				}
	
				RunHitzoneEffect(World.CameraPos + World.CameraNorm)
	
				if(Hacked)
					InterruptHacking(true)
	
		//		World.Quake(float TimeMax, float Power, float Speed, float FadeInTime, float FadeOutTime, int Priority)
				World.Quake(30, 1, 10, 15, 15, 1)
	
				if(!PainSound)
					PainSound = PlaySound(this, g_sCommonSounds[15], SF_ONCE)
			}
		}
		return SR_HIT
	}

//-----------------------------------------------------------------
	void EnableContols(bool stat)
	{
		Controls = stat
	}

//-----------------------------------------------------------------
	void StartJump()
	{
		IsOnGround = false
		SetStatus(ST_JUMP)
		JumpUsingKey = true
		vector JumpVec = "0 0 1"
		
		if(SurfaceAngle > PlayerMaxSurfaceAngleForNormalJump)	//nech sa neda vyskakat na nedostupne miesta. hlavne teren
		{
			JumpVec = JumpVec + LegsContactNormal
			VectorNormalize(JumpVec)
		}
		
		JumpVec = JumpVec * PlayerJumpPow
		dBodyApplyImpulseAt(this, JumpVec, GetMatrix(this, 3))
		PlaySound(this, g_sCommonSounds[0], SF_ONCE)
	}

//---------------------------------------------------------------------------------
	bool CreateRigidBody()
	{
		UpdateEntity(this)
		dBodyCreate(this, ZeroVec, BC_NONE)

		geom = dGeomCreateCapsule(PlayerCapsuleRadius, Vector(0, 0, PlayerCapsuleExtends[Crouch]))//		BC_CAPSULE_Z

		dBodyAddGeom(this, geom, g_vIdentMatrix, "material/player")	//g_vIdentMatrix
		dBodySetMass(this, ZeroVec, 80)
		dBodySetInertiaTensor(this, ZeroVec, ZeroVec)
		dBodyInteractionLayer(this, ET_PLAYER)
		dBodyActive(this, true, true)
		SetFlags(this, TFL_FULLMATRIX | TFL_SOLID)
		
		dBodyCreatePhysXShadow(this, 1, PSF_KINEMATIC|PSF_COLLIDE_CLOTH/*|PSF_COLLIDE_RB*/)
			
		SetEventMask(this, EV_SIMULATE)
		return true
	}

//-----------------------------------------------------------------
	void DestroyRigidBody()
	{
		World.ManipEntity = NULL

		if(DampingConstraint1)
		{
			DampingConstraint1.destroy()
			DampingConstraint1 = NULL
		}

		if(ContactConstraint)
		{
			ContactConstraint.destroy()
			ContactConstraint = NULL
		}

		if(VehiclePlayerColBlock)
		{
			dBodyRemoveBlock(VehiclePlayerColBlock)
			VehiclePlayerColBlock = 0
		}

		Projectile cur = FirstProjectile	//
		while(cur)
		{
			cur.OnShooterBodyRemove()
			cur = cur.NextProjectile
		}
		FirstProjectile = NULL

		if(geom)
		{
			dBodyDestroy(this)
			dGeomDestroy(geom)
			geom = 0
		}
	}
/*
//-----------------------------------------------------------------
	int GetMotionAngle()
	{
		int Angle = 0

		if(KeyDown)
		{
			Angle = 180

			if(KeyRight)
				Angle = 225
			if(KeyLeft)
				Angle = 135
		}
		else
		{
			if(KeyUp)
			{
				Angle = 0

				if(KeyRight)
					Angle = 315
				if(KeyLeft)
					Angle = 45
			}
		}

		if(!KeyUp && !KeyDown)
		{
			if(KeyRight)
				Angle = 270
			if(KeyLeft)
				Angle = 90
		}
		return Angle
	}
*/
//-----------------------------------------------------------------
	float DoControl()
	{
		float pow = 0
		float p = 0

		const float kdirs[16] = {
		-1, 270, 0, 315,
		90, -1, 45, -1,
		180, 225, -1, -1,
		135, -1, -1, -1}

		if(g_Joypad)
		{
			float jx = JoyAxis[0]
			float jy = JoyAxis[1]
			p = sqrt(jx * jx + (jy * jy))

			if(p > JOY_DEATH_ZONE)
				pow = p / JOY_MAX_POW

			clamp pow<0, 1>
		}

		if(pow == 0)
		{
			int dir = 0

			if(KEY(RIGHT, false))
				dir = dir + 1

			if(KEY(MOVE_F, false))
				dir = dir + 2

			if(KEY(LEFT, false))
				dir = dir + 4

			if(KEY(MOVE_B, false))
				dir = dir + 8

			RelMotionAng = kdirs[dir]

			if(dir > 0)
				pow = 1
		}
		else
			RelMotionAng = FixAngle(atan2(-jy, jx) * RAD2DEG - 90)

		float ExactAimFactor = 1 - ExactAimPath
		ExactAimFactor = 0.32 + (ExactAimFactor * 0.3)

		cincx = JoyRAxis[2]
		if(!g_Joypad || fabs(cincx) < JOY_DEATH_ZONE)
			cincx = MouseDeltaX * g_fMouseSpeedH * ExactAimFactor * 1 * 15           
		else
			cincx = cincx * 0.3 * g_fMouseSpeedH * ExactAimFactor

		cincy = JoySliders[0]
		if(!g_Joypad || fabs(cincy) < JOY_DEATH_ZONE)
			cincy = MouseDeltaY * g_fMouseSpeedV * ExactAimFactor * 1 * 15
		else
			cincy = cincy * 0.26 * g_fMouseSpeedV * ExactAimFactor

		return pow
	}

//-----------------------------------------------------------------
float LastCamZ
	vector ZDamping(vector tpos)
	{
		if(g_iFly || status == ST_JUMP)
		{
			LastCamZ = tpos[2]
			return tpos
		}

		float d, f = tpos[2]

		if(LastCamZ == 0)
		{
			LastCamZ = f
		}
		else
		{
			d = ftime * 30
			clamp d<0, 1>
			f = f - LastCamZ * d
			LastCamZ = LastCamZ + f
		}

		tpos[2] = LastCamZ

		return tpos
	}

	float LastTrainAngle
	bool HeadOnWall
	
//-----------------------------------------------------------------
	void HandleCamera()
	{
		MovementSpeedScale = DoControl()

		if(HackStatus == 0)	//v pripade hackmodu iba s tymto statusom. inac prebieha process logovania
		{
#ifdef _XBOX
			CameraAngles[0] = CameraAngles[0] + (JoyRAxis[1] * g_fMouseSpeedV * 1 * -0.2)	//uhly pre kameru 
			CameraAngles[1] = CameraAngles[1] - (JoyRAxis[0] * g_fMouseSpeedH * 1 * 0.2)
#else
			float AccelCX = cincx// * 0.2
			float AccelCY = cincy// * 0.2

			if(CurrentWeapon.ClassNum == CSniperRifle)
			{
				float fabsx = fabs(cincx)
				float fabsy = fabs(cincy)
				float rate, rate2
				
				clamp fabsx<0, 2>
				clamp fabsy<0, 2>

				rate = fabsx / 2
				clamp rate<0.1, 1>
				AccelCX *= rate
				
				rate = fabsy / 2
				clamp rate<0.1, 1>
				AccelCY *= rate
					
				AccelCX *= ExactAim
				AccelCY *= ExactAim
				
				float FlipExactAim = 1 - ExactAim
				AccelCX += cincx * FlipExactAim
				AccelCY += cincy * FlipExactAim
			}
	
			CameraAngles[0] = CameraAngles[0] + AccelCY//(MouseDeltaY * g_fMouseSpeedV * 1 * 15)	//uhly pre kameru 
			CameraAngles[1] = CameraAngles[1] - AccelCX//(MouseDeltaX * g_fMouseSpeedH * 1 * 15)
#endif
			CameraAngles[2] = 0
		}

		float ang = CameraAngles[0]
		clamp ang<-LookLimit, LookLimit>
		CameraAngles[0] = ang

		if(TrainEnt)
		{
			vector TrainAngles = Vector2Angles(GetMatrix(TrainEnt, 0))
			float AngOff = GetAngularOffset(TrainAngles[1], LastTrainAngle)
			CameraAngles[1] = CameraAngles[1] + AngOff
			LastTrainAngle = TrainAngles[1]
		}

		float rang = DEG2RAD * (CameraAngles[1] + RelMotionAng)
		MotionVec = Vector(cos(rang) , sin(rang), 0)
		float LeanAngleBefore = LeanAngle
		bool LeanEnabled = true

		if(KEY(LEAN_LEFT, false))
		{
			if(!HeadOnWall)
			{
				LeanAngle -= ftime * 144.162 * sqrt(sqrt(1 / g_timescale))
	
				if(LeanAngle < -PlayerMaxLeanAngle)
					LeanAngle = -PlayerMaxLeanAngle
			}
			else
				LeanEnabled = false
		}
		else
		{
			if(KEY(LEAN_RIGHT, false))
			{
				if(!HeadOnWall)
				{
					LeanAngle += ftime * 144.162 * sqrt(sqrt(1 / g_timescale))
	
					if(LeanAngle > PlayerMaxLeanAngle)
						LeanAngle = PlayerMaxLeanAngle
				}
				else
					LeanEnabled = false
			}
			else
				LeanEnabled = false
		}

		if(!LeanEnabled)
		{
			float tt = LeanAngle * 0.8851 * ftime * 15 * sqrt(sqrt(1 / g_timescale))
			LeanAngle -= tt
		
			if(fabs(LeanAngle) <= 0.01)
				LeanAngle = 0
		}

		HeadOnWall = false
		LeanEnabled = true

		if(LeanAngle == 0)
			LeanEnabled = false

		vector LeanAngles = Vector(0, 0, LeanAngle)
		vector LeanMat[3]
		RollPitchYawMatrix2(LeanAngles + Vector(0, CameraAngles[1], 0), LeanMat)

		float diff = PlayerCapsuleExtends[0] - PlayerCapsuleExtends[1]	//rozdiel medzi vyskami obidvoch polovicnych valcov
		float CrouchOffset = diff * 2 * (1 - fCrouchPath)
		
		if(Crouch)
			CrouchOffset += diff
			
		CrouchOffset -= PlayerCapsuleExtends[1]	//alebo -= diff * 0.5 ?		//CrouchOffset je offset od originu po vrch valca/zaciatok vrchnej pologule

		if(LeanEnabled == true)
		{
			float LeanVOffset = 30	//ohyb bude tolko pod urovnou originu
			float lng = CrouchOffset + PlayerCameraOffset + LeanVOffset
			vector LeanRadiusVec = LeanMat[2] * lng

			CameraTargetPos = GetMatrix(this, 3) - Vector(0, 0, LeanVOffset) + LeanRadiusVec
			vector CameraNormalPos = GetMatrix(this, 3) + Vector(0, 0, CrouchOffset + PlayerCameraOffset)
			
			vector vec = CameraTargetPos - CameraNormalPos
			VectorNormalize(vec)
			vec = vec * 20
			vec = CameraNormalPos + vec

			ClearFlags(this, TFL_SOLID)

			if(!P2PVisibilityEx(CameraNormalPos, vec, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH))
			{
				LeanEnabled = false
				HeadOnWall = true
				
				if(LeanAngleBefore == 0)
					LeanAngle = 0
			}

			SetFlags(this, TFL_SOLID)
		}

		if(LeanAngle == 0)
		{
			CameraTargetPos = GetMatrix(this, 3) + Vector(0, 0, CrouchOffset + PlayerCameraOffset)
			LeanAngles = ZeroVec
			VirtualBodyCenter = GetMatrix(this, 3)
		}
		else
		{
			float LeanAngOffRate = fabs(LeanAngle) / PlayerMaxLeanAngle
			float MaxOffset = PlayerCapsuleRadius - 1 * LeanAngOffRate	//aspon jeden unit musi byt v playerovom body
			vector BoffsetB = g_CamMat[0] * -MaxOffset

			if(LeanAngle < 0)
				BoffsetB = -BoffsetB

			VirtualBodyCenter = GetMatrix(this, 3) + BoffsetB
		}

//		AddDShape(SHAPE_BBOX, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, VirtualBodyCenter - "1 1 1", VirtualBodyCenter + "1 1 1")

		CameraPos = ZDamping(CameraTargetPos)

		vector WeaponSndPos = CameraPos - Vector(0, 0, PlayerCapsuleExtends[Crouch] + PlayerCameraOffset)
		SetOrigin(WeaponSoundPos, WeaponSndPos)

		vector FinalCameraAngles = CameraAngles + EarthquakeAng + WeaponKickOff + WeaponKickOffSmall - LeanAngles

/*		vector LookVec = Angles2Vector(FinalCameraAngles)
		vector MiniCutsceneOffset = LookVec * -128
		CameraPos = CameraPos + MiniCutsceneOffset*/

		
		World.CameraPos = CameraPos + EarthquakeOff
		World.CameraAngs = FinalCameraAngles
		World.CameraNorm = Angles2Vector(World.CameraAngs)
		SetCamera(World.CameraPos, World.CameraAngs)
		
/*		vector CamMat[4]
		RollPitchYawMatrix2(CameraAngles, CamMat)
		CamMat[3] = World.CameraPos
		SetCameraEx(0, CamMat)*/

		CalcMatrixFromCamera()
		
		float MouseFactorX = fabs(cincx)
		float MouseFactorY = fabs(cincy)
		float MouseFactor = MouseFactorX + MouseFactorY
		
		if(MouseFactor > 0.3)
			CameraCrossScale = MouseFactor * 0.5
		else
			CameraCrossScale -= ftime * 10

		clamp CameraCrossScale<0, 1>
	}

	bool TraceFilterCallback(ExtClass other)
	{
		if(other.ClassType == ClassTypeCreature && other.ClassNum == ClassNumPlayer)
			return false

		return true
	}

//-----------------------------------------------------------------
	bool RBTraceFilter(ExtClass other, vector raypos, vector raydir)
	{
		if(other == World)
			return false

		if(other == this)
			return false

		if(other.ClassType == ClassTypeMiscPhysicsGlass)
		{
			if(other.ClassNum == ClassNumMiscPhysicsGlass2)
				return false
		}

		if(other.ClassType == ClassTypeMiscPhysicsGlass || other.ClassType == ClassTypeMiscShootDetector || other.ClassType == ClassTypeMiscShootingShield)
			return other.IsShot(raypos, raydir)

/*		if(IsInherited(other, CCreatureClass))
			return true

		if(!IsInherited(other, CRigidBodyClass))
			return false

		if(IsInherited(other, CDoorClass))
			return false

		if(ClassName(other) == "misc_physics_glass")
			return false*/

		return true
	}

//-----------------------------------------------------------------
	bool RBTraceFilter2(ExtClass other, vector raypos, vector raydir)
	{
		if(other == this)
			return false

		if(other == World.ManipEntity)
			return false

		if(other.ClassType == ClassTypeCreature || other.ClassType == ClassTypeRBone || other.ClassType == ClassTypeTrigger)
			return false

		if(other.ClassType == ClassTypeMiscPhysicsGlass || other.ClassType == ClassTypeMiscShootDetector || other.ClassType == ClassTypeMiscShootingShield)
			return other.IsShot(raypos, raydir)

		return true
	}

	float DistToGround
//-----------------------------------------------------------------
	bool CheckForGround()
	{
		vector pos = coords	//musi sa tu pouzivat coords a nie GetOrigin() aby sa po nahlej zmene pozicie v crouchi vcas akceptovala nova pozicia
		float lng = PlayerCapsuleExtends[Crouch] + 15.5
		vector DirVec = Vector(0, 0, -lng)
		dShapeCastSetDir(ShapeCast, DirVec)
		vector mat4[4] = {"1 0 0", "0 1 0", "0 0 1"}
		mat4[3] = pos
		SetMatrix(ShapeCast, mat4)
//		AddDShape(SHAPE_LINE, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos, pos + Vector(0, 0, -lng))

		MinClass clipent
		vector cpos, cnorm
		float dist = dShapeCastUpdate(ShapeCast, 0, clipent, cpos, cnorm)

		if(clipent && IsInherited(clipent, CMinClass) && clipent.ClassType == ClassTypeFuncTrain)
		{
			TrainEnt = clipent
			vector TrainAngles = Vector2Angles(GetMatrix(TrainEnt, 0))
			LastTrainAngle = TrainAngles[1]
		}
		else
			TrainEnt = NULL

		DistToGround = dist * lng
		DistToGround -= PlayerCapsuleExtends[Crouch]

		if(dist < 1)
		{
			float Plane[4]
			MinClass TracEnt
			vector end = pos - Vector(0, 0, PlayerCapsuleHalfHeights[Crouch] + 10)

			int fl = ClearFlags(this, TFL_SOLID)
			int fl2 = ClearFlags(DummyPlayer, TFL_SOLID)
			
			dist = TraceLineEx(PLAYER, coords, end, ZeroVec, ZeroVec, TracEnt, Plane, NULL, NULL, TRACE_DEFAULT, NULL)

			SetFlags(this, fl)
			SetFlags(DummyPlayer, fl2)

			if(dist < 1)
			{
				vector norm = Vector(Plane[0], Plane[1], Plane[2])
				float SurfDot = norm * Vector(0, 0, 1)
				SurfDot = fabs(SurfDot)	//fabs pretoze obcas vhychadza presne opacna normala. bug? chybne modely s duplicitnymi polygonmi ktore su otocene?
				clamp SurfDot<-1, 1>
				float SurfAng = acos(SurfDot) * RAD2DEG
//				Print(SurfAng)

/*			if(KeyState(KC_K))
			{
			AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, end - "1 1 1", end + "1 1 1")
			AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords - "1 1 1", coords + "1 1 1")
//			AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords, end)
			
			
			vector to = norm * 5
			to = end + to
			AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, end, to)
			}*/

				if(SurfAng > PlayerMaxSurfaceAngle)
					StrongSurfaceBias = true

//				if(g_iTraceSurfaceType == 2)
//					return 2
			}
			else
				StrongSurfaceBias = true

			return true
		}
		return false
	}

//-----------------------------------------------------------------
	bool HandleFlyMode()
	{
		if(KeyState(KC_H) == true)
		{
			ClearKey(KC_H)
			g_iFly = 1 - g_iFly

			if(g_iFly)
			{
				DestroyRigidBody()
				ClearFlags(this, TFL_SOLID|TFL_TOUCHTRIGGERS|TFL_FULLMATRIX)
				SetMoveType(this, NOCLIP)
				SetWeight(this, 0)
			}
			else
			{
				SetFlags(this, TFL_SOLID|TFL_TOUCHTRIGGERS|TFL_FULLMATRIX)
				SetVelocity(this, ZeroVec)
				SetStatus(ST_JUMP)
				CreateRigidBody()

				if(!ContactConstraint)
				{
					ContactConstraint = dConstraintCreateContact()
					ContactConstraint.setBody1(World)
					ContactConstraint.setBody2(this)
				}
				
				if(!DampingConstraint1)
				{
					DampingConstraint1 = dConstraintCreatePoint()
					DampingConstraint1.setBody1(World)
					DampingConstraint1.setBody2(this)
				}
			}
		}

		if(g_iFly)
		{
			LastStablePos = coords
			LastStayPos	= coords
			VectorNormalize(MotionVec)

			if(!AnyMoveKey)
				SetVelocity(this, ZeroVec)
			else
			{
				float spd = MotionSpeed * 2
				float MovementSpeedFromEditor

#ifdef EDITOR
				if(GetValue(World.Editor, "CameraMovementSpeed", 0, MovementSpeedFromEditor)	//ked sme v editore tak sa riadime editorom
					spd = MovementSpeedFromEditor * 100
#else
				spd = g_FlySpeed	//z developer configu
#endif
				if(MousePress(2))
					spd *= 0.1 * sqrt(1 / g_timescale)


				vector CamStep = ZeroVec
				vector mat[3]
				vector angs = World.CameraAngs
				float flip = angs[0]
				angs[0] = -flip
				RollPitchYawMatrix2(angs, mat)
	
				vector LeftVec = mat[1]
				LeftVec = -LeftVec
				vector ForwardVec = mat[0]
	
				if(KEY(RIGHT, false))	//vpravo
					CamStep += LeftVec
	
				if(KEY(LEFT, false))		//vlavo
					CamStep -= LeftVec
	
				if(KEY(MOVE_F, false))	//dopredu
					CamStep += ForwardVec
	
				if(KEY(MOVE_B, false))	//dozadu
					CamStep -= ForwardVec
	
				VectorNormalize(CamStep)
				CamStep = CamStep * spd
				SetVelocity(this, CamStep)
			}
			return true
		}

		return false
	}


/*
#ifdef EDITOR
	void OpenEditor()
	{
		if(Editor.Active == true)
			return

		Editor.Activate(true)
		Editor.SetCam(World.CameraPos, World.CameraAngs)
		ShowHUD(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
		World.Cross.Show(false)
		World.EditorActive = true
	}

	void CloseEditor()
	{
		if(Editor.Active == false)
			return

		Editor.Activate(false)
//		vector vec = Vector(0, 0, PlayerCapsuleExtends[Crouch] + PlayerCameraOffset)

		CameraPos = World.CameraPos
		CameraAngles = World.CameraAngs
		CameraTargetPos = CameraPos
		SetOrientationFromCamera()
		ResetInputDevice()
		ShowHUD(-1, -1, -1, 1, -1, 1, 1, 0, 0)

		if(World.Cross)
			World.Cross.Show(true)

		World.EditorActive = false
	}

	void InitEditor()
	{
		if(!World.Editor)
		{
			World.Editor = new EntityEditor
			Editor = World.Editor
		}
	}
#endif
*/

//-----------------------------------------------------------------
	void UpdateLightBar()
	{
		float BarWidth = 38
//		float BarLeft = 800 - BarWidth - 17

		float rate = FlashLight / FlashLightMaxTime
		float CurWidth = rate * BarWidth
		float CurLeft = BarWidth - CurWidth

		SetWidgetPos(HUD.WLightBar, CurLeft, 0, BarWidth, 20)
	}

//-----------------------------------------------------------------
	void ProcessFlashLight()
	{
		if(KEY(FLASHLIGHT, true))
		{			
			if(!FlashLightSwitch)
			{
				if(FlashLight > 0)	//ak mame energiu
				{
					if(!FlashLightHandle)
						FlashLightHandle = AddLight(FlashLightBase, 0, LFL_DYNAMIC|LFL_SPOT|LFL_FLASHLIGHT, 550, 512, "255 255 255")

					SetLightCone(FlashLightHandle, 52.0)
					SetLightTexture(FlashLightHandle, "textures/flashlight2.dds")
					ShowWidget(HUD.WLightBar, true)
					ShowWidget(HUD.WLightBarBack, true) 
					SetWidgetColor(HUD.WLightBar, 0xffffffff)
					SetWidgetColor(HUD.WLightBarBack, 0x3fffffff)
				}
			}
			else
			{
				if(FlashLightHandle)
				{
					RemoveLight(FlashLightHandle)
					FlashLightHandle = 0
				}
			}
			FlashLightSwitch = 1 - FlashLightSwitch
			PlaySound(NULL, g_sCommonSounds[31], SF_ONCE)
		}

		if(!FlashLightSwitch)	//baterka vypnuta
		{
			if(FlashLight < FlashLightMaxTime)	//ked je potreba tak dobijame
			{
				float speed = FlashLightMaxTime / FlashLightCharging
				FlashLight += ftime * speed

				if(FlashLight >= FlashLightMaxTime)
					FlashLight = FlashLightMaxTime

				UpdateLightBar()
				FlashLightVisTimer = 2
			}
			else	//mame plno
			{
				if(FlashLightVisTimer > 0)
				{
					FlashLightVisTimer -= ftime
					
					if(FlashLightVisTimer <= 1)
					{
						if(FlashLightVisTimer <= 0)
						{
							FlashLightVisTimer = 0
							ShowWidget(HUD.WLightBar, false)
							ShowWidget(HUD.WLightBarBack, false)
						}
						float alp1 = FlashLightVisTimer * 255
						float alp2 = FlashLightVisTimer * 63
						SetWidgetColor(HUD.WLightBar, AWHITE(alp1))
						SetWidgetColor(HUD.WLightBarBack, AWHITE(alp2))
					}
				}
			}
			return
		}
		else	//BATERKA ZAPNUTA
		{
			FlashLight -= ftime
	
			if(FlashLight <= 0)
			{
				FlashLight = 0
	
				if(FlashLightHandle)	//zhasiname
				{
					RemoveLight(FlashLightHandle)
					FlashLightHandle = 0
					FlashLightSwitch = 0
				}
			}
			UpdateLightBar()
		}
	}

//-----------------------------------------------------------------
	void DropManipEntity(bool ChangeWeap)
	{
		ClearFlags(World.ManipEntity, TFL_TRANSLUCENT)
		vector Vel = GetVelocity(World.ManipEntity)
		float lng = VectorLength(Vel)
		float MaxDropSpeed = 301

		if(lng > MaxDropSpeed)
		{
			float lng2 = lng
			clamp lng2<0, MaxDropSpeed>
			float VelScale = lng / MaxDropSpeed
			float VelScale2 = lng2 / MaxDropSpeed
			VelScale = VelScale2 / VelScale
			SetVelocity(World.ManipEntity, Vel * VelScale)
		}
		else
			SetVelocity(World.ManipEntity, -Vel)				//nechapem jak je to mozne ale potrebna opacna velocity

		ClearEventMask(this, EV_SIMULATE)
		
		if(ManipPointConstraint1)
		{
			ManipPointConstraint1.destroy()
			ManipPointConstraint1 = NULL
		}
		
		if(ManipPointConstraint2)
		{
			ManipPointConstraint2.destroy()
			ManipPointConstraint2 = NULL
		}
		
		if(ManipPointConstraint3)
		{
			ManipPointConstraint3.destroy()
			ManipPointConstraint3 = NULL
		}

		if(ManipAngConstraint1)
		{
			ManipAngConstraint1.destroy()
			ManipAngConstraint1 = NULL	
		}

		if(ManipAngConstraint2)
		{
			ManipAngConstraint2.destroy()
			ManipAngConstraint2 = NULL	
		}
		
		if(ManipAngConstraint3)
		{
			ManipAngConstraint3.destroy()
			ManipAngConstraint3 = NULL	
		}

		if(ChangeWeap)
			WeaponChange(WeaponBeforeManip, true)
/*
		if(WeaponChange(WeaponBeforeManip) == false)	//zbran sa pravdepodobne prave meni takze to nemozeme pustit
			NextWeaponToChange = WeaponBeforeManip				//weapon mu teda vrazime az select aktualnej zbrane skonci*/

		if(ManipMass)	//ragdolom mass nemenime takze ani nevraciame
			dBodySetMass(World.ManipEntity, ZeroVec, ManipMass)

		dBodyInteractionLayer(World.ManipEntity, ManipInterLayer)

		if(IsInherited(World.ManipEntity, CRigidBodyClass))
		{
			RigidBodyClass rbc = World.ManipEntity
			rbc.OnPickedPhysicaly(true)				//dame mu vediet ze ho pustame
		}

		World.ManipEntity = NULL
		SetThrowBar(0, false)
		ManipThrowPowerScale = 0
		ManipTime = 0
	}

//-----------------------------------------------------------------
	void ThrowManipEntity()
	{
		if(World.ManipEntity == NULL)
			return

		MinClass ManEnt = World.ManipEntity
		float ThrowScale = ManipThrowPowerScale	//v DropManipEntity(true) sa nuluje
		DropManipEntity(true)
		float EntMass = dBodyGetMass(ManEnt)
		EntMass *= 520
		vector Force = World.CameraNorm * EntMass
		Force = Force * ThrowScale
		vector off = VectorMatrixMultiply3(g_CamMat, ManipOffset)
		off = off * -0.1
		dBodyApplyImpulseAt(ManEnt, Force, GetMatrix(ManEnt, 3) + off)

//odhodeny predmet bude zranovat po urcitu dobu
		if(IsInherited(ManEnt, CRigidBodyClass))
		{
			RigidBodyClass rbc = ManEnt
			rbc.EnableHurting(true, 3.3, ThrowDamage)
		}
	}

//---------------------------------------------------------------------------------
	void EOnSimulate(MinClass other, float dt)
	{
		if(World.ManipEntity && ManipPointConstraint1)
		{
			vector TargetPoint = IdealManipPos
			vector ErrOff = GetOrigin(World.ManipEntity) - TargetPoint
			float ErrDot
			ErrOff = ErrOff * 0.5
			
	//		AddDShape(SHAPE_BBOX, 0xffffffff, SS_NOZUPDATE|SS_TRANSP|SS_ONCE, TargetPoint - "1 1 1", TargetPoint + "1 1 1")
	
			float errscale = 100
			
			if(IsInherited(World.ManipEntity, CRigidBodyClass))
			{
				RigidBodyClass rbc = World.ManipEntity
	
				if(rbc.NumContacts)
					errscale = 15
				
				rbc.NumContacts = 0
			}
			
			if(ManipPointConstraint1)
			{
				ManipPointConstraint1.setPoint(TargetPoint)
				ManipPointConstraint1.setConstraintError(ErrOff[0])
				
				ManipPointConstraint1.setMinimumMagnitude(-errscale)
				ManipPointConstraint1.setMaximumMagnitude(errscale)
				dConstraintInsert(ManipPointConstraint1)
			}
	
			if(ManipPointConstraint2)
			{
				ManipPointConstraint2.setPoint(TargetPoint)
				ManipPointConstraint2.setConstraintError(ErrOff[1])
				
				ManipPointConstraint2.setMinimumMagnitude(-errscale)
				ManipPointConstraint2.setMaximumMagnitude(errscale)
				dConstraintInsert(ManipPointConstraint2)
			}
	
			if(ManipPointConstraint3)
			{
				ManipPointConstraint3.setPoint(TargetPoint)
				ManipPointConstraint3.setConstraintError(ErrOff[2])
				
				ManipPointConstraint3.setMinimumMagnitude(-errscale)
				ManipPointConstraint3.setMaximumMagnitude(errscale)
				dConstraintInsert(ManipPointConstraint3)
			}
	//--
	
		
	
	
			vector mat[4]
			GetMatrix4(World.ManipEntity, mat)
			
			vector mat2[3]
			vector angs = World.CameraAngs
			angs[0] = 0
			RollPitchYawMatrix2(angs, mat2)
			
			vector resmat[3]
			resmat[0] = VectorMatrixMultiply3(mat2, ManipOffMat[0])
			resmat[1] = VectorMatrixMultiply3(mat2, ManipOffMat[1])
			resmat[2] = VectorMatrixMultiply3(mat2, ManipOffMat[2])
	
			vector rot = RotationVectorTo(resmat, mat)
			errscale = 1.0
	
			if(ManipAngConstraint1)
			{
				vector axis = ManipAngConstraint1.getAxis1()
				ErrDot = ManipAngConstraint1.getAxis1() * rot
				ManipAngConstraint1.setPoint(TargetPoint)
				ManipAngConstraint1.setConstraintError(ErrDot)
				dConstraintInsert(ManipAngConstraint1)
			}
	
			if(ManipAngConstraint2)
			{
				ErrDot = ManipAngConstraint2.getAxis1() * rot
				ManipAngConstraint2.setPoint(TargetPoint)
				ManipAngConstraint2.setConstraintError(ErrDot)
				dConstraintInsert(ManipAngConstraint2)
			}
	
			if(ManipAngConstraint3)
			{
				ErrDot = ManipAngConstraint3.getAxis1() * rot
				ManipAngConstraint3.setPoint(TargetPoint)
				ManipAngConstraint3.setConstraintError(ErrDot)
				dConstraintInsert(ManipAngConstraint3)
			}
		}
		
		if(ContactConstraint && SimulatePlane)
		{
			vector CurrentPos = GetOrigin(this)
			ContactConstraint.setAxis("0 0 1")
			ContactConstraint.setPoint(GetOrigin(this) - Vector(0, 0, PlayerCapsuleHalfHeights[Crouch] - 0.5))
			ContactConstraint.setRestitution(0)
			ContactConstraint.setDynamicFriction(1000) 			
			dConstraintInsert(ContactConstraint)
		}
/*
		bool damp = false
		
		if(MovementDumping || TrainEnt)
			damp = true

		vector Vel
		
		if(TrainEnt)
		{
			DampingConstraint1.setBody1(TrainEnt)
//			Vel = GetVelocity(TrainEnt)
			Vel = dBodyGetVelocityAt(TrainEnt, GetOrigin(this))
//			Vel = Vel * 0.5
//			Vel = Vel - GetVelocity(this)
			Vel = Vel * 1500
			Vel = Vel * dt
			dBodyApplyForce(this, Vel)
		}
		else
		{
			Vel = GetVelocity(this)
			DampingConstraint1.setBody1(World)
		}
*/
		if(DampingConstraint1 && MovementDumping)
		{
			vector Vel = GetVelocity(this)
			vector norm = Vel
			VectorNormalize(norm)
			float vel = Vel * norm
			float damping = 2000
			float maxDamp = fabs(damping * vel * dt)
			DampingConstraint1.setPoint(GetOrigin(this))
			DampingConstraint1.setAxis(norm)
			DampingConstraint1.setMinimumMagnitude(-maxDamp)
			DampingConstraint1.setMaximumMagnitude(maxDamp)
			dConstraintInsert(DampingConstraint1)
		}
	}
	
//-----------------------------------------------------------------
	void PickManipEntity(MinClass ent)
	{
		World.ManipEntity = ent

		if(World.ManipEntity == NULL)
			return

		SetFlags(ent, TFL_TRANSLUCENT)

		vector pos1 = GetMatrix(World.ManipEntity, 3)
		vector pos2 = World.CameraPos
		vector vec = pos1 - pos2
		ManipOffset = VectorMatrixInvMultiply3(g_CamMat, vec)
		ManipDist = VectorNormalize(vec)
		IdealManipPos = pos1

		if(!ManipPointConstraint1)
			ManipPointConstraint1 = dConstraintCreatePoint()

		if(!ManipPointConstraint2)
			ManipPointConstraint2 = dConstraintCreatePoint()
			
		if(!ManipPointConstraint3)
			ManipPointConstraint3 = dConstraintCreatePoint()
			
		if(!ManipAngConstraint1)
			ManipAngConstraint1 = dConstraintCreateAngular()

		if(!ManipAngConstraint2)
			ManipAngConstraint2 = dConstraintCreateAngular()
			
		if(!ManipAngConstraint3)
			ManipAngConstraint3 = dConstraintCreateAngular()

		ManipPointConstraint1.setBody1(World)
		ManipPointConstraint1.setBody2(ent)
		ManipPointConstraint2.setBody1(World)
		ManipPointConstraint2.setBody2(ent)
		ManipPointConstraint3.setBody1(World)
		ManipPointConstraint3.setBody2(ent)

		ManipPointConstraint1.setPoint(pos1)
		ManipPointConstraint2.setPoint(pos1)
		ManipPointConstraint3.setPoint(pos1)

		ManipPointConstraint1.setAxis("1 0 0")
		ManipPointConstraint2.setAxis("0 1 0")
		ManipPointConstraint3.setAxis("0 0 1")

		ManipPointConstraint1.setMinimumMagnitude(-3000)
		ManipPointConstraint2.setMinimumMagnitude(-3000)
		ManipPointConstraint3.setMinimumMagnitude(-3000)

		ManipPointConstraint1.setMaximumMagnitude(3000)
		ManipPointConstraint2.setMaximumMagnitude(3000)
		ManipPointConstraint3.setMaximumMagnitude(3000)

//--
		ManipAngConstraint1.setBody1(World)
		ManipAngConstraint1.setBody2(ent)
		ManipAngConstraint2.setBody1(World)
		ManipAngConstraint2.setBody2(ent)
		ManipAngConstraint3.setBody1(World)
		ManipAngConstraint3.setBody2(ent)

		ManipAngConstraint1.setAxis1("1 0 0")
		ManipAngConstraint2.setAxis1("0 1 0")
		ManipAngConstraint3.setAxis1("0 0 1")
		
		ManipAngConstraint1.setAxis2("1 0 0")
		ManipAngConstraint2.setAxis2("0 1 0")
		ManipAngConstraint3.setAxis2("0 0 1")

		SetEventMask(this, EV_SIMULATE)




		vector mins, maxs, size
		GetBoundBox(ent, mins, maxs)
		size = maxs - mins

		vector mat[4]
		vector mat2[3]
		GetMatrix4(World.ManipEntity, mat)
		vector angs = World.CameraAngs
		angs[0] = 0
		RollPitchYawMatrix2(angs, mat2)
		vector mat3[3]
		
		if(size[0] > 35.2 || size[1] > 35.2 || size[2] > 35.2)	//prilis velky objekt (nad 110 cm) na nosenie takze ho nebudeme zarovnavat na osy kamery a nebudeme ani menit vzdialenost od neho aby sa stym aspon trochu dalo manipulovat
		{
			GetMatrix4(World.ManipEntity, mat3)
			ManipIdealDist = VectorLength(GetMatrix(ent, 3) - World.CameraPos) - 1.0
		}
		else	//spocitame maticu pre zarovanie na kameru s tym aby sa objekt pretocil najkratsou cestou na a idealny offset od kamery
		{

			int BestFront, BestFront2
			int BestRight, BestRight2
			int BestTop, BestTop2
			float SizeToPlayer
	
			float dot0, dot1, dot2, dot3, dot4, dot5, bestdot
			vector axis0 = mat2[0]
			vector axis1 = mat2[1]
			vector axis2 = mat2[2]
			
			vector ax0 = mat[0]
			vector ax1 = mat[1]
			vector ax2 = mat[2]
	
			dot0 = ax0 * axis0	//front vec proti vsetkym osam
			dot1 = ax0 * axis1
			dot2 = ax0 * axis2
			dot3 = ax0 * -axis0
			dot4 = ax0 * -axis1
			dot5 = ax0 * -axis2
	
			bestdot = dot0
			BestFront = 0
			BestFront2 = 0
	
			if(dot1 > bestdot)
			{
				bestdot = dot1
				BestFront = 1
				BestFront2 = 0
			}
	
			if(dot2 > bestdot)
			{
				bestdot = dot2
				BestFront = 2
				BestFront2 = 0
			}
	
			if(dot3 > bestdot)
			{
				bestdot = dot3
				BestFront = 0
				BestFront2 = 1
			}
			
			if(dot4 > bestdot)
			{
				bestdot = dot4
				BestFront = 1
				BestFront2 = 1
			}
	
			if(dot5 > bestdot)
			{
				bestdot = dot5
				BestFront = 2
				BestFront2 = 1
			}
	//--
			dot0 = ax1 * axis0	//front vec proti vsetkym osam
			dot1 = ax1 * axis1
			dot2 = ax1 * axis2
			dot3 = ax1 * -axis0
			dot4 = ax1 * -axis1
			dot5 = ax1 * -axis2
	
			bestdot = dot0
			BestRight = 0
			BestRight2 = 0
	
			if(dot1 > bestdot)
			{
				bestdot = dot1
				BestRight = 1
				BestRight2 = 0
			}
	
			if(dot2 > bestdot)
			{
				bestdot = dot2
				BestRight = 2
				BestRight2 = 0
			}
	
			if(dot3 > bestdot)
			{
				bestdot = dot3
				BestRight = 0
				BestRight2 = 1
			}
			
			if(dot4 > bestdot)
			{
				bestdot = dot4
				BestRight = 1
				BestRight2 = 1
			}
	
			if(dot5 > bestdot)
			{
				bestdot = dot5
				BestRight = 2
				BestRight2 = 1
			}
	
			dot0 = ax2 * axis0	//front vec proti vsetkym osam
			dot1 = ax2 * axis1
			dot2 = ax2 * axis2
			dot3 = ax2 * -axis0
			dot4 = ax2 * -axis1
			dot5 = ax2 * -axis2
	
			bestdot = dot0
			BestTop = 0
			BestTop2 = 0
	
			if(dot1 > bestdot)
			{
				bestdot = dot1
				BestTop = 1
				BestTop2 = 0
			}
	
			if(dot2 > bestdot)
			{
				bestdot = dot2
				BestTop = 2
				BestTop2 = 0
			}
	
			if(dot3 > bestdot)
			{
				bestdot = dot3
				BestTop = 0
				BestTop2 = 1
			}
			
			if(dot4 > bestdot)
			{
				bestdot = dot4
				BestTop = 1
				BestTop2 = 1
			}
	
			if(dot5 > bestdot)
			{
				bestdot = dot5
				BestTop = 2
				BestTop2 = 1
			}
	
			vector ResAx0 = mat2[BestFront]
			vector ResAx1 = mat2[BestRight]
			vector ResAx2 = mat2[BestTop]
	
			if(BestFront2)
				ResAx0 = -ResAx0
	
			if(BestRight2)
				ResAx1 = -ResAx1
	
			if(BestTop2)
				ResAx2 = -ResAx2
	
			mat3[0] = ResAx0
			mat3[1] = ResAx1
			mat3[2] = ResAx2

			if(BestFront == 0)
			{
				if(BestFront2 == 0)
					SizeToPlayer = fabs(maxs[0])
				else
					SizeToPlayer = fabs(mins[0])
			}
			else
			{
				if(BestRight == 0)
				{
					if(BestRight2 == 0)
						SizeToPlayer = fabs(maxs[1])
					else
						SizeToPlayer = fabs(mins[1])
				}
				else
				{
					if(BestTop == 0)
					{
						if(BestTop2 == 0)
							SizeToPlayer = fabs(maxs[2])
						else
							SizeToPlayer = fabs(mins[2])
					}
				}
			}

			ManipIdealDist = 20 + (SizeToPlayer * 2)	//cim vacsi objekt beriem do ruk, tym dalej od kamery musi byt aby neprekazal vo vyhlade
		}

		ManipOffMat[0] = VectorMatrixInvMultiply3(mat2, mat3[0])
		ManipOffMat[1] = VectorMatrixInvMultiply3(mat2, mat3[1])
		ManipOffMat[2] = VectorMatrixInvMultiply3(mat2, mat3[2])

		ManipMass = dBodyGetMass(World.ManipEntity)
		
		ManipInterLayer = dBodyGetInteractionLayer(World.ManipEntity)
		dBodyInteractionLayer(World.ManipEntity, ET_MANIPULATING)

		if(IsInherited(World.ManipEntity, CRigidBodyClass))
		{
			RigidBodyClass rbc = World.ManipEntity
			rbc.OnPickedPhysicaly(false)				//dame mu vediet
		}
		
		dBodySetMass(World.ManipEntity, ZeroVec, 0.1)

		dBodyActive(World.ManipEntity, true, true)
		WeaponBeforeManip = CurrentWeapon.ClassNum
		WeaponChange(CDummyWeapon, true)
		ManipTime = 0
	}

	int ManipUseKeyDown
	int ManipUseKeyCleared
	int ManipUseKeyDownLast
	ExtClass ActivatedEntity	//entita ktoru sme naposledy aktivovali
	bool NeedActivateEndEvent

//-----------------------------------------------------------------
	void ProcessInterations()
	{
		World.Cross.SetType(CurrentWeapon.CrossHair)	//default

		if(World.CutScene)
			World.Cross.Show(false)
		else
			World.Cross.Show(true)

		ManipUseKeyDownLast = ManipUseKeyDown
		ManipUseKeyDown = KEY(USE, false)
		int UseKey = ManipUseKeyDown

		if(ManipUseKeyDown)
		{
			if(ManipUseKeyCleared)
				UseKey = false

			ManipUseKeyCleared = true
		}
		else
			ManipUseKeyCleared = false

		if(World.ManipEntity == NULL)	//bez entity rotujeme natvrdo aby pri picknuti entity bola gula presne natocena podla kamery
		{
			vector from = World.CameraPos
			vector to = from
			to += World.CameraNorm * 70//52

			vector end = to
			CreatureClass best = NULL
			int sparm
			float norm[4], cfrac
			cfrac = TraceLineEx(DEFAULT, from, end, ZeroVec, ZeroVec, best, norm, NULL, NULL, TRACE_ENTS, &RBTraceFilter)

			if(cfrac >= 1)
				best = NULL

			CreatureClass ManipAdept = best
			ExtClass ActivateAdept = best

			if(ActivateAdept && ActivateAdept.ActivateMethod <= ACT_NONE)				//zistime ci sa to vobec ma aktivovat. bud user nastavil ACT_NONE alebo je tam ACT_ENTITY_DEFAULT a to znamena ze entita nenastavila nic tak to berieme ako ACT_NONE
				ActivateAdept = NULL

			if(ActivateAdept && ActivateAdept.ActivateMethod == ACT_USEKEY_DISP)		//ak sa to ma aktivovat displejom tak shader musi mat flag SHF_NEEDGUI
			{
				if(TraceLineToEntity(ActivateAdept, from, to, TrcContact))
				{
					if(!TrcContact.ShaderFlags & SHF_NEEDGUI)
						ActivateAdept = NULL
				}
				else
					ActivateAdept = NULL
			}
			
			if(ActivateAdept && ActivateAdept.ActivateMethod == ACT_USEKEY_DISTANCE)	//to co sa ma aktivovat na distance netrasujeme.
				ActivateAdept = NULL

			if(!ActivateAdept && DistActivFavorit)
				ActivateAdept = DistActivFavorit

			if(ActivateAdept && ActivateAdept.ActivateMethod == ACT_DISTANCE_ONLY)
			{
				ActivateAdept = NULL
				ManipAdept = NULL
			}

			if(ActivateAdept)		//nieco sme zamerali
			{
				int CanActiv = ActivateAdept.CanActivate(0, this)

				if(CanActiv == ACT_ENABLE)	//najprv skusame ci sa to neda aktivovat
				{
					World.Cross.SetType(1)		//use icon

					if(UseKey)
					{
						EnterableVehicle vehicle = ActivateAdept.Owner

						if(vehicle && IsInherited(vehicle, CEnterableVehicle))
						{
							Controlled = vehicle
							SetStatus(ST_NOCONTROL)
							VehiclePlayerColBlock = dBodyCollisionBlock(vehicle.BodyPart, this)
//							AddChild(vehicle.BodyPart, this, 0)
							VehicleControlStatus = 1	//blend kamery do auta
							VehicleControlTimer = 0

							vehicle.BeforeEntering()	//nech sa na to pripravi
							vector CameraMatExt[3]
							vector CameraMatIn[3] 
							RollPitchYawMatrix2(CameraAngles, CameraMatExt)
							vehicle.UpdateCameraSystem()
							RollPitchYawMatrix2(EntityAnglesToCameraAngles(vehicle.GetAnglesForCamera()), CameraMatIn)
							MatrixQuat(CameraMatExt, VehicleQuatExt)
							MatrixQuat(CameraMatIn, VehicleQuatIn)
							VehicleCameraBlendOff = vehicle.BodyPart.CoordToLocal(World.CameraPos)
							WeaponBeforeManip = CurrentWeapon.ClassNum
							WeaponChange(CDummyWeapon, true)
							OnEnterVehicle(0)
						}

						ActivatedEntity = ActivateAdept	//musime si zapametat zo sme naposledy aktivovali aby sme tomu vedeli zavolat ActivateProcessEnd()
						ActivateAdept.Activate(0, this)
						CallMethods(ActivateAdept, ActivateAdept.OnSuccess)
					}

					if(ManipUseKeyDown)
					{
						if(ActivatedEntity && ActivatedEntity == ActivateAdept)
						{
							ActivateAdept.ActivateProcess(0, this)
							World.Cross.Show(false)
						}
					}
					
					if(ActivateAdept.ActivateMethod == ACT_USEKEY || ActivateAdept.ActivateMethod == ACT_USEKEY_DISP)
						ManipAdept = NULL		//to co je mozne aktivovat cez USE sa neda zobrat
				}
				else
				{
					if(UseKey)
					{
						ActivateAdept.ActivateFail(0, this)
						CallMethods(ActivateAdept, ActivateAdept.OnFail)
					}

					if(CanActiv == ACT_BLOCKED)
					{
						World.Cross.SetType(2)		//blocked icon
					}

					if(CanActiv == ACT_DISABLE)	//ked sa to neda aktivovat tak to skusime zobrat
					{
						ManipAdept = ActivateAdept
						KEY(USE, true)					//mozno bude niecomu vadit. je to tu koli kyslikovemu pristroju
					}
				}
			}

			if(!ManipUseKeyDown && ManipUseKeyDownLast != 0)	//keyup
			{
				if(ActivatedEntity)	//ak prebiehal Activacny proces, tak teraz ho musime ukoncit
					NeedActivateEndEvent = true
			}

			if(ActivatedEntity)
			{
				if(ActivateAdept != ActivatedEntity)
					NeedActivateEndEvent = true
	
				if(NeedActivateEndEvent)
				{
					ActivatedEntity.ActivateProcessEnd()
					ActivatedEntity = NULL
					NeedActivateEndEvent = false
				}
			}

			if(ManipAdept && !ChangeWeapon)	//skusime ci sa to da nosit
			{
				if(IsInherited(ManipAdept, CRigidBodyClass))
				{
					RigidBodyClass rb = ManipAdept

					if(rb.GetPickable() == false)
						ManipAdept = NULL
				}
				else
					ManipAdept = NULL

				if(ManipAdept)
				{
					World.Cross.SetType(4)		//grab icon

					if(UseKey)
					{
						if(!ChangeWeapon)
							PickManipEntity(ManipAdept)
					}
				}
			}
		}
		else	//s entitiou rotujeme cez angular velocity
		{
			ManipTime += ftime * 1.92 * sqrt(1 / g_timescale)
			dBodyActive(World.ManipEntity, true, true)
			World.Cross.Show(false)

			if(UseKey)
			{
				DropManipEntity(true)
				return
			}

			if(PrimaryAttack)
			{
				ManipThrowPowerScale += ftime * 2.0 * sqrt(sqrt(1 / g_timescale))
				clamp ManipThrowPowerScale<0, 1>
/*				float Tscale = 1 - ManipThrowPowerScale
				Tscale = Tscale * Tscale * Tscale * Tscale * Tscale
				Tscale = 1 - Tscale

				Print(Tscale)
				Print(ManipThrowPowerScale)*/
				SetThrowBar(ManipThrowPowerScale, true)
			}
			else
			{
				if(ManipThrowPowerScale)
				{
					ThrowManipEntity()
					return
				}
			}

			vector ManipDir = ManipOffset
			ManipDist = VectorNormalize(ManipDir)
			
			float diff = ManipIdealDist - ManipDist
			float off = diff * 10 * ftime
			
			float ff = fabs(diff)
			if(fabs(off) > ff)
				off = diff

			ManipDist += off
			ManipOffset = ManipDir * ManipDist

			vector TargOffset = VectorMatrixMultiply3(g_CamMat, ManipOffset)
			vector CurPos = GetMatrix(World.ManipEntity, 3)
			IdealManipPos = World.CameraPos + TargOffset
			float DifLng = VectorLength(IdealManipPos - CurPos)

			if(DifLng > 20 && ManipTime > 0.5)	//hned po zobrani je DifLng vacsia
			{
				DropManipEntity(true)
				return
			}

			MinClass TracEnt = NULL
			cfrac = TraceLineEx(DEFAULT, World.CameraPos, CurPos, ZeroVec, ZeroVec, TracEnt, norm, NULL, NULL, TRACE_ENTS, &RBTraceFilter2)

			if(cfrac < 1 && TracEnt.IsRigidBody() && !dBodyIsDynamic(TracEnt))	//staticke veci nesmu byt medzi. dynamicke ano (granaty v bednicke a pod)
			{
				DropManipEntity(true)
				return
			}
		}
	}

//-----------------------------------------------------------------
	vector GetWalkPos(vector CurrentPos)
	{
		float MaxStairHeight = PlayerMaxStairHeight		//do gameplay.h
		
//		if(Crouch)
//			MaxStairHeight *= 0.5
		
		float tolerance = 0
		vector From = CurrentPos
		vector To = From
		From[2] = From[2] + PlayerCapsuleExtends[Crouch] + MaxStairHeight
		To[2] = To[2] - PlayerCapsuleExtends[Crouch] + tolerance// - MaxStairHeight

		float CylinderHeight = PlayerCapsuleExtends[Crouch] * 2
		float lng = CylinderHeight + MaxStairHeight - tolerance
		vector DirVec = Vector(0, 0, -lng)
		dShapeCastSetDir(ShapeCast, DirVec)
		vector mat[4] = {"1 0 0", "0 1 0", "0 0 1"}
		mat[3] = From
		SetMatrix(ShapeCast, mat)

		MinClass clipent
		vector cpos, cnorm
		float dist = dShapeCastUpdate(ShapeCast, 0, clipent, cpos, cnorm)
		float ResLng = dist * lng
		float crc = lng - MaxStairHeight - 5

		if(ResLng < lng && ResLng > crc)	//je volne miesto pre playera
		{
			float Plane[4]
			MinClass TracEnt
			vector end = CurrentPos - Vector(0, 0, PlayerCapsuleHalfHeights[Crouch] + 10)
	
			dist = TraceLineEx(DEFAULT, CurrentPos, end, ZeroVec, ZeroVec, TracEnt, Plane, NULL, NULL, TRACE_DEFAULT, NULL)
			
			if(dist < 1)
			{
				vector norm = Vector(Plane[0], Plane[1], Plane[2])
				
				float SurfDot = fabs(norm * Vector(0,0,1))	//fabs pretoze obcas vhychadza presne opacna normala. bug? chybne modely s duplicitnymi polygonmi ktore su otocene?
				if(SurfDot > 0.995)	//povrch musi uplne rovny. na sikmych plochach nechceme plyerovi pomahat
				{
					vector EndPos = From - Vector(0, 0, ResLng)
					return EndPos + Vector(0, 0, PlayerCapsuleExtends[Crouch])	//sem mozeme vrazit playera
				}
			}
//								AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_TRANSP, NewPos - "1 1 1", NewPos + "1 1 1")
		}
/*		else
		{
			if(ResLng >= lng)
			{
				float a = ResLng - lng
				Print(a)
			}
			if(ResLng < crc)
			{
				float b = crc - ResLng
				Print(b)
			}
		}*/
		
		return CurrentPos
	}

//-----------------------------------------------------------------
	bool CameraClampFilter(ExtClass other/*, vector raypos, vector raydir*/)
	{
		if(other == this)
			return false

//		if(other.ClassType == ClassTypeMiscPhysicsGlass || other.ClassType == ClassTypeMiscShootDetector || other.ClassType == ClassTypeMiscShootingShield)
//			return other.IsShot(raypos, raydir)

		return true
	}

	bool AfterLoad
//-----------------------------------------------------------------
	void HandleMovement()
	{
		SimulatePlane = false
		StrongSurfaceBias = false
		dBodyActive(this, true, true)
		player_hands hands = Hands
		g_vPlayerPos = GetMatrix(this, 3)
		g_vPlayerAngs = CameraAngles

//---- getting facts ----- START -----
		KeyUp = KEY(MOVE_F, false)
		KeyDown = KEY(MOVE_B, false)
		KeyLeft = KEY(LEFT, false)
		KeyRight = KEY(RIGHT, false)

		if(KeyUp || KeyDown || KeyLeft || KeyRight || MovementSpeedScale > 0.2)
		{
			DummyPlayer.Movement(KeyUp, KeyDown, KeyLeft, KeyRight)
			AnyMoveKey = true
		}
		else
		{
			DummyPlayer.NoMovement()
			AnyMoveKey = false
		}

//		RelMotionAng = GetMotionAngle()	//uhol pre smer pohybu. relativny ku kamere

		vector OldVel = GetVelocity(this)
		vector VelVec = OldVel
		float OldVelLng = VectorNormalize(VelVec)
		MovementSpeedRate = OldVelLng / 189//PlayerWalkSpeed
		clamp MovementSpeedRate<0, 1>
		MovementCrossScale = MovementSpeedRate * 0.405		//maximalna rychlost playera znamena polovicny podiel na nepresnosti strelby

/*		if(CurrentWeapon.ClassNum == CFlameThrower)
		{
			item_weapon_flamethrower FThrower = CurrentWeapon
			if(FThrower.Firing)
			{
				float CameraFactor = fabs(cincx) / 10
				CameraFactor += MovementSpeedRate
				clamp CameraFactor<0, 1>
				float BRateScale = 1 + (CameraFactor * 5)
				FThrower.SetFlameBirthRateScale(BRateScale)
			}
		}*/
/*
		int sparm
		float norm[4], cfrac
		float rang = DEG2RAD * (CameraAngles[1] + RelMotionAng)
		vector LVec = g_CamMat[1]
		vector MyOff = LVec * -200
		vector to = coords + MyOff
		MinClass TracEnt
		cfrac = TraceLineEx(DEFAULT, coords, to, ZeroVec, ZeroVec, TracEnt, norm, NULL, NULL, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH, &CameraClampFilter)
		
		if(cfrac < 1)	//skracujeme kameru
		{
			if(cfrac < 0)
				cfrac = 0
			MyOff = MyOff * cfrac
		}

		AddDShape(SHAPE_LINE, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, coords, to)
		AddDShape(SHAPE_BBOX, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, to - "1 1 1", to + "1 1 1")*/

		bool SprintBefore = Sprint
		bool CrouchBefore = Crouch

		if(!AfterLoad)
		{
			if(KEY(WALK_MODE, false))
				Sprint = true
			else
				Sprint = false
	
			if(KEY(CROUCH, false) && status != ST_JUMP)
				Crouch = true
			else
				Crouch = false
	
			if(CrouchBefore == true && Crouch == false)	//nesmieme zrusit crouch pokial nad hlavou mame strop
			{
				dShapeCastSetDir(ShapeCast, Vector(0, 0, PlayerCapsuleExtends[0] + (PlayerCapsuleExtends[0] - PlayerCapsuleExtends[1])))
				vector mat4[4] = {"1 0 0", "0 1 0", "0 0 1"}
				mat4[3] = GetMatrix(this, 3)
				SetMatrix(ShapeCast, mat4)
	
				MinClass clipent
				vector cpos, cnorm
	
				float dist = dShapeCastUpdate(ShapeCast, 0, clipent, cpos, cnorm)
				if(dist < 1)
				{
					Crouch = true
				}
			}
	
			if(CrouchBefore != Crouch)
			{
				if(geom)	//stava sa ze si niekto zapne FlyMod a potom da crouch a tu to spadne pretoze to nema geom
				{
					dGeomSetSize(geom, Vector(0, 0, PlayerCapsuleExtends[Crouch]), PlayerCapsuleRadius)
					int FlipCrouch = 1 - Crouch
					vector Offset = Vector(0, 0, PlayerCapsuleExtends[FlipCrouch] - PlayerCapsuleExtends[Crouch])
	
					coords = GetMatrix(this, 3) - Offset
					SetOrigin(this, coords)
					UpdateEntity(this)
				}
			}
		}
		
		IsOnGround = CheckForGround()	//musime to zistovat az tu po pripadnom Crouchi aby pozicia playera okamzite brala z novej hodnoty coords a nie GetOrigin()

		AfterLoad = false

		if(Crouch)
		{
			fCrouch += ftime * 5
			
			if(fCrouch > 1)
				fCrouch = 1

			float flip = 1 - fCrouch		//trochu dynamiky z rychla do pomala
			fCrouchPath = flip * flip
			fCrouchPath = 1 - fCrouchPath
		}
		else
		{
			fCrouch -= ftime * 5
			
			if(fCrouch < 0)
				fCrouch = 0
			
			fCrouchPath = fCrouch * fCrouch
		}

		if(status != ST_JUMP && g_iFly == false)	//surovo ho zrazame k zemi
		{
			float zz = DistToGround - 0.7

			if(zz > 0.5)
				FootstepsTimer += 150 * ftime

			if(IsOnGround && zz > 0)
				SetOrigin(this, coords - Vector(0, 0, zz * 0.5))	//nutno brat poziciu z coords a nie z GetOriginu koli nahlej zmene pozicie v crouchi
		}

		if(ExactAim > 0)
		{
			float FlipExactAim = 1 - ExactAim
			float ExactAimBrake = FlipExactAim * 45 * g_timescale * g_timescale * g_timescale

			MotionSpeed = PlayerExactAimSpeed * sqrt(sqrt(1 / g_timescale))
			MotionAccel = PlayerExactAimAccel * sqrt(1 / g_timescale)
		}
		else
		{
			if(Crouch)
			{
				MotionSpeed = PlayerCrouchSpeed * sqrt(1 / g_timescale)// * MovementSpeedScale
				MotionAccel = PlayerCrouchAccel * sqrt(sqrt(1 / g_timescale))// * MovementSpeedScale
				
				if(TrainEnt)
					MotionSpeed = PlayerWalkSpeed * sqrt(sqrt(1 / g_timescale))//
			}
			else
			{
				if(Sprint)
				{
					MotionSpeed = PlayerRunSpeed * sqrt(sqrt(1 / g_timescale))// * MovementSpeedScale
					MotionAccel = PlayerRunAccel * sqrt(1 / g_timescale)// * MovementSpeedScale
				}
				else
				{
					MotionSpeed = PlayerWalkSpeed * sqrt(sqrt(1 / g_timescale))// * MovementSpeedScale
					MotionAccel = PlayerWalkAccel * sqrt(1 / g_timescale)// * MovementSpeedScale
				}
			}
		}

//---- getting facts ----- END -----
#ifdef DEVELOPER
		if(HandleFlyMode())
			return
#endif

		if(Controls == false)
			return

		if(status == ST_NONE)
		{
//			MovementCrossScale = 0.3333333	//ked sa nepohybujeme, nehybeme kamerou a nestrielame tak presnost zbrane bude presne tretina z weapon.PrecisionFlawMax
			
			if(KEY(JUMP, true)/* && !Crouch*/)
				StartJump()
			else
			{
				if(IsOnGround)
				{
					if(AnyMoveKey)
					{
						SetStatus(ST_RUN)
						hands.PlayWalkAnim()
					}
					else
					{
						if(DistToGround < 0.4 && SurfaceAngle > 5)	//ked je na zemi a na sikmej ploche tak simulujeme rovinu pod nim
						{
							if(!StrongSurfaceBias)	//sme na velmi strmej ploche na ktorej sa uz nechceme udrzat. nestaci podmienovat na SurfaceAngle
								SimulatePlane = true
						}

						if(!TrainEnt)
							BrakeScale = 2
					}
				}
				else
					SetStatus(ST_JUMP)
			}
			
			if(status == ST_NONE && ApplyBackForce())
			{
//				float ExtraBrake = 1 - LegsContactDot * 100 * sqrt(sqrt(sqrt(sqrt(g_timescale))))
//				BrakeForce = BrakeForce + (GetVelocity(this) * -ExtraBrake)
				
/*				if(NumLegsContactNormals && OldVelLng < 5)	//ked uz skoro stojime a mame kontakt so spodnou pologulou tak ho surovo drzime na mieste. snad jedine co zabera proti miniaturnemu sklzavaniu :(
				{
					SetVelocity(this, ZeroVec)
					SetOrigin(this, LastStayPos)	//robi problemy minimalne pri crouchi
				}
				else
				{*/
//					if(!TrainEnt)
					if(!StrongSurfaceBias)
						dBodyApplyForce(this, BrakeForce * BrakeScale)

					LastStayPos = GetOrigin(this)
//				}
			}
		}

		if(status == ST_RUN)
		{
//			MovementCrossScale = 0.5

			ApplyBackForce()

			if(KEY(JUMP, true) && !Crouch)
				StartJump()
			else
			{
				if(AnyMoveKey)
				{
					if(IsOnGround)
					{
						vector StairTestOff = MotionVec * ftime * 250
						vector StairTestPos = coords + StairTestOff
						vector HandledPos = GetWalkPos(StairTestPos)

						if(HandledPos[2] > StairTestPos[2])
						{
							FootstepsTimer += 150 * ftime
							StairTestOff = StairTestOff * 0.25
							vector ResultPos = HandledPos
							vector StairTestPos2 = coords + StairTestOff
							vector HandledPos2 = GetWalkPos(StairTestPos2)
							
//								if(HandledPos2[2] > StairTestPos2[2])
//								{
								ResultPos = HandledPos2
								
								if(ResultPos[2] > HandledPos[2])
									ResultPos[2] = HandledPos[2]
//									Print("teraz")
/*								}
							else
								Print("su rovnake")*/

//							AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_TRANSP, ResultPos - "4 4 4", ResultPos + "4 4 4")
							SetOrigin(this, ResultPos)
						}
						else
						{
							vector Poss = GetOrigin(this)
//							AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_TRANSP, Poss - "1 1 1", Poss + "1 1 1")
						}

						float dot, dot3

						//dotykame sa nejakeho body a neni to world. spomalime akcelerovanie podla kontaktov aby sa viac prejavila vaha/trenie bodies pri tlaceni
						if(NumWorldContactNormals > 0)
						{
							if(WorldContactNormal[2] < 0.1 && WorldContactNormal[2] > -0.1)	//ignorujeme kontakty ktore nemaju vodorovnu normalu. (tie zo spodu tu musime vylucit)
							{
								vector Vel = GetVelocity(this)
								VectorNormalize(Vel)
								vector CNorm = WorldContactNormal

								dot = CNorm * Vel
								if(dot < 0)
									dot = -dot

								dot3 = dot * dot * dot
							}
							else
								dot3 = 0
						}
						else
							dot3 = 0

						float a

						if(OldVelLng > MotionSpeed)	//ked sme prilis rychli tak nepridavame
							a = 0
						else
						{
							a = MotionAccel * 0.5	//polovica akceleracnej sily bude vzdy fixna
							float b = a
							b *= 1 - dot3
							a += b								//druha polovica je podla uhlu dotyku s povrchom
						}
						NumWorldContactNormals = 0

						MotionForce = MotionVec * a
						MotionForce = MotionForce + BrakeForce
						
						vector LegsContNorm2D = LegsContactNormal
						LegsContNorm2D[2] = 0
						VectorNormalize(LegsContNorm2D)
						float dotx = MotionVec * LegsContNorm2D
						clamp dotx<-1, 0>
						dotx = 1 + dotx
						dotx = acos(dotx) * RAD2DEG
						dotx = dotx / 90
						dotx = 1 - dotx	//jak strmo do kopca ideme 1 = ideme priamo do kopca 0 = ideme vodorovne po kopci alebo z kopca 
//							Print(dotx)

						float Sang = SurfaceAngle
						clamp Sang<0, PlayerMaxSurfaceAngle>
						float SurfDip = Sang / PlayerMaxSurfaceAngle	//na jak sikmej ploche stojime. percentualne 0 - kam sa uz neda vyliezt
						SurfDip = 1 - SurfDip
//							Print(SurfDip)

						if(SurfDip == 0)
							MotionForce = MotionForce * dotx

						dBodyApplyForce(this, MotionForce)
					}
					else
						SetStatus(ST_JUMP)
				}
				else
				{
					SetStatus(ST_NONE)
					hands.PlayIdleAnim()
				}

				if(!IsOnGround)
				{
					if(OldVel[2] < -50)
						SetStatus(ST_JUMP)
				}
			}
		}

		if(status == ST_JUMP)
		{
//			MovementCrossScale = 0.8

			vector vl = GetVelocity(this)

			if(IsOnGround && DistToGround < 1 && vl[2] <= 0.1)
			{
				float DifLng = LastStablePos[2] - coords[2]

				if(DifLng > PlayerDropDownHurtHeight)	//do 3 metrov sa playerovi nic nestane
				{
					DifLng -= PlayerDropDownHurtHeight
					DifLng /= PlayerDropDownHurtRange	//dalsie 3 metre znamenaju smrt aj z plneho zravia
					float Hurt = DifLng * (float)MaxHealth
					clamp Hurt<0, MaxHealth>

					if(!Immortality)
						HealthDown(Hurt)

					UpdateHealthBar()

					if(Health == 0)
					{
						if(CanKill())	//nemozno vypadnut z funkcie ked sa nezabil
						{
							Kill()
							return
						}
					}
				}

				if(JumpUsingKey)
				{
					PlaySound(this, g_sCommonSounds[0], SF_ONCE)
					FootstepsTimer = 0
					JumpUsingKey = false
					Noises = Noises | NOISE_THUD
				}

				if(AnyMoveKey)
				{
					SetStatus(ST_RUN)
					hands.PlayWalkAnim()
				}
				else
				{
					SetStatus(ST_NONE)
					hands.PlayIdleAnim()
//					SetZVelocity(0)
				}
			}
			else
			{
				if(AnyMoveKey)
				{
					if(OldVelLng < MotionSpeed)
					{
						MotionForce = MotionVec * PlayerJumpControlAccel
						dBodyApplyForce(this, MotionForce)
					}
				}

/*				if(World.ManipEntity)	//aby neslo vypacit playera hore predmetom ktory drzime
				{
					vector ManVel = GetVelocity(World.ManipEntity)

					if(vl[2] > 10 && ManVel[2] < 1 && ManVel[2] > -1)
						DropManipEntity(true)
				}*/

				if(!IsOnGround && vl[2] >= 10)	//updatujeme maximalnu vysku koli zraneniu padom
					LastStablePos = coords
			}
		}
		else	//vsetky statusy mimo jump
		{
			if(GetFlags(this) & TFL_ONGROUND)
				LastStablePos = coords
		}

// ----- motion process ----- END -----
	}
/*
//-----------------------------------------------------------------
	void ChangeNextWeapon()
	{
		if(ChangeWeapon)	//pokial uz neprebieha proces vymeny zbrane
			return

//		if(CurrentWeapon.Action != 0)
//			return

		int n
		int Current = CurrentWeapon.ClassNum
		WeaponClass wc = NULL

		for(n = Current + 1; n < sizeof(PWeapons); n++)
		{
			wc = GetWeaponPointer(n)
			if(wc && wc.ClassNum != CDummyWeapon)
			{
				WeaponChange(wc.ClassNum, true)
				return
			}
		}

		for(n = 0; n < sizeof(PWeapons); n++)	//boli sme na konci tak zacneme znova
		{
			wc = GetWeaponPointer(n)
			if(wc && wc.ClassNum != CDummyWeapon)	//pokial mame iba jednu zbran tam ju nebudeme prehadzovat
			{
				WeaponChange(wc.ClassNum, true)
				return
			}
		}
	}

//-----------------------------------------------------------------
	void ChangePreviousWeapon()
	{
		if(ChangeWeapon)	//pokial uz neprebieha proces vymeny zbrane
			return

//		if(CurrentWeapon.Action != 0)
//			return

		int n
		int Current = CurrentWeapon.ClassNum
		WeaponClass wc = NULL

		for(n = Current - 1; n >= 0; n--)
		{
			wc = GetWeaponPointer(n)
			if(wc && wc.ClassNum != CDummyWeapon)
			{
				WeaponChange(wc.ClassNum, true)
				return
			}
		}

		for(n = sizeof(PWeapons) - 1; n >= 0; n--)	//boli sme na konci tak zacneme znova
		{
			wc = GetWeaponPointer(n)
			if(wc && wc.ClassNum != CDummyWeapon)	//pokial mame iba jednu zbran tam ju nebudeme prehadzovat
			{
				WeaponChange(wc.ClassNum, true)
				return
			}
		}
	}
*/

//-----------------------------------------------------------------
	void ProcessIdleVars()
	{
		if(IdleVarTimer <= 0)
			return

		if(status != ST_NONE)
		{
			EnableIdleWarAnims(false)
			return
		}

		if(ChangeWeapon)	//pocas reloadu to stopneme
			return

		IdleVarTimer -= ftime * 100

		if(IdleVarTimer <= 0)
		{
			EnableIdleWarAnims(true)
			player_hands hands = Hands
			hands.PlayIdleVarAnim()
		}
	}

//-----------------------------------------------------------------
	void ProcessFootsteps()
	{
		if(status == ST_NONE)
		{
			FootstepsTimer -= ftime * 100
			float FootstepIdleTime = PlayerFootstepTime * 0.8

			if(FootstepsTimer < FootstepIdleTime)
				FootstepsTimer = FootstepIdleTime
			return
		}

		bool RealyJump = false

		if(status == ST_JUMP && JumpUsingKey == true)
			RealyJump = true

		bool FootstepsEnabled = false

		if(DistToGround < PlayerCapsuleRadius)
		{
			if(status == ST_RUN || RealyJump == false)
				FootstepsEnabled = true			
		}

		if(FootstepsEnabled)
		{
//			FootstepsTimer += ftime * 100
			float TimeSpeed = 30

			float Vel = VectorLength(GetVelocity(this))
			float SpeedFactor = Vel / PlayerWalkSpeed
			clamp SpeedFactor<0, 1>
//			Print(Vel)

			TimeSpeed += SpeedFactor * 70
			FootstepsTimer += TimeSpeed * ftime

			if(FootstepsTimer >= PlayerFootstepTime)
			{
				FootstepsTimer = 0
				CurrentFootstep = 1 - CurrentFootstep
				int FootStepParm = CurrentFootstep

				if(FootStepParm == 0)
					FootStepParm = -1

				if(!Sprint/* && SpeedFactor > 0.5 && !Crouch*/)
					atype = t_run
				else
					atype = t_walk

				Footstep(FootStepParm)

				if(!Sprint)
					Noises = Noises | NOISE_FOOTSTEP
			}
			return
		}
	}

//int lastframe
//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		vector Pos = GetOrigin(this)
		float MaxContactZ = Pos[2] - PlayerCapsuleExtends[Crouch] - 0.2

		if(c.Position[2] < MaxContactZ)
		{
			LegsContactNormal = LegsContactNormal + c.Normal
			NumLegsContactNormals++
		}
		
		if(other == World)
			return

	//if(dBodyIsDynamic(other))
/*		if(other.ClassType == ClassTypeFuncTrain)
		{
			if(c.Normal[2] > 0.7) //to bude podlaha
			{
				vector vel = dBodyGetVelocityAt(other, c.Position)
				vel[2] = 0

				vector vel2 = GetVelocity(this)
				vel2[2] = 0
				
				VectorNormalize(vel)
				VectorNormalize(vel2)
				
				BrakeScale = vel * vel2
				BrakeScale = 1 - BrakeScale
				
				//some treshold
				if(fabs(BrakeScale) < 0.01)
					BrakeScale = 0

				clamp BrakeScale<BrakeScale, 1.0>
				TrainEnt = true
				TrainEnt = other
			}
		}		*/

		if(!dBodyIsDynamic(other))
					return

/*
		int diff = g_iViewFrame - lastframe
		lastframe = g_iViewFrame
		if(diff > 1)
		 Print(diff)

		vector pos3 = GetMatrix(World.Player, 3)
		pos3 += World.CameraNorm * 50
		pos3[2] = c.Position[2]

		vector vv = c.Normal * 100
		vector xx = pos3 + c.Position

		if(c.Normal[2] < 0.7)
			AddDShape(SHAPE_LINE, 0xffffffff, 0, pos3, pos3 + vv)
		else
			AddDShape(SHAPE_LINE, 0xffffffff, SS_ONCE, pos3, pos3 + vv)
*/

		WorldContactNormal = WorldContactNormal + c.Normal
		NumWorldContactNormals++
	}

//-----------------------------------------------------------------
	float GetBulletTime()
	{
		return BulletTime
	}

//-----------------------------------------------------------------
	void ProcessBulletTime()
	{
		if(!World.CutScene && !MiniCutsceneStatus && KEY(BULLET_TIME, true))
		{
			bool HackBlock = false
			
			if(HackMode && HackStatus != 0)	//z hacking modu mozeme zapnut bullet time iba v tomto stave
				HackBlock = true
			
			if(!HackBlock && BulletTime > 0)	//ak nejaky vobec mame
				SwitchBulletTimeMode()			//vypina pripadny hacking
		}

		if(World.BulletTimeFade == BulletTimeFadeTarget && BulletTimeFadeTarget == 0)
			return

		float revscale = 1 / g_timescale
		BulletTime -= ftime * revscale * 1.3

                

		if(BulletTime <= 0 && BulletTimeFadeTarget == 1)
		{
			BulletTime = 0
			BulletTimeFadeTarget = 0	//vypiname to
			SetWidgetImage(HUD.WTimeBar, 0)
			BulletTimeFadeOutSound = PlaySound(this, g_sCommonSounds[19], SF_ONCE)	//dobeh
		}

		UpdateBulletTimeBar()

		float FadeSpeed

		if(BulletTimeFadeTarget == 1)
			FadeSpeed = ftime * revscale// * 0.59
		else
			FadeSpeed = -ftime * revscale// * 0.76//0.5

		LoopTimeCounter += ftime

		if(!BulletTimeLoopSound && LoopTimeCounter > 0.2)
			BulletTimeLoopSound = PlaySound(this, g_sCommonSounds[18], 0)			//loop

		World.BulletTimeFade += FadeSpeed
		clamp World.BulletTimeFade<0, 1>

		float CurBulletTimeScale = 1 - BulletTimeScale * World.BulletTimeFade
		g_timescale = 1 - CurBulletTimeScale

//		SetPostEffect(3, 255, 0, World.BulletTimeFade, 0.0)
//		SetPostEffect(5, World.BulletTimeFade * 0.005, World.BulletTimeFade * 0.0 + 0.125, World.BulletTimeFade * 0.0 + 0.125, World.BulletTimeFade * 0.2 + 0.125)		//modra
		SetPostEffect(5, World.BulletTimeFade * 0.005, World.BulletTimeFade * 0.25 + 0.125, World.BulletTimeFade * 0.05 + 0.125, World.BulletTimeFade * 0.0 + 0.125)	//cervena
		float EaxFade = World.BulletTimeFade * 100
		SetEAXEnvironment("$current", "Bullet", EaxFade)	//Drugged

		if(BulletTimeLoopSound)
			SetSoundVolume(BulletTimeLoopSound, LinearVol(World.BulletTimeFade))

		if(BulletTimeFadeTarget == 0 && World.BulletTimeFade == 0)
		{
			if(BulletTimeLoopSound)
			{
				EndSound(BulletTimeLoopSound)
				BulletTimeLoopSound = 0
			}
			SetPostEffect(0, 0, 0, 0, 0)
		}
               

	
	}


//-----------------------------------------------------------------
	void RemoveInteractionAdept(ExtClass adept)
	{
		if(adept == DistActivFavorit)
			DistActivFavorit = NULL

		for(int n = 0; n < InteractionAdeptsNum; n++)
		{
			if(InteractionAdepts[n] == adept)
			{
				int Last = InteractionAdeptsNum - 1
				if(n < Last)
				{
					for(int k = n; k < Last; k++)
						InteractionAdepts[k] = InteractionAdepts[k + 1]
				}
				InteractionAdeptsNum--
				return 
			}
		}
	}

//-----------------------------------------------------------------
	ExtClass IntTraceEnt

	bool InteractionFilter(ExtClass other, vector raypos, vector raydir)
	{
		if(other == this)
			return false

		if(other == IntTraceEnt)
			return false

		if(other.ClassType == ClassTypeMiscPhysicsGlass || other.ClassType == ClassTypeMiscShootDetector || other.ClassType == ClassTypeMiscShootingShield)
			return other.IsShot(raypos, raydir)

		return true
	}

//-----------------------------------------------------------------
	void ProcessDistInteractions()
	{
		ExtClass adept
		float dist, bestdist = 9999999
		vector pos = GetMatrix(this, 3)
		vector AddeptPos
		DistActivFavorit = NULL

		for(int n = 0; n < InteractionAdeptsNum; n++)
		{
			adept = InteractionAdepts[n]
			AddeptPos = GetMatrix(adept, 3)
			dist = VectorLength(pos - AddeptPos)

			if(dist <= adept.ActivateDist)
			{
				if(adept.ActivateMethod == ACT_DISTANCE_ONLY)	//ked sa to ma aktivovat iba ak je vzdialenost mensia nez ActivateDist
				{
					if(adept.CanActivate(0, this) == ACT_ENABLE)
					{
						vector to = GetRandPosInAABB(adept)	//musime odtrasovat visibilitu ale z jedneho miesta by nezafungovalo vzdy
						CreatureClass TracTarg = NULL
						int sparm
						float norm[4], cfrac
						IntTraceEnt = adept
						cfrac = TraceLineEx(DEFAULT, coords, to, ZeroVec, ZeroVec, TracTarg, norm, NULL, NULL, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH|TRACE_BONES, &InteractionFilter)

						if(cfrac == 1)
							adept.Activate(0, this)
					}
				}
				else
				{
					if(adept.ActivateMethod == ACT_USEKEY_DISTANCE)	//z tych ktore sa aktivuju pomocou klavesy USE v definovanom ActivateDist vyberieme najblizsieho adepta
					{
						if(dist < bestdist)
						{
							DistActivFavorit = adept
							bestdist = dist
						}
					}	
				}
			}
		}
		InteractionAdeptsNum = 0
	}

//-----------------------------------------------------------------
	void ProcessCheats()
	{
		if(CheatWeapons)
		{
			ExtClass tmp = new ExtClass()
			tmp._name = "CheatTempClass"

			if(CheatWeapons)
			{
				tmp._value = "CPistol"
				M_AddItem(tmp)

				tmp._value = "CShotgun"
				M_AddItem(tmp)

				tmp._value = "CMachinegun"
				M_AddItem(tmp)

				tmp._value = "CSniperRifle"
				M_AddItem(tmp)

				tmp._value = "CGrenade"
				M_AddItem(tmp)
				
				tmp._value = "CRocketLauncher"
				M_AddItem(tmp)
				
				tmp._value = "CFlameThrower"
				M_AddItem(tmp)
			}

			delete tmp
			CheatWeapons = false
		}

		if(CheatAmmo)
		{
			WeaponClass wc
			int n
			for(n = 0; n < sizeof(PWeapons); n++)
			{
				wc = PWeapons[n]
				if(wc)
					wc.Ammo = wc.AmmoPerMagazine
			}

			for(n = 0; n < sizeof(Ammo); n++)
				Ammo[n] = 100000

			UpdateAmmoBar()
			CheatAmmo = false
		}

		if(CheatImmortality)
		{
			Health = MaxHealth
			UpdateHealthBar()
		}
		
		if(CheatHealth)
		{
			Health = MaxHealth
			UpdateHealthBar()
			CheatHealth = false
		}

		if(!CheatHubbardium && status == ST_NONE && g_timescale == 1 && status != ST_DEATH)
		float BulletTimeNeed = BulletTimeMax / 12.45


		{       
                        if(BulletTime < BulletTimeNeed && LeanAngle == 0 )
                       {
                        if(Crouch)
                        {
                        BulletTime += ftime * 0.969
			clamp BulletTime<0, BulletTimeNeed>
			UpdateBulletTimeBar()
			return
                        }
                        else
                        {
                        BulletTime += ftime * 0.125
			clamp BulletTime<0, BulletTimeNeed>
			UpdateBulletTimeBar()
			return
                        }
                       }
                        if(BulletTime < BulletTimeNeed && LeanAngle != 0 )
                       {
                        BulletTime += ftime * 0.849
			clamp BulletTime<0, BulletTimeNeed>
			UpdateBulletTimeBar()
			return
                        }
                        else
                       {
                        clamp BulletTime<0, BulletTimeMax>
			UpdateBulletTimeBar()
			return
                       }
           
		
}
		
	}
	
//-----------------------------------------------------------------
	void ProcessHealth()
	{
		float rate = Health / MaxHealth

		if(rate <= 0.25)				//ked mame malo energie tak blikame vzdy
			HealthFlashReason = 2
		else								//ak mame dost tak blikame jedine z dovodu ze sme dostali zasah
		{
			if(HealthFlashReason == 2)
				HealthFlashReason = 0
			
			HearthSoundNeedStop = true	//musime nechat dohrat zvuk a za potom ho vypneme
		}

		float FlSpeed = 5 * ftime

		if(HealthFlashRate > 0.5)
			FlSpeed = 0.5 * ftime

		HealthFlashRate += FlSpeed
		clamp HealthFlashRate<0, 1>
		float FinalFlashScale = sin(HealthFlashRate * 180 * DEG2RAD)
		float apl = FinalFlashScale * 255
		SetWidgetColor(HUD.WHealthBarFlash, ARGB(apl,255,255,255))

		if(HealthFlashRate == 1 && HealthFlashReason)
		{
			HealthFlashRate = 0
			HealthFlashReason = 0
		}
	}

//-----------------------------------------------------------------
	void M_OxygenOn(ExtClass other)
	{
		OxygenAtmosphere = true
		UpdateOxygenBar()
	}

//-----------------------------------------------------------------
	void M_OxygenOff(ExtClass other)
	{
		OxygenAtmosphere = false
		ShowHUD(-1, -1, -1, -1, -1, -1, -1, -1, 1, -1)
		UpdateOxygenBar()
		
		if(!OxygenRespirationSnd)
			OxygenRespirationSnd = PlaySound(this, g_sCommonSounds[40 + OxygenRespSndIndex], SF_ONCE | SF_AMBIENT)
	}

//-----------------------------------------------------------------
	void M_HackingOn(ExtClass other)
	{
		HackingDisabled = false
		ShowHUD(-1, -1, -1, -1, -1, -1, -1, -1, -1, 1)
	}

//-----------------------------------------------------------------
	void M_HackingOff(ExtClass other)
	{
		HackingDisabled = true
		ShowHUD(-1, -1, -1, -1, -1, -1, -1, -1, -1, 0)
	}

//-----------------------------------------------------------------
	void ProcessOxygen()
	{
		if(OxygenAdding)	//ked prave cerpame tak toto je mimo provoz
		{
			OxygenAdding = false
			return
		}

		if(OxygenAtmosphere)
		{
			if(oxygen < PlayerMaxOxygen)
			{
				oxygen += ftime * 20
				clamp oxygen<0, PlayerMaxOxygen>
				OxygenPainCounter = 0
				UpdateOxygenBar()

				if(oxygen == PlayerMaxOxygen)
					ShowHUD(-1, -1, -1, -1, -1, -1, -1, -1, 0, -1)
			}
		}
		else
		{
			if(OxygenDownTimer > 0)	//po cerpani klesame az po chvili aby sa pri drzani USE neustale nezapinalo a vypinalo cerpanie
			{
				OxygenDownTimer -= ftime
				
				if(OxygenDownTimer <= 0)
					OxygenDownTimer = 0	
			}
			else
			{
				world wrl = World
				oxygen -= ftime * OxygenDownSpeeds[wrl.AtmosphereToxicity]
				clamp oxygen<0, PlayerMaxOxygen>
	
				if(oxygen == 0)
				{
					OxygenPainCounter += ftime
	
					if(OxygenPainCounter >= NoOxygenHurtTime)	//kazdy sekundu to s nim sekne
					{
						OxygenPainCounter = OxygenPainCounter - NoOxygenHurtTime
						HealthDown(NoOxygenHealthDown)
						UpdateHealthBar()
	
						if(Health == 0)
						{
							if(CanKill())
								Kill()
						}
					}
				}
				UpdateOxygenBar()
			}
		}
	}

//	func_linear_door test4

	//widget HackView
	
	
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		player_hands hands = Hands
		 
/*		if(KeyState(KC_I))
		{
			ClearKey(KC_I)
			_value = "1600, 60"
			World.M_Flash(this)
//			World.FullscreenFlash(1600, 60, "255 255 255", 0, 0.5)
		}*/

		/*
		if(KeyState(KC_I))
		{
			vector to = World.CameraNorm * 256 + World.CameraPos
			float plane[4]
			
			ExtClass target
			if(TraceLineEx(DEFAULT, World.CameraPos, to, ZeroVec, ZeroVec, target, plane, NULL, NULL, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH|TRACE_BONES, NULL) < 1)
			{
				vector project
				
				if(other == World)
					project = -Vector(plane[0], plane[1], plane[2])
				else
					project = World.CameraNorm
				
				int shdr
				
				if(shdr == NULL)
					shdr = GetShader("decals/decalflesh")
				CreateDecal(target, to, project * 16, 16.0, 0, 8, shdr, 0.1, 61.0)
	//			CreateDecal(target, to, project * 8, 8.0, 0, 8, BloodDecalShaders[1], 0.1, 61.0)
			}
		}
		*/

/*		float Plane[4]
		vector From = World.CameraPos - "0 0 8"
		vector end = World.CameraNorm * 100
		end = From + end
		
//		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, From, end) 
		From = end
		end = end - Vector(0,0,PlayerCapsuleHalfHeights[Crouch] + 10)
		MinClass TracEntX
		float dist = TraceLineEx(DEFAULT, From, end, ZeroVec, ZeroVec, TracEntX, Plane, NULL, NULL, TRACE_DEFAULT, NULL)
		
		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, From, end)
		AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, end - "1 1 1", end + "1 1 1")

		if(dist < 1)
		{
			vector normx = Vector(Plane[0], Plane[1], Plane[2])
			Print(normx)
			
			vector vecx = normx * 50
			vecx = end + vecx
			AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, end, vecx)

			float SurfDot = fabs(normx * Vector(0,0,1))	//fabs pretoze obcas vhychadza presne opacna normala. bug? chybne modely s duplicitnymi polygonmi ktore su otocene?
			clamp SurfDot<-1, 1>
			float SurfAng = acos(SurfDot) * RAD2DEG
//				Print(SurfDot)
//				Print(SurfAng)
		}*/

		VectorNormalize(LegsContactNormal)	//priemer contact normal za jeden simulacny krok
		LegsContactDot = LegsContactNormal * Vector(0, 0, 1)
		SurfaceAngle = acos(LegsContactDot) * RAD2DEG

		//SetWidgetWorld(HUD.HackView, globalmap, 1)

		if(KeyState(KC_SUBTRACT))
		{
			r_bumpbias += ftime * -0.02
			clamp r_bumpbias<0, 0.1>
		}

		if(KeyState(KC_ADD))
		{
			r_bumpbias += ftime * 0.02
			clamp r_bumpbias<0, 0.1>
		}

		//kvuli AIcku, at nemusi furt volat GetOrigin()
		coords = GetMatrix(this, 3)

		Noises = 0
		VectorNormalize(WorldContactNormal)
		
		if(status == ST_DEATH)
		{
			DeathCounter -= ftime

			if(DeathMode == DEATHMODE_IN_VEHICLE)	//kamerou pomaly stupame hore a cumime na playera
			{
				EnterableVehicle vehicle = Controlled
				vector TargetAngles = Vector2Angles(GetMatrix(vehicle.BodyPart, 3) - World.CameraPos) 
				float flip2 = TargetAngles[0]
				TargetAngles[0] = -flip2
				TargetAngles[0] = FixAngle(TargetAngles[0])

				vector ags = World.CameraAngs
				ags[0] = FixAngle(ags[0])
				ags[1] = FixAngle(ags[1])

				float AngOff0 = GetAngularOffset(TargetAngles[0], ags[0])
				float AngOff1 = GetAngularOffset(TargetAngles[1], ags[1])
				World.CameraAngs[0] = ags[0] + (AngOff0 * ftime * 10)			//"uklon" kamery postupne vyrovname
				World.CameraAngs[1] = ags[1] + (AngOff1 * ftime * 10)
				
				World.CameraPos[2] = World.CameraPos[2] + (ftime * 50)
			}
			else	//DEATHMODE_NORMAL
			{
				World.CameraPos[2] = World.CameraPos[2] - (ftime * 100)

				if(World.CameraPos[2] <= DeathCameraTarget[2])
					World.CameraPos[2] = DeathCameraTarget[2]
			}

			SetCamera(World.CameraPos, World.CameraAngs)

			if(DeathCounter <= 0)
			{
				DeathCounter = 0
				ClearEventMask(this, EV_ALL)
				World.change_level("Escape=Menu LMB=Restart", false)
				return
			}
			return
		}
//Zaciatok OnFrame()

#ifdef EDITOR
		if(World.EditorActive)
			return
#endif

		ProcessCheats()

/*		if(KeyState(KC_K))
		{
			ClearKey(KC_K)
			vector Forward = g_CamMat[1]
			Forward = -Forward
			vector Left = g_CamMat[0]
			vector Right = -Left
			vector Center = Forward * 60
			Center = GetMatrix(this, 3) + Center

			vector pos1 = Center + (Left * 30)
			vector pos2 = Center + (Right * 30)

			misc_physics_model bedna1 = new misc_physics_model()
			bedna1.coords = pos1
			bedna1.angle = 45 * sqrt(g_timescale)
			bedna1.mass = 39.8 * sqrt(g_timescale)
			bedna1.model = "obj/physics/boxik.xob"
			Throw(bedna1, World, EV_INIT, 0)
			Throw(bedna1, World, EV_LOADED, 0)

			misc_physics_model bedna2 = new misc_physics_model()
			bedna2.coords = pos2
			bedna2.angle = 45 * sqrt(g_timescale)
			bedna2.mass = 36.9 * sqrt(g_timescale)
			bedna2.model = "obj/physics/boxik.xob"
			Throw(bedna2, World, EV_INIT, 0)
			Throw(bedna2, World, EV_LOADED, 0)

			misc_anim_object child1 = new misc_anim_object
			child1.coords = ZeroVec
			child1.model = "obj/physics/cigarettes.xob"
			Throw(child1, World, EV_INIT, 0)
			Throw(child1, World, EV_LOADED, 0)
			SetOrigin(child1, "20 0 25")
			SetAngles(child1, "20 20 20")
			AddChild(bedna1, child1, 0)

			misc_physics_model child2 = new misc_physics_model
			child2.coords = ZeroVec
			child2.model = "obj/physics/cigarettes.xob"
			Throw(child2, World, EV_INIT, 0)
			Throw(child2, World, EV_LOADED, 0)
//			child2.SetTransformEx("0 0 0", pos2 + "0 0 20")
			AddChild(bedna2, child2, 0)
			ClearFlags(child2, TFL_FULLMATRIX)
			SetOrigin(child2, "20 0 25")
			SetAngles(child2, "20 20 20")
		}*/

//niektore veci musia bezat bez ohladu na cokolvek!
		ProcessWeaponSelect()	//prebieha selekcia
		ProcessHitZones()
		ProcessCrosshair()
		ProcessOxygen()
		ProcessBulletTime()

		if(MiniCutsceneStatus)
		{
			DummyPlayer.SetOrientation(coords, CameraAngles[1] + 90)

			if(MiniCutsceneStatus == 1)	//cakame az sa player ukludni. mohol by byt prave v skoku a podobne. zaroven cakame az odlozi zbran
			{
				if(status == ST_DEATH)		//pocas toho nas zabili takze uz nic nebude
					MiniCutsceneStatus = 0
				else
				{
					if(Hands && CurrentWeapon.CanUpdateOwnerAnim())	//musime updatovat ruky aby prebehol unselect/select na ktory cakame
						BumpFrame(Hands, ftime, 0)

					if(status != ST_JUMP && !ChangeWeapon)		//player je v klude tak nastupuje minicutscena
					{
						SetStatus(ST_NOCONTROL)
						DummyPlayer.Transmission()
						dBodyActive(this, false, false)
						SetVelocity(this, ZeroVec)
						SetRenderViewMask(DummyPlayer, 1)
						CurrentWeapon.Show(false)
						hands.Show(false)
						World.Cross.Show(false)
						HearthSoundNeedStop = true

						if(HackMode)						//vypiname recon ked je zapnuty
							SwitchHackMode() 
						
						if(BulletTimeFadeTarget == 1)	//vypiname bullettime ked je zapnuty
							SwitchBulletTimeMode()

						MiniCutsceneCameraScale = 0
						MiniCutsceneStatus = 2
					}
					else
					{
						if(status == ST_JUMP)	//nez player doskoci tak vo vzduchu sa este moze hybat a otacat kamerou
						{
							dBodyActive(this, true, true)
							HandleMovement()
							HandleCamera()
						}
						else	//inak uz ziadny pohyb
						{
							dBodyActive(this, false, false)
							SetVelocity(this, ZeroVec)
						}
					}
				}
			}

			if(MiniCutsceneStatus == 2)	//kamera sa vzdialuje
			{
				if(KeyState(KC_ESCAPE)/* || KEY(USE, true)*/)	//skor nez zacnu hrat dialogy sa to bude dat skipnut
				{
					ClearKey(KC_ESCAPE)
					World.Transmitter.Stop()	//musime vymazat buffer s dialogami ktore tam su
					MiniCutsceneStatus = 4
					CameraAngles[0] = -20	//kameru vraciame do polohy aby pozeral rovno
				}
				else
				{
					MiniCutsceneCameraScale += ftime// * 5
	
					if(MiniCutsceneCameraScale > 1)
					{
						MiniCutsceneCameraScale = 1
						MiniCutsceneStatus = 3
						World.Transmitter.Play(SP_MINICUTSCENE)	//az teraz spustime davku ingame hlasok. tento proces zacal prave na zaklade toho ze tam nejake su
					}
					
					float sflip = 1 - MiniCutsceneCameraScale		//trochu dynamiky z rychla do pomala
					MiniCutsceneCameraScaleDyn = sflip * sflip
					MiniCutsceneCameraScaleDyn = 1 - MiniCutsceneCameraScaleDyn
				}
			}
			
			if(MiniCutsceneStatus == 3)	//pocuvame/sledujeme dialogy
			{
				if(KeyState(KC_ESCAPE) || /*KEY(USE, true) ||*/ World.Transmitter.PlayMode == SP_NONE)	//ak to prerusime alebo dialogy dohrali
				{
					ClearKey(KC_ESCAPE)
					World.Transmitter.Stop()
					MiniCutsceneStatus = 4
					CameraAngles[0] = -20	//kameru vraciame do polohy aby pozeral rovno
				}
			}

			if(MiniCutsceneStatus == 4)	//koncime s tym takze stiahneme kameru a  playera nechame pokracovat
			{
				MiniCutsceneCameraScale -= ftime * 3

				if(MiniCutsceneCameraScale <= 0)
				{
					MiniCutsceneCameraScale = 0
					MiniCutsceneStatus = 0
					WeaponChange(WeaponBeforeMiniCutscene, true)
					WeaponBeforeMiniCutscene = 0
					CurrentWeapon.Show(true)
					SetRenderViewMask(DummyPlayer, 0xfffe)
					hands.Show(true)
					HealthDown(0)	//nech sa pusti zvuk srdca ked je potreba lebo predtym bol vypnuty
					SetVelocity(this, ZeroVec)
					dBodyActive(this, true, true)
					SetStatus(ST_NONE)
				}

/*				float sflip2 = 1 - MiniCutsceneCameraScale		//trochu dynamiky z rychla do pomala
				MiniCutsceneCameraScaleDyn = sflip2 * sflip2
				MiniCutsceneCameraScaleDyn = 1 - MiniCutsceneCameraScaleDyn*/
				MiniCutsceneCameraScaleDyn = MiniCutsceneCameraScale * MiniCutsceneCameraScale
								
				if(MiniCutsceneCameraScaleDyn < 0.1)
					SetRenderViewMask(DummyPlayer, 0xfffe)
			}
			
			if(MiniCutsceneStatus >= 2)
			{
				vector LookVec = World.CameraNorm
				LookVec[2] = 0
				VectorNormalize(LookVec)
				LookVec[2] = -0.5
				VectorNormalize(LookVec)

				vector ang = Vector2Angles(-LookVec)
				if(CameraAngles[0] < 0)
				{
					ang[0] = 360 - ang[0]
					float flip = ang[0]
					ang[0] = -flip
				}
				
				float AngOff = GetAngularOffset(CameraAngles[0], ang[0])
				vector MiniCutsceneAngs = CameraAngles
				MiniCutsceneAngs[0] = MiniCutsceneAngs[0] - (AngOff * MiniCutsceneCameraScaleDyn)

				float OffsetLng = 64 * -MiniCutsceneCameraScaleDyn
				vector CameraOffset = LookVec * OffsetLng
				
				int sparm
				float norm[4], cfrac
				vector to = CameraPos + CameraOffset
				MinClass TracEnt
				cfrac = TraceLineEx(DEFAULT, CameraPos, to, ZeroVec, ZeroVec, TracEnt, norm, NULL, NULL, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH, &CameraClampFilter)
				
				if(cfrac < 1)	//skracujeme kameru
				{
					cfrac -= 0.1
					if(cfrac < 0)
						cfrac = 0
					CameraOffset = CameraOffset * cfrac
				}
				
				World.CameraPos = CameraPos + CameraOffset + EarthquakeOff
				World.CameraAngs = MiniCutsceneAngs
				World.CameraNorm = Angles2Vector(World.CameraAngs)
				SetCamera(World.CameraPos, MiniCutsceneAngs/*World.CameraAngs*/)	
			}
		}

		if(status != ST_NOCONTROL && World.CutScene == false)	//ak nic neovladame
			HandleCamera()	//musi to byt pred ProcessHacking()

		bool InHackedDevice = ProcessHacking()
		bool InVehicle = false
		
		if(!InHackedDevice && World.CutScene == false && !MiniCutsceneStatus)	//ak nic neovladame)
			InVehicle = ProcessVehicleControl()

		if(InHackedDevice || InVehicle || World.CutScene || MiniCutsceneStatus > 1)
			World.Cross.Show(false)

		if(!InHackedDevice && !World.CutScene && !MiniCutsceneStatus)	//true = sme do niecoho nahackovani a funkconsti playera nechceme
		{
			if(InVehicle)
			{
				ProcessHealth()
			}
			else	//normalny stav
			{
				HandleMovement()
				ProcessFlashLight()
	
				if(g_iFly == false)
				{
					ProcessWeapons()
					ProcessIdleVars()
					ProcessInterations()
					ProcessFootsteps()
					ProcessHealth()
					ProcessDistInteractions()
				}
	
				if(KEY(TASKLIST, false))
					World.TaskList.ForceOpening()
	
				if(Hands && CurrentWeapon.CanUpdateOwnerAnim())
					BumpFrame(Hands, ftime, 0)
	
				World.Cross.SetScale(DefaultCrossScale + MovementCrossScale + CameraCrossScale + ShootingCrossScale * ExactAimCrossScale)
				World.Cross.OnFrame()
			}
		}

		BrakeScale = 1.0

		if(Noises)
			Noise(Noises)

		if(KeyState(KC_U))
		{
			ClearKey(KC_U)

//				World.Player.UpdateAgent(true)
//			World.ShowCustomMessage("message is here!", TUTORIAL_DIALOG)
		}

int wnum
vector wfrom
vector wto

CPath pPath

		if(wnum == 1)
		{
			if(pPath)
			{
				pPath.Destroy()
				pPath = NULL
			}

			wto = GetOrigin(this)

			CPosition cfrom = new CPosition
			CPosition cto = new CPosition

			if(World.pPathFinder.PositionForPoint(wfrom, cfrom) && World.pPathFinder.PositionForPoint(wto, cto))
			{
				if(World.pPathFinder.FindClosestPosition(0, cto, cto, 24))
				{
					pPath = World.pPathFinder.FindShortestPath(0, cfrom, cto, 0)
					/*
					if(pPath)
					{
						Print(pPath.Length())
						Print(pPath.Size());
						Print(pPath.Position(0));
					}*/
				}
			}
		}

		vector pos7
#ifdef DEVELOPER
/*
		if(KeyState(KC_J))
		{
			ClearKey(KC_J)

			if(wnum == 0)
			{
				wfrom = GetOrigin(this)
				wnum++
			}
			else
			{
				if(wnum == 1)
				{
					if(pPath)
					{
						pPath.Destroy()
						pPath = NULL
					}
					wnum = 0
				}
			}
		}
*/

		if(KeyState(KC_O))
		{
			ClearKey(KC_O)
			pos7 = World.CameraPos
			pos7 += World.CameraNorm * 50
			vector targ2 = g_CamMat[0] * 25
//			new ShotStreak(pos7 + targ2, pos7 - targ2, 5, 25, 1, "spr/shotstreak")
			new ShotStreak(pos7 + targ2, pos7 - targ2, 5, 25, 1, 5)
		}

/*		if(KeyState(KC_O))
		{
			ClearKey(KC_O)
			pos7 = World.CameraPos
			pos7 += World.CameraNorm * 250
			rigidbody_rope rope = new rigidbody_rope
			rope.coords = pos7
			rope.segnum = 10
			rope.length = 50
			Throw(rope, this, EV_INIT, 0)
		}*/
#endif
#ifdef FIDO
/*
		if(KeyState(KC_J))
		{
			//ClearKey(KC_J)
			
			pos7 = World.CameraPos
			pos7 += World.CameraNorm * 48
			
			ExtClass ec = new ExtClass
			SetOrigin(ec, pos7)
			//UpdateEntity(ec)
			
			int btype

			btype++
			if(btype >= 3)
				btype = 0

			switch(btype)
			{
			case 0:
				ec.SetObject(GetObject("obj/physics/boxik.xob"))
				NxActorCreate(ec, ZeroVec, BC_MODEL, true, 20, 0, 0)
				break

			case 1:
				ec.SetObject(GetObject("obj/physics/bmic.xob"))
				NxActorCreate(ec, ZeroVec, BC_MODEL, true, 3, 0, 0)
				break

			case 2:
				ec.SetObject(GetObject("obj/physics/can.xob"))			
//				ec.SetObject(GetObject("obj/weapons/shell.xob"))
//				NxActorCreate(ec, ZeroVec, BC_MODEL)
				NxActorCreate(ec, ZeroVec, BC_CAPSULE_Z, true, 5, 0, 0)
				break
			}

			vector min,max
			GetBoundBox(ec, min, max)
//			if(NxActorCreate(ec, ZeroVec, BC_NONE, true, 0, 0))
	//		{
		//		NxShapeCreateBox(ec, g_vIdentMatrix, max - min, 0, 10)
			//}
			
			SetFlags(ec, TFL_VISIBLE|TFL_FULLMATRIX|TFL_NOSHADOW)
			
			NxActorSetCollisionGroup(ec, 1)
			SetVelocity(ec, World.CameraNorm * 160)
		}

		if(KeyState(KC_K))
		{
			ClearKey(KC_K)

			pos7 = World.CameraPos
			pos7 += World.CameraNorm * 80
			
			ec = new ExtClass
			SetOrigin(ec, pos7)
			UpdateEntity(ec)
			
			SetFlags(ec, TFL_VISIBLE|TFL_FULLMATRIX|TFL_NOSHADOW)
			ec.SetObject(GetObject("obj/physics/bmic2.xob"))
			
			ClothDef cdef = new ClothDef
			cdef.Thickness = 0.05
			cdef.Density = 1.0 * (1 / g_timescale)
			cdef.BendingStiffness = 1.0 * g_timescale
			cdef.StretchingStiffness = 1.0 * g_timescale
			cdef.DampingCoefficient = 0.5
			cdef.Friction = 0.7
			cdef.Pressure = 2.0
			cdef.CollisionResponseCoefficient = 1.1
			cdef.AttachmentResponseCoefficient = 1.1
			cdef.SolverIterations = 10
			cdef.CollisionGroup = 1
//			cdef.Flags = NX_CLF_PRESSURE|NX_CLF_BENDING|NX_CLF_GRAVITY
//			cdef.Flags = NX_CLF_BENDING|NX_CLF_GRAVITY|NX_CLF_BENDING_ORTHO|NX_CLF_PRESSURE
//			cdef.Flags = NX_CLF_GRAVITY|NX_CLF_BENDING|NX_CLF_BENDING_ORTHO
//			cdef.Flags = NX_CLF_GRAVITY
			cdef.Flags = NX_CLF_GRAVITY|NX_CLF_PRESSURE
			NxClothCreate(ec, "obj/physics/bmic", cdef, false)
		}
*/

		if(KeyState(KC_J))
		{
			ClearKey(KC_J)
			pos7 = World.CameraPos
			pos7 += World.CameraNorm * 100
			creature_ragdoll ragdoll = new creature_ragdoll()
//			ragdoll.model = "obj/creatures/dead/dead01.xob"
	//		ragdoll.ragdoll = "Dead01"
//			ragdoll.model = "obj/creatures/deaddown/deaddown.xob"
//			ragdoll.ragdoll = "DeadDown"
			ragdoll.model = "obj/creatures/deadupper/deadupper.xob"
			ragdoll.ragdoll = "DeadUpper"
			ragdoll.weight = 70.1
			ragdoll.coords = pos7
			ragdoll.angle = 230 * sqrt(g_timescale)
			Throw(ragdoll, World, EV_INIT, 0)
			Throw(ragdoll, World, EV_LOADED, 0)
		}
#endif

//Koniec OnFrame()
		if(VehicleControlStatus != 2)		//flaru musime updatovat tu inac je frame pozadu. ked sme vo vehicli tak ju updatuje vehicle pretoze by bola zase frame pozadu
		{
			if(World.flare)
				World.flare.Update()
		}

		NumWorldContactNormals = 0
		WorldContactNormal = ZeroVec
		NumLegsContactNormals = 0
	}

//---------------------------------------------------------------------------------------------------------
	void UpdateHUD(int id)	//virtual
	{
		switch(id)
		{
			case 1:
				UpdateAmmoBar()
				break
		}
	}

//-----------------------------------------------------------------
	void SetBBoxInfo()	//nastavime BBox koli AI a pod.
	{
		float HalfHeight = PlayerCapsuleHalfHeights[0]
		Maxs[0] = PlayerCapsuleRadius
		Maxs[1] = PlayerCapsuleRadius
		Maxs[2] = HalfHeight
		Mins = -Maxs
	
		vector vec = Maxs - Mins	
		Height = vec[2]
		HHeight = Height * 0.5
		HHVec = Vector(0, 0, HHeight)
		Radius = vec[0] * 0.5
	}
	
//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		if(fl == 1)	//fyzicky item_misc ktory drzime bol vlozeny do triggeru
		{
			if(World.ManipEntity)
				DropManipEntity(true)
			
			return
		}
		
		if(fl == 2)	//signal pre zacatie minicutsceny pretoze sa zacali pustat ingame dialogy
		{
			if(status != ST_DEATH)
			{
				MiniCutsceneStatus = 1
				HideWeaponSelectGUI()

				if(HearthSound)
					HearthSoundNeedStop = true

				if(World.ManipEntity)
				{
					WeaponBeforeMiniCutscene = WeaponBeforeManip
					DropManipEntity(false)
				}
				else
				{
					WeaponBeforeMiniCutscene = CurrentWeapon.ClassNum
					WeaponChange(CDummyWeapon, true)
				}
				PlaySound(this, g_sCommonSounds[67], SF_ONCE)
//				World.FullscreenFlash(600, 100, "255 255 255", 0, 1)
			}
		}
	}

//-----------------------------------------------------------------
	void EOnSoundEnd(ExtClass other, int extra)
	{
		if(extra == PainSound)
		{
			PainSound = 0
			return
		}

		if(extra == BulletTimeFadeInSound)
		{
			BulletTimeFadeInSound = 0
			return
		}

		if(extra == HearthSound)
		{
			if(HearthSoundNeedStop)
				HearthSound = 0
			else
				HearthSound = PlaySound(this, g_sCommonSounds[9], SF_ONCE)

			return
		}
		
		if(extra == OxygenRespirationSnd)
		{
			if(!OxygenAtmosphere)
			{
				OxygenRespSndIndex = 1 - OxygenRespSndIndex
				OxygenRespirationSnd = PlaySound(this, g_sCommonSounds[40 + OxygenRespSndIndex], SF_ONCE | SF_AMBIENT)
			}
			else
				OxygenRespirationSnd = 0
		}
	}
/*
		int magazine
		CurrentWeapon = new item_weapon_pistol(this)
		magazine = PistolAmmoOnStart
		clamp magazine
		pistol.Ammo = PistolAmmoOnStart	//secondary ammo nepouziva
		Ammo[AmmoPistol] =
		CurrentWeapon.Pick(this)

		CurrentWeapon = new item_weapon_shotgun(this)
		CurrentWeapon.Ammo = ShotgunAmmoOnStart
		CurrentWeapon.Pick(this)

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Ammo = MachinegunAmmoOnStart
		CurrentWeapon.Pick(this)

		CurrentWeapon = new item_weapon_sniperrifle(this)
		CurrentWeapon.Ammo = SniperRifleAmmoOnStart
		CurrentWeapon.Pick(this)

		CurrentWeapon = new item_weapon_grenade(this)
		CurrentWeapon.Ammo = GrenadeAmmoOnStart
		CurrentWeapon.Pick(this)

		CurrentWeapon = new item_weapon_jackhammer(this)
		CurrentWeapon.Pick(this)
*/

//-----------------------------------------------------------------
	WeaponClass CreateWeapon(int ClNum, int ammo, int MagazineAmmo)
	{
		WeaponClass wc = NULL
		int PerMagazine
		int StartAmmo
		int AType

		switch(ClNum)
		{
			case CDummyWeapon:
				wc = new item_weapon_dummy()
				PerMagazine = 0
				StartAmmo = 0
				AType = 0
			break

			case CPistol:
				wc = new item_weapon_pistol(this)
				PerMagazine = PistolAmmoPerMagazine
				StartAmmo = PistolAmmoOnStart
				AType = AmmoPistol
			break

			case CMachinegun:
				wc = new item_weapon_machinegun(this)
				PerMagazine = MachinegunAmmoPerMagazine
				StartAmmo = MachinegunAmmoOnStart
				AType = AmmoMachinegun
			break

			case CShotgun:
				wc = new item_weapon_shotgun(this)
				PerMagazine = ShotgunAmmoPerMagazine
				StartAmmo = ShotgunAmmoOnStart
				AType = AmmoShotgun
			break

			case CSniperRifle:
				wc = new item_weapon_sniperrifle(this)
				PerMagazine = SniperRifleAmmoPerMagazine
				StartAmmo = SniperRifleAmmoOnStart
				AType = AmmoSniperRifle
			break

			case CRocketLauncher:
				wc = new item_weapon_rocketlauncher(this)
				PerMagazine = RocketLauncherAmmoPerMagazine
				StartAmmo = RocketLauncherAmmoOnStart
				AType = AmmoRocketLauncher
			break

			case CFlameThrower:
				wc = new item_weapon_flamethrower(this)
				PerMagazine = FlameThrowerAmmoPerMagazine
				StartAmmo = FlameThrowerAmmoOnStart
				AType = AmmoFlameThrower
			break

			case CGrenade:
				wc = new item_weapon_grenade(this)
				PerMagazine = GrenadeAmmoPerMagazine
				StartAmmo = GrenadeAmmoOnStart
				AType = AmmoGrenade
			break

			case CJackHammer:
				wc = new item_weapon_jackhammer(this)
				AType = 0
			break
		}

		if(!wc)
			return NULL

		if(MagazineAmmo > -1)	//zadavame zvlast zasobnik a zbytok
		{
			Ammo[AType] = ammo
			wc.Ammo = MagazineAmmo
		}
		else							//naplnime zasobnik a zbytok
		{
			if(ammo > -1)			//ammo rozpocitame do zasobnika a zbytok playerovi. inac startovnu hodnotu vsetkych nabojov rozlozime zasobnik + zbytok
				StartAmmo = ammo

			wc.Ammo = StartAmmo
			clamp wc.Ammo<0, PerMagazine>
			Ammo[AType] = StartAmmo - wc.Ammo
	
			if(Ammo[AType] < 0)
				Ammo[AType] = 0
		}

		return wc
	}

//-----------------------------------------------------------------
	void WriteCurrentStorageTo(PlayerStorage storage)
	{
		if(!storage)
			return

		int n
		WeaponClass wc
		storage.NumWeapons = 0

		for(n = 0; n < sizeof(PWeapons); n++)
		{
			wc = PWeapons[n]
			if(wc)
			{
				storage.weapons[storage.NumWeapons] = wc.ClassNum
				storage.ammo[storage.NumWeapons] = Ammo[wc.AmmoType]
				storage.MagazinesAmmo[storage.NumWeapons] = wc.Ammo
				storage.NumWeapons++
			}
		}

		storage.CurrentWeapon = CurrentWeapon.ClassNum
		storage.BulletTime = BulletTime
		storage.health = Health
		storage.oxygen = oxygen
		storage.flashlight = FlashLight
		storage.DifficultyLevel = DifficultyLevel
	}

//-----------------------------------------------------------------
	void EOnLoad(ExtClass other, int extra)
	{
		AfterLoad = true
		g_timescale = 1.0
		SetWidgetWorld(WView, globalmap, WViewWidgetCamera)

		if(World.BulletTimeFade == 0)
			SetEAXEnvironment("$current", "Bullet", 0)

		if(!g_PlayerStorage)
			g_PlayerStorage = new PlayerStorage

		WriteCurrentStorageTo(g_PlayerStorage)
		dShapeCastCreate(ShapeCast, false)
		dShapeCastAddSphere(ShapeCast, ZeroVec, PlayerCapsuleRadius - 0.1)
		dBodyInteractionLayer(ShapeCast, ET_PLAYERCAST)
		
		ContactConstraint = dConstraintCreateContact()
		ContactConstraint.setBody1(World)
		ContactConstraint.setBody2(this)

		DampingConstraint1 = dConstraintCreatePoint()
		DampingConstraint1.setBody1(World)
		DampingConstraint1.setBody2(this)

		//restorujeme vlastnosti widget do stavu pred sejvom
/*		ShowHUD(-1, -1, -1, HealthBarVisible, AmmoBarVisible, BulletTimeBarVisible, LightBarVisible, ThrowBarVisible, OxygenBarVisible, ReconSignalVisible)
		UpdateHealthBar()
		UpdateAmmoBar()
		UpdateBulletTimeBar()
		UpdateLightBar()
		UpdateOxygenBar()
		UpdateHackSignalBar()
		
		if(HackModeFade > 0)
		{
			ShowHackGUI(true)
			float HackWindowScale = HackLoginTimer * HackLoginTimer
			SetHackPreviewExtendsSize(HackWindowScale)
		}*/

		if(BulletTimeFadeInSound)	//co hralo pred sejvom musi hrat i po sejve
			BulletTimeFadeInSound = PlaySound(this, g_sCommonSounds[17], SF_ONCE)	//nabeh
		
		if(HearthSound)
			HearthSound = PlaySound(this, g_sCommonSounds[9], SF_ONCE)
		
		if(OxygenRespirationSnd)
			OxygenRespirationSnd = PlaySound(this, g_sCommonSounds[40 + OxygenRespSndIndex], SF_ONCE | SF_AMBIENT)

		if(HackingLoopSound)
			HackingLoopSound = PlaySound(this, g_sCommonSounds[54], SF_AMBIENT)

		if(HackLoopSound)
			HackLoopSound = PlaySound(this, g_sCommonSounds[37], 0)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
#ifdef EDITOR	//
		vector mat[4]
		vector Angles = Vector(0, angle, 0)
		RollPitchYawMatrix2(Angles, mat)
		mat[3] = coords
		SetMatrix(this, mat)
		UpdateEntity(this)
#endif

		if(!ContactConstraint)	//musi byt az tu pretoze nemusi existovat body worldu
		{
			ContactConstraint = dConstraintCreateContact()
			ContactConstraint.setBody1(World)
			ContactConstraint.setBody2(this)
		}

		if(!DampingConstraint1)
		{
			DampingConstraint1 = dConstraintCreatePoint()
			DampingConstraint1.setBody1(World)
			DampingConstraint1.setBody2(this)
		}

		trigger_hack_detector trigger					//prepojime triggery aby sme ich pri hachovani nemuseli hladat
		trigger_hack_detector LastTrigger = NULL
		FirstHackTrigger = GetNextEntity(NULL, CTriggerHackDetector)

		for(trigger = FirstHackTrigger; trigger != NULL; trigger = GetNextEntity(trigger, CTriggerHackDetector))
		{
			if(LastTrigger)
				LastTrigger.NextSiblingTrigger = trigger

			LastTrigger = trigger
		}
	}

//-----------------------------------------------------------------
	void creature_player()
	{
#ifdef EDITOR
	UpdatePlayerPointer(this)	//v GUI editoru musi byt pointer na playera stale platny
#endif

		if(!HUD)
			HUD = new PlayerHUD()

		g_timescale = 1.0
		flags |= FL_NOTEST

		if(g_bRMap)
		{
			coords = g_vPlayerPos
			CameraAngles = g_vPlayerAngs
			g_bRMap = false
		}
		else
			CameraAngles = Vector(0, angle, 0)

		World.Player = this
		ClassType = ClassTypeCreature
		ClassNum = ClassNumPlayer
		SetKind(this, PLAYER)	//asi special info pre engine ze ide o playera

		SetFlags(this, TFL_SOLID | TFL_VISIBLE | TFL_TOUCHTRIGGERS | TFL_FULLMATRIX | TFL_OBBCOLLISION | TFL_USER6)
		SetEventMask(this, EV_INIT|EV_LOADED|EV_FRAME|EV_ONLOAD|EV_CONTACT|EV_SOUNDEND|EV_ONLOAD)

		flags |= FL_CANOPENDOORS|FL_NODISABLE
		SetName("player")

		g_bImmortal = false

		CameraTargetPos = coords + Vector(0, 0, PlayerCapsuleExtends[Crouch] + PlayerCameraOffset)
		CameraPos = CameraTargetPos

#ifndef EDITOR
		World.CameraPos = CameraPos
		World.CameraAngs = CameraAngles
		World.CameraNorm = Angles2Vector(World.CameraAngs)
		SetCamera(World.CameraPos, World.CameraAngs)
		CalcMatrixFromCamera()
#else
		if(World.EditorActive == false)
		{
			World.CameraPos = CameraPos
			World.CameraAngs = CameraAngles
			World.CameraNorm = Angles2Vector(World.CameraAngs)
			SetCamera(World.CameraPos, World.CameraAngs)
			CalcMatrixFromCamera()
		}
#endif

		vector mat[4]
		mat[0] = "1 0 0"
		mat[1] = "0 1 0"
		mat[2] = "0 0 1"
		mat[3] = coords
		SetMatrix(this, mat)
		SetBBoxInfo()
		UpdateEntity(this)
		LastStablePos = coords
		LastStayPos = coords

		TrcContact = new TraceContact
		WeaponSoundPos = new MinClass

		ShapeCast = new MinClass
		SetFlags(ShapeCast, TFL_FULLMATRIX)
		dShapeCastCreate(ShapeCast, false)
		dShapeCastAddSphere(ShapeCast, ZeroVec, PlayerCapsuleRadius - 0.1)
		dBodyInteractionLayer(ShapeCast, ET_PLAYERCAST)

		CreateRigidBody()
		EnableContols(true)

		Hands = new player_hands(this)
		player_hands hands = Hands
		WeaponAngles = CameraAngles	//zbran musi byt natocena podla playera

		FlashLightBase = new MinClass
		SetOrigin(FlashLightBase, "0 0 0")
		SetAngles(FlashLightBase, "0 -90 0")
		AddChild(Hands, FlashLightBase, -1)

		DummyPlayer = new player_dummyobject

		if(g_PlayerStorage)	//ked mu chceme z akehokolvek dovodu vnutit co ma mat, tak vzdy pred nahanim mapy mu to naplnime
		{
			for(int n = 0; n < g_PlayerStorage.NumWeapons; n++)
			{
				if(!GetWeaponPointer(g_PlayerStorage.weapons[n]))
				{
					CurrentWeapon = CreateWeapon(g_PlayerStorage.weapons[n], g_PlayerStorage.ammo[n], g_PlayerStorage.MagazinesAmmo[n])	//uskladnujeme vsetky naboje vcitane zasobnikov
					CurrentWeapon.Pick(this)
				}
			}

			CurrentWeapon = GetWeaponPointer(g_PlayerStorage.CurrentWeapon)
			Health = g_PlayerStorage.health
			FlashLight = g_PlayerStorage.flashlight
			oxygen = g_PlayerStorage.oxygen
			BulletTime = g_PlayerStorage.BulletTime
			DifficultyLevel = g_PlayerStorage.DifficultyLevel
//			World.playerscale = PLAYER_DAMAGE_SCALE[DifficultyLevel]
//			World.enemyscale = ENEMY_DAMAGE_SCALE[DifficultyLevel]
		}
		else	//nikdy sa to nesmie stat !!!
		{
			DPrint("Unknown error. Missing player storage")	
		}

		CurrentWeapon.Show(true)
		SelectWeapon(CurrentWeapon)
		SetStatus(ST_NONE)
		hands.PlayIdleAnim()

		MaxHealth = PlayerMaxHealth 
		HealthDown(0)	//zapne zvuk srdca + blikanie baru. potrebe ked prechadzame z levelu do levelu alebo nahravame z autosejvu
		OxygenAtmosphere = true
		FlashLightVisTimer = 5	//baterka po chvili zhasne
		HealthFlashReason = 0
		HealthFlashRate = 1
		ReconSignalFlash = 360

		ShowHUD(-1, -1, -1, 1, 1, 1, 1, 0, 0, 1 - HackingDisabled)
		UpdateHealthBar()
		UpdateAmmoBar()
		UpdateBulletTimeBar()
		UpdateLightBar()
		UpdateOxygenBar()
		UpdateHackSignalBar()

		FSType = FSPlayer
	}

	void ~creature_player()
	{
		//nemazat playerove zbrane. maze ich mapa!
//		ShowHUD(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
//		ShowHackGUI(false)
//		HideWeaponSelectGUI()
		int n

		for(n = 0; n < sizeof(HUD.WHitZones); n++)
			ShowWidget(HUD.WHitZones[n], false)

		g_iFly = 0

		DestroyRigidBody()

		if(FlashLightBase)
		{
			delete FlashLightBase
			FlashLightBase = NULL
		}

		if(Hands)
		{
			delete Hands
			Hands = NULL
		}

		if(TrcContact)
		{
			delete TrcContact
			TrcContact = NULL
		}

		if(ShapeCast)
		{
			dShapeCastDestroy(ShapeCast)
			delete ShapeCast
		}

		if(WeaponSoundPos)
		{
			delete WeaponSoundPos
			WeaponSoundPos = NULL
		}
		
		if(HUD)
		{
			delete HUD
			HUD = NULL
		}
	}
}

//-----------------------------------------------------------------

void creature_player::SwitchBulletTimeMode()
{
	BulletTimeFadeTarget = 1 - BulletTimeFadeTarget

	if(BulletTimeFadeTarget == 1)
	{
		BulletTimeFadeInSound = PlaySound(this, g_sCommonSounds[17], SF_ONCE)	//nabeh
		LoopTimeCounter = 0
		
	}
	else
		BulletTimeFadeOutSound = PlaySound(this, g_sCommonSounds[19], SF_ONCE)	//dobeh
}