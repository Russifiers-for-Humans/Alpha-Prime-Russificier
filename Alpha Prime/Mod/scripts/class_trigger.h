const int TRIGTYPE_BOX = 0
const int TRIGTYPE_CYLINDER = 1

//=================================================================================
class TriggerClass extends ExtClass
{
	vector coords
	vector mins, maxs
	float angle, angle2, angle3
	string model
	int flags
	int type				//0 box, 1 cylinder
	bool IsCreated
	
	bool IsDynamic
	string OnEnter, OnLeave
	string actors
	string Actors[16]
	int ActorsNum

#ifdef EDITOR
	void EditorOpened()
	{
		if(!IsCreated)	//
		{
			CreateTrigger(this)
			IsCreated = true
		}
		UpdateEntity(this)
	}
	
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		SetOrigin(this, Pos)
		SetAngles(this, Angs)
		UpdateMainVisualizer()
		UpdateEntity(this)
	}
#endif
	
//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
	}

	void Leave(ExtClass other, int numinsiders)
	{
	}
	
	void IsInside(MinClass other, int extra)
	{
	}

	void M_Enable(ExtClass other)
	{
		if(!IsCreated)
		{
			CreateTrigger(this)
			IsCreated = true
		}

		SetEventMask(this, EV_TOUCH)	
	}
	
	void M_Disable(ExtClass other)
	{
		if(IsCreated)
		{
			DestroyTrigger(this)
			IsCreated = 0
		}
		ClearEventMask(this, EV_TOUCH)
	}
	
//-----------------------------------------------------------------
	void EOnExtra(MinClass other, int extra)
	{
		Leave(other, extra)
		CallMethods(this, OnLeave)
	}

//-----------------------------------------------------------------
	bool IsCorrectActor(ExtClass other)
	{
		if(IsInherited(other, CExtClass))
		{
			if(ActorsNum == 0)	//FIXME este inak to bude chciet ale zatial staci
			{
				if(other.ClassType == ClassTypeCreature && other.ClassNum == ClassNumPlayer)
					return true
			}
			
			for(int n = 0; n < ActorsNum; n ++)
			{
				if(other._name == Actors[n])
					return true
				
				if(Actors[n] == "enemies" && other.ClassType == ClassTypeCreature && other.ClassNum != ClassNumPlayer)
					return true
			}
		}
		
		return false
	}

//-----------------------------------------------------------------
	void EOnTouch(MinClass other, int extra)
	{
		if(!IsCorrectActor(other))
			return
		
		vector OrgThis, OrgOther
		float rad
		float lng
		
		if(type == TRIGTYPE_CYLINDER)
		{
			vector omins, omaxs
			vector sizes
			GetBoundBox(other, omins, omaxs)
			sizes = omaxs - omins
			sizes = sizes * 0.5
			float OtherRadius = sizes[0] + sizes[1]
			OtherRadius *= 0.5								//ako radius pouzijeme priemer z XY suradnic boxu
			
			rad = maxs[0]
			OrgThis = GetCenterOfEntity(this)
			OrgOther = GetCenterOfEntity(other)
			lng = VectorLength(OrgThis - OrgOther)
			lng -= OtherRadius
		}
		
		if(type == TRIGTYPE_BOX || lng < rad)
		{
			if(AddTriggerInsider(this, other) == true)
			{
				Enter(other, extra)

#ifdef EDITOR
			if(World.EditorActive == false)
			{
#endif
				if(flags & 1)
					M_Disable(this)

#ifdef EDITOR
			}
#endif

				CallMethods(this, OnEnter)
			}
			else
				IsInside(other, extra)
		}
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		ClassType = ClassTypeTrigger
		SetEventMask(this, EV_EXTRA)
		IsDynamic = 1 - IsBrushEntity(this)

		if(IsDynamic)
		{
			SetOrigin(this, coords)
			SetAngles(this, Vector(angle3, angle, angle2)/*Angles(angle)*/)
	
			if(angle != 0 || angle2 != 0 || angle3 != 0)
				SetFlags(this, TFL_OBBCOLLISION)

			if(mins == ZeroVec)
				mins = "-16 -16 -16"
	
			if(maxs == ZeroVec)
				maxs = "16 16 16"	
				
			CreateModel(this, mins, maxs)
		}
		else
			SelectObject(this, GetObject(model))

		if(actors)
		{
			string Parms[32]
			int NumParms = ParseString(actors, Parms)
	
			for(int n = 0; n < NumParms; n += 2)
				Actors[ActorsNum++] = Parms[n]
		}

		if(flags & 2)
			return
		else
			SetEventMask(this, EV_TOUCH)

/*		if(type == 1)
			AddDCylinder(0x44ffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords, maxs[0], maxs[2] * 2)
		else
			AddDShape(SHAPE_BBOX, 0x44ffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords + mins, coords + maxs)*/
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		if(!flags & 2)
		{
			if(!IsCreated)
			{
				CreateTrigger(this)
				IsCreated = true
			}
		}
		
#ifdef EDITOR
			if(World.EditorActive == true)
			{
				if(!IsCreated)
				{
					CreateTrigger(this)
					IsCreated = true
				}	
			}
#endif

		CallMethods(this, OnLoad)
	}
	
