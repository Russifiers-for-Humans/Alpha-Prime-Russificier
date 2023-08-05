
//=================================================================================
class func_terrain extends ExtClass
{
	string model
	
	void func_terrain()
	{
		SetFlags(this, TFL_VISIBLE)
		SetObject(GetObject(model), "")
	}
}

//=================================================================================
class DoorClass extends FuncClass
{
	float speed
	float wait
	string baseanim
	string beforeopenanim
	string aftercloseanim
	string startsound
	string endsound
	string openingsound
	string startsound2
	string endsound2

	DoorClass ParentDoor				//parent u dvojkridlovych dveri
	trigger_motion MotionTrigger	//interny trigger
	float WayLength, timer
	float ReverseOpenDir				//managuje smer otvarania
	vector FaceNormOnClosed			//kolmica na plochu dveri v zavretej polohe
	string OnOpen, OnOpened, OnClose, OnClosed
	vobject BeforeOpenAnim
	vobject AfterCloseAnim
	vobject BaseAnim
	hsound Sounds[5]
	int LoopSound
	string physparent
	ExtClass PhysParent
	dJoint FixJoint
	float ClosingProblemTime

#ifdef EDITOR
	void EditorOpened()
	{
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		vector Angs = Vector(0, angle, 0)
		SetTransformEx(Angs, coords)
		UpdateEntity(this)
	}
#endif

	void UpdateAgent(bool act)
	{
		//we don't want to place pathengine agents here!!!
	}
	
//---------------------------------------------------------------------------------
	void M_SetWait(ExtClass other)
	{
		wait = atof(other._value)
	}

//---------------------------------------------------------------------------------
	void FixCurrentPosition(bool fix)
	{
		if(fix)
		{
			if(PhysParent == World)
				ActivateBody(false)
			else
			{
				if(!FixJoint)
					FixJoint = dJointCreateFixed(PhysParent, this, GetMatrix(this, 3), true, 0)

				ActivateBody(true)
			}
		}
		else
		{
			if(PhysParent != World)
			{
				if(FixJoint)
				{
					dJointDestroy(FixJoint)
					FixJoint = 0
				}
			}
			ActivateBody(true)
		}
	}

//---------------------------------------------------------------------------------
	int GetFace(MinClass ent)	//zisti na ktorej strane dveri sme
	{
		vector vec = GetMatrix(ent, 3) - coords
		
		float dot = vec * FaceNormOnClosed
		
		if(dot > 0)
			return 0
			
		return 1
	}

//---------------------------------------------------------------------------------
	vector GetFaceNorm()
	{
		return GetMatrix(this, 1)	//normala kolma na plochu dveri
	}

	//---------------------------------------------------------------------------------
	void SetOpenDir(MinClass actor)
	{
		int FacePos = GetFace(actor)
		
		if(FacePos)
			ReverseOpenDir = -1
		else
			ReverseOpenDir = 1
	}

//---------------------------------------------------------------------------------
	bool Opening()
	{
	}

//---------------------------------------------------------------------------------
	bool Closing()
	{
	}
	
//---------------------------------------------------------------------------------
	void BeforeOpen()
	{
	}

//---------------------------------------------------------------------------------
	void BeforeClose()
	{
	}

//----------------------------------------------------------------------------
	void AfterClosed()
	{
	}

//----------------------------------------------------------------------------
	void CreateJoints()
	{
	}

//----------------------------------------------------------------------------
	void DestroyJoints()
	{
	}

//----------------------------------------------------------------------------
	void Open()
	{
		ActionNum = 1
		if(BeforeOpenAnim && status == 0)
		{
			SetAnimSlot(this, 0, BeforeOpenAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
			SetStatus(-1)
			SetEventMask(this, EV_FRAME)
			PlaySound(this, Sounds[0], SF_ONCE)
			CallMethods(this, OnOpen)
			return
		}
		
		BeforeOpen()
		SetStatus(1)
		FixCurrentPosition(false)
		ClearFlags(this, TFL_AREACLOSED)
		
		if(status == 0)	//uz sme to pustili na zaciatku animacie
			PlaySound(this, Sounds[0], SF_ONCE)
		
		if(!LoopSound)
			LoopSound = PlaySound(this, Sounds[2], 0)

		SetEventMask(this, EV_FRAME)
		CallMethods(this, OnOpen)
	}

//----------------------------------------------------------------------------
	void Opened()
	{
		ActionNum = 0
		SetStatus(3)
		timer = wait	//bude sa odpocitavat timer
		
		FixCurrentPosition(true)

		PlaySound(this, Sounds[1], SF_ONCE)
		EndSound(LoopSound)
		LoopSound = 0
		
		if(wait == -1)	//pocas waitingu musi bezat onframe
			ClearEventMask(this, EV_FRAME)

		CallMethods(this, OnOpened)
	}
	
//----------------------------------------------------------------------------
	void Close()
	{
		if(status == 2)
			return

		ActionNum = 1
		BeforeClose()
		SetStatus(2)
		FixCurrentPosition(false)
		PlaySound(this, Sounds[3], SF_ONCE)
		ClosingProblemTime = 0
			
		SetEventMask(this, EV_FRAME)
		CallMethods(this, OnClose)
	}
	
//----------------------------------------------------------------------------
	void Closed()
	{
		ActionNum = 0
		SetStatus(0)

		FixCurrentPosition(true)

		SetFlags(this, TFL_AREACLOSED)
		EndSound(LoopSound)
		LoopSound = 0
		
		if(AfterCloseAnim)
		{
			SetAnimSlot(this, 0, AfterCloseAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
			PlaySound(this, Sounds[4], SF_ONCE)
			SetStatus(-2)
			return
		}

		ClearEventMask(this, EV_FRAME)
		PlaySound(this, Sounds[4], SF_ONCE)
		CallMethods(this, OnClosed)
		AfterClosed()
	}

//---------------------------------------------------------------------------------
	void M_Open(ExtClass other)
	{
		Activate(0, other)	//nevolame priamo Open() pretoze v activate mame funkcnosti na otvorenie susednych dveri + podmienky na animacie atd.
	}

//---------------------------------------------------------------------------------
	void M_Close(ExtClass other)
	{
		Close()
	}

//-----------------------------------------------------------------
	MinClass CreatureInTrigger()
	{
		if(MotionTrigger)
		{
			MinClass insiders[32]
			
			int count = TriggerInsiders(MotionTrigger, insiders)

			for(int n = 0; n < count; n++)
			{
				MinClass cc = insiders[n]

				if(cc.ClassType != ClassTypeCreature)
					continue
					
				if(cc.status == STATE_DEAD)
					continue

				return cc
			}
		}
		return NULL
	}

//-----------------------------------------------------------------
	bool Waiting()
	{
		//pokud je wait -1, tak cekame porad
		if(wait > 0)
		{
			if(CreatureInTrigger())
			{
				timer = wait
				return false	
			}

			timer -= ftime * 100
		}
		else
		{
			FixCurrentPosition(true)
//			ClearEventMask(this, EV_FRAME)
			return false
		}

		if(timer <= 0)//hotovo
		{
			if(ParentDoor)
			{
				if(ParentDoor.timer <= 0)	//pokial susedne dvere uz nemaju nikoho v triggeri mozeme sa zavriet inac by na seba cakali navzajom do nekonecna
					return true
			}
			else
				return true
		}

		return false
	}

//----------------------------------------------------------------------------
	void CreateInternalTrigger()
	{
//vytvorime interny trigger na otvaranie/zatvaranie
		vector ext = ZeroVec
		ext[0] = DoorTriggerOverrunSide	//presah do strany
		ext[1] = DoorTriggerOverrunFront	//presah dopredu/dozadu

		vector tmins = mins - ext
		vector tmaxs = maxs + ext
		
		bool NeedOBB = true

		if(PhysParent == World)
		{		
			if(angle == 0 || angle == 90 || angle == 180 || angle == 270 || angle == 360)
				NeedOBB = false
				
			if(angle == -90 || angle == -180 || angle == -270 || angle == -360)
				NeedOBB = false
		}
		
		if(NeedOBB == false)	//optimalizacia aby sa zbytocne nevytvaral trigger s OBB_COLLISION
		{
			tmins = VectorMatrixMultiply3(Matrix, tmins)
			tmaxs = VectorMatrixMultiply3(Matrix, tmaxs)
	
			float f = tmins[0]
			
			if(f > tmaxs[0])
			{
				tmins[0] = tmaxs[0]
				tmaxs[0] = f
			}
			
			f = tmins[1]
			
			if(f > tmaxs[1])
			{
				tmins[1] = tmaxs[1]
				tmaxs[1] = f
			}
			
			f = tmins[2]
			
			if(f > tmaxs[2])
			{
				tmins[2] = tmaxs[2]
				tmaxs[2] = f
			}						
			
			mins = VectorMatrixMultiply3(Matrix, mins)	//pretransformujeme aj toto aby sme mai pri hladani sparovanych dveri skutocne BBoxy
			maxs = VectorMatrixMultiply3(Matrix, maxs)
	
			f = mins[0]
			
			if(f > maxs[0])
			{
				mins[0] = maxs[0]
				maxs[0] = f
			}
			
			f = mins[1]
			
			if(f > maxs[1])
			{
				mins[1] = maxs[1]
				maxs[1] = f
			}
			
			f = mins[2]
			
			if(f > maxs[2])
			{
				mins[2] = maxs[2]
				maxs[2] = f
			}
		}

		MotionTrigger = new trigger_motion(this, tmins, tmaxs)
		
//		mins = coords + mins	//budu sa podla toho hladat sparovane dvere
//		maxs = coords + maxs
		
		if(NeedOBB)
		{
			SetAngles(MotionTrigger, Vector(0, angle, 0))	//pri zapornych hodnotach angle bude trigger akoby o polovicu mensi. mozno bug
			SetFlags(MotionTrigger, TFL_OBBCOLLISION)
//			Shape dshape = AddDShape(SHAPE_BBOX, 0x2200ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, tmins, tmaxs)
//			dshape.SetMatrix(Matrix)
		}
//		else
//			AddDShape(SHAPE_BBOX, 0x2200ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords + tmins, coords + tmaxs)	

		if(PhysParent != World)
			AddChild(this, MotionTrigger, 0)
		else
			SetOrigin(MotionTrigger, coords)
	}

//----------------------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
//		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, StartPos, EndPos)
		if(status < 0)	//bezi animacka
			BumpFrame(this, ftime, 0)
						
		if(status == 0)
		{
			FixCurrentPosition(true)
			return
		}

		if(status == 1)	//opening
		{
			ActivateBody(true)
			
			if(Opening() == true)
				Opened()

			return
		}
			
		if(status == 2)	//closing
		{
			ActivateBody(true)
			
			if(Closing() == true)
				Closed()
				
			return
		}
		
		if(status == 3)	//waiting
		{
			if(Waiting() == true)
				Close()
		}
	}

//-----------------------------------------------------------------
	void SearchNeighbourDoors()
	{
		DoorClass ec
		vector ext = "1 1 1"	//k presnym bboxom pridame iba minimum aby sme omylom nenasli ine dvere

		//najdi vsechny dotykajici se entity, ktere chteji byt slinkovany
		for(ec = GetNextEntity(NULL, CDoorClass); ec != NULL; ec = GetNextEntity(ec, CDoorClass))
		{
			if(ec != this && ec.ClassNum == ClassNum && !ec.flags & 32)	//nesmieme sparovat dvere s flagom 32
			{
				if(CheckBoundBox(coords + mins - ext, coords + maxs + ext, ec.coords + ec.mins - ext, ec.coords + ec.maxs + ext) == true)
				{
					if(ParentDoor == NULL)
						ParentDoor = ec

					if(ec.ParentDoor == NULL)
						ec.ParentDoor = this
				}
			}
		}		
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		if(!flags & 32)			//nesmu byt sparovane s inymi dverami
			SearchNeighbourDoors()

		if(baseanim)
		{
			BaseAnim = GetObject(beforeopenanim)
			SetAnimSlot(this, 0, BaseAnim, 0, 0, NULL, 30, AF_RESET)	
		}
		
		if(beforeopenanim)
			BeforeOpenAnim = GetObject(beforeopenanim)
			
		if(aftercloseanim)
			AfterCloseAnim = GetObject(aftercloseanim)
			
		if(BeforeOpenAnim || AfterCloseAnim)
			SetEventMask(this, EV_ANIMEND)

		UpdateEntity(this)
		FaceNormOnClosed = GetMatrix(this, 1)
		CallMethods(this, OnLoad)
	}

//----------------------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		SetAnimSlot(this, 0, BaseAnim, 0, 0, NULL, 30, AF_RESET)	

		if(status == -1)
		{
			Open()
			
			if(ParentDoor && !ParentDoor.BeforeOpenAnim)	//pokial susedne dvere nemali animacku tak  sa otvaraju az teraz
				ParentDoor.Open()
				
			return
		}
		
		if(status == -2)	//tato animacka bola len pre oko ale status musime vynulovat
		{
			SetStatus(0)
			CallMethods(this, OnClosed)
		}
	}

//----------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		PhysParent = FindClass(physparent)
		
