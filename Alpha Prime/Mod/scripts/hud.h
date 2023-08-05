
string RandomHexadecimalText(int lng)
{
	const string hexchars[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"}
	string str = ""
	
	for(int n = 0; n < lng; n++)
	{
		str += hexchars[rand(0, 15)]
	}
	return str
}

//-----------------------------------------------------------------
void InitWidgets()
{
	WFlash = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA|WF_ADDITIVE, 0x00ffffff, 10)	
	
	WText = CreateWidget(WTYPE_TEXT, 140, 579, 10, 10, WF_VISIBLE | WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 5)
	SetFont(WText, MainFont)
	
	WDialogText = CreateWidget(WTYPE_TEXT, 50, 500, 700, 100, WF_SOURCEALPHA|WF_BLEND|WF_CENTER|WF_VCENTER, ARGB(255,255,255,255), 15)
	SetFont(WDialogText, SmallFont)
}

//-----------------------------------------------------------------
void CleanupWidgets()
{
	if(WFlash)
	{
		DestroyWidget(WFlash)
		WFlash = NULL
	}
	
	if(WText)
	{
		DestroyWidget(WText)
		WText = NULL
	}
	
	if(WView)
	{
		DestroyWidget(WView)
		WView = NULL
	}
	
	if(WMousePointer)
	{
		DestroyWidget(WMousePointer)
		WMousePointer = NULL
	}
	
	if(WDialogText)
	{
		DestroyWidget(WDialogText)
		WDialogText = NULL
	}
}

//-----------------------------------------------------------------
class PlayerHUD extends _serializable
{
	widget WHumanStat
	widget WHealthBar, WHealthBarBack, WHealthBarText, WHealthBarFlash
	widget WTimeBar
	widget WAmmoBar, WAmmoBarBack, WAmmoBarText
	widget WOxygenBar, WOxygenBarBack, WOxygenBarText
	widget WThrowBar, WThrowBarBack
	widget WeaponList[10]
	widget WLightBar, WLightBarBack
	widget WHitZones[4]
	widget WHackingStrip
	widget WHackingModeGrid, WHackingModeGrid2
	widget WHackingModeCross[4]
	widget WHackingPreviewView, WHackingPreviewNoise, WHackingPreviewFrame, WHackingPreviewTitle
	widget WHackingSignal, WHackingSignalBack, WHackingSignalFlash