//-----------------------------------------------------------------
	void TriggerClass()
	{
		SetEventMask(this, EV_INIT|EV_LOADED)
	}

//-----------------------------------------------------------------
	void ~TriggerClass()
	{
		if(IsCreated)
		{
			ClearEventMask(this, EV_EXTRA)
			DestroyTrigger(this)
		}

		if(IsDynamic)
			RemoveModel(this)
	}
}

//=================================================================================
class trigger_universal extends TriggerClass
{
}

class trigger_hurt extends TriggerClass
{
	float hurtdelay
	int damage
	CreatureClass Insiders[16]
	float HurtTimers[16]
	int NumInsiders
	ShotStorage HurtInfo

//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
		if(IsInherited(other, CCreatureClass) == false)
			return

		if(NumInsiders >= sizeof(Insiders))
			return
		
		Insiders[NumInsiders] = other
		HurtTimers[NumInsiders++] = 0
		CreatureClass cc = other
		cc.Shoot(HurtInfo)

		SetEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void Leave(ExtClass other, int numinsiders)
	{
		int index = -1
		int n

		for(n = 0; n < NumInsiders; n++)
		{
			if(Insiders[n] == other)
			{
				index = n
				break
			}
		}

		if(index == -1)
			return

		Insiders[index] = NULL
		HurtTimers[n] = 0
		NumInsiders--

		for(n = index; n < NumInsiders; n++)
		{
			Insiders[n] = Insiders[n + 1]
			HurtTimers[n] = HurtTimers[n + 1]
		}
		
		if(NumInsiders <= 0)
			ClearEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		CreatureClass cc

		for(int n = 0; n < NumInsiders; n++)
		{
			cc = Insiders[n]

			if(cc)
			{
				HurtTimers[n] = HurtTimers[n] + (ftime * 100)
				
				if(HurtTimers[n] >= hurtdelay)
				{
					HurtTimers[n] = 0
					cc.Shoot(HurtInfo)
				}
			}
		}
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(hurtdelay == 0)
			hurtdelay = 100
		
		if(damage == 0)
			damage = 100
			
		HurtInfo = new ShotStorage	//mozno dat do classu
		HurtInfo.HurtType = HT_HURT_TRIGGER
		HurtInfo.damage = damage
		HurtInfo.shooter = this
	}
}




//=================================================================================
class trigger_item_detector extends TriggerClass
{
	RigidBodyClass item
	string itemangles

//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(GetEventMask(this) & EV_TOUCH)
			return ACT_BLOCKED

		return ACT_DISABLE
	}

//-----------------------------------------------------------------
	bool IsCorrectActor(MinClass other)
	{
		if(IsInherited(other, CItemMisc) == false)
			return false

		if(other.ClassNum != UseClass)
			return false
			
		return true
	}

//-----------------------------------------------------------------
	void M_Clear(ExtClass other)
	{
		if(item)
		{
			delete item
			item = NULL
		}
		SetEventMask(this, EV_TOUCH)
	}