		if(!PhysParent)
			PhysParent = World
		else
			AddToParent(PhysParent)	//sme zavisly na tejto entite
			
		if(wait == 0)	
			wait = 200
			
		if(speed == 0)	
			speed = 200
			
		if(!flags & 1)
			SetFlags(this, TFL_AREACLOSED)
		
		if(startsound)
			Sounds[0] = GetSoundEx(startsound, 1, SF_3D)			//dvere sa zacinaju otvarat

		if(endsound)
			Sounds[1] = GetSoundEx(endsound, 1, SF_3D)			//dvere sa otvorili (dorazili na koniec)

		if(openingsound)
			Sounds[2] = GetSoundEx(openingsound, 1, SF_3D)		//lool. hraje ked sa otvaraju aj ked sa zatvaraju

		if(startsound2)
			Sounds[3] = GetSoundEx(startsound2, 1, SF_3D)		//ked sa zacinaju zatvarat

		if(endsound2)
			Sounds[4] = GetSoundEx(endsound2, 1, SF_3D)			//ked sa zavreli
			
	}

//----------------------------------------------------------------------------
	void DoorClass()
	{
		ReverseOpenDir = 1	//default direction
		SetFlags(this, TFL_AREASPLITTER | TFL_SOLID | TFL_OBBCOLLISION)
		SetPickable(false)
	}

//----------------------------------------------------------------------------
	void OnParentRemove()
	{
		DestroyJoints()	//prejointujeme sa na world
		PhysParent = World
		CreateJoints()
	}

//----------------------------------------------------------------------------
	void ~DoorClass()
	{
		DestroyJoints()

		if(MotionTrigger)
		{
			delete MotionTrigger
			MotionTrigger = NULL
		}
		RemoveFromParent()
	}
}


class func_door extends DoorClass
{
	float dirangle				//ma vecsiu prioritu ako angle. pokial pouzito tak urcuje smer pohybu
	float dist					//o kolko maju presahovat cez zarubne
	vector direction			//smer pohybu
	vector StartPos, EndPos, CurPos
	vector StartPosLocal, EndPosLocal, DirectionLocal
	dJoint JointSliding, JointLinearLimit, JointAngularLimit

#ifdef EDITOR
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		SetTransformEx(Angs, Pos)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}

	void CreateEditorVisuals()
	{
	}
#endif

//---------------------------------------------------------------------------------
	void M_SetDist(ExtClass other)
	{
		if(status != 0)	//jointy setupujeme iba v zakladnej poze
			return

		float NewDist = atof(other._value)
		float dif = NewDist - dist
		WayLength -= dif		//preshah zarubni
		dist = NewDist

		StartPos = coords
		EndPos = direction * WayLength
		EndPos = coords + EndPos

		float HalfLength = WayLength * 0.5
		vector JointPos = coords

		if(JointLinearLimit)
		{
			dJointDestroy(JointLinearLimit)
			JointLinearLimit = 0
		}
				
		JointLinearLimit = dJointCreateLinearLimit(World, JointPos, this, JointPos + (direction * -HalfLength), HalfLength, 0)
		
		StartPosLocal = PhysParent.CoordToLocal(StartPos)
		EndPosLocal = PhysParent.CoordToLocal(EndPos)
	}

//---------------------------------------------------------------------------------
	void SetGravityDir(float OpenDir)
	{
		if(direction[2] != 1 && direction[2] != -1)
			return

		float Gravity = -9.8 * ReverseOpenDir
		
		if(OpenDir != 1)
			Gravity = -Gravity
			
		vector GravityDir = direction * Gravity
		dBodySetGravity(this, GravityDir)
	}

