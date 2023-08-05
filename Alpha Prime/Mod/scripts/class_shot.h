//=================================================================================
class DefaultShell extends RigidBodyClass
{
//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		PlaySound(this, g_sCommonSounds[2], SF_ONCE)
		ClearEventMask(this, EV_CONTACT)
	}

//-----------------------------------------------------------------
	void DefaultShell(vobject obj, vector mat[])
	{
		ClassType = -1
	   SetFlags(this, TFL_VISIBLE|TFL_FULLMATRIX)
		ClearFlags(this, TFL_TOUCHTRIGGERS)

		ClearEventMask(this, EV_ALL)
		SetPickable(false)
		SelectObject(this, obj)
		SetMatrix(this, mat)

	}

//-----------------------------------------------------------------
	void ~DefaultShell()
	{
		RemoveBodyAndGeom()
	}
}


//=================================================================================
class PhysXShell extends MinClass
{
//-----------------------------------------------------------------
        void ~PhysXShell()
        {
		NxActorDestroy(this)
	}
//-----------------------------------------------------------------
void PhysXShell(vobject obj, vector mat[])
	{
		ClassType = -1
	   SetFlags(this, TFL_VISIBLE|TFL_FULLMATRIX)

		SelectObject(this, obj)

		SetObject(obj)
		SetMatrix(this, mat)

//		NxActorCreate(this, ZeroVec, BC_CAPSULE_Z, true, 1, 0, 0)
		NxActorCreate(this, ZeroVec, BC_MODEL, true, 1, 0, 0)
		NxActorSetCollisionGroup(this, 1)
		
	}
}

//=================================================================================
void WorldClass::ThrowShell(MinClass pivot, MinClass owner, int type, float speed, bool sound)
{	
	vector mat[4]
	GetMatrix4(pivot, mat)
	float distq
	
	if(owner.ClassNum != ClassNumPlayer)
	{
		vector org = mat[3]
		distq = VectorLengthSq(org - CameraPos)
		if(distq > 124 * 124)
			return

		// TODO: pokud neni videt, mohlo by se aspon poustet cinkani nabojnic, aby to nebylo podezrele :)
		if(!IsBoxVisible(org, org, true))
			return
	}

	type--

	vector vel = pivot.VectorToParent("0.612372 -0.353553 0.707107")
	float rnd = frand(1.5, 2.5) * -speed
	vel = vel * rnd
	vel = GetVelocity(owner) * 0.8 + vel

	int top = ShellTop[type]
	DefaultShell shell = Shells[type][top]
	
	if(shell == NULL)
	{
		vobject obj = ShellObjects[type]

		{
			shell = new DefaultShell(obj, mat)
	
			if(!dBodyCreate(shell, ZeroVec, BC_MODEL))
			{
				delete shell
				return
			}
			shell.rb = true
			dBodySetMass(shell, ZeroVec, 50)
			dBodyInteractionLayer(shell, ET_SHELL)
			dBodyActive(shell, true, true)
		}
		
		Shells[type][top] = shell
	}
	else
	{
		SetMatrix(shell, mat)
	}
		
   if(sound && distq > 64 * 64)
   	SetEventMask(shell, EV_CONTACT)
	else
		ClearEventMask(shell, EV_CONTACT)

	SetVelocity(shell, vel)
	
	ShellTop[type] = top + 1 % ShellBufferSize
}


//---------------------------------------------------------------------------------
void ReleaseRicParticles()
{
	for(int n = 0; n < sizeof(RicParticles); n++)
	{
		if(RicParticles[n])
		{
			ReleaseObject(RicParticles[n], 0)
			RicParticles[n] = VNULL
		}
	}
}

//---------------------------------------------------------------------------------
hsound GetRicSound(int Mat, int RSndType)
{
	if(Mat >= MATERIAL_LAST)
	{
		Print(String("GetRicSound out of material table " + itoa(Mat)))
		Mat = 0
	}
	if(RSndType >= SE_COUNT)
	{
		Print(String("GetRicSound out of sound table " + itoa(RSndType)))
		RSndType = 0
	}
	if(RicSounds[RSndType][Mat])
		return RicSounds[RSndType][Mat]
		
	return RicSounds[RSndType][0]	//default
}

//---------------------------------------------------------------------------------
hshader GetDecalShader(int Mat, int RDecalType)
{
	if(Mat >= MATERIAL_LAST)
	{
		DPrint(String("GetDecalShader out of material table " + itoa(Mat)))
		Mat = 0
	}

	if(RDecalType >= DE_COUNT)
	{
		DPrint(String("GetDecalShader out of decal table " + itoa(RDecalType)))
		RDecalType = 0
	}

	if(DecalShaders[RDecalType][Mat])
		return DecalShaders[RDecalType][Mat]
		
	return DecalShaders[RDecalType][0]	//default
}

