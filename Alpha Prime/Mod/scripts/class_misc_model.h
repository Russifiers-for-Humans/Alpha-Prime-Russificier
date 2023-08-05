class misc_physx_drinkmachine extends ExtClass
{
	vector coords
	float angle, angle2, angle3
	float timeout
	vobject MainObject

	hsound hs
	ExtClass Cans[32]
	int Top

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
	}

	vector Matrix[4]
//-----------------------------------------------------------------
	void EOnTick(MinClass other, int extra)
	{
		if(g_physx)
			return

		timeout += TICK_PERIOD

		if(timeout > 0.5)
		{
			timeout = 0

			ExtClass ec = Cans[Top]

			if(ec == NULL)
			{
				ec = new ExtClass
				SetOrigin(ec, coords)
				SetAngles(ec, Vector(angle3 + 90, angle + 90, angle2))
				UpdateEntity(ec)
				GetMatrix4(ec, Matrix)

				SetFlags(ec, TFL_VISIBLE|TFL_FULLMATRIX|TFL_NOSHADOW)
				ec.SetObject(MainObject, "$remap 'obj/sharedtextures/can' 'obj/sharedtextures/can2b';")

				NxActorCreate(ec, ZeroVec, BC_MODEL, true, 1.5, 0, 0)
				NxActorSetCollisionGroup(ec, 1)

				Cans[Top] = ec
			}
			else
			{
				SetMatrix(ec, Matrix)
			}

			Top = Top + 1 & 31

			vector vec
			vec = GetMatrix(this, 0)

			vec[0] = vec[0] * frand(0.95, 1.05)
			vec[1] = vec[1] * frand(0.95, 1.05)
			vec[2] = vec[2] * frand(0.95, 1.05)
			SetVelocity(ec, vec * 35)
			PlaySound(this, hs, SF_ONCE)
		}
	}

//-----------------------------------------------------------------
	void misc_physx_drinkmachine()
	{
		ClassType = 666
		SetEventMask(this, EV_INIT | EV_LOADED)
		SetOrigin(this, coords)

		if(g_physx)
			SetEventMask(this, EV_TICK)

		MainObject = GetObject("obj/physics/cansmall.xob")
		SetFlags(this, TFL_VISIBLE|TFL_SOLID)

		hs = GetSoundEx("Plate_item", 1, SF_3D)
	}

//-----------------------------------------------------------------
	void ~misc_physx_drinkmachine()
	{
		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}

class misc_physx_box extends ExtClass
{
	vector coords
	float angle, angle2, angle3
	bool body

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
	}

//-----------------------------------------------------------------
	void misc_physx_box()
	{
		ClassType = 666
		SetEventMask(this, EV_INIT | EV_LOADED)
		SetOrigin(this, coords)

		if(g_physx)
		{
			body = true
			CreateModel(this, "-4 -4 -4", "4 4 4")
			dBodyCreate(this, ZeroVec, BC_OBB)
			dBodyActive(this, false, false)
			dBodyCreatePhysXShadow(this, 0, PSF_COLLIDE_CLOTH)
			SetFlags(this, TFL_VISIBLE|TFL_SOLID)
		}
	}


//-----------------------------------------------------------------
	void ~misc_physx_box()
	{
		if(body)
		{
			dBodyDestroyPhysXShadow(this)
			dBodyDestroy(this)
			RemoveModel(this)
		}
	}
}

class misc_model extends RigidBodyClass
{
	bool _nocollision
	string model
	float angle, angle2, angle3
	vector coords
	vobject MainObject
	bool railing

//---------------------------------------------------------------------------------
	void UpdateAgent(bool act)
	{
		//we don't want to place pathengine agents here!!!
	}

//---------------------------------------------------------------------------------
#ifdef EDITOR
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		angle3 = Angs[0]
		angle = Angs[1]
		angle2 = Angs[2]
		SetTransformEx(Angs, Pos)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()    //musime updatovat zhora