//---------------------------------------------------------------------------------
	void CleaningArea(vector MovementDir, float ClosingProgress)
	{
		if(!MotionTrigger)
			return

		if(MotionTrigger.CleaningFrame == World.ActualFrame)	//u dvojkridlovych dveri to budu robit iba jedny z nich
			return

		MotionTrigger.CleaningFrame = World.ActualFrame
		vector mins, maxs, InsMins, InsMaxs
		GetCBoundBox(this, mins, maxs)
		vector offset = MovementDir * ClosingProgress * -WayLength
		mins = mins + offset
		maxs = maxs + offset
		mins = mins - "8 8 8"
		maxs = maxs + "8 8 8"

//		AddDShape(SHAPE_BBOX, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, mins, maxs)

		vector MyCenter = maxs - mins
		MyCenter = MyCenter * 0.5
		MyCenter = mins + MyCenter
		vector FaceNorm = GetFaceNorm()	//default
		vector CleaningDir
		MinClass insiders[32]
		MinClass insider
		float dot
		vector off
		int count = TriggerInsiders(MotionTrigger, insiders)
		bool CleaningDirIsKnown = false
		int n
		
		//vytah medzi tymi entitami bohuzial neni pretoze je deaktivovany
/*		for(n = 0; n < count; n++)	//zistime ci medzi entitami neni vytah. ak ano tak potom je CleaningDir jasny
		{
			insider = insiders[n]

			if(ClassName(insiders[n]) == "func_plat")
			{
				vector pos = GetMatrix(insiders[n], 3)
				pos[2] = MyCenter[2]
				off = pos - MyCenter
				dot = off * FaceNorm

				if(dot < 0)
					CleaningDir = -FaceNorm
				else
					CleaningDir = FaceNorm

				CleaningDirIsKnown = true
				break	
			}
		}*/

		for(n = 0; n < count; n++)
		{
			insider = insiders[n]
			//Print(insider)

			if(insider == World.Player)
				continue

			if(insider == this)
				continue

			if(ParentDoor && insider == ParentDoor)
				continue

			bool IsRigidBody = insider.IsRigidBody()
			bool IsRagdoll = false

			if(!IsRigidBody)
			{
				if(IsInherited(insider, CCreatureClass))
				{
					CreatureClass cc = insider

					if(!cc.ragdollActive)
						continue
					
					IsRagdoll = true
				}
				else
					continue
			}
			else
			{			
				if(VectorLength(GetVelocity(insider)) > 100)	//nemiesame sa do toho co sa rychle pohybuje
					continue
			}
			
			GetCBoundBox(insider, InsMins, InsMaxs)

			if(!CheckBoundBox(mins, maxs, InsMins, InsMaxs))
				continue

			if(!CleaningDirIsKnown)	//ak este neni jasne na ktoru stranu to je potrebne vytlacit tak vyberieme stranu na ktoru sme blizsie
			{
				vector center = InsMaxs - InsMins
				center = center * 0.5
				center = InsMins + center
				off = center - MyCenter
				dot = off * FaceNorm

				if(dot < 0)
					CleaningDir = -FaceNorm
				else
					CleaningDir = FaceNorm
			}

			float ForceScale
			vector CleaningForce

			if(IsRagdoll)
			{
				CreatureClass rag = insider
				
				for(int b = 0; b < sizeof(rag.RBones); b++)
				{
					RBone CurBone = rag.RBones[b]

					if(CurBone)
					{
						vector BonePos = GetMatrix(CurBone, 3)
						InsMins = BonePos - "8 8 8"
						InsMaxs = BonePos + "8 8 8"

						if(!CheckBoundBox(mins, maxs, InsMins, InsMaxs))
							continue

						ForceScale = 50 * dBodyGetMass(CurBone)
						CleaningForce = CleaningDir * ForceScale
						dBodyApplyImpulse(CurBone, CleaningForce)
					}
				}
			}
			else
			{
				ForceScale = 100 * dBodyGetMass(insider)
				CleaningForce = CleaningDir * ForceScale
				dBodyApplyForce(insider, CleaningForce)	
			}
		}
	}

//---------------------------------------------------------------------------------
	void BeforeOpen()
	{
		SetGravityDir(1)
	}
	
//---------------------------------------------------------------------------------
	void BeforeClose()
	{
		SetGravityDir(-1)
	}
	
//---------------------------------------------------------------------------------
	bool Opening()
	{
		CurPos = GetMatrix(this, 3)
		
		vector CurPosLocal = PhysParent.CoordToLocal(CurPos)
		float FromStart = VectorLength(StartPosLocal - CurPosLocal)
		vector MoveDirection = PhysParent.VectorToParent(DirectionLocal)
		vector ParentMat[4]
		
		if(FromStart < WayLength)
		{
			dBodyApplyForce(this, MoveDirection * speed)
			return false
		}

		vector RealEndPos = PhysParent.CoordToParent(EndPosLocal)
		vector mat[4]
		GetMatrix4(this, mat)
		mat[3] = RealEndPos//EndPos
		SetTransform(mat)	
		return true
	}

//---------------------------------------------------------------------------------
	bool Closing()
	{
		CurPos = GetMatrix(this, 3)
		vector CurPosLocal = PhysParent.CoordToLocal(CurPos)
		float FromEnd = VectorLength(EndPosLocal - CurPosLocal)
		vector MoveDirection = PhysParent.VectorToParent(DirectionLocal)
		vector ParentMat[4]

		if(FromEnd < WayLength)
		{
			float WayRate = FromEnd / WayLength
			clamp WayRate<0, 1>
			WayRate = 1 - WayRate
			CleaningArea(MoveDirection, WayRate)
/*			float Vel = VectorLength(GetVelocity(this))	//nemoze byt pouzite na vsetkych dverach pretoze na OnClose moze visiet dolezita akcia

			if(Vel < 20)
			{
				ClosingProblemTime += ftime

				if(ClosingProblemTime > 2)
				{
					ClosingProblemTime = 0
					SetStatus(1)
					return false
				}
			}*/

			vector Force = MoveDirection * -speed
			dBodyApplyForce(this, Force)
			return false
		}
		
		vector RealStartPos = PhysParent.CoordToParent(StartPosLocal)
		vector mat[4]
		GetMatrix4(this, mat)
		mat[3] = RealStartPos//StartPos
		SetTransform(mat)
		return true
	}

//-----------------------------------------------------------------
	void Activate(int flag, MinClass other) //implementace virtualni funkce
	{
		if(flag == 1) //zavolal to interny trigger
		{
			if(flags & DisableFuncFlag)
				return

			if(!flags & 16)	//musi mat AutoOpen flag
				return

			if(other.ClassType != ClassTypeCreature)	//interny trigger detekuje vsetko fyzikalne koli cleaningu ale dvere otvaraju iba potvory a player
				return
	
			if(other.ClassNum != ClassNumPlayer)
				return
		}	

		if(status != 0 && status != 2)	//zaroven je to ochrana proti rekurzivnemu volaniu medzi dverami!
			return

		if(flag == 1 && other.ClassNum == ClassNumPlayer)
		{
			NotifyNoises(other, GetMatrix(other, 3), NOISERANGE_PHYSICS, NOISE_PHYSICS)
		}
		
		if(!BeforeOpenAnim && ParentDoor && ParentDoor.BeforeOpenAnim)	//jediny pripad kedy nevolame open
			status = status
		else
			Open()
			
		if(ParentDoor != NULL)
		{
			bool ParentOpen = false	//ked maju obidvoje dvere animacku alebo obidvoje nemaju tak mozeme aktivovat druhe dvere. inac budu druhe dvere 
		
			if(BeforeOpenAnim && ParentDoor.BeforeOpenAnim)
				ParentOpen = true
				
			if(!BeforeOpenAnim && !ParentDoor.BeforeOpenAnim)
				ParentOpen = true
				
			if(BeforeOpenAnim && !ParentDoor.BeforeOpenAnim)
				ParentOpen = false
				
			if(!BeforeOpenAnim && ParentDoor.BeforeOpenAnim)
				ParentOpen = true
				
			if(ParentDoor.status != 0 && ParentDoor.status != 2)	//aby si nevolali Activate rekurzivne!
				ParentOpen = false
				
			if(ParentOpen)
				ParentDoor.Activate(0, other)
		}
	}

//---------------------------------------------------------------------------------
	void EOnJointBreak(class other, int extra)	//ked sa utrhne joint. volane obidvom spojenym body
	{
		if(extra == JointSliding || extra == JointLinearLimit)
		{
			if(JointSliding)
				dJointDestroy(JointSliding)

			if(JointAngularLimit)
				dJointDestroy(JointAngularLimit)
				
			if(JointLinearLimit)
				dJointDestroy(JointLinearLimit)
			
			JointSliding = 0
			JointLinearLimit = 0
			JointAngularLimit = 0
			
			dBodySetMass(this, ZeroVec, mass)	//Defaultny inertia
			ClearEventMask(this, EV_FRAME)
			SetStatus(0)
		}
	}

