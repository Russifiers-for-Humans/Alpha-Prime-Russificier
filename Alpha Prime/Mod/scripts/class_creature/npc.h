//=================================================================================
class HumanoidNPC extends AICreatureClass
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

	vector min, max
	GetBoundBox(this, min, max)
	
	//float thick = max[0] - min[0] * 0.5
	float thick = 8.0
	float zheight = max[2] - min[2] * 0.5 - thick

	geom = dGeomCreateCapsule(thick, Vector(0, 0, zheight))

	dBodyAddGeom(this, geom, g_vIdentMatrix, "material/creatureflesh")
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

	if(PLookQuat[0] != lquat[0] || PLookQuat[1] != lquat[1] || PLookQuat[2] != lquat[2] || PLookQuat[3] != lquat[3])
	{
		QuatMatrix(lquat, mat)
		SetBoneGlobal(this, B_Spine1, mat)
		QuatCopy(lquat, PLookQuat)
	}
	
	if(PLookQuat2[0] != lquat2[0] || PLookQuat2[1] != lquat2[1] || PLookQuat2[2] != lquat2[2] || PLookQuat2[3] != lquat2[3])
	{
		QuatMatrix(lquat2, mat)
		SetBoneGlobal(this, B_Head, mat)
		QuatCopy(lquat2, PLookQuat2)
	}
}

//-----------------------------------------------------------------
void HumanoidNPC()
{
	ClassNum = 0

	flags |= FL_CANOPENDOORS

	fMinCosForSlowTurn = cos(120 * DEG2RAD)
	fMaxStrafeDistance = 72
	bAlignOnGround = false
	fZDamper = 10
	fADamper = 15

	LookAxis = -2
	LookBone = B_Hlava

	ShapeIndex = 0

	RagdollMaterial = "material/flesh"
	RagdollDef = "humanoidtest"
	RagdollMass = 50

	bLongRange = true
	bShortRange = false

	FSType = FSPlayer

}
}