		if(root)
			root.RefreshInEditor()  //namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}

	void CreateEditorVisuals()
	{
	}
#endif
/*
//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(ActivateMethod <= ACT_NONE)
			return ACT_DISABLE

		if(ActivateMethod == ACT_USEKEY_DISP)
		{
			if(Display && Display.CanActivate(0, this) == ACT_ENABLE)
				return ACT_ENABLE
			else
				return ACT_DISABLE
		}

		if(ActivateMethod != ACT_USEKEY)
			return ACT_DISABLE

		return ACT_ENABLE
	}*/

//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{
		//HOTFIX: some bodies are on wrong positions. Why???
		if(rb)
		{
			rb = false
			dBodyDestroy(this)
		}
		
		if(CreateRigidBody(GEOMTYPE_NONE, "", model))
		{
			dBodyInteractionLayer(this, ET_WORLD)   //misc_modely su vzdy staticke takze to budeme chapat ako world
			ActivateBody(false)
			dBodyCreatePhysXShadow(this, 0, PSF_COLLIDE_CLOTH|PSF_COLLIDE_RB)
		}
		ClearFlags(this, TFL_TOUCHTRIGGERS)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)


#ifndef EDITOR
		if(g_physx && strstr(model, "obj/misc/pipe_0") == 0)
		{
			ClothDef cdef = new ClothDef
			cdef.Thickness = 0.05
			cdef.Density = 1.0
			cdef.BendingStiffness = 1.0
			cdef.StretchingStiffness = 1.0
			cdef.DampingCoefficient = 0.7
			cdef.Friction = 0.5
			cdef.Pressure = 2.0
			cdef.CollisionResponseCoefficient = 1.0
			cdef.AttachmentResponseCoefficient = 1.0
			cdef.SolverIterations = 5
			cdef.CollisionGroup = 0
//          cdef.Flags = NX_CLF_BENDING|NX_CLF_GRAVITY
//          cdef.Flags = NX_CLF_PRESSURE|NX_CLF_BENDING|NX_CLF_GRAVITY
//          cdef.Flags = NX_CLF_BENDING|NX_CLF_GRAVITY|NX_CLF_BENDING_ORTHO|NX_CLF_PRESSURE
//            cdef.Flags = NX_CLF_GRAVITY|NX_CLF_PRESSURE
//            cdef.Flags = NX_CLF_BENDING|NX_CLF_GRAVITY|NX_CLF_BENDING_ORTHO
			cdef.Flags = NX_CLF_GRAVITY|NX_CLF_PRESSURE
			NxClothCreate(this, "textures/trim/Pipe_03", cdef, true)

			delete cdef
		}
