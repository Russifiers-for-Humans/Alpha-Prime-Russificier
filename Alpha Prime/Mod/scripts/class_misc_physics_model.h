//=================================================================================
class creature_ragdoll extends CreatureClass
{
	float angle2, angle3
	vobject RagdollObject
//	rigidbody_rope DRope
//	string FixationBones
	string ragdoll
	string pose
	string material
	float weight

#ifdef EDITOR
//-----------------------------------------------------------------
	void SetOrientation(vector pos, float angle)	//nech v editore nerobi nic
	{
	}
	
	float SetYaw(float angle)
	{
	}

	void SetInitOrientation()
	{
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		angle2 = atof(GetKeyValue("angle2"))
		angle3 = atof(GetKeyValue("angle3"))
		vector Angs = Vector(angle3, angle, angle2)

		ClearFlags(this, TFL_FULLMATRIX)
		SetOrigin(this, coords)
		SetAngles(this, Angs)
		UpdateEntity(this)
	}

//-----------------------------------------------------------------------------------
	void EditorOpened()
	{
		if(ragdollActive)
		{
			dRagdollDestroy(this)
			ragdollActive = false
		}

		if(pose)
		{
			vobject anim = AddPAnim(pose)
			SetAnimSlot(this, 0, anim, 0, 0, NULL, 30, AF_ONCE)
			SetFrame(this, 0, 0)
			BumpFrame(this, 0, 0)
		}
		UpdateEntity(this)
		SetInitOrientation()
	}
/*
//-----------------------------------------------------------------------------------
	void AfterReloadInEditor()
	{
		SetInitOrientation()
	}
*/
//-----------------------------------------------------------------------------------
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		ClearFlags(this, TFL_FULLMATRIX)
		SetOrigin(this, Pos)
		SetAngles(this, Angs)
		UpdateMainVisualizer()
		UpdateEntity(this)
	}
#endif
/*
//-----------------------------------------------------------------
	void CreateFixationJoints()
	{
		string Parms[32]
		int NumParms = ParseString(FixationBones, Parms)

		for(int n = 0; n < NumParms; n++)
		{
			if(Parms[n] != ",")
			{
				string BoneName = "B_" + Parms[n]
				int BoneValue = -1
				
				if(GetValue(NULL, BoneName, 0, BoneValue))
				{
					
				}
				else
				{
					string Err = "Not existing bone name " + Parms[n] + "in FixationBones key of " + _name
					DPrint(Err)	
				}
			}
		}		
	}
*/
//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)

#ifdef GERMAN
		return
#endif

#ifdef EDITOR
		if(!World.EditorActive)	//ragdol vytvarat iba v playmode
		{
#endif
			if(!ragdollActive)
				InitRagdoll(ragdoll, weight, material)
#ifdef EDITOR
		}
#endif
//		CreateFixationJoints()
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
#ifndef GERMAN
		ClearEventMask(this, 0xffffffff)
		SetEventMask(this, EV_LOADED|EV_ONLOAD|EV_ONSAVE)
		SetFlags(this, TFL_VISIBLE|TFL_SOLID|TFL_DYNAMICBBOX)
		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
		SetKind(this, MONSTER)

		if(material == "")
			material = "material/flesh"
			
		if(model == "")
//			model = "obj/creatures/securityspider01/securityspider01.xob"
			model = "obj/creatures/lighttrooper/lighttrooper.xob"

		if(!ragdoll)
//			ragdoll = "spacesuit"
			ragdoll = "humanoid2"
//			ragdoll = "securityspider"

		if(weight == 0)
			weight = 50

		RagdollObject = GetObject(model)
		SetObject(RagdollObject, _materialremap)

		if(pose)
		{
			SetAnimSlot(this, 0, AddPAnim(pose), 0, 0, NULL, 30, AF_ONCE)
			SetFrame(this, 0, 0)
			BumpFrame(this, 0, 0)
			UpdateEntity(this)
		}
#endif
		status = STATE_DEAD
	}


//-----------------------------------------------------------------
	int Shoot(ShotStorage shot)
	{
#ifndef GERMAN
		float dmg = shot.damage		//kolko energie to ma zobrat
		vector dir = shot.dir
		vector pos = shot.to	
		float pow = shot.power

		if(shot.HurtType == HT_SHOT)
		{
			if(shot.Bone >= 0 && RBones[shot.Bone])
			{
				pow *= shot.NumBullets * 350
				clamp pow<500, 3500>
	
				dBodyApplyImpulseAt(RBones[shot.Bone], shot.dir * pow, shot.to)
			}
			return 1
		}

		if(shot.HurtType == HT_EXPLOSION)
		{
			float	scale
			pow *= ExplosionImpactScale	//koli moznosti vyscalovat vsetky bodies v hre
			pow *= 0.6							//na potvory je to vela

			WakeupRagdoll()
			ApplyExplosionOnRagdoll(shot.from, shot.radius, pow)
			return 1
		}
#endif
	}

//-----------------------------------------------------------------
	void creature_ragdoll()
	{
		flags |= FL_NOTEST
		SetFlags(this, TFL_TOUCHTRIGGERS)
	}

//-----------------------------------------------------------------
	void ~creature_ragdoll()
	{
		if(RagdollObject)
			ReleaseObject(RagdollObject, 0)
	}
}