//---------------------------------------------------------------------------------
vobject GetParticleObject(int Mat, int RPartType)
{
	if(Mat >= MATERIAL_LAST)
	{
		Print(String("GetParticleObject out of material table " + itoa(Mat)))
		Mat = 0
	}
	if(RPartType >= PE_COUNT)
	{
		Print(String("GetParticleObject out of particle table " + itoa(RPartType)))
		RPartType = 0
	}
	vobject vobj = RicParticles[RPartType][Mat]
	
	if(!vobj)
		vobj = RicParticles[RPartType][0]
	
	return vobj
}

//---------------------------------------------------------------------------------
void ShotEffects(ShotStorage stor, MinClass target, vector pos, vector norm, int sparm)
{
	//TODO: az budou ric soundy, tak se musi vytvorit temporalni entita, ktera jen prehraje zvuk a pak
	// se smaze

	if(stor.shooter != World.Player && stor.MarkSize > 0)
	{
		if(target != World.Player && !IsBoxVisible(pos, pos, false))
			return
	}

	float lifetime

	//ragdolly a spol. jsou narocny jako prase, tak to omezime
	if(IsInherited(target, CCreatureClass))
		lifetime = 20.0
	else
		lifetime = 45.0
	
	if(InCutscene)
		lifetime = 300.0

	int RichMat = Surf2Mat(sparm)						//material zo sounds/materials.h
	int LogMat = GetLogicalMaterial(RichMat)		//jeden z 11 default materialov

	bool dogore = true
	
#ifdef GERMAN
//nebudem prznit mrtvoly
	if(target.ClassType == ClassTypeCreature || IsInherited(target, CRBone))
	{
		if(target.status >= STATE_DIE)
			dogore = true
	}
#endif

	//add bloody spurts
	if(LogMat == MATERIAL_FLESH)
	{
		
		
			if(IsInherited(target, CAICreatureClass))
			{
				CreatureClass ctarget = target
				ctarget.AddBleedSource(stor)
                                ctarget.AddSSource(stor)
			}
	

		for(int n = 0; n < 2; n++)
		{
			vector v = Vector(norm[0] + frand(-0.3, 0.3), norm[1] + frand(-0.3, 0.3), -1)
			VectorNormalize(v)
			
			MinClass other
			vector to = v * 65.3 + pos
			float plane[4]
			
			if(TraceLineEx(DEFAULT, pos, to, ZeroVec, ZeroVec, other, plane, NULL, NULL, TRACE_WORLD, NULL) < 1)
			{
//AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos, to)

				CreateDecal(other, to, Vector(plane[0], plane[1], plane[2]) * -8, 1.0, 0, 32, BloodDecalShaders[rand(0,4)], lifetime, 61.0)
			}
		}
	}
	

	if(stor.StreakLength > 0 && stor.StreakShader != 0)
		ShotStreak str = new ShotStreak(stor.VisualsFrom, stor.to, stor.StreakWidth, stor.StreakLength, stor.StreakSpeed, stor.StreakShader)
	
   misc_particle_effector eff = NULL
   
   //player nema particly, aby nestrikaly prez celou obrazovku
   //a ani decaly, protoze nemaji smysl.
   if(stor.power == -1 || target != World.Player)
   {
		vobject ParticlesObj = GetParticleObject(LogMat, stor.RicParticlesType)
	
		if(ParticlesObj && dogore)
		{
			eff = new misc_particle_effector
			eff.SetParmsEx(ParticlesObj, pos, true, 2)
		}
	
		if(stor.MarkSize != -1)
		{
			float nearplane
			vector project
			
			if(target.ClassType == ClassTypeCreature || IsInherited(target, CRBone))
			{
#ifdef GERMAN
				if(!dogore)
					goto nogore
#endif
				nearplane = 6.0
				project = stor.dir
				VectorNormalize(project)
			}
			else
			{
				if(target == World)
					nearplane = 1.0
				else
					nearplane = 4.0
	
				project = -norm
			}
			
			CreateDecal(target, stor.to, project * 8.0, nearplane, 0, stor.MarkSize, GetDecalShader(LogMat, stor.RicDecalType), lifetime, 0)
		}
nogore:;

		if(eff != NULL)
		{
			vector BounceVec = stor.dir	//spravime si "odrazeny" vector
			float backoff = norm * BounceVec
			backoff *= 2			//2 = odraz presne podla uhla dopadu
			vector change = norm * backoff
			BounceVec -= change
		
			vector mat[4]
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
			mat[3] = pos
			SetFlags(eff, TFL_FULLMATRIX)
			SetMatrix(eff, mat)
			UpdateEntity(eff)
		}	
	}

	if(stor.RicSndType != -1)
	{
		hsound hSnd = GetRicSound(LogMat, stor.RicSndType)
	
		if(hSnd)
	   	PlaySound(eff, hSnd, SF_ONCE)
   }
}




