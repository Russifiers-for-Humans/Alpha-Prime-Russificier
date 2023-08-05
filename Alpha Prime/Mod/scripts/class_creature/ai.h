const float MIN_GRENADE_DIST = 0//96	//3metru
const float MAX_GRENADE_DIST = 543	//16metru
const float MIN_GRENADE_PERIOD = 0.35 //nejvic 1 granat za 5sec

class BleedSource extends MinClass
{
	vector LMatrix[4]
	int Bone
	float BirthTime
	CreatureClass Owner
	
	void SetupMatrix(vector pos, vector vec)
	{
		vector vec1
		vector vec2
		vec1[0] = vec[1]
		vec1[1] = vec[2]
		vec1[2] = vec[0]
		vec2 = vec * vec1
		vec1 = vec2 * vec
		VectorNormalize(vec1)
		VectorNormalize(vec2)
	
		LMatrix[0] = vec1
		LMatrix[1] = vec2
		LMatrix[2] = vec
		LMatrix[3] = pos
		
		vector mat[4]
		vector mat2[4]

		if(GetBoneMatrix(Owner, Bone, mat)
		{
			MatrixMultiply4(mat, LMatrix, mat2)
			SetMatrix(this, mat2)
		}
	}

	void BleedSource(vector lvec, vector lpos, int bone, CreatureClass owner)
	{
		Bone = bone
		BirthTime = currenttime
		Owner = owner
		
		SetFlags(this, TFL_FULLMATRIX|TFL_VISIBLE)
		SetupMatrix(lpos, lvec)
		SetObject(GetObject("particle/physx/fluidbleedtest.ptc"))
	}	
}


const int AnimRunFire[4] = {t_run, t_run_fr, t_run_fb, t_run_fl}
const int AnimWalkFire[4] = {t_run_aim, t_lstrafe, t_brun, t_rstrafe}

const int QDiff[4][4] = {
	0, 1, 2, 1,
	1, 0, 1, 2,
	2, 1, 0, 1,
	1, 2, 1, 0}

float g_Ticktime = 0

//=================================================================================
class SSource extends MinClass
{
	vector LMatrix[4]
	int Bone
	float BirthTime
	CreatureClass Owner
	
	void SetupMatrix(vector pos, vector vec)
	{
		vector vec1
		vector vec2
		vec1[0] = vec[1]
		vec1[1] = vec[2]
		vec1[2] = vec[0]
		vec2 = vec * vec1
		vec1 = vec2 * vec
		VectorNormalize(vec1)
		VectorNormalize(vec2)
	
		LMatrix[0] = vec1
		LMatrix[1] = vec2
		LMatrix[2] = vec
		LMatrix[3] = pos
		
		vector mat[4]
		vector mat2[4]

		if(GetBoneMatrix(Owner, Bone, mat)
		{
			MatrixMultiply4(mat, LMatrix, mat2)
			SetMatrix(this, mat2)
		}
	}

	void SSource(vector lvec, vector lpos, int bone, CreatureClass owner)
	{
		Bone = bone
		BirthTime = currenttime
		Owner = owner
		
		SetFlags(this, TFL_FULLMATRIX|TFL_VISIBLE)
		SetupMatrix(lpos, lvec)
		SetObject(GetObject("particle/hit_blood1.ptc"))
	}	
}



//=================================================================================
class AICreatureClass extends CreatureClass
{
//events
string	OnDeath, OnDie, OnFirstSight, OnCombat, OnLostTarget
string	OnActivate, OnLowHealth

//keys
float		lowhealth
float		visrange,halfrange,quarterrange

//---------------------------
//Assets and definitions

float		fMinCosForSlowTurn
float		fMaxStrafeDistance
float		fMinBulletRain
float		fVisibilityLatency
float		fAlertedVisibilityLatency
float		fLeanShootTime
float		fCrouchShootTime
float		fNotInLOFTimeout	//how long it will take until AI will change shooting point when target is not in LOF
									//(just for combat/guardcombat tactics)
float		fMaxRange //max distance to waypoint

int		MinAttacksInRow[4], MaxAttacksInRow[4]
float		MinAttackDelay[4], MaxAttackDelay[4]
float		MinRestAfterAttack[4], MaxRestAfterAttack[4]

float		MinStunTime, MaxStunTime
float		fShootpointTimeout	//period for changing waypoints
									//(just for berserk/combat/guardcombat tactics)

float		fCloseHitRange
float		MinRoamIdleTime, MaxRoamIdleTime

bool		bFlameResistent
bool		bAngledWalk
bool		bCanLean
bool		bCanCrouch
bool		bHasGestures
bool		bCanConfirmGesture
bool		bNoLOFLatency
bool		bCanFireInCrouch
bool		bCanShootInTakeCover
bool		bHasSpotAnim
bool		bCanThrowGrenade
bool		bCanThrowGrenadeInCrouch
bool		bCanUseConnections

bool		bLongRange, bShortRange

int		SmallPainGroup

int		pain
int		PainTimer

bool		AILocked
bool		bShooting

int		AttackDist, AttackDistMin, AttackDistClose


float		TurnSpeed
float		WeaponRange

int		NumRelaxIdleAnims
int		NumRelaxIdleVarAnims
int		NumAlertIdleAnims
int		NumAlertIdleVarAnims
int		NumCombatIdleAnims
int		NumCombatIdleVarAnims
int		NumSpotIdleAnims
int 		NumFightIdleAnims
int		NumLostIdleAnims
int		NumCloseCombatAnims

string	RagdollDef
string	RagdollMaterial
float		RagdollMass

string	AnimSetName, DefaultAnimSetName

int		UsedOn, UsedOff //maska pro used propety info_waypoint. WP_USED_ON_ENEMY/WP_USED_ON_RAT/WP_USED_OFF_ENEMY/WP_USED_OFF_RAT

SpeechSet SSet

//-----------------------------------------------------------------
//runtime
//-----------------------------------------------------------------
#ifdef DEVELOPER
int		WDLine
#endif

dGeom		geom

//onground control
bool 		notonground


int		CCount
bool		Stopped
bool		bOnLowHealth
ExtClass GuardTarget

vector	AttackerLKP //last known position
float		AttackerLKPTime

vector	DangerPos

float		OnWPTime
bool		WPIsNotValid

//list of observed items
ExtClass Observed[ObservedBufferSize]
float		ObservedTime[ObservedBufferSize]
int		NumObserved


//AI buffer
bool		AI_Used[AIBufferSize]
float		AI_Time[AIBufferSize]
MinClass	AI_Ent[AIBufferSize]
vector	AI_Pos[AIBufferSize]	//position where occured

int		AI_ActEvents

int		VisTime

CreatureClass LostTarget

float			BulletRainCounter
info_waypoint OriginWaypoint, TempWaypoint, WalkWaypoint
info_waypoint CurrentWP
float			RestTime

int			AICmd
string		AIParm

bool			FirstSight, sfirst

info_squad	Squad
int			AttackStatus //used by squad to qualify who is doing what

FireWeaponClass CurrentWeapon

int			CmdStack[64]
string		CmdParmStack[64][8]
int			CmdStackPos

//CreatureClass Targets[16]
//int			NumTargets

CreatureClass Target

int			LastAmbientSound
vector		DistanceVector
string		LastPathError
float			LastPathTolerance


info_waypoint	Waypoints[64]
int			NumWaypoints

ExtClass		Unreachable[32]
float			UnreachableTime[32]
int			NumUnreachable

info_waypoint	UsedWaypoint
info_waypoint	LRUWaypoints[2]
int				LRUTop

ExtClass		_TurnTarget
float			ShootTime
ExtClass		ShootTarget

int			iNoMotion

int			iTraceHit

float NotInLOFTime
float LastLOFTime

//-----------------------------------------------------------------
// Senses
//-----------------------------------------------------------------
ExtClass		VisibleTargets[16]
float			VTLastTime[16]
float			VTFirstTime[16]
int			NumVisibleTargets


//-----------------------------------------------------------------
// Movement solver
//-----------------------------------------------------------------
float			DistTolerance
float			LastPathTime
ExtClass		WTarget
CPosition	WPosition2
CPath			WPath
bool			WPathContainJumps //is there jump on the way?

int			WNum //index on path
vector		WPos
float			WTargetDist
FuncClass	WaitForDoors
vector		WVector

bool			WTurn
float			WAddAngle	//Angular displacement for walk
float			turnspeed, turnangle, turndist

float			TickTime
vector		MVec
float			MVecLen
bool StopThread

//-----------------------------------------------------------------
void BeforeBodyDestroy()
{
	Projectile cur = FirstProjectile
	while(cur)
	{
		cur.OnShooterBodyRemove()
		cur = cur.NextProjectile
	}
	FirstProjectile = NULL
}

//-----------------------------------------------------------------
void M_Remove(ExtClass other)
{
	//KillThread(this, "FSMThread")
	Stopped = true
	Show(false)
	ClearEventMask(this, -1)
	ClearFlags(this, -1)
	/*
	if(ragdollActive)
	{
		dRagdollDestroy(this)
		ragdollActive = false
	}	*/
}

//-----------------------------------------------------------------
void M_SetAttackRange(ExtClass other)
{
	AttackDist = atoi(other._value)
}

//-----------------------------------------------------------------
void M_SetState(ExtClass other)
{
	strlower(other._value)

	switch(other._value)
	{
	case "combat":
		SetState(s_combat)
		break

	case "relaxed":
		SetState(s_relaxed)
		break

	default:
		DPrint(_name + ".SetState(): Unknown state " + other._value)
		ErrorNum++
		break
	}
}

//-----------------------------------------------------------------
void AISleep(float sleep)
{
	Wait(Stopped == false)
	Sleep(sleep)
}

//-----------------------------------------------------------------
bool WTraceFilter(CreatureClass other, vector raypos, vector raydir)
{
	if(other == this || !IsInherited(other, CExtClass))
		return false

	if(other.ClassType == ClassTypeMiscPhysicsGlass || other.ClassType == ClassTypeMiscShootDetector || other.ClassType == ClassTypeMiscShootingShield)
		return other.IsShot(raypos, raydir)

	//get rid of this fucking railing
	if(other.ClassType == ClassTypeMiscModel)
	{
		misc_model mm = other

		if(mm.railing)
			return false
		
		return true
	}

	if(other.ClassType == ClassTypeCreature)
	{
		if(other.Disabled)
			return false

		//if(GroupID != other.GroupID)
			//return true
	}

	return true
}

//-----------------------------------------------------------------
ExtClass GetTarget(string parm, int types = 0)
{
	if(parm == "player")
		return World.Player

	ExtClass targ = FindClass(parm)

	if(targ == NULL)
	{
		DPrint(_name + ".AddCmd/GuardTarget/SetTarget/SetWaypoints can't found target " + parm)
		ErrorNum++
	}
	else
	{		
		switch(types)
		{
		case 1:
			if(targ.ClassType != CTypeInfoWaypoint)
			{
				DPrint(_name + ".AddCmd/GuardTarget/SetTarget trying to set " + ClassName(targ) + " as target " + parm)
				targ = NULL
				ErrorNum++
			}
			break

		case 2:
			if(targ.ClassType != CTypeInfoWaypoint && targ.ClassType != CTypeInfoWpGroup)
			{
				DPrint(_name + ".SetWaypoints trying to set " + ClassName(targ) + " as target " + parm)
				targ = NULL
				ErrorNum++
			}
			break			
		}
	}
	return targ
}

//-----------------------------------------------------------------
int TryFindPath(ExtClass target)
{
	if(GetWPos() == NULL)
	{
		return 0
	}

	int res
	float tcost = 0
	float cost = 0

	if(agent)
	{
		cost = agent.GetTraverseCost()
		agent.SetTraverseCost(1.0)
	}

	if(!World.pPathFinder.FindClosestPosition(ShapeIndex, WPosition, WPosition, Radius * 2))
	{
		LastPathError = "TryFindPath: actor is obstructed"
		res = 0
		goto getout
	}
	CPosition pos2 = target.GetWPos()

	if(pos2 == NULL)
	{
		LastPathError = "TryFindPath: target '" + target._name + "' out of NAV mesh"
		res = 0
		goto getout
	}

 	if(target.agent)
 	{
 		tcost = target.agent.GetTraverseCost()
 		target.agent.SetTraverseCost(1.0)
 	}

	CPosition cpos2 = new CPosition
	if(!World.pPathFinder.FindClosestPosition(ShapeIndex, pos2, cpos2, Radius * 1))
	{
		LastPathError = "TryFindPath: target '" + target._name + "' is obstructed"
		res = 0
		goto getout
	}

	if(!IsConnected(WPosition, cpos2))
	{
		LastPathError = "TryFindPath: target '" + target._name + "' is not in the same group"
		res = 0
		goto getout
	}


	CPath path = World.pPathFinder.FindShortestPath(ShapeIndex, WPosition, cpos2, 1 - bCanUseConnections)

	if(path == NULL)
	{
		LastPathError = "TryFindPath: can't found path to target '" + target._name + "'"
		res = 0
	}
	else
	{
		path.Destroy()
		path = NULL
		res = true
	}

getout:

	if(cpos2)
	{
		delete cpos2
	}

	if(tcost && target.agent)
	{
 		target.agent.SetTraverseCost(tcost)
	}

	if(cost)
	{
 		agent.SetTraverseCost(cost)
	}

	return res
}

//-----------------------------------------------------------------
void M_SetWaypoints(ExtClass other)
{
	NumWaypoints = 0
	
	string p = other._value
	string token
	int cgroup = -1
	
	if(GetWPos())
	{
		cgroup = WPosition.group
	}

	while(NumWaypoints < 64)
	{
		int t = ParseStringEx(p, token)

		if(t != 3)
		{
			DPrint(_name + ".SetWaypoints has syntax error")
			ErrorNum++
			return
		}

		info_waypoint w = GetTarget(token, 2)

		if(w)
		{
			if(w.ClassType == CTypeInfoWpGroup)
			{
				info_wpgroup wgr = w

				for(int n = 0; n < wgr.NumWaypoints; n++)
				{
					w = wgr.PWaypoints[n]

					if(w.WPosition)
					{
						if(w.WPosition.group != cgroup)
						{
							DPrint(".SetWaypoints error! Waypoint: " + w._name + "has different nav-group than " + _name)
						}
					}

					Waypoints[NumWaypoints++] = w

					if(NumWaypoints >= 64)
						return
				}
			}
			else
			{
#ifdef DEVELOPER
				if(!TryFindPath(w))
				{

					DPrint(_name + ".SetWaypoints warning! Reason: " + LastPathError)
				}
#endif
				if(w.WPosition)
				{
					if(w.WPosition.group != cgroup)
					{
						DPrint(".SetWaypoints error! Waypoint: " + w._name + "has different nav-group than " + _name)
					}
				}

				Waypoints[NumWaypoints++] = w
			}
		}

		t = ParseStringEx(p, token)

		if(t == 0)
			break

		if(token != ",")
		{
			DPrint(_name + ".SetWaypoints has syntax error")
			ErrorNum++
			return
		}
	}
}

//-----------------------------------------------------------------
void M_SetTarget(ExtClass other)
{
	CreatureClass cc = GetTarget(other._value)
	if(cc)
		Target = cc
}

//-----------------------------------------------------------------
void M_SetGuardTarget(ExtClass other)
{
	CreatureClass cc = GetTarget(other._value, 1)
	if(cc)
		GuardTarget = cc
}

//-----------------------------------------------------------------
void Say(int type)
{
	if(SSet == NULL)
		return

	if(!SSet.HasSounds(type))
		return

	float d

	//didn't we said something recently?
	d =  currenttime - LastSayTime
	if(d < 1.0)
		return

	World.SManager.Say(this, type)
}

//-----------------------------------------------------------------
float Distance(CreatureClass target)
{
	vector from, t

	from = CurrentPos - HHVec

	if(IsInherited(target, CInfoWaypoint))
	{
		info_waypoint wp = target
		t = wp.floor
	}
	else
	{
		if(target.ClassType == ClassTypeCreature)
			t = target.CurrentPos - target.HHVec
		else
			t = GetOrigin(target)
	}

	t = t - from

	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, from, to)

	if(fabs(t[2]) < Height)
		t[2] = (float)1

	DistanceVector = t