//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
		ExtClass pla = World.Player
		pla.Activate(1, this)
		ClearEventMask(this, EV_TOUCH)
		item = other
		item.RemoveBodyAndGeom()
		item.SetPickable(false)
		ClearFlags(item, TFL_FULLMATRIX)
		AddChild(this, item, 7)
		SetOrigin(item, ZeroVec)
		SetAngles(item, atov(itemangles))
		CallMethods(this, OnSuccess)
	}

	void trigger_item_detector()
	{
		SetFlags(this, TFL_SOLID | TFL_OBBCOLLISION)	//player ho musi detekovat trasovanim
	}
}



//=================================================================================
class trigger_hack_detector extends TriggerClass
{
	string hackentities
	string HackNames[8]
	int HackNamesNum
	trigger_hack_detector NextSiblingTrigger
	ExtClass HackEntities[8]
	int HackEntitiesNum

//-----------------------------------------------------------------
	void UpdateEntitiesList()
	{
		HackEntitiesNum = 0
		ExtClass ent

		for(int n = 0; n < HackNamesNum; n++)	//musime zakazdym prehladat pretoze niektore entity ako napriklad mina sa mazu takze nemusia existovat
		{
			ent = FindClass(HackNames[n])
			
			if(ent && ent.HackInterf/* && ent.HackInterf.CanHacking()*/)
				HackEntities[HackEntitiesNum++] = ent
		}
	}

//-----------------------------------------------------------------
	int GetHackAdepts(ExtClass adepts[])	//vola player ked sa rozhodne pre tento trigger
	{
		UpdateEntitiesList()

		for(int n = 0; n < HackEntitiesNum; n++)
			adepts[n] = HackEntities[n]

		return HackEntitiesNum
	}

//-----------------------------------------------------------------
	bool ContainHackableEntities()
	{
		ExtClass ent
		for(int n = 0; n < HackEntitiesNum; n++)
		{
			ent = HackEntities[n]

			if(ent.HackInterf && ent.HackInterf.CanHacking())
				return true
		}
		return false
	}
	
//get nearest corner of AABB
//-----------------------------------------------------------------
	vector GetNearestPosition(vector pos)
	{
		vector res

		if(pos[0] <= mins[0])
			res[0] = mins[0]
		else
		{
			if(pos[0] >= maxs[0])
				res[0] = maxs[0]
			else
				res[0] = pos[0]
		}
	
		if(pos[1] <= mins[1])
			res[1] = mins[1]
		else
		{
			if(pos[1] >= maxs[1])
				res[1] = maxs[1]
			else
				res[1] = pos[1]
		}

		if(pos[2] <= mins[2])
			res[2] = mins[2]
		else
		{
			if(pos[2] >= maxs[2])
				res[2] = maxs[2]
			else
				res[2] = pos[2]
		}

		return res
	}

//-----------------------------------------------------------------
	bool IsCorrectActor(MinClass other)
	{
		if(other == World.Player)
			return true
		
		return false
	}
/*
//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
		UpdateEntitiesList()
//		CallMethods(this, OnSuccess)
	}

//-----------------------------------------------------------------
	void Leave(ExtClass other, int numinsiders)
	{
		CreatureClass pla = World.Player
		pla.SetHackAdepts(NULL, 0)
//		CallMethods(this, OnSuccess)
	}
*/
//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		if(hackentities)
		{
			string Parms[16]
			int NumParms = ParseString(hackentities, Parms)

			for(int n = 0; n < NumParms; n += 2)
				HackNames[HackNamesNum++] = Parms[n]
		}
		UpdateEntitiesList()
	}

	void trigger_hack_detector()
	{
		SetEventMask(this, EV_LOADED)
	}
}



