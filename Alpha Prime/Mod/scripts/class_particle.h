const float ZeroFloat = 0

class misc_particle_effector extends ExtClass
{
	vobject MainObject
	vector coords
	float speed
	string model
	int flags
	float angle, angle2, angle3
	bool SwitchStatus
	string EffectorNames[16]
	float  BRates[16]
	float  BRatesRND[16]
	int NumEffectors
	bool KillOnIdle
	int OnceAction
	float EffectTime
	float CurrentTime
	float LastTime
	MinClass boneowner
	int boneid
	int LastPnum
  	bool AnyParticleEmitedFromStart
	int AnimFlags

	void ParentOnBone(MinClass owner, int bone)
 	{
		boneowner = owner
		boneid = bone

		vector mat[4]
		GetBoneMatrix(boneowner, boneid, mat);
		SetOrigin(this, mat[3])
	}

	void SetAnglesFromVec(vector vec)
	{
		vector ang = Vector2Angles(vec)
		ang[0] = FixAngle(ang[0] - 90)
		SetAngles(this, ang)
	}

	int ParticlesActive()
	{
		if(LastPnum == 0 && status != 1)	//zatal nebol vyemitovany ani jeden tak nesmieme vratit false. to plati iba ked nebol hned na zaciatku zavolany switch(false)
			return true
		
		int Pnum = GetEffectorParticleCount(this)
		
		if(Pnum > 0)
			LastPnum = Pnum	//musime vediet ze bol vyemitovany aspon jeden (vybuchy a pod)
		
		return Pnum
	}

	void Restart()
	{
		ResetEffectorPosition(this)
		SetEffectorParm(this, -1, EP_CURRENT_TIME, ZeroFloat)
		LastTime = 0
		CurrentTime = 0
		AnyParticleEmitedFromStart = false
	}

	void M_Restart(ExtClass other)
	{
		Restart()
	}

	void Switch(bool stat)
	{
		SwitchStatus = stat
		ClearEventMask(this, EV_FRAME | EV_VISIBLE)
		
		if(stat)
		{
			status = 0
			SetFlags(this, TFL_VISIBLE)
			AnyParticleEmitedFromStart = false
			
			if(flags & 1)
				SetEventMask(this, EV_FRAME)
			else
				SetEventMask(this, EV_VISIBLE)
				
			//Nevim, proc tu byl BumpFrame ale je to SPATNE. Effectory jsou poruznu zapinany timerama
			//po celym levelu a je dost nezadouci, aby se i ty, ktere nejsou videt, updatovaly!
			
//			BumpFrame(this, ftime * speed, 0)
			ResetEffectorPosition(this)	//aby zresetoval staru poziciu na novu. inac by roztahoval partikly medzi poslednym emitovanym miestom

			for(int n = 0; n < NumEffectors; n++)
			{
				SetEffectorParm(this, n, EP_BIRTH_RATE, BRates[n])
				SetEffectorParm(this, n, EP_BIRTH_RATE_RND, BRatesRND[n])
			}
		}
		else
		{
			status = 1
			
			if(ParticlesActive() == 0)		//neni nic vyemitovane takze nepotrebujeme pustat OnFrame
			{
				ClearFlags(this, TFL_VISIBLE)
				return
			}
			
			SetEventMask(this, EV_FRAME)
			SetEffectorParm(this, -1, EP_BIRTH_RATE, ZeroFloat)
			SetEffectorParm(this, -1, EP_BIRTH_RATE_RND, ZeroFloat)
		}
	}

	void Destroy()	//efekt dobehne a samo sa to zmaze
	{
		KillOnIdle = true
		Switch(false)
	}

	void SetSpeed(float Speed)
	{
		speed = Speed
	}

	void M_Show(ExtClass other)
	{
		Switch(true)
	}

	void M_Hide(ExtClass other)
	{
		Switch(false)
	}

	void M_SetAngles(ExtClass other)
	{
		string Parms[8]
		int NumParms = ParseString(other._value, Parms)
		
		if(NumParms != 5)
		{
			Print(String("wrong num of parameters " + _name + ".Say"))
			return
		}

		vector angs
		angs[0] = atof(Parms[0])
		angs[1] = atof(Parms[2])
		angs[2] = atof(Parms[4])
		
		SetAngles(this, angs)
	}

	void EndLoop()
	{
		if(OnceAction == 1)
			Switch(false)
			
		if(OnceAction == 2)
			Destroy()
	}