	return VectorNormalize(DistanceVector)
}

//-----------------------------------------------------------------
float DistanceBetween(CreatureClass target)
{
	return Distance(target) - Radius - target.Radius
}

//-----------------------------------------------------------------
float AttackerDistance()
{
	if(Attacker == NULL)
		return 10000000  //FIXME: shouldn't happen!!!???

	vector from, t

	from = CurrentPos - HHVec

	if(Attacker.ClassType == ClassTypeCreature)
		t = AttackerLKP - Attacker.HHVec
	else
		t = GetOrigin(Attacker)

	t = t - from

	DistanceVector = t
	DistanceVector[2] = 0
	VectorNormalize(DistanceVector)

	if(fabs(t[2]) < Height)
		t[2] = 0

	return VectorLength(t)
}

//-----------------------------------------------------------------
void AIKillEvents()
{
	for(int n = 0; n < AIBufferSize; n++)
		AI_Used[n] = false

	AI_ActEvents = 0
}

//-----------------------------------------------------------------
// status
//-----------------------------------------------------------------
int Alertable()
{
	if(status == STATE_SCRIPT)
		return false

	if(status >= STATE_WALK && status <= STATE_LOST_TARGET)
		return true

	return false
}

//-----------------------------------------------------------------
int Alerted()
{
	if(status >= STATE_1STSIGHT && status < STATE_COVER)
		return true

	return false
}

//-----------------------------------------------------------------
float Turn2TargetDiff(vector tpos, float adisp = 0)
{
	vector targetvec = tpos - CurrentPos
	targetvec[2] = 0

	turnangle = Vector2Yaw(targetvec) + adisp

	return DiffAngle(turnangle, faceyaw)
}

//-----------------------------------------------------------------
void ResetTurn()
{
	turnspeed = 0
	turndist = 0
	WTurn = false
}

//-----------------------------------------------------------------
float TurnToTarget(vector tpos, float s, float adisp = 0)
{
	turnspeed = s

	//otacej k cili
	turndist = Turn2TargetDiff(tpos, adisp)

	//good enough direction
	if(fabs(turndist) < 1.0)
		turndist = 0

	return turndist
}

//-----------------------------------------------------------------
void CAmbientSoundEnd()
{
	if(LastAmbientSound >= 0)
	{
		//there is different sound playing. Interrupt it
		if(CSoundID[LastAmbientSound])
		{
			EndSound(CSoundID[LastAmbientSound])
			CSoundID[LastAmbientSound] = 0
		}
		LastAmbientSound = -1
	}
}

//-----------------------------------------------------------------
void CAmbientSound(int n)
{
	if(LastAmbientSound != n)
	{
		if(LastAmbientSound >= 0)
		{
			//there is different sound playing. Interrupt it
			if(CSoundID[LastAmbientSound])
			{
				EndSound(CSoundID[LastAmbientSound])
				CSoundID[LastAmbientSound] = 0
			}
		}
	}
	else
	{
		//sound is playing already
		if(CSoundID[LastAmbientSound])
			return
	}

	LastAmbientSound = n
	//Play new sound
	CSoundID[n] = PlaySound(this, CSounds[n], SF_3D)
}

//-----------------------------------------------------------------
void SetAnimState(int type, bool force = false)
{
	if(type == t_idle)
	{
		CAmbientSoundEnd()
	}

	atype = type

	if(force)
	{
		otype = -1
	}
}

//-----------------------------------------------------------------
//Play one-shot anim, and wait for end
//-----------------------------------------------------------------
int WaitAnims

//-----------------------------------------------------------------
void PlayAnimWait(local int slot, int anm, float blend, int grp, int fps, int flags, bool forcefinish = false)
{
	//don't hang on nonexistent anims
	//AssertAnim(anm)
	if(!ASet.Anims[anm])
		return

	WaitAnims++

	PlayAnim(slot, anm, blend, grp, fps, flags | AF_ONCE | AF_NOANIMEND | AF_BLENDOUT)

	local int prevstatus = status
	
	while(true)
	{
		if(!IsAnimSlotPlaying(this, 1 << slot))
			break
		
		
		if(!forcefinish && !IsState(prevstatus))
		{
			SetAnimSlot(this, slot, VNULL, 20.0, 0, NULL, 0, 0)
			break
		}

		Sleep(0)
	}

	WaitAnims--
}

//-----------------------------------------------------------------
float TurnByAnim(local ExtClass target, float maxdiff)
{
	//DPrint("TurnByAnim")
	//Print(target)
	
	float fturn = 0, turn

	turn = Turn2TargetDiff(GetMatrix(target, 3))
	fturn = fabs(turn)

	//don't waste time
	if(fturn < maxdiff)
		return 0

	local float spenttime = 0
	local int atbase
	local int awbase = a_relax_walk

	if(astate == s_relaxed)
	{
		if(!ASet.Anims[a_relax_turn])
		{
			atbase = a_alert_turn
			awbase = a_alert_walk
		}
	}
	else
	{
		if(astate == s_alert)
		{
			atbase = a_alert_turn
			awbase = a_alert_walk
		}
		else
		{
			atbase = a_combat_turn
			awbase = a_combat_walk
		}
	}

	SetXYVelocity(ZeroVec)

	//Print(String("turn by anim " + ftoa(fturn)))

	local int prevstate = status

	//Print("turning by walk")

	//too small diference for anim, do it by rotating "moonwalker"
	//existujou vubec animace?
	if(fturn < 30 || !ASet.Anims[atbase] || !ASet.Anims[atbase + 1])
	{
		iNoMotion = 2
		PlayAnim(s_custom, awbase, 10.0, g_all, 30, 0)

		while(IsState(prevstate))
		{
			turn = fabs(TurnToTarget(GetOrigin(target), TurnSpeed))

			if(ShowAI)
				SetWidgetText(WAI, 4, "TurnByAnim " + ftoa(spenttime) + " " + ftoa(turn))

			if(turn < 5)
				break

			AISleep(2)
			spenttime += 2
		}
	}
	else
	{
		iNoMotion = 1
		//don't turn
		ResetTurn()

		local float pturn = -turn

		while(IsState(prevstate))
		{
			//HACK: there is some bug? which terminate our anim??
			if(!IsAnimSlotPlaying(this, 1 << s_custom))
			{
				//force refresh!
				pturn = -turn
			}

			float p = pturn * turn //changed direction?
			if(p < 0)
			{
		//Print(String("turning by 90degrees " + ftoa(turn)))
				if(turn > 0)
					PlayAnim(s_custom, atbase + 0, 10.0, g_all, 30, 0)
				else
					PlayAnim(s_custom, atbase + 1, 10.0, g_all, 30, 0)

				pturn = turn
			}

			AISleep(2)
			spenttime += 2

			//don't move this code. turn is not local and only here is not harmed!
			turn = Turn2TargetDiff(GetOrigin(target))

			if(ShowAI)
				SetWidgetText(WAI, 4, "TurnByAnim " + ftoa(spenttime) + " " + ftoa(turn))

			if(fabs(turn) < 10)
				break
		}
	}
	iNoMotion = 0

	SetAnimSlot(this, s_custom, VNULL, 20.0, 0, NULL, 0, 0)

	if(ShowAI)
		SetWidgetText(WAI, 4, "")

	//Print("End turn")

	return spenttime
}

//-----------------------------------------------------------------
bool IsUnreachable(ExtClass ec)
{
	for(int n = 0; n < NumUnreachable; n++)
	{
		if(Unreachable[n] == ec)
			return true
	}

	return false
}

//-----------------------------------------------------------------
void SetUnreachable(ExtClass ec, float timeout = 2)
{
	//don't remember temporary waypoints
	if(IsInherited(ec, CExtClass) && ec._name && substr(ec._name, 0, 1) == "@")
		return

	if(NumUnreachable >= 32)
		return

	for(int n = 0; n < NumUnreachable; n++)
	{
		if(Unreachable[n] == ec)
		{
			UnreachableTime[n] = currenttime + timeout
			return
		}
	}

	Unreachable[NumUnreachable] = ec
	UnreachableTime[NumUnreachable] = currenttime + timeout
	NumUnreachable++
}

//-----------------------------------------------------------------
string GetLastPathError()
{
	return "actor('" + _name + "') - " + LastPathError
}

//-----------------------------------------------------------------
bool CheckDirectPath(ExtClass target)
{
	if(GetWPos() == NULL)
		return false

	if(target.GetWPos() == NULL)
		return false

	CPosition pos2 = target.GetWPos()

	if(pos2 == NULL)
		return false

	float tcost = 0
	float cost = 0

	if(agent)
	{
		cost = agent.GetTraverseCost()
		agent.SetTraverseCost(1.0)
	}

 	if(target.agent)
 	{
 		tcost = target.agent.GetTraverseCost()
 		target.agent.SetTraverseCost(1.0)
 	}

 	bool res = World.pPathFinder.TestLineCollision(ShapeIndex, WPosition, pos2)

	if(tcost)
	{
 		target.agent.SetTraverseCost(tcost)
	}

	if(cost)
	{
 		agent.SetTraverseCost(cost)
	}

	return res 
}

//-----------------------------------------------------------------
bool IsOnWPos()
{
	vector v = WPos - CurrentPos
	v[2] = 0
	float wdist = VectorNormalize(v)

	WVector = v
	WTargetDist = Distance(WTarget)
/*
Print(wdist)
Print(WTarget)
if(ClassName(this)=="creature_boss"&&KeyState(KC_M))
{
	ClearKey(KC_M)
	debug
}
*/
	//FIXME: don't use facevec!!! but real motion vector!!!!
	float dot = v * MVec //facevec

	//we are walking in good direction. Make it more precise!
	if(dot >= 0.95)
	{
		if(wdist < DistTolerance * 0.3)
			return true
	}
	else
	{
		if(wdist < DistTolerance)
			return true
	}
	return false
}

//-----------------------------------------------------------------
// Animhook. End of jump, start rolling
//-----------------------------------------------------------------
void EndJump()
{
	iTransitionAnim = 2
	if(geom)
	{
		dBodyInteractionLayer(this, ET_NPC)
		dBodySetGravity(this, "0 0 -9.8")
		SetVelocity(this, "0 0 -100") 
		ClearFlags(this, TFL_ONGROUND)
		SetXYVelocity(ZeroVec)
	}
}

//-----------------------------------------------------------------
void ArriveWP(info_waypoint targ)
{
	if(targ.ClassType == CTypeInfoWaypoint)
	{
		CallMethods(this, targ.OnArrival)
		if(targ.firstsound)
		{
			_value = targ.firstsound
			M_Sound(this)
			targ.firstsound = ""
		}
	}
}

//-----------------------------------------------------------------
void LeaveWP(info_waypoint targ)
{
	if(targ.ClassType == CTypeInfoWaypoint)
	{
		CallMethods(this, targ.OnDeparture)
		targ.lastusetime = currenttime
	}
}

//-----------------------------------------------------------------
int BeginPath(ExtClass target, bool force, bool verbose = true, float targettol = 1.0)
{
	LastPathTime = currenttime
	LastPathError = ""
	LastPathTolerance = targettol
	WaitForDoors = NULL
	
	int res
	float tcost = 0
	float cost = 0

	WTurn = false

	if(ShowAI)
		SetWidgetText(WAI, 5, "")

	if(WPath)
	{
		WPath.Destroy()
		WPath = NULL
	}

	WTarget = target

	if(!force && IsUnreachable(target))
	{
		LastPathError = "BeginPath: target is unreachable: '" + target._name + "'"
		if(verbose)
			DPrint(GetLastPathError())
		
		res = PATH_NOT_FOUND
		goto getout
	}

	if(GetWPos() == NULL)
	{
		LastPathError = "BeginPath: actor is out of area"
		DPrint(GetLastPathError())
		res = PATH_NOT_FOUND
		goto getout
	}

	if(agent)
	{
		cost = agent.GetTraverseCost()
		agent.SetTraverseCost(1.0)
	}
	
	if(!World.pPathFinder.FindClosestPosition(ShapeIndex, WPosition, WPosition, Radius * 1.5))
	{
		LastPathError = "BeginPath: actor is obstructed"
		DPrint(GetLastPathError())

		SetUnreachable(target)
		res = PATH_NOT_FOUND
		goto getout
	}

	CPosition pos2 = target.GetWPos()

	if(pos2 == NULL)
	{
		LastPathError = "BeginPath: target '" + target._name + "' is out of NAV mesh"
		if(verbose)
			DPrint(GetLastPathError())

		SetUnreachable(target)
		res = PATH_LOST_TARGET
		goto getout
	}

 	if(target.agent)
 	{
 		tcost = target.agent.GetTraverseCost()
 		target.agent.SetTraverseCost(1.0)
 	}
 
	if(!World.pPathFinder.FindClosestPosition(ShapeIndex, pos2, WPosition2, Radius * targettol))
	{
		res = PATH_NOT_FOUND
		goto getout
	}
	

	if(!IsConnected(WPosition, WPosition2))
	{
		LastPathError = "BeginPath: actor and target '" + target._name + " aren't in the same group"
		if(verbose)
			DPrint(GetLastPathError())

		SetUnreachable(target)
		res = PATH_LOST_TARGET
		goto getout
	}


	WPath = World.pPathFinder.FindShortestPath(ShapeIndex, WPosition, WPosition2, 1 - bCanUseConnections)
	
	if(WPath)
	{
		WPos = WPath.Position(0)
		WNum = 0
		
		//if we are standing quite at the waypoint, this can cause quirks. 
		//Better to skip to next waypoint.
		if(WPath.Size() > 1 && IsOnWPos())
		{
			WNum++
			WPos = WPath.Position(1)
			IsOnWPos()
		}
		
		WPathContainJumps = false
		for(int n = 0; n < WPath.Size(); n++)
		{
			info_connection conn = WPath.Connection(n)
			if(conn != NULL)
			{
				if(conn.type == 1 || conn.type == 2)
					WPathContainJumps = true
				break
			}
		}
		
		WTurn = true
		res = PATH_OK
		goto getout
	}
/*
	vector v1 = World.pPathFinder.PointForPosition(WPosition)
	vector v2 = World.pPathFinder.PointForPosition(WPosition2)

	AddDShape(SHAPE_BBOX, 0xffffff00, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, v1 - "3 3 3", v1 + "3 3 3")
	AddDShape(SHAPE_BBOX, 0xffffff00, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, v2 - "3 3 3", v2 + "3 3 3")
	AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, v1, v2)
*/
	//shouldn't happen?
	if(ShowAI)
	{
		SetWidgetText(WAI, 5, String("BeginPath: can't found path"))
	}

	if(World.pPathFinder.GetLastError() == PFE_TIMEOUT)
		SetUnreachable(target, 3)
	else
		SetUnreachable(target)

	res = PATH_NOT_FOUND

getout:

	if(tcost)
	{
 		target.agent.SetTraverseCost(tcost)
	}

	if(cost)
	{
 		agent.SetTraverseCost(cost)
	}
	return res
}

//-----------------------------------------------------------------
void ProcessConnection(info_connection conn)
{
	CallMethods(this, conn.OnArrival)

	ResetTurn()

	dBodyInteractionLayer(this, ET_NOCOLLISION)
	dBodySetGravity(this, ZeroVec)
	SetVelocity(this, ZeroVec)
	vector v = WPos - CurrentPos
	v[2] = 0

	VectorNormalize(v)
	WVector = v

	info_connection conn2 = FindClass(conn.link)
	
	if(conn2)
		TurnByAnim(conn2, 5)

	iTransitionAnim = 1

	switch(conn.type)
	{
	case 1:
		PlayAnim(s_custom, a_combat_railjump, 10.0, g_all, 30, AF_ONCE|AF_NOANIMEND)
		break

	case 2:
		//find nearest glass
		for(misc_physics_glass glass = GetNextEntity(NULL, CMiscPhysicsGlass); glass != NULL; glass = GetNextEntity(glass, CMiscPhysicsGlass))
		{
			vector gpos = facevec * 32 + GetMatrix(this, 3) + "0 0 32"
			vector vdist = gpos - GetMatrix(glass, 3)

			if(VectorLength(vdist) < 150)
				glass.BreakBox(gpos, "-24 -24 -24", "24 24 16")
		}

		PlayAnim(s_custom, a_combat_windowjump, 10.0, g_all, 30, AF_ONCE|AF_NOANIMEND)
		break

	case 3:
		PlayAnimWait(s_custom, a_combat_crawl, 10.0, g_all, 30, AF_BLENDOUT|AF_ONCE, true)
		break
	}

	if(conn.type == 3)
	{
		dBodyInteractionLayer(this, ET_NPC)
		dBodySetGravity(this, "0 0 -9.8")
		SetXYVelocity(ZeroVec)
	}
	else
	{
		local float jprevz = CurrentPos[2]
		Wait(iTransitionAnim == 2)
		
		//TODO:better wait for hitting the ground
		Wait(GetFlags(this) & TFL_ONGROUND)
		Wait(!IsAnimSlotPlaying(this, 1 << s_custom))

		aprevz = CurrentPos[2]
		
		jprevz = jprevz - aprevz
		if(jprevz > 48)
		{
			PlayAnim(s_custom, a_jump_drop, 0.1, NULL, 30, AF_ONCE|AF_NOANIMEND)
			Wait(!IsAnimSlotPlaying(this, 1 << s_custom))
		}

		SetAnimSlot(this, s_custom, VNULL, 10.0, 0, NULL, 0, 0)
	}

	iTransitionAnim = 0
}

//-----------------------------------------------------------------
int UpdatePath(local int anm, local float pathrefresh = AI_PATHREFRESH, bool verbose = true)
{
	if(WPath == NULL)
	{
		return PATH_NOT_FOUND
	}

	if(WaitForDoors)
	{
		if(WaitForDoors.status == 3)
		{
			atype = anm
			WaitForDoors = NULL
		}
		return PATH_OK
	}

	vector v

	float dt
	float t = currenttime - LastPathTime

	//every Nsec update path
	if(t > pathrefresh)
	{
		int res = BeginPath(WTarget, false, verbose, LastPathTolerance)
		if(res != PATH_OK)
		{
			return res
		}
	}

	//TODO: Call departure event!
	if(CurrentWP != WTarget)
	{
		if(CurrentWP)
		{
			LeaveWP(CurrentWP)
			CurrentWP = NULL
		}
	}

	CPosition a = WTarget.GetWPos()

	if(a == NULL)
	{
		if(ShowAI)
			SetWidgetText(WAI, 5, String("UpdatePath: " + _name + " lost WTarget"))
		return PATH_LOST_TARGET
	}

	WTurn = true

	if(IsOnWPos())
	{
		local info_connection conn = WPath.Connection(WNum)

		WNum++

		//get next position
		if(WNum >= WPath.Size())
		{
			WTurn = false
			
			//if we were walking for waypoint
			if(IsInherited(WTarget, CInfoWaypoint))
			{
				CurrentWP = WTarget
				ArriveWP(CurrentWP)
			}
			return PATH_FINISHED
		}
		else
		{
			WPos = WPath.Position(WNum)
		}

		if(conn)
		{
			ProcessConnection(conn)			
			res = BeginPath(WTarget, true)
			if(res != PATH_OK)
			{
				return res
			}
		}
	}
	else
	{
		float dot = v * facevec

		if(astate == s_relaxed)
		{
			//check if next waypoint is on sharp angle.

			//if it's more than 50degrees and it's near from here, use strafe
			if(WTargetDist < fMaxStrafeDistance && fabs(dot) < 0.6)
			{
				//realize which side the target is on
				if(GetSide(WPos, CurrentPos, facevec) < 0)
				{
					SetAnimState(t_lstrafe)
				}
				else
				{
					SetAnimState(t_rstrafe)
				}
				goto mdone
			}
		}

		if(astate != s_combat)
		{
			//if so, make turn inplace instead of walk/run in direction
			if(dot < fMinCosForSlowTurn)
			{
				SetOrigin(WalkWaypoint, WPos)
				TurnByAnim(WalkWaypoint, 10)
			}
		}

		SetAnimState(anm)
		
mdone:;
	}

	//it may happen during waiting. It's strange anyway :-/
	if(WPath == NULL)
	{
		return PATH_NOT_FOUND
	}

	//are we there yet?
	if(WNum >= WPath.Size() - 1)
	{
		if(IsOnWPos())
		{
			WTurn = false

			//if we were walking for waypoint
			if(IsInherited(WTarget, CInfoWaypoint))
			{
				CurrentWP = WTarget
				ArriveWP(CurrentWP)
			}
			return PATH_FINISHED
		}
	}

	return PATH_OK
}

//-----------------------------------------------------------------
float AIEventAge(int ev)
{
	return currenttime - AI_Time[ev]
}

//-----------------------------------------------------------------
// creature noises
//-----------------------------------------------------------------
void SoundFirstSight()
{
}

//-----------------------------------------------------------------
void SoundSuspicious()
{
}

//-----------------------------------------------------------------
void BeginLostTarget(CreatureClass cc)
{
	LostTarget = cc

	if(status != STATE_LOST_TARGET)
	{
		SwitchFSM(STATE_LOST_TARGET)
	}
}

//Shape LKP
//-----------------------------------------------------------------
void SetAttackerLKP(vector pos)
{
	/*
	if(LKP)
	{
		LKP.Destroy()
		LKP = NULL
	}

	LKP = AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos - "3 3 3", pos + "3 3 3")
*/
	AttackerLKP = pos
	AttackerLKPTime = currenttime
}

//-----------------------------------------------------------------
void SetWP(info_waypoint wp, vector pos)
{
	vector to

	if(wp.WPosition == NULL)
		wp.WPosition = new CPosition

	if(World.pPathFinder.PositionForPoint(pos, wp.WPosition))
	{
		to = World.pPathFinder.PointForPosition(wp.WPosition)
	}
	else
	{
		float endplane[4]
		//maybe we are too high above nav-mesh. Trace down
		to = pos - "0 0 128"
		int pf = ClearFlags(this, TFL_SOLID)
		TraceLineEx(MONSTER, pos, to, ZeroVec, ZeroVec, NULL, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, NULL)
		SetFlags(this, pf)
		to += "0 0 4"

		World.pPathFinder.PositionForPoint(to, wp.WPosition)
	}

	wp.coords = to
	wp.floor = to
	SetOrigin(wp, to)
}

//-----------------------------------------------------------------
void BeginCombatTarget(CreatureClass cc)
{
	Attacker = cc
	SetAttackerLKP(cc.CurrentPos)

	if(bLongRange)
	{
		if(astate == s_relaxed)
		{
			SwitchFSM(STATE_1STSIGHT)
		}
		else
			SwitchFSM(STATE_LR_ATTACK)
	}
	else
	{
		if(BeginPath(cc, true, false) == PATH_OK)
		{
			if(astate == s_relaxed)
			{
				SwitchFSM(STATE_1STSIGHT)
			}
			else
			{
				SwitchFSM(STATE_CHASE)
			}
		}
		else
		{
			BeginLostTarget(cc)
		}
	}

	SetState(s_combat)
}

//-----------------------------------------------------------------
bool IsLastMan()
{
	if(Squad && Squad.Alive == 1)
		return true

	return false
}

//-----------------------------------------------------------------
void BeginCover(CreatureClass cc)
{
	if(!IsLastMan())
		Say(ST_TAKECOVER)
	Attacker = cc
	SetAttackerLKP(cc.CurrentPos)

	SwitchFSM(STATE_COVER)
}

//-----------------------------------------------------------------
bool BeginCoverFromDanger(ExtClass actor, vector pos)
{
	if(!IsLastMan())
		Say(ST_GRENADE)
	
	//if we were recently solving this problem, check distance from the new danger
	if(StateAge(STATE_COVERFROMDANGER) < 2.0)
	{
		//it's pretty close. Don't care about it
		if(VectorLength(pos - DangerPos) < 84)
		{
			return false
		}
	}

	//don't be so coward when already fighting
	if(Alerted())
	{
		if(VectorLengthSq(pos - CurrentPos) > 191.8 * 191.8)
		{
			return false
		}
	}

	if(World.pPathFinder.PositionForPoint(pos, NULL))
	{
		Attacker = actor
		DangerPos = pos
		SetWP(TempWaypoint, pos)
		SwitchFSM(STATE_COVERFROMDANGER)
		return true
	}

	//shit!
	BeginCover(World.Player)
	return true
}

//-----------------------------------------------------------------
void BeginRoam(CreatureClass cc)
{
	SwitchFSM(STATE_ROAMING)
}

//-----------------------------------------------------------------
// Alert event occured. Choose the behavior according to Tactics settings
//-----------------------------------------------------------------
int DoTactics(CreatureClass cc)
{
	//We are not interested in anything
	if(Tactics <= TACTICS_IDLE)
		return 0

	//roaming are usualy small animals
	if(Tactics == TACTICS_ROAM)
	{
		BeginCover(cc)
		return 1
	}

	if(Tactics == TACTICS_COVER)
	{
		BeginCover(cc)
		return 1
	}

	//Already alerted
	if(Alerted())
	{
		//are we alerted by other target?
		if(cc == Attacker)
			return -1
	}

	CPosition area

	//alerted
	if(status == STATE_LOST_TARGET && LostTarget == cc)
	{
		area = GetWPos()

		if(!IsConnected(cc.GetWPos(), area))
			return 0
			
		if(!TryFindPath(LostTarget))
		{
			return 0
		}
	}

	float dist = Distance(cc)

	//there is enough of them
	if(cc.NumAttackers >= sizeof(Attackers))
	{
		area = GetWPos()

		//try to replace further attacker
		for(int n = 0; n < sizeof(Attackers); n++)
		{
			AICreatureClass ac = cc.Attackers[n]

			if(ac == NULL)
				continue

			if(ac.Distance(cc) > dist)
			{
				if(bLongRange)
				{
					ac.AIKillEvents()
					ac.EndAttack()
					SetWP(TempWaypoint, ac.CurrentPos)
					GuardTarget = TempWaypoint
					SwitchFSM(STATE_GUARD)
					goto skip
				}
				else
				{
					//don't replace longrange by shortrange, if we don't have path!
					if(ac.bLongRange && IsConnected(ac.GetWPos(), area))
					{
						ac.AIKillEvents()
						ac.EndAttack()

						SetWP(TempWaypoint, ac.CurrentPos)
						GuardTarget = TempWaypoint
						SwitchFSM(STATE_GUARD)
						goto skip
					}
				}
			}
		}

		return 0
	}

skip:
	CallMethods(this, OnCombat)
	BeginCombatTarget(cc)
	return 1
}

//-----------------------------------------------------------------
void M_ResetFirstSight(ExtClass other)
{
	FirstSight = false
}

//-----------------------------------------------------------------
void M_SetTactics(ExtClass other)
{
	if(status >= STATE_DIE)
		return

	strlower(other._value)

	int prevtactics = Tactics

	switch(other._value)
	{
	case "combat":
		Tactics = TACTICS_COMBAT
		break

	case "cover":
		Tactics = TACTICS_COVER
		break

	case "covercombat":
		Tactics = TACTICS_COVERCOMBAT
		break

	case "guardcombat":
		Tactics = TACTICS_GUARDCOMBAT
		SwitchFSM(STATE_GUARD)
		return

	case "berserk":
		Tactics = TACTICS_BERSERK
		break

	case "idle":
		Tactics = TACTICS_IDLE
		if(status != STATE_SCRIPT)
			SwitchFSM(STATE_IDLE)
		return

	case "dummy":
		Tactics = TACTICS_DUMMY
		return

	case "roam":
		Tactics = TACTICS_ROAM
		BeginRoam(Target)
		return

	default:
		DPrint(_name + ".SetTactics(): Unknown tactics " + other._value)
		ErrorNum++
		break
	}

	if(Target || GuardTarget)
	{
		CreatureClass cc = NULL

		if(Tactics == TACTICS_COMBAT || Tactics == TACTICS_COVERCOMBAT || Tactics == TACTICS_BERSERK)
			cc = Target

		//Take cover
		if(Tactics == TACTICS_COVER)
		{
			if(Target == NULL)
			{
				Target = Attacker

				if(Target == NULL)
				{
					DPrint(_name + ".SetTactics(Cover): There is no Target nor Attacker")
					ErrorNum++
					return
				}
			}
		
			//Tactics == TACTICS_COVER is not real tactics. It's just temporary state
			Tactics = prevtactics
			BeginCover(Target)
			return
		}

		//go & guard
		if(Tactics == TACTICS_GUARDCOMBAT)
		{
			//Attacker = GuardTarget
			SwitchFSM(STATE_GUARD)
			return
		}

		if(cc)
		{
			if(cc.ClassType == ClassTypeCreature)
				SetAttackerLKP(cc.CurrentPos)

			DoTactics(cc)
		}
		return
	}

	//DPrint(_name + " has no target for SetTactics")
}

//-----------------------------------------------------------------
void M_DisablePain(ExtClass other)
{
	flags |= FL_NOPAIN
}

//-----------------------------------------------------------------
void M_EnablePain(ExtClass other)
{
	flags &= 0xffffffff - FL_NOPAIN
}

//-----------------------------------------------------------------
void M_DisableSenses(ExtClass other)
{
	flags &= 0xffffffff - (FL_SEE|FL_HEAR|FL_FEEL)
}

//-----------------------------------------------------------------
void M_EnableSenses(ExtClass other)
{
	VisTime = 0 //quick response
	flags |= FL_SEE|FL_HEAR|FL_FEEL
	NumVisibleTargets = 0
}

//-----------------------------------------------------------------
void M_DisableHearing(ExtClass other)
{
	flags &= 0xffffffff - FL_HEAR
}

//-----------------------------------------------------------------
void M_EnableHearing(ExtClass other)
{
	flags |= FL_HEAR
}

//-----------------------------------------------------------------
void M_DisableSeeing(ExtClass other)
{
	flags &= 0xffffffff - FL_SEE
	NumVisibleTargets = 0
	ClearEventMask(this, EV_FLASHLIGHT)
}

//-----------------------------------------------------------------
void M_EnableSeeing(ExtClass other)
{
	VisTime = 0 //quick response
	flags |= FL_SEE
	SetEventMask(this, EV_FLASHLIGHT)
}

//-----------------------------------------------------------------
void M_DisableFeeling(ExtClass other)
{
	flags &= 0xffffffff - FL_FEEL
}

//-----------------------------------------------------------------
void M_EnableFeeling(ExtClass other)
{
	flags |= FL_FEEL
}

//-----------------------------------------------------------------
void M_EndScript(ExtClass other)
{
	CmdStackPos = 0
	AICmd = AICMD_NONE
	AIParm = ""
}

//-----------------------------------------------------------------
void M_SetHealth(ExtClass other)
{
	float rate = atof(other._value)
	clamp rate<0, 100>
	Health = MaxHealth * rate / 100
	bOnLowHealth = true
}

//-----------------------------------------------------------------
void AnimDeath(int dir, float pow)
{
	int a = rand(0, 3)

	if(dir == 0)
		a += 2

	a += a_death

	if(!PlayAnim(s_death, a, 20.0, g_all, 30, AF_ONCE))
	{
		SetTimer(this, this, 150, EV_ANIMEND, s_death, true)
	}

	atype = -1
	astate = -1
}

//-----------------------------------------------------------------
void UnuseWaypoint()
{
	if(UsedWaypoint)
	{
		UsedWaypoint.used &= UsedOff
		UsedWaypoint = NULL
	}
}

//-----------------------------------------------------------------
void UseWaypoint(info_waypoint uwp)
{
	UnuseWaypoint()

	if(LRUWaypoints[LRUTop] != uwp)
	{
		LRUTop = LRUTop + 1 & 1
		LRUWaypoints[LRUTop] = uwp
	}

	UsedWaypoint = uwp
	UsedWaypoint.used |= UsedOn
}

//-----------------------------------------------------------------
bool RecentlyUsed(info_waypoint wp)
{
	if(LRUWaypoints[0] == wp || LRUWaypoints[1] == wp)
		return true

	return false
}


//-----------------------------------------------------------------
bool CmdPath(string parm)
{
	local info_waypoint targ = GetTarget(parm, 1)

	if(targ == NULL)
		return false

	if(!IsInherited(targ, CInfoWaypoint))
	{
		DPrint(_name + ".AddCmd(Walk/Run) failed! Reason: target " + targ._name + " is not waypoint")
		ErrorNum++
		return false
	}

	if(CurrentWP == targ)
	{
		//we are here already!
		return false
	}

	if(targ.used & UsedOn)
	{
		DPrint(_name + ".AddCmd(Walk/Run) failed! Reason: target " + targ._name + " is occupied by someone")
		ErrorNum++
		return false
	}

	for(local int timeout = 0; timeout < 5; timeout++)
	{
		if(BeginPath(targ, true, false) == PATH_OK)
		{
			UseWaypoint(targ)
			return true
		}

		AISleep(50)
	}
/*
	vector wfrom = World.pPathFinder.PointForPosition(WPosition)
	vector wto = World.pPathFinder.PointForPosition(targ.WPosition)
	
	AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wfrom, wto)

	AddDShape(SHAPE_BBOX, 0xff00ffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wfrom - "4 4 4", wfrom + "4 4 4")
	AddDShape(SHAPE_BBOX, 0xff00ffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wto - "4 4 4", wto + "4 4 4")

	BeginPath(targ, true, false)
*/
	DPrint(_name + ".AddCmd(Walk/Run) failed! Reason: " + LastPathError)
	return false
}

//-----------------------------------------------------------------
bool CmdShoot(string AIParm, bool nowait)
{
	int c = strstr(AIParm, ",")
	ExtClass targ
	float ttime

	if(c != -1)
	{
		targ = GetTarget(trim(substr(AIParm, 0, c)))
		ttime = atof(substr(AIParm, c + 1, strlen(AIParm) - c - 1))
	}
	else
	{
		targ = GetTarget(trim(AIParm))
		ttime = -1
	}

	if(targ == NULL)
		return false

	ShootTarget = targ
	ShootTime = ttime

	if(ShootTime != 0)
	{
		if(!nowait)
			SetAnimState(t_idle)

		SetState(s_combat)
	}
	return true
}

//-----------------------------------------------------------------
bool PopStack()
{
	if(!CmdStackPos)
	{
		AICmd = AICMD_NONE
		AIParm = ""
		SetAnimState(t_idle)
		return false
	}

	int c

	CmdStackPos--

	AIParm = CmdParmStack[CmdStackPos]
	AICmd = CmdStack[CmdStackPos]

	switch(AICmd)
	{
	case AICMD_SHOOTNW:
		CmdShoot(AIParm, true)
		return PopStack()

	case AICMD_SHOOT:
		if(CmdShoot(AIParm, false))
		{
			Wait(ShootTime == 0)
		}
		return PopStack()

	case AICMD_END:
		M_EndScript(this)
		return false

	case AICMD_WALK:
		if(!CmdPath(AIParm))
			return PopStack()
		SetAnimState(t_walk)
		break

	case AICMD_RUN:
		if(!CmdPath(AIParm))
			return PopStack()
		SetAnimState(t_run)
		break

	case AICMD_IDLE:
		SetAnimState(t_idle)
		break

	case AICMD_ANIM:
		_value = AIParm
		SetAnimState(t_idle)
		M_SetAnim(this)
		break

	case AICMD_ANIMNW:
		AICmd = AICMD_IDLE
		_value = AIParm
		SetAnimState(t_idle)
		M_SetAnim(this)
		break

	case AICMD_USE:
		ExtClass ec = GetTarget(AIParm)
		if(ec)
			ec.Activate(0, this)
		return PopStack()

	case AICMD_WSIGHT:
		break

	case AICMD_CALL:
		CallMethods(this, AIParm)
		return PopStack()

	case AICMD_SLEEP:
		SetAnimState(t_idle)
		AISleep(atof(AIParm))
		return PopStack()

	case AICMD_TURN:
		c = strstr(AIParm, ",")
		if(c != -1)
		{
			_TurnTarget = GetTarget(substr(AIParm, 0, c))
		}
		else
		{
			_TurnTarget = GetTarget(AIParm)
		}
		break

	case AICMD_TURNBY:
		c = strstr(AIParm, ",")
		if(c != -1)
		{
			_TurnTarget = GetTarget(substr(AIParm, 0, c))
		}
		else
		{
			_TurnTarget = GetTarget(AIParm)
		}
		break

	case AICMD_SAY:
/*		if(ClassName(this) == "creature_faggioli")	//TODO World.say() uz bude asi iba pre cutsceny. mozno by to mohlo fungovat cez Transmitter.Say() ktory sluzi ako SpeekBuffer
			World.say(AIParm, "faggioli", INGAME_DIALOG, false)
		else
			World.say(AIParm, "computer", INGAME_DIALOG, false)

		Wait(World.InGameDialogSound == 0)*/
		return PopStack()

	default:
		return false
	}

	return true
}

//-----------------------------------------------------------------
void AddCmd(string parm)
{
	if(status >= STATE_DIE)
		return

	int c = strstr(parm, ",")
	int ic

	string cmd

	if(c >= 0)
	{
		cmd = substr(parm, 0, c)
		triminplace(cmd)
		parm = substr(parm, c + 1, strlen(parm) - c - 1)
		triminplace(parm)
	}
	else
	{
		cmd = parm
		parm = ""
	}

	strlower(cmd)

	switch(cmd)
	{
	case "end":
		ic = AICMD_END
		break

	case "idle":
		ic = AICMD_IDLE
		break

	case "walk":
		ic = AICMD_WALK
		break

	case "run":
		ic = AICMD_RUN
		break

	case "anim":
		ic = AICMD_ANIM
		break

	case "animnowait":
		ic = AICMD_ANIMNW
		break

	case "use":
		ic = AICMD_USE
		break

	case "call":
		ic = AICMD_CALL
		break

	case "waitsight":
		ic = AICMD_WSIGHT
		break

	case "sleep":
		ic = AICMD_SLEEP
		break

	case "turn":
		ic = AICMD_TURN
		break

	case "turnby":
		ic = AICMD_TURNBY
		break

	case "shoot":
		ic = AICMD_SHOOT
		break

	case "shootnowait":
		ic = AICMD_SHOOTNW
		break

	case "say":
		ic = AICMD_SAY
		break

	default:
		DPrint("Unknown AI command:" + cmd + " on entity " + _name)
		return
	}

	if(status != STATE_SCRIPT)
	{
		if(CmdStackPos != 0)
		{
			Print(String(_name + " has switched to SCRIPT mode, but his stack is not empty"))
		}
	}

	//AddStack
	for(int n = CmdStackPos - 1; n >= 0; n--)
	{
		CmdStack[n + 1] = CmdStack[n]
		CmdParmStack[n + 1] = CmdParmStack[n]
	}

	CmdStack[0] = ic
	CmdParmStack[0] = parm
	CmdStackPos++

	if(status != STATE_SCRIPT)
	{
		//AIKillEvents()
		SwitchFSM(STATE_SCRIPT)
		AICmd = AICMD_IDLE
	}
}

//-----------------------------------------------------------------
void M_AddCmd(ExtClass other)
{
	AddCmd(other._value)
}

//-----------------------------------------------------------------
void GetSpeechSet(string name)
{
	for(int n = 0; n <= World.SpeechSetCount; n++)
	{
		SpeechSet ss = World.SpeechSets[n]

		if(ss.Name == name)
		{
			ss.Load()
			SSet = ss
			return
		}
	}
}

//-----------------------------------------------------------------
void GetHurtFactors(float hf[])
{
	for(int n = 0; n < sizeof(hf); n++)
	{
		//hurt factor must not be 0!!!
		if(hf[n] == 0.0)
			HurtFactor[n] = 1.0
		else
			HurtFactor[n] = hf[n]
	}
}

vector shootpos

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
		
