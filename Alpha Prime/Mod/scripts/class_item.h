
//=================================================================
class item_misc extends ItemClass
{
//---------------------------------------------------------------------------------------------------------
	void CanActivate(int fl, ExtClass other)
	{
		if(status == 0 && Definition.pickable && World.NumFlyingItems == 0)	//	//Definition.pickable znamena ze sa ma rozletiet k playerovi.
			return ACT_ENABLE

		return ACT_DISABLE
	}

//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		if(World.ManipEntity == this)
			return

		status = 1
		SetPickable(false)
		SetEventMask(this, EV_FRAME)
//		PlaySound(this, g_sCommonSounds[8], SF_ONCE)
		RemoveBodyAndGeom()
		World.NumFlyingItems++
	}

//-----------------------------------------------------------------
   int Pick(CreatureClass owner)
   {
   	Show(false)
		owner.PickItem(ClassNum, true)
		World.InfDialog.ShowMessage("#You_picked " + Definition.name)
		SetOrigin(this, coords)
		CallMethods(this, OnPick)

		CreatureClass pla = World.Player
		pla.RemoveInteractionAdept(this)
#ifndef EDITOR
     	delete this
#endif
		return 0
   }

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		if(name == "")
			name = "plechovka_mala"

		if(ActivateDist == 0)
			ActivateDist = 50

		ClassNum = World.ItemColPtr.GetItemIndex(name)
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)
		MainObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)

		if(!MainObject)
			return

		if(!Definition.pickable)
			ActivateMethod = ACT_NONE

      SelectObject(this, MainObject)
		Show(true)
		vector angles = Vector(angle3, angle, angle2)

		if(Definition.physics == true)	//item je definovany ako fyzikalny
		{
			SetTransformEx(angles, coords)

			if(CreateRigidBody(GEOMTYPE_NONE, "", Definition.model) == true)
				ActivateBody(true)
		}
		else
		{
			SetOrigin(this, coords)
			SetAngles(this, angles)
		}
	}
}


//=================================================================
class item_ammo extends item_misc
{
	int type

//---------------------------------------------------------------------------------------------------------
	void PlayFlySound()
	{
		PlaySound(this, g_sCommonSounds[1], SF_ONCE)
	}

//---------------------------------------------------------------------------------------------------------
	void CanActivate(int fl, ExtClass other)
	{
		if(status != 0 || !Definition.pickable || World.NumFlyingItems != 0)	//	//Definition.pickable znamena ze sa ma rozletiet k playerovi.
			return ACT_DISABLE

		if(IsInherited(other, CCreatureClass))
		{
			CreatureClass cc = other
			if(cc.ClassNum == ClassNumPlayer)
			{
				if(cc.Ammo[type] >= MaxAmmos[type])		//ClassNum a AmmoType na seba presne sedia takze mozeme indexovat podla jedneho aj druheho
					return ACT_DISABLE
			}
		}
		return ACT_ENABLE
	}
		
//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		if(World.ManipEntity == this)
			return

		status = 1
		SetPickable(false)
		RemoveBodyAndGeom()
		SetEventMask(this, EV_FRAME)
		World.NumFlyingItems++
	}

//-----------------------------------------------------------------
	int GetQuantity()
	{
		return 0
	}

//-----------------------------------------------------------------
   int Pick(CreatureClass owner)
   {
		Owner = owner
		owner.PickAmmo(type, GetQuantity())
		World.InfDialog.ShowMessage("#You_picked " + Definition.name)
		SetOrigin(this, coords)
   	Show(false)
   	CallMethods(this, OnPick)

		CreatureClass pla = World.Player
		pla.RemoveInteractionAdept(this)
		
#ifndef EDITOR
			delete this
#endif
		return 0
   }
}

//=================================================================
class item_ammo_gun extends item_ammo
{
//-----------------------------------------------------------------
	int GetQuantity()
	{
		CreatureClass pla = World.Player
		return AmmoGunQuantity[pla.GetDifficultyLevel()]
	}

//-----------------------------------------------------------------
	void item_ammo_gun()
	{
		name = "ammo_pistol"
   	type = AmmoPistol
	}
}

//=================================================================
class item_ammo_shotgun extends item_ammo
{
//-----------------------------------------------------------------
	int GetQuantity()
	{
		CreatureClass pla = World.Player
		return AmmoShotgunQuantity[pla.GetDifficultyLevel()]
	}

//-----------------------------------------------------------------
	void item_ammo_shotgun()
	{
		name = "ammo_Shotgun"
   	type = AmmoShotgun
	}
}

