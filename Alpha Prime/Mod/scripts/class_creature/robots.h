
//=================================================================================
class item_weapon_spider extends FireWeaponClass
{
//-----------------------------------------------------------------
	void OnActionEnd()
	{
		if(Action == 1)	//shot
		{
			EndAction()
		}

		EndAction()
	}

//-----------------------------------------------------------------
	int Action(int n, vector targ)
	{
		if(!CanAct(n))
			return false

		Ammo = 100	//FIXME

		if(PrimaryFire(targ))
		{
			Action = 0
			return true
		}

		Action = 0
		return false
	}

//-----------------------------------------------------------------
	void item_weapon_spider()
	{
		ClassNum = CDronWeapon
		SelectObject(this, MainObject)

		AmmoPerMagazine = PistolAmmoPerMagazine
		AmmoType = AmmoPistol
		Ammo = 1000
		NumActions = 1

		Shooters[0] = new ShooterBullets(this, 1)
		NumShooters = 1

		ADelay[0] = 0.1	//primary attack
		RicSndSkip = 3

		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 6.57
		ShotStor.precision = 0.04
		ShotStor.StreakWidth = 16.8  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 72  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 5003  * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 3
		ShotStor.damage = WDmgSpiderRobot
		ShotStor.power = 2
		ShotStor.CameraHitFactor = 1
		ShotStor.RicDecalType = DE_PLASMA		//DE_BULLET, DE_PLASMA
		ShotStor.RicSndType = SE_PLASMA			//SE_BULLET, SE_PLASMA
		ShotStor.RicParticlesType = PE_PLASMA	//PE_BULLET, PE_PLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "-90 0 0", 1)
		Flash.rangle = ZeroVec

		InitSounds(WS_WeaponSpider)
	}
}