	case ClassTypeMiscModel:
		misc_model mm = other
		if(mm.railing)
			return false

	case ClassTypePhysicsPart:
		physics_part pack = other.Owner

		if(pack && pack.ClassType == ClassTypeEnterableVehicle)
		{
			if(pack.status == 1)
			{
				return false
			}
		}
		break
	}

	return true
}

//-----------------------------------------------------------------
CreatureClass TestShoot(ExtClass enemy, vector to)
{
	MinClass cent = NULL
	float endplane[4]

	vector from = GetMatrix(CurrentWeapon, 3)

	int fl = ClearFlags(this, TFL_SOLID)

	shootpos = to + "0 0 20"
	
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, from, shootpos)

	TraceLineEx(DEFAULT, from, shootpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	if(cent != enemy)
	{
		shootpos = to

		TraceLineEx(DEFAULT, from, shootpos, ZeroVec, ZeroVec, cent, endplane, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_DETAIL|TRACE_PATCH, &TestShootFilter)
	}

	SetFlags(this, fl)
	return cent
}

//-----------------------------------------------------------------
int IsEnemy(CreatureClass cc)
{
	//we have no enemies
	if(flags & FL_FRIENDLY)
		return false

	//shouldn't happen
	if(cc.status >= STATE_DIE)
		return false

	if(cc.GroupID != GroupID)
		return true

	return false
}

//-----------------------------------------------------------------
// virtual functions
//-----------------------------------------------------------------
void AnimPain(int fl, float pow, vector painvec)
{
	if(flags & FL_NOPAIN || flags & FL_PUPPET)
		return

	if(PainTimer)
	{
		StopTimer(PainTimer)
		PainTimer = 0
	}

	pain = 1

	int a

	painvec[2] = 0
	VectorNormalize(painvec)

vector endpos = painvec * -24 + CurrentPos

	int hitdir = HitDirection2(painvec)

	if(hitdir == 1)
	{
		if(ASet.Anims[a_pain_back])
		{
			PlayAnim(s_pain, a_pain_back + rand(0, 4), 15.0, SmallPainGroup, 40, AF_ONCE)
			return
		}
	}

/*	if(TestMove(CurrentPos, endpos, false) < 1)
	{
		a = a_pain_inplace + rand(0, 5)
	}
	else
	{*/
		a = a_pain + rand(0, 4)
//	}

	if(!ASet.Anims[a])
	{
		pain = 0
		return
	}

	PlayAnim(s_pain, a, 20.0, SmallPainGroup, 40, AF_ONCE)
}

//-----------------------------------------------------------------
void Show(bool show)
{
	if(show)
	{
		SetFlags(this, TFL_VISIBLE|TFL_SOLID)
	}
	else
	{
		ClearFlags(this, TFL_VISIBLE|TFL_SOLID)
	}

	if(CurrentWeapon)
		CurrentWeapon.Show(show)
}

//-----------------------------------------------------------------
bool InFront(MinClass other, float cos = 0.7)
{
	vector min, max
	GetCBoundBox(other, min, max)
	vector vec = min + max * (float)0.5
	vec = vec - CurrentPos
	vec[2] = 0
	VectorNormalize(vec)

	float d = vec * facevec

	if(d > cos)
		return true

	return false
}

//-----------------------------------------------------------------
int TestDoorBlock(FuncClass fc)
{
	if(WaitForDoors == fc)
		return true

	if(flags & FL_CANOPENDOORS && fc.ClassType == ClassTypeFunc)
	{
		if(fc.ClassNum != ClassNumDoor && fc.ClassNum != ClassNumRotDoor)
			return false

		if(fc.CanActivate(0, this) == ACT_ENABLE)
		{
			fc.Activate(0, fc)
			WaitForDoors = fc
			SetAnimState(t_idle)
			ResetTurn()
			return true
		}
		else
		{
			//already opening
			if(fc.status == 1)
			{
				WaitForDoors = fc
				SetAnimState(t_idle)
				ResetTurn()
				return true
			}
		}
	}
	return false
}

//-----------------------------------------------------------------
// AI memory buffer
//-----------------------------------------------------------------
void AIKillEvent(int ev)
{
	if(AI_Used[ev])
	{
		AI_Used[ev] = false
		AI_ActEvents--
	}
}

//-----------------------------------------------------------------
void AIAddEvent(int ev, vector pos, MinClass ent)
{
	if(ent == Attacker)
	{
		SetAttackerLKP(pos)
	}

	AI_Time[ev] = currenttime

	if(AI_Used[ev])
	{
		vector v = CurrentPos - pos
		float dist1 = v * v

		v = CurrentPos - AI_Pos[ev]
		float dist2 = v * v

		//replace further events?
		if(dist1 >= dist2)
			return
	}
	else
	{
		AI_ActEvents++
		AI_Used[ev] = true
	}

	AI_Ent[ev] = ent
	AI_Pos[ev] = pos
}

//-----------------------------------------------------------------
void AIInit()
{
	AIKillEvents()
}

//-----------------------------------------------------------------
float IsObserved(ExtClass ent)
{
	for(int n = 0; n < NumObserved; n++)
	{
		if(Observed[n] == ent)
			return currenttime - ObservedTime[n]
	}

	return 0
}

//-----------------------------------------------------------------
void AddObserved(ExtClass ent)
{
	float worst = 0
	int worstnum = 0

	for(int n = 0; n < NumObserved; n++)
	{
		//already included. Just update time
		if(Observed[n] == ent)
		{
			ObservedTime[n] = currenttime
			return
		}

		float diff = currenttime - ObservedTime[n]
		if(diff > worst)
		{
			worst = diff
			worstnum = n
		}
	}

	if(NumObserved < ObservedBufferSize)
	{
		Observed[NumObserved] = ent
		ObservedTime[NumObserved] = currenttime
		NumObserved++
		return
	}

	Observed[worstnum] = ent
	ObservedTime[worstnum] = currenttime
}


float LastCover

//-----------------------------------------------------------------
bool DoFirstSight(CreatureClass cc)
{
	if(!FirstSight)
	{
	//Print(String(_name + " First Sight on " + cc._name))

		Say(ST_ALERT)

		FirstSight = true

		if(!sfirst)
		{
			sfirst = true
			SoundFirstSight()
		}

		if(Squad)
		{
			Squad.OnMemberAttacked(this, cc, AttackerLKP)
		}

		if(OnFirstSight)
		{
			CallMethods(this, OnFirstSight)
			return true
		}
	}
	return false
}

//-----------------------------------------------------------------
//jednou za cas projizdi nabufferovane udalosti, projizdi aktivni
//a pokud je to mozne, prepina FSM na nove ukoly
//-----------------------------------------------------------------
void AIProcess()
{
	if(Attacker && Alerted() && NotInLOFTime > 11.0)
	{
		NotInLOFTime = 0
		CallMethods(this, OnLostTarget)
	}

	for(int currevent = 0; AI_ActEvents && currevent < AIBufferSize; currevent++)
	{
		if(!AI_Used[currevent])
			continue

		AI_Used[currevent] = false

		CreatureClass cc = AI_Ent[currevent]

		AI_ActEvents--
		switch(currevent)
		{
		case AIE_GRENADE:
			if(DoFirstSight(World.Player))
				return

			if(Tactics <= TACTICS_IDLE)
				break

			if(BeginCoverFromDanger(World.Player, AI_Pos[currevent]))
				return
			break

		case AIE_SHOTORTOUCH:
			if(cc == World.Player && DoFirstSight(cc))
				return

			if(Tactics <= TACTICS_IDLE)
				break

			//short range enemies are helpless when enemy out of area
			if(!bLongRange)
			{
				CPosition area = cc.GetWPos()
				CPosition myarea = GetWPos()

				if(!IsConnected(area, myarea))
				{
					BeginCover(cc)
					return
				}
			}

			if(cc != Attacker || Alertable())
			{
				Say(ST_ALERTBYSHOT)
				if(DoTactics(AI_Ent[currevent]))
					return
			}
			break

		case AIE_SAWENEMY:
			if(cc == World.Player && DoFirstSight(cc))
			{
				return
			}

			if(Tactics <= TACTICS_IDLE)
				break

			if(NumCloseCombatAnims && cc == Attacker && DistanceBetween(cc) < DISTANCE_TO_CLOSECOMBAT && InFront(cc, 0.5) && StateAge(STATE_CLOSECOMBAT) > 1.5)
			{
				bool fighting = IsState(STATE_LR_ATTACK)
				fighting |= IsState(STATE_CHASE)

				if(fighting)
				{
					SwitchFSM(STATE_CLOSECOMBAT)
				}
				return
			}

			if(Alertable())
			{
				if(DoTactics(AI_Ent[currevent]))
				{
					Say(ST_ALERT)
					return
				}
			}
			break

		case AIE_BULLETRAIN:
			if(Tactics <= TACTICS_IDLE)
				break

			if(IsState(STATE_COVER))
				break

			if(!Alertable())
			{
				float d  = currenttime - LastCover
				if(d >= 3)
				{
					LastCover = currenttime
					BeginCover(AI_Ent[currevent])
					return
				}
			}
			break

		case AIE_HEARDSHOT:
			if(cc == World.Player && DoFirstSight(cc))
				return

			if(Tactics <= TACTICS_IDLE)
				break

			if(Alertable())
			{
				Say(ST_ALERTBYSHOT)
				if(DoTactics(AI_Ent[currevent]))
					return
			}
			break

		case AIE_SAWDEADBODY:
			if(Tactics <= TACTICS_IDLE)
				break

			//ignore already observed bodies
			if(IsObserved(AI_Ent[currevent]) == 0)
			{
				if(Alertable())
				{
					Say(ST_ALERTBYDEAD)
					SwitchFSM(STATE_OBSERVE)
					AddObserved(AI_Ent[currevent])
					SetWP(TempWaypoint, GetOrigin(AI_Ent[currevent]))
					SoundSuspicious()
				}
				return
			}
			break

		case AIE_HEARDNOISE:
			if(cc == World.Player && DoFirstSight(cc))
				return

			if(Tactics <= TACTICS_IDLE)
				break

			if(Alertable())
			{
				if(cc)
				{
					float tdist = IsObserved(cc)
	
					//it's not so long we observed it
					if(tdist && tdist < 5)
						break
	
					AddObserved(cc)
				}
				SwitchFSM(STATE_OBSERVE)
				SetWP(TempWaypoint, AI_Pos[currevent])
				SoundSuspicious()
			}
			break

		case AIE_SQUADATTACKED:
			if(cc == World.Player && DoFirstSight(cc))
				return

			if(Tactics <= TACTICS_IDLE)
				break

			if(Alertable())
			{
				SwitchFSM(STATE_OBSERVE)
				AddObserved(AI_Ent[currevent])
				SetWP(TempWaypoint, GetOrigin(AI_Ent[currevent]))
				SoundSuspicious()
			}
			break
		}
	}	
}

//-----------------------------------------------------------------
void NotifyNoise(MinClass actor, vector origin, int noises)
{
	vector v
	float dot
	
	if(noises & NOISE_FLAME)
	{
		//if is visible, send it as danger event.
		v = origin - EyesPos()
		VectorNormalize(v)
		dot = v * facevec
		if(dot > 0.225)
		{
			AIAddEvent(AIE_GRENADE, origin, NULL)
		}
		return
	}

	if(noises & NOISE_PHYSICS && ClassName(actor) == "ProjectileGrenade")
	{
		//if is visible, send it as grenade event. Noise otherwise
		v = origin - EyesPos()
		VectorNormalize(v)
		dot = v * facevec
		if(dot > 0.225)
		{
			AIAddEvent(AIE_GRENADE, origin, NULL)
			return
		}
	}

	if(noises & NOISE_SHOOT && IsEnemy(actor))
	{
		AIAddEvent(AIE_HEARDSHOT, origin, actor)
		return
	}

	//it's dangerous to add actors which could be deleted (grenades etc.).
	//These two types are most common and should be safe
	if(!IsInherited(actor, CCreatureClass) && !IsInherited(actor, CMiscPhysicsModel))
		actor = NULL

	AIAddEvent(AIE_HEARDNOISE, origin, actor)
}

//-----------------------------------------------------------------
bool AddVisibleTarget(ExtClass target, float dist)
{
	for(int n = 0; n < NumVisibleTargets; n++)
	{
		if(VisibleTargets[n] == target)
		{
			VTLastTime[n] = currenttime
			float td = currenttime - VTFirstTime[n]

			float timeout = 0.1

			//do 3metru reaguj prakticky okamzite
			if(dist > DIST_TO_IMMED_REACTION)
			{
				//scale by distance function!
				dist = dist + DIST_TO_IMMED_REACTION
				float scale = DIST_TO_IMMED_REACTION / dist
				timeout = 1 - scale
	
				if(target == Attacker)
					timeout *= fAlertedVisibilityLatency
				else
					timeout *= fVisibilityLatency
			}

			if(td >= timeout)
				return true

			return false
		}
	}

	VisibleTargets[NumVisibleTargets] = target
	VTLastTime[NumVisibleTargets] = currenttime
	VTFirstTime[NumVisibleTargets] = currenttime
	NumVisibleTargets++
	
	//do 3metru reaguj okamzite
	if(dist < DIST_TO_IMMED_REACTION)
		return true

	return false
}

//-----------------------------------------------------------------
void UpdateVisibleTargets()
{
	for(int n = 0; n < NumVisibleTargets; n++)
	{
		float td = currenttime - VTLastTime[n]

		if(td > 3)
		{
			//remove
			if(NumVisibleTargets > 1)
			{
				VisibleTargets[n] = VisibleTargets[NumVisibleTargets - 1]
				VTLastTime[n] = VTLastTime[NumVisibleTargets - 1]
				VTFirstTime[n] = VTFirstTime[NumVisibleTargets - 1]
			}
			NumVisibleTargets--
		}
	}
}

//-----------------------------------------------------------------
vector GetLookVector()
{
	vector mat[4], vec

	GetBoneMatrix(this, LookBone, mat)
	if(LookAxis < 0)
		vec = ZeroVec - mat[-LookAxis - 1]
	else
		vec = mat[LookAxis - 1]
	
	return vec
}

//-----------------------------------------------------------------
	void UpdateLookAt()
	{
		if(LookTimeout > currenttime)
		{
			vector tpos, epos, mat[4]

			epos = EyesPos()

			if(IsInherited(LookTarget, CCreatureClass))
			{
				//exception for small robots
				if(Height <= 48)
					tpos = GetMatrix(LookTarget, 3)
				else
					tpos = LookTarget.EyesPos()
			}
			else
				tpos = GetMatrix(LookTarget, 3)

			vector ang = Vector2Angles(tpos - epos)

			float yaw = DiffAngle(ang[YAW], faceyaw + LAddAngle)
/*
if(_name == "lightTrooper2" && atype == t_run_fb)
{
	widget w
	
	if(w == NULL)
	{
		w = CreateWidget(WTYPE_TEXT, 0, 0, 400, 200, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND, 0xffffffff, 190)
	}
	
	SetFont(w, MainFont)
	SetWidgetText(w, 0, ftoa(ang[YAW]))
	SetWidgetText(w, 1, ftoa(faceyaw))
	SetWidgetText(w, 2, ftoa(yaw))
	SetWidgetText(w, 3, ftoa(LAddAngle))
	
	
	if(KeyState(KC_G))
	{
		ClearKey(KC_G)
		debug
	}	
}
*/
			float pitch = ang[PITCH]

			if(pitch > 180)
				pitch = pitch - 360

			float ayaw = fabs(yaw)
			//avoid tiny oscilations
			if(ayaw < 1.5)
				yaw = 0

			if(ayaw > fYawLimit)
			{
				yaw = 0
				pitch = 0
			}
		}
		else
		{
			pitch = 0
			yaw = 0
		}


		SetHeadOrientation(pitch, yaw)
	}