//=================================================================================
class AISniperClass extends HumanoidNPC
{
//-----------------------------------------------------------------
//snipers don't care about height or AttackerLKP
//-----------------------------------------------------------------
float AttackerDistance2()
{
	if(Attacker == NULL)
		return 10000000  //FIXME: shouldn't happen!!!???

	vector from, t

	from = CurrentPos - HHVec

	t = GetOrigin(Attacker) - from

	DistanceVector = t
	DistanceVector[2] = 0
	VectorNormalize(DistanceVector)

	if(fabs(t[2]) < Height)
		t[2] = 0

	return VectorLength(t)
}

//-----------------------------------------------------------------
bool LRAttack()
{
	SetAnimState(t_idle)

	Attacker.AddAttacker(this)

local bool crouched = false
bool ret = true

	while(true)
	{
		Wait(Stopped == false)

		if(!IsState(STATE_LR_ATTACK))
		{
			ret = false
			goto getout
		}
		AttackStatus = 1

		//turn to attacker if we need that
		TurnByAnim(Attacker, 10)

		Sleep(AI_REFRESH)
		if(!IsState(STATE_LR_ATTACK))
		{
			ret = false
			goto getout
		}
	
		//we can reload now. Try it
		if(CurrentWeapon.NumActions >= 2 && CurrentWeapon.Ammo < 2)
		{
			bShooting = false

			Wait(CurrentWeapon.CanAct(2))

			if(!IsState(STATE_LR_ATTACK))
			{
				ret = false
				goto getout
			}

			//may changed meanwhile
			if(CurrentWeapon.Ammo < 2)
			{
				ReloadGun()
			}
		}

		SetAnimState(t_idle)
		local int prevs = status

		if(!crouched)
		{
			PlayAnimWait(s_walk - 1, a_crouch + 1, 10.0, g_all, 30, AF_ONCE)
			crouched = true
		}
		PlayAnim(s_walk - 1, a_crouch + 0, 10.0, g_all, 30, 0)

		NotInLOFTime = 0
		ShootOn(0, true)
		local int res = 0
		while(IsState(prevs))
		{
			float dist = AttackerDistance2()

			//We're too near to Attacker
			if(dist < AttackDistMin)
			{
				res = 1
				break
			}

			float dot = DistanceVector * facevec
			
			if(dot < 0.5) // >60'
			{
				res = 2
				break
			}
			

			if(NotInLOFTime > 5)
			{
				break
			}
			
			if(CurrentWP && !IsCurrentWPValid(false))
			{
				break
			}

			if(ShowAI)
				SetWidgetText(WAI, 4, "Shoot-Idle " + ftoa(NotInLOFTime))

			Sleep(AI_REFRESH)
		}
		ShootOff()

		if(!IsState(STATE_LR_ATTACK) || IsKilled(STATE_RETURN))
		{
			ret = false
			break
		}

		//just turn to attacker
		if(res == 2)
		{
			PlayAnimWait(s_walk - 1, a_crouch + 2, 10.0, g_all, 30, AF_ONCE)
			crouched = false
			
			continue
		}

		if(res == 1 && bShortRange) //prepni na shortrange
		{
			CPosition warea = GetWPos()
			if(IsConnected(warea, Attacker.GetWPos()))
			{
				if(BeginPath(Attacker, false) == PATH_OK)
				{
					SwitchFSM(STATE_CHASE)
					ret = false
					break
				}
			}
		}

		local info_waypoint wp = FindWaypoint(CurrentWP, AttackerLKP, AttackDistMin, 512, 2, Attacker)

		if(wp != NULL && CurrentWP != wp)
		{
			crouched = false
			PlayAnimWait(s_walk - 1, a_crouch + 2, 10.0, g_all, 30, AF_ONCE)
/*
if(_name == "heavyTrooper0")
{
	Print(wp)
	Print(CurrentWP)
}
*/
			Say(ST_CHANGINGWP)
			LRNextSP(wp)

			SetAnimState(t_idle)

			if(!IsState(STATE_LR_ATTACK))
			{
				ret = false
				break
			}
		}
		else
		{
			Sleep(frand(MinAttackDelay[iAttackType], MaxAttackDelay[iAttackType])) //at least time for shoot repeat
		}
		Sleep(AI_REFRESH) //avoid deadlock
	}

getout:
	if(crouched)
	{
		PlayAnimWait(s_walk - 1, a_crouch + 2, 10.0, g_all, 30, AF_ONCE)
	}
	
	return ret
}

//-----------------------------------------------------------------
void AILongRangeAttack()
{
	SetState(s_combat)
	AttackStatus = 1

	Sleep(AI_REFRESH)

	LRAttack()
}

}

//=================================================================================
class creature_faggioli extends HumanoidNPC
{
//-----------------------------------------------------------------
	void creature_faggioli()
	{
		if(!SoundSetName)
			SoundSetName = "Faggioli"

   	DefaultAnimSetName = "LightTrooper"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 60

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = FaggioliHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 3
		MaxRestAfterAttack[0] = 3.5

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.5
		MaxAttackDelay[1] = 1.0
		MinRestAfterAttack[1] = 4
		MaxRestAfterAttack[1] = 5

		fMinBulletRain = 1.5

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgFaggioli

		MainObject = GetObject("obj/creatures/prospector/prospector.xob")
		SetObject(MainObject, _materialremap)
	}
}

//=================================================================================
class creature_warren extends HumanoidNPC
{
//-----------------------------------------------------------------
	void creature_warren()
	{
		if(!SoundSetName)
			SoundSetName = "Warren"

   	DefaultAnimSetName = "LightTrooper"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 62

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = WarrenHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 2
		MaxRestAfterAttack[0] = 3

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.29
		MaxAttackDelay[1] = 0.72
		MinRestAfterAttack[1] = 3
		MaxRestAfterAttack[1] = 4

		fMinBulletRain = 3

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgWarren

		MainObject = GetObject("obj/creatures/waren/waren.xob")
		SetObject(MainObject, _materialremap)
	}
}