//-----------------------------------------------------------------
/*	void EOnContact(MinClass other, Contact c)
	{
		vector Cpos = c.Position
		vector Pos = GetMatrix(this, 3)
		Cpos[2] = Pos[2]
		vector vec2 = Pos - Cpos
		VectorNormalize(vec2)
		float dot = fabs(GetMatrix(this, 1) * vec2) //normala po ktorej jazdia dvere
		
		if(dot < 0.05)	//je to dotyk v smere pohybu dveri
		{
			if(status == 2)
			{
				SetStatus(1)
				PlaySound(this, Sounds[3], SF_ONCE)
			}
		}
	}*/

//----------------------------------------------------------------------------
	void CreateJoints()
	{
		if(rb)
		{
			vector JointPos = coords// + (direction * WayLength)

			if(!JointSliding)
				JointSliding = dJointCreateSliding(PhysParent, this, JointPos, direction, false, 0)	//tu uz nema nekonecny inertia takze musime zaistit aby sa to nepretacalo

			if(PhysParent != World)
			{
				if(!JointAngularLimit)
				{
					vector axis = GetMatrix(this, 2)
					JointAngularLimit = dJointCreateAngularLimit(PhysParent, axis, this, axis, 0, 0)
				}
			}
				
			float HalfLength = WayLength * 0.5
			JointLinearLimit = dJointCreateLinearLimit(PhysParent, JointPos, this, JointPos + (direction * -HalfLength), HalfLength, 0)
		}		
	}

//----------------------------------------------------------------------------
	void DestroyJoints()
	{
		if(FixJoint)
		{
			dJointDestroy(FixJoint)
			FixJoint = 0
		}

		if(JointSliding)
		{
			dJointDestroy(JointSliding)
			JointSliding = 0
		}

		if(JointAngularLimit)
		{
			dJointDestroy(JointAngularLimit)
			JointAngularLimit = 0	
		}

		if(JointLinearLimit)
		{
			dJointDestroy(JointLinearLimit)
			JointLinearLimit = 0
		}
	}

//----------------------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CreateJoints()
		
		StartPosLocal = PhysParent.CoordToLocal(StartPos)
		EndPosLocal = PhysParent.CoordToLocal(EndPos)
		DirectionLocal = PhysParent.VectorToLocal(direction)
		
		if(PhysParent != World && rb)
			FixCurrentPosition(true)
	}

//----------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(!model)
			model = "obj/debug/dvere_OriginDoluVRohu.xob"
			
		MainObject = GetObject(model)
		SetObject(MainObject, _materialremap)

		GetBoundBox(this, mins, maxs)
		vector sizes = maxs - mins

		if(dirangle != 0)	//ma vecsiu prioritu ako angle
			direction = GetDirectionFromAngle(dirangle)
		else
			direction = GetDirectionFromAngle(angle)

		if(dirangle == -1 || dirangle == -2)
			WayLength = sizes[2]	//pre pohyb hore a dole to je jasne
		else
			WayLength = sizes[0]	//toto je osa po ktorej jazdia dvere. fixna dohoda ze tak budu vymodelovane
		
		if(dist == 0)
			dist = 2

		if(speed == 0)
			speed = 200

		if(WayLength < dist)				//stale sa niekto cuduje ze sa mu to chova divne ked tam vlozi nezmysel
			dist = 0
			
		WayLength -= dist		//preshah zarubni

		StartPos = coords
		EndPos = direction * WayLength
		EndPos = coords + EndPos
//		AddDShape(SHAPE_BBOX, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, StartPos - "4 4 4", StartPos + "4 4 4")
//		AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, EndPos - "4 4 4", EndPos + "4 4 4")

		vector mat[4]
		RollPitchYawMatrix2(Vector(0, angle, 0), mat)
		mat[3] = coords
		SetTransform(mat)
		SetOrigin(this, coords)
		
		if(mass == 0)
			SetMass(40)

		if(CreateRigidBody(GEOMTYPE_NONE, "", model) == true)
		{
			dBodyInteractionLayer(this, ET_DOOR)
			
			if(PhysParent == World)
				dBodySetInertiaTensor(this, ZeroVec, ZeroVec)

			CreateInternalTrigger()
			
			dBodyCreatePhysXShadow(this, 2, true)
		}
             

		
		if(display && !Display)
		{
			Display = new func_display(this, display, false)	//musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)

			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
		ClearFlags(this, TFL_TOUCHTRIGGERS)	//clearujeme po tom co sa v rigidbodyclasse zaplo v CreateRigidBody()
	}

//----------------------------------------------------------------------------
	void func_door()
	{
		ClassNum = ClassNumDoor
		Show(true)
		SetEventMask(this, EV_JOINTBREAK|EV_INIT)
	}

//----------------------------------------------------------------------------
	void ~func_door()
	{
		DestroyJoints()		
		RemoveBodyAndGeom()
		
		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}


//=================================================================================
class func_door_rot extends DoorClass
{
	int axis
	float openingangle
	vector StartAxis, EndAxis
	int StartAxisIndex	//ktora osa matice bola pouzita pre StartAxis
	dJoint JointHinge, JointAngLimit

#ifdef EDITOR
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		vector angs = ZeroVec
		angs[axis] = Angs[1]
		SetTransformEx(angs, Pos)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}

	void CreateEditorVisuals()
	{
	}
#endif

	//---------------------------------------------------------------------------------
	void SetOpenDir(MinClass actor)
	{
		int FacePos = GetFace(actor)
		
		if(FacePos)
			ReverseOpenDir = -1
		else
			ReverseOpenDir = 1

		if(axis == 2)							//vynimka
			ReverseOpenDir = -ReverseOpenDir
	}
	
//---------------------------------------------------------------------------------
	void BeforeOpen()
	{
		if(status != 0)	//nemozeme menit angular limit pokial dvere niesu v zavretej polohe
			return			//toto nastane iba v pripade ze dvere sa zatvaraju smerom k nam a pocas toho im zavolame open()
			
		if(JointAngLimit)
		{
			dJointDestroy(JointAngLimit)
			JointAngLimit = 0
		}
			
		vector JointPos = coords
		vector AxisForAngularLimit
		
		float X, Y, x, y

		if(axis == 0)
		{
			x = StartAxis[2]
			y = StartAxis[0]
		}
		
		if(axis == 1)
		{
			x = StartAxis[0]
			y = StartAxis[1]
		}

		if(axis == 2)
		{
			x = StartAxis[1]
			y = StartAxis[2]
		}

		float TargetAngle = openingangle * DEG2RAD

		X =  x * cos(TargetAngle * ReverseOpenDir)
		X += y * sin(TargetAngle * ReverseOpenDir)
		
		Y = -x * sin(TargetAngle * ReverseOpenDir)
		Y += y * cos(TargetAngle * ReverseOpenDir)

		if(axis == 0)
		{
			EndAxis[0] = Y
			EndAxis[1] = 0
			EndAxis[2] = X
		}
		
		if(axis == 1)
		{
			EndAxis[0] = X
			EndAxis[1] = Y
			EndAxis[2] = 0
		}

		if(axis == 2)
		{
			EndAxis[0] = 0
			EndAxis[1] = X
			EndAxis[2] = Y
		}

		float HalfTargetAngle = TargetAngle * 0.5
		AxisForAngularLimit = StartAxis + EndAxis
		VectorNormalize(AxisForAngularLimit)

/*		Print(StartAxis)
		Print(EndAxis)
		Print(AxisForAngularLimit)*/
/*
		AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE, JointPos, AxisForAngularLimit * 128.0 + JointPos)
		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE, JointPos, StartAxis * 128.0 + JointPos)
		AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE, JointPos, EndAxis * 128.0 + JointPos)*/
		
		JointAngLimit = dJointCreateAngularLimit(World, EndAxis, this, AxisForAngularLimit, HalfTargetAngle, 0)
	}

//---------------------------------------------------------------------------------
	void AfterClosed()
	{
		if(flags & 16)	//open on trigger
		{
			MinClass cc = CreatureInTrigger()
	
			if(cc)
			{
				SetOpenDir(cc)
				Open()
				
				if(ParentDoor != NULL)
					ParentDoor.Open()
			}
		}
	}

