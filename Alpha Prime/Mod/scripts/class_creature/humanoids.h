//=================================================================================
class creature_technician1 extends HumanoidNPC
{	
//-----------------------------------------------------------------
   void creature_technician1()
   {
		if(!SoundSetName)
			SoundSetName = "Technician"

		DefaultAnimSetName = "Technician_pistol"
		
		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 67

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = TechnicianHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 2
		MaxRestAfterAttack[0] = 2.75

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.5
		MaxAttackDelay[1] = 0.75
		MinRestAfterAttack[1] = 2.85
		MaxRestAfterAttack[1] = 3.85
	
		fMinBulletRain = 3

		CurrentWeapon = new item_weapon_pistol(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgTechnician

      const string t1models[2] = {"obj/creatures/technicist/technicist.xob", "obj/creatures/technicist3/technicist3.xob"}
		MainObject = GetObject(t1models[rand(0, 2)])
		SetObject(MainObject, _materialremap)
   }
}

//=================================================================================
class creature_technician2 extends HumanoidNPC
{	
//-----------------------------------------------------------------
   void creature_technician2()
   {
		if(!SoundSetName)
			SoundSetName = "Technician"

		DefaultAnimSetName = "Technician_pistol"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 48

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = TechnicianHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 2
		MaxRestAfterAttack[0] = 2.95

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.5
		MaxAttackDelay[1] = 0.85
		MinRestAfterAttack[1] = 2.9
		MaxRestAfterAttack[1] = 3.9
	
		fMinBulletRain = 2.89

		CurrentWeapon = new item_weapon_pistol(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgTechnician

      const string t2models[2] = {"obj/creatures/technicist2/technicist2.xob", "obj/creatures/technicist4/technicist4.xob"}
		MainObject = GetObject(t2models[rand(0, 2)])
		SetObject(MainObject, _materialremap)
   }
}

//=================================================================================
class creature_guard1 extends HumanoidNPC
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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}	

//-----------------------------------------------------------------
   void creature_guard1()
   {
		if(!SoundSetName)
			SoundSetName = "Guard"

   	DefaultAnimSetName = "Guard"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 57

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = GuardHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.5
		MaxRestAfterAttack[0] = 2.1

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 15
		MinAttackDelay[1] = 0.35
		MaxAttackDelay[1] = 0.72
		MinRestAfterAttack[1] = 2.1
		MaxRestAfterAttack[1] = 2.2
	
		fMinBulletRain = 3

		CurrentWeapon = new item_weapon_shotgun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgGuard1

		MainObject = GetObject("obj/creatures/guard/guard.xob")
		SetObject(MainObject, _materialremap)
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)			
  }
}

//=================================================================================
class creature_guard2 extends HumanoidNPC
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
	

	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}	

//-----------------------------------------------------------------
   void creature_guard2()
   {
		if(!SoundSetName)
			SoundSetName = "Guard"

   	DefaultAnimSetName = "Guard"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 57

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = GuardHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.9
		MaxRestAfterAttack[0] = 2.1

		MinAttacksInRow[1] = 5
		MaxAttacksInRow[1] = 15
		MinAttackDelay[1] = 0.3
		MaxAttackDelay[1] = 0.6
		MinRestAfterAttack[1] = 2.2
		MaxRestAfterAttack[1] = 2.4
	
		fMinBulletRain = 2.98

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgGuard2

		MainObject = GetObject("obj/creatures/guard/guard.xob")
		SetObject(MainObject, "$remap 'guard' 'guard2';")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
  }
}

//=================================================================================
class creature_lighttrooper extends HumanoidNPC
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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}

	
//-----------------------------------------------------------------
   void creature_lighttrooper()
   {
		if(!SoundSetName)
			SoundSetName = "LightTrooper"

   	DefaultAnimSetName = "LightTrooper"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 59

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = LightTrooperHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1.1
		MaxRestAfterAttack[0] = 2

		MinAttacksInRow[1] = 7
		MaxAttacksInRow[1] = 16
		MinAttackDelay[1] = 0.21
		MaxAttackDelay[1] = 0.66
		MinRestAfterAttack[1] = 1.9
		MaxRestAfterAttack[1] = 2.1
	
		fMinBulletRain = 3.02
  		SmallPainGroup = g_smallpain

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgLightTrooper

		MainObject = GetObject("obj/creatures/lighttrooper/lighttrooper.xob")
		SetObject(MainObject, "$remap 'EnemyLights1' 'EnemyLights1_blue';$remap 'EnemyLights2' 'EnemyLights2_blue';")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)

  		GetSpeechSet("Trooper")
	}
}