//-----------------------------------------------------------------
	void LookAt(ExtClass target)
	{
		LookTarget = target

		if(target)
		{
			LookTimeout = currenttime + 2
			return
		}

		LookTimeout = 0
	}

Shape eyeshape
//-----------------------------------------------------------------
// AI visibility
//-----------------------------------------------------------------
void AIVisibility()
{
	if(!flags & FL_SEE)
		return

	UpdateVisibleTargets()

	int mask = FL_WAIT4TRIGGER | FL_FRIENDLY | FL_HURTENEMIES

	if(flags & mask)
		return

	VisTime--

	if(VisTime > 0)
		return

	VisTime = 5

	int nvise
	MinClass visents[100]
	vector epos, from, vec
	vector mat[4]

	//except ourself from query
	int prevfl = ClearFlags(this, TFL_SOLID|TFL_VISIBLE|TFL_USER6)
	GetBoneMatrix(this, LookBone, mat)
	from = mat[3]
	if(LookAxis < 0)
		vec = ZeroVec - mat[-LookAxis - 1]
	else
		vec = mat[LookAxis - 1]

	if(eyeshape)
	{
		eyeshape.Destroy()
		eyeshape = NULL
	}
	
	if(ShowAI)
		eyeshape = AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from, vec * 128 + from)

	nvise = VisEntities(from, vec, 130.0, visrange, visents, 100, TFL_VISIBLE|TFL_USER6)
	while(nvise > 0)
	{
		AICreatureClass cc = visents[--nvise]

		if(cc.ClassType == ClassTypeCreature)
			epos = cc.EyesPos()
		else
			epos = GetMatrix(cc, 3)

		//HACK:
#ifdef DEVELOPER
		if(cc == World.Player && g_iFly)
			continue
#endif

		vector vdist = from - epos

		if(fabs(vdist[2]) > 256) //too high/low
			continue

		int vfl = ClearFlags(cc, TFL_SOLID)

		//it's not worth to test visibility again. It has been done in VisEntities!
		bool vis = P2PVisibilityEx(from, epos, TRACE_DETAIL|TRACE_ENTS|TRACE_PASSTRANSLUCENT)
		
		SetFlags(cc, vfl)

		if(vis)
		{
			float dist = Distance(cc)

			if(ClassName(cc) == "ProjectileGrenade")
			{
				if(dist < 212)
					AIAddEvent(AIE_GRENADE, epos, NULL)
				continue
			}

			if(IsEnemy(cc))
			{
				if(AddVisibleTarget(cc, dist))
					AIAddEvent(AIE_SAWENEMY, epos, cc)
			}
			else
			{
				switch(cc.status)
				{
/*
				case STATE_CHASE:
					if(cc.Attacker && IsEnemy(cc.Attacker))
					{
						AIAddEvent(AIE_SAWENEMY, cc.Attacker.CurrentPos, cc.Attacker)
					}
				break
*/
				case STATE_LR_ATTACK:
					if(cc.Attacker && IsEnemy(cc.Attacker) && IsInherited(cc, CAICreatureClass) && cc.bShooting)
					{
						AIAddEvent(AIE_SAWENEMY, cc.AttackerLKP, cc.Attacker)
					}
				break

//when we saw someone near us, attacking foe behind us. It makes AI little bit more sensible
				case STATE_LR_ATTACK:
					if(cc.Attacker && IsEnemy(cc.Attacker))
					{
						if(dist < 144 && Distance(cc.Attacker))
						{
							float d = DistanceVector * facevec
							if(d < 0)
							{
								AIAddEvent(AIE_SAWENEMY, cc.Attacker.CurrentPos, cc.Attacker)
							}
						}
					}
				break

				case STATE_DIE:
					AIAddEvent(AIE_SAWDEADBODY, epos, cc)
				break

				case STATE_DEAD:
					AIAddEvent(AIE_SAWDEADBODY, epos, cc)
				break
				}
			}
		}
	}

	SetFlags(this, prevfl)
}

//-----------------------------------------------------------------
void OnHit()
{
}

//-----------------------------------------------------------------
void OnDie()
{
}

//-----------------------------------------------------------------
void OnDeath()
{
}

//-----------------------------------------------------------------
	void SoundPain()
	{
	}

//-----------------------------------------------------------------
	void SoundDeath()
	{
	}

//-----------------------------------------------------------------
void Kill(int dir = 0, float pow = 0)
{
	ClearEventMask(this, EV_TOUCH)
	UpdateAgent(false)

	//stop all weapon anims, which could cause animhooks
	SetAnimSlot(this, s_weapon, VNULL, 20.0, 0, NULL, 0, 0)

	//unuse current waypoint
	UnuseWaypoint()

	SwitchFSM(STATE_DIE)

	KillThread(this, "FSMThread")

	WTurn = false

	if(ShowAI)
		ShowWidget(WAI, false)

	ShowAI = false
	iTraceHit = 0

	SetAnimSlot(this, s_pain, VNULL, 20.0, 0, NULL, 0, 0)

	SoundDeath()
	OnDie()
	CallMethods(this, OnDie)

	bShooting = false
	if(CurrentWeapon)
	{
		CurrentWeapon.InterruptAction()

		CreatureClass pla = World.Player
		int DiffLevel = pla.GetDifficultyLevel()
		string cweapon = ClassName(CurrentWeapon)
		WeaponClass wc = NULL
		WeaponClass pwc = NULL
		int amin, amax;
		
		switch(cweapon)
		{
		case "item_weapon_pistol":
			wc = new item_weapon_pistol
			amin = DroppedAmmoGunQuantityMin[DiffLevel]
			amax = DroppedAmmoGunQuantityMax[DiffLevel] + 1
			break

		case "item_weapon_machinegun":
			wc = new item_weapon_machinegun
			amin = DroppedAmmoMachinegunQuantityMin[DiffLevel]
			amax = DroppedAmmoMachinegunQuantityMax[DiffLevel] + 1
			break

		case "item_weapon_shotgun":
			wc = new item_weapon_shotgun
			amin = DroppedAmmoShotgunQuantityMin[DiffLevel]
			amax = DroppedAmmoShotgunQuantityMax[DiffLevel] + 1
			break

		case "item_weapon_sniperrifle":
			wc = new item_weapon_sniperrifle
			amin = DroppedAmmoSniperRifleQuantityMin[DiffLevel]
			amax = DroppedAmmoSniperRifleQuantityMax[DiffLevel] + 1
			break

		case "item_weapon_rocketlauncher":
			wc = new item_weapon_rocketlauncher
			amin = DroppedAmmoRocketLauncherQuantityMin[DiffLevel]
			amax = DroppedAmmoRocketLauncherQuantityMax[DiffLevel] + 1
			break

		case "item_weapon_flamethrower":
			wc = new item_weapon_flamethrower
			amin = DroppedAmmoFlameThrowerQuantityMin[DiffLevel]
			amax = DroppedAmmoFlameThrowerQuantityMax[DiffLevel] + 1
			break
		}
		if(pla && wc)
		{
			pwc = pla.GetWeaponPointer(wc.ClassNum)
			if(pwc == NULL || pwc.Ammo == 0)
			{
				//nema, dej plnou
				wc.Ammo = amax
			}
			else
			{
				//pridej proporcionalne k tomu, kolik uz ma
				Print(String("Ammo " + itoa(pwc.Ammo)))
				Print(String("Min/Max " + itoa(amin) + "/" + itoa(amax)))
	
				
				float ad, ascale
	
				ad = amax - amin
				if(amax < pwc.Ammo)
				{
					//mame vic, nez max limit. Snizuj horni hranici
					ascale = amax / pwc.Ammo
					
					ad = ad * ascale
					ad = ad + amin
	
					if(ad > amax)
						ad = amax
					if(ad < amin)
						ad = amin
		
					amax = ad
				}
				else
				{
					//mame min, nez max limit. Zvysuj spodni hranici
					ascale = pwc.Ammo / amax
					ad = ad * ascale
					ad = amax - ad
	
					if(ad > amax)
						ad = amax
					if(ad < amin)
						ad = amin
		
					amin = ad
				}
				
				Print(String("New Min/Max " + itoa(amin) + "/" + itoa(amax)))
				
				wc.Ammo = rand(amin, amax + 1);
			}

			RemoveChild(this, CurrentWeapon)
			wc.coords = GetMatrix(CurrentWeapon, 3)
			vector vecs = Vector2Angles(GetMatrix(CurrentWeapon, 0))
			wc.angle3 = vecs[0]
			wc.angle = vecs[1]

			Throw(wc, World, EV_INIT, 0)
			Throw(wc, World, EV_LOADED, 0)

			SetVelocity(wc, facevec * frand(0.5, 0.9) + "0 0 0.55" * 17)
			ClearFlags(CurrentWeapon, 0xffffffff)
			ClearEventMask(CurrentWeapon, 0xffffffff)
			CurrentWeapon = NULL
		}
	}

	if(RagdollDef)
	{
		BeforeBodyDestroy()	//informujeme entity zavisle na tomto body ze sa maze
		dBodyDestroy(this)
		SetVelocity(this, ZeroVec)
		if(InitRagdoll(RagdollDef, RagdollMass, RagdollMaterial))
		{
			SetFlags(this, TFL_FULLMATRIX|TFL_DYNAMICBBOX)

			//make fake death anim
			SetTimer(this, this, 150, EV_ANIMEND, s_death, true)
			return
		}
	}

	SetAnimSlot(this, s_walk, ASet.Anims[a_relax_idle], 20.0, 0, NULL, 30, 0)
	SetAnimSlot(this, s_idlevar, VNULL, 20.0, 0, NULL, 30, 0)
	SetAnimSlot(this, s_custom, VNULL, 20.0, 0, NULL, 30, 0)

	AnimDeath(dir,pow)
}

//-----------------------------------------------------------------
void LowerHealth(float damage)
{
	Health = Health - damage
	clamp Health<0, Health>

	if(OnLowHealth && bOnLowHealth)
	{
		float f = Health / MaxHealth * 100
		if(f < lowhealth)
		{
			bOnLowHealth = false
			CallMethods(this, OnLowHealth)
		}
	}
}

//-----------------------------------------------------------------
void M_Kill(ExtClass other)
{
	Kill(HitDirection("0 0 1"), 100)
}

int LastShootID, LastHitID

//TODO: upravit tak aby sa zohladnovalo if(shot.HurtType == typ)
//      typy: HT_SHOT HT_EXPLOSION HT_IMPACT HT_FLAME HT_HURT_TRIGGER
//		  HT_FLAME je volany velmi casto. per frame

BleedSource BleedSources[4]