//---------------------------------------------------------------------------------
	bool Opening()
	{
		vector CurAxis = GetMatrix(this, StartAxisIndex)
//		vector cc = coords + "0 0 0.2"
//		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_ONCE, cc, CurAxis * 128.0 + cc)
		
		float ff = CurAxis * EndAxis

		if(ff >= 0.9999)
			return true

		float spd = speed * ReverseOpenDir
		vector ForceAxis

		switch(axis)
		{
		case 0:
			ForceAxis = GetMatrix(this, 0) * -spd
			break
		
		case 1:
			ForceAxis = GetMatrix(this, 1) * -spd
			break

		case 2:
			ForceAxis = GetMatrix(this, 1) * spd
			break
		}
		dBodyApplyForce(this, ForceAxis)
		return false
	}

//---------------------------------------------------------------------------------
	bool Closing()
	{
		vector CurAxis = GetMatrix(this, StartAxisIndex)
//		vector cc = coords + "0 0 0.2"
//		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_ONCE, cc, CurAxis * 128.0 + cc)
		
		float ff = CurAxis * StartAxis

		if(ff >= 0.9999)	//vznikaju drobne nepresnosti takze takto
			return true

		float spd = speed * ReverseOpenDir
		vector ForceAxis

		switch(axis)
		{
		case 0:
			ForceAxis = GetMatrix(this, 0) * spd
			break
		
		case 1:
			ForceAxis = GetMatrix(this, 1) * spd
			break

		case 2:
			ForceAxis = GetMatrix(this, 1) * -spd
			break
		}
			
		dBodyApplyForce(this, ForceAxis)
		return false
	}

//-----------------------------------------------------------------
	void Activate(int flag, MinClass other) //implementace virtualni funkce
	{
		if(flag == 1) //zavolal to interny trigger.
		{
			if(!flags & 16)	//musi mat AutoOpen flag
				return
				
			if(status != 0 && status != 2)	//zaroven je to ochrana proti rekurzivnemu volaniu medzi dverami!
				return
				
			int FacePos = GetFace(other)

			if(status == 2)	//ked sa dvere prave zatvaraju smerom od nas tak nemozeme volat Open() koli angular limitu
			{
				if(FacePos == 0 && ReverseOpenDir == -1)
					return
				
				if(FacePos == 1 && ReverseOpenDir == 1)
					return
			}
			
			SetOpenDir(other)	//mozeme si byt isti ze other je nejaka creatura takze nastavime aj SetOpenDir()
		}	

		if(!BeforeOpenAnim && ParentDoor && ParentDoor.BeforeOpenAnim)	//jediny pripad kedy nevolame open
			status = status
		else
			Open()
			
		if(ParentDoor != NULL)
		{
			bool ParentOpen = false	//ked maju obidvoje dvere animacku alebo obidvoje nemaju tak mozeme aktivovat druhe dvere. inac budu druhe dvere 
		
			if(BeforeOpenAnim && ParentDoor.BeforeOpenAnim)
				ParentOpen = true
				
			if(!BeforeOpenAnim && !ParentDoor.BeforeOpenAnim)
				ParentOpen = true
				
			if(BeforeOpenAnim && !ParentDoor.BeforeOpenAnim)
				ParentOpen = false
				
			if(!BeforeOpenAnim && ParentDoor.BeforeOpenAnim)
				ParentOpen = true
				
			if(ParentDoor.status != 0 && ParentDoor.status != 2)	//aby si nevolali Activate rekurzivne!
				ParentOpen = false
				
			if(ParentOpen)
				ParentDoor.Activate(1, other)
		}
	}

//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		if(status == 2)
			SetStatus(1)
	}

//---------------------------------------------------------------------------------
	void EOnJointBreak(class other, int extra)	//ked sa utrhne joint. volane obidvom spojenym body
	{
		if(extra == JointHinge || extra == JointAngLimit)
		{
			if(JointHinge)
				dJointDestroy(JointHinge)
				
			if(JointAngLimit)
				dJointDestroy(JointAngLimit)
				
			JointHinge = 0
			JointAngLimit = 0
			
			dBodySetMass(this, ZeroVec, mass)	//Defaultny inertia
			ClearEventMask(this, EV_FRAME)
			SetStatus(0)
		}
	}

//----------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(!model)
			model = "obj/debug/dvere_OriginDoluVRohu.xob"	//dveretest_Rot.xob
			
		MainObject = GetObject(model)
		SetObject(MainObject, _materialremap)

		GetBoundBox(this, mins, maxs)
		vector sizes = maxs - mins

		if(speed == 0)
			speed = 200

		if(openingangle == 0)
			openingangle = 90
			
		axis = 1	//default
		
		if(flags & 2)
			axis = 0
			
		if(flags & 4)
			axis = 1

		if(flags & 8)
			axis = 2
						
		vector Angs = ZeroVec
		Angs[axis] = angle
		
		vector mat[4]
		RollPitchYawMatrix2(Angs, mat)
		mat[3] = coords
		SetTransform(mat)
		SetOrigin(this, coords)

		if(CreateRigidBody(GEOMTYPE_NONE, "", model) == true)
		{
			dBodyInteractionLayer(this, ET_DOOR)
			FixCurrentPosition(true)
			
			vector JointPos = coords
			vector AxisForHinge
	
			if(axis == 0)
				AxisForHinge = Matrix[1]
	
			if(axis == 1)
				AxisForHinge = Matrix[2]
	
			if(axis == 2)
				AxisForHinge = Matrix[0]
	
			if(axis == 0)
				StartAxisIndex = 2
	
			if(axis == 1)
				StartAxisIndex = 0
	
			if(axis == 2)
				StartAxisIndex = 2
				
			StartAxis = Matrix[StartAxisIndex]
	
			JointHinge = dJointCreateHinge(World, this, JointPos, AxisForHinge, false, 0)
			
	//		AddDShape(SHAPE_LINE, 0xffff555, SS_NOZUPDATE|SS_DOUBLESIDE, JointPos, AxisForHinge * 64.0 + JointPos)
	//		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE, JointPos, AxisForAngularLimit * 64.0 + JointPos)
	
			CreateInternalTrigger()
			dBodyCreatePhysXShadow(this, 2, true)
		}
		
		if(display && !Display)
		{
			Display = new func_display(this, display, false)	//musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
	
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
		ClearFlags(this, TFL_TOUCHTRIGGERS)	//clearujeme po tom co sa v rigidbodyclasse zaplo v CreateRigidBody()
	}

//----------------------------------------------------------------------------
	void func_door_rot()
	{
		ClassNum = ClassNumRotDoor
		Show(true)
		SetEventMask(this, EV_JOINTBREAK|EV_INIT)
	}

//----------------------------------------------------------------------------
	void ~func_door_rot()
	{
		if(JointHinge)
			dJointDestroy(JointHinge)
		
		if(JointAngLimit)
			dJointDestroy(JointAngLimit)
			
		RemoveBodyAndGeom()
		
		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}

//=================================================================================
class misc_shoot_detector extends ExtClass
{
	vector coords
	string model
	vector mins, maxs
	string OnShot, OnShotFail
	int flags
	int ShotWait
	float angle, angle2, angle3

//-----------------------------------------------------------------
	int IsShot(vector rayorigin, vector raydirection)
	{
		if(flags & 2)
			return true

		if(ShotWait == 0)
		{
			ShotWait = 50
			
			if(!flags & 1)	//once
			{
				SetEventMask(this, EV_TIMER)
				SetTimer(this, this, ShotWait, EV_TIMER, 0, true)
			}
			CallMethods(this, OnShot)
		}

		return false
	}

//-----------------------------------------------------------------
	int Shoot(ShotStorage shot)	
	{
		if(shot.HurtType != HT_SHOT)
			return 0

		CreatureClass shooter = shot.shooter
		WeaponClass wc = NULL

		if(shooter && IsInherited(shooter, CCreatureClass)
			wc = shooter.CurrentWeapon
		
		if(!wc)
			return 0

		if(ShotWait > 0)
			return 0
			
//		if(wc.Owner != World.Player)
//			return 0

		switch(wc.ClassNum)
		{
			case CPistol:
				if(!flags & 4)
					return 0
			break
			
			case CMachinegun:
				if(!flags & 8)
					return 0
			break

			case CShotgun:
				if(!flags & 16)
					return 0
			break

			case CSniperRifle:
				if(!flags & 32)
					return 0
			break

			case CGrenade:
				if(!flags & 64)
					return 0
			break

			case CJackHammer:
				if(!flags & 128)
					return 0
			break

			case CDronWeapon:
				if(!flags & 1024)
					return 0
			break

/*			case CSpiderWeapon:
				if(!flags & 2048)
					return 0
			break*/

			default:
				CallMethods(this, OnShotFail)
				return 0
			break
		}

		ShotWait = 50
		CallMethods(this, OnShot)

		if(flags & 1)	//once
			return 0

		SetEventMask(this, EV_TIMER)
		SetTimer(this, this, ShotWait, EV_TIMER, 0, true)
	}
//-----------------------------------------------------------------

	void EOnTimer(MinClass other, int extra)
	{
		ShotWait = 0
		ClearEventMask(this, EV_TIMER)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(mins == ZeroVec)
			mins = "-16 -16 -16"

		if(maxs == ZeroVec)
			maxs = "16 16 16"
			
		ClassType = ClassTypeMiscShootDetector
		CreateModel(this, mins, maxs)
		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
		SetFlags(this, TFL_SOLID|TFL_OBBCOLLISION)
	}

//-----------------------------------------------------------------
	void ~misc_shoot_detector()
	{
		RemoveModel(this)
	}
}

//=================================================================================
/*
func_rotating
speed       -  rychlost. defaultne 200
flags			-  X,Y osa 4,8
*/
//=================================================================================
class func_rotating extends ExtClass
{
	int flags
	vector vel
	float StartAngle
	vector vec
	float speed
	vobject MainObject
	string model
	vector coords

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		vec = vel * ftime
		vec = vec + GetAngles(this)
		SetAngles(this, vec)
		return
	}

//-----------------------------------------------------------------
	void M_Start(ExtClass other)
	{
		SetEventMask(this, EV_FRAME)
	}
	
//-----------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		ClearEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