//=================================================================================
class trigger_motion extends ExtClass
{
	ExtClass Entity
	vector tmins, tmaxs
	int CleaningFrame	//pouzivaju to dvojkridlove dvere

//-----------------------------------------------------------------
	void EOnTouch(MinClass other, int extra)
	{
		if(!GetFlags(other) & TFL_TOUCHTRIGGERS)
			return
//		if(!other.IsRigidBody())
//			return

		if( AddTriggerInsider(this, other) == true)
		{
			//zavolame virtualni fci. Kazda materska entita si ji osetri po svem
			Entity.Activate(1, other)
		}
	}

//-----------------------------------------------------------------
	void trigger_motion(ExtClass ent, vector mins, vector maxs)
	{
		tmins = mins
		tmaxs = maxs

		CreateModel(this, tmins, tmaxs)
		CallMethods(this, OnLoad)

		ClassType = ClassTypeTrigger
		CreateTrigger(this)

		SetEventMask(this, EV_EXTRA)

		SetEventMask(this, EV_TOUCH)

		Entity = ent
	}

//-----------------------------------------------------------------
	void ~trigger_motion()
	{
		DestroyTrigger(this)
		RemoveModel(this)
	}
}



//=================================================================================
class trigger_teleport extends ExtClass
{
	string model
	string OnEnter, OnLeave	
	int flags
	vector Pos
	float angle
	float dist
	trigger_teleport trig2

	vector GetTracedPos(MinClass Ent)
	{
		MinClass entity = NULL
		int in1, in2
		float pla[4]
		vector start = GetCenterOfEntity(Ent) + "0 0 50"
		vector end = start + "0 0 -250"
		vector vec = start - end
		VectorNormalize(vec)
		end = end + vec
		int efl, pfl
		
		pfl = ClearFlags(World.Player, TFL_SOLID)
		efl = ClearFlags(Ent, TFL_SOLID)
		
		TraceLine(World.Player, start, end, entity, pla, in1, in2)
		
		SetFlags(World.Player, pfl)
		SetFlags(Ent, efl)
		return end
	}

	void Wait()
	{
		ActionNum = 1
		Sleep(10)
		ActionNum = 0
	}
//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
		if(other.ClassNum != ClassNumPlayer)
			return
		
		if(trig2)
		{
			if(trig2.ActionNum > 0)
				return

			thread Wait()
			vector vec = Angles2Vector(Angles(trig2.angle))
			vec = vec * dist
			vec = Pos + vec
			SetOrigin(World.Player, vec)
			CreatureClass cc = World.Player
			cc.ChangeYaw(trig2.angle)
			SetVelocity(World.Player, ZeroVec)
			UpdateEntity(World.Player)
		}
	}

	void Leave(ExtClass other, int numinsiders)
	{
	}
	
	void IsInside(MinClass other, int extra)
	{
	}

	void M_Enable(ExtClass other)
	{
		SetEventMask(this, EV_TOUCH)	
	}
	
	void M_Disable(ExtClass other)
	{
		ClearEventMask(this, EV_TOUCH)	
	}
	
//-----------------------------------------------------------------
	void EOnExtra(MinClass other, int extra)
	{
		CallMethods(this, OnLeave)
		Leave(other, extra)
		trig2 = FindClass(link)
	}
	
//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		trig2 = FindClass(link)
		
		if(trig2)
		{
			Pos = GetTracedPos(trig2)//GetCenterOfEntity(trig2)
			Throw(trig2, this, EV_EXTRA, 0)	//nutne koli editoru
		}
		CallMethods(this, OnLoad)
	}
	
//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		ClassType = ClassTypeTrigger

		SetEventMask(this, EV_EXTRA)
		SetEventMask(this, EV_TOUCH)

		SelectObject(this, GetObject(model))	
		SetEventMask(this, EV_LOADED)
	}
//-----------------------------------------------------------------
	void EOnTouch(MinClass other, int extra)
	{
		if(flags & 1)
		{
			if(other.ClassType != ClassTypeCreature)
				return
			if(other.ClassNum != ClassNumPlayer)
				return
		}

		if( AddTriggerInsider(this,other) == true)
		{
			CallMethods(this, OnEnter)
			Enter(other, extra)
		}
		else
			IsInside(other, extra)
	}

//-----------------------------------------------------------------
	void trigger_teleport()
	{
		CreateTrigger(this)
	}