	void Update()
	{
		if(boneowner != NULL)
		{
			vector mat[4]
			GetBoneMatrix(boneowner, boneid, mat);
			SetOrigin(this, mat[3])
			SetAnglesFromVec(mat[1])
		}
 		
		if(status == 1)	//bol zavolany Switch(false) tak pockame nez vyemitovane partikly zmiznu az potom to vypneme
		{
			if(ParticlesActive() == false)
			{
				if(KillOnIdle)
					delete this
					
				ClearFlags(this, TFL_VISIBLE)
				ClearEventMask(this, EV_FRAME | EV_VISIBLE)
				status = 0	
				return
			}
		}
		else
		{
			if(OnceAction)
			{
				bool AnyParticleEmitedNow = ParticlesActive()
				
				if(AnyParticleEmitedNow)
					AnyParticleEmitedFromStart = true

				if(AnyParticleEmitedFromStart)
				{
					GetEffectorParm(this, 0, EP_CURRENT_TIME, CurrentTime)
					
					if(CurrentTime < LastTime || CurrentTime > EffectTime || AnyParticleEmitedNow == false)
					{
						LastTime = 0
						CurrentTime = 0
						EndLoop()
					}
				}
			}	
		}

		LastTime = CurrentTime

		BumpFrame(this, ftime, AnimFlags)
	}

	int EOnVisible(MinClass other, int extra)
	{
		Update()
		return 0	
	}
	
	void EOnFrame(MinClass other, int extra)
	{
		Update()
	}

	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)			
	}

	void Init()
	{
		if(MainObject)
			return
			
		int n
		
		if(model == "")
			model = "particle/torch.ptc" 

		if(!boneowner)
		{
			SetOrigin(this, coords)
			SetAngles(this, Vector(angle3, angle, angle2))
		}
		
		UpdateEntity(this)
		
		speed = 1
		MainObject = GetObject(model)
		SelectObject(this, MainObject)
		SetEventMask(this, EV_LOADED)
		
		if(flags & 2)
			OnceAction = 1

		if(SwitchStatus == false)
			return

		SetFlags(this, TFL_VISIBLE)

		if(flags & 1)
		{
			SetEventMask(this, EV_FRAME)
		}
		else
			SetEventMask(this, EV_VISIBLE)

		NumEffectors = GetEmitors(this, EffectorNames, sizeof(EffectorNames))
		float tmp

		for(n = 0; n < NumEffectors; n++)
		{
			GetEffectorParm(this, n, EP_BIRTH_RATE, tmp)
			BRates[n] = tmp
			
			GetEffectorParm(this, n, EP_BIRTH_RATE_RND, tmp)
			BRatesRND[n] = tmp
		}
		
		GetEffectorParm(this, 0, EP_EFFECT_TIME, EffectTime)
		
	}
	
	void EOnInit(MinClass other, int extra)
	{
		Init()
	}

	void SetParms(string Model, vector Coords, bool AlwaysUpdate, int OnceAct)	//pokial neni vlozena v mape pouzit toto
	{
//		AlwaysUpdate = true //FIXME!!!
		
		model = Model
		coords = Coords
		OnceAction = OnceAct
		
		if(AlwaysUpdate)
		{
			if(!flags & 1)
				flags = flags | 1
		}
		
		Init()
	}

	void SetParmsEx(vobject obj, vector Coords, bool AlwaysUpdate, int OnceAct)	//pokial neni vlozena v mape pouzit toto
	{
		OnceAction = OnceAct
		
		if(!boneowner)
		{
			SetOrigin(this, Coords)
			UpdateEntity(this)
			SetAngles(this, Vector(angle3, angle, angle2))
		}

		speed = 1
		SelectObject(this, obj)
		
		if(flags & 2)
			OnceAction = 1

		SetFlags(this, TFL_VISIBLE)

		if(AlwaysUpdate)
		{
			flags = flags | 1
			SetEventMask(this, EV_FRAME)
		}
		else
			SetEventMask(this, EV_VISIBLE)

		NumEffectors = GetEmitors(this, EffectorNames, sizeof(EffectorNames))
		float tmp

		for(int n = 0; n < NumEffectors; n++)
		{
			GetEffectorParm(this, n, EP_BIRTH_RATE, tmp)
			BRates[n] = tmp
			
			GetEffectorParm(this, n, EP_BIRTH_RATE_RND, tmp)
			BRatesRND[n] = tmp
		}
		
		GetEffectorParm(this, 0, EP_EFFECT_TIME, EffectTime)
	}

	void misc_particle_effector()
	{
		SwitchStatus = true
	}
	
	void ~misc_particle_effector()
	{
		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}








//==================================================================================================
// klasicka corona
//==================================================================================================
class misc_effect_glow extends ExtClass
{
	vector coords
	string model
	float offset
	float maxalpha
	float minalpha
	float fadedist
	float fadeangle
	
