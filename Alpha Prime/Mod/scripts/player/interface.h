

//Movement states
const int ST_NONE = 0
const int ST_RUN = 1
const int ST_JUMP = 2
const int ST_DEATH = 3
const int ST_NOCONTROL = 4	//nieco ovlada. je v aute, nahackovany do miny a pod.

class player_interface extends CreatureClass
{
	WeaponClass	ChangeWeapon		//nova zbran ktoru menime za staru
	int			WeapSelectStatus	//0- zbran sa nemeni 1- prebieha unselect 2- prebieha select
	WeaponClass PWeapons[32]      //pointery na vlastni entity
	int			RealWeaponsNum		//pocet skutocnych zbrani (nezahrnuto DummyWeapon a DeviceWeapon)
	float IdleVarTimer
	ExtClass InteractionAdepts[80]
	int 		InteractionAdeptsNum
	float ExactAim					//0 - 1 medzi presnym mierenim a normalnym
	PlayerHUD HUD
//	int NextWeaponToChange

//---------------------------------------------------------------------------------------------------------
	void UpdateHealthBar()
	{
		float rate = Health / MaxHealth
		float BarWidth = rate * (float)200
		
		if(rate <= 0.25)
		{
			SetWidgetImage(HUD.WHealthBar, 1)
			SetWidgetImage(HUD.WHealthBarFlash, 1)
			SetWidgetTextColor(HUD.WHealthBarText, ARGB(255,253,106,42))
		}
		else
		{
			SetWidgetImage(HUD.WHealthBar, 0)
			SetWidgetImage(HUD.WHealthBarFlash, 0)
			SetWidgetTextColor(HUD.WHealthBarText, ARGB(255,136,241,254))
		}

//		float add = rate * 40
		SetWidgetPos(HUD.WHealthBar, 17, 565, BarWidth, 18)
		SetWidgetPos(HUD.WHealthBarFlash, -8, 534, BarWidth + 50, 80)

		float h = rate * 100
		int Numeric = h
		
		if(Numeric == 0 && h > 0)	//nemoze ukazovat nulu ked ma este trosicku energie!
			Numeric = 1
		else
		{
			if(Numeric == 100 && h < 100)	//nemoze ukazovat maximum v skutocnosti tam neni plnych 100%
				Numeric = 99
		}

		SetWidgetText(HUD.WHealthBarText, 0, itoa(Numeric))
	}

//---------------------------------------------------------------------------------------------------------
	void UpdateAmmoBar()
	{
/*		Print(CurrentWeapon.AmmoPerMagazine)
		Print(CurrentWeapon.Ammo)
		Print(Ammo[CurrentWeapon.AmmoType])*/
		
		int MaxAmmoPerMagazine = CurrentWeapon.AmmoPerMagazine
		int AmmoInMagazine = CurrentWeapon.Ammo
		int OtherAmmo = Ammo[CurrentWeapon.AmmoType]
		int ImageIndex = CurrentWeapon.ClassNum - 1

/*		if(CurrentWeapon.ClassNum == CGrenade)	//trapna vynimka na granaty
		{
			AmmoInMagazine = AmmoInMagazine + OtherAmmo
			MaxAmmoPerMagazine = 5
			OtherAmmo = MaxAmmoPerMagazine

			if(AmmoInMagazine > MaxAmmoPerMagazine)
				AmmoInMagazine = MaxAmmoPerMagazine
		}*/

		if(CurrentWeapon.ClassNum == CGrenade)	//trapna vynimka na granaty
		{
			AmmoInMagazine = AmmoInMagazine + OtherAmmo
			MaxAmmoPerMagazine = GrenadeMaximumAmmo
			OtherAmmo = MaxAmmoPerMagazine

			if(AmmoInMagazine > MaxAmmoPerMagazine)
				AmmoInMagazine = MaxAmmoPerMagazine
		}

		float BarLeft
		float BarTop
		float BarWidth
		float BarHeight = 18
		
		switch(CurrentWeapon.ClassNum)
		{
			case CPistol:
				BarWidth = 70
				BarLeft = 800 - BarWidth - 17
				BarTop = 565
				break

			case CShotgun:
				BarWidth = 64
				BarLeft = 800 - BarWidth - 17
				BarTop = 565
				break

			case CMachinegun:
				BarWidth = 112
				BarLeft = 800 - BarWidth - 17
				BarTop = 565
				break

			case CSniperRifle:
				BarWidth = 80
				BarLeft = 800 - BarWidth - 17
				BarTop = 565
				break
				
			case CGrenade:
				BarWidth = 110
				BarLeft = 800 - BarWidth - 17
				BarTop = 565
				break
			default:
			
		}
		
		if(MaxAmmoPerMagazine > 0)
		{
			float rate = AmmoInMagazine / MaxAmmoPerMagazine
			float CurWidth = rate * BarWidth
			float CurLeft = BarLeft + (BarWidth - CurWidth)
			SetWidgetPos(HUD.WAmmoBar, CurLeft, BarTop, CurWidth, BarHeight)
			SetWidgetPos(HUD.WAmmoBarBack, BarLeft, BarTop, BarWidth, BarHeight)
			SetWidgetImage(HUD.WAmmoBar, ImageIndex)
			SetWidgetImage(HUD.WAmmoBarBack, ImageIndex)
			ShowWidget(HUD.WAmmoBar, true)
			ShowWidget(HUD.WAmmoBarBack, true)
			ShowWidget(HUD.WAmmoBarText, true)
			
			if(CurrentWeapon.ClassNum == CGrenade)
				SetWidgetText(HUD.WAmmoBarText, 0, itoa(AmmoInMagazine))
			else
				SetWidgetText(HUD.WAmmoBarText, 0, itoa(AmmoInMagazine) + "/" + itoa(OtherAmmo))
		}
		else
		{
			ShowWidget(HUD.WAmmoBar, false)
			ShowWidget(HUD.WAmmoBarBack, false)
			ShowWidget(HUD.WAmmoBarText, false)
		}
	}

//---------------------------------------------------------------------------------------------------------
	bool EnableIdleWarAnims(bool stat)
	{
		if(stat && status == ST_NONE && CurrentWeapon.Action == 0 && ExactAim == 0)
			IdleVarTimer = frand(IdleWarAnimWaitMin, IdleWarAnimWaitMax)
		else
		{
			IdleVarTimer = 0
			SetAnimSlot(Hands, s_walk - 1, VNULL, 15, 15, NULL, 30, 0)
		}
	}

//---------------------------------------------------------------------------------------------------------
	bool SelectWeapon(WeaponClass wc)
	{
	}

//---------------------------------------------------------------------------------------------------------
	bool UnselectWeapon()
	{
	}

//---------------------------------------------------------------------------------------------------------
	void AddItem(ExtClass cc)	//Item je pred kamerou
	{
		if(InteractionAdeptsNum >= 80)
			return

#ifdef EDITOR
		if(World.EditorActive == true)
			return
#endif

		if(cc == CurrentWeapon)
			return
			
		InteractionAdepts[InteractionAdeptsNum++] = cc
	}

//---------------------------------------------------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
	}

//---------------------------------------------------------------------------------------------------------
	void player_interface()
	{
		if(World.MusicPlayer == NULL)
			World.MusicPlayer = new music_player()
	}
}
