const string AIStatusNames[32] = {
	"NONE",
	"WALK",
	"IDLE",
	"OBSERVE",
	"SCRIPT",
	"GUARD",
	"RETURN",
	"LOST_TARGET",
	"1STSIGHT",
	"CHASE",
	"LR_ATTACK",
	"ATTACK",
	"CLOSECOMBAT",
	"COVER",
	"ROAMING",
	"BORN",
	"COVERFROMDANGER",
	"DIE",
	"DEAD",
	"ERROR"}


class WeaponClass;

const int GROUP_PLAYER	= 1
const int GROUP_NPC		= 2
const int GROUP_1			= 3
const int GROUP_2			= 4
const int GROUP_3			= 5
const int GROUP_4			= 6
const int GROUP_5			= 7
const int GROUP_6			= 8
const int GROUP_7			= 9

//=================================================================================
class Actor extends ExtClass
{
//keys
	vector   coords
	int      angle
	int		flags
	string	_materialremap

//visual
	vobject	MainObject
	
	int 		MaterialType //because of ragdolls

	vector	facevec
	float		faceyaw
///

//sounds
	hsound	CSounds[128]
	int		ASoundID[128]
	int		CSoundID[128]

	string	SoundSetName

//puppet anims
	string	PAnimNames[64]
	vobject	PAnims[64]
	int		NumPAnims
	int		MotionAnim


//-----------------------------------------------------------------
//Common Animhook sounds
void ASound(int n)
{
	hsound hs = g_sCommonSounds[n]
	if(!hs)
	{
		ASoundID[n] = 0
		return
	}
	ASoundID[n] = PlaySound(this, hs, SF_3D|SF_ONCE)
}

//-----------------------------------------------------------------
void ASoundLoop(int n)
{
	hsound hs = g_sCommonSounds[n]
	if(!hs)
	{
		ASoundID[n] = 0
		return
	}
	ASoundID[n] = PlaySound(this, hs, SF_3D)
}

//-----------------------------------------------------------------
void ASoundEnd(int n)
{
	EndSound(ASoundID[n])
}

//-----------------------------------------------------------------
//concreete animhook sounds
void CSound(int n)
{
	hsound hs = CSounds[n]
	if(!hs)
	{
		CSoundID[n] = 0
		return
	}

	CSoundID[n] = PlaySound(this, CSounds[n], SF_3D|SF_ONCE)
}

//-----------------------------------------------------------------
void CSoundLoop(int n)
{
	CSoundID[n] = PlaySound(this, CSounds[n], SF_3D)
}

//-----------------------------------------------------------------
void CSoundEnd(int n)
{
	EndSound(CSoundID[n])
}

//-----------------------------------------------------------------
vobject AddPAnim(string name)
{
	strlower(name)

	for(int n = 0; n < NumPAnims; n++)
	{
		if(PAnimNames[n] == name)
			return PAnims[n]
	}

	PAnimNames[n] = name
	PAnims[n] = GetObject(name)
	if(PAnims[n] == VNULL)
		Print(String(_name + ".AddPAnim: can't open " + name))

	NumPAnims++
	return PAnims[n]
}

//-----------------------------------------------------------------
void M_Show(ExtClass other)
{
	Show(true)
}
//-----------------------------------------------------------------
void M_Hide(ExtClass other)
{
	Show(false)
}

//-----------------------------------------------------------------
void SetYaw(float yaw)
{
	faceyaw = yaw

	SetAngles(this, Vector(0, yaw, 0))
	yaw *= DEG2RAD
	facevec = Vector(cos(yaw), sin(yaw), 0)
}

//-----------------------------------------------------------------
void SetOrientation(vector pos, float angle)
{
	SetOrigin(this, pos)
	SetYaw(angle)
}

//-----------------------------------------------------------------
void M_TurnToTarget(ExtClass other)
{
	other = FindClass(other._value)

	if(other)
	{
		vector vec = GetOrigin(other)
		vec = vec - GetOrigin(this)
		vec[2] = 0

		SetOrientation(GetOrigin(this), Vector2Yaw(vec))
	}
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

	vector org = GetOrigin(targ)

	int pfl = ClearFlags(this, TFL_SOLID)

	vector mins,maxs
	GetBoundBox(this, mins, maxs)
	
	//when waypoint is on the ground, we have to adjust position
	if(!TestPosition(this, org, mins, maxs, TRACE_DEFAULT))
	{
		vector from = org + Vector(0, 0, maxs[2] - mins[2])
		float plane[4]

		SetFlags(this, pfl)
		TraceLine(this, from, org, NULL, plane, NULL, NULL)
	}

	SetFlags(this, pfl)

	org = org + "0 0 0.03125"

	SetOrientation(org, Yaw(GetAngles(targ)))
}

//-----------------------------------------------------------------
/*
uz jednou je v misc_anim_object. Vyresit
void M_SeekAnim(ExtClass other)
{
	string val = other._value

	int p = strstr(val, ",")
	if(p == -1)
		return

	int slot = atoi(substr(val, 0, p))
	int frame = atoi(substr(val, p + 1, strlen(val) - p - 1))

	SetFrame(this, slot, frame)
}
*/
//-----------------------------------------------------------------
void M_SetAnim(ExtClass other)
{
	string val = other._value

	int slot = 0, n = 0, p
	float blendout = 0, blend = 0, fps = 30
	string name
	BoneMask bmask = NULL
	int af = 0
	string parm

loop:
	p = strstr(val, ",")
	if(p == -1)
		parm = val
	else
	{
		parm = substr(val, 0, p)
		val = substr(val, p + 1, strlen(val) - p - 1)
	}

	triminplace(parm)
	triminplace(val)

	if(n < 2)
	{
		switch(n)
		{
		case 0:
			slot = atoi(parm)
		break

		case 1:
			name = parm
		break
		}
	}
	else
	{
		strlower(parm)

		if(strstr(parm, "group") == 0)
		{
			parm = substr(parm, 5, strlen(parm) - 5)
			triminplace(parm)

			int grp
			if(!GetValue(NULL, parm, 0, grp) || grp < 0 || grp >= 32)
				Print("SetAnim: wrong group number")
			else
			{
				bmask = BoneMasks[grp]
			}
		}
		else
		{
			if(strstr(parm, "blendout") == 0)
			{
				blendout = atof(substr(parm, 8, strlen(parm) - 8))
			}
			else
			{
				if(strstr(parm, "blend") == 0)
				{
					blend = atof(substr(parm, 5, strlen(parm) - 5))
				}
				else
				{
					if(strstr(parm, "fps") == 0)
					{
						fps = atof(substr(parm, 3, strlen(parm) - 3))
						af |= AF_FORCEFPS
					}
					else
					{
						switch(parm)
						{
							case "wrap":
								af |= AF_WRAP
								break
								
							case "motion":
								MotionAnim = -2
								break
								
							case "once":
								af |= AF_ONCE
								break

							case "reset":
								af |= AF_RESET
								break

							default:
								Print(String("SetAnim: wrong parameter - " + parm))
								break
						}
					}
				}
			}
		}
	}
	n++
	if(p != -1)
		goto loop

	if(n < 2)
	{
		Print("SetAnim: not enough parameters")
		return
	}

	vobject anm
	if(name == "NULL" || name == "null")
		anm = VNULL
	else
	{
		anm = AddPAnim(name)
	}

	slot += s_cutscene
	clamp slot<s_cutscene, 7>

	if(blendout >= 0)
		af |= AF_BLENDOUT
	else
		blendout = 0

	SetAnimSlot(this, slot, anm, blend, blendout, bmask, fps, af|AF_RESET)	
}

//-----------------------------------------------------------------
void Sound(int sound, int flags)
{
	if(!CSounds[sound])
		return

	PlaySound(this, CSounds[sound], flags)
}

//-----------------------------------------------------------------
int RDSndCount
float RagdollMass
float RagdollTimeout
int NRDSounds


void WakeupRagdoll()
{
#ifdef GERMAN
	if(RagdollTimeout <= 0)
	{
		dBodyActive(this, true, true)
		dBodyInteractionLayer(this, ET_RAGDOLL)
	}
	RagdollTimeout = RAGDOLL_TIMEOUT
#endif
}			

void Actor()
{
	NRDSounds = 10
}

void ~Actor()
{
	ReleaseObject(MainObject, 0)

	for(int n = 0; n < NumPAnims; n++)
		ReleaseObject(PAnims[n], 0)
}
}



