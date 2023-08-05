//=================================================================================
class item_weapon_boss extends FireWeaponClass
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
	int PrimaryFire(vector target)
	{
		if(CheckForAmmo() == false)	//zistime ci mame strelivo na vystrel
			return false

		Ammo--
		int sfl = ClearFlags(Owner, TFL_SOLID)
		vector targ, dir
		float prec = 0

		if(Flash)
			Flash.Switch(true)

		vector start

		start = GetMatrix(this, 3)

		dir = target - start
		VectorNormalize(dir)

		Shooter shoter = Shooters[0]
		ShotStor.shooter = Owner
		target = target - start
		VectorNormalize(target)
		target = target * 8192 + start
		ShotStor.from = start
		ShotStor.to = target
		ShotStor.dir = dir

		ShotStor.precision = PrecisionFlawMax

		if(Flash)
			ShotStor.VisualsFrom = GetMatrix(Flash, 3)
		else
			ShotStor.VisualsFrom = start

		shoter.Fire(ShotStor)
		SetFlags(Owner, sfl)

		if(ActionSound)
			EndSound(ActionSound)

		ActionSound = PlaySound(Owner, Sounds[0], SF_ONCE)

		if(AttackAnim)
		{
			SetAnimSlot(Owner, s_weapon, AttackAnim, 10.0, 10.0, NULL, 30, AF_ONCE|AF_RESET|AF_BLENDOUT)
		}

		return true
	}

//-----------------------------------------------------------------
	int SecondaryFire(vector target)
	{
		return false
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
	void item_weapon_boss()
	{
		ClassNum = CBossWeapon

		AmmoPerMagazine = PistolAmmoPerMagazine
		AmmoType = AmmoPistol
		Ammo = -1
		NumActions = 1

		NumShooters = 1		
		Shooters[0] = new ShooterProjectile(this, AMMO_ROCKETBOSS)

		ADelay[0] = 1.5	//primary attack
		RicSndSkip = 3

		ShotStor.NumBullets = 1
		ShotStor.MarkSize = -1
		ShotStor.precision = 0.04
		ShotStor.StreakWidth = 12
		ShotStor.StreakLength = 30
		ShotStor.StreakSpeed = 5000
		ShotStor.StreakShader = 3
		ShotStor.damage = WDmgBoss
		ShotStor.power = 2
		ShotStor.CameraHitFactor = 1
		ShotStor.RicDecalType = DE_PLASMA		//DE_BULLET, DE_PLASMA
		ShotStor.RicSndType = SE_PLASMA			//SE_BULLET, SE_PLASMA
		ShotStor.RicParticlesType = PE_PLASMA	//PE_BULLET, PE_PLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		AttackAnim = GetObject("anm/boss/C_Fire_primary.anm")	

		Flash = new weapon_flash(this, 0.1, "-90 0 0", 1)
		Flash.rangle = ZeroVec

		InitSounds(WS_WeaponBoss)
	}
}

#ifndef DEMO
//=================================================================================
class creature_boss extends AICreatureClass
{
misc_particle_effector ParticleEmitor
int EmitorSound

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
/*
void SoundDeath()
{
	Sound(21, SF_ONCE)
}
*/
//-----------------------------------------------------------------
void SoundPain()
{
  Sound(20, SF_ONCE)
}

//-----------------------------------------------------------------
void CreateBody()
{
	dBodyCreate(this, ZeroVec, BC_NONE)

	vector min, max
	GetBoundBox(this, min, max)
	
	geom = dGeomCreateCapsule(23, Vector(0, 0, 13))
	vector mat[4] = {"1 0 0", "0 1 0", "0 0 1", "0 0 -3"}
	
	dBodyAddGeom(this, geom, mat, "material/creatureflesh")
	dBodySetMass(this, ZeroVec, 90)
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

		QuatRotationRollPitchYaw(quat, Vector(yaw * -0.5, 0, pitch * -0.2))
		//QuatRotationRollPitchYaw(quat, ZeroVec)
		
//		QuatRotationRollPitchYaw(quat2, Vector(yaw * -0.3, 0, pitch * -0.7))
		QuatRotationRollPitchYaw(quat2, Vector(yaw * -0.5, 0, pitch * -0.8))
	}
	else
	{
		QuatIdentity(quat)
		QuatIdentity(quat2)
	}

	QuatLerp(lquat, PLookQuat, quat, ftime * 9)
	QuatLerp(lquat2, PLookQuat2, quat2, ftime * 9)