//=================================================================================
class creature_heavyspider extends AICreatureClass
{
//-----------------------------------------------------------------
void SoundFirstSight()
{
	Sound(6, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundSuspicious()
{
	Sound(6, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundDeath()
{
	Sound(21, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundPain()
{
  Sound(20, SF_ONCE)
}

//-----------------------------------------------------------------
	void SetBBoxInfo()
	{
		Height = 48
		HHeight = 23
		HHVec = Vector(0, 0, HHeight)
		Radius = 23
		Mins = "-23 -23 -23"
		Maxs = "23 23 23"

		SetBoundBox(this, Mins, Maxs)
	}

//-----------------------------------------------------------------
	void CreateBody()
	{
		dBodyCreate(this, ZeroVec, BC_NONE)

		geom = dGeomCreateSphere(23)

		dBodyAddGeom(this, geom, g_vIdentMatrix, "material/creaturemetal")

		dBodySetMass(this, ZeroVec, 250)
	}

	vobject hitobj
	MinClass hitent
	float lasthittime
	vector hitup
	HLIGHT hitlight

//-----------------------------------------------------------------
	void OnHit()
	{
		if(hitent == NULL)
		{
			hitent = new MinClass
			hitent.SetObject(hitobj, "$remap 'dron01_hit' 'heavyspider01_hit';")
		}

		if(hitlight)
		{
			RemoveLight(hitlight)
		}

		hitlight = AddLight(hitent,0,LFL_DYNAMIC|LFL_POINT, 0, 100, "50 50 200")
		SetFlags(hitent, TFL_VISIBLE|TFL_FULLMATRIX)
		vector mat[4]
		GetBoneMatrix(this, B_Body, mat)
		SetMatrix(hitent, mat)

		lasthittime = currenttime

		hitup[0] = 0
		hitup[1] = cos(currenttime * 50)
		hitup[2] = sin(currenttime * 50)
	}

//-----------------------------------------------------------------
	void OnDie()
	{
		if(hitlight)
		{
			RemoveLight(hitlight)
		}

		if(hitent)
		{
			delete hitent
			hitent = NULL
		}

		vector mat[4]
		GetBoneMatrix(this, B_Podvozek, mat)

		light_effect leff = new light_effect(mat[3], 0, 150, "20 20 255", 0)
		leff.Flash(1, true)

		misc_particle_effector eff = new misc_particle_effector
		eff.SetParms("particle/steam_smoke.ptc", ZeroVec, true, 2)
		AddChild(this, eff, 7)
                
		eff = new misc_particle_effector()

		eff.SetParms("particle/tachyonexplode3.ptc", GetMatrix(this, 3), true, 2)

		ClearEventMask(this, EV_VISIBLE)
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		if(lasthittime && hitent)
		{
			float td = currenttime - lasthittime

			const float maxtime = 0.1

			if(td > maxtime)
			{
				lasthittime = 0
				ClearFlags(hitent, TFL_VISIBLE)
				if(hitlight)
					RemoveLight(hitlight)
			}
			else
			{
				float c = maxtime - td
				clamp c<0, maxtime>

				c *= 255.0 / maxtime
				hitent.Color = ARGBF(c,c,c,c)

				c *= 1 / 255
				SetLightEx(hitlight, 0, 100, "20 20 255" * c);

				vector mat[4]
				GetBoneMatrix(this, B_Podvozek, mat)

				vector center = GetMatrix(this, 3)//mat[3]
				vector front = World.CameraPos - center
				VectorNormalize(front)

				vector right = front * hitup
				vector up = right * front

				mat[0] = front * 1.75
				mat[1] = right * 1.75
				mat[2] = up * 1.75
				mat[3] = center
				SetMatrix(hitent, mat)
			}
		}

		return 0
	}

//-----------------------------------------------------------------
	void SetHeadOrientation(float pitch, float yaw)
	{
		float lquat[4], quat[4]
		float lquat2[4], quat2[4]
		vector mat[4]

		if(pitch || yaw)
		{
			clamp pitch<-45, 45>
			//prepare quaternion for head rotation. Transform is in local space of object,
			//so axes are always the same
			QuatRotationRollPitchYaw(quat, Vector(-yaw, 0, 0))
			QuatRotationRollPitchYaw(quat2, Vector(0, 0, -pitch))
		}
		else
		{
			QuatIdentity(quat)
			QuatIdentity(quat2)
		}

		mat[3] = ZeroVec

		QuatLerp(lquat, PLookQuat, quat, ftime * 9)
		QuatMatrix(lquat, mat)
		SetBoneGlobal(this, B_Hlava, mat)

		QuatLerp(lquat2, PLookQuat2, quat2, ftime * 9)
		QuatMatrix(lquat2, mat)
		SetBoneMatrix(this, B_Hlava2, mat)

		QuatCopy(lquat, PLookQuat)
		QuatCopy(lquat2, PLookQuat2)
	}

//-----------------------------------------------------------------
	bool CanHitTarget(vector tpos)
	{
		return true
	}

//-----------------------------------------------------------------
	void creature_heavyspider()
	{
		ClassNum = 0
		Health = HeavySpiderHealth

		flags |= FL_CANOPENDOORS

		if(!SoundSetName)
			SoundSetName = "HeavySpider"

   	DefaultAnimSetName = "HeavySpider"

		RagdollMaterial = "material/metal"
		RagdollDef = "heavyspider"
		RagdollMass = 240

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 25
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.8
		MaxRestAfterAttack[0] = 2.8

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 15
		MinAttackDelay[1] = 0.4
		MaxAttackDelay[1] = 0.8
		MinRestAfterAttack[1] = 2
		MaxRestAfterAttack[1] = 4

		fMinBulletRain = 10
		bFlameResistent = true

		CurrentWeapon = new item_weapon_spider()
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_pivot0)

		bLongRange = true
		bShortRange = false

		fMinCosForSlowTurn = cos(60 * DEG2RAD)
		fMaxStrafeDistance = 76
		bAlignOnGround = false
		fZDamper = 10
		fADamper = 15

		fYawLimit = 360

		LookAxis = 3
		LookBone = B_Hlava2

		MainObject = GetObject("obj/creatures/heavyspider01/heavyspider01.xob")
		SelectObject(this, MainObject)

		hitobj = GetObject("obj/creatures/dron01/dron01_hit.xob")

		ShapeIndex = 2
		AgentCost = 10

		FSType = FSSpiderRobot
	}
}

//=================================================================================
class item_weapon_securityspider extends FireWeaponClass
{
//-----------------------------------------------------------------
	void OnActionEnd()
	{
		if(Action == 1)	//shot
		{
			EndAction()
		}

		EndAction()
	}

//-----------------------------------------------------------------
	int Action(int n, vector targ)
	{
		if(!CanAct(n))
			return false

		Ammo = 100	//FIXME

		if(PrimaryFire(targ))
		{
			Action = 0
			return true
		}

		Action = 0
		return false
	}

//-----------------------------------------------------------------
	void item_weapon_securityspider()
	{
		ClassNum = CDronWeapon
		SelectObject(this, MainObject)

		AmmoPerMagazine = PistolAmmoPerMagazine
		AmmoType = AmmoPistol
		Ammo = 1000
		NumActions = 1

		Shooters[0] = new ShooterBullets(this, 1)
		NumShooters = 1

		ADelay[0] = 0.2	//primary attack
		RicSndSkip = 3

		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 4
		ShotStor.precision = 0.040
		ShotStor.StreakWidth = 14.4  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 72  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 4993  * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 3
		ShotStor.damage = WDmgSecuritySpider1
		ShotStor.power = 2
		ShotStor.CameraHitFactor = 1
		ShotStor.RicDecalType = DE_PLASMA		//DE_BULLET, DE_PLASMA
		ShotStor.RicSndType = SE_PLASMA			//SE_BULLET, SE_PLASMA
		ShotStor.RicParticlesType = PE_PLASMA	//PE_BULLET, PE_PLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "-90 0 0", 1)
		Flash.rangle = ZeroVec

		InitSounds(WS_WeaponSpider)
	}
}

//=================================================================================
class creature_securityspider1 extends AICreatureClass
{
	
	float LightsAnimTimer
	int 	LightsAnimFrame
	float LightsFadeOutTimer
	
//-----------------------------------------------------------------
void SoundFirstSight()
{
	Sound(22, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundSuspicious()
{
	Sound(23, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundDeath()
{
	Sound(21, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundPain()
{
  Sound(20, SF_ONCE)
}

//-----------------------------------------------------------------
bool CanHitTarget(vector tpos)
{
	//return true, if we are at the good angle
	vector mat[4]
	GetMatrix4(CurrentWeapon, mat)

	float dot
	vector aimvec1, aimvec2
	
	aimvec2 = mat[1]
	dot = UpVec * aimvec2

	aimvec1 = tpos - mat[3]
	aimvec1[2] = 0
	VectorNormalize(aimvec1)

	aimvec2[2] = 0
	VectorNormalize(aimvec2)

	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, mat[3], mat[2] * 64 + mat[3])	
	//AddDShape(SHAPE_LINE, 0xfff00fff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, mat[3], aimvec2 * 64 + mat[3])

	dot = aimvec1 * aimvec2

	if(dot < 0.89)
		return false

	return true
}

//-----------------------------------------------------------------
	void CreateBody()
	{
		dBodyCreate(this, ZeroVec, BC_NONE)

		geom = dGeomCreateSphere(20)

		dBodyAddGeom(this, geom, g_vIdentMatrix, "material/creaturemetal")

		dBodySetMass(this, ZeroVec, 250)
	}

	vobject hitobj
	MinClass hitent
	float lasthittime
	vector hitup
	HLIGHT hitlight

//-----------------------------------------------------------------
	void OnHit()
	{
		if(hitent == NULL)
		{
			hitent = new MinClass
			hitent.SetObject(hitobj, "$remap 'dron01_hit' 'heavyspider01_hit';")
		}

		if(hitlight)
		{
			RemoveLight(hitlight)
		}

		hitlight = AddLight(hitent,0,LFL_DYNAMIC|LFL_POINT, 0, 100, "50 50 200")
		SetFlags(hitent, TFL_VISIBLE|TFL_FULLMATRIX)
		vector mat[4]
		GetBoneMatrix(this, B_Body, mat)
		SetMatrix(hitent, mat)

		lasthittime = currenttime

		hitup[0] = 0
		hitup[1] = cos(currenttime * 50)
		hitup[2] = sin(currenttime * 50)
	}

//-----------------------------------------------------------------
	void OnDie()
	{
		if(hitlight)
		{
			RemoveLight(hitlight)
		}

		if(hitent)
		{
			delete hitent
			hitent = NULL
		}

		vector mat[4]
		GetBoneMatrix(this, B_Podvozek, mat)

		light_effect leff = new light_effect(mat[3], 0, 150, "20 20 255", 0)
		leff.Flash(1, true)

		misc_particle_effector eff = new misc_particle_effector
		eff.SetParms("particle/steam_smoke.ptc", ZeroVec, true, 2)
		AddChild(this, eff, 7)
                
		eff = new misc_particle_effector()
		
		eff.SetParms("particle/tachyonexplode3.ptc", GetMatrix(this, 3), true, 2)
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		if(status < STATE_DIE)
		{
			LightsAnimTimer += ftime
					
			if(LightsAnimTimer >= 0.25)	//4 virtualne FPS
			{
				LightsAnimTimer -= 0.25
				LightsAnimFrame = LightsAnimFrame + 1 % 4
				UVAnim = 0.25 * LightsAnimFrame
			}
		}
		else
		{
			LightsFadeOutTimer += ftime * 0.3

			if(LightsFadeOutTimer >= 1)
			{
				LightsFadeOutTimer = 1
				ClearEventMask(this, EV_VISIBLE)
			}
			else
				SetEventMask(this, EV_VISIBLE)

			float Intensity = 1 - LightsFadeOutTimer * 255			
			int iIntensity = Intensity
			Color2 = ARGB(0, iIntensity, iIntensity, iIntensity)	//staticke svetielka				// 		abgr
		}


		if(lasthittime && hitent)
		{
			float td = currenttime - lasthittime

			const float maxtime = 0.1

			if(td > maxtime)
			{
				lasthittime = 0
				ClearFlags(hitent, TFL_VISIBLE)
				if(hitlight)
					RemoveLight(hitlight)
			}
			else
			{
				float c = maxtime - td
				clamp c<0, maxtime>

				c *= 255.0 / maxtime
				hitent.Color = ARGBF(c,c,c,c)

				c *= 1 / 255
				SetLightEx(hitlight, 0, 100, "20 20 255" * c);

				vector mat[4]
				GetBoneMatrix(this, B_Podvozek, mat)

				vector center = GetMatrix(this, 3)
				vector front = World.CameraPos - center
				VectorNormalize(front)

				vector right = front * hitup
				vector up = right * front

				mat[0] = front * 1.75
				mat[1] = right * 1.75
				mat[2] = up * 1.75
				mat[3] = center
				SetMatrix(hitent, mat)
			}
		}

		return 0
	}

//-----------------------------------------------------------------
	void SetHeadOrientation(float pitch, float yaw)
	{
		float lquat[4], quat[4]
		vector mat[4]

		if(pitch || yaw)
		{
			clamp pitch<-35, 35>
			clamp yaw<-60, 60>

			QuatRotationRollPitchYaw(quat, Vector(-yaw, 0, -pitch))
		}
		else
		{
			QuatIdentity(quat)
		}

		QuatLerp(lquat, PLookQuat, quat, ftime * 9)

		QuatMatrix(lquat, mat)
		SetBoneGlobal(this, B_Spine, mat)

		QuatCopy(lquat, PLookQuat)
	}

//-----------------------------------------------------------------
	void creature_securityspider1()
	{
		ClassNum = 0
		Health = SecuritySpiderHealth

		flags |= FL_CANOPENDOORS

		if(!SoundSetName)
			SoundSetName = "SecuritySpider"

   	DefaultAnimSetName = "SecuritySpider"

		RagdollMaterial = "material/metal"
		RagdollDef = "securityspider"
		RagdollMass = 75

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.8
		MaxRestAfterAttack[0] = 2.8

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.4
		MaxAttackDelay[1] = 0.9
		MinRestAfterAttack[1] = 2
		MaxRestAfterAttack[1] = 4

		fMinBulletRain = 25
		bFlameResistent = true

		CurrentWeapon = new item_weapon_securityspider()
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_pivot0)
		CurrentWeapon.ShotStor.damage = WDmgSecuritySpider1

		SetAngles(CurrentWeapon, "0 180 0")

		bLongRange = true
		bShortRange = false

		fMinCosForSlowTurn = cos(60 * DEG2RAD)
		fMaxStrafeDistance = 76
		bAlignOnGround = false
		fZDamper = 10
		fADamper = 15

		fYawLimit = 240

		LookAxis = -3
		LookBone = B_Hlava

		MainObject = GetObject("obj/creatures/securityspider01/securityspider01.xob")
		SetObject(MainObject, "$remap 'EnemyLights1' 'EnemyLights1_red';$remap 'EnemyLights2' 'EnemyLights2_blue'")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
		
		hitobj = GetObject("obj/creatures/dron01/dron01_hit.xob")

		ShapeIndex = 1

		FSType = FSSpiderRobot
	}
}

//=================================================================================
class item_weapon_dron extends FireWeaponClass
{
//-----------------------------------------------------------------
	void OnActionEnd()
	{
		if(Action == 1)	//shot
		{
			EndAction()
		}

		EndAction()
	}

//-----------------------------------------------------------------
	int Action(int n, vector targ)
	{
		if(!CanAct(n))
			return false

		Ammo = 100	//FIXME

		if(PrimaryFire(targ))
		{
			Action = 0
			return true
		}

		Action = 0
		return false
	}

//-----------------------------------------------------------------
	void item_weapon_dron()
	{
		ClassNum = CDronWeapon
		SelectObject(this, MainObject)

		AmmoPerMagazine = PistolAmmoPerMagazine
		AmmoType = AmmoShotgun
		Ammo = AmmoPerMagazine
		NumActions = 1
		RicSndSkip = 3

		Shooters[0] = new ShooterBullets(this, 1)
		NumShooters = 1

		ShotStor.damage = WDmgDron
		ShotStor.power = 3
		ShotStor.CameraHitFactor = 0.2
		ShotStor.NumBullets = 2
		ShotStor.MarkSize = 6
		ShotStor.precision = 0.0402
		ShotStor.StreakWidth = 7.2  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 72  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 5000  * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 2
		ShotStor.RicDecalType = DE_PLASMA		//DE_BULLET, DE_PLASMA
		ShotStor.RicSndType = SE_PLASMA			//SE_BULLET, SE_PLASMA
		ShotStor.RicParticlesType = PE_PLASMA	//PE_BULLET, PE_PLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		InitSounds(WS_WeaponDron)
	}
}

//=================================================================================
class creature_dron extends AICreatureClass
{
	int iColorBright
	int iColorEye
	int iColorEngine
	int iColorFlare1
	int iColorFlare2
	int iColorFlare3

	vobject hitobj
	MinClass hitent
	float lasthittime
	vector hitup
	HLIGHT hitlight
	misc_particle_effector EngineEffect
	hsound hs
	int LastFrame
	float LastNotifyTime //kdy jsme naposledy informovali AI o hluku

//-----------------------------------------------------------------
void SoundSuspicious()
{
	Sound(26, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundFirstSight()
{
	Sound(29, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundDeath()
{
	Sound(24, SF_ONCE)
	Sound(30, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundPain()
{
  Sound(25, SF_ONCE)
  Sound(31, SF_ONCE)
}

//-----------------------------------------------------------------
bool CanHitTarget(vector tpos)
{
	vector aimvec1 = tpos - CurrentPos
	aimvec1[2] = 0
	VectorNormalize(aimvec1)

	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, mat[3], mat[2] * 64 + mat[3])	
	//AddDShape(SHAPE_LINE, 0xfff00fff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, mat[3], aimvec2 * 64 + mat[3])

	float dot = aimvec1 * facevec

	if(dot < 0.95)
		return false

	return true
}


//-----------------------------------------------------------------
	void SetBBoxInfo()
	{
		Height = 48
		HHeight = 24
		HHVec = Vector(0, 0, HHeight)
		Radius = 12
		Mins = "-12 -12 -20"
		Maxs = "12 12 20"

		SetBoundBox(this, Mins, Maxs)
	}

//-----------------------------------------------------------------
	void SetHeadOrientation(float pitch, float yaw)
	{
		float lquat[4], quat[4]
		vector mat[4]

		if(pitch || yaw)
		{
			clamp pitch<-45, 45>

			//prepare quaternion for head rotation. Transform is in local space of object,
			//so axes are always the same
			QuatRotationRollPitchYaw(quat, Vector(-yaw, 0, -pitch))
		}
		else
		{
			QuatIdentity(quat)
		}

		QuatLerp(lquat, PLookQuat, quat, ftime * 9)

		QuatMatrix(lquat, mat)
		SetBoneGlobal(this, B_Body, mat)

		QuatCopy(lquat, PLookQuat)
	}

//-----------------------------------------------------------------
	void CreateBody()
	{
		dBodyCreate(this, ZeroVec, BC_NONE)

		geom = dGeomCreateCapsule(12, Vector(0, 0, 12))

		dBodyAddGeom(this, geom, g_vIdentMatrix, "material/creaturemetal")

		dBodySetMass(this, ZeroVec, 100)
	}

//-----------------------------------------------------------------
	void OnHit()
	{
		if(hitent == NULL)
		{
			hitent = new MinClass
			SelectObject(hitent, hitobj)
		}

		if(hitlight)
		{
			RemoveLight(hitlight)
		}

		hitlight = AddLight(hitent,0,LFL_DYNAMIC|LFL_POINT, 0, 100, "200 100 50")
		SetFlags(hitent, TFL_VISIBLE|TFL_FULLMATRIX)
		vector mat[4]
		GetBoneMatrix(this, B_Body, mat)
		SetMatrix(hitent, mat)

		lasthittime = currenttime

		hitup[0] = 0
		hitup[1] = cos(currenttime * 50)
		hitup[2] = sin(currenttime * 50)
	}

//-----------------------------------------------------------------
	void OnDie()
	{
		if(EngineEffect)
		{
			delete EngineEffect
			EngineEffect = NULL
		}

		if(hitlight)
		{
			RemoveLight(hitlight)
		}

		if(hitent)
		{
			delete hitent
			hitent = NULL
		}

		vector mat[4]
		GetBoneMatrix(this, B_Body, mat)

		light_effect leff = new light_effect(mat[3], 0, 150, "255 20 20", 0)
		leff.Flash(1, true)

		iColorBright = 0
		iColorEye = 0
		iColorEngine = 0
		iColorFlare1 = 0
		iColorFlare2 = 0
		iColorFlare3 = 0
		ClearEventMask(this, EV_VISIBLE)

		misc_particle_effector eff = new misc_particle_effector
		eff.SetParms("particle/steam_smoke.ptc", ZeroVec, true, 2)
		AddChild(this, eff, 7)

		eff = new misc_particle_effector()

		dBodyDestroy(this)
		dGeomDestroy(geom)

		GetBoneMatrix(this, B_Body, mat)

		SetBone(this, B_Layer1, ZeroVec, ZeroVec, 1.0)

		SetOrigin(this, mat[3])
		UpdateEntity(this)

		dBodyCreate(this, ZeroVec, BC_NONE)
		geom = dGeomCreateSphere(8)
		dBodyAddGeom(this, geom, g_vIdentMatrix, "material/rigidmetal")
		dBodyInteractionLayer(this, ET_DEFAULT)

		dBodySetMass(this, ZeroVec, 20)
		dBodyActive(this, true, true)

		SetFlags(this, TFL_FULLMATRIX)

		eff = new misc_particle_effector()
		eff.SetParms("particle/tachyonexplode.ptc", GetMatrix(this, 3), true, 2)

		SetAnimSlot(this, s_walk, VNULL, 4.0, 1, NULL, 0, 0)

		SetEventMask(this, EV_CONTACT|EV_SOUNDEND)
	}

//-----------------------------------------------------------------
// Cela tahle sracka je prudce temporalni. Pozdeji vyresit asi nahrazenim potvory za
// misc_physics_object
//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		if(!hs) //nemame impact sound, tak proc se zdrzovat
		{
			return
		}

		if(other == World.Player)
			return

		float TargetFrame = g_FPS * MinImpactSoundsDelay	//zvuky nesmu hrat kratko po sebe
		int TF = LastFrame + TargetFrame

		if(g_iViewFrame <= TF)
			return

		vector ImpNorm = c.GetNormalImpulse()
		vector VelBefore, VelAfter
		float VelLngBefore = c.GetRelativeVelocityBefore(VelBefore)

		float ImpNormLng = VectorLength(ImpNorm)
		ImpNormLng /= (float)20

		if(VelLngBefore < 0)
			VelLngBefore = -VelLngBefore

//		vector Vel = GetVelocity(this)
//		float VelLng = VectorLength(Vel)

		float MinNormLng = 20 * 0.002
		float MinVelLngBefore = 20 * 0.00675

		if(ImpNormLng > MinNormLng && VelLngBefore > MinVelLngBefore)
		{
			int ps = PlaySound(this, hs, SF_ONCE)
			float vol = VelLngBefore
			clamp vol<0, 1>
			LastFrame = g_iViewFrame

			//teoreticky by se to melo limitovat jen od urcite hlasitosti, ale z hlediska gameplaye
			//chce proste player odlakat potvoru hozenym predmetem a ne spekulovat, jak velky ramus
			//musi udelat, aby potvora zareagovala
			float diff = currenttime - LastNotifyTime
			if(diff > 2.0)
			{
				LastNotifyTime = currenttime
				NotifyNoises(this, GetMatrix(this, 3), NOISERANGE_PHYSICS, NOISE_PHYSICS)
			}

			vol = LinearVol(vol)
			SetSoundVolume(ps, vol)
		}
	}
//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		if(lasthittime)
		{
			float td = currenttime - lasthittime

			const float maxtime = 0.1

			if(td > maxtime)
			{
				lasthittime = 0
				ClearFlags(hitent, TFL_VISIBLE)
				if(hitlight)
					RemoveLight(hitlight)
			}
			else
			{
				float c = maxtime - td
				clamp c<0, maxtime>

				c *= 255.0 / maxtime
				hitent.Color = ARGBF(c,c,c,c)

				c *= 1 / 255
				SetLightEx(hitlight, 0, 100, "200 100 50" * c);

				vector mat[4]
				GetBoneMatrix(this, B_Body, mat)

				vector center = mat[3]
				vector front = World.CameraPos - center
				VectorNormalize(front)

				vector right = front * hitup
				vector up = right * front

				mat[0] = front
				mat[1] = right
				mat[2] = up

				SetMatrix(hitent, mat)
			}
		}

		if(status < STATE_DIE)
		{
			int jbc = tab(wave, g_fTime * 2)
			iColorBright = ARGBF(255, jbc, jbc, jbc)

			iColorEye = ARGBF(0, 200, 100, 0)
			iColorEngine = ARGBF(0, 255, 0, 0)
			iColorFlare1 = ARGBF(sin(g_fTime * 10) + 20 * 64, 220, 100, 0)
			iColorFlare2 = ARGBF(sin(g_fTime * 25) + 20 * 64, 130, 150, 170)
			iColorFlare3 = ARGBF(sin(g_fTime * 3) + 20 * 64, 220, 100, 0)
		}
		return 0
	}

//-----------------------------------------------------------------
	void ~creature_dron()
	{
		if(EngineEffect)
		{
			delete EngineEffect
			EngineEffect = NULL
		}
	}

//-----------------------------------------------------------------
	void creature_dron()
	{
		Health = Dron1Health

		flags |= FL_CANOPENDOORS

		Humanoid = false

		if(!SoundSetName)
			SoundSetName = "Dron"

   	DefaultAnimSetName = "Dron"

		MinAttacksInRow[0] = 8
		MaxAttacksInRow[0] = 12
		MinAttackDelay[0] = 0.3
		MaxAttackDelay[0] = 0.3
		MinRestAfterAttack[0] = 2
		MaxRestAfterAttack[0] = 2

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.9
		MaxAttackDelay[1] = 0.9
		MinRestAfterAttack[1] = 2
		MaxRestAfterAttack[1] = 4

		fMinBulletRain = 1.1
		fShootpointTimeout = 2
		bFlameResistent = true

		CurrentWeapon = new item_weapon_dron()
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_pivot0)

		bLongRange = true
		bShortRange = false

		fZDamper = 10
		bAlignOnGround = false
		fMinCosForSlowTurn = cos(120 * DEG2RAD)
		fMaxStrafeDistance = 80

		LookBone = B_Body
		LookAxis = -3
		MainObject = GetObject("obj/creatures/dron01/dron01.xob")
		SetObject(MainObject, "$remap 'dron01_eye' 'null'; $remap 'dron01_engine' 'null'")

		hitobj = GetObject("obj/creatures/dron01/dron01_hit.xob")

		ShapeIndex = 0

		hs = GetSoundEx("physic_metal_default", 1, SF_3D)

		EngineEffect = new misc_particle_effector
		EngineEffect.SetParms("particle/dron_drive.ptc", "0 -7 5", false, 0)
		SetAngles(EngineEffect, "0 0 45")

		AddChild(CurrentWeapon, EngineEffect, 0)

		FSType = FSNone
	}
}

//=================================================================================
class ServiceBot extends AICreatureClass
{
//-----------------------------------------------------------------
void SoundFirstSight()
{
	Sound(6, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundSuspicious()
{
	Sound(6, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundDeath()
{
	Sound(21, SF_ONCE)
}

//-----------------------------------------------------------------
void SoundPain()
{
  Sound(20, SF_ONCE)
}

//-----------------------------------------------------------------
	void CreateBody()
	{
		dBodyCreate(this, ZeroVec, BC_NONE)

		geom = dGeomCreateCapsule(8, Vector(0, 0, 19.5))

		dBodyAddGeom(this, geom, g_vIdentMatrix, "material/creaturemetal")

		dBodySetMass(this, ZeroVec, 150)
	}

//-----------------------------------------------------------------
	void SetHeadOrientation(float pitch, float yaw)
	{
		float lquat[4], quat[4]
		float lquat2[4], quat2[4]
		vector mat[4]

		if(pitch || yaw)
		{
			clamp pitch<-45, 45>
			clamp yaw<-110, 110>

			QuatRotationRollPitchYaw(quat, Vector(yaw * -0.8, 0, pitch * -0.2))
			QuatRotationRollPitchYaw(quat2, Vector(yaw * -0.3, 0, pitch * -0.7))
		}
		else
		{
			QuatIdentity(quat)
			QuatIdentity(quat2)
		}

		QuatLerp(lquat, PLookQuat, quat, ftime * 9)
		QuatLerp(lquat2, PLookQuat2, quat2, ftime * 9)

		QuatMatrix(lquat, mat)
		SetBoneGlobal(this, B_Spine1, mat)

		QuatMatrix(lquat2, mat)
		SetBoneGlobal(this, B_Head, mat)

		QuatCopy(lquat, PLookQuat)
		QuatCopy(lquat2, PLookQuat2)
	}

//-----------------------------------------------------------------
	void ServiceBot()
	{
		flags |= FL_CANOPENDOORS

		RagdollMaterial = "material/metal"
		RagdollDef = "humanoidtest"
		RagdollMass = 70

		bLongRange = true
		bShortRange = true
		bFlameResistent = false

		fMinCosForSlowTurn = cos(60 * DEG2RAD)
		fMaxStrafeDistance = 72
		bAlignOnGround = false
		fZDamper = 10
		fADamper = 15

		LookAxis = -2
		LookBone = B_Hlava

		ShapeIndex = 0

  		SmallPainGroup = g_smallpain

		FSType = FSHumanoidRobot
	}
}

//=================================================================================
class creature_servicebot1 extends ServiceBot
{
	float LightsAnimTimer
	int 	LightsAnimFrame
	float LightsFadeOutTimer

//-----------------------------------------------------------------
	void EOnVisible(MinClass other, int extra)
	{
		if(status < STATE_DIE)
		{
			LightsAnimTimer += ftime
					
			if(LightsAnimTimer >= 0.25)	//4 virtualne FPS
			{
				LightsAnimTimer -= 0.25
				LightsAnimFrame = LightsAnimFrame + 1 % 4
				UVAnim = 0.25 * LightsAnimFrame
			}
		}
		else
		{
			LightsFadeOutTimer += ftime * 0.3

			if(LightsFadeOutTimer >= 1)
			{
				LightsFadeOutTimer = 1
				ClearEventMask(this, EV_VISIBLE)
			}
			else
				SetEventMask(this, EV_VISIBLE)

			float Intensity = 1 - LightsFadeOutTimer * 255			
			int iIntensity = Intensity
			Color2 = ARGB(0, iIntensity, iIntensity, iIntensity)	//staticke svetielka				// 		abgr
		}
	}

//-----------------------------------------------------------------
	void creature_servicebot1()
	{
		if(!SoundSetName)
			SoundSetName = "ServiceBot"

   	DefaultAnimSetName = "ServiceBot"
		
		Health = ServiceBot1Health

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1
		MaxRestAfterAttack[0] = 2

		MinAttacksInRow[1] = 6
		MaxAttacksInRow[1] = 12
		MinAttackDelay[1] = 0.2
		MaxAttackDelay[1] = 0.48
		MinRestAfterAttack[1] = 2.1
		MaxRestAfterAttack[1] = 2.3

		fMinBulletRain = 3.09

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_pivot0)
		CurrentWeapon.ShotStor.damage = WDmgServiceBot1
                
		MainObject = GetObject("obj/creatures/servicebot01/servicebot_01.xob")
		SelectObject(this, MainObject)
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)		
	}
}


//=================================================================================
class creature_servicebot2 extends ServiceBot
{         
	float LightsAnimTimer
	int 	LightsAnimFrame
	float LightsFadeOutTimer

//-----------------------------------------------------------------
	void EOnVisible(MinClass other, int extra)
	{
		if(status < STATE_DIE)
		{
			LightsAnimTimer += ftime
					
			if(LightsAnimTimer >= 0.25)	//4 virtualne FPS
			{
				LightsAnimTimer -= 0.25
				LightsAnimFrame = LightsAnimFrame + 1 % 4
				UVAnim = 0.25 * LightsAnimFrame
			}
		}
		else
		{
			LightsFadeOutTimer += ftime * 0.3

			if(LightsFadeOutTimer >= 1)
			{
				LightsFadeOutTimer = 1
				ClearEventMask(this, EV_VISIBLE)
			}
			else
				SetEventMask(this, EV_VISIBLE)

			float Intensity = 1 - LightsFadeOutTimer * 255			
			int iIntensity = Intensity
			Color2 = ARGB(0, iIntensity, iIntensity, iIntensity)	//staticke svetielka				// 		abgr
		}
	}
	
//-----------------------------------------------------------------
	void creature_servicebot2()
	{
		if(!SoundSetName)
			SoundSetName = "ServiceBot"

   	DefaultAnimSetName = "SecurityBot" //servicebot

		Health = ServiceBot2Health

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1
		MaxRestAfterAttack[0] = 1.4

		MinAttacksInRow[1] = 6
		MaxAttacksInRow[1] = 12
		MinAttackDelay[1] = 0.18
		MaxAttackDelay[1] = 0.36
		MinRestAfterAttack[1] = 1.5
		MaxRestAfterAttack[1] = 1.7

		fMinBulletRain = 4.1

		CurrentWeapon = new item_weapon_shotgun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_pivot0)
		CurrentWeapon.ShotStor.damage = WDmgServiceBot2
               

		MainObject = GetObject("obj/creatures/servicebot01/servicebot_02.xob")
		SetObject(MainObject, "$remap 'EnemyLights1' 'EnemyLights1_red';$remap 'EnemyLights2' 'EnemyLights2_red';")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
	}
}