	void PlayerHUD()
	{
		int n
		WHumanStat = CreateWidget(WTYPE_IMAGE, 550, 12, 256, 91, WF_SOURCEALPHA|WF_BLEND | WF_NOFILTER, 0x00000000, 1)
		LoadWidgetImage(WHumanStat, 0, "gfx/hud_healthbar/healthbar_body.tga")
	
		//width: 1 to 121 (health 1% to 100%)
		WHealthBar = CreateWidget(WTYPE_IMAGE, 17, 565, 200, 18, WF_SOURCEALPHA|WF_BLEND | WF_ADDITIVE, 0x8fffffff, 3)	//75% transp
		LoadWidgetImage(WHealthBar, 0, "gfx/hud/healthbar.tga")
		LoadWidgetImage(WHealthBar, 1, "gfx/hud/healthbar_low.tga")
	
		WHealthBarFlash = CreateWidget(WTYPE_IMAGE, 17, 565, 200, 18, WF_SOURCEALPHA|WF_ADDITIVE | WF_STRETCH, 0xbfffffff, 2)	//75% transp
		LoadWidgetImage(WHealthBarFlash, 0, "gfx/hud/healthbar_flash.tga")
		LoadWidgetImage(WHealthBarFlash, 1, "gfx/hud/healthbar_low_flash.tga")
	//	LoadWidgetImage(WHealthBarFlash, 0, "textures/debug_UV.tga")
	//	LoadWidgetImage(WHealthBarFlash, 1, "textures/debug_UV.tga")
	
		WOxygenBar = CreateWidget(WTYPE_IMAGE, 17, 490, 100, 16, WF_SOURCEALPHA|WF_BLEND | WF_ADDITIVE, 0xbfffffff, 3)	//75% transp
		WOxygenBarBack = CreateWidget(WTYPE_IMAGE, 17, 495, 100, 16, WF_SOURCEALPHA|WF_BLEND, 0x3fffffff, 2)	//25% transp
		WOxygenBarText = CreateWidget(WTYPE_TEXT, 17, 480, 100, 32, WF_SOURCEALPHA|WF_BLEND, ARGB(255,130,255,255), 4)	//25% transp
		LoadWidgetImage(WOxygenBar, 0, "gfx/hud/oxygenbar.tga")
		LoadWidgetImage(WOxygenBarBack, 0, "gfx/hud/oxygenbar.tga")
		SetWidgetTextEx(WOxygenBarText, 0, 1, "#OXYGEN")
		SetFont(WOxygenBarText, MainFont)
	//	LoadWidgetImage(WOxygenBar, 0, "gfx/hud/oxygenbar_low.tga")
	
		WHealthBarBack = CreateWidget(WTYPE_IMAGE, 17, 565, 200, 18, WF_SOURCEALPHA|WF_BLEND, 0x3fffffff, 2)	//25% transp
		LoadWidgetImage(WHealthBarBack, 0, "gfx/hud/healthbar.tga")
	
		WHealthBarText = CreateWidget(WTYPE_TEXT, 17, 545, 128, 18, WF_SOURCEALPHA|WF_BLEND, ARGB(255,130,255,255), 3)
		SetFont(WHealthBarText, MainFont)
	
		WTimeBar = CreateWidget(WTYPE_IMAGE, 17, 565, 242, 32, WF_SOURCEALPHA|WF_BLEND | WF_ADDITIVE, 0xbfffffff, 3)	//75% transp
		LoadWidgetImage(WTimeBar, 0, "gfx/hud/timebar.tga")
		LoadWidgetImage(WTimeBar, 1, "gfx/hud/timebar_low.tga")
	
		WAmmoBar = CreateWidget(WTYPE_IMAGE, 662, 570, 128, 18, WF_SOURCEALPHA|WF_BLEND | WF_ADDITIVE, 0xbfffffff, 3)	//75% transp
		LoadWidgetImage(WAmmoBar, 1, "gfx/hud/ammobar_gun.tga")
		LoadWidgetImage(WAmmoBar, 2, "gfx/hud/ammobar_shotgun.tga")	//shotgun
		LoadWidgetImage(WAmmoBar, 3, "gfx/hud/ammobar_machinegun.tga")
		LoadWidgetImage(WAmmoBar, 4, "gfx/hud/ammobar_pulserifle.tga")
		LoadWidgetImage(WAmmoBar, 5, "gfx/hud/ammobar_machinegun.tga")	//RocketLauncher
		LoadWidgetImage(WAmmoBar, 6, "gfx/hud/ammobar_machinegun.tga")   //FlameThrower  
		LoadWidgetImage(WAmmoBar, 7, "gfx/hud/ammobar_grenade.tga")
		
		WAmmoBarBack = CreateWidget(WTYPE_IMAGE, 662, 570, 128, 18, WF_SOURCEALPHA|WF_BLEND, 0x3f000000, 2)	//25% transp
		LoadWidgetImage(WAmmoBarBack, 1, "gfx/hud/ammobar_gun.tga")
		LoadWidgetImage(WAmmoBarBack, 2, "gfx/hud/ammobar_shotgun.tga")
		LoadWidgetImage(WAmmoBarBack, 3, "gfx/hud/ammobar_machinegun.tga")
		LoadWidgetImage(WAmmoBarBack, 4, "gfx/hud/ammobar_pulserifle.tga")
		LoadWidgetImage(WAmmoBarBack, 5, "gfx/hud/ammobar_machinegun.tga")	//RocketLauncher
		LoadWidgetImage(WAmmoBarBack, 6, "gfx/hud/ammobar_machinegun.tga")   //FlameThrower  
		LoadWidgetImage(WAmmoBarBack, 7, "gfx/hud/ammobar_grenade.tga")
	
		WAmmoBarText = CreateWidget(WTYPE_TEXT, 655, 545, 128, 18, WF_SOURCEALPHA|WF_BLEND|WF_RALIGN, ARGB(255,130,255,255), 3)
		SetFont(WAmmoBarText, MainFont)
		
		WThrowBar = CreateWidget(WTYPE_IMAGE, 360, 350, 4, 28, WF_SOURCEALPHA|WF_BLEND|WF_SOURCEALPHA|WF_FLIPV, 0xbfffffff, 3)
		WThrowBarBack = CreateWidget(WTYPE_IMAGE, 360, 350, 80, 28, WF_SOURCEALPHA|WF_BLEND|WF_SOURCEALPHA|WF_FLIPV, 0x3f000000, 2)
		LoadWidgetImage(WThrowBar, 0, "gfx/hud/throwbar.tga")
		LoadWidgetImage(WThrowBarBack, 0, "gfx/hud/throwbar.tga")
	
	/*	WeaponList[0] = CreateWidget(WTYPE_IMAGE, 662, 570, 128, 20, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 2)
		WeaponList[1] = CreateWidget(WTYPE_IMAGE, 662, 570, 128, 20, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 2)
		LoadWidgetImage(WeaponList[0], 0, "gfx/hud/slot_gun.tga")
		LoadWidgetImage(WeaponList[1], 0, "gfx/hud/slot_machinegun.tga")*/
		
		for(n = 0; n < MaxAvailableWeapons; n++)
		{
			WeaponList[n] = CreateWidget(WTYPE_IMAGE, 662, 570, 128, 20, WF_SOURCEALPHA|WF_BLEND | WF_NOWRAP, 0xffffffff, 2)
	//		LoadWidgetImage(WeaponList[n], 0, "gfx/hud/slot_empty.tga")	
		}
		LoadWidgetImage(WeaponList[0], 0, "gfx/hud/slot_jackhammer.tga")
		LoadWidgetImage(WeaponList[1], 0, "gfx/hud/slot_gun.tga")
		LoadWidgetImage(WeaponList[2], 0, "gfx/hud/slot_shotgun.tga")
		LoadWidgetImage(WeaponList[3], 0, "gfx/hud/slot_machinegun.tga")
		LoadWidgetImage(WeaponList[4], 0, "gfx/hud/slot_pulserifle.tga")
		LoadWidgetImage(WeaponList[5], 0, "gfx/hud/slot_disruptor.tga")	//RocketLauncher
		LoadWidgetImage(WeaponList[6], 0, "gfx/hud/slot_flamethrower.tga")	//FlameThrower
		LoadWidgetImage(WeaponList[7], 0, "gfx/hud/slot_grenade.tga")
	
		WLightBarBack = CreateWidget(WTYPE_IMAGE, 745, 45, 38, 20, WF_SOURCEALPHA|WF_BLEND, 0x3f000000, 2)
		LoadWidgetImage(WLightBarBack, 0, "gfx/hud/lightbar.tga")
		WLightBar = CreateWidgetEx(WLightBarBack, WTYPE_IMAGE, 745, 17, 38, 20, WF_SOURCEALPHA|WF_BLEND | WF_ADDITIVE, 0xffffffff, 3)
		LoadWidgetImage(WLightBar, 0, "gfx/hud/lightbar.tga")
	
		WHitZones[0] = CreateWidget(WTYPE_IMAGE, 0, 0, 64, 600, WF_SOURCEALPHA|WF_BLEND|WF_STRETCH|WF_ADDITIVE, 0xffffffff, 2)
		LoadWidgetImage(WHitZones[0], 0, "gfx/hud/pain_left.tga")
	
		WHitZones[1] = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 64, WF_SOURCEALPHA|WF_BLEND|WF_STRETCH|WF_ADDITIVE, 0xffffffff, 2)
		LoadWidgetImage(WHitZones[1], 0, "gfx/hud/pain_top.tga")
	
		WHitZones[2] = CreateWidget(WTYPE_IMAGE, 736, 0, 64, 600, WF_SOURCEALPHA|WF_BLEND|WF_STRETCH|WF_FLIPU|WF_ADDITIVE, 0xffffffff, 2)
		LoadWidgetImage(WHitZones[2], 0, "gfx/hud/pain_left.tga")
	
		WHitZones[3] = CreateWidget(WTYPE_IMAGE, 0, 536, 800, 64, WF_SOURCEALPHA|WF_BLEND|WF_STRETCH|WF_FLIPV|WF_ADDITIVE, 0xffffffff, 2)
		LoadWidgetImage(WHitZones[3], 0, "gfx/hud/pain_top.tga")
		
		WHackingStrip = CreateWidget(WTYPE_IMAGE, 0, 0, 256, 600, WF_SOURCEALPHA|WF_BLEND|WF_STRETCH|WF_ADDITIVE, 0xffffffff, 4)
		LoadWidgetImage(WHackingStrip, 0, "gfx/remotecontrol/radar_grad1.tga")
	