//=================================================================================
class RBone extends MinClass
{
	Actor Owner
	int NSounds

	void EOnSoundEnd(MinClass other, int extra)
	{
		SetEventMask(this, EV_TOUCH)
		Owner.RDSndCount--
	}

	void EOnTouch(MinClass other, int extra)
	{
		if(Owner.RDSndCount > 1)
			return

		int impact = extra >> 8 & 255

		float relimpact = impact / 70//Owner.RagdollMass

		if(relimpact > 2.5)
		{
			hsound hs = RagdollPhysicsSounds[Owner.MaterialType]
			if(hs)
			{
				int isound = PlaySound(this, hs, SF_ONCE)

				if(isound == 0)
					return

				float v = relimpact * 0.15
				clamp v<0.05, 1>

				SetSoundVolume(isound, LinearVol(v))
				Owner.RDSndCount++
				NSounds--
				if(NSounds <= 0)
				{
					ClearEventMask(this, EV_TOUCH|EV_SOUNDEND)
					return
				}
			}
			ClearEventMask(this, EV_TOUCH)
		}
	}

	void Init(MinClass owner, int nsounds)
	{
		Owner = owner
		SetEventMask(this, EV_TOUCH|EV_SOUNDEND)
		SetFlags(this, TFL_COLLIDEONLYWORLD)
		NSounds = nsounds
	}
	
	void RBone()
	{
		ClassType = ClassTypeRBone	
	}
}

vobject GetParticleObject(int Mat, int RPartType);
hshader GetDecalShader(int Mat, int RDecalType);



//=================================================================================
class Projectile extends RigidBodyClass
{
	Projectile NextProjectile
	string model
	vobject MainObject
	vector from
	vector target
	vector dir
	float	length
	float size
	float speed
	bool	dynamic
	float damage
	hsound Sound1, Sound2
	ShotStorage SStorage
	dBlock block
	CreatureClass ColBlockOtherBody

//-----------------------------------------------------------------
	void RemoveColBlock(bool RemoveDependencies);

//-----------------------------------------------------------------
	void OnShooterBodyRemove()
	{
		RemoveColBlock(false)
	}
//-----------------------------------------------------------------
	bool CreateBody();
//-----------------------------------------------------------------
	void Fire(ShotStorage ShotStor);
//-----------------------------------------------------------------
	void BeforeFire()
	{
	}
//-----------------------------------------------------------------
	void ModifyParms(int version)
	{
	}
//-----------------------------------------------------------------
	void InitStorage()
	{
	}
//-----------------------------------------------------------------
	void Projectile(int version);
}