//-----------------------------------------------------------------
	void ~trigger_teleport()
	{
		DestroyTrigger(this)
	}
}





//=================================================================================
class trigger_motion2 extends ExtClass
{
	ExtClass Owner
	vector tmins, tmaxs

//-----------------------------------------------------------------
	void EOnTouch(MinClass other, int extra)
	{
		if(!IsInherited(other, CRigidBodyClass) && !IsInherited(other, CCreatureClass))
			return

		Owner.Activate(1, other)
	}

//-----------------------------------------------------------------
	void trigger_motion2(ExtClass owner, vector mins, vector maxs)
	{
		Owner = owner
		tmins = mins
		tmaxs = maxs

		ClassType = ClassTypeTrigger		
		SetEventMask(this, EV_TOUCH | EV_EXTRA)
		CreateModel(this, tmins, tmaxs)
		CreateTrigger(this)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void ~trigger_motion2()
	{
		DestroyTrigger(this)
		RemoveModel(this)
	}
}

//-----------------------------------------------------------------
class trigger_look extends ExtClass
{
	int flags
	float lookangle
	float lookdist
	vector coords
	string OnLook
	bool enabled

//-----------------------------------------------------------------
	void M_Enable(ExtClass other)
	{
		enabled = true
	}

//-----------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		enabled = false
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		vector dir = GetMatrix(this, 3) - World.CameraPos
		float dist = VectorNormalize(dir)
		float dot = World.CameraNorm * dir
		clamp dot<-1, 1>
		float ang = acos(dot) * RAD2DEG

		if(ang < lookangle)	//vidime ho
		{
			if(lookdist && dist > lookdist)
				return 0

			if(!enabled)
				return 0

			ClearEventMask(this, EV_VISIBLE)
			
			if(!flags & 1)		//once
				SetEventMask(this, EV_NOTVISIBLE)

			CallMethods(this, OnLook)
		}
		return 0
	}

//-----------------------------------------------------------------
	void EOnNotVisible(MinClass other, int extra)
	{
		ClearEventMask(this, EV_NOTVISIBLE)
		SetEventMask(this, EV_VISIBLE)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	int EOnInit(MinClass other, int extra)
	{
		if(lookangle == 0)
			lookangle = 30
		
		SetOrigin(this, coords)
	}

//-----------------------------------------------------------------
	void trigger_look()
	{
		SetFlags(this, TFL_VISIBLE)
		SetEventMask(this, EV_VISIBLE)
	}
}


//=================================================================
class trigger_push extends TriggerClass
{
	int NumInsiders
	float power

//-----------------------------------------------------------------
	void M_SetPower(ExtClass other)
	{
		power = atof(other._value)
		
		if(power < 0)
			power = 0
	}

//-----------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		if(IsCreated)
		{
			DestroyTrigger(this)
			IsCreated = 0
		}
		ClearEventMask(this, EV_TOUCH|EV_FRAME)
	}
/*
//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
//		SetEventMask(this, EV_FRAME)
	}*/

//-----------------------------------------------------------------
	void Enter(ExtClass other, int numinsiders)
	{
		M_Enable(this)
	}

//-----------------------------------------------------------------
	void Leave(ExtClass other, int numinsiders)
	{
		if(numinsiders == 0)
			ClearEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	bool IsCorrectActor(MinClass other)
	{
		if(other.IsRigidBody())
			return true

		return false
	}

//-----------------------------------------------------------------
	void EOnTouch(MinClass other, int extra)
	{
		if(!IsCorrectActor(other))
			return

		SetEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		MinClass Insiders[32]
		MinClass Insider
		NumInsiders = TriggerInsiders(this, Insiders)
		vector ForceDir = GetMatrix(this, 0)
		
		for(int n = 0; n < NumInsiders; n++)
		{
			Insider = Insiders[n]

			if(Insider && Insider.IsRigidBody())
				dBodyApplyForce(Insider, ForceDir * power)
		}
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(power == 0)
			power = 2000

		ClearEventMask(this, EV_FRAME)
	}
}