const int NEED_SHOOT_EVENT_FLAG = 2048	//12-ty bit
//=================================================================================
class misc_physics_model extends RigidBodyClass
{
	int flags
	string model
	float angle, angle2, angle3
	vector coords
	vobject MainObject
	string OnShoot

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

//---------------------------------------------------------------------------------
	void M_EnableShootEvent(ExtClass other)
	{
		if(!flags & NEED_SHOOT_EVENT_FLAG)
			flags |= NEED_SHOOT_EVENT_FLAG
	}

//---------------------------------------------------------------------------------------------------------
    int CanActivate(int fl, ExtClass other)
    {
        if(ActivateMethod == ACT_USEKEY)
            return ACT_ENABLE

        if(ActivateMethod == ACT_USEKEY_DISP)
        {
            if(Display && Display.CanActivate(0, this) == ACT_ENABLE)
                return ACT_ENABLE
        }
        
        return ACT_DISABLE
    }

//---------------------------------------------------------------------------------
	int Shoot(ShotStorage shot)	//kopia z RigidBodyClassu + nieco navyse. mozno to hodit do inej funkcie
	{
		float ImpactPower

		if(shot.HurtType == HT_SHOT)
		{
			ImpactPower = shot.power * ShootImpactScale
	
			if(rb)
			{
				ImpactPower *= weaponscale
				float ke = dBodyGetKineticEnergy(this)
	
				if(ke > 10)	//aby sa nic nepohybovalo extremnymi rychlostami
					ImpactPower *= 0.1

				if(World.ManipEntity == this)
					ImpactPower *= 0.1

				vector vec = shot.dir * ImpactPower
				dBodyApplyImpulseAt(this, vec, shot.to)
			}
			
			if(flags & NEED_SHOOT_EVENT_FLAG)
			{
				flags &= 0xffffffff - NEED_SHOOT_EVENT_FLAG
				CallMethods(this, OnShoot)
			}
			return 1
		}
		
		if(shot.HurtType == HT_EXPLOSION)
		{
			ImpactPower = shot.power * ExplosionImpactScale
			
			if(rb)
			{
				vector Org = GetCenterOfEntity(this)
				vector ForceDir = Org - shot.from
				float dist = VectorNormalize(ForceDir)
				float scale = dist / shot.radius
				clamp scale<0, 1>
				scale = 1 -scale
				ImpactPower += ImpactPower * scale
				float MassScale = mass / 30
				
				if(MassScale > 2)
					MassScale = 2
					
				ImpactPower *= MassScale	//lahsie telesa musia dostat mensiu silu inac su velmi velke rozdiely
				
				if(World.ManipEntity == this)
					ImpactPower *= 0.01

				vector Force = ForceDir * ImpactPower
				dBodyApplyImpulseAt(this, Force, Org + "0 0 10")
			}
			
			if(flags & NEED_SHOOT_EVENT_FLAG)
			{
				flags &= 0xffffffff - NEED_SHOOT_EVENT_FLAG
				CallMethods(this, OnShoot)
			}
			return true
		}
		
		if(shot.HurtType == HT_FLAME)
		{
			dBodyApplyForce(this, shot.dir * shot.power)
			
			if(flags & NEED_SHOOT_EVENT_FLAG)
			{
				flags &= 0xffffffff - NEED_SHOOT_EVENT_FLAG
				CallMethods(this, OnShoot)
			}
			return true
		}
		
		return true
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

		if(flags & 1)
			SetPickable(false)

		if(!model)
			model = "obj/physics/box.xob"

		MainObject = GetObject(model)

		if(MainObject)
		{
			SetObject(MainObject, _materialremap)
		}
		//protoze je uplne jasny, ze se nikdo s nastavovanim _pathblock
		//srat nebude, tak aspon odfiltrujeme maly veci
		if(_pathblock == 0)
		{
			//TODO: cleanup this mess!
			vector mins, maxs
			GetBoundBox(this, mins, maxs)
			maxs -= mins
			if(maxs[0] < MIN_OBSTACLE_SIZE && maxs[1] < MIN_OBSTACLE_SIZE && maxs[2] < MIN_OBSTACLE_SIZE)
				_pathblock = -1
		}

		if(_pathblock != -1)
		{
			SetEventMask(this, EV_EXTRA)
			SetFlags(this, TFL_PATHBLOCK)
		}

		SetTransformEx(Vector(angle3, angle, angle2), coords)
		SetAngles(this, Vector(angle3, angle, angle2))
		if(CreateRigidBody(GEOMTYPE_NONE, "", model))
		{
			ActivateBody(true)
			dBodyCreatePhysXShadow(this, 1, PSF_KINEMATIC|PSF_COLLIDE_CLOTH|PSF_COLLIDE_RB)
		}

		if(display && !Display)
		{
			Display = new func_display(this, display, false)	//musi sa vytvarat az ked ma entita selectnuty object!
			SetEventMask(this, EV_VISIBLE)
	   
			if(ActivateMethod == ACT_ENTITY_DEFAULT)
				ActivateMethod = ACT_USEKEY_DISP
		}
	}

//-----------------------------------------------------------------
	void EOnExtra(MinClass other, int extra)
	{
		//kdykoliv se uklidni, updatuj
		if(!extra & 1)
			UpdateAgent(true)
		//else
			//UpdateAgent(false) //toto neni dobry napad, protoze se obcas aktivuje bedna, kolem ktere potvora bezi
			 //a nasledne zmizi obstacle z pathfinderu, coz vede ke kolizi...
	}

//-----------------------------------------------------------------
	void misc_physics_model()
	{
		agent = NULL
		ClassType = 666//ClassTypeMisc
		SetEventMask(this, EV_INIT | EV_LOADED)
		SetFlags(this, TFL_VISIBLE)
	}

//-----------------------------------------------------------------
	void Cleanup()
	{
		dBodyDestroyPhysXShadow(this)
		RemoveBodyAndGeom()

		if(MainObject)
			ReleaseObject(MainObject, 0)
	}

//-----------------------------------------------------------------
	void ~misc_physics_model()
	{
		Cleanup()
	}
}

class misc_physics_bmodel extends misc_physics_model
{
}

//=================================================================
class misc_shooting_shield extends misc_physics_model
{
	int EnabledSide
	float row0[4]
	float row1[4]
	float UVShift
	float UVScale

//-----------------------------------------------------------------
	void M_SetShootingSide(ExtClass other)
	{
		EnabledSide = atoi(other._value)
		clamp EnabledSide<0, 1>
	}

//-----------------------------------------------------------------
	int IsShot(vector rayorigin, vector raydirection)
	{
		float dot = GetMatrix(this, 1) * raydirection
		int side = EnabledSide

		if(dot > 0)
			side = 1 - side

		if(side)
			return true

		return false
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		UVShift += ftime * 5// * 100
		
		UVScale = 10.0//sin(UVShift * DEG2RAD) + 1 * 0.5
	}

	//-----------------------------------------------------------------
	void BeforeInit()
	{
		if(!model)
			model = "obj/weapons/energy_wall.xob"
	}
	
//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		ActivateBody(false)
		vector mins, maxs
		GetBoundBox(this, mins, maxs)
		mins[1] = -0.04					//pre presne detekcie zasahov to zase znizime
		maxs[1] = 0.04
		SetBoundBox(this, mins, maxs)
	}

//-----------------------------------------------------------------
	void misc_shooting_shield()
	{
		ClassType = ClassTypeMiscShootingShield
		row0[0] = 0.008
		row0[1] = 0.0
		row0[2] = 0.0
		row0[3] = 0.0

		row1[0] = 0.0
		row1[1] = 0.008
		row1[2] = 0.0
		row1[3] = 0.0
		
		Color = 0xffffe0d0	//abgr
		SetEventMask(this, EV_FRAME)
	}
}