#endif
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{

		if(!model)
		   model = "obj/physics/boxik.xob"
		
		if(impactsounds == 3)   //user do kluca nic nenastavil tak my to chceme defaultne vypnute
		   impactsounds = 0
		
		MainObject = GetObject(model)
		SetObject(MainObject, _materialremap)
		
		SetTransformEx(Vector(angle3, angle, angle2), coords)
		SetOrigin(this, coords)
		
		string mm = model
		strlower(mm)
		
		if(strstr(mm, "obj/static/rail") != -1)
		{
		   railing = true
		}
		
		if(_nocollision)
		   return
		
		if(CreateRigidBody(GEOMTYPE_NONE, "", model))
		{
		   dBodyInteractionLayer(this, ET_WORLD)   //misc_modely su vzdy staticke takze to budeme chapat ako world
		   ActivateBody(false)
		   dBodyCreatePhysXShadow(this, 0, PSF_COLLIDE_CLOTH|PSF_COLLIDE_RB)
		}
		
		ClearFlags(this, TFL_TOUCHTRIGGERS)
		
		if(display && !Display)
		{
			Display = new func_display(this, display, false)    //musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
		
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
	}

//-----------------------------------------------------------------
	void misc_model()
	{
		ClassType = ClassTypeMiscModel
		SetEventMask(this, EV_INIT | EV_LOADED)
		SetFlags(this, TFL_VISIBLE|TFL_SOLID|TFL_NOSHADOW)
		SetPickable(false)

		if(g_physx && strstr(model, "obj/misc/pipe_0") == 0)
			_surfaces = ""
	}

//-----------------------------------------------------------------
	void ~misc_model()
	{
//      if(!_nocollision)
//      {
			if(IsRigidBody())   //FIXME v _nocollision sa nachadza hodnota 1024. co to je?!
			{
				dBodyDestroyPhysXShadow(this)
				RemoveBodyAndGeom()
			}
//      }

		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}
//=================================================================================
class misc_anim_object extends Actor
{
	vector coords
	int angle
	int angle2
	int angle3
	string model
	string anim

	int speed

	int type
	int flags
	int fps
	int EditFrame
	bool Disabled

	string OnStart, OnEnd, OnPause, OnContinue

#ifdef EDITOR
	void EditorOpened()
	{
		if(EditFrame)
		{
			SetFrame(this, 6, EditFrame)
			BumpFrame(this, 0, 0)
			UpdateEntity(this)
		}
	}
#endif

	void M_Enable(ExtClass other)   //enabluje aktivovanie
	{
		Disabled = false
	}

	void M_Disable(ExtClass other)
	{
		Disabled = true
	}

//---------------------------------------------------------------------------------------------------------
	void CanActivate(int fl, ExtClass other)
	{
		if(Disabled)                //su pripady kedy potrebujeme disablovat aj ked to displej povoluje
		   return ACT_DISABLE

	  if(ActivateMethod <= ACT_NONE)
		  return ACT_DISABLE

	 if(ActivateMethod == ACT_USEKEY_DISP)
	 {
		 if(Display && Display.CanActivate(0, this) == ACT_ENABLE)
			 return ACT_ENABLE
		 else
			 return ACT_DISABLE
	 }

	 if(ActivateMethod == ACT_USEKEY)
	 {
		 return ACT_ENABLE
	 }

	 return ACT_DISABLE

/*
		if(status == 0)
			return ACT_ENABLE

		return ACT_DISABLE*/
	}

//-----------------------------------------------------------------
	void Switch(bool onoff)
	{
		status = onoff

		if(flags & 8)
		{
			if(onoff)
				SetEventMask(this, EV_VISIBLE)
			else
			{
				if(!Display)
					ClearEventMask(this, EV_VISIBLE)
			}
		}
		else
		{
			if(onoff)
				SetEventMask(this, EV_FRAME)
			else
			{
				if(!Display)
					ClearEventMask(this, EV_FRAME)
			}
		}
	}

//-----------------------------------------------------------------
	void M_Start(ExtClass other)
	{
		Switch(true)
		CallMethods(this, OnStart)
	}

//-----------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		Switch(false)
		CallMethods(this, OnEnd)
	}

//-----------------------------------------------------------------
	void M_Pause(ExtClass other)
	{
		Switch(false)
		CallMethods(this, OnPause)
	}

//-----------------------------------------------------------------
	void M_Continue(ExtClass other)
	{
		Switch(true)
		CallMethods(this, OnContinue)
	}

//-----------------------------------------------------------------
	void M_SeekAnim(ExtClass other)
	{
		string val = other._value

		int slot = 0, n = 0, p
		float frame
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

		switch(n)
		{
		case 0:
			slot = atoi(parm)
		break

		case 1:
			frame = atof(parm)
		break
		}

		n++
		if(p != -1)
			goto loop

		if(n < 2)
		{
			Print("SeekAnim: not enough parameters")
			return
		}

		slot += s_cutscene
		clamp slot<s_cutscene, 7>

		SetFrame(this, slot, frame)
	}

//-----------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		CallMethods(this, OnEnd)
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		//evidentne to nejaky debil pouzil jen jako neanimovany objekt
		if(NumPAnims == 0)
		{
			ClearEventMask(this, EV_FRAME)
			return
		}
		//looped anim if present
		BumpFrame(this, ftime, 0)
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		//evidentne to nejaky debil pouzil jen jako neanimovany objekt
		if(NumPAnims == 0 && !Display)
		{
			ClearEventMask(this, EV_VISIBLE)
		}
		else
		{
			//looped anim if present
			BumpFrame(this, ftime, 0)
		}
		return 0
	}

	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		ClassType = ClassTypeMisc

		if(fps == 0)
			fps = 30

		SetOrigin(this,coords)
		SetAngles(this, Vector(angle3, angle, angle2))

		MainObject = GetObject(model)
		if(MainObject)
		{
			SetObject(MainObject, _materialremap)

			int p = strstr(model,".xob")

			if(p != -1)
			{
				if(anim != "")
				{
					vobject anm = AddPAnim(anim)

					if(anm)
					{
						int aflags = 0

						if(flags & 1)
							aflags |= AF_ONCE

						if(flags & 32)
							aflags |= AF_WRAP

						//slot 6 because of cutscenes (s_cutscene + 0...1)
						SetAnimSlot(this, 6, anm, 0, 1, NULL, fps, aflags)

						if(flags & 4)
							Switch(true)
					}
				}

				type = 1
			}
			else
			{
				type = 0
			}

			SetFlags(this,TFL_VISIBLE)
			SetEventMask(this,EV_ANIMEND|EV_INIT|EV_LOADED)
		}
		else
		{
			ClearEventMask(this, EV_ALL)
		}

		if(speed == 0)
			speed = 5


		if(flags & 16)
			SetFlags(this, TFL_NOSHADOW)

