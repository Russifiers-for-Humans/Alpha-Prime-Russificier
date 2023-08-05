//TODO: make unified system for conditions, instead of many querying functions
//TODO: replace various combat states by Behavior classes with implemented innerloop
//TODO: create capability bits instead of many bools. Create Set/(Get?) methods

class AICreatureClass;

//-----------------------------------------------------------------
// compute on which side is target when looking 'from' in 'facevec' direction
// < 0 is on left
// > 0 is on right
//-----------------------------------------------------------------
float GetSide(vector target, vector from, vector facevec)
{
	//construct side plane
	vector pnormal = facevec * UpVec
	float pdist = pnormal * from

	return pnormal * target - pdist
}

//-----------------------------------------------------------------
vector FaceVectorTo(vector from, vector to)
{
	vector vec = to - from
	
	vec[2] = 0
	VectorNormalize(vec)
	return vec
}

//=================================================================================

//event slot indexes.
const int AIE_BULLETRAIN	= 0
const int AIE_SHOTORTOUCH	= 1
const int AIE_SAWENEMY		= 2
const int AIE_HEARDSHOT		= 3
const int AIE_SAWDEADBODY	= 4
const int AIE_SAWSHOTBODY	= 5
const int AIE_HEARDNOISE	= 6
const int AIE_SQUADATTACKED= 7
const int AIE_GRENADE		= 8

const int AIBufferSize = 9


const int AICMD_NONE			= 0
const int AICMD_IDLE			= 1
const int AICMD_RUN			= 2
const int AICMD_WALK			= 3
const int AICMD_ANIM			= 4
const int AICMD_USE			= 5
const int AICMD_CALL			= 6
const int AICMD_WSIGHT		= 7
const int AICMD_ANIMNW		= 8
const int AICMD_SLEEP		= 9
const int AICMD_TURN			= 10
const int AICMD_TURNBY		= 11
const int AICMD_SHOOT		= 12
const int AICMD_SHOOTNW		= 13
const int AICMD_SAY			= 14
const int AICMD_END			= 15

const int TACTICS_DUMMY			= 0
const int TACTICS_IDLE			= 1
const int TACTICS_COMBAT		= 2	//changing waypoints when target is not in LOF
const int TACTICS_COVER			= 3	//take cover
const int TACTICS_COVERCOMBAT = 4	//changing waypoints only when attacked/not covered
const int TACTICS_GUARDCOMBAT = 5	//starts as guard. Become combatant after alerted
const int TACTICS_ROAM			= 6
const int TACTICS_BERSERK		= 7	//boss, fighter

const int PATH_OK				= 1
const int PATH_LOST_TARGET	= 2
const int PATH_NOT_FOUND	= 4
const int PATH_FINISHED		= 5

const int WP_USED_ON_ENEMY = 0x80000000
const int WP_USED_OFF_ENEMY = 0x7fffffff

const string AICmdNames[16] = {
	"NONE",
	"IDLE",
	"RUN",
	"WALK",
	"ANIM",
	"USE",
	"CALL",
	"WAITSIGHT",
	"ANIMNW",
	"SLEEP",
	"TURN",
	"TURNBY",
	"SHOOT",
	"SHOOTNW",
	"SAY",
	"END"}

const string AITacticsNames[8] = {
	"TACTICS_DUMMY",
	"TACTICS_IDLE",
	"TACTICS_COMBAT",
	"TACTICS_COVER",
	"TACTICS_COVERCOMBAT",
	"TACTICS_GUARDCOMBAT",
	"TACTICS_ROAM"}

//global counter, to make it 'random' on every monster
//unfortunately useless, as we don't have various animations :-(
int CombatWalkCount = 0
int CreatureCount = 0

//idle types
const int it_default = 0
const int it_fight = 1
const int it_lost = 2

const float AI_PATHREFRESH = 2.0 //update refresh in seconds
const float AI_REFRESH = 4 //for innerloops with UpdatePath
const float AI_SLOWREFRESH = 20 //for idle-methods

const int ObservedBufferSize = 8


#include "scripts/class_creature/ai.h"

//-----------------------------------------------------------------
// TODO: celou tuhle vec predelat na dynamicky sphere triggery a notifikace bude provedena prez eventy
//-----------------------------------------------------------------
void NotifyNoises(ExtClass actor, vector origin, float radius, int noises)
{
	float lasttime
	ExtClass lastactor
	int lastnoises
	vector lastorigin
	
	int mask = noises & lastnoises
	
	if(VectorLengthSq(origin - lastorigin) < 16.0 * 16.0)
	{
		if(actor == lastactor)
		{
			if(mask <= lastnoises)
			{
				if(lasttime > currenttime - 0.5)
				{
					return
				}
			}
		}
	}

	lastactor = actor
	lastnoises = noises
	lasttime = currenttime
	lastorigin = origin

	CreatureClass mc[64], cc
	int num = SphereQuery(origin, radius, mc, 64, TFL_VISIBLE|TFL_USER6)

	mask = FL_WAIT4TRIGGER | FL_FRIENDLY

	for(int n = 0; n < num; n++)
	{
		cc = mc[n]

		if(IsInherited(cc, CCreatureClass))
		{
			if(!cc.flags & mask && cc.flags & FL_HEAR)
			{
				//velmi hrube, netestuje ani detail brushe -> temer PHS
				//TODO: real PHS
				if(P2PVisibilityEx(origin, GetOrigin(cc), TRACE_VISTEST))
					cc.NotifyNoise(actor, origin, noises)
			}
		}
	}
}