		WHackingModeGrid = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA/*|WF_NOFILTER|WF_EXACTPOS|WF_EXACTSIZE|WF_ADDITIVE*/|WF_BLEND, 0xffffffff/*0x1effffff*/, 1)	//12 % transp
		LoadWidgetImage(WHackingModeGrid, 0, "gfx/remotecontrol/radar_grid.tga")
	
		WHackingModeGrid2 = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 15)	//12 % transp
		LoadWidgetImage(WHackingModeGrid2, 0, "gfx/remotecontrol/radar_grid.tga")
	
		WHackingModeCross[0] = CreateWidget(WTYPE_IMAGE, 0, 0, 1024, 32, WF_SOURCEALPHA|WF_ADDITIVE, 0x1effffff, 9)
		WHackingModeCross[1] = CreateWidget(WTYPE_IMAGE, 0, 0, 1024, 32, WF_SOURCEALPHA|WF_FLIPU|WF_ADDITIVE, 0x1effffff, 9)
		WHackingModeCross[2] = CreateWidget(WTYPE_IMAGE, 0, 0, 32, 1024, WF_SOURCEALPHA|WF_ADDITIVE, 0x1effffff, 9)
		WHackingModeCross[3] = CreateWidget(WTYPE_IMAGE, 0, 0, 32, 1024, WF_SOURCEALPHA|WF_FLIPV|WF_ADDITIVE, 0x1effffff, 9)
		LoadWidgetImage(WHackingModeCross[0], 0, "gfx/remotecontrol/cross_h.tga")
		LoadWidgetImage(WHackingModeCross[1], 0, "gfx/remotecontrol/cross_h.tga")
		LoadWidgetImage(WHackingModeCross[2], 0, "gfx/remotecontrol/cross_v.tga")
		LoadWidgetImage(WHackingModeCross[3], 0, "gfx/remotecontrol/cross_v.tga")
		
		WHackingPreviewView = CreateWidget(WTYPE_VIEW, 17, 60, 160, 120, 0, 0, 11)
		WHackingPreviewNoise = CreateWidget(WTYPE_IMAGE, 0, 0, 320, 240, WF_ADDITIVE|WF_CUSTOMUV, ARGB(255,136,241,254), 12)
		LoadWidgetImage(WHackingPreviewNoise, 0, "gfx/remotecontrol/noise.tga")
		WHackingPreviewFrame = CreateWidget(WTYPE_IMAGE, 10, 17, 280, 220, WF_SOURCEALPHA|WF_BLEND|WF_STRETCH, 0xffffffff, 10)
		LoadWidgetImage(WHackingPreviewFrame, 0, "gfx/remotecontrol/frame1.tga")
		WHackingPreviewTitle = CreateWidget(WTYPE_TEXT, 18, 17, 270, 20, WF_SOURCEALPHA|WF_ADDITIVE|WF_NOFILTER, ARGB(255,130,255,255), 16)
		LoadWidgetImage(WHackingPreviewTitle, 0, "gfx/remotecontrol/titlegrad.tga")
		SetFont(WHackingPreviewTitle, MainFont)
		SetWidgetTextOffset(WHackingPreviewTitle, 4, 1)
		SetWidgetTextColor(WHackingPreviewTitle, 0xff000000)
		SetWidgetColor(WHackingPreviewTitle, 0xffffffff)
	
		WHackingSignal = CreateWidget(WTYPE_IMAGE, 756, 17, 32, 32, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 10)
		WHackingSignalBack = CreateWidget(WTYPE_IMAGE, 756, 17, 32, 32, WF_SOURCEALPHA|WF_BLEND, 0x3fffffff, 9)
		LoadWidgetImage(WHackingSignal, 0, "gfx/remotecontrol/signal.tga")
		LoadWidgetImage(WHackingSignalBack, 0, "gfx/remotecontrol/signal.tga")
	
		WHackingSignalFlash = CreateWidget(WTYPE_IMAGE, 710, -28, 120, 120, WF_SOURCEALPHA|WF_BLEND | WF_ADDITIVE | WF_STRETCH, 0x00ffffff, 11)	//75% transp
		LoadWidgetImage(WHackingSignalFlash, 0, "gfx/remotecontrol/signal_flash.tga")		
	}

//-----------------------------------------------------------------
	void ~PlayerHUD()
	{
		if(WHumanStat)
		{
			DestroyWidget(WHumanStat)
			WHumanStat = NULL
		}
		
		if(WHealthBar)
		{
			DestroyWidget(WHealthBar)
			WHealthBar = NULL
		}
		
		if(WHealthBarFlash)
		{
			DestroyWidget(WHealthBarFlash)
			WHealthBarFlash = NULL
		}
		
		if(WHealthBarBack)
		{
			DestroyWidget(WHealthBarBack)
			WHealthBarBack = NULL
		}
		
		if(WHealthBarText)
		{
			DestroyWidget(WHealthBarText)
			WHealthBarText = NULL
		}
		
		if(WTimeBar)
		{
			DestroyWidget(WTimeBar)
			WTimeBar = NULL
		}
		
		if(WLightBar)
		{
			DestroyWidget(WLightBar)
			WLightBar = NULL
		}
		
		if(WLightBarBack)
		{
			DestroyWidget(WLightBarBack)
			WLightBarBack = NULL
		}
		
		if(WAmmoBar)
		{
			DestroyWidget(WAmmoBar)
			WAmmoBar = NULL
		}
		
		if(WAmmoBarBack)
		{
			DestroyWidget(WAmmoBarBack)
			WAmmoBarBack = NULL
		}
		
		if(WAmmoBarText)
		{
			DestroyWidget(WAmmoBarText)
			WAmmoBarText = NULL
		}
		
		if(WThrowBar)
		{
			DestroyWidget(WThrowBar)
			WThrowBar = NULL
		}
		
		if(WThrowBarBack)
		{
			DestroyWidget(WThrowBarBack)
			WThrowBarBack = NULL
		}

		if(WHackingModeGrid)
		{
			DestroyWidget(WHackingModeGrid)
			WHackingModeGrid = NULL
		}
		
		if(WHackingModeGrid2)
		{
			DestroyWidget(WHackingModeGrid2)
			WHackingModeGrid2 = NULL
		}
		
		if(WHackingStrip)
		{
			DestroyWidget(WHackingStrip)
			WHackingStrip = NULL
		}
		
		if(WOxygenBar)
		{
			DestroyWidget(WOxygenBar)
			WOxygenBar = NULL
		}
		
		if(WOxygenBarBack)
		{
			DestroyWidget(WOxygenBarBack)
			WOxygenBarBack = NULL
		}
		
		if(WOxygenBarText)
		{
			DestroyWidget(WOxygenBarText)
			WOxygenBarText = NULL
		}
		
		if(WHackingPreviewView)
		{
			DestroyWidget(WHackingPreviewView)
			WHackingPreviewView = NULL
		}

		if(WHackingPreviewNoise)
		{
			DestroyWidget(WHackingPreviewNoise)
			WHackingPreviewNoise = NULL
		}
		
		if(WHackingPreviewFrame)
		{
			DestroyWidget(WHackingPreviewFrame)
			WHackingPreviewFrame = NULL
		}
		
		if(WHackingPreviewTitle)
		{
			DestroyWidget(WHackingPreviewTitle)
			WHackingPreviewTitle = NULL
		}
		
		if(WHackingSignal)
		{
			DestroyWidget(WHackingSignal)
			WHackingSignal = NULL
		}
		
		if(WHackingSignalBack)
		{
			DestroyWidget(WHackingSignalBack)
			WHackingSignalBack = NULL
		}
		
		if(WHackingSignalFlash)
		{
			DestroyWidget(WHackingSignalFlash)
			WHackingSignalFlash = NULL
		}

		int n
		for(n = 0; n < sizeof(WHackingModeCross); n++)
		{
			if(WHackingModeCross[n])
			{
				DestroyWidget(WHackingModeCross[n])
				WHackingModeCross[n] = NULL
			}
		}
		
		for(n = 0; n < sizeof(WeaponList); n++)
		{
			if(WeaponList[n])
			{
				DestroyWidget(WeaponList[n])
				WeaponList[n] = NULL
			}
		}
	
		for(n = 0; n < sizeof(WHitZones); n++)
		{
			if(WHitZones[n])
			{
				DestroyWidget(WHitZones[n])
				WHitZones[n] = NULL
			}
		}
	}
}