int g_NumGlassDebrises

class GlassDebris extends RigidBodyClass
{
	float timer

	void EOnTick(MinClass other, int extra)
	{
		if(g_NumGlassDebrises > 8)
			timer -= TICK_PERIOD * 3
		else
			timer -= TICK_PERIOD

		if(timer <= 0 && Dependencies == 0)
			delete this
	}

	void GlassDebris()
	{
		SetEventMask(this, EV_TICK)
		SetFlags(this, TFL_VISIBLE)
		timer = 10	//10 sekund
		SetPickable(false)
		g_NumGlassDebrises++
	}

	void ~GlassDebris()
	{
		RemoveBodyAndGeom()
		g_NumGlassDebrises--
	}
}

class GlassDebrisX extends ExtClass
{
	float timer

	void EOnTick(MinClass other, int extra)
	{
		timer -= TICK_PERIOD

		if(timer <= 0)
		{
			NxActorDestroy(this)
			delete this
		}
	}

	void GlassDebrisX()
	{
		SetEventMask(this, EV_TICK)
		SetFlags(this, TFL_VISIBLE|TFL_FULLMATRIX|TFL_NOSHADOW)
		timer = 15
	}
}

//stavy
const int GL_FULL = 7		//cele sklo priesvitne
const int GL_FULL2 = 6		//cele sklo popraskane

//const int GL_EMPTY = 14		//uplne rozbite sklo (nezostalo nic)
const int GL_BROKEN0 = 0		//nastiepene sklo
const int GL_BROKEN1 = 1		//nastiepene sklo
const int GL_BROKEN2 = 2		//nastiepene sklo
const int GL_BROKEN3 = 3		//nastiepene sklo
const int GL_BROKEN4 = 4		//nastiepene sklo
const int GL_BROKEN5 = 5		//nastiepene sklo
const int GL_NONE = 127		//nedefinovane nic

//rotacie
const int GL_BOTTOM = 0		//normalny stav
const int GL_LEFT = 1
const int GL_TOP = 2
const int GL_RIGHT = 3


const int TypeMasks[4] = {0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff}

class misc_physics_glass extends RigidBodyClass
{
	vobject GlassObject
	vector coords
	float angle
	float angle2
	float angle3
	float width, height
	float gridsize
	int skin
	int flags

	trigger_motion2 MTrigger
	vobject DebrisObj[10]
	int NumHSegments
	int NumVSegments
	int NumAllSegments
	int NumVert
	int NumInd
	int NumShots
	hsound Sound
	vector Force
	int ShotSegmentX
	int ShotSegmentY
	int ShotLayer
	int BreakAction	//co sposobilo break. strela, dotyk inej entity, vybuch, fyzicky naraz