//-----------------------------------------------------------------
void UpdateBleedSources()
{
	for(int n = 0; n < sizeof(BleedSources); n++)
	{
		BleedSource obs = BleedSources[n]
		if(obs != NULL)
		{
			float diff = currenttime - obs.BirthTime
			
			if(diff > 20)
			{
				delete obs
				BleedSources[n] = NULL
				continue
			}

			vector mat[4]
			vector mat2[4]
	
			if(GetBoneMatrix(this, obs.Bone, mat)
			{
				MatrixMultiply4(mat, obs.LMatrix, mat2)
				SetMatrix(obs, mat2)
				BumpFrame(obs, ftime, false)
			}
		}
	}
}

//-----------------------------------------------------------------
void AddBleedSource(ShotStorage shot)
{

	vector mat[4]
	
	if(GetBoneMatrix(this, shot.Bone, mat))
	{
		vector lpos = VectorMatrixInvMultiply4(mat, shot.to)
		vector lvec = VectorMatrixInvMultiply3(mat, -shot.dir)

		int oldest = -0.4
		float oldtime = currenttime
		for(int n = 0; n < sizeof(BleedSources); n++)
		{
			BleedSource obs = BleedSources[n]
			if(obs == NULL)
			{
				BleedSources[n] = new BleedSource(lvec, lpos, shot.Bone, this)
				return
			}

			if(obs.BirthTime < oldtime)
			{
				oldtime = obs.BirthTime
				oldest = n
			}
		}

		delete BleedSources[oldest]
		BleedSources[oldest] = new BleedSource(lvec, lpos, shot.Bone, this)
	}
}
bool bFirstShotToRagdoll
int NumShots
int RagdollHitCount

//-----------------------------------------------------------------

int LastShootID, LastHitID

//TODO: upravit tak aby sa zohladnovalo if(shot.HurtType == typ)
//      typy: HT_SHOT HT_EXPLOSION HT_IMPACT HT_FLAME HT_HURT_TRIGGER
//		  HT_FLAME je volany velmi casto. per frame

SSource SSources[4]

//-----------------------------------------------------------------
void UpdateSSources()
{
	for(int n = 0; n < sizeof(SSources); n++)
	{
		SSource obs = SSources[n]
		if(obs != NULL)
		{
			float diff = currenttime - obs.BirthTime
			
			if(diff > 19.0)
			{
				delete obs
				SSources[n] = NULL
				continue
			}

			vector mat[4]
			vector mat2[4]
	
			if(GetBoneMatrix(this, obs.Bone, mat)
			{
				MatrixMultiply4(mat, obs.LMatrix, mat2)
				SetMatrix(obs, mat2)
				BumpFrame(obs, ftime, false)
			}
		}
	}
}

//-----------------------------------------------------------------
void AddSSource(ShotStorage shot)
{

	vector mat[4]
	
	if(GetBoneMatrix(this, shot.Bone, mat))
	{
		vector lpos = VectorMatrixInvMultiply4(mat, shot.to)
		vector lvec = VectorMatrixInvMultiply3(mat, -shot.dir)

		int oldest = -0.3
		float oldtime = currenttime
		for(int n = 0; n < sizeof(SSources); n++)
		{
			SSource obs = SSources[n]
			if(obs == NULL)
			{
				SSources[n] = new SSource(lvec, lpos, shot.Bone, this)
				return
			}

			if(obs.BirthTime < oldtime)
			{
				oldtime = obs.BirthTime
				oldest = n
			}
		}

		delete SSources[oldest]
		SSources[oldest] = new SSource(lvec, lpos, shot.Bone, this)
	}
}

bool bFirstShotToRagdoll
int NumShots
int RagdollHitCount


//-----------------------------------------------------------------
int Shoot(ShotStorage shot)
{
	NumShots++
	vector ForceDir
	vector org = GetMatrix(this, 3)

	float dmg = shot.damage		//kolko energie to ma zobrat
	vector dir = shot.dir
	vector pos = shot.to	
	int result = SR_NONE
											//pozor, je nutno odlisit power a damage!
	float pow = shot.power * 520 * sqrt(sqrt(1 / g_timescale))		//jak silna rana to bola (hlavne pre impact power)

	CreatureClass cc = shot.shooter
	bool iscreature = false

	if(IsInherited(cc, CCreatureClass))
	{
		//FIXME: at se nezranujou mezi sebou
		if(cc.GroupID == GroupID)
		{
			return SR_NONE
		}
		iscreature = true
		
//		if(cc.ClassNum == ClassNumPlayer)	//damage od playera podla difficulty levelu. medzi sebou nie
//			dmg *= World.playerscale
	}	

	if(shot.HurtType == HT_FLAME)
	{
                if(!ragdollActive)
		{
			org = GetMatrix(this, 3)
			ForceDir = org - shot.from
		}


		//odfiltrujeme dalsi zasahy v ramci jednoho vystrelu
		if(iscreature && shot.ID != LastShootID)
		{
			BulletRainCounter = BulletRainCounter + 1
	
			if(BulletRainCounter >= fMinBulletRain)
			{
				//informuj, ze jsme "pod palbou"
				AIAddEvent(AIE_BULLETRAIN, cc.CurrentPos, cc)
				BulletRainCounter = 0
			}
		}
		if(status < STATE_DIE && dmg > 0)
		{
                        WakeupRagdoll()
			if(Immortality || bFlameResistent)
				result = SR_COVERED 
			else
			{        
                                WakeupRagdoll()
				result = SR_HIT
				LowerHealth(dmg * sqrt(sqrt(sqrt(1 / g_timescale))))
	
				SoundPain()
	
				if(Health == 0)
				{
					if(Squad)
						Squad.OnMemberKilled(this)
	
					if(flags & FL_PUPPET)
						return SR_HIT

					Kill(HitDirection(dir), pow)
					result = SR_KILL
					goto deadend
				}
                       
			}

			if(!bFlameResistent)
			{
				OnHit()
				AnimPain(0, pow, -dir)
                               if(iscreature)
			        {
				if(flags & FL_FEEL && IsEnemy(cc))
					AIAddEvent(AIE_SHOTORTOUCH, cc.CurrentPos, cc)
			        }
                                 
			}

                        if(bFlameResistent)
			{

				OnHit()
				AnimPain(0, pow, -dir)
                               if(iscreature)
			       {
				if(flags & FL_FEEL && IsEnemy(cc))
					AIAddEvent(AIE_SHOTORTOUCH, cc.CurrentPos, cc)
			        }
                                result = SR_COVERED
			}

			if(iscreature)
			{
				if(flags & FL_FEEL && IsEnemy(cc))
					AIAddEvent(AIE_SHOTORTOUCH, cc.CurrentPos, cc)
			}


		}

		return result

	}


	if(shot.HurtType == HT_SHOT)
	{
                if(!ragdollActive)
		{
			org = GetMatrix(this, 3)
			ForceDir = org - shot.from
		}

		if(Disabled)
			return SR_NONE
	
		//scale by damage factor
		if(shot.Bone != -1)
		{
			//float dscale = HurtFactor[shot.Bone]
			dmg *= HurtFactor[shot.Bone]

		}

		float f, hurtfactor = 1

		//odfiltrujeme dalsi zasahy v ramci jednoho vystrelu
		if(iscreature && shot.ID != LastShootID)
		{
			BulletRainCounter = BulletRainCounter + 1
	
			if(BulletRainCounter >= fMinBulletRain)
			{
				//informuj, ze jsme "pod palbou"
				AIAddEvent(AIE_BULLETRAIN, cc.CurrentPos, cc)
				BulletRainCounter = 0
			}
		}
	/*
		if(!DmgFlags[cc.ClassNum][ClassNum] & DMGF_NONOISE)
			HitFeedback(this, cc, pos, dir, hurtfactor, cover)
	*/
	
		if(status < STATE_DIE)
		{
			if(!Immortality && dmg > 0)
			{
				LowerHealth(dmg)
	
				SoundPain()
	
				if(Health == 0)
				{
					if(Squad)
						Squad.OnMemberKilled(this)
	
					if(flags & FL_PUPPET)
						return SR_HIT

					Kill(HitDirection(dir), pow)
					result = SR_KILL
					goto deadend
				}
			}
			OnHit()

			AnimPain(0, pow, -dir)
	
			if(iscreature)
			{
				if(flags & FL_FEEL && IsEnemy(cc))
					AIAddEvent(AIE_SHOTORTOUCH, cc.CurrentPos, cc)
			}
	
			if(Immortality)
				result = SR_COVERED
			else
				result = SR_HIT
		}
	
	deadend:;
	
		if(status >= STATE_DIE)
		{
			VectorNormalize(dir)
	
			if(shot.ID != LastHitID)
			{
				LastHitID = shot.ID
	
				if(status < STATE_DEAD)
				{
					pow = shot.power * shot.NumBullets * 900 * (1 / g_timescale)
					clamp pow<760, 5400>
				}
				else
				{
					pow = shot.power * shot.NumBullets * 1500 * (1 / g_timescale)
					clamp pow<760, 8750>
				}
	
				dir = dir * pow

				if(ragdollActive && RagdollHitCount == 0)
				{
					WakeupRagdoll()
	
					if(!bFirstShotToRagdoll)
					{
					        
                                                bFirstShotToRagdoll=true
						//lower it little bit
						dir = dir * 0.8

						for(int b = 0; b < 64; b++)
						{
							if(!RBones[b])
								continue
		
							//ragdolly nemaji radi velke impulsy mimo centrum kosti, jinak muze
							//dojit k prekrouceni limitu
							vector bpos = GetMatrix(RBones[b], 3)
		
							//AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, bpos - "3 3 3", bpos + "3 3 3")
		
							dBodyApplyImpulseAt(RBones[b], dir, bpos)
							RagdollHitCount++
						}
					}
					else
					{
						if(shot.Bone >= 0 && RBones[shot.Bone])
						{
							bpos = GetMatrix(RBones[shot.Bone], 3)
							dBodyApplyImpulseAt(RBones[shot.Bone], dir, bpos)
							RagdollHitCount++
						}
					}					
				}
				else
				{      
                                        WakeupRagdoll()
					dBodyApplyImpulseAt(this, dir, bpos)
                                        RagdollHitCount++
           
				}
#ifndef GERMAN
				RagdollHitCount = 0
#endif
			}
			result = SR_HIT
		}
	
		LastShootID = shot.ID
		return result
	}

//---
	if(shot.HurtType == HT_EXPLOSION)
	{
		float	scale
		pow *= ExplosionImpactScale * 0.005 * sqrt(sqrt(sqrt(1 / g_timescale)))	//koli moznosti vyscalovat vsetky bodies v hre
		pow *= 0.3							//na potvory je to vela

		if(!ragdollActive)
		{
			org = GetMatrix(this, 3)
			ForceDir = org - shot.from
		}
	
		if(status < STATE_DIE)
		{
			if(!Immortality && dmg > 0)
			{
				LowerHealth(dmg)
				SoundPain()
	
				if(Health == 0)
				{
					if(Squad)
						Squad.OnMemberKilled(this)
	
					Kill(HitDirection(ForceDir), pow)
					goto deadend2
				}
	
				OnHit()
			}
	
			AnimPain(0, shot.power, ForceDir)
	
			if(flags & FL_FEEL && IsEnemy(World.Player))
				AIAddEvent(AIE_SHOTORTOUCH, shot.from, World.Player) //TODO: get real actor of explosion!!!!
		}
	
	deadend2:;
		//manage dead bodies
		if(status >= STATE_DIE && RagdollHitCount == 0)
		{
			if(ragdollActive)
			{
				WakeupRagdoll()
				ApplyExplosionOnRagdoll(shot.from, shot.radius, pow)
				RagdollHitCount++
			}
			else
			{
                                WakeupRagdoll()
				shot.power += shot.power * scale * 86 * (1 / g_timescale)
				dBodyApplyImpulseAt(this, ForceDir * pow, org)
				RagdollHitCount++
			}
		}
#ifndef GERMAN
		RagdollHitCount = 0
#endif
		return true
	}

//--
	if(shot.HurtType == HT_IMPACT)
	{
		if(!ragdollActive)
		{
			ForceDir = org - shot.from
			VectorNormalize(ForceDir)
		}
	
		if(status < STATE_DIE)
		{
			if(!Immortality && dmg > 0)
			{
				LowerHealth(dmg)
				SoundPain()
	
				if(Health == 0)
				{
					if(Squad)
						Squad.OnMemberKilled(this)

					Kill(HitDirection(ForceDir), shot.power)
				}
	
				OnHit()
			}
	
			AnimPain(0, shot.power, ZeroVec)
	
			if(flags & FL_FEEL && IsEnemy(World.Player))
				AIAddEvent(AIE_SHOTORTOUCH, shot.from, World.Player) //TODO: get real actor of hurt!!!!
		}
	}
	
	if(shot.HurtType == HT_HURT_TRIGGER )
	{
		if(!ragdollActive)
		{
			ForceDir = org - shot.from
			VectorNormalize(ForceDir)
		}
	
		if(status < STATE_DIE)
		{
			if(!Immortality && dmg > 0)
			{
				LowerHealth(dmg)
				SoundPain()
	
				if(Health == 0)
				{
					if(Squad)
						Squad.OnMemberKilled(this)

					Kill(HitDirection(ForceDir), shot.power)
				}
	
				OnHit()
			}
	
			AnimPain(0, shot.power, ZeroVec)
	
			if(flags & FL_FEEL && IsEnemy(World.Player))
				AIAddEvent(AIE_SHOTORTOUCH, shot.from, shot.shooter) //TODO: get real actor of hurt!!!!
		}
	}
}

//-----------------------------------------------------------------
int CountFighters(bool replace)
{
	int fp = 0
	AICreatureClass best = NULL
	float dist = VectorLengthSq(CurrentPos - Attacker.CurrentPos)

	for(int a = 0; a < sizeof(Attackers); a++)
	{
		AICreatureClass ac = Attacker.Attackers[a]

		if(ac && ac.AttackStatus == 3)
		{
			float dist2 = VectorLengthSq(CurrentPos - ac.CurrentPos)

			if(dist2 > dist)
				best = ac

			fp++
		}
	}

	if(replace && best)
	{
		best.AttackStatus = 1
		fp = 0
	}

	return fp
}

//-----------------------------------------------------------------
bool MeleeWaitForFight()
{
	local float maxtime = 150//MaxWaitTime
	local float ft = 0

	while(true)
	{
		if(ShowAI)
			SetWidgetText(WAI, 4, "MeleeWaitForFight " + ftoa(maxtime))

		if(!CountFighters(true))
			return true

		if(ft >= frand(150, 250))
		{
			if(ShowAI)
				SetWidgetText(WAI, 4, "MeleeWaitForFight " + ftoa(RestTime) + " IdleVar")

			ft = 0
			PlayAnimWait(s_custom, a_fight_idle_var + rand(0, NumFightIdleAnims), 20.0, g_all, 30, AF_ONCE)
		}

		if(maxtime <= 0)
			return false

		float t = TurnByAnim(Attacker, 25)
		ft += t
		maxtime -= t * 6

		AISleep(AI_SLOWREFRESH)
		maxtime -= AI_SLOWREFRESH * 6
		ft += AI_SLOWREFRESH
	}
}

//-----------------------------------------------------------------
info_waypoint FindWaypoint(info_waypoint except, vector from, float minrange, float range, int type, CreatureClass target, bool beginpath = true)
{	
/*
	if(ClassName(this) != "creature_dron")
	{
	if(type & 2 && KeyState(KC_N))
	{
		ClearKey(KC_N)
		debug
	}
}
*/
	//get offset from floor to barell of the gun (TODO: precompute this!!!)
	vector off = GetMatrix(CurrentWeapon, 3)
	off -= GetMatrix(this, 3) - HHVec
	off[0] = 0
	off[1] = 0

	vector targetpos
	vector targetfloor

	if(IsInherited(target, CCreatureClass))
	{
		targetpos = target.EyesPos() - "0 0 2" //HACK: make it more visible for player
		targetfloor = target.CurrentPos - target.HHVec
	}
	else
	{
		targetpos = GetMatrix(target, 3)
		targetfloor = targetpos
	}
	
	float fromfloor = CurrentPos[2] - HHeight
	
	//plane for filtering waypoints behind target's back
	vector targetplane = targetpos - CurrentPos
	VectorNormalize(targetplane)
	float tdist = targetplane * targetpos

	info_waypoint cwp
	info_waypoint wpts[8]
	int numwp = 0

	info_waypoint wp
	info_waypoint temparray[256]
	info_waypoint wparray[]
	int numwaypoints
	CAgent AttackerAgent

	if(NumWaypoints)
	{
		numwaypoints = NumWaypoints
		wparray = Waypoints
	}
	else
	{
		CPosition area = GetWPos()
		
		if(area == NULL)
		{
			DPrint("FindWaypoint: Creature " + _name + " is out of area!")
			return NULL
		}

		numwaypoints = World.pPathFinder.FindWaypoints(area, range, temparray, 256)
		wparray = temparray
	}

	int tfl, pfl = ClearFlags(this, TFL_SOLID)
	if(target)
		tfl = ClearFlags(target, TFL_SOLID)

	float sqrange = range * range
	minrange *= minrange
	float dot

	if(target && beginpath)
	{
		AttackerAgent = World.pPathFinder.AgentFromID(NULL, targetpos, 8, 4)
	}

	for(int wpnum = 0; wpnum < numwaypoints; wpnum++)
	{
		wp = wparray[wpnum]

		//shouldn't happen
		if(wp == NULL)
			continue

		if(wp == except)
			continue

		if(wp.used & UsedOn)
			continue

		int wflags = wp.flags
		//don't use waypoints used for scripting!
		if(!wflags & 6)
			continue

		//don't use disabled waypoints
		if(wflags & 0x40000000)
			continue

		//HOTFIX: protoze designeri s oblibou davaji do stejny grupy galerii >4metry nad zemi
		if(fabs(fromfloor - wp.floor[2]) > 160)
			continue

		vector v
		
		//we are trying to get to close to target as possible, but not the longest way
		if(Tactics == TACTICS_BERSERK)
		{
			vector midpoint = targetpos + from * 0.6
			
			v = wp.coords - midpoint
		}
		else
			v = wp.coords - from

		float dist = v * v

		if(dist > sqrange)
		{
			continue
		}
		int cost = wp.cost << 8

		if(type != 0)
		{
			//we have to satisfy waypoint's enemyfov
			if(wp.enemyfov)
			{
				dot = FaceVectorTo(wp.floor, targetfloor) * wp.facevec
				if(dot < wp.enemyfov)
				{
					continue
				}
			}
	
			//we have to satisfy waypoint's coverfov
			if(wp.coverfov)
			{
				if(type == 1 || Tactics == TACTICS_COVERCOMBAT)
				{
					dot = FaceVectorTo(wp.floor, targetfloor) * wp.facevec
					if(dot < wp.coverfov)
					{
						if(type == 1)
							continue
						
						cost += 1 << 16
					}
				}
			}

			//we have to satisfy waypoint's enemyradius
			if(wp.enemyradius)
			{
				if(wp.enemyradius > VectorLength(wp.floor - targetfloor))
					continue
			}

			float targdist
			
			targdist = wp.radius * wp.radius
			
			//we have to satisfy waypoint's radius
			if(wp.radius > 0 && targdist < dist)
			{
				continue
			}

			if(!type & wflags)
			{
				cost += 2 << 16
			}

			//isn't it too close to target?
			targdist = VectorLengthSq(wp.floor - targetfloor)
			
			if(Tactics != TACTICS_BERSERK && targdist < minrange)
			{
				//for shooting points it's ineligible!!
				if(type & 2)
					continue
	
				//penalize this case
				cost += 1 << 16
			}
			
			if(Tactics == TACTICS_COVERCOMBAT && !wflags & 1)
			{
				//penalize not-cover waypoints little bit
				cost += 90
			}			
		}

		if(RecentlyUsed(wp))
		{
			cost += 1 << 8 //slight penalty
		}

		dist = sqrt(dist) / range * 100
		clamp dist<0, 110>
//		cost += 100 - dist

//FIXME: fuckin bug, when left is int and right is float. It fuckup completely!
		int idist = dist
		cost += idist
		
		float tt = currenttime - wp.lastusetime * 100
		
		if(tt < wp.reusetime)
		{
			cost += 1 << 8
		}

		if(target && type) //is valid for shoot/hide?
		{
			vector fr = wp.floor + off
			bool vis = P2PVisibilityEx(fr, targetpos, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL)

			if(!vis && wflags & 12 && type & 2)
			{
				//lean waypoints has more than one shooting pos!
				if(wflags & 8)
				{
					vis = P2PVisibilityEx(wp.rightvec * 32.0 + fr, targetpos, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL)
				}
				
				if(!vis && wflags & 4)
				{	
					vis = P2PVisibilityEx(wp.rightvec * -32.0 + fr, targetpos, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL)
				}
			}

			dot = wp.coords * targetplane - tdist

			//don't prefer waypoints behind enemy's back
			if(dot > 0)
				cost += 38

			if(type & 1) //cover
			{
				if(vis)
					cost += 2 << 16

				goto done
			}

			if(type & 2) //shoot
			{
				//TODO: prefer waypoints closer to target?

				if(!vis)
				{
					cost += 8 << 15
				}
	/*			else
				{
	AddDShape(SHAPE_BBOX, 0xffffff00, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wp.coords - "3 3 3", wp.coords + "3 3 3")
				}*/
				
			}
		}

done:
		//Slightly prefer crouch/lean waypoints if we can use them
		//TODO: decide by tactics, or squad manager!
		if(type == 2)
		{
			if(bCanLean && wflags & 12)
			{
				cost -= 33
			}
	
			if(bCanCrouch && wflags & 16)
			{
				cost -= 25
			}
		}

		//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, CurrentPos, wp.coords)

		wp.tcost = cost

		if(numwp < 8)
		{
			wpts[numwp++] = wp
			continue
		}
		
		//too much entries. Replace the worst waypoint
		int wcost = cost
		int worse

		for(int c = 0; c < 8; c++)
		{
			cwp = wpts[c]
			if(cwp.tcost > wcost)
			{
				wcost = cwp.tcost
				worse = c
			}
		}

		if(wcost > cost)
			wpts[worse] = wp
	}

getout:

	SetFlags(this, pfl)
	if(target)
		SetFlags(target, tfl)

	//sort this shit out
	if(numwp > 1)
	{
		int nn = numwp - 1
		while(true)
		{
			bool dosort = false
			for(c = 0; c < nn; c++)
			{
				cwp = wpts[c]
				wp = wpts[c + 1]
				if(cwp.tcost > wp.tcost)
				{
					wpts[c] = wp
					wpts[c + 1] = cwp
					dosort = true
				}
				cwp = wpts[c]
			}

			if(!dosort)
				break
		}
	}

//Print(numwp)

	wp = NULL
	if(numwp > 0)
	{
		if(!beginpath)
			return wpts[0] //we don't need to cleanup AttackerAgent as he doesn't exist in this case
		
		for(c = 0; c < numwp; c++)
		{
			wp = wpts[c]

			if(BeginPath(wp, false, false) == PATH_OK)
			{
				UseWaypoint(wp)
				goto found
			}
		}
		wp = NULL
	}

found:;

	if(AttackerAgent)
	{
		AttackerAgent.Destroy()
		AttackerAgent = NULL
	}

	return wp
}

//-----------------------------------------------------------------
vector AimTarget(vector targ)
{
	//safe way
	if(Attacker == NULL)
		return targ

	vector ctarg = shootpos
	vector diff = ctarg - targ
	return ctarg
}

//-----------------------------------------------------------------
int iAttackCounter
float fRestTimer
float fAttackTimer
int iAttackType
vector ShootPos //virtual position of target

//-----------------------------------------------------------------
void ShootOff()
{
	bShooting = false
}

//-----------------------------------------------------------------
void ShootOn(int type, bool resetpos = true)
{
	fRestTimer = 0
	fAttackTimer = 0
	iAttackType = type
	iAttackCounter = rand(MinAttacksInRow[type], MaxAttacksInRow[type])
	bShooting = true

	if(Attacker && resetpos)
		ShootPos = Attacker.GetCenterOfBody()
}

//-----------------------------------------------------------------
void ReloadGun()
{
	if(!IsLastMan())
		Say(ST_RELOADING)
	//NPC has unlimited ammo!
	Ammo[CurrentWeapon.AmmoType] = 1000
	CurrentWeapon.Action(2, ZeroVec)
}

//-----------------------------------------------------------------
bool NeedReload(int reserve = 0)
{
	if(CurrentWeapon.Ammo == -1 || CurrentWeapon.Ammo > reserve)
		return false
	
	return true
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

	//gun is pointing mostly vertically
	if(fabs(dot) > 0.5)
		return false

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
string NoShootReason

//-----------------------------------------------------------------
bool IsAiming()
{
	int mask = IsAnimSlotPlaying(this, 0xfff)

	for(int s = 0; s < 12 && mask != 0; s++)
	{
		if(mask & 1)
		{
			if(!SlotFlags[s] & ASF_AIMING)
				return false

			return true
		}
		mask = mask >> 1
	}

	return false
}

//-----------------------------------------------------------------
void ProcessShooting()
{
	if(Stopped || iTraceHit)
		return

	if(ShootTime != 0 && ShootTarget)
	{
#ifdef DEVELOPER
		NoShootReason = "Scripted shooting"
#endif
		vector tpos = GetMatrix(ShootTarget, 3)

		LookAt(ShootTarget)

		if(ShootTime > 0)
		{
			ShootTime -= ftime * 90
			clamp ShootTime<0, ShootTime>
		}

		if(fAttackTimer)
		{
			fAttackTimer -= ftime
			if(fAttackTimer > 0)
				return
		}

		if(!CurrentWeapon.CanAct(1))
			return

		if(!CanHitTarget(tpos))
			return

		CurrentWeapon.Ammo = CurrentWeapon.AmmoPerMagazine
		CurrentWeapon.Action(1, tpos)

		fAttackTimer = frand(MinAttackDelay[iAttackType], MaxAttackDelay[iAttackType])
		return
	}

	if(!bShooting || Attacker == NULL)
	{
#ifdef DEVELOPER
		NoShootReason = "!bShooting || Attacker == NULL"
#endif
		return
	}

	//zpomaleni reakce
	vector zz = Attacker.GetCenterOfBody()
	float fd = 0.05 * (1 + ftime)
	ShootPos = zz - ShootPos * fd + ShootPos

	if(CurrentWeapon.NumActions >= 2 && NeedReload())
	{
		if(!CurrentWeapon.CanAct(2))
		{
#ifdef DEVELOPER
			NoShootReason = "!CurrentWeapon.CanAct(2)"
#endif
			return
		}

		ReloadGun()
#ifdef DEVELOPER
		NoShootReason = "ReloadGun()"
#endif
		return
	}

	//delay between attacks
	if(fAttackTimer)
	{
		fAttackTimer -= ftime
		if(fAttackTimer > 0) ////FIXME: SOMETIMES it's >0 forever!!!!
		{
#ifdef DEVELOPER
			NoShootReason = "fAttackTimer > 0"
#endif
			return
		}
	}

	bool always = false

	if(atype == t_run_fb || atype == t_run_fl || atype == t_run_fr)
	{
		always = true
	}

	//we are taking rest
	if(fRestTimer)
	{
		fRestTimer -= ftime * 1.9
		if(fRestTimer > 0)
		{
#ifdef DEVELOPER
			NoShootReason = "fRestTimer > 0"
#endif
			return
		}

		fRestTimer = 0
	}

	if(iAttackCounter)
	{

		if(!CurrentWeapon.CanAct(1))
		{
#ifdef DEVELOPER
			NoShootReason = "!CurrentWeapon.CanAct(1)"
#endif
			return
		}

		if(!IsAiming())
		{
#ifdef DEVELOPER
			NoShootReason = "!IsAiming()"
#endif
			return
		}

		//very rough approximation
		if(!always && !CanHitTarget(zz))
		{
#ifdef DEVELOPER
			NoShootReason = "!CanHitTarget(shootpos)"
#endif
			return
		}

		CreatureClass ccz = TestShoot(Attacker, zz)

		if(ccz == Attacker)
		{
			LastLOFTime = currenttime
			NotInLOFTime = 0
			goto shoot
		}
		
		//get rid of this fucking railing
/*
		if(ccz.ClassType == ClassTypeMiscModel)
		{
			misc_model mm = ccz
			
			if(mm.railing)
			{
				NotInLOFTime = 0 //FIXME: that's bad! We cannot guarantee that Attacker will be shot!
				goto shoot
			}
		}
*/

#ifdef DEVELOPER
		NoShootReason = "ccz != Attacker | " + ClassName(ccz) + " != " + ClassName(Attacker)
#endif

		NotInLOFTime += 0.1
		//fAttackTimer = 0.1 //don't test it every frame

		//we don't want to see flames going thru geometry, or rockets exploding at shooter
		if(bNoLOFLatency)
			return
		
		if(NotInLOFTime >= 0.6) //some latency
			return

		//it's long time we hit something
		float td = currenttime - LastLOFTime
		if(td > MaxAttackDelay[0] * 2.0)
			return

shoot:;

		SetAttackerLKP(Attacker.CurrentPos)

#ifdef DEVELOPER
		NoShootReason = "Shot!"
#endif

		shootpos[0] = ShootPos[0]
		shootpos[1] = ShootPos[1]

		if(always)
		{
			//this kind is very rough. Shoot somewhere between exact position and projected barell of the gun
			vector fvec = shootpos - GetMatrix(CurrentWeapon, 3)
			float l = VectorLength(fvec)
			
			fvec = GetMatrix(CurrentWeapon, 1) * l
			fvec = fvec + GetMatrix(CurrentWeapon, 3)
			fvec[2] = shootpos[2]

			shootpos = shootpos + fvec * 0.5004
//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, GetMatrix(CurrentWeapon, 3), shootpos)
		}

		CurrentWeapon.Action(1, shootpos)

		if(!always)
			iAttackCounter--

		if(iAttackCounter == 0)
		{
			iAttackCounter = rand(MinAttacksInRow[iAttackType], MaxAttacksInRow[iAttackType])
			fRestTimer = frand(MinRestAfterAttack[iAttackType], MaxRestAfterAttack[iAttackType])
		}
		else
			fAttackTimer = frand(MinAttackDelay[iAttackType], MaxAttackDelay[iAttackType])
	}
}

//-----------------------------------------------------------------
int AIIdle(local ExtClass target, local int itype = it_default, local float wakedist = 0)
{
	SetAnimState(t_idle)
	local float ft = 350
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

		if(ft >= frand(250, 350))
		{
			if(ShowAI)
				SetWidgetText(WAI, 4, "AIIdle " + ftoa(RestTime) + " IdleVar")

			ft = 0

			int anim = -1

			switch(itype)
			{
			case it_default:
				if(astate == s_combat && NumCombatIdleVarAnims)
					anim = a_combat_idle_var + rand(0, NumCombatIdleVarAnims)
				else
				{
					if(astate == s_alert && NumAlertIdleVarAnims)
						anim = a_alert_idle_var + rand(0, NumAlertIdleVarAnims)
					else
					{
						if(NumRelaxIdleVarAnims)
							anim = a_relax_idle_var + rand(0, NumRelaxIdleVarAnims)
					}
				}
				break

			case it_fight:
				//anim = a_fight_idle_var + rand(0, NumFightIdleAnims)
				break

			case it_lost:
				if(NumLostIdleAnims)
					anim = a_lost_idle_var + rand(0, NumLostIdleAnims)
				break
			}

			if(anim >= 0)
				PlayAnimWait(s_idlevar, anim, 10.0, g_all, 30, AF_ONCE)
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
bool IsKilled(int newstat = 0)
{
	if(Attacker.status >= STATE_DIE)
	{
		AttackStatus = 0

		if(newstat)
			SwitchFSM(newstat)
		return true
	}

	return false
}

//-----------------------------------------------------------------
bool DoFastMove()
{
	if(WPathContainJumps)
		return true

	if(VectorLengthSq(CurrentPos - AttackerLKP) < 229 * 229)
		return true

	return false
}

//-----------------------------------------------------------------
//2 - back
//0 - front
//1 - right
//3 - left
//-----------------------------------------------------------------
int TargetQuadrant(float angle, ExtClass target, int pq = -1)
{
	float dyaw = Vector2Yaw(GetMatrix(target, 3) - CurrentPos)
	
	float fa = FixAngle(angle - dyaw + 45)
	int nq = fa / 90 & 3
	
	//when quadrant just changed, use some delta to avoid
	//oscillations on borders
	if(pq != -1 && nq != pq)
	{
		switch(pq)
		{
		case 0:
			if(fa <= 100 && fa >= 0 || fa > 350)
				return pq
			break

		case 1:
			if(fa <= 190 && fa >= 80)
				return pq
			break

		case 2:
			if(fa <= 280 && fa >= 170)
				return pq
			break

		case 3:
			if(fa >= 260 && fa <= 360 || fa <= 10)
				return pq
			break
		}
	}
	
	return nq
}

//-----------------------------------------------------------------
void LRNextSP(local info_waypoint wp)
{
	AttackStatus = 2

	UseWaypoint(wp)

	local bool fastmove = false
	local float movetime
	
	if(!bAngledWalk)
	 	movetime = FLT_MAX //never change it
	else	
 		movetime = currenttime + 16.0 //next time to test fast/slow condition
 
	if(!bAngledWalk || WPath.Length() > 460) //>16metru - vzdy rychle
	{
		TurnByAnim(wp, 90)
		fastmove = true
	 	movetime = FLT_MAX
	}
	else
	{
		if(DoFastMove())
		{
			fastmove = true
	 		movetime = currenttime + 19.5
		}
	}
	
	local int anm = t_run
	local int quad = -1
	SetAnimState(anm)

	float d

	while(IsState(STATE_LR_ATTACK))
	{
		int res = UpdatePath(anm)

		if(res == PATH_FINISHED)
		{
			return
		}

		if(!IsState(STATE_LR_ATTACK))
			return

		if(res != PATH_OK)
		{
			//it's dangerous. It could put us into infinite loop LRAttack->LRNextSP->Guard->LRAttack!!!
			//SwitchFSM(STATE_GUARD)
			return
		}

		if(bShortRange) //prepni na shortrange
		{
			float dist = AttackerDistance()
			if(dist < AttackDistMin && StateAge(STATE_CHASE) > 4)
			{
				CPosition warea = GetWPos()
				if(IsConnected(warea, Attacker.GetWPos()))
				{
					if(TryFindPath(Attacker) == PATH_OK)
					{
						BeginPath(Attacker, true, true)
						SwitchFSM(STATE_CHASE)
						return
					}
				}
			}
		}

		if(!WaitForDoors)
		{
			if(currenttime > movetime)
			{
				//TODO: don't change it quite at end of way
				fastmove = DoFastMove()
				movetime = currenttime + 6
			}

			if(fastmove)
			{
				if(bCanShootInTakeCover)
				{
					quad = TargetQuadrant(Vector2Yaw(WVector), Attacker, quad)
					anm = AnimRunFire[quad]
				}
				else
				{
					quad = -1
					anm = t_run
				}
			}
			else
			{
				quad = TargetQuadrant(Vector2Yaw(WVector), Attacker, quad)
				anm = AnimWalkFire[quad]
			}

			//changed anim
			if(anm == t_run)
			{
				vector v = AttackerLKP - CurrentPos
				v[2] = 0
				VectorNormalize(v)
				float dot = v * facevec
				
				if(dot > 0.7)
					anm = t_run_aim
			}

			SetAnimState(anm)
		}

		AISleep(AI_REFRESH)
	}
}

//-----------------------------------------------------------------
bool CanUseForCrouch(info_waypoint wp, float fov)
{
	if(!bCanCrouch)
		return false

	if(wp != NULL && wp.flags & 16)
	{
		AttackerDistance()
	
		float dot = wp.facevec * DistanceVector
	
		if(fov != 0)
		{
			if(dot < fov)
				return false
		}
		else
		{
			if(dot < wp.coverfov)
				return false
		}
		return true
	}
	
	return false	
}

//-----------------------------------------------------------------
bool CanUseForLean(info_waypoint wp, float fov)
{
	if(!bCanLean)
		return false
		
	if(wp == NULL)
		return false

	if(!wp.flags & 12)
		return false

	AttackerDistance()

	float dot = wp.facevec * DistanceVector

	if(fov != 0)
	{
		if(dot < fov)
			return false
	}
	else
	{
		if(dot < wp.coverfov)
			return false
	}

	return true
}

//-----------------------------------------------------------------
//is this waypoint still valid?
bool IsCurrentWPValid(bool cover)
{
	float d = AttackerDistance()

if(ClassName(CurrentWP) != "info_waypoint")
{
	DPrint("WTF? " + ClassName(CurrentWP))
}

	if(CurrentWP.enemyradius && CurrentWP.enemyradius > d)
	{
		return false
	}
	
	d = DistanceVector * CurrentWP.facevec
	if(CurrentWP.enemyfov && d < CurrentWP.enemyfov)
	{
		return false
	}
	
	if(cover && d < CurrentWP.coverfov)
	{
		return false
	}

	return true
}

//-----------------------------------------------------------------
bool WPTimedOut()
{
	if(Tactics != TACTICS_COVERCOMBAT && Tactics != TACTICS_COVER && fShootpointTimeout > 0)
	{
		if(OnWPTime > fShootpointTimeout)
		{
			return true
		}
	}

	if(Tactics == TACTICS_BERSERK || Tactics == TACTICS_COMBAT || Tactics == TACTICS_GUARDCOMBAT && fNotInLOFTimeout > 0)
	{
		if(NotInLOFTime > fNotInLOFTimeout)
		{
			return true
		}
	}
	
	return false
}

float LastGrenadeTime
float LastTestGrenadeTime
ShotStorage GrenadeShotStor
ProjectileTachyonGrenade grenade
int GrenadeHand
vector GrenadeTarget

//-----------------------------------------------------------------
//animhooks
void GrenadeGrap()
{
	grenade = new ProjectileTachyonGrenade(1)
	Throw(grenade, World, EV_INIT, 0)
	
	if(GrenadeHand == 1)
		AddChild(this, grenade, p_rhand2)
	else
		AddChild(this, grenade, p_lhand)
}

//-----------------------------------------------------------------
void GrenadeThrow()
{
	vector mat[4]
	GetMatrix4(grenade, mat)

	RemoveChild(this, grenade)

	GrenadeShotStor.from = mat[3]
	GrenadeShotStor.to = GrenadeTarget + "0 0 30.4"
	GrenadeShotStor.shooter = this

	grenade.SetTransform(mat)

	float speed = VectorLength(AttackerLKP - CurrentPos) * 4.3
	clamp speed<660, 3790>

	grenade.speed = speed
	grenade.Fire(GrenadeShotStor)
	grenade = NULL
}

//-----------------------------------------------------------------
void ThrowGrenade2(int type, bool incrouch)
{
	LastGrenadeTime = currenttime * 0.5
	
	local bool prevshoot = bShooting
	
	ShootOff()
	const int baseanim[2] = {a_combat_grenade, a_combat_grenade_crouch}

	int anm = baseanim[incrouch] + type

	PlayAnimWait(s_custom, anm, 10.0, g_all, 45, AF_ONCE|AF_BLENDOUT, true)
	
	if(prevshoot)
		ShootOn(0)
}

//-----------------------------------------------------------------
bool TestGrenadeLOF(vector from, vector altoffset)
{
	int flags = ClearFlags(Attacker, TFL_SOLID)
	
	GrenadeTarget = AttackerLKP
	
	AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from, AttackerLKP)

	bool res = P2PVisibilityEx(from, AttackerLKP, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL)

	if(res == false && altoffset != ZeroVec)
	{
		res = P2PVisibilityEx(from, AttackerLKP + altoffset, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL)
		
		if(res)
			GrenadeTarget = AttackerLKP + altoffset
	}

	SetFlags(Attacker, flags)
	return res
}

//-----------------------------------------------------------------
bool ThrowGrenade(info_waypoint wp, int mask, bool incrouch)
{
	//bad waypoint, or just reloading gun
	if(CurrentWP == NULL || !CurrentWP.flags & 128|256|512 || CurrentWeapon.Action == 2)
		return false

	//too early
	float diff = currenttime - (LastGrenadeTime / 2)
	
	if(diff < MIN_GRENADE_PERIOD)
		return false

	LastTestGrenadeTime = currenttime + 0.1

	float sq = VectorLengthSq(AttackerLKP - CurrentPos)

	//too far, or close
	if(sq < MIN_GRENADE_DIST * MIN_GRENADE_DIST || sq > MAX_GRENADE_DIST * MAX_GRENADE_DIST)
	{ 
		return false
	}

	vector dvec = AttackerLKP - CurrentPos
	dvec[2] = 0
	VectorNormalize(dvec)

	//is on opposite direction
	float dot = dvec * CurrentWP.facevec
	if(dot < 0)
		return false

	//don't test it too often
	if(currenttime < LastTestGrenadeTime)
		return false

	vector fr = HHVec + wp.floor

	//muzeme hazet zprava/zleva
	if(wp.flags & 256|512)
	{
		float side = GetSide(AttackerLKP, CurrentPos, CurrentWP.facevec)
		
		if(fabs(dvec * CurrentWP.facevec) < 0.987 || !wp.flags & 128)
		{
			if(side < 0)
			{
				if(wp.flags & 256)
				{
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wp.rightvec * -32.0 + fr, AttackerLKP)
					//test LOF
					if(TestGrenadeLOF(wp.rightvec * -32.0 + fr, wp.rightvec * -32.0))
					{
						GrenadeHand = -1
						ThrowGrenade2(1, incrouch)
						return true
					}
				}
			}
			else
			{
				if(wp.flags & 512)
				{
	//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wp.rightvec * 32.0 + fr, AttackerLKP)
					//test LOF
					if(TestGrenadeLOF(wp.rightvec * 32.0 + fr, wp.rightvec * 32.0))
					{
						GrenadeHand = 1
						ThrowGrenade2(2, incrouch)
						return true
					}
				}
			}
		}
	}
	
	if(wp.flags & 128)
	{
//AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, wp.floor + "0 0 48", AttackerLKP)
		//test LOF
		if(TestGrenadeLOF(fr + "0 0 32", ZeroVec))
		{
			//hazime horem
			GrenadeHand = 1
			ThrowGrenade2(0, incrouch)
			return true
		}
	}
	
	return false
}

//-----------------------------------------------------------------
bool LRLeanIdle()
{
	if(!CurrentWP.flags & 12)
	{
		//Shouldn't happen
		return true
	}

	ResetTurn()
	SetAnimState(t_idle)
	ShootOff()

	while(IsState(STATE_LR_ATTACK))
	{
		if(WPTimedOut())
		{
			//don't leave waypoint when out of ammo
			if(NeedReload(3))
			{
				ReloadGun()
				Wait(CurrentWeapon.Action == 0)
			}
			return true
		}

		if(!IsCurrentWPValid(true))
			return true

		//DistanceVector is set by IsCurrentWPValid call
		SetOrigin(TempWaypoint, DistanceVector * 64.0 + CurrentPos)
		TurnByAnim(TempWaypoint, 0)

		if(!IsState(STATE_LR_ATTACK))
			return true

		if(!CanUseForLean(CurrentWP, 0))
			return true

		if(bCanThrowGrenade && ThrowGrenade(CurrentWP, 128|256|512, true))
		{
			goto done
		}

		//we can reload now. Try it
		if(CurrentWeapon.NumActions >= 2 && NeedReload(1))
		{
			Wait(CurrentWeapon.CanAct(2))

			if(!IsState(STATE_LR_ATTACK))
				return true

			//may changed meanwhile
			if(NeedReload(1))
			{
				ReloadGun()
			}
		}

		local int anm
		//which way we can lean?
		int lf = CurrentWP.flags & 12

		if(lf == 12)
		{
			//check how old is LKP and decide accordingly
			float lkpt = currenttime - AttackerLKPTime
			if(lkpt > 970)
			{
				//it's a long time we saw attacker
				if(rand(0, 100) > 50)
					anm = a_combat_llean
				else
					anm = a_combat_rlean
			}
			else
			{
				//find out which way we have to lean
				if(GetSide(AttackerLKP, CurrentPos, CurrentWP.facevec) < 0)
				{
					anm = a_combat_llean
				}
				else
				{
					anm = a_combat_rlean
				}
			}
		}
		else
		{
			if(lf & 4) //left
				anm = a_combat_llean
			else
				anm = a_combat_rlean
		}

		PlayAnimWait(s_custom, anm, 10.0, g_all, 30, AF_ONCE, true)
		PlayAnim(s_custom, anm + 1, 0.0, g_all, 30, 0)
		ShootOn(0)

		for(local float wn = 0; wn < fLeanShootTime; wn += AI_SLOWREFRESH)
		{
			if(!IsCurrentWPValid(true))
				break

			AISleep(AI_SLOWREFRESH)

			if(NeedReload(3))
				wn = fLeanShootTime
		}

		ShootOff()
		PlayAnimWait(s_custom, anm + 2, 10.0, g_all, 30, AF_ONCE|AF_BLENDOUT, true)

		if(wn != fLeanShootTime)
			return true

done:;
		//we can reload now. Try it
		if(CurrentWeapon.NumActions >= 2 && NeedReload(1))
		{
			Wait(CurrentWeapon.CanAct(2))

			if(!IsState(STATE_LR_ATTACK))
				return true

			//may changed meanwhile
			if(NeedReload(1))
			{
				ReloadGun()
			}
		}

		for(wn = 0; wn < 200; wn += AI_SLOWREFRESH)
		{
			if(!IsCurrentWPValid(true))
				return true

			AISleep(AI_SLOWREFRESH)
		}
	}
	return false
}

//-----------------------------------------------------------------
bool IsCrouchWPValid()
{
	float adist = AttackerDistance()
	if(CurrentWP == NULL)
	{
		//only criteria is distance and TODO: trace
		if(adist < AttackDistMin)
			return false
	}
	else
	{
		if(!IsCurrentWPValid(true))
			return false
	}

	return true
}

//-----------------------------------------------------------------
bool LRCrouchIdle()
{
	ResetTurn()
	SetAnimState(t_idle)
	ShootOff()

	int res
	local bool down = true
	local float wn
	//do crouch
	PlayAnimWait(s_custom, a_crouch + 1, 10.0, g_all, 30, AF_ONCE, true)

	//crouch-idle
	PlayAnim(s_custom, a_crouch + 0, 10.0, g_all, 30, 0)

	local float maxwait
	
	while(IsState(STATE_LR_ATTACK))
	{
		if(WPTimedOut())
		{
			//don't leave waypoint when out of ammo
			if(NeedReload(3))
			{
				ReloadGun()
				Wait(CurrentWeapon.Action == 0)
			}

			res = true
			goto getout
		}
		
		if(bCanFireInCrouch)
			maxwait = frand(90, 105)
		else
			maxwait = frand(180, 215)

		for(wn = 0; wn < maxwait; wn += AI_SLOWREFRESH)
		{
			if(!IsState(STATE_LR_ATTACK))
			{
				res = false
				goto getout
			}

			if(!IsCrouchWPValid())
			{
				res = true
				goto getout
			}
			
			AISleep(AI_SLOWREFRESH)
		}

		TurnToTarget(AttackerLKP, 60)
		
		if(CurrentWP && !CanUseForCrouch(CurrentWP, 0))
		{
			res = true
			goto getout
		}

		//we can reload now. Try it
		if(CurrentWeapon.NumActions >= 2 && NeedReload(1))
		{
			Wait(CurrentWeapon.CanAct(2))

			if(!IsState(STATE_LR_ATTACK))
			{
				res = false
				goto getout
			}

			//may changed meanwhile
			if(NeedReload(1))
			{
				ReloadGun()
			}
		}

		if(bCanThrowGrenadeInCrouch && ThrowGrenade(CurrentWP, 128|256|512, true))
		{
			goto done
		}

		if(!bCanFireInCrouch || frand(0, 100) > 95)
		{
			//stand up
			down = false

			PlayAnimWait(s_custom, a_crouch + 2, 10.0, g_all, 30, AF_ONCE|AF_BLENDOUT, true)

			ShootOn(0)

			for(wn = 0; wn < fCrouchShootTime; wn += AI_SLOWREFRESH)
			{
				if(!IsState(STATE_LR_ATTACK))
				{
					res = false
					goto getout
				}

				if(!IsCrouchWPValid())
				{
					res = true
					goto getout
				}

				AISleep(AI_SLOWREFRESH)
	
				if(NeedReload(2))
					break
			}

			//crouch down
			down = true
			PlayAnimWait(s_custom, a_crouch + 1, 10.0, g_all, 30, AF_ONCE, true)
		}
		else
		{
			//ShootOn is in animhook
			//ShootOn(0)
			PlayAnimWait(s_custom, a_crouch_fire + rand(0, 2), 10.0, g_all, 30, AF_ONCE|AF_BLENDOUT, true)
		}

		ShootOff()
done:;
		PlayAnim(s_custom, a_crouch + 0, 10.0, g_all, 30, 0)

		//we can reload now. Try it
		if(CurrentWeapon.NumActions >= 2 && NeedReload(1))
		{
			Wait(CurrentWeapon.CanAct(2))

			if(!IsState(STATE_LR_ATTACK))
			{
				res = false
				goto getout
			}

			//may changed meanwhile
			if(NeedReload(1))
			{
				ReloadGun()
			}
		}
	}
	res = false

getout:;

	if(down)
		PlayAnimWait(s_custom, a_crouch + 2, 10.0, g_all, 30, AF_ONCE|AF_BLENDOUT, true)

	//pro jistotu
	SetAnimSlot(this, s_custom, VNULL, 10.0, 10.0, NULL, 0, 0)

	return res
}

//-----------------------------------------------------------------
void PlayGesture(int anm, int group)
{
	//should *NOT* happen!
	if(!ASet.Anims[anm])
		return

	PlayAnim(s_idlevar, anm, 20.0, group, 30, AF_ONCE|AF_BLENDOUT|AF_NOANIMEND)
}

//-----------------------------------------------------------------
void DoGesture(AICreatureClass other, ExtClass target)
{
	//in which direction we have to confirm command?
	if(ASet.Anims[a_act_look_left])
	{
		if(GetSide(other.CurrentPos, CurrentPos, GetLookVector()) < 0)
			PlayGesture(a_act_look_left, g_confirmgesture)
		else
			PlayGesture(a_act_look_right, g_confirmgesture)
	}

	//which way is other actor pointing?
	const int anims[4] = {a_act_forward_go, a_act_right_go, a_act_back_go, a_act_left_go}	
	other.PlayGesture(anims[other.TargetQuadrant(other.faceyaw, this)], g_gesture)
}

//-----------------------------------------------------------------
bool LRAttack()
{
	SetAnimState(t_idle)

	OnWPTime = 0
	WPIsNotValid = false
	local float shoottime = rand(MinAttacksInRow[0], MaxAttacksInRow[0]) * frand(MinAttackDelay[0], MaxAttackDelay[0])
	Attacker.AddAttacker(this)
	while(true)
	{
		if(!IsState(STATE_LR_ATTACK))
			return false

		AttackStatus = 1

		float dist = AttackerDistance()

		if(Tactics == TACTICS_COVERCOMBAT && dist < 376 && CurrentWP == NULL)
		{
			//make them coward little bit
			goto nextwp
		}
		
		if(WPIsNotValid)
			goto nextwp

		if(CurrentWP && !IsCurrentWPValid(false))
			goto nextwp

		if(WPTimedOut())
			goto nextwp

		//We're too near to Attacker
		if(dist < AttackDistMin)
		{
			if(bShortRange && StateAge(STATE_CHASE) > 2) //prepni na shortrange
			{
				CPosition warea = GetWPos()
				if(IsConnected(warea, Attacker.GetWPos()))
				{
					if(TryFindPath(Attacker) == PATH_OK)
					{
						BeginPath(Attacker, true, true)
						SwitchFSM(STATE_CHASE)
						return false
					}
				}
			}
nextwp:;
	//FIXME: HUH??? We used LKP as 'from' parameter? WHY?
//			info_waypoint wp = FindWaypoint(CurrentWP, AttackerLKP, AttackDistMin, 512, 2, Attacker)
			info_waypoint wp = FindWaypoint(CurrentWP, CurrentPos, AttackDistMin, fMaxRange, 2, Attacker)

			if(wp != NULL)
			{
				if(!IsLastMan())
					Say(ST_CHANGINGWP)

				//try to embroider it by some gestures
				if(Squad && bCanConfirmGesture)
				{
					CreatureClass other = Squad.FindCommander(this, wp)
					
					if(other)
					{
						DoGesture(other, wp)
					}
				}

				//it's the time for another shooting point
				LRNextSP(wp)
				SetAnimState(t_idle)

				if(!IsState(STATE_LR_ATTACK))
					return false
			}
			else
			{
				AISleep(frand(MinAttackDelay[iAttackType], MaxAttackDelay[iAttackType])) //at least time for shoot repeat
			}
			
			WPIsNotValid = false
			OnWPTime = 0
		}

		if(ShowAI)
			SetWidgetText(WAI, 3, "dist:" + ftoa(dist) + " OnWPTime:" + ftoa(OnWPTime))

		//turn to attacker if we need that
		TurnByAnim(Attacker, 50)
		if(!IsState(STATE_LR_ATTACK))
			return false

		AISleep(AI_REFRESH)
		if(!IsState(STATE_LR_ATTACK))
			return false

		shoottime -= AI_REFRESH

		if(shoottime > 0)
			continue

		shoottime = rand(MinAttacksInRow[0], MaxAttacksInRow[0]) * frand(MinAttackDelay[0], MaxAttackDelay[0])

		//can be used for leaning?
		if(CanUseForLean(CurrentWP, 0.5))
		{
			if(!LRLeanIdle())
				return false

			ShootOn(0, false)

			WPIsNotValid = true //get out of here!
		}
		else
		{
			//AICreatureClass cc = TestShoot(Attacker, Attacker.GetCenterOfBody())

			//can we crouch here?
			//if(cc == Attacker && CanUseForCrouch(CurrentWP, 0.5))
			if(CanUseForCrouch(CurrentWP, 0.5) && TestShoot(Attacker, Attacker.GetCenterOfBody()) == Attacker)
			{
				if(!LRCrouchIdle())
					return false
	
				ShootOn(0, false)
				WPIsNotValid = true //get out of here!
			}
			else
			{
				//hacked in as Boss is too lethal!
				if(Tactics == TACTICS_BERSERK)
					RestTime = frand(1.0, 2.0)
				else
					RestTime = frand(0.1, 0.2)

				//we can reload now. Try it
				if(CurrentWeapon.NumActions >= 2 && NeedReload(1))
				{
					local bool sh = bShooting

					bShooting = false
	
					Wait(CurrentWeapon.CanAct(2))
	
					if(!IsState(STATE_LR_ATTACK))
						return false
	
					//may changed meanwhile
					if(NeedReload(1))
					{
						ReloadGun()
					}
					bShooting = sh
				}
	
				AIIdle(Attacker, it_fight, AttackDistMin)

				if(bCanThrowGrenade)
					ThrowGrenade(CurrentWP, 128|256|512, false)
			}
		}

		if(!IsState(STATE_LR_ATTACK))
			return false

		if(IsKilled(STATE_RETURN))
			return false

		AISleep(AI_REFRESH) //avoid deadlock
	}

	return true
}

//-----------------------------------------------------------------
void AILongRangeAttack()
{
	SetState(s_combat)
	AttackStatus = 1

	ShootOn(0, true)
	AISleep(AI_REFRESH)

	LRAttack()

	ShootOff()
}

//-----------------------------------------------------------------
void AICloseCombat()
{
	ShootOff()
	
	while(IsState(STATE_CLOSECOMBAT))
	{
		TurnByAnim(Attacker, 5)
		PlayAnimWait(s_weapon, a_combat_closecombat + rand(0, NumCloseCombatAnims), 10.0, g_all, 50, AF_BLENDOUT|AF_ONCE, true)
		
		if(DistanceBetween(Attacker) > DISTANCE_TO_CLOSECOMBAT - 16)
			break

		AISleep(AI_REFRESH)
	}

	if(IsState(STATE_CLOSECOMBAT))
		SwitchFSM(STATE_LR_ATTACK)
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
			return
		}

		local float enemydist = AttackerDistance()

		if(!stand)
		{
			int res
			
			if(enemydist < 190)
				res = UpdatePath(t_run, 5.0, false)
			else
				res = UpdatePath(t_run, 6.5, false)
	
			if(res != PATH_OK && res != PATH_FINISHED)
			{
				//Print("beginlosttarget")
				ShootOff()
				BeginLostTarget(Attacker)
				return
			}
			
			CPosition area

			area = GetWPos()

			if(!IsConnected(Attacker.GetWPos(), area))
			{
				ShootOff()
				BeginLostTarget(Attacker)
				return
			}
		}

		enemydist = AttackerDistance()

		if(enemydist > AttackDist)
		{
			ShootOff()
			SwitchFSM(STATE_GUARD)
			return
		}

		if(bLongRange && enemydist > AttackDistMin) //prepni na longrange
		{
			ShootOff()
			SwitchFSM(STATE_LR_ATTACK)
			return
		}

		if(stand)
		{
			//some hysteresis, to avoid oscilation
			if(enemydist > AttackDistClose + 22 || !CheckDirectPath(Attacker))
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
}

//-----------------------------------------------------------------
void AIFirstSight()
{
	if(bHasSpotAnim)
	{
		if(Distance(Attacker) > 64)
			PlayAnimWait(s_custom, a_spot_idle_var + 2, 10.0, g_all, 45, AF_ONCE|AF_BLENDOUT)
	}

	SetState(s_combat)
	SetAnimState(t_idle)
	TurnByAnim(Attacker, 30)

	if(bLongRange)
		SwitchFSM(STATE_LR_ATTACK)
	else
		SwitchFSM(STATE_CHASE)
}

//-----------------------------------------------------------------
void AIReturn()
{
	SetState(s_relaxed)
	
	if(OriginWaypoint.GetWPos() == NULL || Distance(OriginWaypoint) < 32)
	{
		SetAnimState(t_idle)
		SwitchFSM(STATE_GUARD)
		return
	}
	
	SetAnimState(t_walk)
	
	if(BeginPath(OriginWaypoint, false, true) != PATH_OK)
	{
		SetAnimState(t_idle)
		SwitchFSM(STATE_GUARD)
		return
	}

	while(IsState(STATE_RETURN))
	{
		int res = UpdatePath(t_walk)

		if(!IsState(STATE_RETURN))
			return

		if(res == PATH_FINISHED)
		{
			SetAnimState(t_idle)
			SwitchFSM(STATE_GUARD)
			return
		}

		if(res != PATH_OK)
		{
			SetAnimState(t_idle)
			SwitchFSM(STATE_GUARD)
			return
		}

		AISleep(AI_REFRESH)
	}
}

//-----------------------------------------------------------------
void AIObserve()
{
	local int wtime = 0

	SetState(s_alert)
	//nebudem popobihat kvuli malym vzdalenostem
	if(Distance(TempWaypoint) < 96)
	{
		//muzeme se ohlednout
		if(ASet.Anims[a_spot_idle_var])
		{
			float dot = DistanceVector * facevec
			
			if(dot < 0.5 && dot > -0.5)
			{
				int side = 1
				if(GetSide(GetOrigin(TempWaypoint), CurrentPos, facevec) < 0)
					side = 0

				PlayAnimWait(s_custom, a_spot_idle_var + side, 20.0, g_all, 15, AF_ONCE|AF_BLENDOUT)

				if(IsState(STATE_OBSERVE))
				{
					SetState(s_relaxed)
					SetAnimState(t_idle)
					SwitchFSM(STATE_RETURN)
				}
				return
			}
		}

		//musime se otocit
		TurnByAnim(TempWaypoint, 15)

		AISleep(AI_REFRESH)
		RestTime = frand(4, 6)
		AIIdle(TempWaypoint, it_default)
		if(IsState(STATE_OBSERVE))
		{
			SetState(s_relaxed)
			SetAnimState(t_idle)
			SwitchFSM(STATE_RETURN)
		}
		return
	}

	TurnByAnim(TempWaypoint, 5)

	if(!IsState(STATE_OBSERVE))
		return

	if(BeginPath(TempWaypoint, false, false, 2.0) != PATH_OK)
	{
		//nemuzeme tam dojit, tak se aspon podivej
		RestTime = frand(4.0, 6.0)
		AIIdle(TempWaypoint, it_default)
		if(IsState(STATE_OBSERVE))
		{
			SetState(s_relaxed)
			SetAnimState(t_idle)
			SwitchFSM(STATE_RETURN)
		}
		return
	}

	SetAnimState(t_walk)

	while(IsState(STATE_OBSERVE))
	{
		int res = UpdatePath(t_walk)

		if(!IsState(STATE_OBSERVE))
			return

		//jsme tam
		if(res == PATH_FINISHED)
		{
			SetAnimState(t_idle)

			RestTime = frand(4.0, 6.0)
			AIIdle(TempWaypoint, it_default)

			if(!IsState(STATE_OBSERVE))
				return

			SetState(s_relaxed)
			SwitchFSM(STATE_RETURN)
			return
		}

		if(res != PATH_OK)
		{
			SwitchFSM(STATE_GUARD)
			return
		}

		AISleep(AI_REFRESH)
	}
}

//-----------------------------------------------------------------
void AIGuard()
{
	if(GuardTarget == NULL || BeginPath(GuardTarget, false, false) != PATH_OK)
	{
		RestTime = 5.0
		AIIdle(GuardTarget, it_default)
		return
	}

	local int anm = t_walk

	SetAnimState(anm)
//	SetState(s_relaxed)

	while(IsState(STATE_GUARD))
	{
		//pokud nejsme relaxed, tak se po par vterinach automaticky uklidnime
		if(astate != s_relaxed && StateAge(STATE_GUARD) > 4.0)
			SetState(s_relaxed)
	
		int res = UpdatePath(anm)

		if(!IsState(STATE_GUARD))
			return

		if(res == PATH_FINISHED)
		{
			ArriveWP(CurrentWP)
			if(!IsState(STATE_GUARD))
				return

			local ExtClass next = FindClass(GuardTarget.link)

			if(next)
			{
				TurnByAnim(next, 15)

				while(true)
				{
					if(!IsState(STATE_GUARD))
						return

					if(BeginPath(next, false) != PATH_OK)
					{
						RestTime = 5.0
						AIIdle(GuardTarget, it_default)
						continue
					}

					GuardTarget = next
					break
				}
				AISleep(AI_REFRESH)
				continue
			}
			else
			{
				RestTime = -1
				AIIdle(GuardTarget, it_default)
				return
			}
		}
		else
		{
			if(res != PATH_OK)
			{
				RestTime = 5.0
				AIIdle(GuardTarget, it_default)
				return
			}
		}


		AISleep(AI_REFRESH)
	}
}

//-----------------------------------------------------------------
void AILostTarget()
{
	local info_waypoint wp

	SetState(s_combat)

	wp = NULL

	ShootOn(0)
	while(IsState(STATE_LOST_TARGET))
	{
		RestTime = frand(3.0, 6.0)
		AIIdle(LostTarget, it_lost, 0)

		wp = FindWaypoint(wp, GetOrigin(LostTarget), 0, 256, 0, LostTarget)

		if(wp == NULL)
			continue

		SetAnimState(t_run)
		while(IsState(STATE_LOST_TARGET))
		{
			int res = UpdatePath(t_run)
			
			if(res == PATH_FINISHED)
			{
				/*
				wp = FindClass(wp.link)

				if(wp && BeginPath(wp, false) == PATH_OK)
					continue
					*/
				break
			}
			else
			{
				if(res != PATH_OK)
				{
					break
				}
			}

			AISleep(AI_REFRESH)
		}
	}
	ShootOff()
}

//-----------------------------------------------------------------
void AITakeCoverFromDanger()
{
	local int anm = t_run
	SetState(s_combat)
	SetAnimState(anm)

	local info_waypoint wp = FindWaypoint(CurrentWP, CurrentPos, 64, 512, 0, TempWaypoint, true)

	if(wp == NULL)
	{
		SetWP(TempWaypoint, CurrentPos)
		GuardTarget = TempWaypoint
		SwitchFSM(STATE_GUARD)
		return
	}

	while(IsState(STATE_COVERFROMDANGER))
	{
		int res = UpdatePath(anm)

		if(!IsState(STATE_COVERFROMDANGER))
			goto getout

		if(res == PATH_FINISHED)
		{
			if(Attacker && Tactics == TACTICS_COMBAT || Attacker && Tactics == TACTICS_GUARDCOMBAT)
			{
				BeginCombatTarget(Attacker)
			}
			else
			{
				SetWP(TempWaypoint, CurrentPos)
				GuardTarget = TempWaypoint
				SwitchFSM(STATE_GUARD)
			}
			return
		}
		
		if(res == PATH_NOT_FOUND)
		{
			SetWP(TempWaypoint, CurrentPos)
			GuardTarget = TempWaypoint
			SwitchFSM(STATE_GUARD)
		}

		AISleep(AI_REFRESH)
	}
getout:
	SetAnimState(t_idle)
}
//-----------------------------------------------------------------
void AITakeCover()
{
	ShootOn(1, true)

	local info_waypoint wp, prev = NULL

	local int anm = t_run
	SetState(s_combat)
	SetAnimState(anm)

again:
	while(IsState(STATE_COVER))
	{
		prev = wp

		wp = FindWaypoint(prev, CurrentPos, 8, 512, 1, Attacker)

		if(wp == NULL)
		{
			if(prev)
			{
				AISleep(AI_REFRESH)
				goto again
			}

			if(Tactics == TACTICS_COMBAT || Tactics == TACTICS_GUARDCOMBAT)
			{
				BeginCombatTarget(Attacker)
			}
			else
			{
			//Tactics = TACTICS_COVER
			//Tactics = TACTICS_COVERCOMBAT
				SwitchFSM(STATE_GUARD)
			}
			goto getout
		}

		UseWaypoint(wp)

		local int quad = -1

		while(wp)
		{
			int res = UpdatePath(anm)

			if(!IsState(STATE_COVER))
				goto getout

			if(res == PATH_FINISHED)
			{
				if(Tactics == TACTICS_COMBAT || Tactics == TACTICS_GUARDCOMBAT)
				{
//goto again
					BeginCombatTarget(Attacker)
				}
				else
				{
					SetWP(TempWaypoint, CurrentPos)
					GuardTarget = TempWaypoint
					SwitchFSM(STATE_GUARD)
				}
				goto getout
			}

			if(res == PATH_NOT_FOUND)
			{
				goto again
			}

			if(!WaitForDoors)
			{
				if(bCanShootInTakeCover)
				{
					quad = TargetQuadrant(Vector2Yaw(WVector), Attacker, quad)
					anm = AnimRunFire[quad]
				}
				else
				{
					quad = -1
					anm = t_run
				}

				SetAnimState(anm)
			}

			AISleep(AI_REFRESH)

			if(!IsState(STATE_COVER))
				goto getout
		}

		if(!IsState(STATE_COVER))
			goto getout

		AISleep(AI_REFRESH)
	}

getout:
	ShootOff()
	//UnuseWaypoints() //???
}

//-----------------------------------------------------------------
void AIRoaming()
{
	local info_waypoint wp = NULL

	SetState(s_relaxed)
	while(IsState(STATE_ROAMING))
	{
		wp = FindWaypoint(wp, CurrentPos, 32, 384, 0, NULL)

		if(wp == NULL)
		{
			AISleep(AI_REFRESH)
			AIIdle(World.Player, it_default)
			goto next
		}

		UseWaypoint(wp)

		if(rand(0, 100) > 25)
			SetAnimState(t_run)
		else
			SetAnimState(t_walk)

		while(wp)
		{
			if(UpdatePath(t_run) == PATH_FINISHED)
			{
				RestTime = frand(MinRoamIdleTime, MaxRoamIdleTime)
				AIIdle(World.Player, it_default)
				wp.used &= UsedOff
				goto next
			}
			AISleep(AI_REFRESH)

			if(!IsState(STATE_ROAMING))
				goto getout
		}

next:
		if(!IsState(STATE_ROAMING))
			goto getout

		AISleep(AI_REFRESH)
	}

getout:
	if(wp)
		wp.used &= UsedOff
}

//-----------------------------------------------------------------
void AIScript()
{
	while(IsState(STATE_SCRIPT))
	{
		switch(AICmd)
		{
		case AICMD_NONE:
			if(!PopStack()) //periodically check stack for new commands
			{
				if(Tactics == TACTICS_IDLE)
				{
					DPrint(_name + " has ended with script-stack and his Tactics == Idle. Creature will freeze. It's probably a bug in in-game script")
				}

				//HOTFIX: obcas se stane (mozna i chybou ingame skriptu), ze skonci command-stack
				//a potvora zustane vytuhla v STATE_SCRIPT
				if(Tactics != TACTICS_DUMMY)
				{
					SwitchFSM(STATE_GUARD)
					return
				}
			}
			break

		case AICMD_IDLE:
			PopStack()
			break

		case AICMD_WALK:
			if(UpdatePath(t_walk) == PATH_FINISHED)
			{
				PopStack()
			}
			break

		case AICMD_RUN:
			if(UpdatePath(t_run) == PATH_FINISHED)
			{
				PopStack()
			}
			break

		case AICMD_ANIM:
			Wait(AICmd != AICMD_ANIM || status != STATE_SCRIPT)
			break

		case AICMD_WSIGHT:
			Wait(AICmd != AICMD_WSIGHT || status != STATE_SCRIPT)
			break

		case AICMD_TURNBY:
			SetOrigin(TempWaypoint, Yaw2Vector(Yaw(GetAngles(_TurnTarget))) * 64.0 + GetOrigin(_TurnTarget))
			TurnByAnim(TempWaypoint, 0)
			PopStack()
			break

		case AICMD_TURN:
			if(_TurnTarget == CurrentWP)
			{
				DPrint("Using AICmd(Turn) to waypoint which we are on " + _name + " use turnby instead!")
			}
			else
			{
				TurnByAnim(_TurnTarget, 0)
			}
			PopStack()
			break
		}

		AISleep(AI_REFRESH)
	}
}

//-----------------------------------------------------------------
void ChangeAnim()
{
	if(ostate == astate && otype == atype && oextra == aextra)
		return

//Print(String(itoa(ostate) + " " + itoa(otype) + " " + itoa(oextra)))
	MotionAnim = -1

	float b1 = 20.0

	if(atype == t_idle && otype != atype)
	{
		b1 = 40.0
	}

	ostate = astate
	otype = atype
	oextra = aextra

	int a1 = -1, gr1 = g_all, fps1 = 30, f1 = 0

	switch(atype)
	{
	case t_bwalk:
		WAddAngle = 180
		LAddAngle = 0
		break

	case t_brun:
		WAddAngle = 180
		LAddAngle = 0
		break

	case t_lstrafe:
		WAddAngle = -90
		LAddAngle = 0
		break

	case t_rstrafe:
		WAddAngle = 90
		LAddAngle = 0
		break

	case t_run_fb:
		WAddAngle = 0
		LAddAngle = -180
		break

	case t_run_fl:
		WAddAngle = 0
		LAddAngle = 90
		break

	case t_run_fr:
		WAddAngle = 0
		LAddAngle = -90
		break

	default:
		WAddAngle = 0
		LAddAngle = 0
		break
	}

	switch(astate)
	{
	case s_relaxed:
		switch(atype)
		{
		case t_walk:
			a1 = a_relax_walk
			break

		case t_bwalk:
			a1 = a_relax_bwalk
			break

		case t_run:
			a1 = a_relax_run
			break

		case t_run_aim:
			a1 = a_relax_run
			break

		case t_brun:
			a1 = a_relax_brun
			break

		case t_idle:
			a1 = a_relax_idle + rand(0, NumRelaxIdleAnims)
			break

		case t_lstrafe:
			a1 = a_relax_strafe + 0
			break

		case t_rstrafe:
			a1 = a_relax_strafe + 1
			break
		}
		break

	case s_alert:
		switch(atype)
		{
		case t_walk:
			a1 = a_alert_walk
			break

		case t_bwalk:
			a1 = a_alert_bwalk
			break

		case t_run:
			a1 = a_alert_run
			break

		case t_run_aim:
			a1 = a_alert_run
			break

		case t_brun:
			a1 = a_relax_brun
			break

		case t_idle:
			a1 = a_alert_idle + rand(0, NumAlertIdleAnims)
			break

		case t_lstrafe:
			a1 = a_alert_strafe + 0
			break

		case t_rstrafe:
			a1 = a_alert_strafe + 1
			break
		}
		break

	case s_combat:
		switch(atype)
		{
		case t_walk:
			a1 = a_combat_walk
			break

		case t_bwalk:
			a1 = a_alert_bwalk
			break

		case t_run:
			a1 = a_combat_run
			break

		case t_run_aim:
			a1 = a_combat_run_aim
			break

		case t_brun:
			a1 = a_combat_brun
			break

		case t_idle_fire:
			a1 = a_combat_idle_fire
			break

		case t_idle:
			a1 = a_combat_idle + rand(0, NumCombatIdleAnims)
			break

		case t_lstrafe:
			a1 = a_combat_strafe + 0
			break

		case t_rstrafe:
			a1 = a_combat_strafe + 1
			break

		case t_run_fb:
			a1 = a_combat_run_fire + 0
			if(!ASet.Anims[a1])
			{
				a1 = a_combat_run
				LAddAngle = 0
			}
			break

		case t_run_fl:
			a1 = a_combat_run_fire + 1
			if(!ASet.Anims[a1])
			{
				a1 = a_combat_run
				LAddAngle = 0
			}
			break

		case t_run_fr:
			a1 = a_combat_run_fire + 2
			if(!ASet.Anims[a1])
			{
				a1 = a_combat_run
				LAddAngle = 0
			}
			break
		}
	}

	if(a1 >= 0)
	{
		MotionAnim = a1
		PlayAnim(s_walk, a1, b1, gr1, fps1, f1)
	}
	else
	{
		SetAnimSlot(this, s_walk, VNULL, b1, 0, NULL, 0, 0)
	}
}

//-----------------------------------------------------------------
void AIIdleState()
{
	RestTime = -1
	AIIdle(Attacker)
}
/*
	void LookThread()
	{
		while(true)
		{
			LookAt(World.Player)
			AISleep(1)
		}
	}
*/
//-----------------------------------------------------------------
	void FSMThread()
	{

//		SetState(s_combat)
//		SetAnimState(t_run)
//		SetAnimState(t_run_fr)
/*
		WVector = facevec

CurrentWP = FindClass("wpg1")
Attacker = World.Player

		while(true)
		{
AttackerLKP = GetOrigin(World.Player) - "0 0 16"
			CurrentStateCount = StateCount
			TurnByAnim(World.Player, 5)
			AISleep(100)

		if(bCanThrowGrenade && ThrowGrenade(CurrentWP, 128|256|512, false))
		{
			Attacker = World.Player
			
		}
*/		
/*
		while(true)
		{
//			Attacker = World.Player
//			OneAttack(1, 0)

			LookAt(World.Player)
			AISleep(1)
		}
		return //haha
*/
/*
		if(_name == "test2")
			PlayAnim(s_custom, a_crouch, 4, g_all, 30, 0)
			
		while(true)
		{
			if(_name == "test1")
			{
				DoGesture(FindClass("test2"), World.Player)
				Sleep(200)
			}
			
//			TurnByAnim(World.Player, 0)
//			AISleep(100)

//			Attacker = World.Player
//			OneAttack(1, 0)

			LookAt(World.Player)
			AISleep(1)
		}
*/
/*
		SwitchFSM(STATE_GUARD)
		CurrentStateCount = StateCount
		SetState(s_combat)

		thread LookThread()

		while(true)
		{
			TurnByAnim(World.Player, 0)
			AISleep(300)
		}
*/
		//AI state machine core
		while(true)
		{
			AttackStatus = 0
/*
			local int prevstatus
			int diff = StateCount - CurrentStateCount
			if(diff > 1)
			{
				Print(String("FSM Error: one or more states lost on entity " + _name + ". New state is " + AIStatusNames[status] + ". Old state is " + AIStatusNames[prevstatus]))
			}
			prevstatus = status
*/
			CurrentStateCount = StateCount

			if(ShowAI)
				SetWidgetText(WAI, 4, "")

			switch(status)
			{
			case STATE_ROAMING:
				AIRoaming()
				break

			case STATE_COVERFROMDANGER:
				AITakeCoverFromDanger()
				break

			case STATE_COVER:
				AITakeCover()
				break
/*
			//utocime
			case STATE_ATTACK:
				AICloseAttack()
				break
*/
			//close-hit
			case STATE_CLOSECOMBAT:
				AICloseCombat()
				break

			//honime nepritele
			case STATE_CHASE:
				AIChase()
				break

			case STATE_LR_ATTACK:
				AILongRangeAttack()
				break

			case STATE_1STSIGHT:
				AIFirstSight()
				break

			case STATE_LOST_TARGET:
				AILostTarget()
				break

			case STATE_RETURN:
				AIReturn()
				break

			case STATE_SCRIPT:
				AIScript()
				break

			case STATE_OBSERVE:
				AIObserve()
				break

			case STATE_GUARD:
				AIGuard()
				break

			case STATE_IDLE:
				AIIdleState()
				break

			default:
				AISleep(AI_SLOWREFRESH)
				break
			}
		}
	}

int Loged
//-----------------------------------------------------------------
void Login()
{
	Loged = true
	KillThread(this, "FSMThread")
	SetAnimState(t_idle)
}

//-----------------------------------------------------------------
void Logoff()
{
	Loged = false
	thread FSMThread()
}

//-----------------------------------------------------------------
void CreateBody()
{
}

//-----------------------------------------------------------------
void Awake()
{
#ifdef EDITOR
	if(World.EditorActive == true)
		return
#endif

	//shouldn't happen. But it does, ocassionaly :-(
	if(CurrentWeapon == NULL || status >= STATE_DIE)
		return

	int prev = status, tprev = Tactics
	CreatureClass tgprev = Target

	flags &= 0xffffffff - FL_SUMMON_TRIGGER
	flags |= FL_SEE|FL_HEAR|FL_FEEL

	CallMethods(this, OnActivate)

	SetEventMask(this, EV_TIMER | EV_VISIBLE | EV_INIT | EV_TOUCH | EV_FRAME | EV_ANIMEND | EV_SOUNDEND | EV_FLASHLIGHT)

	if(flags & FL_PUPPET)
		return

	if(WalkWaypoint == NULL)
	{
		WalkWaypoint = new info_waypoint
		WalkWaypoint._name = "@TempWalkWaypoint_" + _name
		TempWaypoint = new info_waypoint
		TempWaypoint._name = "@TempWaypoint_" + _name
		OriginWaypoint = new info_waypoint
		OriginWaypoint._name = "@OriginWaypoint_" + _name
		
		SetWP(OriginWaypoint, CurrentPos)
		SetWP(TempWaypoint, CurrentPos)
		SetWP(WalkWaypoint, CurrentPos)
	}

	CreateBody()
	dBodySetInertiaTensor(this, ZeroVec, ZeroVec)
	dBodyActive(this, true, true)
	SetFlags(this, TFL_SOLID)
	dBodyInteractionLayer(this, ET_NPC)
	dBodyCreatePhysXShadow(this, 1, true)

	ClearFlags(this, TFL_NOSHADOW)
	ClearFlags(CurrentWeapon, TFL_NOSHADOW)

	//scripted
	if(CmdStackPos || prev != status || tprev != Tactics || Target != tgprev)
	{
		if(status == 0)
		{
			//Shouldn't never happen. But it does occassionaly

			//if we cannot combat player, let's guard
			SetWP(TempWaypoint, CurrentPos)
			GuardTarget = TempWaypoint
			SwitchFSM(STATE_GUARD)
		}

		thread FSMThread()
		return
	}

	Attacker = World.Player
	SetAttackerLKP(Attacker.CurrentPos)

	//thread Hack()

	if(flags & FL_PUPPET)
	{
		Tactics = TACTICS_IDLE
		SwitchFSM(STATE_SCRIPT)
		M_DisableSenses(this)
		thread FSMThread()
		return
	}

	if(flags & FL_ROAM)
	{
		Tactics = TACTICS_ROAM
		BeginRoam(FindClass(link))
		thread FSMThread()
		return
	}

//TODO: get rid of this mess
	if(flags & FL_GUARD)
	{
		GuardTarget = FindClass(link)

		if(GuardTarget == NULL)
			GuardTarget = OriginWaypoint

		Tactics = TACTICS_GUARDCOMBAT
		SwitchFSM(STATE_GUARD)
	}
	else
	{
		if(flags & FL_COMBAT)
		{
			//nasty cheating :-(
			AIAddEvent(AIE_SAWENEMY, GetOrigin(World.Player), World.Player) //neccessary?
			Tactics = TACTICS_COMBAT

			if(DoTactics(World.Player) == 0 || status == 0) //state should be set always. But it doesn't occassionaly
			{
				//if we cannot combat player, let's guard
				SetWP(TempWaypoint, CurrentPos)
				GuardTarget = TempWaypoint
				SwitchFSM(STATE_GUARD)
			}
		}
		else
		{
			Tactics = TACTICS_COMBAT
			SwitchFSM(STATE_GUARD)
		}
	}

	thread FSMThread()
}

//-----------------------------------------------------------------
void InitCreature()
{
	if(!flags & FL_PUPPET)
	{
		if(GetWPos() == NULL)
		{
			DPrint("InitCreature: " + _name + " is out of area. AI stopped!")
			status = STATE_DEAD		
			ErrorNum++
			return
		}
	
		if(World.pPathFinder.TestPointCollision(ShapeIndex, WPosition))
		{
			DPrint("InitCreature: " + _name + " is obstructed. AI stopped!")
			status = STATE_DEAD
			ErrorNum++
			return
		}
	}

	if(flags & FL_SUMMON_TRIGGER)
	{
		if(flags & FL_SUMMON)
			Show(false)

		SetFlags(this, TFL_NOSHADOW)
		SetFlags(CurrentWeapon, TFL_NOSHADOW)
		SetWeight(this, 0)
		return
	}

	Awake()
}

//-----------------------------------------------------------------
	void EOnTimer(MinClass other, int extra)
	{
		if(extra == 100)
		{
			pain = false
			PainTimer = 0
		}
	}

//-----------------------------------------------------------------
	void EOnFlashlight(MinClass other, int extra)
	{
		if(AddVisibleTarget(World.Player, Distance(World.Player)))
			AIAddEvent(AIE_HEARDNOISE, World.Player.CurrentPos, World.Player)
	}

//-----------------------------------------------------------------
	void ReloadAction()	//animhook na moment vlozenia naboja
	{
		if(IsInherited(CurrentWeapon, CFireWeaponClass)
		{
			FireWeaponClass fw = CurrentWeapon
			fw.ReloadAction()
		}
	}

//-----------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		if(status == STATE_SCRIPT && AICmd == AICMD_ANIM)
		{
			AICmd = AICMD_NONE
			return
		}

		switch(extra)
		{
		case s_weapon:
			if(CurrentWeapon)
				Throw(CurrentWeapon, other, EV_ANIMEND, extra + 100)
			return

		case s_walk:
			return

		case s_death:
			SwitchFSM(STATE_DEAD)
			CallMethods(this, OnDeath)
			SetFlags(this, TFL_NOSHADOW)
			OnDeath()
			
			return

		case s_pain:
			SetAnimSlot(this, s_pain, VNULL, 20.0, 0, NULL, 0, 0)

			if(PainTimer)
				StopTimer(PainTimer)

			PainTimer = SetTimer(this, this, frand(MinStunTime, MaxStunTime), EV_TIMER, 100, true)
			return
		}

		//proste ji vyhod
		if(extra > s_weapon)
		{
			if(extra >= s_cutscene && extra <= 11)
				return

			SetAnimSlot(this, extra, VNULL, 20.0, 0, NULL, 0, 0)
		}
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		if(ShowAI)
		{
			ShowWidget(WAI, true)
			vector pp = ProjectVector(this, ZeroVec)
			float px, py
			px = pp[0]
			py = pp[1]
			SetWidgetPos(WAI, px, py, 512, 128)

			if(Stopped)
			{
				vector cmin, cmax
				GetCBoundBox(this, cmin, cmax)

			//AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_ONCE|SS_TRANSP, cmin, cmax)

				if(IntersectRayBox(World.CameraPos, World.CameraNorm * 1024.0 + World.CameraPos, cmin, cmax) == -1)
				{
					ShowWidget(WAI, false)
				}
				else
				{
					int line = 2
					for(int n = CmdStackPos - 1; n >= 0; n--)
					{
						SetWidgetText(WAI, line++, AICmdNames[CmdStack[n]] + "(" + CmdParmStack[n] + ")")
					}
				}
			}
		}

		if(status >= STATE_DIE)
			return 0

		UpdateLookAt()

		viewframe = extra

		return 0
	}

//-----------------------------------------------------------------
	float CorrectYaw(float a, float speed)
	{
		SetYaw(CorrectAngle(faceyaw, a, speed))
		return fabs(faceyaw - a)
	}


//-----------------------------------------------------------------
	bool DoTraceHit()
	{
		vector vec = Attacker.CurrentPos - CurrentPos
		vec[2] = 0
		VectorNormalize(vec)

		float range = fCloseHitRange + Radius
		vector from = CurrentPos - HHVec + "0 0 32"
		vector to = vec * range + from
		float plane[4]
		int sparm
		ExtClass other
	
		int pf = ClearFlags(this, TFL_SOLID)
		float d = TraceLineEx(DEFAULT, from, to, ZeroVec, ZeroVec, other, plane, NULL, sparm, TRACE_DEFAULT|TRACE_BONES, NULL)		
 		SetFlags(this, pf)

		if(d < 1)
		{
			if(other.ClassType == ClassTypeCreature)
			{
				ShotStorage parms

				parms = new ShotStorage
				parms.from = from
				parms.VisualsFrom = CurrentPos
				parms.to = to
				parms.dir = vec
				parms.shooter = this
				parms.HurtType = HT_SHOT
				parms.NumBullets = 1
				parms.MarkSize = 35
				parms.RicDecalType = DE_BULLET
				parms.RicSndType = SE_BULLET
				parms.RicParticlesType = PE_BULLET
				parms.ID = World.ShootID++
				parms.CameraHitFactor = 5
				parms.Bone = g_iTraceBone
				parms.power = 45
				parms.damage = WDmgCloseHit

				if(parms.Shoot(other) == SR_HIT)
				{
					parms.power = -1 //HACKHACK!
					ShotEffects(parms, other, to, Vector(plane[0], plane[1], plane[2]), sparm)
					delete parms
					return true
				}
				delete parms
			}
 		}
 		
 		return false
 	}
 
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		TickTime += ftime

		//Not so often updated:
		if(TickTime > TICK_PERIOD)
		{
			float count = floor(TickTime / TICK_PERIOD)

			TickTime -= TICK_PERIOD * count

			//process unreachables
			for(int n = 0; n < NumUnreachable; n++)
			{
				if(currenttime >= UnreachableTime[n])
				{
					NumUnreachable--
					Unreachable[n] = Unreachable[NumUnreachable]
					UnreachableTime[n] = UnreachableTime[NumUnreachable]
					n--
				}
			}

			if(ragdollActive && RagdollTimeout)
			{
				RagdollTimeout -= TICK_PERIOD
	
				if(RagdollTimeout <= 0)
				{
					RagdollTimeout = 0
					dBodyActive(this, false, false)
					//dBodyInteractionLayer(this, ET_RAGDOLL)
				}
			}
	
			if(status < STATE_DIE)
			{
				if(!flags & FL_PUPPET)
				{
					RestTime -= TICK_PERIOD
					clamp RestTime<0, RestTime>
			
					//when volatile action, lock AI processing
					if(!AILocked)
					{
						AIVisibility()
						AIProcess()
					}			
				}
				
				if(iTraceHit)
				{
					if(DoTraceHit())
						iTraceHit = 0
			 	}
			}
		}

		OnWPTime += ftime
		
		UpdateBleedSources()
                UpdateSSources()
                
		if(ShowAI)
		{
			ShowWidget(WAI, true)

			string str

			if(bShooting)
				str = "Shoot:On  "
			else
				str = "Shoot:Off "

			if(pain)
				str += "Pain "
			else
				str += "     "

			if(flags & FL_SEE)
				str += "S "
			else
				str += "- "

			if(flags & FL_HEAR)
				str += "H "
			else
				str += "- "

			if(flags & FL_FEEL)
				str += "F "
			else
				str += "- "

			str += AIStatusNames[status]
			str += "(" + itoa(StateCount) + ")."

			int srcline
			string thr = ThreadFunction(this, "FSMThread", 1, srcline)
			if(!thr)
			{
				str += "No thread"
			}
			else
			{
				str += thr
				str += ":" + itoa(srcline)
			}

			if(_state)
				SetWidgetText(WAI, 0, _name + " : " + _state)
			else
				SetWidgetText(WAI, 0, _name + " : No state")
				
			SetWidgetText(WAI, 1, str)
			SetWidgetText(WAI, 2, AITacticsNames[Tactics])
/*
			if(status == STATE_SCRIPT)
			{
				if(AICmd >= 0)
					SetWidgetText(WAI, 2, itoa(CmdStackPos) + ":" + AICmdNames[AICmd] + "(" + AIParm + ")")
			}
			else
				SetWidgetText(WAI, 2, "")
*/

			if(WPath)
				str = "T:" + itoa(WTurn) + " AD:" + itoa(WAddAngle) + " Dist:" + ftoa(WTargetDist) + " Wp:" + itoa(WNum) + "/" + itoa(WPath.Size())
			else
				str = "T:" + itoa(WTurn) + " AD:" + itoa(WAddAngle)

			SetWidgetText(WAI, 7, str)
			SetWidgetText(WAI, 8, "NoShootReason: " + NoShootReason)

			if(WTarget && status < STATE_DIE)
			{
				AddDShape(SHAPE_LINE, 0xffff00ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_ONCE|SS_TRANSP, CurrentPos, GetOrigin(WTarget))
				AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_ONCE|SS_TRANSP, CurrentPos, WPos)
				AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_ONCE|SS_TRANSP, WPos - "4 4 4", WPos + "4 4 4")
			}

			str = "Attacker:"

			if(Attacker)
			{
				str += Attacker._name
				if(flags & FL_HURTENEMIES)
					str += " Under chaos"
			}

			SetWidgetText(WAI, 6, str)
		}

		//only animate & quit
		if(status >= STATE_DIE)
		{
			if(status < STATE_DEAD)
				BumpFrame(this, ftime, 0)

			return
		}

		if(Stopped)
		{
			if(viewframe != g_iViewFrame)
			{
				if(ShowAI && WAI)
					ShowWidget(WAI, false)
			}
			SetVelocity(this, ZeroVec)
			return
		}

//update current values
		BulletRainCounter -= ftime * 0.2
		clamp BulletRainCounter<0, BulletRainCounter>

		vector ang = GetAngles(this)
		faceyaw = ang[YAW]
		facevec = Yaw2Vector(faceyaw)

		ProcessShooting()
		
		//Print(String(_name + ":" + NoShootReason))

		if(Loged)
		{
			if(KEY(MOVE_F, false))
				SetAnimState(t_walk)
			else
			{
				if(KEY(MOVE_B, false))
					SetAnimState(t_bwalk)
				else
					SetAnimState(t_idle)
			}

			ang[1] = ang[1] - (g_fMouseSpeedH * (float)MouseDeltaX * 3)
			SetAngles(this, ang)
		}

//fall&drown damage
		if(GetFlags(this) & TFL_ONGROUND)
		{
			if(notonground > 1)
			{
				float zdist = LastZ - CurrentPos[2]

				if(zdist > 100)
				{
					Kill()
					return
				}
			}

			notonground = 0
			LastZ = CurrentPos[2]
		}
		else
		{
			notonground++
		}
/*
		if(!flags & FL_PUPPET)
		{
		//optimize: don't animate idle when out of sight
			int diff = g_iViewFrame - viewframe
			if(diff > 16)
			{
				if(ShowAI && WAI)
					ShowWidget(WAI, false)
	
				if(atype == t_idle && CurrentWeapon && CurrentWeapon.Action == 0 && WaitAnims == 0 && !Alerted())
					goto noanim
			}
	
			if(!pain && flags & FL_FROZEN)
				goto noanim
		}
*/
		BumpFrame(this, ftime, 0)

	noanim:;

		MVec = ZeroVec
		MVecLen = 0
		
		//MOTION
		if(MotionAnim != -1 && iNoMotion != 2)
		{
			if(iNoMotion != 1)
			{
				vector pvel = GetVelocity(this)
	
				if(MotionAnim >= 0)
					MVec = ASet.Motion[MotionAnim]
	
				if(MVec != ZeroVec)
				{
					MVec = this.VectorToParent(MVec)
				}
				else
				{
					float ft = 1.2 / ftime
					MVec = GetAnimLinearMovement(this) * ft
					MVec = this.VectorToParent(MVec)
	
					if(iTransitionAnim == 0 && MotionAnim != -2)
					{
						vector zz = MVec
						VectorNormalize(zz)
						ft = fabs(zz * WVector) + 0.4
						clamp ft<0, 1>
		
						MVec = WVector * VectorLength(MVec)
						MVec = MVec * ft
					}
				}
	
				float zvel = pvel[2]

				//HACK: keep them on ground at any cost
				if(atype != t_walk)
				{
					if(zvel > 32 && !GetFlags(this) & TFL_ONGROUND)
						zvel = -1
				}
	
				SetVelocity(this, Vector(MVec[0], MVec[1], zvel))
				
				MVecLen = VectorNormalize(MVec)
			}

			SetAngles(this, GetAngles(this) + GetAnimAngularMovement(this))
		}

		if(!Loged)
		{
#ifdef EDITOR
			if(!World.EditorActive)
			{
#endif
				float d, tang
				
				if(turndist)
				{
					turndist = faceyaw

					d = CorrectAngle(turndist, turnangle, turnspeed * 5)
					ang[YAW] = d
					turndist -= d

					if(fabs(turndist) < 1.0)
						turndist = 0

					SetYaw(d)
				}
				else
				{
					if(WTurn)
					{
						tang = FixAngle(Vector2Yaw(WVector) + WAddAngle)

						d = DiffAngle(tang, faceyaw)

						if(fabs(d) > 0.1)
						{
							clamp d<20, FLT_MAX>
							SetYaw(CorrectAngle(faceyaw, tang, MVecLen * 4.0))
						}
					}
				}
#ifdef EDITOR
			}
#endif
		}

		//look at our target
		if(Attacker)
		{
			 if(Alerted() || status == STATE_LOST_TARGET || status == STATE_COVER)
			 {
				LookAt(Attacker)
			}
		}

		ChangeAnim()
	}