//-----------------------------------------------------------------
int GetSayCharacterImageIndex(string SayCharacter)
{
	strlower(SayCharacter)

	if(SayCharacter == "livie")
		return 0

	if(SayCharacter == "warren")
		return 1

	if(SayCharacter == "computer")
		return 2

	if(SayCharacter == "arnold")
		return 3

	if(SayCharacter == "faggioli")
		return 4

	if(SayCharacter == "bruce")
		return 5

	if(SayCharacter == "olivier")
		return 6

	if(SayCharacter == "tutorial")	//v tutoriali to sice nepotrebujeme ale nech sa nerobia vynimky
		return 2

	return -1
}

class SizablePanel extends _serializable
{
	int left, top, width, height
	int TopFrameHeight
	int TextFrameHeight
	int BottomFrameHeight
	widget TopFrame
	widget TextFrame
	widget BottomFrame

//-----------------------------------------------------------------
	void Update()
	{
		SetWidgetPos(TopFrame, left, top, width, TopFrameHeight)
		SetWidgetPos(TextFrame, left, top + TopFrameHeight, width, TextFrameHeight)
		SetWidgetPos(BottomFrame, left, top + TopFrameHeight + TextFrameHeight, width, BottomFrameHeight)
	}

//-----------------------------------------------------------------
	void SetPos(int Left, int Top)
	{
		left = Left
		top = Top
		Update()
	}

//-----------------------------------------------------------------
	int GetHeight()
	{
		return TopFrameHeight + TextFrameHeight + BottomFrameHeight
	}

//-----------------------------------------------------------------
	void Show(bool stat)
	{
		ShowWidget(TopFrame, stat)
		ShowWidget(TextFrame, stat)
		ShowWidget(BottomFrame, stat)
	}
	
//-----------------------------------------------------------------
	void SetPanelText(string text)
	{
		for(int n = 0; n < 16; n++)
			SetWidgetText(TextFrame, n, "")

		int NumLines = SetWidgetTextEx(TextFrame, 0, 1, text)

		if(NumLines == 0)
			NumLines = 1
		
		if(text == "")
			NumLines = 0

		TextFrameHeight = 40 * NumLines
		int h = TopFrameHeight + TextFrameHeight + BottomFrameHeight
			
		Update()
	}

//-----------------------------------------------------------------
	void ~SizablePanel()
	{
		if(TopFrame)
		{
			DestroyWidget(TopFrame)
			TopFrame = NULL
		}
		
		if(TextFrame)
		{
			DestroyWidget(TextFrame)
			TextFrame = NULL
		}

		if(BottomFrame)
		{
			DestroyWidget(BottomFrame)
			BottomFrame = NULL
		}
	}
}