	if(PLookQuat[0] != lquat[0] || PLookQuat[1] != lquat[1] || PLookQuat[2] != lquat[2] || PLookQuat[3] != lquat[3])
	{
		QuatMatrix(lquat, mat)
		SetBoneGlobal(this, B_Spine1, mat)
		QuatCopy(lquat, PLookQuat)
	}
	
	if(PLookQuat2[0] != lquat2[0] || PLookQuat2[1] != lquat2[1] || PLookQuat2[2] != lquat2[2] || PLookQuat2[3] != lquat2[3])
	{
		QuatMatrix(lquat2, mat)
		SetBoneGlobal(this, B_RightArmRoll, mat)
		QuatCopy(lquat2, PLookQuat2)
	}
}

//-----------------------------------------------------------------
bool TestShootFilter(ExtClass other, vector raypos, vector raydir)
{
	switch(other.ClassType)
	{
	case ClassTypeMiscPhysicsGlass:
		return false

	case ClassTypeMiscShootDetector:
		return other.IsShot(raypos, raydir)

	case ClassTypeMiscShootingShield:
		return other.IsShot(raypos, raydir)		
	}
	
	return true
}
//-----------------------------------------------------------------
CreatureClass TestShoot(ExtClass enemy, vector to)
{
	MinClass cent = NULL
	float endplane[4]

	vector ttpos
	vector from = GetMatrix(CurrentWeapon, 3)
	vector off1 = GetMatrix(CurrentWeapon, 0) * 0.75
	vector off2 = GetMatrix(CurrentWeapon, 2) * 0.75

	int fl = ClearFlags(this, TFL_SOLID)

//1st try
	shootpos = to + "0 0 20"

	ttpos = shootpos - off1 - off2
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from - off1 - off2, ttpos)
	TraceLineEx(DEFAULT, from - off1 - off2, ttpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	if(cent == enemy)
	{
		SetFlags(this, fl)
		return cent
	}
	ttpos = shootpos + off1 - off2
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from + off1 - off2, ttpos)
	TraceLineEx(DEFAULT, from + off1 - off2, ttpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	if(cent == enemy)
	{
		SetFlags(this, fl)
		return cent
	}
	ttpos = shootpos + off2
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from + off2, ttpos)
	TraceLineEx(DEFAULT, from + off2, ttpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	if(cent == enemy)
	{
		SetFlags(this, fl)
		return cent
	}

//2nd try
	shootpos = to

	ttpos = shootpos - off1 - off2
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from - off1 - off2, ttpos)
	TraceLineEx(DEFAULT, from - off1 - off2, ttpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	if(cent == enemy)
	{
		SetFlags(this, fl)
		return cent
	}
	ttpos = shootpos + off1 - off2
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from + off1 - off2, ttpos)
	TraceLineEx(DEFAULT, from + off1 - off2, ttpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	if(cent == enemy)
	{
		SetFlags(this, fl)
		return cent
	}
	ttpos = shootpos + off2
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from + off2, ttpos)
	TraceLineEx(DEFAULT, from + off2, ttpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)

	SetFlags(this, fl)
	return cent
}