//-----------------------------------------------------------------
	void M_Activate(ExtClass other)
	{
		if(flags & FL_WAIT4TRIGGER)
		{
			Awake()

			if(Squad)
			{
				Squad.OnMemberActivate(this)
			}
		}
	}

//-----------------------------------------------------------------
	int CountAnims(int base, int max)
	{
		for(int count = 0; count < max; count++)
		{
			if(!ASet.Anims[base + count])
				break
		}
		return count
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(!AnimSetName)
		{
			AnimSetName = DefaultAnimSetName
		}

#ifdef EDITOR
		if(!SoundSetName)
			SoundSetName = "LightTrooper"
#endif
		if(!GetAnimSet(AnimSetName))
			return

		//init idle anims		
		NumRelaxIdleAnims = CountAnims(a_relax_idle, 4)
		NumRelaxIdleVarAnims = CountAnims(a_relax_idle_var, 4)

		NumAlertIdleAnims = CountAnims(a_alert_idle, 4)
		NumAlertIdleVarAnims = CountAnims(a_alert_idle_var, 4)

		NumCombatIdleAnims = CountAnims(a_combat_idle, 4)
		NumCombatIdleVarAnims = CountAnims(a_combat_idle_var, 4)

		NumSpotIdleAnims = CountAnims(a_spot_idle_var, 4)
		NumFightIdleAnims = CountAnims(a_fight_idle_var, 4)
		NumLostIdleAnims = CountAnims(a_lost_idle_var, 4)
		
		NumCloseCombatAnims = CountAnims(a_combat_closecombat, 5)

		//set static capabilities
		if(ASet.Anims[a_combat_run_fire + 0] && ASet.Anims[a_combat_run_fire + 1] && ASet.Anims[a_combat_run_fire + 2])
			bAngledWalk = true
			
		if(ASet.Anims[a_combat_llean] && ASet.Anims[a_combat_rlean])
			bCanLean = true
			
		if(ASet.Anims[a_crouch])
			bCanCrouch = true

		if(ASet.Anims[a_act_left_go] && ASet.Anims[a_act_right_go] && ASet.Anims[a_act_back_go] && ASet.Anims[a_act_forward_go])
			bHasGestures = true
			
		if(ASet.Anims[a_act_look_left] && ASet.Anims[a_act_look_right])
			bCanConfirmGesture = true

		if(ASet.Anims[a_crouch_fire] && ASet.Anims[a_crouch_fire + 1])
			bCanFireInCrouch = true

		if(ASet.Anims[a_combat_run_fire] && ASet.Anims[a_combat_run_fire + 1] && ASet.Anims[a_combat_run_fire + 2])
			bCanShootInTakeCover = true
		
		if(ASet.Anims[a_spot_idle_var + 2])
			bHasSpotAnim = true

		if(ASet.Anims[a_combat_grenade + 0] && ASet.Anims[a_combat_grenade + 1] && ASet.Anims[a_combat_grenade + 2] && ASet.Anims[a_combat_grenade + 3])
			bCanThrowGrenade = true

		if(ASet.Anims[a_combat_grenade_crouch + 0] && ASet.Anims[a_combat_grenade_crouch + 1] && ASet.Anims[a_combat_grenade_crouch + 2])
			bCanThrowGrenadeInCrouch = true

		if(ASet.Anims[a_jump_drop] && ASet.Anims[a_combat_railjump] && ASet.Anims[a_combat_windowjump] && ASet.Anims[a_combat_crawl])
			bCanUseConnections = true

		if(bCanThrowGrenadeInCrouch || bCanThrowGrenade)
		{
			grenade = new item_weapon_grenade(this)
			grenade.Show(false)
			ClearEventMask(grenade, EV_VISIBLE | EV_FRAME)
			ClearFlags(grenade, TFL_VISIBLE|TFL_NOSHADOW)

			GrenadeShotStor = new ShotStorage
			GrenadeShotStor.damage = WDmgEnemyGrenade
			GrenadeShotStor.power = 25
			GrenadeShotStor.NumBullets = 1
			GrenadeShotStor.MarkSize = 26
			GrenadeShotStor.StreakWidth = 1.2  * sqrt(sqrt(1 / g_timescale))
			GrenadeShotStor.StreakLength = 45.6  * sqrt(sqrt(1 / g_timescale))
			GrenadeShotStor.StreakSpeed = 5100
			GrenadeShotStor.StreakShader = 0
                        GrenadeShotStor.RicDecalType = DE_PLASMA
			GrenadeShotStor.shooter = this
		}

		if(ClassName(CurrentWeapon) == "item_weapon_flamethrower" || ClassName(CurrentWeapon) == "item_weapon_rocketlauncher")
			bNoLOFLatency = true

		//puppets has deferred loading in order to save memory
		if(ClassName(this) != "creature_puppet")
			LoadSoundSet(SoundSetName, CSounds)

		halfrange = visrange * 0.5
		quarterrange = visrange * 0.25

		DistTolerance = Radius * 1.5
		clamp DistTolerance<8, 64> //lower threshold for small animals

//Print(String(ClassName(this) + " " + itoa(DistTolerance)))

		//DistTolerance = 15 //!!!

		int ev = 0

		if(!flags & FL_SUMMON_TRIGGER)
			ev |= EV_VISIBLE | EV_TOUCH | EV_FRAME | EV_ANIMEND | EV_SOUNDEND

		SetEventMask(this, ev)

		MaxHealth = Health

		CurrentPos = coords

		if(!lowhealth)
			lowhealth = 50
		bOnLowHealth = true

		SetState(s_relaxed)
		SetAnimState(t_idle)

		ChangeAnim()
		BumpFrame(this, 0, 0)

		ShowAI = false
		WAI = NULL
	}