class DialogPanel extends SizablePanel
{
	widget FaceIcon
	int FaceIconWidth, FaceIconHeight
	float DialogScroll
	int	status

//-----------------------------------------------------------------
	void Update()
	{
		SetWidgetPos(TopFrame, left, top, width, TopFrameHeight)
		SetWidgetPos(TextFrame, left, top + TopFrameHeight, width, TextFrameHeight)
		SetWidgetPos(BottomFrame, left, top + TopFrameHeight + TextFrameHeight, width, BottomFrameHeight)
		SetWidgetPos(FaceIcon, left - 18, top - 12, FaceIconWidth, FaceIconHeight)
	}

//-----------------------------------------------------------------
	void Show(bool stat)
	{
		ShowWidget(TopFrame, stat)
		ShowWidget(TextFrame, stat)
		ShowWidget(BottomFrame, stat)
		ShowWidget(FaceIcon, stat)
	}

//-----------------------------------------------------------------
	void Open()
	{
		if(status == 2)	//waiting
			return

		status = 1
		Show(true)
	}

//-----------------------------------------------------------------
	void Close()
	{
		if(status == 0)	//is closed
			return

		status = 3
	}

//-----------------------------------------------------------------
	void SetDialogText(string text, string SayCharacter)
	{
		for(int n = 0; n < 16; n++)
			SetWidgetText(TextFrame, n, "")

		int NumLines = SetWidgetTextEx(TextFrame, 0, 1, text)
		int IconIndex = GetSayCharacterImageIndex(SayCharacter)
		
		if(IconIndex == -1)
			IconIndex = 0

		SetWidgetImage(FaceIcon, IconIndex)

		if(NumLines == 0)
			NumLines = 1
		
		if(text == "")
			NumLines = 0

		TextFrameHeight = 40 * NumLines
		int h = TopFrameHeight + TextFrameHeight + BottomFrameHeight
		
		if(FaceIconHeight > h)
			TextFrameHeight = FaceIconHeight - TopFrameHeight - BottomFrameHeight
			
		Update()
	}

//-----------------------------------------------------------------
	void OnFrame()
	{
		if(status == 0 || status == 2)
			return
		
		if(status == 1)	//vysuva sa
		{
			DialogScroll += ftime * 4
			
			if(DialogScroll >= 1)
			{
				DialogScroll = 1
				status = 2	//opened
			}
		}

		if(status == 3)	//zasuva sa
		{
			DialogScroll -= ftime * 4
			
			if(DialogScroll <= 0)
			{
				DialogScroll = 0
				status = 0
				Show(false)
			}
		}

		float flip = 1 - DialogScroll
		float FinalScroll = flip * flip
		FinalScroll = 1 - FinalScroll

		float lng = GetHeight()
		float scroll = lng + 17 * FinalScroll - lng
		SetPos(17, scroll)
	}

//-----------------------------------------------------------------
	void DialogPanel(int Left, int Top)
	{
		left = Left
		top = Top
		width = 512
		TopFrameHeight = 10
		TextFrameHeight = 0
		BottomFrameHeight = 4
		FaceIconWidth = 90
		FaceIconHeight = 90
		TopFrame = CreateWidget(WTYPE_IMAGE, 0, 0, 100, 100, WF_SOURCEALPHA|WF_BLEND, 0x00ffffff, 2)	//top
		TextFrame = CreateWidget(WTYPE_TEXT, 0, 0, width, 100, WF_SOURCEALPHA|WF_BLEND/*|WF_VCENTER*/, ARGB(255,130,255,255), 2)	//text
		BottomFrame = CreateWidget(WTYPE_IMAGE, 0, 0, 100, 100, WF_SOURCEALPHA|WF_BLEND, 0x00ffffff, 2)	//bottom
		FaceIcon = CreateWidget(WTYPE_IMAGE, 0, 0, 100, 100, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 3)
		LoadWidgetImage(TopFrame, 0, "gfx/hud/dialog_top.tga")
		//LoadWidgetImage(TextFrame, 0, "gfx/hud/dialog_middle.tga")
		LoadWidgetImage(BottomFrame, 0, "gfx/hud/dialog_bottom.tga")
		LoadWidgetImage(FaceIcon, 0, "gfx/hud/dialog_face1.tga")			//livie
		LoadWidgetImage(FaceIcon, 1, "gfx/hud/dialog_face2.tga")			//warren
		LoadWidgetImage(FaceIcon, 2, "gfx/hud/dialog_face3.tga")			//computer
		LoadWidgetImage(FaceIcon, 3, "gfx/hud/dialog_face4.tga")			//arnold
		LoadWidgetImage(FaceIcon, 4, "gfx/hud/dialog_face5.tga")			//faggioli
		LoadWidgetImage(FaceIcon, 5, "gfx/hud/dialog_face6.tga")			//bruce
		LoadWidgetImage(FaceIcon, 6, "gfx/hud/dialog_face7.tga")			//olivier
		
//		SetWidgetTextColor(TextFrame, 0xff000000)
//		SetWidgetColor(TextFrame, 0xffffffff)

		SetWidgetTextOffset(TextFrame, FaceIconWidth - 20, 0)
		SetWidgetTextSpacing(TextFrame, 0, -5)
		SetFont(TextFrame, MainFont)
		Update()
	}

//-----------------------------------------------------------------
	void ~DialogPanel()
	{
		if(FaceIcon) DestroyWidget(FaceIcon)
	}
}

//=================================================================
class SubtitlesDialog extends _serializable
{
	widget msg[2]

//-----------------------------------------------------------------
	void SetText(string text)
	{
		for(int n = 0; n < 8; n++)
		{
			SetWidgetText(msg[0], n, "")
			SetWidgetText(msg[1], n, "")
		}

		SetWidgetTextEx(msg[0], 0, 1, text)
		SetWidgetTextEx(msg[1], 0, 1, text)
	}

//-----------------------------------------------------------------
	void Show(bool show)
	{
		ShowWidget(msg[0], show)
		ShowWidget(msg[1], show)
	}
	
//-----------------------------------------------------------------
	void SubtitlesDialog(int left, int top, int width, int height)
	{
		int ShadowSize = 1
		msg[0] = CreateWidget(WTYPE_TEXT, left, top, width, height, WF_SOURCEALPHA|WF_BLEND|WF_CENTER|WF_VCENTER, ARGB(255,255,255,255), 2)
		msg[1] = CreateWidget(WTYPE_TEXT, left + ShadowSize, top + ShadowSize, width, height, WF_SOURCEALPHA|WF_BLEND|WF_CENTER|WF_VCENTER, ARGB(255,0,0,0), 1)
		
		SetWidgetTextOffset(msg[0], 10, 0)
		SetWidgetTextOffset(msg[1], 10, 0)
//		SetWidgetTextSpacing(msg[0], 0, -5)
//		SetWidgetTextSpacing(msg[1], 0, -5)
		SetFont(msg[0], SmallFont)
		SetFont(msg[1], SmallFont)
	}

//-----------------------------------------------------------------
	void ~SubtitlesDialog()
	{
		if(msg[0])
		{
			DestroyWidget(msg[0])
			msg[0] = NULL
		}

		if(msg[1])
		{
			DestroyWidget(msg[1])
			msg[1] = NULL
		}
	}
}