//-----------------------------------------------------------------
bool CanHitTarget(vector tpos)
{
	if(atype != t_idle)
		return false

	vector mat[4]
	GetBoneMatrix(this, B_RightHand, mat)

	float dot
	vector aimvec1, aimvec2
	
	aimvec2 = mat[2] * -1
	dot = UpVec * aimvec2

	//gun is pointing mostly vertically
	if(fabs(dot) > 0.5)
		return false

	aimvec1 = tpos - mat[3]

	dot = aimvec1 * aimvec1
	if(dot < 96*96)
		return false

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
void FlameOn()
{
	if(EmitorSound)
	{
		EndSound(EmitorSound)
		EmitorSound = 0
	}

	ParticleEmitor.Switch(true)
	EmitorSound = PlaySound(this, CurrentWeapon.Sounds[3], 0)
}

//-----------------------------------------------------------------
void FlameOff()
{
	ParticleEmitor.Switch(false)
	if(EmitorSound)
	{
		EndSound(EmitorSound)
		EmitorSound = 0
	}
}

//-----------------------------------------------------------------
void AIChase()
{
	Attacker.AddAttacker(this)
	SetState(s_combat)

	SetAnimState(t_run)
	local bool stand = false

	while(IsState(STATE_CHASE))
	{
		AISleep(AI_REFRESH)

		if(Attacker.status >= STATE_DIE)
		{
			ShootOff()
			AIKillEvents()
			SwitchFSM(STATE_RETURN)
			FlameOff()
			return
		}

		local float enemydist = AttackerDistance()

		if(!stand)
		{
			int res
			
			if(enemydist < 256)
				res = UpdatePath(t_run, 0.5)
			else
				res = UpdatePath(t_run, 2.0)
	
			if(res != PATH_OK && res != PATH_FINISHED)
			{
				//Print("beginlosttarget")
				ShootOff()				
				BeginCombatTarget(Attacker)
				FlameOff()
				return
			}
			
			CPosition area

			area = GetWPos()

			if(!IsConnected(Attacker.GetWPos(), area))
			{
				ShootOff()
				BeginCombatTarget(Attacker)
				FlameOff()
				return
			}
		}

		enemydist = AttackerDistance()

		if(enemydist > AttackDist)
		{
			ShootOff()
			SwitchFSM(STATE_GUARD)
			FlameOff()
			return
		}

		if(bLongRange && enemydist > AttackDistMin) //prepni na longrange
		{
			ShootOff()
			FlameOff()

			if(StateAge(STATE_CLOSECOMBAT) > 3.5 && DistanceBetween(Attacker) < 256.0 && InFront(Attacker, 0.5))
			{
				SwitchFSM(STATE_CLOSECOMBAT)
			}
			else
			{
				SwitchFSM(STATE_LR_ATTACK)
			}
			
			return
		}

		float dot = facevec * DistanceVector
		if(dot > 0.85 && enemydist < 128)
		{
			if(!ParticleEmitor.SwitchStatus)
			{
				FlameOn()
			}
		}
		else
		{
			if(ParticleEmitor.SwitchStatus)
			{
				FlameOff()
			}
		}

		if(stand)
		{
			//some hysteresis, to avoid oscilation
			if(enemydist > AttackDistClose + 24 || !CheckDirectPath(Attacker))
			{
				SetAnimState(t_run)
				stand = false
			}
			else
			{
				if(CurrentWeapon.Action && ASet.Anims[a_combat_idle_fire])
					SetAnimState(t_idle_fire)
				else
					SetAnimState(t_idle)
					
				TurnByAnim(Attacker, 30)
			}
		}
		else
		{
			if(enemydist <= AttackDistMin)
			{
				if(!bShooting)
					ShootOn(0, true)

				if(enemydist <= AttackDistClose && CheckDirectPath(Attacker))
				{
					SetAnimState(t_idle)
					stand = true
					ResetTurn()
				}
			}
			else
			{
				if(bShooting)
					ShootOff()
			}			
		}
	}
	ShootOff()
	FlameOff()
}

	ShotStorage FlameParms
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(ParticleEmitor.SwitchStatus)
		{
			vector mat[4]
			GetBoneMatrix(this, B_Hlava, mat)
			
			vector from = mat[3]
			vector to = GetOrigin(Attacker)
			vector dir = to - from
			VectorNormalize(dir)
/*
			vector dir = mat[2] * -1 + "0 0 0.35"
			VectorNormalize(dir)
			vector to = dir * 128 + from
*/		
			//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, from, to)

			float plane[4]

			int pfl = ClearFlags(this, TFL_SOLID)

			if(TraceLineEx(DEFAULT, from, to, ZeroVec, ZeroVec, other, plane, NULL, NULL, TRACE_DEFAULT|TRACE_BONES, NULL) < 1)
			{
				if(other.ClassType == ClassTypeCreature)
				{
					FlameParms.from = from
					FlameParms.VisualsFrom = from
					FlameParms.to = to
					FlameParms.dir = dir
					FlameParms.shooter = this
					FlameParms.damage = WDmgBoss2 * ftime
					FlameParms.ID = World.ShootID++
					FlameParms.Bone = g_iTraceBone
					FlameParms.Shoot(other)
				}
			}
			SetFlags(this, pfl)
		}
	}

//-----------------------------------------------------------------
	void TraceEnd()
	{
		/*
		vector mat[4], mid
		GetBoneMatrix(this, B_LeftHand, mat)
		mid = mat[3]
		GetBoneMatrix(this, B_RightHand, mat)
		mid = mid + mat[3] * 0.5

		misc_particle_effector eff = new misc_particle_effector
//		eff.SetParms("particle/bosshit.ptc", mid, true, false)
		eff.SetParms("particle/tachyonexplode.ptc", mid, true, false)
		eff.Switch(true)
*/
		iTraceHit = 0
		CurrentWeapon.TraceEnd()
	}