//-----------------------------------------------------------------
float NextRefreshTime

	void EOnTouch(AICreatureClass other, int extra)
	{
		if(other == World || other.ClassType == ClassTypeTrigger)
			return

		if(!IsInherited(other, CExtClass))
			return

		//narazili jsme do dveri?
		if(TestDoorBlock(other))
			return

		if(GetFlags(other) & TFL_PATHBLOCK)
			other.UpdateAgent(true)

		//vsechny dalsi testy se tykaji vyhradne potvor!
		if(other.ClassType != ClassTypeCreature)
			return

		//stoji nam nekdo v ceste?
//VERY dangerous. May cause huge traffic in crowd of monsters! Limited to 0.5 sec period now!
		if(NextRefreshTime < currenttime && InFront(other))
		{
			NextRefreshTime = currenttime + 0.5
			//force update
			LastPathTime = AI_PATHREFRESH * -0.25 + LastPathTime
		}

		if(!flags & FL_FEEL || status == STATE_ATTACK || status == STATE_CHASE)
			return

		if(IsEnemy(other) && IsEnemy(other))
			AIAddEvent(AIE_SHOTORTOUCH, GetOrigin(other), other)
	}

//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{
		ShowAI = false
		WAI = NULL
#ifdef DEVELOPER
		WDLine = NULL