//=================================================================================
class creature_lighttrooper2 extends HumanoidNPC
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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}

//-----------------------------------------------------------------
   void creature_lighttrooper2()
   {
		if(!SoundSetName)
			SoundSetName = "LightTrooper"

   	DefaultAnimSetName = "Guard"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 63

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = LightTrooperHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 20
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1
		MaxRestAfterAttack[0] = 2

		MinAttacksInRow[1] = 6
		MaxAttacksInRow[1] = 18
		MinAttackDelay[1] = 0.205
		MaxAttackDelay[1] = 0.35
		MinRestAfterAttack[1] = 1.9
		MaxRestAfterAttack[1] = 2.2
	
		fMinBulletRain = 3.88
  		SmallPainGroup = g_smallpain

		CurrentWeapon = new item_weapon_shotgun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgLightTrooper2

		MainObject = GetObject("obj/creatures/lighttrooper/lighttrooper.xob")
		SetObject(MainObject, "$remap 'LightTrooper' 'LightTrooper2';$remap 'LightTrooperHelmet' 'LightTrooperHelmet2';$remap 'EnemyLights1' 'EnemyLights1_red';$remap 'EnemyLights2' 'EnemyLights2_red';")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
  		GetSpeechSet("Trooper")
	}
}

//=================================================================================
class creature_lighttrooper3 extends AISniperClass
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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}

//-----------------------------------------------------------------
   void creature_lighttrooper3()
   {
		if(!SoundSetName)
			SoundSetName = "LightTrooper"

   	DefaultAnimSetName = "LightTrooper"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 65

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = LightTrooperHealth

		MinAttacksInRow[0] = 5
		MaxAttacksInRow[0] = 7
		MinAttackDelay[0] = 0.3
		MaxAttackDelay[0] = 0.3
		MinRestAfterAttack[0] = 2
		MaxRestAfterAttack[0] = 3

		MinAttacksInRow[1] = 8
		MaxAttacksInRow[1] = 10
		MinAttackDelay[1] = 0.5
		MaxAttackDelay[1] = 1.0
		MinRestAfterAttack[1] = 2.6
		MaxRestAfterAttack[1] = 3.6
	
		fMinBulletRain = 2.66
  		SmallPainGroup = g_smallpain

		CurrentWeapon = new item_weapon_sniperrifle(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgLightTrooper3

		MainObject = GetObject("obj/creatures/lighttrooper/lighttrooper.xob")
		SetObject(MainObject, "$remap 'LightTrooper' 'LightTrooper3';$remap 'LightTrooperHelmet' 'LightTrooperHelmet3'")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
  		
  		GetSpeechSet("Trooper")
	}
}

//=================================================================================
class creature_heavytrooper_ft extends HumanoidNPC
{
	float LightsAnimTimer
	int 	LightsAnimFrame
	float LightsFadeOutTimer

//-----------------------------------------------------------------
void AITakeCover()
{
	local int prevstatus = status
	
	SetState(s_combat)
	SetAnimState(t_idle)
	TurnByAnim(Attacker, 10)
	ShootOff()
	
	PlayAnimWait(s_walk - 1, a_combat_cover, 20.0, g_all, 30, AF_ONCE)
	PlayAnim(s_walk - 1, a_combat_cover + 1, 5.0, g_all, 30, 0)
	
	local float timeout = 150

	if(prevstatus == STATE_COVERFROMDANGER)
		timeout = 300
	
	while(timeout > 0 && IsState(prevstatus))
	{
		if(prevstatus == STATE_COVERFROMDANGER)
		{
			if(AIEventAge(AIE_GRENADE) > 2.0)
				break
		}

		timeout -= AI_REFRESH
		AISleep(AI_REFRESH)
	}

	PlayAnimWait(s_walk - 1, a_combat_cover + 2, 20.0, g_all, 30, AF_ONCE|AF_BLENDOUT)

	if(IsState(prevstatus))
		SwitchFSM(STATE_GUARD)
	
	BulletRainCounter = 0
}

//-----------------------------------------------------------------
void AITakeCoverFromDanger()
{
	AITakeCover()
}

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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}