	int SegMaskIndex[16][16]	//typ textury pre kazdy segment
	int SegMaskRot[16][16]		//otocenie textury pre kazdy segment 0 - 3

#ifdef EDITOR
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		if(!flags & 1)
		{
			SetTransformEx(Angs, Pos)
		}
		else
		{
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
#endif

//-------------------------------------------------------------------------
	int GetTypeFor(int X, int Y, int Z)
	{
		int shift = 24 - (Z * 8)
		int temp = SegMaskIndex[X][Y]
		int mask = TypeMasks[Z]
		temp = temp & mask
		temp = temp >> shift
		return temp
	}

//-------------------------------------------------------------------------
	void SetTypeFor(int X, int Y, int Z, int type)
	{
		int shift = 24 - (Z * 8)
		int temp = SegMaskIndex[X][Y]
		type = type << shift
		int mask = TypeMasks[Z]
		temp &= -1 ^ mask
		temp = temp | type
		SegMaskIndex[X][Y] = temp
	}

//-------------------------------------------------------------------------
	int GetRotFor(int X, int Y, int Z)
	{
		int shift = 24 - (Z * 8)
		int temp = SegMaskRot[X][Y]
		int mask = TypeMasks[Z]
		temp = temp & mask
		temp = temp >> shift
		return temp
	}

//-------------------------------------------------------------------------
	void SetRotFor(int X, int Y, int Z, int rot)
	{
		int shift = 24 - (Z * 8)
		int temp = SegMaskRot[X][Y]
		rot = rot << shift
		int mask = TypeMasks[Z]
		temp &= -1 ^ mask
		temp = temp | rot
		SegMaskRot[X][Y] = temp
	}

//-------------------------------------------------------------------------
	void RegenerateGlassObject()
	{
		SelectObject(this, VNULL)
		if(GlassObject)
		{
			ReleaseObject(GlassObject, 0)
			GlassObject = VNULL
		}

		vector vertex1
		vector vertex2
		vector vertex3
		vector vertex4
		NumVert = 0
		NumInd = 0
		int SegIndex
		int CurMaskIndex
		int CurRot

		int FirstVert = 0
		int SecondVert = 0
		int ThirdVert = 0
		int FourthVert = 0

		for(int v = 0; v < NumVSegments; v++)
		{
			for(int h = 0; h < NumHSegments; h++)
			{
				for(int n = 0; n < 4; n++)
				{
					SegIndex = GetTypeFor(h, v, n)

					if(SegIndex != GL_NONE)	//ak je nadefinovane viac typov na jeden segment tak vytvarame pre kazdy typ novu vrstvu
					{
						vertex1[0] = h * gridsize
						vertex1[1] = 0
						vertex1[2] = v * gridsize + gridsize

						vertex2[0] = vertex1[0] + gridsize
						vertex2[1] = 0
						vertex2[2] = vertex1[2]

						vertex3[0] = vertex1[0] + gridsize
						vertex3[1] = 0
						vertex3[2] = vertex1[2] - gridsize

						vertex4[0] = vertex1[0]
						vertex4[1] = 0
						vertex4[2] = vertex1[2] - gridsize

						FirstVert = NumVert
						SecondVert = NumVert + 1
						ThirdVert = NumVert + 2
						FourthVert = NumVert + 3

						g_TempVerts[NumVert++] = vertex1
						g_TempVerts[NumVert++] = vertex2
						g_TempVerts[NumVert++] = vertex3
						g_TempVerts[NumVert++] = vertex4

						g_TempIndices[NumInd++] = FirstVert
						g_TempIndices[NumInd++] = SecondVert
						g_TempIndices[NumInd++] = ThirdVert

						g_TempIndices[NumInd++] = ThirdVert
						g_TempIndices[NumInd++] = FourthVert
						g_TempIndices[NumInd++] = FirstVert

						CurRot = GetRotFor(h, v, n)
						CurMaskIndex = SegIndex * 8

						g_TempUVs[FirstVert * 2] 		= GlassMaskUV[CurRot][CurMaskIndex + 0]
						g_TempUVs[FirstVert * 2 + 1] 	= GlassMaskUV[CurRot][CurMaskIndex + 1]

						g_TempUVs[SecondVert * 2] 		= GlassMaskUV[CurRot][CurMaskIndex + 2]
						g_TempUVs[SecondVert * 2 + 1] 	= GlassMaskUV[CurRot][CurMaskIndex + 3]

						g_TempUVs[ThirdVert * 2]	 		= GlassMaskUV[CurRot][CurMaskIndex + 4]
						g_TempUVs[ThirdVert * 2 + 1] 	= GlassMaskUV[CurRot][CurMaskIndex + 5]

						g_TempUVs[FourthVert * 2]	 	= GlassMaskUV[CurRot][CurMaskIndex + 6]
						g_TempUVs[FourthVert * 2 + 1] 	= GlassMaskUV[CurRot][CurMaskIndex + 7]
					}
				}
			}
		}

		if(NumVert == 0)
			delete this

		int nverts[1]
		int numindices[1]
		string shaders[1]
		nverts[0] = NumVert
		numindices[0] = NumInd
		shaders[0] = "textures/glass/brokenglass" + itoa(skin)

		GlassObject = CreateXOB(1, nverts, numindices, shaders)
		SetObject(GlassObject, _materialremap)
		UpdateIndices(GlassObject, 0, g_TempIndices)	//najprv sa musia updatovat indexy!
		UpdateVertsEx(this, 0, g_TempVerts, g_TempUVs)

//		ActivateBody(true)
		UpdateEntity(this)
	}

//-------------------------------------------------------------------------
	void GenerateStartObject()
	{
		vector Vertices[4]
		int	 Indices[6]
		float	 UVs[8]

		Vertices[0] = ZeroVec
		Vertices[1] = Vector(0, 0, height)
		Vertices[2] = Vector(width, 0, height)
		Vertices[3] = Vector(width, 0, 0)

		Indices[0] = 0
		Indices[1] = 1
		Indices[2] = 2
		Indices[3] = 2
		Indices[4] = 3
		Indices[5] = 0

		UVs[0] = 0.75
		UVs[1] = 1
		UVs[2] = 0.75
		UVs[3] = 0.5
		UVs[4] = 1
		UVs[5] = 0.5
		UVs[6] = 1
		UVs[7] = 1

		int nverts[1]
		int numindices[1]
		string shaders[1]
		nverts[0] = 4
		numindices[0] = 6
		shaders[0] = "textures/glass/brokenglass" + itoa(skin)

		GlassObject = CreateXOB(1, nverts, numindices, shaders)
		SetObject(GlassObject, _materialremap)
		UpdateIndices(GlassObject, 0, Indices)	//najprv sa musia updatovat indexy!
		UpdateVertsEx(this, 0, Vertices, UVs)

		UpdateEntity(this)
	}

//-------------------------------------------------------------------------
	bool IsBrokenType(int type)
	{
		if(type == GL_BROKEN0 || type == GL_BROKEN1 || type == GL_BROKEN2 || type == GL_BROKEN3 || type == GL_BROKEN4 || type == GL_BROKEN5)
			return true

		return false
	}

//-------------------------------------------------------------------------
	bool IsFullType(int type)
	{
		if(type == GL_FULL || type == GL_FULL2)
			return true

		return false
	}

//-------------------------------------------------------------------------
	void ChangeFullType()
	{
		for(int v = 0; v < NumVSegments; v++)
		{
			for(int h = 0; h < NumHSegments; h++)
				SetTypeFor(h, v, 0, 6)		//nahodne rozlozime cele ale popraskane skla
		}
	}

//-------------------------------------------------------------------------
	void ThrowDebris(int X, int Y)
	{
		vector mat[4], pos
		GetMatrix4(this, mat)

		float HalfGridSize = gridsize * 0.5
		float QuarterGridSize = gridsize * 0.25

		pos[0] = gridsize * (float)X + HalfGridSize
		pos[1] = 0
		pos[2] = gridsize * (float)Y + HalfGridSize

		if(X == 0)
			pos[0] = pos[0] + QuarterGridSize

		if(Y == 0)
			pos[2] = pos[2] + QuarterGridSize

		int NumH = NumHSegments - 1
		int NumV = NumVSegments - 1

		if(X == NumH)
			pos[0] = pos[0] - QuarterGridSize

		if(Y == NumV)
			pos[2] = pos[2] - QuarterGridSize

		pos = VectorMatrixMultiply4(mat, pos)
		mat[3] = pos

		if(g_physx)
		{
			//neco efektivnejsiho
			GlassDebrisX debx = new GlassDebrisX()

			SetMatrix(debx, mat)
			SelectObject(debx, DebrisObj[rand(0, 10)])
			NxActorCreate(debx, ZeroVec, BC_OBB, true, 1.5, 0, 0)
			SetVelocity(debx, Force * 10)
		}
		else
		{
			GlassDebris deb = new GlassDebris()
			SelectObject(deb, DebrisObj[rand(0, 10)])
	
			deb.SetMass(0.4)
			deb.SetTransform(mat)
	
			if(deb.CreateRigidBody(GEOMTYPE_BOX, "material/glassdebris", ""))
			{
				deb.ActivateBody(true)
		
				dBodyInteractionLayer(deb, ET_DEBRIS)
		
				vector offset
				offset[0] = frand(-0.03, 0.03)
				offset[1] = frand(-0.03, 0.03)
				offset[2] = frand(-0.03, 0.03)
		
				dBodyApplyImpulseAt(deb, Force, pos + offset)
			}
		}
	}

//-------------------------------------------------------------------------
	bool IsInRange(int X, int Y)
	{
		int MaxX = NumHSegments - 1
		int MaxY = NumVSegments - 1

		if(X < 0 || Y < 0 || X > MaxX || Y > MaxY)
			return false

		return true
	}

//-------------------------------------------------------------------------
	void BreakGlass(local int X, local int Y, local int LayerIndex, bool NoRecursions)
	{
		local int OldType = GetTypeFor(X, Y, LayerIndex)
		local int OldRot = GetRotFor(X, Y, LayerIndex)

		local int LeftX = X - 1
		local int RightX = X + 1
		local int TopY = Y + 1
		local int BottomY = Y - 1

		if(RightX >= NumHSegments)
			RightX = -1

		if(TopY >= NumVSegments)
			TopY = -1

		local bool IsFull = false
		if(IsFullType(GetTypeFor(X, Y, 0)))
			IsFull = true

		if(IsFull)	//ak je cele tak ma celych aj vsetkych susedov
		{
			SetTypeFor(X, Y, 0, rand(0, 6))
			SetTypeFor(X, Y, 1, rand(0, 6))
			SetTypeFor(X, Y, 2, rand(0, 6))
			SetTypeFor(X, Y, 3, rand(0, 6))

			SetRotFor(X, Y, 0, GL_BOTTOM)
			SetRotFor(X, Y, 1, GL_LEFT)
			SetRotFor(X, Y, 2, GL_TOP)
			SetRotFor(X, Y, 3, GL_RIGHT)

			int Debris = 1

			if(BreakAction == 2)	//rozbija to vybuch
				Debris = rand(0, 4)

			if(Debris > 0)
				ThrowDebris(X, Y)

			if(NoRecursions)
				return

			local int tmp		//kriz okolo trafeneho segmentu musi mat cesty k okrajom. inac rozbijame cele rady
			local int tmpX
			local int tmpY
			local int n
			local bool LeftWay = true
			local bool RightWay = true
			local bool TopWay = true
			local bool BottomWay = true

			tmpX = X - 1
			tmpY = Y

			while(tmpX >= 0)
			{
				if(!IsFullType(GetTypeFor(tmpX, Y, 0)))
				{
					LeftWay = false
					for(n = X - 1; n > tmpX; n--)
					{
						BreakGlass(n, Y, 0, true)

						tmp = Y + 1
						if(IsInRange(X, tmp) && IsFullType(GetTypeFor(n, tmp, 0)) && rand(0, 2) == 0)	//nohodne nieco rozbijeme potri tom aby to nevyzeralo linearne
							BreakGlass(n, tmp, 0, true)

						tmp = Y - 1
						if(IsInRange(X, tmp) && IsFullType(GetTypeFor(n, tmp, 0)) && rand(0, 2) == 0)
							BreakGlass(n, tmp, 0, true)
					}
					break
				}
				tmpX--
			}

			tmpX = X + 1
			tmpY = Y

			while(tmpX < NumHSegments)
			{
				if(!IsFullType(GetTypeFor(tmpX, Y, 0)))
				{
					RightWay = false
					for(n = X + 1; n < tmpX; n++)
					{
						BreakGlass(n, Y, 0, true)

						tmp = Y + 1
						if(IsInRange(X, tmp) && IsFullType(GetTypeFor(n, Y + 1, 0)) && rand(0, 2) == 0)
							BreakGlass(n, Y + 1, 0, true)

						tmp = Y - 1
						if(IsInRange(X, tmp) && IsFullType(GetTypeFor(n, Y - 1, 0)) && rand(0, 2) == 0)
							BreakGlass(n, Y - 1, 0, true)
					}
					break
				}
				tmpX++
			}

			tmpX = X
			tmpY = Y - 1

			while(tmpY >= 0)
			{
				if(!IsFullType(GetTypeFor(X, tmpY, 0)))
				{
					BottomWay = false
					for(n = Y - 1; n > tmpY; n--)
					{
						BreakGlass(X, n, 0, true)

						tmp = X + 1
						if(IsInRange(tmp, Y) && IsFullType(GetTypeFor(tmp, n, 0)) && rand(0, 2) == 0)
							BreakGlass(tmp, n, 0, true)

						tmp = X - 1
						if(IsInRange(tmp, Y) && IsFullType(GetTypeFor(tmp, n, 0)) && rand(0, 2) == 0)
							BreakGlass(tmp, n, 0, true)
					}
					break
				}
				tmpY--
			}

			tmpX = X
			tmpY = Y + 1

			while(tmpY < NumVSegments)
			{
				if(!IsFullType(GetTypeFor(X, tmpY, 0)))
				{
					TopWay = false
					for(n = Y + 1; n < tmpY; n++)
					{
						BreakGlass(X, n, 0, true)

						tmp = X + 1
						if(IsInRange(tmp, Y) && IsFullType(GetTypeFor(tmp, n, 0)) && rand(0, 2) == 0)
							BreakGlass(tmp, n, 0, true)

						tmp = X - 1
						if(IsInRange(tmp, Y) && IsFullType(GetTypeFor(tmp, n, 0)) && rand(0, 2) == 0)
							BreakGlass(tmp, n, 0, true)
					}
					break
				}
				tmpY++
			}
			return
		}

		if(!IsFull)
		{
			SetTypeFor(X, Y, LayerIndex, GL_NONE)	//zasiahnuty kusok musi vypadnut

			if(LayerIndex == 1 && LeftX >= 0)	//sused vlavo
			{
				if(IsFullType(GetTypeFor(LeftX, Y, 0)))	//sused je cely
					BreakGlass(LeftX, Y, 3, false)

				SetTypeFor(LeftX, Y, 3, GL_NONE)
			}

			if(LayerIndex == 3 && RightX >= 0)	//sused vpravo
			{
				if(IsFullType(GetTypeFor(RightX, Y, 0)))
					BreakGlass(RightX, Y, 1, false)

				SetTypeFor(RightX, Y, 1, GL_NONE)
			}

			if(LayerIndex == 2 && TopY >= 0)	//sused hore
			{
				if(IsFullType(GetTypeFor(X, TopY, 0)))
					BreakGlass(X, TopY, 0, false)

				SetTypeFor(X, TopY, 0, GL_NONE)
			}

			if(LayerIndex == 0 && BottomY >= 0)	//sused dole
			{
				if(IsFullType(GetTypeFor(X, BottomY, 0)))
					BreakGlass(X, BottomY, 2, false)

				SetTypeFor(X, BottomY, 2, GL_NONE)
			}
		}
	}

//-------------------------------------------------------------------------
	void HandleGlassShape()
	{
		int TypeLeft
		int TypeRight
		int TypeTop
		int TypeBottom

		int TLeft
		int TRight
		int TTop
		int TBottom

		int LeftX
		int RightX
		int TopY
		int BottomY
		int v, h
		int VertNum = NumVSegments - 1
		int HorzNum = NumHSegments - 1
		bool LeftWay
		bool RightWay
		bool TopWay
		bool BottomWay

		for(v = 1; v < VertNum; v++)		//kazdy segment ktory neni okrajovy musi mat aspon jednu priamu cestu k okraju
		{
			for(h = 1; h < HorzNum; h++)
			{
				if(IsFullType(GetTypeFor(h, v, 0)))
				{
					LeftWay = true
					RightWay = true
					TopWay = true
					BottomWay = true

					LeftX = h - 1
					while(LeftX >= 0)
					{
						if(!IsFullType(GetTypeFor(LeftX, v, 0)))
						{
							LeftWay = false
							break
						}
						LeftX--
					}

					RightX = h + 1
					while(RightX < NumHSegments)
					{
						if(!IsFullType(GetTypeFor(RightX, v, 0)))
						{
							RightWay = false
							break
						}
						RightX++
					}

					TopY = v - 1
					while(TopY >= 0)
					{
						if(!IsFullType(GetTypeFor(h, TopY, 0)))
						{
							TopWay = false
							break
						}
						TopY--
					}

					BottomY = v + 1
					while(BottomY < NumHSegments)
					{
						if(!IsFullType(GetTypeFor(h, BottomY, 0)))
						{
							BottomWay = false
							break
						}
						BottomY++
					}

					if(!LeftWay && !RightWay && !TopWay && !BottomWay)	//ak neni ziadna cesta k okraju. musime to rozbit
						BreakGlass(h, v, 0, true)
				}
			}
		}

		for(v = 0; v < NumVSegments; v++)		//nemozu byt dva breaknute vedla seba lebo vznikaju nerealne stavy. prejdeme vsetkych susedov
		{
			for(h = 0; h < NumHSegments; h++)
			{
				LeftX = h - 1
				RightX = h + 1
				TopY = v + 1
				BottomY = v - 1

				if(RightX >= NumHSegments)
					RightX = -1

				if(TopY >= NumVSegments)
					TopY = -1

				TypeBottom = GetTypeFor(h, v, 0)
				TypeLeft = GetTypeFor(h, v, 1)
				TypeTop = GetTypeFor(h, v, 2)
				TypeRight = GetTypeFor(h, v, 3)

				if(LeftX >= 0)	//sused vlavo
				{
					if(IsBrokenType(TypeLeft) && IsBrokenType(GetTypeFor(LeftX, v, 3)))
					{
						SetTypeFor(LeftX, v, 3, GL_NONE)
						SetTypeFor(h, v, 1, GL_NONE)
					}
				}

				if(RightX >= 0)	//sused vpravo
				{
					if(IsBrokenType(TypeRight) && IsBrokenType(GetTypeFor(RightX, v, 1)))
					{
						SetTypeFor(RightX, v, 1, GL_NONE)
						SetTypeFor(h, v, 3, GL_NONE)
					}
				}

				if(TopY >= 0)	//sused hore
				{
					if(IsBrokenType(TypeTop) && IsBrokenType(GetTypeFor(h, TopY, 0)))
					{
						SetTypeFor(h, TopY, 0, GL_NONE)
						SetTypeFor(h, v, 2, GL_NONE)
					}
				}

				if(BottomY >= 0)	//sused dole
				{
					if(IsBrokenType(TypeBottom) && IsBrokenType(GetTypeFor(h, BottomY, 2)))
					{
						SetTypeFor(h, BottomY, 2, GL_NONE)
						SetTypeFor(h, v, 0, GL_NONE)
					}
				}
			}
		}
	}

//-------------------------------------------------------------------------
	void CreateTouchTrigger()
	{
		if(MTrigger)
			return

		vector mins, maxs
		GetBoundBox(this, mins, maxs)

		MTrigger = new trigger_motion2(this, mins, maxs)
		SetFlags(MTrigger, TFL_OBBCOLLISION)
		SetOrigin(MTrigger, coords)
		SetAngles(MTrigger, Vector(angle3, angle, angle2))
	}

//-------------------------------------------------------------------------
	void BreakBox(vector pos, vector mins, vector maxs)
	{
		NumShots++

		if(NumShots == 1 && !flags & 1)	//prvy zasah ked je sklo fyzikalne
		{
			RemoveBodyAndGeom()	//zrusime
			ChangeFullType()		//mozeme zrusit fyziku
			CreateTouchTrigger()
		}

		vector offset = pos - coords
		vector vec
		vector mat[3]
		GetMatrix4(this, mat)
		vec = VectorMatrixInvMultiply3(mat, offset)
		offset = vec

		bool NeedTrans = false
		vector axis = mat[0]
		if(axis[0] == 0 || axis[0] == 1 || axis[0] == -1)
		{
			if(axis[1] == 0 || axis[1] == 1 || axis[1] == -1)
			{
				if(axis[2] == 0 || axis[2] == 1 || axis[2] == -1)
					NeedTrans = true
			}
		}

		if(NeedTrans)
		{
			vector tmins, tmaxs
			tmins = VectorMatrixMultiply3(mat, mins)
			tmaxs = VectorMatrixMultiply3(mat, maxs)
			mins = tmins
			maxs = tmaxs

			float f = mins[0]

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

//		AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, coords + mins, coords + maxs)

		float MinX, MaxX
		float MinY, MaxY
		MinX = offset[0] + mins[0]
		MaxX = offset[0] + maxs[0]
		MinY = offset[2] + mins[2]
		MaxY = offset[2] + maxs[2]

		float SegCenterX, SegCenterY
		float HalfGridSize = gridsize * 0.5
		int NumBroken = 0
		BreakAction = 3

		for(int v = 0; v < NumVSegments; v++)
		{
			for(int h = 0; h < NumHSegments; h++)
			{
				SegCenterX = h * gridsize + HalfGridSize
				SegCenterY = v * gridsize + HalfGridSize

				if(SegCenterX > MinX && SegCenterX < MaxX && SegCenterY > MinY && SegCenterY < MaxY)
				{
					if(IsFullType(GetTypeFor(h, v, 0)))
					{
						BreakGlass(h, v, 0, false)
						NumBroken++
					}
				}
			}
		}

		if(NumBroken > 0)
		{
			HandleGlassShape()
			RegenerateGlassObject()
			PlaySound(this, Sound, SF_ONCE)
		}
	}
	

//-------------------------------------------------------------------------
	void Activate(int fl, MinClass other)
	{
		vector mins, maxs, pos
		ExtClass MyParent = GetParent()

		if(MyParent && other == MyParent)
			return

#ifdef EDITOR
		return	//v editore dochadza k extremnym situaciam v suvislosti s triggerom ktory nam toto vola.
#endif

		if(ClassName(other) == "GlassDebris" || other.ClassType == ClassTypeMiscPhysicsGlass)
			return

		pos = GetMatrix(other, 3)

		if(other.ClassType == ClassTypeCreature)
		{
			CreatureClass cc = other
			mins = cc.Mins
			maxs = cc.Maxs
		}
		else
		{
			GetCBoundBox(other, mins, maxs)
			mins = mins - pos
			maxs = maxs - pos
		}
		BreakBox(pos, mins, maxs)
	}

//-----------------------------------------------------------------
	void AfterShoot(int SegmentX, int SegmentY, int Layer)
	{
		NumShots++

		if(NumShots == 1 && !flags & 1)	//prvy zasah ked je sklo fyzikalne
		{
			RemoveBodyAndGeom()	//zrusime
			ChangeFullType()		//mozeme zrusit fyziku
			CreateTouchTrigger()
		}

		BreakGlass(SegmentX, SegmentY, Layer, false)
	}

//-----------------------------------------------------------------
	int IsShot(vector rayorigin, vector raydirection)
	{
		vector plane = GetMatrix(this, 1)
		float dist = GetMatrix(this, 3) * plane

		float vd = plane * raydirection
		float v0 = plane * rayorigin - dist
		float scale = v0 / vd

		if(scale > 0)
			return false	//poloprimka je za plochou

		vector intersect = rayorigin + (raydirection * -scale)

		vector offset = intersect - GetMatrix(this, 3)
		vector vec
		vector mat[3]
		GetMatrix4(this, mat)
		vec = VectorMatrixInvMultiply3(mat, offset)
		offset = vec

		float X = offset[0]	//pozicia zasahu
		float Y = offset[2]

		if(X < 0 || X > width || Y < 0 || Y > height)
			return false

//		vec = coords + vec
//		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords, pos)
//		AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, coords, vec)
//		AddDShape(SHAPE_BBOX, 0xffffffff, 0, intersect - "2 2 2", intersect + "2 2 2")

		int SegmentX = X / gridsize
		int SegmentY = Y / gridsize
	
		int MaxHindex = NumHSegments - 1
		int MaxVindex = NumVSegments - 1
		clamp SegmentX<0, MaxHindex>
		clamp SegmentY<0, MaxVindex>

		if(IsFullType(GetTypeFor(SegmentX, SegmentY, 0)))	//ked rozbijame cely stvorec tak nemusime testovat per pixel
		{
			ShotSegmentX = SegmentX	//len si poznamename ze je mozny zasah. O skutocnom zasahu sa rozhodne az ked bude dotrasovane
			ShotSegmentY = SegmentY
			ShotLayer = 0
			return true
		}

		float SegRateX = X / gridsize
		SegRateX -= (float)SegmentX		//percentualne v trafenom stvorci
		float SegRateY = Y / gridsize
		SegRateY -= (float)SegmentY

		int TexturePtr[]
		int twidth, theight, pitch
		int tex = LockTexture("textures/glass/brokenglass" + itoa(skin), TexturePtr, twidth, theight, pitch)
		int MaxIndex = twidth * theight - 1

		if(!tex)
			return false

		int MaxX = twidth - 1
		int MaxY = theight - 1

		for(int n = 0; n < 4; n++)	//prebehneme vsetky layery a zistime ci sme v niektorom trafili do casti s alfou
		{
			int SegIndex = GetTypeFor(SegmentX, SegmentY, n)

			if(SegIndex == 127)	//prazdne sklo
				continue

			int UVIndex = SegIndex * 8
			int SegRot = GetRotFor(SegmentX, SegmentY, n)

			float SgRateX, SgRateY				//prehodime do inych aby sa neprepisali pri dalsom prechode
			if(SegRot == 1 || SegRot == 3)	//prislusne to otocime
			{
				SgRateX = SegRateY
				SgRateY = SegRateX
			}
			else
			{
				SgRateX = SegRateX
				SgRateY = SegRateY
			}

			if(SegRot == 0 || SegRot == 1)
			{
				SgRateX = 1 - SgRateX
				SgRateY = 1 - SgRateY
			}

			if(SegRot == 0 || SegRot == 2)
				SgRateX = 1 - SgRateX

			float UVX0 = GlassMaskUV[0][UVIndex + 0]
			float UVY0 = GlassMaskUV[0][UVIndex + 1]

			float realRateX = UVX0 + (SgRateX * 0.25)	//percentualne na skutocnej (spolocnej) texture
			float realRateY = UVY0 + (SgRateY * 0.5)

			float rateX = X / width
			float rateY = Y / height

			int TargetPixel

			float ftx = realRateX * (float)twidth
			float fty = realRateY * (float)theight
			int tx = ftx
			int ty = fty

			clamp tx<0, MaxX>
			clamp ty<0, MaxY>

			int TextureIndex = ty * twidth + tx
			
			if(TextureIndex > MaxIndex)
				TargetPixel = 0
			else
				TargetPixel = TexturePtr[TextureIndex] >> 24	//nechame iba alfu

			if(TargetPixel != 0)	//iba uplne nulova alfa je zasah mimo!
			{
				UnlockTexture(tex)
				ShotSegmentX = SegmentX	//len si poznamename ze je mozny zasah. O skutocnom zasahu sa rozhodne az ked bude dotrasovane. bude zavolany shoot()
				ShotSegmentY = SegmentY
				ShotLayer = n
				return true
			}
		}

		UnlockTexture(tex)

		return false
	}

//-------------------------------------------------------------------------
	int Shoot(ShotStorage shot)
	{
		if(shot.HurtType == HT_SHOT)
		{
			Force = shot.dir * 25
			BreakAction = 1
			AfterShoot(ShotSegmentX, ShotSegmentY, ShotLayer)
			HandleGlassShape()
			RegenerateGlassObject()
			PlaySound(this, Sound, SF_ONCE)
			return 1
		}
		
		if(shot.HurtType == HT_EXPLOSION)
		{
			int TargetH = NumHSegments - 0
			int TargetV = NumVSegments - 0
			float lng
			float HalfGridSize = gridsize * 0.5
			vector direction
	
			vector mat[3]
			GetMatrix4(this, mat)
	
			vector SegPos
			int ShotDisabled
			BreakAction = 2
	
			for(int v = 0; v < TargetV; v++)
			{
				for(int h = 0; h < TargetH; h++)
				{
					if(IsFullType(GetTypeFor(h, v, 0))/* && rand(0, 2) == 1*/)
					{
						SegPos[0] = h * gridsize + HalfGridSize
						SegPos[1] = 0
						SegPos[2] = v * gridsize + HalfGridSize
						SegPos = VectorMatrixMultiply3(mat, SegPos)
						SegPos = coords + SegPos
						direction = SegPos - shot.from
						lng = VectorNormalize(direction)
	
						float rate = lng / shot.radius
						ShotDisabled = 1
	
						if(rate < 0.3)
							ShotDisabled = 0	//to co je blizko musi vypadnut
						else
						{
							if(rate < 0.4)
								ShotDisabled = rand(0, 2)
							else
							{
								if(rate < 0.5)
									ShotDisabled = rand(0, 3)
								else
								{
									if(rate < 0.6)
										ShotDisabled = rand(0, 4)	//nahodne nieco nechame na dalsie odsrelovanie ;)
									else
										ShotDisabled = rand(0, 10)
								}
							}
						}
	
						if(ShotDisabled == 0)
						{
							Force = direction * 50
							AfterShoot(h, v, 0)
						}
	//					AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, SegPos + "-2 -2 -2", SegPos + "2 2 2")
	//					AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos, SegPos)
					}
				}
			}
	
			HandleGlassShape()
			RegenerateGlassObject()
			PlaySound(this, Sound, SF_ONCE)
		}
		return 0
	}
/*
//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		if(!rb)
			return

		vector ImpNorm = c.GetNormalImpulse()
		float ImpNormLng = VectorLength(ImpNorm)
		ImpNormLng /= (float)mass

		vector vel = GetVelocity(other)

		if(rb)
		{
			ClearEventMask(this, EV_ONCONTACT)
			RemoveBodyAndGeom()	//zrusime
			ChangeFullType()		//mozeme zrusit fyziku
			HandleGlassShape()
			RegenerateGlassObject()
			CreateTouchTrigger()

//			Force = ImpNorm * 25
//			BreakAction = 4
//			AfterShoot(ShotSegmentX, ShotSegmentY, ShotLayer)
//			HandleGlassShape()
//			RegenerateGlassObject()
//			PlaySound(this, Sound, SF_ONCE)
		}
	}
*/
//-------------------------------------------------------------------------
	void OnParent(ExtClass parent)
	{
		if(MTrigger)
		{
			AddChild(this, MTrigger, 7)	//boli sme naparenteni na nieco ine tak musime so sebou brat aj trigger ;-)
			SetOrigin(MTrigger, ZeroVec)
			SetAngles(MTrigger, ZeroVec)
		}
	}

//-------------------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(skin <= 0)
			skin = 1
			
		//skin = 2

		if(gridsize == 0)
			gridsize = 8

		if(width == 0)
			width = 32

		if(height == 0)
			height = 32

		NumHSegments = width / gridsize
		NumVSegments = height / gridsize
		NumAllSegments = NumHSegments * NumVSegments

		string ErrMsg

		if(NumHSegments > 16)
		{
			ErrMsg = "entity " + _name + " is to large!. please decrease width key!"
			DPrint(ErrMsg)
			return
		}

		if(NumVSegments > 16)
		{
			ErrMsg = "entity " + _name + " is to large!. please decrease height key!"
			DPrint(ErrMsg)
			return
		}

		if(NumAllSegments > 256)
		{
			ErrMsg = "entity " + _name + " is to large!. please decrease width or height key!"
			DPrint(ErrMsg)
			return
		}

		for(int v = 0; v < NumVSegments; v++)
		{
			for(int h = 0; h < NumHSegments; h++)
			{
				SetTypeFor(h, v, 0, GL_FULL)		//na zaciatku su vsetky skla cele
				SetTypeFor(h, v, 1, GL_NONE)		//dalsie vrstvy nepouzite
				SetTypeFor(h, v, 2, GL_NONE)
				SetTypeFor(h, v, 3, GL_NONE)
			}
		}

		RegenerateGlassObject()
//		GenerateStartObject()
		vector mins
		vector maxs
		GetBoundBox(this, mins, maxs)	//mins[1] a maxs[1] su teraz nulove

//		flags |= 1

		if(!flags & 1)
		{
			mins[1] = -1						//pre fyziku nastavime toto
			maxs[1] = 1
			SetBoundBox(this, mins, maxs)
			SetMass(1)
			SetWeaponScale(1)
			SetTransformEx(Vector(angle3, angle, angle2), coords)
			CreateRigidBody(GEOMTYPE_BOX, "material/glass", _name)
		}
		else	//je bez fyziky
		{
			SetOrigin(this, coords)
			SetAngles(this, Vector(angle3, angle, angle2))
			ChangeFullType()
			CreateTouchTrigger()		//uz necakame na prvy zasah takze musi byt rozbitelne dotykom od zaciatku 
		}
		
		mins[1] = -0.04					//pre presne detekcie zasahov to zase znizime
		maxs[1] = 0.04
		SetBoundBox(this, mins, maxs)

		DebrisObj[0] = GetObject("obj/physicsparts/glassdebris01.xob")
		DebrisObj[1] = GetObject("obj/physicsparts/glassdebris02.xob")
		DebrisObj[2] = GetObject("obj/physicsparts/glassdebris03.xob")
		DebrisObj[3] = GetObject("obj/physicsparts/glassdebris04.xob")
		DebrisObj[4] = GetObject("obj/physicsparts/glassdebris05.xob")
		DebrisObj[5] = GetObject("obj/physicsparts/glassdebris06.xob")
		DebrisObj[6] = GetObject("obj/physicsparts/glassdebris07.xob")
		DebrisObj[7] = GetObject("obj/physicsparts/glassdebris08.xob")
		DebrisObj[8] = GetObject("obj/physicsparts/glassdebris09.xob")
		DebrisObj[9] = GetObject("obj/physicsparts/glassdebris10.xob")

		Sound = GetSoundEx("glass_break", 1, SF_3D|SF_GLOBAL)
	}

//-------------------------------------------------------------------------
	void misc_physics_glass()
	{
      SetFlags(this, TFL_VISIBLE | TFL_SOLID | TFL_OBBCOLLISION | TFL_TRANSLUCENT)
      SetEventMask(this, EV_INIT | EV_LOADED/* | EV_ONCONTACT*/)
      SetPickable(false)
      ClassType = ClassTypeMiscPhysicsGlass
	}

//-------------------------------------------------------------------------
	void ~misc_physics_glass()
	{
		RemoveBodyAndGeom()

		SetObject(VNULL)
		if(GlassObject)
		{
			ReleaseObject(GlassObject, 0)
		}
		
		if(MTrigger)
			delete MTrigger

		for(int n = 0; n < sizeof(DebrisObj); n++)
		{
			if(DebrisObj[n])
				ReleaseObject(DebrisObj[n], 0)
		}
	}
}