//=================================================================================
class CreatureClass extends Actor
{
	int		Color2
	float		UVAnim

/////
	float		stepwidth

	vector	CurrentPos

	float		fZDamper
	float		fADamper
	float		LastZ

	float		fYawLimit
	
	int		LookAxis, LookBone

	int		iTransitionAnim //jump over, crawl under and so on

//align to ground
	float		aang1, aang2
	float		aprevz

//Bounding info
	float		Height, HHeight, Radius
	vector	Mins, Maxs, HHVec

	ItemClass	CurrentWeapon

	int GroupID

/////
	string	model

//Global parameters
	float		Health,MaxHealth

	bool		bAlignOnGround
	bool		Humanoid //we need to know if creature has ordinary head and so on...

//Runtime
	int		atype,otype
	int 		astate,ostate
	int		aextra,oextra

	int		Noises

	MinClass Hands	//playerove ruky

	//items
	int		Ammo[10]
	int		AmmoType

//for AIChase/AIMeleeAttack

//attacking us
	CreatureClass	Attackers[10]
	int		NumAttackers

//we are attacking
	CreatureClass	EnemyEnt
	int		AttackerNum

	bool		Immortality

	AnimSet	ASet

	//bool 		DisabledAnim
	bool		Disabled

	CreatureClass Attacker

	widget	WAI
	bool		ShowAI
	int		Tactics

	int		astatus

	RBone		RBones[64]
	bool		ragdollActive

	float		HurtFactor[64]

	int		ShapeIndex
	float		AgentCost

	int		FSType

	float		LastSayTime