	vobject MainObject
	ExtClass effect
	float distscale
	float AlpRange
	float OnOffScale
	float OnOffTargetScale
	int flags

//---------------------------------------------------------------------------------------------------
	void Switch(bool stat)
	{
		if(stat)
		{
			SetFlags(effect, TFL_VISIBLE)
			SetFlags(this, TFL_VISIBLE)
		}
		else
		{
			ClearFlags(effect, TFL_VISIBLE)
			ClearFlags(this, TFL_VISIBLE)
		}
	}

//---------------------------------------------------------------------------------------------------
	void Show(bool stat)
	{
		Switch(stat)
	}

//---------------------------------------------------------------------------------------------------
OcclusionQuery query

//---------------------------------------------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		if(r_recursion != 0)
			return 1

		SetEventMask(this, EV_NOTVISIBLE)
		vector CamPos = World.CameraPos
		vector CoordsPos = GetMatrix(this, 3)
		vector Dir = CoordsPos - CamPos
		float dist = VectorNormalize(Dir)
		float dot = World.CameraNorm * Dir
		
		if(dist > fadedist)
			distscale = 1
		else
			distscale = dist / fadedist
		
		if(dot < 0.6)
			return 0

		vector off = Dir * offset
		vector end = CoordsPos - off
		OnOffTargetScale = 1
		

		if(flags & 1)
		{		
			if(!query)
				query = CreateOcclusionQuery(end, 1)
			else
				query.SetPosition(end)

			if(query.GetResult() <= 0)
				OnOffTargetScale = 0
		}

		if(OnOffScale > OnOffTargetScale)
		{
			OnOffScale = ftime * -4 + OnOffScale

			if(OnOffScale < OnOffTargetScale)
				OnOffScale = OnOffTargetScale
		}
		else
		{
			if(OnOffScale < OnOffTargetScale)
			{
				OnOffScale = ftime * 4 + OnOffScale
	
				if(OnOffScale > OnOffTargetScale)
					OnOffScale = OnOffTargetScale
			}
		}

		float CurAngle = 1 - dot * 180	//uhol
		float AngleScale = 0
		
		if(CurAngle <= fadeangle)
		 	AngleScale = 1 - (CurAngle / fadeangle)

		float AngleAlpha = AngleScale * AlpRange	//alfa z toho aky mame odklon pohladu
		float DistAlpha = distscale * AlpRange
		float OnOffAlpha = OnOffScale * AlpRange
		float FinalAlpha = AngleAlpha * DistAlpha * OnOffAlpha
		FinalAlpha += minalpha

		effect.TFactor[3] = FinalAlpha
		float fa = FinalAlpha * 255		
		effect.Color = fa << 24 | 0xffffff

		SetOrigin(effect, CoordsPos - off)			
		return 0
	}

//---------------------------------------------------------------------------------------------------
	void EOnNotVisible(class other, int extra)
	{
		if(r_recursion != 0)
			return

		if(query)
		{
			query.Destroy()
			query = NULL
		}

		OnOffScale = 0
		effect.TFactor[3] = 0
		effect.Color = 0
		ClearEventMask(this, EV_NOTVISIBLE)
	}

//---------------------------------------------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//---------------------------------------------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(!model)
			model = "spr/effects/glow/corona4.spr"

		if(maxalpha == 0)	//scale na alfu
			maxalpha = 1
			
		if(maxalpha > 1)
			maxalpha = 1
			
		if(minalpha > maxalpha)
			minalpha = maxalpha
		
		AlpRange = maxalpha - minalpha	//v tomto rozsahu pracujeme s alfou
		
		if(fadedist == 0)
			fadedist = 128
			
		if(fadeangle == 0)
			fadeangle = 70
			
		if(fadeangle > 180)
			fadeangle = 180
			
		effect = new MinClass()
		MainObject = GetObject(model)
		SelectObject(effect, MainObject)
		SetOrigin(this, coords)
		Show(true)
		SetFlags(effect, TFL_PARTICLE)
		effect.TFactor[0] = 1
		effect.TFactor[1] = 1
		effect.TFactor[2] = 1
		effect.TFactor[3] = 0

		if(extra == 1)
			ClearEventMask(this, EV_INIT)	//neni to entita z mapy takze nesmeieme dovolit aby sa to zavolalo este raz
		else
			SetEventMask(this, EV_LOADED)

		SetBoundBox(this, "-64 -64 -64", "64 64 64")
	}

//---------------------------------------------------------------------------------------------------
	void SetParms(vector Coords, float Offset, string Sprite)
	{
		coords = Coords + "0 0 1"
		offset = Offset
		model = Sprite
		Throw(this, this, EV_INIT, 1)
	}

//---------------------------------------------------------------------------------------------------
	void misc_effect_glow()
	{
		SetEventMask(this, EV_VISIBLE)
	}

//---------------------------------------------------------------------------------------------------
	void ~misc_effect_glow()
	{
		if(query)
		{
			query.Destroy()
		}

		if(MainObject)
			ReleaseObject(MainObject, 0)
			
		if(effect)
			delete effect
	}
}