//=================================================================================
class creature_NPCcommander extends HumanoidNPC
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
	void creature_NPCcommander()
	{
		if(!SoundSetName)
			SoundSetName = "NPCCommander"

   	DefaultAnimSetName = "LightTrooper"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 60

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = NPCcommanderHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.8
		MaxRestAfterAttack[0] = 2.2

		MinAttacksInRow[1] = 9
		MaxAttacksInRow[1] = 19
		MinAttackDelay[1] = 0.22
		MaxAttackDelay[1] = 0.71
		MinRestAfterAttack[1] = 2.2
		MaxRestAfterAttack[1] = 3.3

		fMinBulletRain = 3.5

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgNPCcommander

		MainObject = GetObject("obj/creatures/commander/commander.xob")
		SetObject(MainObject, _materialremap)
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)		
	}
}

//=================================================================================
class creature_puppet extends HumanoidNPC
{
	string weapon
	string _materialremap
	FacialAnimationPlayer LipsyncEnt
	bool lefthanded
	float fEyesTarget, fEyesWeight, fEyesSpeed

//-----------------------------------------------------------------
	void M_Say(ExtClass other)
	{
		UpdateEntity(this)
		int hsnd = World.Dialog(this, other._value, SF_NOTEST)

		if(hsnd)
		{
			LipsyncEnt.PlayAnim(other._value, hsnd)
		}
	}

//-----------------------------------------------------------------
	void M_Shoot(ExtClass other)
	{
		string tokens[8]

		int nt = ParseString(other._value, tokens)

		if(nt < 1)
			return

		ExtClass ec = FindClass(tokens[0])

		int bone = B_Spine

		if(nt >= 3 && tokens[2])
		{
			if(!GetValue(NULL, "B_" + tokens[2], 0, bone))
				return
		}

		int numshots = 5

		if(nt >= 5 && tokens[4])
		{
			numshots = atoi(tokens[4])
		}

		while(numshots)
		{
			if(CurrentWeapon.CanAct(1))
			{
				vector bmat[4]
				if(!GetBoneMatrix(ec, bone, bmat))
					break

				CurrentWeapon.Action(1, bmat[3])
				numshots--
			}
			Sleep(5)
		}
	}

//-----------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		ClearEventMask(this, EV_FRAME)
	}
	
	float NextBlinkTime, StartBlinkTime
	float BlinkLength
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(fEyesTarget != -1)
		{
			if(fEyesWeight != fEyesTarget)
			{
				if(fEyesSpeed == 0)
				{
					fEyesWeight = fEyesTarget
				}
				else
				{
					float finc = ftime * (1 / fEyesSpeed)
					if(fEyesTarget < fEyesWeight)
					{
						fEyesWeight = fEyesWeight - finc
						if(fEyesWeight < fEyesTarget)
							fEyesWeight = fEyesTarget
					}
					else
					{
						fEyesWeight = fEyesWeight + finc
						if(fEyesWeight > fEyesTarget)
							fEyesWeight = fEyesTarget
					}
				}
				SetMorphState(this, "Mrkani_L", fEyesWeight)
				SetMorphState(this, "Mrkani_R", fEyesWeight)
			}
			return
		}

		if(Health == 0)
		{
			fEyesWeight = 0
			SetMorphState(this, "Mrkani_L", 0)
			SetMorphState(this, "Mrkani_R", 0)
			return
		}

		if(currenttime > NextBlinkTime)
		{
			StartBlinkTime = currenttime
			NextBlinkTime = currenttime + frand(3.0, 4.7)
			
			if(rand(0,100) > 80)
				BlinkLength = M_PI * 2
			else
				BlinkLength = M_PI
		}

		float t = currenttime - StartBlinkTime * 15.0

		if(t < BlinkLength)
		{
			
			fEyesWeight = fabs(sin(t))

			SetMorphState(this, "Mrkani_L", fEyesWeight)
			SetMorphState(this, "Mrkani_R", fEyesWeight)
		}
	}