//-----------------------------------------------------------------
int AIIdle(local ExtClass target, local int itype = it_default, local float wakedist = 0)
{
	SetAnimState(t_idle)
	local float ft = 0
	local int prevs = status
	local int res = 0

	ResetTurn()
	while(RestTime != 0 && IsState(prevs))
	{
		if(ShowAI)
			SetWidgetText(WAI, 4, "AIIdle " + ftoa(RestTime))

		if(target && wakedist > 0)
		{
			if(Distance(target) < wakedist)
			{
				res = 1
				goto getout
			}
		}

		AISleep(AI_SLOWREFRESH)
		ft += AI_SLOWREFRESH

		if(ft >= 100)
		{
			if(ShowAI)
				SetWidgetText(WAI, 4, "AIIdle " + ftoa(RestTime) + " IdleVar")

			ft = 0

			int anim = -1

			switch(itype)
			{
			case it_default:
				if(astate == s_combat)
					anim = a_combat_idle_var + rand(0, NumCombatIdleVarAnims)
				else
				{
					if(astate == s_alert)
						anim = a_alert_idle_var + rand(0, NumAlertIdleVarAnims)
					else
						anim = a_relax_idle_var + rand(0, NumRelaxIdleVarAnims)
				}
				break

			case it_fight:
				if(astate == s_combat)
					anim = a_combat_idle_var + rand(0, NumCombatIdleVarAnims)
				else
				{
					if(astate == s_alert)
						anim = a_alert_idle_var + rand(0, NumAlertIdleVarAnims)
					else
						anim = a_relax_idle_var + rand(0, NumRelaxIdleVarAnims)
				}
				break

			case it_lost:
				anim = a_lost_idle_var + rand(0, NumLostIdleAnims)
				break
			}

			if(anim >= 0)
			{
				PlayAnimWait(s_idlevar, anim, 10.0, g_all, 30, AF_ONCE)
			}
		}

		if(target)
		{
			ft += TurnByAnim(target, 50)
		}
	}

getout:
	if(ShowAI)
		SetWidgetText(WAI, 4, "")

	return res
}

//-----------------------------------------------------------------
	void ~creature_boss()
	{
		delete FlameParms
		delete ParticleEmitor
	}

//-----------------------------------------------------------------
	void creature_boss()
	{
		if(!SoundSetName)
			SoundSetName = "Boss"

		DefaultAnimSetName = "Boss"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = ""
		RagdollMass = 200

		bLongRange = true
		bShortRange = true
		bNoLOFLatency = true

		AttackDistMin = 300
		AttackDistClose = 128
		AttackDist = 2048

		fMinCosForSlowTurn = cos(120 * DEG2RAD)
		fMaxStrafeDistance = 72
		bAlignOnGround = true
		fZDamper = 10
		fADamper = 15

		fMaxRange = 768

		fCloseHitRange = 80

		LookAxis = -3
		LookBone = B_Head

		ShapeIndex = 2

		fShootpointTimeout = 5.0
		fNotInLOFTimeout = 3.5

		FSType = FSPlayer

		Health = BossHealth

		MinAttacksInRow[0] = 1
		MaxAttacksInRow[0] = 1
		MinAttackDelay[0] = 2.5
		MaxAttackDelay[0] = 2.5
		MinRestAfterAttack[0] = 2
		MaxRestAfterAttack[0] = 3

		MinAttacksInRow[1] = 1
		MaxAttacksInRow[1] = 1
		MinAttackDelay[1] = 2.5
		MaxAttackDelay[1] = 2.5
		MinRestAfterAttack[1] = 2
		MaxRestAfterAttack[1] = 3

		fMinBulletRain = 25

		CurrentWeapon = new item_weapon_boss()
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)

		//Jabbovina - nemame pivot, takze je zbran otocena uplne blbe
		SetAngles(CurrentWeapon, "180 0 90")
		MainObject = GetObject("obj/creatures/boss/boss.xob")
		SetObject(MainObject, _materialremap)

		ParticleEmitor = new misc_particle_effector
		ParticleEmitor.SetParms("particle/boss_firestream.ptc", ZeroVec, true, false)
		SetAngles(ParticleEmitor, "180 0 0")
		AddChild(this, ParticleEmitor, p_lhand)
		ParticleEmitor.Switch(false)

		FlameParms = new ShotStorage
		FlameParms.HurtType = HT_FLAME
		FlameParms.NumBullets = 1
		FlameParms.MarkSize = 32
		FlameParms.RicDecalType = DE_BULLET
		FlameParms.RicSndType = SE_BULLET
		FlameParms.RicParticlesType = PE_BULLET
		FlameParms.CameraHitFactor = 0
		FlameParms.power = 30
	}
}

#endif