class MessageWindow extends _serializable
{
	widget envelope, BackGround
	widget msg[8]
	float timers[8]
	float offset[8]
	int	position[8]
	int ptr
	int status
	int LineHeight
	int AreaHeight
	int AreaWidth
	int TextColor
	float Opacity
	float GlobalTransp
	bool Automanaged	//je mu volane neustale OnFrame takze si sam zobrazuje a skryva widgety

//-----------------------------------------------------------------
	void UpdateColor()
	{
		int color = TextColor
		float fade, alpha

		for(int n = 0; n < sizeof(msg); n++)
		{
			if(timers[n] > 1)
				fade = timers[n] - 2
			else
				fade = 0

			clamp fade<0,1>
			fade = 1 - fade
			alpha = fade * GlobalTransp * (float)Opacity
			color &= 0x0082ffff   //0x00ffffff
			color |= alpha << 24
			SetWidgetTextColor(msg[n], color)
		}
		
		if(BackGround)
		{
			alpha = GlobalTransp * (float)Opacity
			color &= 0x0082ffff   //0x00ffffff
			color |= alpha << 24
			SetWidgetColor(BackGround, color)
		}
	}

//-----------------------------------------------------------------
	void SetGlobalTransp(float val)
	{
		GlobalTransp = val
		UpdateColor()
	}
	
//-----------------------------------------------------------------
	void Show(bool stat)
	{
//		if(stat == false)	//show neni potrebny
//		{
			for(int n = 0; n < sizeof(msg); n++)
			{
//				position[n] = 0
				ShowWidget(msg[n], stat)
			}
			ShowWidget(envelope, stat)
			
			if(BackGround)
				ShowWidget(BackGround, stat)
//			status = 0
//			ptr = 0
//		}
	}

//-----------------------------------------------------------------
	void ShowMessage(string message)
	{
 		status = 1
 		
 		if(Automanaged)
 		{
	 		ShowWidget(envelope, true)
	 		ShowWidget(msg[ptr], true)
 		}

 		SetWidgetTextColor(msg[ptr], TextColor)
 		SetWidgetTextEx(msg[ptr], 0, 1, message)	//znak "#" sa nemoze pridavat tu!!!
 		timers[ptr] = 0
		offset[ptr] = AreaHeight

		int n
 		for(n = 0; n < sizeof(msg); n++)
 		{
 			if(position[n] == 1)
 			{
 				offset[ptr] = offset[n] + (float)LineHeight
 				break
 			}
 		}

 		for(n = 0; n < sizeof(msg); n++)
 		{
 			if(position[n] > 0)
 				position[n] = position[n] + 1
 		}

 		position[ptr] = 1
 		 		
 		ptr++

 		if(ptr >= sizeof(msg))
 			ptr = 0
	}

//-----------------------------------------------------------------
	void OnFrame()
	{
		if(status == 0)
			return
		
		int n
		float pos
		int NumActive = 0
		float TimeStep = ftime * 0.15
		float TimeStep2 = ftime * 2
		float TimeStep3 = ftime * 200
		
 		for(n = 0; n < sizeof(msg); n++)
 		{
 			if(position[n] > 0)
 			{
 				NumActive++
 				offset[n] = offset[n] - TimeStep3
 				float end = (float)position[n] * (float)LineHeight
 				end = AreaHeight - end

 				if(offset[n] <= end)
 					offset[n] = end

 				SetWidgetPos(msg[n], 0, offset[n], AreaWidth, LineHeight)

				if(timers[n] <= 1)	//waiting
				{
					timers[n] = timers[n] + TimeStep

					if(timers[n] >= 1)
						timers[n] = 2
				}
				else						//fading
				{
					timers[n] = timers[n] + TimeStep2
					
					if(timers[n] >= 3)
					{
						timers[n] = 3
						
						if(Automanaged)
							ShowWidget(msg[n], false)

						position[n] = 0
						SetWidgetPos(msg[n], 0, 151, AreaWidth, LineHeight)
					}
				}
 			}
 		}

		UpdateColor()

		if(Automanaged && NumActive == 0)
		{
			ShowWidget(envelope, false)
			status = 0
		}
	}

//-----------------------------------------------------------------
	void Init(int Left, int Top, int Width, int Height, int TxtOffset, string background, int Font, int TxtColor, int AddFlags, int SortOffset)
	{
		GlobalTransp = 1
		TextColor = TxtColor

		AreaHeight = Height - (TxtOffset * 2)
		AreaWidth = Width - (TxtOffset * 2)

		if(background)
		{
			BackGround = CreateWidget(WTYPE_IMAGE, Left, Top, Width, Height, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, SortOffset)
			LoadWidgetImage(BackGround, 0, background)
		}

		envelope = CreateWidget(WTYPE_IMAGE, Left + TxtOffset, Top + TxtOffset, AreaWidth, AreaHeight, WF_SOURCEALPHA|WF_BLEND|WF_VISIBLE, 0x00ffffff, SortOffset + 1)

		for(int n = 0; n < sizeof(msg); n++)
		{
			msg[n] = CreateWidgetEx(envelope, WTYPE_TEXT, 0, 151, Width, LineHeight, WF_SOURCEALPHA|WF_BLEND|WF_VCENTER|AddFlags, ARGB(255,130,255,255), SortOffset + 2)
			SetWidgetTextColor(msg[n], TextColor)
			SetFont(msg[n], Font)
		}		
	}

//-----------------------------------------------------------------
	void ~MessageWindow()
	{
		for(int n = 0; n < sizeof(msg); n++)
		{
			if(msg[n])
			{
				DestroyWidget(msg[n])	
				msg[n] = NULL
			}
		}

		if(envelope)
		{
			DestroyWidget(envelope)
			envelope = NULL
		}
		
		if(BackGround)
		{
			DestroyWidget(BackGround)
			BackGround = NULL
		}
	}
}

//-----------------------------------------------------------------
class InfoDialog extends MessageWindow
{
	void InfoDialog(int Left, int Top)
	{
		Automanaged = true
		LineHeight = 16
		AreaHeight = 150
		AreaWidth = 300
		Opacity = 255
		Init(Left, Top, AreaWidth, AreaHeight, 0, "", MainFont, 0xffffffff, WF_CENTER, 2)
	}
}

//-----------------------------------------------------------------
class HackInfoDialog extends MessageWindow
{
	void HackInfoDialog(int Left, int Top)
	{
		Automanaged = false
		LineHeight = 16
		AreaHeight = 150
		AreaWidth = 280
		Opacity = 255
		Init(Left, Top, AreaWidth, AreaHeight, 15, "gfx/remotecontrol/frame2.tga", MainFont, 0xffffffff/*ARGB(255,136,241,254)*/, WF_ADDITIVE, 4)
	}
}