//---------------------------------------------------------------------------------
bool SpeechManager::Say(CreatureClass owner, int type)
{
	float d

	float dist = -1
	int worst

//	Print(String(owner._name + " trying to say " + itoa(type)))

	for(int n = 0; n < SPEAKERS_COUNT; n++)
	{
		if(Emit[type][n])
		{
			d = Dist[type][n]
			if(d > dist)
			{
				dist = d
				worst = n
			}
		}

		//didn't someone say this recently?
		d = currenttime - SpeakTime[type][n]
		if(d < 0.5)
			return false
	}

	vector v

	v = World.Player.CurrentPos - owner.CurrentPos

	d = v * v //distsq

	if(dist > 22000 && d < dist)
	{
		//isn't someone too far, that we can replace him?
		Dist[type][worst] = d
		Speakers[type][worst] = owner
		return true
	}

	for(n = 0; n < SPEAKERS_COUNT; n++)
	{
		if(Speakers[type][n] == NULL)
		{
			Speakers[type][n] = owner
			SpeakTime[type][n] = currenttime
			Emit[type][n] = true
			Dist[type][n] = d
			return true
		}
	}
	return false
}

//---------------------------------------------------------------------------------
void SpeechManager::Update()
{
	for(int type = 0; type < ST_SIZEOF; type++)
	{
		for(int n = 0; n < SPEAKERS_COUNT; n++)
		{
			if(Emit[type][n])
			{
				Emit[type][n] = false

				AICreatureClass cc = Speakers[type][n]
//	Print(String(cc._name + " saying " + itoa(type)))
				PlaySound(Speakers[type][n], cc.SSet.GetSound(type), SF_ONCE)
				continue
			}

			float d = currenttime - SpeakTime[type][n]
			if(d > 2)
			{
				Speakers[type][n] = NULL
			}
		}
	}
}

//---------------------------------------------------------------------------------
void info_squad::M_Activate(ExtClass other)
{
	for(local int n = 0; n < NumMembers; n++)
	{
		AICreatureClass ai = Members[n]
		if(other != ai && ai.status < STATE_DIE)
			ai.M_Activate(this)
	}
	if(!bActivated)
	{
		bActivated = true
		CallMethods(this, OnActivate)
	}
}

//---------------------------------------------------------------------------------
void info_squad::OnMemberActivate(CreatureClass cc)
{
	if(!flags & 1)
		return

	M_Activate(cc)
}

//---------------------------------------------------------------------------------
void info_squad::OnMemberKilled(CreatureClass cc)
{
	Alive--

	CallMethods(this, OnKill)

	if(Alive == 0)
	{
		CallMethods(this, OnKillOff)
		return
	}

	//if(!flags & 4)
	//{
		if(Alive == 1)
			CallMethods(this, OnLastMan)
		
//		return
	//}
	
	AICreatureClass best = NULL
	float bestdist = 9999 * 9999

	//find nearest team-mate to announce kill
	for(int n = 0; n < NumMembers; n++)
	{
		AICreatureClass ai = Members[n]
		if(ai != cc && ai.status < STATE_DIE)
		{
			vector vdist = cc.CurrentPos - ai.CurrentPos
			float dist = vdist * vdist

			if(dist < bestdist)
			{
				bestdist = dist
				best = ai
			}
		}
	}

	if(best)
	{
		if(Alive == 1)
		{
			best.Say(ST_SQUADLASTMAN)			
			CallMethods(this, OnLastMan)
		}
		else
		{
			best.Say(ST_TEAMMATEKILLED)
		}
	}
}

//---------------------------------------------------------------------------------
void info_squad::OnMemberAttacked(CreatureClass cc, CreatureClass attacker, vector pos)
{
	AICreatureClass aicc = cc

	if(bAttacked)
	{
		bAttacked = true
		aicc.Say(ST_SQUADATTACKED)
	}

	if(!flags & 2)
		return

	for(int n = 0; n < NumMembers; n++)
	{
		AICreatureClass ai = Members[n]
		if(ai != cc && ai.status < STATE_DIE)
		{
			ai.AIAddEvent(AIE_SQUADATTACKED, pos, attacker)
		}
	}
}