	int		viewframe
//LookAt
float			LAddAngle	//Angular displacement for look at
CreatureClass LookTarget
float LookTimeout
float PLookQuat[4]
float PLookQuat2[4] //some creatures has separate bone for pitch/yaw

Projectile FirstProjectile

//-----------------------------------------------------------------
void M_SetHealth(ExtClass other)
{
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

	vector org = GetOrigin(targ)

	org = org + HHVec + "0 0 1"

	SetOrientation(org, Yaw(GetAngles(targ)))
}

//-----------------------------------------------------------------
void AddProjectileDependence(Projectile proj)
{
	proj.NextProjectile = FirstProjectile
	FirstProjectile = proj
}

//-----------------------------------------------------------------
void RemoveProjectileDependence(Projectile proj)
{
	Projectile cur = FirstProjectile
	Projectile next = NULL
	Projectile last = NULL

	while(cur)
	{
		next = cur.NextProjectile

		if(cur == proj)
		{
			if(last)
				last.NextProjectile = next
			else
				FirstProjectile = next

			return
		}
		last = cur
		cur = next
	}
}

//-----------------------------------------------------------------
void M_SetGroup(ExtClass other)
{
	string grp = other._value
	strlower(grp)
	
	switch(grp)
	{
	case "player":
		GroupID = GROUP_PLAYER
		break
	case "npc":
		GroupID = GROUP_NPC
		break
	case "group1":
		GroupID = GROUP_1
		break
	case "group2":
		GroupID = GROUP_2
		break
	case "group3":
		GroupID = GROUP_3
		break
	case "group4":
		GroupID = GROUP_4
		break
	case "group5":
		GroupID = GROUP_5
		break
	case "group6":
		GroupID = GROUP_6
		break
	case "group7":
		GroupID = GROUP_7
		break
	default:
		DPrint("Unknown group ID: " + grp)
		ErrorNum++
	}
}

//-----------------------------------------------------------------
void Show(bool show)
{
	if(show)
	{
		SetFlags(this, TFL_VISIBLE)
	}
	else
	{
		ClearFlags(this, TFL_VISIBLE)
	}

	if(CurrentWeapon)
		CurrentWeapon.Show(show)
}

//-----------------------------------------------------------------
void M_ShowWeapon(ExtClass other)
{
	if(CurrentWeapon)
		CurrentWeapon.Show(true)
}
//-----------------------------------------------------------------
void M_HideWeapon(ExtClass other)
{
	if(CurrentWeapon)
		CurrentWeapon.Show(false)
}

//-----------------------------------------------------------------
void M_Hurt(ExtClass other)
{
	string tokens[8]
	int nt = ParseString(other._value, tokens)
	
	if(nt < 1)
		return

	int bone = B_Spine

	if(!GetValue(NULL, "B_" + tokens[0], 0, bone))
	{
		DPrint(_name + ".Hurt() has undefined bone " + tokens[0])
		return
	}

	UpdateEntity(this)
	vector mat[4]
	float plane[4]
	GetBoneMatrix(this, bone, mat)
	vector norm = mat[1] * -1
	vector to = mat[3]
	vector pos = norm * 12 + to
	norm = -norm

	if(TraceLineEx(DEFAULT, pos, to, ZeroVec, ZeroVec, other, plane, NULL, NULL, TRACE_RAGDOLLS|TRACE_DEFAULT|TRACE_BONES, NULL) >= 1)
		return

//AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos, to)
//AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos-"1 1 1", pos+"1 1 1")

   misc_particle_effector eff = NULL
   /*
	vobject ParticlesObj = GetParticleObject(MATERIAL_FLESH, PE_BULLET)
	
	if(ParticlesObj)
	{
		eff = new misc_particle_effector
		eff.SetParmsEx(ParticlesObj, to, true, 2)
		vector BounceVec = norm	//spravime si "odrazeny" vector
		float backoff = norm * BounceVec
		backoff *= 2			//2 = odraz presne podla uhla dopadu
		vector change = norm * backoff
		BounceVec -= change
	
		vector vec1
		vector vec2
		vec1[0] = BounceVec[1]	//spravime si kolmy vektor
		vec1[1] = BounceVec[2]
		vec1[2] = BounceVec[0]
		vec2 = BounceVec * vec1
		vec1 = vec2 * BounceVec
		VectorNormalize(vec1)
		VectorNormalize(vec2)
	
		mat[0] = vec1
		mat[1] = vec2
		mat[2] = BounceVec
		mat[3] = to
		SetFlags(eff, TFL_FULLMATRIX)
		SetMatrix(eff, mat)
		UpdateEntity(eff)
	}
*/
	CreateDecal(this, to, norm * 8.0, 6.0, 0, 8, GetDecalShader(MATERIAL_FLESH, DE_BULLET), -1, 0)	

	//add bloody spurts
	for(int n = 0; n < 2; n++)
	{
//			vector v = Vector(norm[0] + frand(-0.3, 0.3), norm[1] + frand(-0.3, 0.3), -1)
		vector v = Vector(frand(-0.3, 0.3), frand(-0.3, 0.3), -1)
		VectorNormalize(v)
		
		vector dto = v * 75.3 + to
		
		if(TraceLineEx(DEFAULT, to, dto, ZeroVec, ZeroVec, other, plane, NULL, NULL, TRACE_WORLD, NULL) < 1)
		{
//AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos, to)

			CreateDecal(other, dto, Vector(plane[0], plane[1], plane[2]) * -8, 3.0, 0, 32, BloodDecalShaders[rand(0,4)], -1, 0)
		}
	}

}

//-----------------------------------------------------------------
void UpdateAgent(bool set)
{
	//HACK - don't block pathfinder by NPCs
//	if(this != World.Player || status > STATE_DIE)
	if(status > STATE_DIE)
	{
		if(agent)
		{
			agent.Destroy()
			agent = NULL
		}
		return
	}

	if(!set)
	{
		if(agent)
		{
			agent.Destroy()
			agent = NULL
		}
	}
	else
	{
		if(!GetFlags(this) & TFL_SOLID)
			return

		agent = World.pPathFinder.AgentFromID(agent, CurrentPos, ShapeIndex, AgentCost)
		
		if(agent)
		{
			agent.SetOwner(this)
		}
	}
}

//-----------------------------------------------------------------
void SetBBoxInfo()
{
	GetBoundBox(this, Mins, Maxs)

	vector vec = Maxs - Mins

	Height = vec[2]
	HHeight = Height * 0.5
	HHVec = Vector(0, 0, HHeight)
	Radius = vec[0] * 0.5
}

//-----------------------------------------------------------------
void SetOrientation(vector pos, float angle)
{
	CurrentPos = pos
	LastZ = pos[2]
	aprevz = LastZ
	SetOrigin(this, pos)
	SetYaw(angle)
}

//-----------------------------------------------------------------
//pass anim exceptions to weapons
//-----------------------------------------------------------------
void EOnAnimEnd(MinClass other, int extra)
{
	if(extra == s_weapon && CurrentWeapon)
		Throw(CurrentWeapon, this, EV_ANIMEND, extra)
}

//-----------------------------------------------------------------
	void ApplyExplosionOnRagdoll(vector pos, float radius, float power)
	{
		vector mins, maxs
		GetCBoundBox(this, mins, maxs)

		vector Org = mins + maxs * 0.5
		vector ForceDir = Org - pos
		float dist = VectorNormalize(ForceDir)
		ForceDir = ForceDir + "0 0 1"		//maly fake aby ich to hadzalo viac do vysky
		ForceDir = ForceDir * 0.5
		float scale = dist / radius
		scale = 1 -scale
		power += power * scale
		vector Force = ForceDir * power
		RBone CurBone

		for(int n = 0; n < sizeof(RBones); n++)
		{
			CurBone = RBones[n]
			if(CurBone)
				dBodyApplyImpulse(RBones[n], Force)
		}
	}

//-----------------------------------------------------------------
	int Shoot(ShotStorage shot)
	{
		if(shot.HurtType == HT_EXPLOSION)
		{
			float pow = shot.power * ExplosionImpactScale
	
			if(ragdollActive)
			{
				ApplyExplosionOnRagdoll(shot.from, shot.radius, pow)
				return true
			}
		}
		return false
	}

vector lastart
//-----------------------------------------------------------------
	CPosition GetWPos()
	{
		if(WAFrame == g_iViewFrame)
			return WPosition

		if(WPosition == NULL)
			WPosition = new CPosition

		//and after some decent distance
		vector start = GetMatrix(this, 3)
		if(VectorLengthSq(start - lastart) > 2)
		{
			lastart = start

			if(!World.pPathFinder.PositionForPoint(start + "0 0 20", WPosition))
			{
				delete WPosition
				WPosition = NULL
			}
		}
		WAFrame = g_iViewFrame

		return WPosition
	}


//-----------------------------------------------------------------
	void M_Solid(ExtClass other)
	{
		SetFlags(this, TFL_SOLID)
	}
//-----------------------------------------------------------------
	void M_NonSolid(ExtClass other)
	{
		ClearFlags(this, TFL_SOLID)
	}

//-----------------------------------------------------------------
	bool UseAmmoType(int Atype)
	{
		if(AmmoType == Atype)
			return true

		return false
	}

//-----------------------------------------------------------------
	bool InitRagdoll(string rdef, float mass, string mat)
	{
		MaterialType = dMaterialGetType(mat)

		UpdateEntity(this)
		RagdollMass = mass
		
		//reset all custom transformations
		for(int n = 0; n < 64; n++)
		{
			SetBone(this, n, ZeroVec, ZeroVec, 1.0)
			SetBoneMatrix(this, n, g_vIdentMatrix)
			SetBoneGlobal(this, n, g_vIdentMatrix)
		}
		
		if(dRagdollCreate(this, rdef, mass, mat, CRBone, RBones))
		{
			ragdollActive = true

			SetFlags(this, TFL_SOLID|TFL_FULLMATRIX|TFL_DYNAMICBBOX|TFL_COLLIDEONLYWORLD)

			for(n = 0; n < 64; n++)
			{
				RBone bone = RBones[n]

				if(bone)
					bone.Init(this, NRDSounds)
			}

			if(rdef == "heavyspider")
				dBodyInteractionLayer(this, ET_DEFAULT)
			else
				dBodyInteractionLayer(this, ET_RAGDOLL)

			RagdollTimeout = RAGDOLL_TIMEOUT
			return true
		}
		return false
	}

int SlotFlags[12]
/*
void AssertAnim(int anm)
{
	if(!ASet.Anims[anm])
	{
		Print(String(_name + " trying to play NULL anim " + itoa(anm)))
		DumpStack()
	}
}
*/
//-----------------------------------------------------------------
// Play anim from AnimSet
//-----------------------------------------------------------------
bool PlayAnim(int slot, int anm, float blend, int grp, int fps, int flags)
{
	if(ASet == NULL)
		return false

	//AssertAnim(anm)
	
	//topmost priority exception
	if(fps < 0)
	{
		fps = -fps
		flags |= AF_FORCEFPS
	}
	else
	{
		//2nd priority - AnimSet
		if(ASet.FPS[anm])
		{
			fps = ASet.FPS[anm]

			flags |= AF_FORCEFPS
		}

		//3rd - from animdef, if present. Otherwise 'fps' value
	}
/*
if(!ASet.Anims[anm])
{
	DPrint(_name + " has undefined anim " + itoa(anm))
}
*/
	SlotFlags[slot] = ASet.Flags[anm]
	
	SetAnimSlot(this, slot, ASet.Anims[anm], blend, blend, BoneMasks[grp], fps, flags)

	if(!ASet.Anims[anm])
		return false

	return true
}

/*
string Int2Hexa(int in)
{
	const string num[16] = {
		"0", "1", "2", "3",
		"4", "5", "6", "7",
		"8", "9", "a", "b",
		"c", "d", "e", "f"}
		
	string out = ""
	for(int n = 0; n < 8 && in != 0; n++)
	{
		out = num[in & 0x0f] + out
		in = in >> 4
	}
	
	return "0x0" + out
	
}
*/
//-----------------------------------------------------------------
vobject DustFootRun, DustFootWalk

//-----------------------------------------------------------------
void Footstep(float f)
{
	int ground = GetGroundFlags(this)

	if(ground == -1)
		ground = MATERIAL_METAL << SURF_MATERIAL_SHIFT

	if(ground & SURF_DUST)
	{
		vector pos = GetMatrix(this, 1) * stepwidth * 0.5 * f + CurrentPos - "0 0 26"
		
		if(IsBoxVisible(pos - "10 10 4", pos + "10 10 16", true))
		{
			misc_particle_effector childparticles
			
			childparticles = new misc_particle_effector
	
			if(atype == t_run)
				childparticles.SetParmsEx(DustFootRun, pos, true, 2)
			else
				childparticles.SetParmsEx(DustFootWalk, pos, true, 2)
		}
	}

	hsound hs
	
	int surf = Surf2Mat(ground) & 31
		
	if(f < 0)
		hs = LFootstepSounds[FSType][surf]
	else
		hs = RFootstepSounds[FSType][surf]

	if(hs)
	{
		int ph = PlaySound(this, hs, SF_ONCE|SF_QUARTERRANGE)
		
		if(atype == t_walk && ph)
			SetSoundVolume(ph, 0.87)
	}
}

//-----------------------------------------------------------------
void RFootstep()
{
	Footstep(-1)
}

//-----------------------------------------------------------------
void LFootstep()
{
	Footstep(1)
}

//-----------------------------------------------------------------
	//for proper FSM switching
	int StateCount, CurrentStateCount

//-----------------------------------------------------------------
// some creatures (player) can crouch
//-----------------------------------------------------------------
	vector GetCenterOfBody()
	{
		return GetMatrix(this, 3)
	}

//-----------------------------------------------------------------
	int StartBurn(bool DynLight)
	{
		return 0
	}

//-----------------------------------------------------------------
	void RemoveAttacker(int att)
	{
		if(Attackers[att])
		{
			Attackers[att] = NULL
			NumAttackers--
		}
	}

//-----------------------------------------------------------------
	void EndAttack()
	{
		if(AttackerNum >= 0)
		{
			EnemyEnt.RemoveAttacker(AttackerNum)
			AttackerNum = -1
		}
	}

//-----------------------------------------------------------------
	void AddAttacker(CreatureClass attacker)
	{
		//if attacker already has target
		if(attacker.AttackerNum >= 0)
		{
			//it's me!! :-)
			if(attacker.EnemyEnt == this)
			{
				//use quicker way
				return
			}

			attacker.EnemyEnt.RemoveAttacker(attacker.AttackerNum)
			attacker.AttackerNum = -1
			attacker.EnemyEnt = NULL
		}

		for(int n = 0; n < sizeof(Attackers); n++)
		{
			if(Attackers[n] == NULL)
			{
				NumAttackers++
				Attackers[n] = attacker

				attacker.EnemyEnt = this
				attacker.AttackerNum = n
				return
			}
		}
		return //shouldn't happen
	}

//-----------------------------------------------------------------
	void ChangeYaw(float yaw)
	{
	}

//-----------------------------------------------------------------
	void NotifyNoise(MinClass actor, vector origin, int noises)
	{
	}

//-----------------------------------------------------------------
	vector EyesPos()
	{
		vector mat[4]

		if(!GetBoneMatrix(this, B_Hlava, mat))
		{
			return GetOrigin(this)
		}

		return mat[3]
	}

//-----------------------------------------------------------------
//hit to:
//back  - 0
//left  - 1
//front - 2
//right - 3
int HitDirection(vector dir)
{
	float dyaw = FixAngle(atan2(dir[1], dir[0]) * RAD2DEG)
	return FixAngle(faceyaw - dyaw + 45) / 90
}

int HitDirection2(vector dir)
{
	float dyaw = FixAngle(atan2(dir[1], dir[0]) * RAD2DEG)
	return FixAngle(faceyaw - dyaw + 90) / 180
}

//-----------------------------------------------------------------
	void SetXYVelocity(vector vel)
	{
		vector v = GetVelocity(this)
		vel[2] = v[2]
		SetVelocity(this, vel)
	}

//-----------------------------------------------------------------
	void SetZVelocity(float zvel)
	{
		vector v = GetVelocity(this)
		v[2] = zvel
		SetVelocity(this, v)
	}

//-----------------------------------------------------------------
	bool TraceFilter(ExtClass other, vector pos, vector dir)
	{
		if(other == this || other.ClassType == ClassTypeCreature || other.ClassType == ClassTypeFunc)
			return false

		return true
	}

//-----------------------------------------------------------------
	void M_EnableImmortality(ExtClass other)
	{
		Immortality = true
	}