class TasklistDialog extends SizablePanel
{
	DialogPanel	DPanel
	InfoDialog	InfDialog
	widget		Checkboxs[8]
	widget		WTaskTexts[8]
	widget		Label
	string		TaskTexts[8]
	bool			TaskCompleted[8]
	int			TaskTextLines[8]
	int			NumTasks
	int			status
	float			ScrollRate
	int			LineHeight	//vyska jedneho riadku textu
	int			TaskSpace	//medzera medzi jednotlivymi taskmi
	int			LabelHeight
	bool			NeedOpened
	float			AutoOpened

//-----------------------------------------------------------------
	void Update()
	{
		SetWidgetPos(TopFrame, left, top, width, TopFrameHeight)
		SetWidgetPos(TextFrame, left, top + TopFrameHeight, width, TextFrameHeight)
		SetWidgetPos(BottomFrame, left, top + TopFrameHeight + TextFrameHeight, width, BottomFrameHeight)
		SetWidgetPos(Label, left + 10, top + 10, width, 32)

		int TaskPosX = left
		int TaskPosY = top + 10 + LabelHeight

		for(int n = 0; n < NumTasks; n++)
		{
			if(n > 0)
			{
				TaskPosY += TaskTextLines[n - 1] * LineHeight
				TaskPosY += TaskSpace	//medzera medzi taskmi
			}
				
			SetWidgetPos(WTaskTexts[n], TaskPosX + 32, TaskPosY - 3, width, TaskTextLines[n] * LineHeight)
			SetWidgetPos(Checkboxs[n], TaskPosX + 10 , TaskPosY, 16, 16)
		}
	}

//-----------------------------------------------------------------
	void Show(bool stat)
	{
		ShowWidget(TopFrame, stat)
		ShowWidget(TextFrame, stat)
		ShowWidget(BottomFrame, stat)
		ShowWidget(Label, stat)

		int n
		for(n = 0; n < NumTasks; n++)
		{
			ShowWidget(Checkboxs[n], stat)
			ShowWidget(WTaskTexts[n], stat)
		}
		
		for(n = n; n < sizeof(Checkboxs); n++)
		{
			ShowWidget(Checkboxs[n], false)
			ShowWidget(WTaskTexts[n], false)
		}
	}

//-----------------------------------------------------------------
	void Open()
	{
		if(status == 0)
			Show(true)

		if(ScrollRate == 0)
			PlaySound(NULL, g_sCommonSounds[11], SF_ONCE)

		status = 1
	}

//-----------------------------------------------------------------
	void Close()
	{
		if(ScrollRate == 1)
			PlaySound(NULL, g_sCommonSounds[12], SF_ONCE)

		status = 3
	}

//-----------------------------------------------------------------
	void OnFrame()
	{
		if(AutoOpened > 0)
		{
			AutoOpened -= ftime * 0.15
			NeedOpened = true

			if(AutoOpened <= 0)
				AutoOpened = 0
		}
			
		if(NeedOpened)
		{
			if(status == 0)
				Open()
		}
		else
		{
			if(status == 2)
				Close()
		}

		if(status == 0)
			return

		if(status == 1)	//scroluje do screeny
		{
			ScrollRate += ftime * 4
			
			if(ScrollRate >= 1)
			{
				ScrollRate = 1
				status = 2	//nic sa nedeje
			}
		}

		if(status == 3)	//scroluje prec
		{
			ScrollRate -= ftime * 4

			if(ScrollRate <= 0)
			{
				ScrollRate = 0
				status = 0
				AutoOpened = 0
				Show(false)
			}
		}

		int space = 17
		int Top = DPanel.top + DPanel.GetHeight() + space

		float flip = 1 - ScrollRate
		float FinalScroll = flip * flip
		FinalScroll = 1 - FinalScroll

		float lng = width
		float scroll = lng + 17 * FinalScroll - lng
		SetPos(scroll, Top)
		NeedOpened = false
	}

//-----------------------------------------------------------------
	void ForceOpening()
	{
		NeedOpened = true
	}
/*
//-----------------------------------------------------------------
	void SwitchPos()
	{
		if(status == 1 || status == 2)
			Close()
		else
			Open()
	}
*/
//-----------------------------------------------------------------
	bool AddTask(string task)
	{
		if(NumTasks >= sizeof(Checkboxs))
			return false

		int n
		for(n = 0; n < NumTasks; n++)
		{
			if(TaskTexts[n] == task)
				return false
		}
		
		TaskTexts[NumTasks] = task
		TaskCompleted[NumTasks] = false
		task = "#" + task
		int NumLines = SetWidgetTextEx(WTaskTexts[NumTasks], 0, 1, task)
		SetWidgetImage(Checkboxs[NumTasks], 0)
		
		NumLines++

		if(task == "")
			NumLines = 0

		TaskTextLines[NumTasks] = NumLines
		NumTasks++

		if(status > 0)
			Show(true)

		TextFrameHeight = LabelHeight

		for(n = 0; n < NumTasks; n++)
		{
			if(n > 0)
				TextFrameHeight += TaskSpace

			TextFrameHeight += TaskTextLines[n] * LineHeight
		}
		
		TextFrameHeight += 4	//spodny okraj

		Update()
		InfDialog.ShowMessage("#New_task_available...")
		PlaySound(NULL, g_sCommonSounds[13], SF_ONCE)
		AutoOpened = 1
		return true
	}

//-----------------------------------------------------------------
	bool CheckTask(string task)
	{
		for(int n = 0; n < NumTasks; n++)
		{
			if(TaskTexts[n] == task && TaskCompleted[n] == false)
			{
				TaskCompleted[n] = true
				SetWidgetImage(Checkboxs[n], 1)
				SetWidgetTextColor(WTaskTexts[n], ARGB(255,30,155,155))
				InfDialog.ShowMessage("#Task_completed...")
				PlaySound(NULL, g_sCommonSounds[14], SF_ONCE)
				AutoOpened = 1
				return true
			}
		}
		return false
	}

//-----------------------------------------------------------------
	void TasklistDialog(DialogPanel panel, InfoDialog dlg, int Left, int Top)
	{
		DPanel = panel
		InfDialog = dlg
		LabelHeight = 40
		LineHeight = 20
		TaskSpace = 8
		left = Left
		top = Top
		width = 512
		TopFrameHeight = 10
		TextFrameHeight = 64
		BottomFrameHeight = 4
		TopFrame = CreateWidget(WTYPE_IMAGE, left, 0, width, 100, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 17)	//top
		TextFrame = CreateWidget(WTYPE_IMAGE, left, 0, width, 100, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 17)	//posluzi iba ako pozadie
		BottomFrame = CreateWidget(WTYPE_IMAGE, left, 0, width, 100, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 17)	//bottom
		Label = CreateWidget(WTYPE_TEXT, left, 0, 100, 30, WF_SOURCEALPHA|WF_BLEND, ARGB(255,130,255,255), 18)
		SetFont(Label, MainFont)
		SetWidgetTextEx(Label, 0, 1, "#Objectives :")
		LoadWidgetImage(TopFrame, 0, "gfx/hud/dialog_top.tga")
		LoadWidgetImage(TextFrame, 0, "gfx/hud/dialog_middle.tga")
		LoadWidgetImage(BottomFrame, 0, "gfx/hud/dialog_bottom.tga")

		for(int n = 0; n < sizeof(Checkboxs); n++)
		{
			Checkboxs[n] = CreateWidget(WTYPE_IMAGE, left, 0, 16, 16, WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 18)	//top
			LoadWidgetImage(Checkboxs[n], 0, "gfx/hud/checkbox_off.tga")
			LoadWidgetImage(Checkboxs[n], 1, "gfx/hud/checkbox_on.tga")
			WTaskTexts[n] = CreateWidget(WTYPE_TEXT, left, 0, width - 32, 64, WF_SOURCEALPHA|WF_BLEND, ARGB(255,130,255,255), 18)	//text
//			SetWidgetTextOffset(WTaskTexts[n], 32, 0)
			SetWidgetTextSpacing(WTaskTexts[n], 0, -2)
			SetFont(WTaskTexts[n], MainFont)
		}
		Update()
	}
	
//-----------------------------------------------------------------
	void ~TasklistDialog()
	{
		if(Label)
		{
			DestroyWidget(Label)
			Label = NULL
		}

		for(int n = 0; n < sizeof(Checkboxs); n++)
		{
			if(Checkboxs[n])
			{
				DestroyWidget(Checkboxs[n])
				DestroyWidget(WTaskTexts[n])
				Checkboxs[n] = NULL
				WTaskTexts[n] = NULL
			}
		}
	}
}