//---------------------------------------------------------------------------------
void info_squad::EOnLoaded(MinClass other, int extra)
{
	string str = members

	NumMembers = 0
	while(true)
	{
		string ainame

		if(ParseStringEx(str, ainame) != 3)
			break

		AICreatureClass ai = FindClass(ainame)

		if(ai == NULL)
		{
			DPrint("info_squad '" + _name + "' contains nonexistent entity '" + ainame + "'")
			ErrorNum++
		}
		else
		{
			if(!IsInherited(ai, CAICreatureClass))
			{
				DPrint("info_squad '" + _name + "' contains '" + ainame + "' of type '" + ClassName(ai) + "'")
				ErrorNum++
			}
			else
			{
				ai.Squad = this
				Members[NumMembers++] = ai
				if(NumMembers >= 64)
				{
					DPrint("info_squad '" + _name + "' has too much entities")
					ErrorNum++
					break
				}
			}
		}

		if(ParseStringEx(str, ainame) == 0)
			break

		if(ainame != ",")
		{
			DPrint("info_squad '" + _name + "' syntax error")
			ErrorNum++
			break
		}
	}

	Alive = NumMembers

	CallMethods(this, OnLoad)
}

//=================================================================================
class FacialAnimationPlayer extends MinClass
{
	vobject Anim
	vobject MorphObj
	ExtClass ActorEntity
	string id
	int hsnd
	float starttime

//-----------------------------------------------------------------
    void ApplyMorph(int bone, string name)
    {
        vector mat[4]
        float weight

        GetBoneMatrix(this, bone, mat)
        weight = mat[3][2] / 32
        SetMorphState(ActorEntity, name, weight)
/*
int lframe

if(name == "QW")// && id == "3_1_Arn0002")
{
	float tt = currenttime-starttime * 30
	
	int frame = tt
	if(frame!=lframe)
	{
		lframe=frame
		Print(String(itoa(frame) + " " + id + " " + ftoa(weight)))
	}
}
*/
    }

//-----------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		ClearEventMask(this, EV_FRAME)
		
		SetAnimSlot(this, 0, VNULL, 0, 0, NULL, 30, AF_ONCE)
		
		if(Anim)
		{
	   	ReleaseObject(Anim, true)
	   	Anim = VNULL
		}

		SetMorphState(ActorEntity, "AI", 0.0)
		SetMorphState(ActorEntity, "BMP", 0.0)
		SetMorphState(ActorEntity, "CDG", 0.0)
		SetMorphState(ActorEntity, "E", 0.0)
		SetMorphState(ActorEntity, "FV", 0.0)
		SetMorphState(ActorEntity, "O", 0.0)
		SetMorphState(ActorEntity, "QW", 0.0)
		SetMorphState(ActorEntity, "U", 0.0)
	}
	
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		BumpFrame(this, ftime, false)

		ApplyMorph(B_BrowsDown_L, "BrowsDown_L")
		ApplyMorph(B_BrowsDown_R, "BrowsDown_R")
		ApplyMorph(B_BrowsUp_L, "BrowsUp_L")
		ApplyMorph(B_BrowsUp_R, "BrowsUp_R")
		ApplyMorph(B_BrowsUpSide_L, "BrowsUpSide_L")
		ApplyMorph(B_BrowsUpSide_R, "BrowsUpSide_R")
		ApplyMorph(B_Privreni_L, "Privreni_L")
		ApplyMorph(B_Privreni_R, "Privreni_R")
		ApplyMorph(B_Angry, "Angry")
		ApplyMorph(B_Sad, "Sad")
		ApplyMorph(B_AI, "AI")
		ApplyMorph(B_BMP, "BMP")
		ApplyMorph(B_CDG, "CDG")
		ApplyMorph(B_E, "E")
		ApplyMorph(B_FV, "FV")
		ApplyMorph(B_O, "O")
		ApplyMorph(B_QW, "QW")
		ApplyMorph(B_U, "U")
	}

//-----------------------------------------------------------------
	void StopAnim()
	{
		ClearEventMask(this, EV_FRAME)

		SetMorphState(ActorEntity, "AI", 0)
		SetMorphState(ActorEntity, "BMP", 0)
		SetMorphState(ActorEntity, "CDG", 0)
		SetMorphState(ActorEntity, "E", 0)
		SetMorphState(ActorEntity, "FV", 0)
		SetMorphState(ActorEntity, "O", 0)
		SetMorphState(ActorEntity, "QW", 0)
		SetMorphState(ActorEntity, "U", 0)
	}
	
//-----------------------------------------------------------------
	bool PlayAnim(string anm, int hs)
	{
		hsnd = hs
		starttime = currenttime
		id = anm
		string lipsyncanim = "cutscene/dialogs/" + anm + ".anm"

		Anim = GetObject(lipsyncanim)
		if(Anim)
		{
			SetAnimSlot(this, 0, Anim, 0, 0, NULL, 30, AF_ONCE)
			SetEventMask(this, EV_FRAME)
		}
	}
	
//-----------------------------------------------------------------
	void FacialAnimationPlayer(MinClass actor)
	{
		ActorEntity = actor
		MorphObj = GetObject("obj/morphs.xob")
		SelectObject(this, MorphObj)
		
		SetEventMask(this, EV_ANIMEND)
	}

//-----------------------------------------------------------------
	void ~FacialAnimationPlayer()
	{
		if(Anim)
		{
	   	ReleaseObject(Anim, true)
	   	Anim = VNULL
		}
		
		SetObject(VNULL)
   	ReleaseObject(MorphObj, true)
   }
}