#ifdef EDITOR
		if(World.EditorActive)
		{
			if(EditFrame)
			{
			   SetFrame(this, 6, EditFrame)
				BumpFrame(this, 0, 0)
				UpdateEntity(this)
			}
		}
#endif

//musi probehnout aspon prvni snimek, jinak jsou XOB objekty neinicializovane
		BumpFrame(this, 0, 0)
		SetFlags(this, TFL_TRIGGER)

		UpdateEntity(this)

		if(display && !Display)
		{
			Display = new func_display(this, display, false)    //musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
			
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}

		if(SoundSetName)
			LoadSoundSet(SoundSetName, CSounds)
	}

//-----------------------------------------------------------------
	void misc_anim_object()
	{
		SetFlags(this,TFL_SOLID|TFL_OBBCOLLISION|TFL_DYNAMICBBOX)
	}
}

//=================================================================================
#ifdef NEPOUZITO
//=================================================================================
class misc_testhead extends Actor
{
	float frame
	vector coords
	int angle
	int angle2
	int angle3
	string model

	float p_face0,p_face1,p_face2,p_face3

	ExtClass animobj
	hsound  sound


//-----------------------------------------------------------------
	void Anim(int bone, string name)
	{
		vector mat[4]
		float weight

		GetBoneMatrix(animobj, bone, mat)
		weight = mat[3][2]/32
		SetMorphState(this, name, weight)

		string msg = name + " " + ftoa(weight)
//      Print(msg)
	}


//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		BumpFrame(animobj, ftime, 0)

//      Print("---------------------")
		Anim(B_BrowsDown_L, "BrowsDown_L")
		Anim(B_BrowsDown_R, "BrowsDown_R")
		Anim(B_BrowsUp_L,   "BrowsUp_L")
		Anim(B_BrowsUp_R,   "BrowsUp_R")
		Anim(B_BrowsUpSide_L,   "BrowsUpSide_L")
		Anim(B_BrowsUpSide_R,   "BrowsUpSide_R")
		Anim(B_Mrkani_L,    "Mrkani_L")
		Anim(B_Mrkani_R,    "Mrkani_R")
		Anim(B_Privreni_L,  "Privreni_L")
		Anim(B_Privreni_R,  "Privreni_R")
		Anim(B_Angry,       "Angry")
		Anim(B_Sad,     "Sad")
		Anim(B_AI,      "AI")
		Anim(B_BMP,     "BMP")
		Anim(B_CDG,     "CDG")
		Anim(B_E,       "E")
		Anim(B_FV,      "FV")
		Anim(B_O,       "O")
		Anim(B_QW,      "QW")
		Anim(B_U,       "U")