	void M_DisableImmortality(ExtClass other)
	{
		Immortality = false
	}

	void M_Disable(ExtClass other)
	{
		Disabled = true
	}

	void M_Enable(ExtClass other)
	{
		Disabled = false
	}

//-----------------------------------------------------------------
// default implementation. Suitable for humanoids
//-----------------------------------------------------------------
	void SetHeadOrientation(float pitch, float yaw)
	{
		float lquat[4], quat[4]
		vector mat[4]

		if(pitch || yaw)
		{
			clamp yaw<-70, 70>
			clamp pitch<-15, 15>

			//prepare quaternion for head rotation. Transform is in local space of object,
			//so axes are always the same
			QuatRotationRollPitchYaw(quat, Vector(-yaw, 0, -pitch))
		}
		else
		{
			QuatIdentity(quat)
		}

		QuatLerp(lquat, PLookQuat, quat, ftime * 9)

		if(PLookQuat[0] != lquat[0] || PLookQuat[1] != lquat[1] || PLookQuat[2] != lquat[2] || PLookQuat[3] != lquat[3])
		{
			QuatMatrix(lquat, mat)
			SetBoneGlobal(this, B_Common_Head, mat)
			QuatCopy(lquat, PLookQuat)
		}
	}

Shape wshape

//-----------------------------------------------------------------
	bool WTraceFilter(CreatureClass other, vector raypos, vector raydir)
	{
		if(other.ClassType == ClassTypeMiscPhysicsGlass || other.ClassType == ClassTypeMiscShootDetector || other.ClassType == ClassTypeMiscShootingShield)
			return other.IsShot(raypos, raydir)

		if(other == this || !IsInherited(other, CExtClass))
			return false

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
//!!!!!!!!!!!!!!Just for player!!!!!!!!!!!!!!!!!!
	void DropManipEntity(bool ChangeWeap)
	{
	}

	void RemoveHackAdept(ExtClass ent)
	{
	}

	void InterruptHacking(bool HackModeOff)
	{
	}

	void RemoveInteractionAdept(ExtClass adept)
	{
	}

	WeaponClass CreateWeapon(int ClNum, int ammo, int MagazineAmmo)
	{
		return NULL
	}

	bool PickWeapon(ItemClass item)
	{
		//allow monsters to pickup any weapons
		return true
	}

	void PickAmmo(int type, int quantity)
	{
	}

	bool WeaponChange(int WeaponIndex, bool RequestToWeapon)
	{
	}

	int GetBestAvailableWeapon()
	{
	}

	void UpdateHUD(int id)
	{
	}

	void SetThrowBar(float PosRate, bool show)
	{
	}

	float GetOxygen()
	{
		return 0
	}

	float GetBulletTime()
	{
		return 0
	}

	void AddOxygen(float quantity)
	{
	}

	void AddHealth(float quantity)
	{
	}

	bool GetImmortality()
	{
		return Immortality
	}

	int GetDifficultyLevel()
	{
		return 0	
	}

	MinClass GetWeaponPointer(int Windex)
	{
		return NULL
	}

	void SetWeaponPointer(int Windex, MinClass ptr)
	{
	}

//-----------------------------------------------------------------
	int SetState(int st)
	{
		if(st != astate)
		{
			astate = st
			return false
		}
		return true
	}

	float StateTime[32]
	
//-----------------------------------------------------------------
	float StateAge(int st)
	{
		return currenttime - StateTime[st]
	}
	
//-----------------------------------------------------------------
	void SwitchFSM(int st)
	{
		StateTime[st] = currenttime
		StateTime[status] = currenttime
		
		if(status != st)
		{

//na testovani zahadnych stavu v AI
/*
if(_name == "heavyTrooper0")
{
			Print(String(_name + ".SwitchFSM: OldStatus " + AIStatusNames[status] + " NewStatus " + AIStatusNames[st]))
			if(StateCount != CurrentStateCount)
			{
				Print(String(_name + "Missed one or more state changes"))
			}
			DumpStack()
}
*/
			//HACK: avoid accidental change when dying
			if(status >= STATE_DIE && st < status)
				return

			status = st

			if(st |= STATE_1STSIGHT	&& st != STATE_CHASE && st != STATE_LR_ATTACK && st != STATE_ATTACK)
				EndAttack()
		}
		StateCount++
	}

//-----------------------------------------------------------------
	bool IsState(int st)
	{
		if(st != status || StateCount != CurrentStateCount)
			return false

		return true
	}

//-----------------------------------------------------------------
	void SelectItem(ItemClass item, int hand)
	{
		//vypni aktualni item
		RemoveChild(this, CurrentWeapon)

		if(CurrentWeapon != NULL)
			CurrentWeapon.Unselect()

		//a zapni novy
		item.Select()

		CurrentWeapon = item

		SetOrigin(item, ZeroVec)
		AddChild(this, item, hand)

		//vynut update animace
		otype = -1
	}

//-----------------------------------------------------------------
	void AlignToGround()
	{
		if(iTransitionAnim)
			return

		if(bAlignOnGround)
		{
			int fl = ClearFlags(this, TFL_SOLID)

			vector head, ltail, rtail
			vector head2, ltail2, rtail2
			vector height

			float plane[4]
			vector vec = facevec * Radius

			vector center = CurrentPos + "0 0 10" //for very steep angles
			head = Maxs

			//Based on radius * tan(60') + epsilon?
			height[2] = Radius * 1.732 + HHeight

			head = center + vec

			head2 = head - height
			if(TraceLineEx(MONSTER, head, head2, ZeroVec, ZeroVec, NULL, plane, NULL, NULL, TRACE_WORLD|TRACE_ENTS, NULL) >= 1)
			{
				SetFlags(this, fl)
				return
			}

	//		AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, head, head2)

			ltail = center - vec
			rtail = ltail
			ltail[0] = ltail[0] - vec[1]
			ltail[1] = ltail[1] + vec[0]
			ltail2 = ltail - height
			float ltailfrac = TraceLineEx(MONSTER, ltail, ltail2, ZeroVec, ZeroVec, NULL, plane, NULL, NULL, TRACE_WORLD|TRACE_ENTS, NULL)

	//		AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, ltail, ltail2)

			rtail[0] = rtail[0] + vec[1]
			rtail[1] = rtail[1] - vec[0]
			rtail2 = rtail - height
			float rtailfrac = TraceLineEx(MONSTER, rtail, rtail2, ZeroVec, ZeroVec, NULL, plane, NULL, NULL, TRACE_WORLD|TRACE_ENTS, NULL)

	//		AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, rtail, rtail2)

			SetFlags(this, fl)

			//fix case when we have one leg in the air
			if(ltailfrac >= 1)
			{
				ltailfrac = rtailfrac
				ltail2[2] = rtail2[2]
			}

			if(rtailfrac >= 1)
			{
				rtailfrac = ltailfrac
				rtail2[2] = ltail2[2]
			}

			float a1, a2

			if(ltailfrac >= 1 || ltailfrac >= 1)
			{
				//that's bad

				a1 = 0
				a2 = 0
			}
			else
			{
				vector e0, e1

				e0 = head2 - ltail2
				e1 = head2 - rtail2

				vector normal = e0 * e1
				VectorNormalize(normal)

				e1 = normal * facevec
				e0 = e1 * normal

//				AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, center, e0 * (float)64 + center)
//				AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, center, e1 * (float)64 + center)
//				AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, center, normal * (float)64 + center)

				const vector up = "0 0 1"
				a1 = asin(e0 * up) * RAD2DEG
				a2 = asin(e1 * up) * RAD2DEG
			}

			aang1 = CorrectAngle(aang1, a1, fADamper)
			aang2 = CorrectAngle(aang2, a2, fADamper)
		}
		else
		{
			aang1 = 0
			aang2 = 0
		}

		float f = CurrentPos[2]
		float d = ftime * fZDamper
		clamp d<0, 1>
		f = f - aprevz * d
		aprevz = aprevz + f

		vector vv
		vv[1] = aprevz - CurrentPos[2]

		//FIXME: odstrasujici ukazka extremniho programovani :-)
		if(ClassName(this) == "creature_dron")
			SetBone(this, B_Layer1, Vector(0, aang2, aang1), vv + "0 16 0", 1.0)
		else
			SetBone(this, B_Layer1, Vector(0, aang2, aang1), vv, 1.0)
	}

	vector LastAlignPos
	vector LastAgentPos
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		CurrentPos = GetOrigin(this)

		if(status < STATE_DIE)
		{
			//someone removed us. Update it!
			if(agent == NULL)
			{
				LastAgentPos = CurrentPos
				UpdateAgent(true)
			}
			else
			{
				vector dist = LastAgentPos - CurrentPos
				float f = dist * dist
				if(f > 4)
				{
					LastAgentPos = CurrentPos
					UpdateAgent(true)
				}
			}
		}

		//controlled AI's - player
		if(astatus != 0)
		{
			return
		}

		Noises = 0

		if(status != STATE_BORN && status < STATE_DIE)
		{
			if(VectorLengthSq(LastAlignPos - CurrentPos) > 0.0)
			{
				LastAlignPos = CurrentPos
				AlignToGround()
			}
		}
	}

//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{
		ShowAI = false
	}