//-----------------------------------------------------------------
	void BeforeInit()
	{
		if(Health == 0)
			Health = TechnicianHealth

		if(!SoundSetName)
			SoundSetName = "Technician"

   	DefaultAnimSetName = "Technician_pistol"
		
		switch(weapon)
		{
		case "item_weapon_dummy":
			CurrentWeapon = new item_weapon_dummy()
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

		case "item_weapon_rocketlauncher":
			CurrentWeapon = new item_weapon_rocketlauncher(this)
			break

		case "item_weapon_flamethrower":
			CurrentWeapon = new item_weapon_flamethrower(this)
			break

		case "item_weapon_jackhammer":
			CurrentWeapon = new item_weapon_jackhammer(this)
			break

		default:
			CurrentWeapon = new item_weapon_dummy()
			break
		}

		CurrentWeapon.Pick(this)
		if(lefthanded)
			SelectItem(CurrentWeapon, p_lhand)
		else
			SelectItem(CurrentWeapon, p_rhand)

		CurrentWeapon.ShotStor.damage = WDmgTechnician

		if(!model)
			model = "obj/creatures/technicist/technicist.xob"
			
		MainObject = GetObject(model)
		SetObject(MainObject, _materialremap)
		
		if(strstr(model, "livia.xob") != -1)
		{
			vector headmat[4] = {"1.075 0 0", "0 1 0", "0 0 0.94", ZeroVec}
//			SetBoneMatrix(this, B_Head, headmat)
			SetBoneMatrix(this, B_Neck, headmat)
		}
	}
	
//-----------------------------------------------------------------
	void EOnSoundEnd(MinClass other, int extra)
	{
		for(int n = 0; n < 4; n++)
		{
			if(World.DlgSound[n] == extra)
			{
				//it was cutscene dialog. Stop Lipsync player!
				World.DlgSound[n] = 0
				World.DlgID[n] = ""
				LipsyncEnt.StopAnim()
				return
			}
		}
	}
	
//-----------------------------------------------------------------
void M_Hide(ExtClass other)
{
	Show(false)
	for(int n = 0; n < sizeof(CSoundID); n++)
	{
		if(CSoundID[n])
			EndSound(CSoundID[n])
	}
}

//-----------------------------------------------------------------
void M_OpenEyes(ExtClass other)
{
	fEyesTarget = 0.0
	fEyesSpeed = atof(other._value)
}

//-----------------------------------------------------------------
void M_CloseEyes(ExtClass other)
{
	fEyesTarget = 1.0
	fEyesSpeed = atof(other._value)
}

//-----------------------------------------------------------------
void EndCutscene()
{
	ClearEventMask(this, EV_FRAME)
}

//-----------------------------------------------------------------
void BeginCutscene()
{
	SetEventMask(this, EV_FRAME)
}

//-----------------------------------------------------------------
   void creature_puppet()
   {
		GetHurtFactors(HumanHurtFactors)

		fEyesTarget = -1.0
		fEyesWeight = 0.0

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 50

   	DefaultAnimSetName = "LightTrooper"

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.8
		MaxRestAfterAttack[0] = 2.2

		MinAttacksInRow[1] = 9
		MaxAttacksInRow[1] = 18
		MinAttackDelay[1] = 0.5
		MaxAttackDelay[1] = 0.95
		MinRestAfterAttack[1] = 2.2
		MaxRestAfterAttack[1] = 3.8
	
		fMinBulletRain = 3.1
		
		ClearFlags(this, TFL_USER6)
		SetKind(this, DEFAULT)
		SetEventMask(this, EV_SOUNDEND)
		LipsyncEnt = new FacialAnimationPlayer(this)
	}
   
   void ~creature_puppet()
   {
   	//delete LipsyncEnt
   }
   
}

#include "scripts/class_creature/humanoids.h"