//=================================================================
class item_ammo_machinegun extends item_ammo
{
//-----------------------------------------------------------------
	int GetQuantity()
	{
		CreatureClass pla = World.Player
		return AmmoMachinegunQuantity[pla.GetDifficultyLevel()]
	}

//-----------------------------------------------------------------
	void item_ammo_machinegun()
	{
		name = "ammo_Machinegun"
   	type = AmmoMachinegun
	}
}

//=================================================================
class item_ammo_sniperrifle extends item_ammo
{
//-----------------------------------------------------------------
	int GetQuantity()
	{
		CreatureClass pla = World.Player
		return AmmoSniperRifleQuantity[pla.GetDifficultyLevel()]
	}

//-----------------------------------------------------------------
	void item_ammo_sniperrifle()
	{
		name = "ammo_SniperRifle"
   	type = AmmoSniperRifle
	}
}

//=================================================================
class item_ammo_rocketlauncher extends item_ammo
{
//-----------------------------------------------------------------
	int GetQuantity()
	{
		CreatureClass pla = World.Player
		return AmmoRocketLauncherQuantity[pla.GetDifficultyLevel()]
	}

//-----------------------------------------------------------------
	void item_ammo_rocketlauncher()
	{
		name = "ammo_RocketLauncher"
   	type = AmmoRocketLauncher
	}
}

//=================================================================
class item_ammo_flamethrower extends item_ammo
{
//-----------------------------------------------------------------
	int GetQuantity()
	{
		CreatureClass pla = World.Player
		return AmmoFlameThrowerQuantity[pla.GetDifficultyLevel()]
	}

//-----------------------------------------------------------------
	void item_ammo_flamethrower()
	{
		name = "ammo_FlameThrower"
   	type = AmmoFlameThrower
	}
}
//=================================================================
class item_hubbardium extends item_misc
{
//---------------------------------------------------------------------------------------------------------
	void PlayFlySound()
	{
		PlaySound(this, g_sCommonSounds[8], SF_ONCE)
	}

//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		if(World.ManipEntity == this)
			return

		CreatureClass cc = other

		if(cc.GetBulletTime() >= BulletTimeMax)	//neberieme to ked to nepotrebujeme doplnit
			return

		status = 1
		SetPickable(false)
		RemoveBodyAndGeom()
		SetEventMask(this, EV_FRAME)
		World.NumFlyingItems++
	}

//-----------------------------------------------------------------
   int Pick(CreatureClass owner)
   {
		Owner = owner
		owner.PickAmmo(-1, 0)
		World.InfDialog.ShowMessage("#You_picked " + Definition.name)
		SetOrigin(this, coords)
   	Show(false)
   	CallMethods(this, OnPick)

		CreatureClass pla = World.Player
		pla.RemoveInteractionAdept(this)
#ifndef EDITOR
			delete this
#endif
		return 0
   }
  
//-----------------------------------------------------------------
	void item_hubbardium()
	{
		name = "Hubbardium"
	}
}

//=================================================================
class item_medikit extends item_misc
{
//---------------------------------------------------------------------------------------------------------
	void OnPickPhysicaly()
	{
	}

//---------------------------------------------------------------------------------------------------------
	void PlayFlySound()
	{
		PlaySound(this, g_sCommonSounds[1], SF_ONCE)
	}
	
//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		CreatureClass cc = other

		if(cc.Health >= cc.MaxHealth)	//neberieme to ked to nepotrebujeme doplnit
			return
		
		if(World.ManipEntity == this)
			return
		
		status = 1
		SetPickable(false)
		RemoveBodyAndGeom()
		SetEventMask(this, EV_FRAME)
		World.NumFlyingItems++
	}	

//-----------------------------------------------------------------
   int Pick(CreatureClass owner)
   {
		Owner = owner
		CreatureClass pla = owner
		owner.AddHealth(MedikitHealth[pla.GetDifficultyLevel()])
		World.InfDialog.ShowMessage("#You_picked " + Definition.name)
		SetOrigin(this, coords)
   	Show(false)
   	CallMethods(this, OnPick)

		pla.RemoveInteractionAdept(this)
#ifndef EDITOR
			delete this
#endif
		return 0
   }
  
//-----------------------------------------------------------------
	void item_medikit()
	{
		name = "Medikit"
	}
}