//-----------------------------------------------------------------
	void EOnSave(MinClass other, int extra)
	{
		ShowAI = false
	}

//-----------------------------------------------------------------
	void BeforeInit()
	{
		//protoze v creature_puppet musime pridat zbran az v Initu,
		//jinak to samozrejme nefunguje ve workbenchi
	}
	
//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetEventMask(this, EV_ONSAVE)
	
		BeforeInit()
		float plane[4]
		vector vec

		SetBBoxInfo()	//nastavi Mins a Max

		SetOrientation(coords, angle)
		UpdateEntity(this)
	}

//-----------------------------------------------------------------
	void TraceStart()
	{
	}

//-----------------------------------------------------------------
	void TraceEnd()
	{
	}

//-----------------------------------------------------------------
	void AddBleedSource(ShotStorage shot)
	{
	}
//-----------------------------------------------------------------
	void AddSSource(ShotStorage shot)
	{
	}
	
//-----------------------------------------------------------------
	bool GetAnimSet(string name, bool fromload = false)
	{
		for(int n = 0; n < World.NumAnimSets; n++)
		{
			ASet = World.AnimSets[n]
	
			if(ASet.Name == name)
			{
				ASet.Load()
	
				if(!fromload)
				{
					//HACK: Nastav prvni frame nejake normalni pozy
					if(ASet.Anims[a_relax_idle])
					{
						SetAnimSlot(this, s_walk, ASet.Anims[a_relax_idle], 0.0, 0, NULL, 30, 0)
						BumpFrame(this, 0, 0)
					}
				}
				return true
			}
		}
		ASet = NULL
		DPrint("AnimSet '" + name + "' doesn't exist")
		return false
	}