	void EOnInit(MinClass other, int extra)
	{
		if(model)
		{
			MainObject = GetObject(model)
			SelectObject(this, MainObject)
		}

		int fl
		float f

	//defaults
		if( speed == 0)
			speed = 200

		ClassNum = ClassNumRotating

		if(flags & 4)
		{
			fl = 0
		}
		else
		{
			if(flags & 8)
				fl = 2
			else
				fl = 1
		}

		f = 1
		vel[fl] = f

		if(flags & 2)
			vel = -vel

		vel = vel * speed

		if(flags & 1)
			ClearEventMask(this, EV_FRAME)
		else
			SetEventMask(this, EV_FRAME)

		if(StartAngle != 0)
		{
			vec[fl] = StartAngle
			SetAngles(this, vec)
		}
		SetOrigin(this, coords)

		if(display && !Display)
		{
			Display = new func_display(this, display, false)	//musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
	
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
	}

	void func_rotating()
	{
		SetEventMask(this, EV_LOADED | EV_INIT)
	}

	void ~func_rotating()
	{
		if(MainObject)
		{
			ReleaseObject(MainObject, 0)
			MainObject = VNULL
		}
	}
}






class misc_random_caller extends ExtClass
{
	vector coords
	string OnTimer
	float minwait, maxwait
	bool SpawnEnabled

	void RandomizeWait()
	{
		if(SpawnEnabled)
			SetTimer(this, this, frand(minwait, maxwait), EV_TIMER, 0, true)
	}

	void M_Start(ExtClass other)
	{
		SpawnEnabled = true
		SetEventMask(this, EV_TIMER)
		RandomizeWait()
	}

	void M_Stop(ExtClass other)
	{
		SpawnEnabled = false
		ClearEventMask(this, EV_TIMER)
	}

	void EOnTimer(MinClass other, int extra)
	{
		RandomizeWait()
		CallMethods(this, OnTimer)
	}
	
	void EOnLoaded(MinClass other, int extra)
	{
		if(minwait == 0)
			minwait = 300
			
		if(maxwait == 0)
			maxwait = 600
			
		SetOrigin(this, coords)
		CallMethods(this, OnLoad)
	}
	
	void misc_random_caller()
	{
		SetEventMask(this, EV_LOADED)	
	}
}

//==========================================================================
/*
	diffusemap
	   anim func_image image_frame
		"textures/texture1.tga"
		"textures/texture2.tga"
		"textures/texture3.tga"
*/

class func_image extends ExtClass
{
	int image_frame
	string model
	vector coords
	float angle, angle2, angle3
	int numimages
	vobject MainObject
	string _materialremap

//--------------------------------------------------------------------------
	void M_SetImage(ExtClass other)
	{
		image_frame = atoi(other._value)
	}

//--------------------------------------------------------------------------
	void M_Increment(ExtClass other)
	{
		image_frame++

		if(image_frame >= numimages)
			image_frame = 0
	}

//--------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(!model)
			model = "obj/static/display02_small.xob"

		MainObject = GetObject(model)

		if(MainObject)
		{
			SetObject(MainObject, _materialremap)
		}

		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
		Show(true)
		ClearFlags(this, TFL_SOLID)
	}

//--------------------------------------------------------------------------
	void func_image()
	{
		SetEventMask(this, EV_INIT)
	}
}

//==========================================================================
class func_plat extends FuncClass
{
	int flags
	float length
	float speed
	float dirangle
	float angle2, angle3
	vector direction
	vector EndPos
	vector CurPos
	dJoint JointPrismatic, JointLinLimit, JointMotor
	string OnEnd, OnStart, OnBeginToStart, OnBeginToEnd
	string anim1, anim2
	vobject anm1, anm2
	float AnimFrame
	string sound1
	string sound2
	hsound Sounds[2]
	int	hsnd0
	float WorkingTime

#ifdef EDITOR
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		angle3 = Angs[0]
		angle = Angs[1]
		angle2 = Angs[2]
		SetTransformEx(Vector(angle3, angle, angle2), Pos)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}

	void CreateEditorVisuals()
	{
	}
#endif

//---------------------------------------------------------------------------------
	void SetGravityDir(float OpenDir)
	{
		float Gravity = 9.8 * OpenDir
		vector GravityDir = direction * Gravity
		dBodySetGravity(this, GravityDir)
	}

//-------------------------------------------------------------------
	void ToEndPos()
	{
		if(CurPos == EndPos)
			return

		if(JointMotor)
			dJointDestroy(JointMotor)

		JointMotor = dJointCreateLinearMotor(World, this, direction, speed, 50000)

		SetEventMask(this, EV_FRAME)
//		SetGravityDir(-1)
		ActivateBody(true)

		if(anm1)
		{
			SetAnimSlot(this, 0, anm1, 0, 0, NULL, 30, AF_RESET)
			SetFrame(this, 0, AnimFrame)
		}

		if(Sounds[0])
		{
			if(!hsnd0)
				hsnd0 = PlaySound(this, Sounds[0], 0)
		}

		status = 1
		WorkingTime = 0
		CallMethods(this, OnBeginToEnd)
	}

//-------------------------------------------------------------------
	void ToStartPos()
	{
		if(CurPos == coords)
			return

		if(JointMotor)
			dJointDestroy(JointMotor)

		JointMotor = dJointCreateLinearMotor(World, this, direction, -speed , 50000)

		SetEventMask(this, EV_FRAME)
//		SetGravityDir(1)
		ActivateBody(true)

		if(anm2)
		{
			SetAnimSlot(this, 0, anm2, 0, 0, NULL, 30, AF_RESET)
			SetFrame(this, 0, AnimFrame)
		}

		if(Sounds[0])
		{
			if(!hsnd0)
				hsnd0 = PlaySound(this, Sounds[0], 0)
		}

		status = 2
		WorkingTime = 0
		CallMethods(this, OnBeginToStart)
	}

//-------------------------------------------------------------------
	void M_ToEnd(ExtClass other)
	{
		ToEndPos()
	}
	
//-------------------------------------------------------------------
	void M_ToStart(ExtClass other)
	{
		ToStartPos()
	}

//-------------------------------------------------------------------
	void M_SwitchPos(ExtClass other)
	{
		if(status != 0)
			return

		if(CurPos == coords)
		{
			ToEndPos()
			return
		}

		if(CurPos == EndPos)
		{
			ToStartPos()
			return
		}
	}