//-----------------------------------------------------------------
   void creature_heavytrooper_ft()
   {
		if(!SoundSetName)
			SoundSetName = "HeavyTrooperFT"

   	DefaultAnimSetName = "HeavyTrooperFT"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/metal"
		RagdollDef = "FatHumanoid"
		RagdollMass = 100

		bLongRange = false
		bShortRange = true

		FSType = FSPlayer

		Health = HeavyTrooperFTHealth

		MinAttacksInRow[0] = 25
		MaxAttacksInRow[0] = 30
		MinAttackDelay[0] = 0.1
		MaxAttackDelay[0] = 0.1
		MinRestAfterAttack[0] = 1
		MaxRestAfterAttack[0] = 2

		MinAttacksInRow[1] = 9
		MaxAttacksInRow[1] = 18
		MinAttackDelay[1] = 0.2
		MaxAttackDelay[1] = 0.2
		MinRestAfterAttack[1] = 2.5
		MaxRestAfterAttack[1] = 3.5

		AttackDistMin = 245

		fMinBulletRain = 16.9
  		SmallPainGroup = g_smallpain

		CurrentWeapon = new item_weapon_flamethrower(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgHeavyTrooperFT

		MainObject = GetObject("obj/creatures/heavytrooper/heavytrooper.xob")
		SetObject(MainObject, "$remap 'EnemyLights1' 'EnemyLights1_red';$remap 'EnemyLights2' 'EnemyLights2_red';")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
		
  		GetSpeechSet("Trooper")
  		
		CurrentWeapon.Ammo = -1
   }
}

//=================================================================================
class creature_heavytrooper_rl extends AISniperClass
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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}

//-----------------------------------------------------------------
   void creature_heavytrooper_rl()
   {
		if(!SoundSetName)
			SoundSetName = "HeavyTrooperRL"

   	DefaultAnimSetName = "HeavyTrooperRL"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/metal"
		RagdollDef = "FatHumanoid"
		RagdollMass = 97

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = HeavyTrooperRLHealth

		MinAttacksInRow[0] = 1
		MaxAttacksInRow[0] = 3
		MinAttackDelay[0] = 1.0
		MaxAttackDelay[0] = 1.0
		MinRestAfterAttack[0] = 1.1
		MaxRestAfterAttack[0] = 2.2

		MinAttacksInRow[1] = 1
		MaxAttacksInRow[1] = 3
		MinAttackDelay[1] = 0.9
		MaxAttackDelay[1] = 0.9
		MinRestAfterAttack[1] = 2.2
		MaxRestAfterAttack[1] = 3.2
	
		fMinBulletRain = FLT_MAX
  		SmallPainGroup = g_smallpain

		CurrentWeapon = new item_weapon_rocketlauncher(this)

		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgHeavyTrooperRL

		MainObject = GetObject("obj/creatures/heavytrooper/heavytrooper.xob")
		SetObject(MainObject, "$remap 'HeavyTrooper' 'HeavyTrooper2';$remap 'HeavyTrooperHelmet' 'HeavyTrooperHelmet2'")
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
  		GetSpeechSet("Trooper")
   }
}

//=================================================================================
class creature_heavycommander extends HumanoidNPC
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
	void RFootstep()
	{
		Footstep(-1)
		Sound(51, SF_ONCE)
	}
	
//-----------------------------------------------------------------
	void LFootstep()
	{
		Footstep(1)
		Sound(51, SF_ONCE)
	}

//-----------------------------------------------------------------
   void creature_heavycommander()
   {
		if(!SoundSetName)
			SoundSetName = "HeavyCommander"

   	DefaultAnimSetName = "LightTrooper"

		GetHurtFactors(HumanHurtFactors)

		RagdollMaterial = "material/flesh"
		RagdollDef = "humanoid2"
		RagdollMass = 60

		bLongRange = true
		bShortRange = false

		FSType = FSPlayer

		Health = CommanderHealth

		MinAttacksInRow[0] = 15
		MaxAttacksInRow[0] = 25
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
	
		fMinBulletRain = FLT_MAX
  		SmallPainGroup = g_smallpain

		CurrentWeapon = new item_weapon_machinegun(this)
		CurrentWeapon.Pick(this)
		SelectItem(CurrentWeapon, p_rhand)
		CurrentWeapon.ShotStor.damage = WDmgCommander

      const string hcmodels[2] = {"obj/creatures/heavycommander/heavycommander.xob", "obj/creatures/heavycommander2/heavycommander2.xob"}
		MainObject = GetObject(hcmodels[rand(0, 2)])

		SetObject(MainObject, _materialremap)
		Color2 = 0xffffffff	//svetielka
		SetEventMask(this, EV_VISIBLE)
   }
}