		if(KeyState(KC_NUMPAD7))
		{
			frame += 1.0
			SetFrame(animobj, 0, 0.0)
			BumpFrame(animobj, ftime, 0)

			PlaySound(this, sound, SF_ONCE)


			if (frame > 81.0)
				frame = 0.0
		}


/*      if(KeyState(KC_NUMPAD7))
		{
			p_face0 += ftime * 0.5
		}

		if(KeyState(KC_NUMPAD4))
		{
			p_face0 -= ftime * 0.5
		}

		clamp p_face0<0,1>
		SetMorphState(this, "AI", p_face0)

		if(KeyState(KC_NUMPAD8))
		{
			p_face1 += ftime * 0.5
		}

		if(KeyState(KC_NUMPAD5))
		{
			p_face1 -= ftime * 0.5
		}

		clamp p_face1<0,1>
		SetMorphState(this, "E", p_face1)
*/
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		Print("AGAIN")
		SetAnimSlot(animobj, 0, GetObject("anm/morphs/I_1_Arn0008.anm"), 0, 0, NULL, 30, AF_ONCE)
		PlaySound(this, sound, SF_ONCE)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		ClassType = ClassTypeMisc
		frame = 0.0

		SetOrigin(this,coords)
		SetAngles(this, Vector(angle3, angle, angle2))

		if(!model)
		{
			model = "obj/creatures/arnold/arnold_cutscene.xob"
		}

		MainObject = GetObject(model)
		if(MainObject)
		{
			SelectObject(this, MainObject)

			SetFlags(this,TFL_VISIBLE)
			SetEventMask(this,EV_INIT|EV_LOADED|EV_FRAME|EV_ANIMEND)
		}
		else
		{
			ClearEventMask(this, EV_ALL)
		}

		sound = GetSoundEx("I_1_Arn0008", 1, SF_3D)
		PlaySound(this, sound, SF_ONCE)
		Print(sound)

		//musi probehnout aspon prvni snimek, jinak jsou X objekty neinicializovane
		BumpFrame(this, 0, 0)

		animobj = new ExtClass

		SelectObject(animobj, GetObject("obj/creatures/arnold/morphs.xob"))
		SetAnimSlot(animobj, 0, GetObject("anm/morphs/I_1_Arn0008.anm"), 0, 0, NULL, 30, AF_ONCE)
//      SetAnimSlot(animobj, 0, GetObject("obj/creatures/arnold/morph_test_export_basic.anm"), 0, 0, NULL, 30, 0)
//      SetAnimSlot(animobj, 0, GetObject("obj/creatures/arnold/morph_test_export_dvadohromady.anm"), 0, 0, NULL, 30, AF_USER)
	}

//-----------------------------------------------------------------
	void misc_testhead()
	{
		SetFlags(this,TFL_SOLID|TFL_OBBCOLLISION)
		ActivateMethod = 0

	}
}
#endif

//=================================================================================
// test class, loads object, morphs and dialogue sound
//=================================================================================
class MiscTestDialogue extends Actor
{
	vobject     m_Object
	vobject     m_MorphObject
	vobject     m_Dialogue
	hsound      m_Sound

	ExtClass    m_AnimObject

	string      m_strObject
	string      m_strMorphName
	string      m_strSoundName