//-----------------------------------------------------------------
	void ~CreatureClass()
	{
		if(WAI)
			DestroyWidget(WAI)

		if(ragdollActive)
		{
			dRagdollDestroy(this)
			for(int n = 0; n < sizeof(RBones); n++)
			{
				RBones[n] = NULL
			}
		}
	}

//-----------------------------------------------------------------
	void CreatureClass()
	{
		ClassType = ClassTypeCreature

		if(angle < 0)
			angle = 0

		SetFlags(this, TFL_USER6|TFL_DYNAMICBBOX)

		Color2 = 0xffffffff

		otype = -1
		ostate = -1
		oextra = -1

		stepwidth = 8.5

		fZDamper = 10
		fADamper = 20

		fYawLimit = 114

		SetEventMask(this, EV_ANIMEND|EV_ONLOAD)

		AttackerNum = -1

		Humanoid = true

		QuatIdentity(PLookQuat)
		QuatIdentity(PLookQuat2)

		DustFootRun = GetObject("particle/dustfootrun.ptc")
		DustFootWalk = GetObject("particle/dustfootwalk.ptc")

		GroupID = GROUP_PLAYER
		FSType = FSNone

		AgentCost = -1

		for(int n = 0; n < sizeof(HurtFactor); n++)
			HurtFactor[n] = 1.0
	}
}