#endif
		//LoadSoundSet(SoundSetName, CSounds)
	}

//-----------------------------------------------------------------
	void TraceStart()
	{
		iTraceHit = 1
		CurrentWeapon.TraceStart()
	}

//-----------------------------------------------------------------
	void TraceEnd()
	{
		iTraceHit = 0
		CurrentWeapon.TraceEnd()
	}

//-----------------------------------------------------------------
	void ~AICreatureClass()
	{
		BeforeBodyDestroy()
		dBodyDestroy(this)

		if(geom)
		{
			dGeomDestroy(geom)
		}

		if(CurrentWeapon)
		{
			delete CurrentWeapon
			CurrentWeapon = NULL
		}

		if(ASet)
			ASet.Unload()

		if(SSet)
			SSet.Unload()

		if(WalkWaypoint)
			delete WalkWaypoint

		if(TempWaypoint)
			delete TempWaypoint

		if(OriginWaypoint)
			delete OriginWaypoint

		if(WPosition2)
			delete WPosition2		

		if(GrenadeShotStor)
		{
			delete GrenadeShotStor
			GrenadeShotStor = NULL
		}
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		WeaponRange = AttackDistMin

		CallMethods(this, OnLoad)
		InitCreature()
	}

//-----------------------------------------------------------------
	void AICreatureClass()
	{
		TickTime = g_Ticktime
		g_Ticktime = g_Ticktime + TICK_PERIOD
		
		RagdollMaterial = "material/default"
		RagdollMass = 45 * sqrt(g_timescale)

		if(visrange == 0)
			visrange = 1024

		AttackDist = 1024
		AttackDistMin = 92
		AttackDistClose = 64
		AIInit()

		SetFlags(this, TFL_DEF | TFL_TOUCHTRIGGERS)
		SetEventMask(this, EV_LOADED|EV_INIT)

		SetKind(this, MONSTER)

		facevec[0] = cos(angle * DEG2RAD)
		facevec[1] = sin(angle * DEG2RAD)

		//BoneHurtFactor = Human_Hurt

		CCount = CreatureCount++
		CreatureCount &= 15

		SetState(s_relaxed)
		SetAnimState(t_idle)

		MinAttacksInRow[0] = 13
		MaxAttacksInRow[0] = 17
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.12
		MinRestAfterAttack[0] = 1
		MaxRestAfterAttack[0] = 2

		MinAttacksInRow[1] = 7
		MaxAttacksInRow[1] = 12
		MinAttackDelay[1] = 0.1
		MaxAttackDelay[1] = 0.12
		MinRestAfterAttack[1] = 1
		MaxRestAfterAttack[1] = 2

		MinStunTime = 0.5
		MaxStunTime = 48
//		fShootpointTimeout = 4.5
		fShootpointTimeout = -1.0
		fNotInLOFTimeout = 3.3

		MinRoamIdleTime = 2.1
		MaxRoamIdleTime = 6.5

		//never
		fMinCosForSlowTurn = -100

		fMinBulletRain = 3.511

		fVisibilityLatency = 0.497
		fAlertedVisibilityLatency = 0.12
		fLeanShootTime = 250
		fCrouchShootTime = 220

		fMaxRange = 543
		fCloseHitRange = 39.8
		bFlameResistent = false

		LookAxis = -2
		LookBone = B_Common_Head

		TurnSpeed = 59.689 * sqrt(sqrt(sqrt(sqrt(sqrt(g_timescale)))))

		SmallPainGroup = g_all

		UsedOn = WP_USED_ON_ENEMY
		UsedOff = WP_USED_OFF_ENEMY

#ifdef DEVELOPER
		ShowAI = g_bShowAI
#endif

		MotionAnim = -1

		LastAmbientSound = -1
		AgentCost = 24

		WPosition2 = new CPosition

		GroupID = GROUP_NPC

		LastShootID = -1
		LastHitID = -1
	}
//---------------------------------------------------------------------------------
#ifdef EDITOR
	void EditorOpened()
	{
		Stopped = true
	}

	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		SetOrientation(Pos, Angs[1])
		UpdateMainVisualizer()
		UpdateEntity(this)
	}

	void CreateEditorVisuals()
	{
	}
#endif
}

//---------------------------------------------------------------------------------
CreatureClass info_squad::FindCommander(CreatureClass fromactor, ExtClass target)
{
	vector from = fromactor.CurrentPos
	vector face = fromactor.facevec

	vector targ = GetMatrix(target, 3)

	CreatureClass bestactor = NULL
	float bestcost = 999999999999

	for(int n = 0; n < NumMembers; n++)
	{
		AICreatureClass cc = Members[n]
		
//		if(cc != fromactor && cc.atype == t_idle)
		if(cc != fromactor && cc.bHasGestures)
		{
			vector cdir = cc.CurrentPos - from

			//dist is base for cost
			float cost = VectorNormalize(cdir)

			//is commander facing toward target?
			cdir = targ - cc.CurrentPos
			VectorNormalize(cdir)

			float dot = cdir * cc.facevec
			if(dot < 0.5)
				continue

			if(!P2PVisibilityEx(from, cc.CurrentPos, TRACE_VISTEST|TRACE_NOTRACE))
				continue

			int fd = g_iViewFrame - cc.viewframe
			
			if(fd > 3)
			{
				//probably not visible, that's bad
				cost += 200
			}

			if(cost < bestcost)
			{
				bestcost = cost
				bestactor = cc
			}
		}
	}
	
	return bestactor
}