	bool        m_bReady

//-----------------------------------------------------------------
	void Anim(int bone, string name)
	{
		vector mat[4]
		float weight

		GetBoneMatrix(m_AnimObject, bone, mat)
		weight = mat[3][2]/32
		SetMorphState(this, name, weight)
	}


//-----------------------------------------------------------------
//  void EOnFrame(MinClass other, int extra)
	void SetBonesSetup()
	{
//      BumpFrame(m_AnimObject, ftime, 0)
		Anim(B_BrowsDown_L, "BrowsDown_L")
		Anim(B_BrowsDown_R, "BrowsDown_R")
		Anim(B_BrowsUp_L,   "BrowsUp_L")
		Anim(B_BrowsUp_R,   "BrowsUp_R")
		Anim(B_BrowsUpSide_L,   "BrowsUpSide_L")
		Anim(B_BrowsUpSide_R,   "BrowsUpSide_R")
		Anim(B_Mrkani_L,    "Mrkani_L")
		Anim(B_Mrkani_R,    "Mrkani_R")
		Anim(B_Privreni_L,  "Privreni_L")
		Anim(B_Privreni_R,  "Privreni_R")
		Anim(B_Angry,       "Angry")
		Anim(B_Sad,     "Sad")
		Anim(B_AI,      "AI")
		Anim(B_BMP,     "BMP")
		Anim(B_CDG,     "CDG")
		Anim(B_E,       "E")
		Anim(B_FV,      "FV")
		Anim(B_O,       "O")
		Anim(B_QW,      "QW")
		Anim(B_U,       "U")
	}


	//-----------------------------------------------------------------
	// Name: StartAnim : play animation with sound
	//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}


	//-----------------------------------------------------------------
	// Name: StartAnim : play animation with sound
	//-----------------------------------------------------------------
	void StartAnim()
	{
		if (m_bReady)
		{
			SelectObject(m_AnimObject, m_MorphObject)
			SetAnimSlot(m_AnimObject, 0, m_Dialogue, 0, 0, NULL, 30, AF_ONCE)
			PlaySound(this, m_Sound, SF_ONCE)
		}
	}

	//-----------------------------------------------------------------
	// Name: MiscTestDialogue -> loads object with morphs, sounds and
	// lipsync
	//-----------------------------------------------------------------
	void MiscTestDialogue(string objectname, string morphname, string soundname)
	{
		Print ("Wait, loading object...")
		m_bReady = false

		SetFlags(this,TFL_SOLID|TFL_OBBCOLLISION)
		ActivateMethod = 0

		m_strObject     = objectname
		m_strMorphName  = morphname
		m_strSoundName  = soundname

		ClassType = ClassTypeMisc

		string tmp = "anm/morphs/" + m_strSoundName + ".anm"

		m_Object        = GetObject(m_strObject)
		m_MorphObject   = GetObject(m_strMorphName)
		m_Dialogue      = GetObject(tmp)

		m_Sound         = GetSoundEx(m_strSoundName, 1, SF_3D)

		m_AnimObject    = new ExtClass

		if (m_Object && m_Dialogue && m_Sound && m_AnimObject)
		{
			SelectObject(this, m_Object)


			SetFlags(this,TFL_VISIBLE)
			SetEventMask(this, 0)//EV_LOADED|EV_FRAME)

			SelectObject(m_AnimObject, m_MorphObject)

			BumpFrame(this, 0, 0)
			m_bReady = true
			Print ("Successfuly loaded...")
			} else
		{
				ClearEventMask(this, EV_ALL)
		}

	}
 

		//-----------------------------------------------------------------
		// Name: ~MiscTestDialogue : clear all
		//-----------------------------------------------------------------
		void ~MiscTestDialogue()
		{
		ReleaseObject(m_Object, 0)
		ReleaseObject(m_Dialogue, 0)
		ReleaseObject(m_MorphObject, 0)

		ReleaseSound(m_Sound)
		delete m_AnimObject
	}

}