//-----------------------------------------------------------------
void Projectile::RemoveColBlock(bool RemoveDependencies)
{
	if(RemoveDependencies)
	{
		if(ColBlockOtherBody.ClassType == ClassTypeCreature)
			ColBlockOtherBody.RemoveProjectileDependence(this)
	}

	if(block)
	{
		dBodyRemoveBlock(block)
		block = 0
	}
}

//-----------------------------------------------------------------
void RigidBodyClass::HurtEnemy(ShotStorage PhysHurtInfo)
{
	PhysHurtInfo.target.Shoot(PhysHurtInfo)
}

//=================================================================================
void DisableEnemies(bool player)
{
	/*
	for(CreatureClass cl = GetNextEntity(NULL, CAICreatureClass); cl != NULL; cl = GetNextEntity(cl, CAICreatureClass))
	{
		if(ClassName(cl) == "creature_player_puppet")
			continue

		if(cl.flags & FL_NODISABLE)
			continue

		cl.PlayAnim(0, a_relax_idle, 10.0, g_all, 30, 0)
	}
	*/
}

//-----------------------------------------------------------------
void EnableEnemies()
{
	/*
	for(CreatureClass cl = GetNextEntity(NULL, CAICreatureClass); cl != NULL; cl = GetNextEntity(cl, CAICreatureClass))
	{
		if(cl)
			SetAnimSlot(cl, 0, VNULL, 10.0, 1, NULL, 0, 0)
	}
	*/
}