//-------------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		CurPos = GetMatrix(this, 3)
		vector vel = GetVelocity(this)
		float Mdot = vel * direction
		float CurDist, VelLng
		WorkingTime += ftime

		if(status == 1)
		{
			CurDist = VectorLength(CurPos - coords)

			if(CurDist >= length || Mdot < 0)
			{
				if(CurDist >= length)	//iba ak sme naozaj na konci
				{
					SetVelocity(this, ZeroVec)
					CurPos = EndPos
					SetOrigin(this, EndPos)
					ActivateBody(false)
					ClearEventMask(this, EV_FRAME)
					status = 0
	
					if(JointMotor)
					{
						dJointDestroy(JointMotor)
						JointMotor = 0
					}
	
					if(hsnd0)
					{
						EndSound(hsnd0)
						hsnd0 = 0
					}
	
					if(Sounds[1])
						PlaySound(this, Sounds[1], SF_ONCE)
	
					CallMethods(this, OnEnd)
					return
				}
				
				if(WorkingTime > 1 && CurDist < length)	//prekazka zabranila dokonceniu tak ho posleme nazad
				{
					VelLng = VectorLength(vel)
					
					if(VelLng > 1)
						ToStartPos()
				}

				return
			}
		}
		
		if(status == 2)
		{
			CurDist = VectorLength(CurPos - EndPos)

			if(CurDist >= length || Mdot > 0)
			{
				if(CurDist >= length)
				{
					SetVelocity(this, ZeroVec)
					CurPos = coords
					SetOrigin(this, coords)
					ActivateBody(false)
					ClearEventMask(this, EV_FRAME)
					status = 0
	
					if(JointMotor)
					{
						dJointDestroy(JointMotor)
						JointMotor = 0
					}
	
					if(hsnd0)
					{
						EndSound(hsnd0)
						hsnd0 = 0
					}
	
					if(Sounds[1])
						PlaySound(this, Sounds[1], SF_ONCE)
	
					CallMethods(this, OnStart)
					return
				}

				if(WorkingTime > 1 && CurDist < length)	//prekazka zabranila dokonceniu tak ho posleme nazad
				{
					VelLng = VectorLength(vel)

					if(VelLng > 1)
						ToEndPos()
				}

				return
			}
		}

		ActivateBody(true)
		AnimFrame = BumpFrame(this, ftime, 0)
	}

//-------------------------------------------------------------------
	void CreateJoints()
	{
		if(!rb)
			return

		if(!JointPrismatic)
			JointPrismatic = dJointCreatePrismatic(World, this, coords, direction, false, 0)

		if(!JointLinLimit)
		{
			float HalfLength = length * 0.5
			vector MidlePos = direction * HalfLength
			MidlePos = coords + MidlePos
			
			JointLinLimit = dJointCreateLinearLimit(World, MidlePos, this, coords, HalfLength, 0)
		}
	}

//-------------------------------------------------------------------
	void DestroyJoints()
	{
		if(JointPrismatic)
		{
			dJointDestroy(JointPrismatic)
			JointPrismatic = 0
		}
		
		if(JointLinLimit)
		{
			dJointDestroy(JointLinLimit)
			JointLinLimit = 0
		}

		if(JointMotor)
		{
			dJointDestroy(JointMotor)
			JointMotor = 0
		}
	}

//-------------------------------------------------------------------
	void M_SetLength(ExtClass other)
	{
		if(status != 0)
			return
		
		if(CurPos != coords)	//iba z pociatocnej polohy
			return
		
		length = atof(other._value)
		EndPos = direction * length
		EndPos = coords + EndPos
		CurPos = coords
		DestroyJoints()
		CreateJoints()
	}

//-------------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}
	
//-------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(length == 0)
			length = 64

		if(speed == 0)
			speed = 50

		direction = GetDirectionFromAngle(dirangle)
		EndPos = direction * length
		EndPos = coords + EndPos
		CurPos = coords

		if(!model)
			model = "obj/elevators/elevator_01.xob"

		MainObject = GetObject(model)
		
		if(MainObject)
		{
			SelectObject(this, MainObject)

			if(anim1)
				anm1 = GetObject(anim1)
			
			if(anim2)
				anm2 = GetObject(anim2)

			SetTransformEx(Vector(angle3, angle, angle2), coords)

			if(CreateRigidBody(GEOMTYPE_NONE, "", model) == true)
			{
				CreateJoints()
				ActivateBody(false)
				dBodyInteractionLayer(this, ET_DOOR)
				dBodySetGravity(this, ZeroVec)
			}
			SetPickable(false)
			Show(true)
		}
		
		if(!sound1)
			sound1 = "SOUND/DOOR/door_01_START"

		if(!sound2)
			sound2 = "SOUND/EFFECTS/Explosion_flash_01"

		Sounds[0] = GetSoundEx(sound1, 1, SF_3D)
		Sounds[1] = GetSoundEx(sound2, 1, SF_3D)

		if(display && !Display)
		{
			Display = new func_display(this, display, false)	//musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
	
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
		SetFlags(this, TFL_TOUCHTRIGGERS)
	}

//-------------------------------------------------------------------	
	void func_plat()
	{
		SetEventMask(this, EV_INIT | EV_LOADED)
		SetFlags(this, TFL_SOLID)
	}
	
//-------------------------------------------------------------------	
	void ~func_plat()
	{
		DestroyJoints()
		RemoveBodyAndGeom()

		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}




//=================================================================================
//=================================================================================
class PumpDevice extends RigidBodyClass
{
	string model
	float angle, angle2, angle3
	vector coords
	float PumpTimer
	float PistonHeight
	vobject MainObject
	string pumpsound, joinsound, unjoinsound
	hsound PumpSound, JoinSound, UnJoinSound

#ifdef EDITOR
	void EditorOpened()
	{
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		angle2 = atof(GetKeyValue("angle2"))
		angle3 = atof(GetKeyValue("angle3"))
		vector Angs = Vector(angle3, angle, angle2)

		if(GetParent() == NULL)
			SetTransformEx(Angs, coords)
		
		UpdateEntity(this)
	}

	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		if(GetParent() == NULL)	//normalny stav
			SetTransformEx(Angs, Pos)
		else											//zrejme je na niecom naparentena
		{
			ClearFlags(this, TFL_FULLMATRIX)
			SetOrigin(this, Pos)	
			SetAngles(this, Angs)
		}

		UpdateMainVisualizer()
		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}
	
	void CreateEditorVisuals()
	{
	}
#endif

//-----------------------------------------------------------------
	void Activate(int fl, ExtClass other)		//aktivacia sa podarila
	{
		PumpTimer = 1
		func_display disp = Display

		if(disp)
			disp.RunAction("PUMP")

		if(JoinSound)
			PlaySound(this, JoinSound, SF_ONCE)
	}
/*
//-----------------------------------------------------------------
	void ActivateEnd()
	{
		if(UnJoinSound)
			PlaySound(this, UnJoinSound, SF_ONCE)
	}
*/
//-----------------------------------------------------------------
	void ActivateFail(int fl, ExtClass other)	//aktivacia sa nepodarila. na vypis hlasok co k tomu potrebujes atd.
	{
	}
	
//-----------------------------------------------------------------
	void OnPump()
	{
	}
	
//-----------------------------------------------------------------
	void ActivateProcess(int fl, ExtClass other)
	{
		if(CanActivate(0, World.Player)/*PlayerOxygen < PlayerMaxOxygen*/)
		{
			OnPump()
		}
	}

//-----------------------------------------------------------------
	void ActivateProcessEnd()	//skoncilo volanie ActivateProcess()
	{
		func_display disp = Display

		if(disp)
			disp.RunAction("IDLE")

		if(UnJoinSound)
			PlaySound(this, UnJoinSound, SF_ONCE)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void BeforeInit()
	{
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		BeforeInit()
		SetPickable(false)

		MainObject = GetObject(model)

		if(MainObject)
			SetObject(MainObject, _materialremap)

		SetTransformEx(Vector(angle3, angle, angle2), coords)
		SetAngles(this, Vector(angle3, angle, angle2))
		CreateRigidBody(GEOMTYPE_NONE, "", model)

		PumpSound = GetSoundEx(pumpsound, 1, SF_3D)
		JoinSound = GetSoundEx(joinsound, 1, SF_3D)
		UnJoinSound = GetSoundEx(unjoinsound, 1, SF_3D)

		if(display && !Display)
		{
			Display = new func_display(this, display, false)	//musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
	
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
		ClearFlags(this, TFL_TOUCHTRIGGERS)	//clearujeme po tom co sa v rigidbodyclasse zaplo v CreateRigidBody()
	}

//-----------------------------------------------------------------
	void PumpDevice()
	{
		ClassType = ClassTypeFunc
		PistonHeight = 4.8	//aby bol v hornej polohe
		
		if(ActivateMethod == ACT_ENTITY_DEFAULT)
			ActivateMethod = ACT_USEKEY

		SetEventMask(this, EV_INIT | EV_LOADED)
		SetFlags(this, TFL_VISIBLE)
	}

//-----------------------------------------------------------------
	void ~PumpDevice()
	{
		RemoveBodyAndGeom()

		if(MainObject)
		{
			ReleaseObject(MainObject, 0)
			MainObject = VNULL
		}
	}
}


//=================================================================================
class func_oxygen_device extends PumpDevice
{
//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(other.ClassNum == ClassNumPlayer)
		{
			CreatureClass pla = other
	
			if(pla.GetOxygen() < PlayerMaxOxygen)
				return ACT_ENABLE
		}

		return ACT_DISABLE
	}

//-----------------------------------------------------------------
	void OnPump()
	{
		CreatureClass pla = World.Player
		pla.AddOxygen(OxygenPumpUpSpeed * ftime)
		
		PistonHeight += ftime * 10
		float PistonPos = sin(PistonHeight)
		PistonPos += 1
		PistonPos *= 0.5
		PistonPos *= 9.6	//zdvih
		SetBone(this, B_OxDev_pist, ZeroVec, Vector(0, 0, -PistonPos), 1)

		PumpTimer += ftime
		if(PumpTimer >= 0.33333)
		{
			PumpTimer = 0
			
			if(PumpSound)
				PlaySound(this, PumpSound, SF_ONCE)
		}
	}

//-----------------------------------------------------------------
	void BeforeInit()
	{
		if(!model)
			model = "obj/static/OxygenDevice.xob"

		if(!display)
			display = "displays/OxygenDevice.dpl"

		if(!pumpsound)
			pumpsound = "OxygenPumpUp"
			
		if(!joinsound)
			joinsound = "OxygenDeviceJoin"
			
		if(!unjoinsound)
			unjoinsound = "OxygenDeviceUnjoin"
	}
}