//============================================================================
class CrossHairClass extends _serializable
{
	widget Dot
	widget Left, Top, Right, Bottom
	float CurScale
	float TargetScale
	float ScreenCenterX, ScreenCenterY
	float	MinSize
	float	MaxSize
	int	TypeIndex
	int	SkinIndex
	int	NumSkins
	bool	visible

//-----------------------------------------------------------------
	float GetPrecisionScale()
	{
		float res = MaxSize - MinSize * CurScale
		res += MinSize

		if(MaxSize > 0)
			res /= MaxSize
			
		return res
	}

//-----------------------------------------------------------------
	void Update()
	{
		ScreenCenterX = g_iWidth / 2
		ScreenCenterY = g_iHeight / 2

		SetWidgetPos(Dot, ScreenCenterX - 32, ScreenCenterY - 32, 64, 64)

		if(TypeIndex > 0)
			return

		float offset1 = MinSize
		offset1 += MaxSize - MinSize * CurScale
		float offset2 = offset1 + 32
		float wleft = ScreenCenterX - 16
		float wtop = ScreenCenterY - 16

		SetWidgetPos(Left, ScreenCenterX - offset2 - 1, wtop, 32, 32)
		SetWidgetPos(Right, ScreenCenterX + offset1, wtop, 32, 32)
		SetWidgetPos(Top, wleft, ScreenCenterY - offset2 - 1, 32, 32)
		SetWidgetPos(Bottom, wleft, ScreenCenterY + offset1, 32, 32)
	}

//-----------------------------------------------------------------
	void OnFrame()
	{
		if(CurScale != TargetScale)
		{
			float dif = TargetScale - CurScale
			CurScale += dif * (0.1 * ftime * 100)
			Update()
		}
	}

//-----------------------------------------------------------------
	void Show(bool stat)
	{
		visible = stat
		
		if(TypeIndex != 5)
			ShowWidget(Dot, stat)
		else						//ziadny cross
		{
			if(stat == false)
				ShowWidget(Dot, stat)
		}
		
		if(TypeIndex == 0)
		{
			ShowWidget(Left, stat)
			ShowWidget(Right, stat)
			ShowWidget(Top, stat)
			ShowWidget(Bottom, stat)
		}
	}

//-----------------------------------------------------------------
	void SetScale(float scale)
	{
		TargetScale = scale
		clamp TargetScale<0, 1>
	}

//-----------------------------------------------------------------
	void SetMinAndMaxSize(float SizeMin, float SizeMax)
	{
		MinSize = SizeMin
		MaxSize = SizeMax
	}

//-----------------------------------------------------------------
	void SetSkin(int skin)
	{
		int MaxSkinIndex = NumSkins - 1
		clamp skin<0, MaxSkinIndex>
		SkinIndex = skin

		SetWidgetImage(Left, skin)
		SetWidgetImage(Right, skin)
		SetWidgetImage(Top, skin)
		SetWidgetImage(Bottom, skin)
	}

//-----------------------------------------------------------------
	void SetType(int type)
	{
		TypeIndex = type
		
		if(type != 5)
		{
			SetWidgetImage(Dot, type)
			ShowWidget(Dot, visible)
		}
		else
			ShowWidget(Dot, false)	//absolutne ziadny cross

		if(type > 0)	//typ nula su zbrane takze sizovatelny kurzor
		{
			ShowWidget(Left, false)
			ShowWidget(Right, false)
			ShowWidget(Top, false)
			ShowWidget(Bottom, false)
		}

		Update()
	}

//-----------------------------------------------------------------
	void CrossHairClass()
	{
		ScreenCenterX = g_iWidth / 2
		ScreenCenterY = g_iHeight / 2
		CurScale = 0
		TargetScale = 0

		Dot = CreateWidget(WTYPE_IMAGE, ScreenCenterX - 32, ScreenCenterY - 32, 64, 64, WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTPOS|WF_EXACTSIZE, 0xffffffff, 3)
		Left = CreateWidget(WTYPE_IMAGE, ScreenCenterX - 32, ScreenCenterY - 16, 32, 32, WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTPOS|WF_EXACTSIZE|WF_FLIPU, 0xffffffff, 3)
		Right = CreateWidget(WTYPE_IMAGE, ScreenCenterX, ScreenCenterY - 16, 32, 32, WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTPOS|WF_EXACTSIZE, 0xffffffff, 3)
		Top = CreateWidget(WTYPE_IMAGE, ScreenCenterX - 16, ScreenCenterY - 32, 32, 32, WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTPOS|WF_EXACTSIZE|WF_FLIPV, 0xffffffff, 3)
		Bottom = CreateWidget(WTYPE_IMAGE, ScreenCenterX - 16, ScreenCenterY, 32, 32, WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTPOS|WF_EXACTSIZE, 0xffffffff, 3)

		LoadWidgetImage(Dot, 0, "gfx/gunsight/dot.tga")	//zbran		"gfx/gunsight/dot.tga"	gfx/cross_null.tga
		LoadWidgetImage(Dot, 1, "gfx/hud/icon_use.tga")		//use icon
		LoadWidgetImage(Dot, 2, "gfx/hud/icon_no.tga")	//use no
		LoadWidgetImage(Dot, 3, "gfx/gunsight/dot.tga")	//no weapon
		LoadWidgetImage(Dot, 4, "gfx/hud/icon_grab.tga")//grab icon

		string Stype
		NumSkins = 2

		for(int n = 0; n < NumSkins; n++)
		{
			Stype = itoa(n + 1)
			LoadWidgetImage(Left, n, "gfx/gunsight/" + Stype + "r.tga")
			LoadWidgetImage(Right, n, "gfx/gunsight/" + Stype + "r.tga")
			LoadWidgetImage(Top, n, "gfx/gunsight/" + Stype + "d.tga")
			LoadWidgetImage(Bottom, n, "gfx/gunsight/" + Stype + "d.tga")
		}

		SetSkin(0)
		Update()
//		Show(true)
	}

//-----------------------------------------------------------------
	void ~CrossHairClass()
	{
		if(Dot)
		{
			DestroyWidget(Dot)
			Dot = NULL
		}

		if(Left)
		{
			DestroyWidget(Left)
			Left = NULL
		}

		if(Right)
		{
			DestroyWidget(Right)
			Right = NULL
		}

		if(Top)
		{
			DestroyWidget(Top)
			Top = NULL
		}

		if(Bottom)
		{
			DestroyWidget(Bottom)
			Bottom = NULL
		}
	}
}