//=================================================================================
class func_health_device extends PumpDevice
{
	float health, MaxHealth

//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(health > 0)
			return ACT_ENABLE
/*
		if(other.ClassNum == ClassNumPlayer)
		{
			CreatureClass pla = other
	
			if(health > 0 && pla.Health < PlayerMaxHealth)
				return ACT_ENABLE
		}*/

		return ACT_DISABLE
	}

//-----------------------------------------------------------------
	void UpdateBar()
	{
		float rate = health / MaxHealth
		clamp rate<0, 1>
		float PistonPos = rate * 9.6	//zdvih
		SetBone(this, B_OxDev_pist, ZeroVec, Vector(0, 0, -9.6 + PistonPos), 1)	
	}

//-----------------------------------------------------------------
	void OnPump()
	{
		CreatureClass pla = World.Player
		if(health <= 0 || pla.Health >= PlayerMaxHealth)
			return

		float ToAdd = HealthPumpUpSpeed * ftime
		clamp ToAdd<0, health>
		float res = pla.Health + ToAdd
		
		if(res > PlayerMaxHealth)
			ToAdd -= res - PlayerMaxHealth
		
		clamp ToAdd<0, PlayerMaxHealth>
		pla.AddHealth(ToAdd)
		health -= ToAdd
		
		if(health <= 0 && Display)
		{
			func_display disp = Display
			disp.RunAction("EMPTY")
		}
		
		UpdateBar()

		PumpTimer += ftime
		if(PumpTimer >= 0.33333)
		{
			PumpTimer = 0
			
			if(PumpSound)
				PlaySound(this, PumpSound, SF_ONCE)
		}
	}

//-----------------------------------------------------------------
	void ActivateProcessEnd()	//skoncilo volanie ActivateProcess()
	{
		func_display disp = Display

		if(disp && health > 0)	//inak tam uz visi EMPTY
			disp.RunAction("IDLE")

		if(UnJoinSound)
			PlaySound(this, UnJoinSound, SF_ONCE)
	}

//-----------------------------------------------------------------
	void BeforeInit()
	{
		if(!model)
			model = "obj/static/HealthDevice.xob"

		if(!display)
			display = "displays/healthdevice.dpl"

		if(!pumpsound)
			pumpsound = "OxygenPumpUp"
			
		if(!joinsound)
			joinsound = "OxygenDeviceJoin"
			
		if(!unjoinsound)
			unjoinsound = "OxygenDeviceUnjoin"
		
		CreatureClass pla = World.Player
		MaxHealth = HealthDeviceHealth[pla.GetDifficultyLevel()]
		health = MaxHealth
		UpdateBar()
	}
}

//========================================================================
class func_trigger extends ExtClass
{
	int flags
	vector coords
	float angle, angle2, angle3
	string model
	string anm1, anm2
	string sound1, sound2
	string OnStartPos, OnEndPos, OnRunToStart, OnRunToEnd
	vobject MainObject
	vobject anims[2]
	hsound sounds[2]
	int CurSide
	int status
	bool ActivateEnabled

//-----------------------------------------------------------------
	void SetCurAnimStartPos()
	{
		SetAnimSlot(this, 0, anims[CurSide], 0, 0, NULL, 30, AF_ONCE)
	}

//-----------------------------------------------------------------
	void CallPosEvent()
	{
		int HumanLookSide = CurSide
		
		if(flags & 1)
			HumanLookSide = 1 - HumanLookSide
		
		if(HumanLookSide == 0)
			CallMethods(this, OnStartPos)
		else
			CallMethods(this, OnEndPos)
	}

//-----------------------------------------------------------------
	void Start()
	{
		if(anims[CurSide])
		{		
			SetAnimSlot(this, 0, anims[CurSide], 0, 0, NULL, 30, AF_ONCE)
			SetEventMask(this, EV_FRAME)
			status = 1
			PlaySound(this, sounds[CurSide], SF_ONCE)

			int HumanLookSide = CurSide
			
			if(flags & 1)
				HumanLookSide = 1 - HumanLookSide

			if(HumanLookSide == 0)
				CallMethods(this, OnRunToEnd)
			else
				CallMethods(this, OnRunToStart)
		}
	}

//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(other.ClassNum != ClassNumPlayer)
			return ACT_DISABLE

		if(!ActivateEnabled)
			return ACT_DISABLE
		
		if(status != 0)
			return ACT_DISABLE

		if(!anims[CurSide])
			return ACT_DISABLE

		return ACT_ENABLE
	}

//-----------------------------------------------------------------
	void Activate(int flag, MinClass other) //implementace virtualni funkce
	{
		Start()
	}

//------------------------------------------------------------------------
	void M_Enable(ExtClass other)
	{
		ActivateEnabled = true
	}

//------------------------------------------------------------------------
	void M_SwitchPos(ExtClass other)
	{
		Start()
	}

//------------------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		ActivateEnabled = false
	}

//------------------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		if(extra == 0)
		{
			int OtherAnim = 1 - CurSide
			
			if(anims[OtherAnim])	//nahodime hned druhu animacku ak ju mame
			{
				CurSide = OtherAnim
				SetCurAnimStartPos()
				ClearEventMask(this, EV_FRAME)
				status = 0
				CallPosEvent()
			}
		}
	}

//------------------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(status == 1)	//iny pripad nemoze nastat ale istota
			BumpFrame(this, ftime, 0)
	}

//------------------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
		
		if(!model)
			model = "obj/static/switch_device_1_handle.xob"

		if(!anm1)
			anm1 = "obj/static/switch_device_1_handle.anm"

		if(!anm2)
			anm2 = "obj/static/switch_device_1_handle_up.anm"

		if(!sound1)
			sound1 = "paka01_activating"

		if(!sound2)
			sound2 = "paka01_deactivating"

		if(flags & 1)		//start v opacnej pozicii
			CurSide = 1

		MainObject = GetObject(model)

		if(MainObject)
		{
			SetObject(MainObject, "")
			anims[0] = GetObject(anm1)
			anims[1] = GetObject(anm2)
			sounds[0] = GetSoundEx(sound1, 1, SF_3D)
			sounds[1] = GetSoundEx(sound2, 1, SF_3D)
		}
		
		ActivateEnabled = true

		if(anims[CurSide])
		{
			SetCurAnimStartPos()
			BumpFrame(this, 0, 0)
			CallPosEvent()
		}
		else
		{
			string MissingAnim = anm1
			if(CurSide == 1)
				MissingAnim = anm2

			DPrint(String("Missing anim " + MissingAnim + " in entity " + _name))
		}
	}

//------------------------------------------------------------------------
	void func_trigger()
	{
		ClassType = ClassTypeFunc
		
		if(ActivateMethod == ACT_ENTITY_DEFAULT)
			ActivateMethod = ACT_USEKEY

		ClassType = ClassTypeFunc
		SetEventMask(this, EV_INIT | EV_LOADED | EV_ANIMEND)
		SetFlags(this, TFL_VISIBLE | TFL_SOLID | TFL_OBBCOLLISION | TFL_DYNAMICBBOX)
	}
}