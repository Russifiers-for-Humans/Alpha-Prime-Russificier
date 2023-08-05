
//-----------------------------------------------------------------
void UpdateMouse()
{
	MouseX = MouseX + MouseDeltaX
	float v = 0
	
	if(fabs(JoyAxis[0]) > JOY_DEATH_ZONE)
		v = JoyAxis[0] * 0.02

	MouseX = MouseX + v
	clamp MouseX<0, 800>
	MouseY = MouseY + MouseDeltaY
	v = 0
	
	if(fabs(JoyAxis[1]) > JOY_DEATH_ZONE)
		v = JoyAxis[1] * 0.02
	
	MouseY = MouseY + v
	clamp MouseY<0, 600>
	SetWidgetPos(WMousePointer, MouseX, MouseY, 63, 63)
}

class ConfirmDialog
{
	widget background
	widget ButtonYes
	widget ButtonNo
	string ResultOnYes
	string ResultOnNo
	int ButtonYesLeft
	int ButtonYesTop
	int ButtonYesWidth
	int ButtonYesHeight
	int ButtonNoLeft
	int ButtonNoTop
	int ButtonNoWidth
	int ButtonNoHeight
	int ButtonInactiveColor
	int ButtonActiveColor
	
	int ButtonNoSizes[4]

	string Update()
	{
		if(KeyState(KC_RETURN))
		{
			ClearKey(KC_RETURN)
			return ResultOnYes
		}

		if(KeyState(KC_ESCAPE))
		{
			ClearKey(KC_ESCAPE)
			return ResultOnNo
		}

		UpdateMouse()

		if(InRectEx(ButtonYesLeft ,ButtonYesTop ,ButtonYesWidth ,ButtonYesHeight, MouseX, MouseY) == true)
		{
			SetWidgetTextColor(ButtonYes, ButtonActiveColor)

			if(MousePress(0))
			{
				ClearMousePress(0)
				return ResultOnYes
			}
		}
		else
			SetWidgetTextColor(ButtonYes, ButtonInactiveColor)

		if(InRectEx(ButtonNoLeft ,ButtonNoTop ,ButtonNoWidth ,ButtonNoHeight, MouseX, MouseY) == true)
		{
			SetWidgetTextColor(ButtonNo, ButtonActiveColor)

			if(MousePress(0))
			{
				ClearMousePress(0)
				return ResultOnNo
			}
		}
		else
			SetWidgetTextColor(ButtonNo, ButtonInactiveColor)

		return ""
	}

	void ConfirmDialog(string request, string ResultYes, string ResultNo)
	{
		ResultOnYes = ResultYes
		ResultOnNo = ResultNo
		
		int BackLeft = 272
		int BackTop = 268
		int BackWidth = 256
		int BackHeight = 64
		
		ButtonYesLeft = BackLeft
		ButtonYesTop = BackTop + 30
		ButtonYesWidth = 96
		ButtonYesHeight = 32
		ButtonNoLeft = BackLeft + 128 + 16
		ButtonNoTop = BackTop + 30
		ButtonNoWidth = 96
		ButtonNoHeight = 32
		
		ButtonInactiveColor = 0xff000000
		ButtonActiveColor = 0xffff0000

		background = CreateWidget(WTYPE_TEXT, BackLeft, BackTop, BackWidth, BackHeight, WF_CENTER, ARGB(255,0,0,0)/*ARGB(255,255,255,255)*/, 400)
		ButtonYes = CreateWidget(WTYPE_TEXT, ButtonYesLeft, ButtonYesTop, ButtonYesWidth, ButtonYesHeight, WF_CENTER, ARGB(255,0,0,0), 401)
		ButtonNo = CreateWidget(WTYPE_TEXT, ButtonNoLeft, ButtonNoTop, ButtonNoWidth, ButtonNoHeight, WF_CENTER, ARGB(255,0,0,0), 401)
		SetWidgetColor(background, 0xff56f7fd)
		SetWidgetTextColor(ButtonYes, ButtonInactiveColor)
		SetWidgetTextColor(ButtonNo, ButtonInactiveColor)
		SetWidgetTextOffset(background, 4, 10)
//		LoadWidgetImage(background, 0, "gfx/hud/ConfirmDialogBack.tga")
//		LoadWidgetImage(ButtonYes, 0, "gfx/hud/ConfirmDialogButton.tga")
//		LoadWidgetImage(ButtonNo, 0, "gfx/hud/ConfirmDialogButton.tga")
		SetWidgetTextEx(ButtonYes, 0, true, "#menuitem1_yes")
		SetWidgetTextEx(ButtonNo, 0, true, "#menuitem1_no")
		SetFont(background, MainFont)
		SetFont(ButtonYes, LargeFont)
		SetFont(ButtonNo, LargeFont)
		SetWidgetTextEx(background, 0, true, request)
		ShowWidget(background, true)
		ShowWidget(ButtonYes, true)
		ShowWidget(ButtonNo, true)
	}
	
	void ~ConfirmDialog()
	{
		if(ButtonYes)
		{
			DestroyWidget(ButtonYes)
			ButtonYes = NULL
		}

		if(ButtonNo)
		{
			DestroyWidget(ButtonNo)
			ButtonNo = NULL
		}

		if(background)
		{
			DestroyWidget(background)
			background = NULL
		}
	}
}
ConfirmDialog ConfDialog

const int NumCreditsLines = 150
widget CreditsSegments[NumCreditsLines]
widget CreditsBack
string CreditsTexts[NumCreditsLines]
int	 NumLines
int	 CreditsLeftOffsets[NumCreditsLines]
int	 CreditsTopOffsets[NumCreditsLines]
int	 CreditsWidths[NumCreditsLines]
int	 CreditsHeights[NumCreditsLines]
int	 CreditsWidth
float	 CreditsFullHeight
float	 CreditsCurrentPos
const int CreditsSpeed = 90
float ResolutionScale

void CreditsSpace(float space)
{
	CreditsFullHeight += space * ResolutionScale
}

bool CreditsTextLine(string text, int Font, int FontColor, int LeftOffset, int TopOffset, int Width)
{
	float Height
	
	switch(Font)
	{
	case SmallFont:
		Height = 14 * ResolutionScale
		break

	case MainFont:
		Height = 20 * ResolutionScale
		break
			
	case LargeFont:
		Height = 42 * ResolutionScale
		break
	}

	int fl = WF_SOURCEALPHA | WF_BLEND | WF_NOFILTER | WF_EXACTSIZE | WF_EXACTPOS
	
	if(LeftOffset == -1)
	{
		LeftOffset = 0
		fl |= WF_CENTER
	}
	
	widget w = NULL
	w = CreateWidget(WTYPE_TEXT, LeftOffset, g_iHeight + 10, Width, Height, fl, FontColor, 151)
	
	if(!w)
		return false
		
	SetFont(w, Font)
	SetWidgetTextEx(w, 0, 0, text)
	ShowWidget(w, false)
	CreditsSegments[NumLines] = w
	CreditsLeftOffsets[NumLines] = LeftOffset
	CreditsWidths[NumLines] = Width
	CreditsHeights[NumLines] = Height
	CreditsTopOffsets[NumLines++] = CreditsFullHeight + g_iHeight
	CreditsFullHeight += TopOffset + Height
	return true
}

bool CreditsBitmap(string BitmapPath, int BitmapColor, float LeftOffset, int TopOffset, int Width, int Height)
{
	LeftOffset = g_iWidth - Width
	LeftOffset *= 0.5
	
	widget w = NULL
	w = CreateWidget(WTYPE_IMAGE, LeftOffset , g_iHeight + 10, Width, Height, WF_SOURCEALPHA | WF_BLEND | WF_NOFILTER | WF_CENTER | WF_CENTER | WF_EXACTSIZE | WF_EXACTPOS, BitmapColor, 150)
	
	if(!w)
		return false
	
	LoadWidgetImage(w, 0, BitmapPath)
	ShowWidget(w, false)
	CreditsSegments[NumLines] = w
	CreditsLeftOffsets[NumLines] = LeftOffset
	CreditsWidths[NumLines] = Width
	CreditsHeights[NumLines] = Height
	CreditsTopOffsets[NumLines++] = CreditsFullHeight + g_iHeight
	CreditsFullHeight += TopOffset + Height
	return true
}

bool CreditsBackgroundColor(int color)
{
	CreditsBack = CreateWidget(WTYPE_IMAGE, 0, 0, 800, 600, WF_SOURCEALPHA | WF_BLEND | WF_NOFILTER | WF_VISIBLE, color, 149)
}


void UpdateCredits()
{
//	CreditsCurrentPos += ftime * 100
	CreditsCurrentPos += ftime * CreditsSpeed
	int actpos

	int xx = CreditsFullHeight + g_iHeight
	if(CreditsCurrentPos >= xx)
		CreditsCurrentPos = 0
	
	int yy
	float	tmp = CreditsCurrentPos + g_iHeight
		
	for(int n = 0; n < NumLines; n++)
	{
		yy = CreditsCurrentPos - CreditsHeights[n]
			
		if(CreditsTopOffsets[n] > yy && CreditsTopOffsets[n] < tmp)
		{
			actpos = CreditsTopOffsets[n] - CreditsCurrentPos

			SetWidgetPos(CreditsSegments[n], CreditsLeftOffsets[n], actpos, CreditsWidths[n], CreditsHeights[n])
			ShowWidget(CreditsSegments[n], true)
		}
		else
			ShowWidget(CreditsSegments[n], false)
	}
}

void DestroyCredits()
{
	for(int n = 0; n < NumLines; n++)
	{
		if(CreditsSegments[n])
			DestroyWidget(CreditsSegments[n])
			
		CreditsSegments[n] = NULL
	}
	NumLines = 0

	if(CreditsBack)
	{
		DestroyWidget(CreditsBack)
		CreditsBack = NULL
	}
}



string MenuPath = "gfx/menu/"

int LMouseX,LMouseY

string MenuActive

string MenuStack[16][2]
int MenuStackOff

int ListKeyInput

//=================================================================================
hsound HButtonSelect
hsound HButtonEnter
hsound HButtonSwitch
hsound HEscape
int	 SButtonSelect
int	 SButtonEnter
int	 SButtonSwitch
int	 SEscape

//-----------------------------------------------------------------
void PlayMenuSound(int type)
{
	switch(type)
	{
	case 1:
		SButtonSelect = PlaySound(NULL, HButtonSelect, SF_ONCE | SF_AMBIENT)
		return

	case 2:
		SButtonEnter = PlaySound(NULL, HButtonEnter, SF_ONCE | SF_AMBIENT)
		return
		
	case 3:
		SButtonSwitch = PlaySound(NULL, HButtonSwitch, SF_ONCE | SF_AMBIENT)
		return
		
	case 4:
		SEscape = PlaySound(NULL, HEscape, SF_ONCE | SF_AMBIENT)
		return
	}
}

//-----------------------------------------------------------------
const int GUI_TYPE_BUTTON = 1
const int GUI_TYPE_IMAGE = 2
const int GUI_TYPE_SLIDER = 3
const int GUI_TYPE_LIST_VALUE = 4
const int GUI_TYPE_CUSTOM = 5

const int GUI_ACTION_GETOPT = 1
const int GUI_ACTION_APPLYOPT = 2
const int GUI_ACTION_BACK = 4
const int GUI_ACTION_SAVE = 8
const int GUI_ACTION_MENU = 16
const int GUI_ACTION_CALL = 32

//=================================================================================
const int MAX_MENU_ITEM_VARIANTS = 64
const int MAX_MENU_ITEMS = 32

//current screen
widget	GUIWidgets[64]
int		GUINumWidgets

widget	GUIHintWidget
widget	GUIHintInfoWidget

int		GUIItemTypes[MAX_MENU_ITEMS]
int		GUIItemActions[MAX_MENU_ITEMS]
int		GUIItemFlags[MAX_MENU_ITEMS]
string	GUIItemActionParms[MAX_MENU_ITEMS]
string 	GUIItemIDs[MAX_MENU_ITEMS]
string	GUIItemHints[MAX_MENU_ITEMS]
string	GUIItemHintInfos[MAX_MENU_ITEMS]
string 	GUIItemStrings[MAX_MENU_ITEMS][MAX_MENU_ITEM_VARIANTS]
int		GUIItemRect[MAX_MENU_ITEMS][4]
int		GUIItemWidgets[MAX_MENU_ITEMS]
int		GUIItemValue[MAX_MENU_ITEMS][2] //id, index
int		GUINumItems
int		GUILastControl

int		GUIFont
int		GUIActiveColor = 0xffe5ffff
int		GUIInactiveColor = 0xff56fdff
int		GUIDisabledColor = 0x6634dbdd
int		GUIActiveColorImage = 0xffffffff
int		GUIInactiveColorImage = 0xaa78ffff
int		GUIDisabledColorImage = 0x6634dbdd

//-----------------------------------------------------------------
int GUIGetItemIndex(string id)
{
	for(int n = 0; n < GUINumItems; n++)
	{
		if(GUIItemIDs[n] == id)
			return n
	}

	return -1
}

//-----------------------------------------------------------------
void GUISetFont(int font)
{
	GUIFont = font
}


//-----------------------------------------------------------------
void GUISetColors(int acolor, int icolor, int dcolor)
{
	GUIActiveColor = acolor
	GUIInactiveColor = icolor
	GUIDisabledColor = dcolor
}

//-----------------------------------------------------------------
int GUIAddWidget(int type, int left, int top, int width, int height, int sort, int wflag, int color = 0xffffffff)
{
	//widget w = CreateWidget(type, left, top, width, height, wflag|WF_VISIBLE, 0xff7f7f7f, sort + 200)
	widget w = CreateWidget(type, left, top, width, height, wflag|WF_VISIBLE, color, sort+200)
	
	if(GUIFont)
		SetFont(w, GUIFont)
		
	GUIWidgets[GUINumWidgets++] = w
	return GUINumWidgets - 1
}

//-----------------------------------------------------------------
void GUIAction(int action, string parm = "")
{
	GUIItemActions[GUINumItems - 1] = action
	GUIItemActionParms[GUINumItems - 1] = parm
}

//-----------------------------------------------------------------
void GUIInitItem(int item, int type, int left, int top, int width, int height)
{
	GUIItemTypes[item] = type
	GUIItemRect[item][0] = left
	GUIItemRect[item][1] = top
	GUIItemRect[item][2] = left + width
	GUIItemRect[item][3] = top + height
	GUIItemActions[item] = 0
	GUIItemValue[item][0] = 0
	GUIItemValue[item][1] = 0
	GUIItemIDs[item] = ""
	GUIItemFlags[item] = 0
	GUIItemHints[item] = ""
	GUIItemHintInfos[item] = ""
	GUIItemStrings[item][0] = ""
}

//-----------------------------------------------------------------
void GUIListValue(int id, string text[], int left, int top, int width, int height, int sort, int wflag, int color = 0xffffffff)
{
	GUIInitItem(GUINumItems, GUI_TYPE_LIST_VALUE, left, top, width, height)

	int max = MAX_MENU_ITEM_VARIANTS - 1
	for(int n = 0; n < sizeof(text); n++)
	{
		if(n >= max)
			break
			
		GUIItemStrings[GUINumItems][n] = text[n]
	}
		
	GUIItemStrings[GUINumItems][n] = "" //terminator
	
	GUIItemWidgets[GUINumItems] = GUIAddWidget(WTYPE_TEXT, left, top, width, height, sort, wflag, color)
	SetWidgetTextEx(GUIWidgets[GUIItemWidgets[GUINumItems]], 0, true, text[0])

	//link with control
	GUIItemValue[GUINumItems][0] = id
	GUIItemValue[GUINumItems][1] = GUILastControl
	GUIItemValue[GUILastControl][0] = -1
	GUIItemValue[GUILastControl][1] = GUINumItems
	GUINumItems++	
}

//-----------------------------------------------------------------
void GUIHint(string hint)
{
	GUIItemHints[GUILastControl] = hint
}

void GUIHintInfo(string hint)
{
	GUIItemHintInfos[GUILastControl] = hint
}

//-----------------------------------------------------------------
void GUISlider(int id, int len, string image, int left, int top, int width, int height, int sort, int wflag)
{
	GUIInitItem(GUINumItems, GUI_TYPE_SLIDER, left, top, width, height)
	GUIItemStrings[GUINumItems][0] = itoa(len) //achjo :-)
	GUIItemWidgets[GUINumItems] = GUIAddWidget(WTYPE_IMAGE, left, top, width, height, sort, wflag, 0xffffffff) // hack - fullbright sliders
	LoadWidgetImage(GUIWidgets[GUIItemWidgets[GUINumItems]], 0, MenuPath + image)
	
	//link with control
	GUIItemValue[GUINumItems][0] = id
	GUIItemValue[GUINumItems][1] = GUILastControl
	GUIItemValue[GUILastControl][0] = -1
	GUIItemValue[GUILastControl][1] = GUINumItems	
	GUINumItems++	
}

//-----------------------------------------------------------------
void GUICustom(string id, int left, int top, int width, int height)
{
	GUIInitItem(GUINumItems, GUI_TYPE_CUSTOM, left, top, width, height)
	GUIItemIDs[GUINumItems] = id
	GUINumItems++
}

//-----------------------------------------------------------------
void GUIImage(string image, int left, int top, int width, int height, int sort, int wflag, int color = 0xffffffff)
{
	int w = GUIAddWidget(WTYPE_IMAGE, left, top, width, height, sort, wflag, color)

	if(strstr(image, "gfx/") == -1)
		image = MenuPath + image	

	LoadWidgetImage(GUIWidgets[w], 0, image)
	
	SetWidgetColor(GUIWidgets[w], color)
}

//-----------------------------------------------------------------
void GUITitle(string text, int left, int top, int width, int height, int sort, int wflag, int color = 0xffffffff, int font = -1)
{
	int w = GUIAddWidget(WTYPE_TEXT, left, top, width, height, sort, wflag, color)
	SetWidgetText(GUIWidgets[w], 1, "")
	SetWidgetTextEx(GUIWidgets[w], 0, true, text)
	SetWidgetTextColor(GUIWidgets[w], color)
	
	if(font != -1)
		SetFont(GUIWidgets[w], font)
}

//-----------------------------------------------------------------
void GUIImageButton(string id, string image, int left, int top, int width, int height, int sort, int wflag, int color = 0xffffffff)
{
	GUIInitItem(GUINumItems, GUI_TYPE_IMAGE, left, top, width, height)
	GUIItemIDs[GUINumItems] = id
	GUIItemWidgets[GUINumItems] = GUIAddWidget(WTYPE_IMAGE, left, top, width, height, sort, wflag, color)
	LoadWidgetImage(GUIWidgets[GUIItemWidgets[GUINumItems]], 0, MenuPath + image)	
	GUILastControl = GUINumItems
	GUINumItems++
}

//-----------------------------------------------------------------
void GUIButton(string id, string text, int left, int top, int width, int height, int sort, int wflag, int color = 0xffffffff)
{
	GUIInitItem(GUINumItems, GUI_TYPE_BUTTON, left, top, width, height)
	GUIItemIDs[GUINumItems] = id
	GUIItemStrings[GUINumItems][0] = text
	GUIItemWidgets[GUINumItems] = GUIAddWidget(WTYPE_TEXT, left, top, width, height, sort, wflag|WF_SOURCEALPHA | WF_BLEND, color)
	SetWidgetTextEx(GUIWidgets[GUIItemWidgets[GUINumItems]], 0, true, text)
	GUILastControl = GUINumItems
	GUINumItems++
}

//-----------------------------------------------------------------
void GUICleanup()
{
	for(int n = 0; n < GUINumWidgets; n++)
		DestroyWidget(GUIWidgets[n])

	GUINumWidgets = 0
	GUINumItems = 0
}

//-----------------------------------------------------------------
bool GUIIsControlItem(int index)
{
	if(GUIItemTypes[index] == GUI_TYPE_BUTTON || GUIItemTypes[index] == GUI_TYPE_IMAGE)
		return true
	
	return false
}

//-----------------------------------------------------------------
void GUIDisableItem(string item)
{
	int index = GUIGetItemIndex(item)
	GUIItemFlags[index] = GUIItemFlags[index] | 1

	if(GUIIsControlItem(index))
	{
		int val = GUIItemValue[index][1]
		if(val)
		{
			GUIItemFlags[val] = GUIItemFlags[val] | 1
			if(GUIItemTypes[val] == GUI_TYPE_IMAGE)
				SetWidgetColor(GUIWidgets[GUIItemWidgets[val]], GUIDisabledColor)
			else
				SetWidgetTextColor(GUIWidgets[GUIItemWidgets[val]], GUIDisabledColor)
		}
	}

	if(GUIItemTypes[index] == GUI_TYPE_IMAGE)
		SetWidgetColor(GUIWidgets[GUIItemWidgets[index]], GUIDisabledColor)
	else
		SetWidgetTextColor(GUIWidgets[GUIItemWidgets[index]], GUIDisabledColor)
}

//-----------------------------------------------------------------
void GUIEnableItem(string item)
{
	int index = GUIGetItemIndex(item)
	GUIItemFlags[index] = GUIItemFlags[index] & 0xfffffffe

	if(GUIIsControlItem(index))
	{
		int val = GUIItemValue[index][1]
		if(val)
		{
			GUIItemFlags[val] = GUIItemFlags[val] & 0xfffffffe
			if(GUIItemTypes[val] == GUI_TYPE_IMAGE)
				SetWidgetColor(GUIWidgets[GUIItemWidgets[val]], GUIInactiveColor)
			else
				SetWidgetTextColor(GUIWidgets[GUIItemWidgets[val]], GUIInactiveColor)
		}
	}

	if(GUIItemTypes[index] == GUI_TYPE_IMAGE)
		SetWidgetColor(GUIWidgets[GUIItemWidgets[index]], GUIInactiveColor)
	else
		SetWidgetTextColor(GUIWidgets[GUIItemWidgets[index]], GUIInactiveColor)
}

//-----------------------------------------------------------------
void GUIActivateItem(string item, int state)
{
	int index = GUIGetItemIndex(item)
	
	widget w = GUIWidgets[GUIItemWidgets[index]]
	
	if(GUIItemFlags[index] & 1)
		state = -1

	if(GUIItemTypes[index] == GUI_TYPE_IMAGE)
	{
		switch(state)
		{
		case 0:
			SetWidgetColor(w, GUIInactiveColorImage)
			break
			
		case 1:
			SetWidgetColor(w, GUIActiveColorImage)
			break
			
		case -1:
			SetWidgetColor(w, GUIDisabledColorImage)
			break
		}	
	}
	else
	{
		switch(state)
		{
		case 0:
			SetWidgetTextColor(w, GUIInactiveColor)
			break
			
		case 1:
			SetWidgetTextColor(w, GUIActiveColor)
			break
			
		case -1:
			SetWidgetTextColor(w, GUIDisabledColor)
			break
		}	
	}

	if(!GUIHintWidget)
	{
		GUIHintWidget = CreateWidget(WTYPE_TEXT, 430, 140, 340, 32, WF_VISIBLE|WF_SOURCEALPHA|WF_BLEND, 0xff0e2b2d, 202)
		SetFont(GUIHintWidget, MainFont)
	}

	if(!GUIHintInfoWidget)
	{
		GUIHintInfoWidget = CreateWidget(WTYPE_TEXT, 430, 160, 350, 50, WF_VISIBLE|WF_SOURCEALPHA|WF_BLEND, GUIInactiveColor, 202)
		SetWidgetTextSpacing(GUIHintInfoWidget, 0, -6)
		SetFont(GUIHintInfoWidget, MainFont)
	}
   	
	if(GUIItemHints[index] && state == 1)
	{
		ShowWidget(GUIHintWidget, true)
		SetWidgetTextEx(GUIHintWidget, 0, true, GUIItemHints[index])
   }
	else
	{
		ShowWidget(GUIHintWidget, false)
	}

	if(GUIItemHintInfos[index] && state == 1)
	{
		ShowWidget(GUIHintInfoWidget, true)
		SetWidgetText(GUIHintInfoWidget, 1, "")
		SetWidgetText(GUIHintInfoWidget, 2, "")
		SetWidgetText(GUIHintInfoWidget, 3, "")
		SetWidgetTextEx(GUIHintInfoWidget, 0, true, GUIItemHintInfos[index])
	}
	else
	{
		ShowWidget(GUIHintInfoWidget, false)
	}
}

//-----------------------------------------------------------------
int GUICompareMouse(int item)
{
	if(MouseX < GUIItemRect[item][0] || MouseX > GUIItemRect[item][2] || MouseY < GUIItemRect[item][1] || MouseY > GUIItemRect[item][3])
		return -1

	return MouseX - GUIItemRect[item][0]
}

//-----------------------------------------------------------------
int GUICheckMouse()
{
	int btn = GUIGetItemIndex(ActButton)

	for(int n = 0; n < GUINumItems; n++)
	{
		int nbtn = n
		
		if(GUIItemFlags[nbtn] & 1)
			continue

		int v = GUICompareMouse(nbtn)

		if(v >= 0)
		{
			if(!GUIIsControlItem(nbtn))
			{
				nbtn = GUIItemValue[nbtn][1]
			}
			
			if(btn != nbtn)
			{
				if(btn >= 0)
					GUIActivateItem(GUIItemIDs[btn], 0)

				GUIActivateItem(GUIItemIDs[nbtn], 1)
				ActButton = GUIItemIDs[nbtn]

				PlayMenuSound(1)
			}
			return nbtn
		}
	}

	return -1
}

//-----------------------------------------------------------------
void SetMenuValue(int item)
{
	switch(GUIItemTypes[item])
	{
	case GUI_TYPE_SLIDER:
		int width = GUIItemRect[item][2] - GUIItemRect[item][0]
		int maxv = atoi(GUIItemStrings[item][0])
		int w = ItemValues[GUIItemValue[item][0]] *  width / maxv
		SetWidgetPos(GUIWidgets[GUIItemWidgets[item]], GUIItemRect[item][0], GUIItemRect[item][1], w, GUIItemRect[item][3] - GUIItemRect[item][1])
	break

	case GUI_TYPE_LIST_VALUE:
		string str = GUIItemStrings[item][ItemValues[GUIItemValue[item][0]]]
		SetWidgetTextEx(GUIWidgets[GUIItemWidgets[item]], 0, true, str)
	break
	}
}

//-----------------------------------------------------------------
void GUISetMenuItems()
{
	for(int n = 0; n < GUINumItems; n++)
	{
		if(GUIIsControlItem(n))
			continue
		
		SetMenuValue(n)
	}
}

//-----------------------------------------------------------------
int GUIMenuItemVariants(int item)
{
	for(int c = 0; c < MAX_MENU_ITEM_VARIANTS; c++)
	{
		if(!GUIItemStrings[item][c])
			return c
	}

	return 0
}

//-----------------------------------------------------------------
void SwitchMenuItem(string item, int dir)
{
	int n = GUIGetItemIndex(item)
	
	//custom prvek
	if(n == -1)
		return
		
	int maxv, v, val, id

	if(GUIIsControlItem(n)) //search for value item
	{
		val = GUIItemValue[n][1]
		if(val == 0)
			return
	}
	else
	{
		val = n
	}
	
	id = GUIItemValue[val][0]
	
	v = ItemValues[id]

	switch(GUIItemTypes[val])
	{
		case GUI_TYPE_LIST_VALUE:
			if(dir == 0)
				dir = 1

			maxv = GUIMenuItemVariants(val)
			v += dir
			if(v >= maxv)
				v = 0
			if(v < 0)
				v = maxv - 1

			ItemValues[id] = v
			SetMenuValue(val)
			ClearMousePress(0)
			PlayMenuSound(3)
		break

		case GUI_TYPE_SLIDER:
			maxv = atoi(GUIItemStrings[val][0])
			if(MousePress(0))
			{
				v = GUICompareMouse(val)

				if(v >= 0)
				{
					int width = GUIItemRect[val][2] - GUIItemRect[val][0]
					
					v = v * maxv / width
					clamp v<0, maxv>
					
					ItemValues[id] = v
					SetMenuValue(val)
				}
			}
			else
			{
				v = ItemValues[id]
				v += dir * maxv / 10
				clamp v<0, maxv>
				ItemValues[id] = v
				SetMenuValue(val)
			}
		break
	}
}


//-----------------------------------------------------------------
bool GUINextItem(int hdir = 0, int vdir = 1)
{
	if(ActButton == "")
	{
		//je to na nejakem custom prvku
		return false
	}
	
	string pbutton = ActButton

	int pindex = GUIGetItemIndex(ActButton)
	int bestindex = -1
	int bestdist = 800
	
	for(int index = 0; index < GUINumItems; index++)
	{
		if(GUIItemFlags[index] & 1)
			continue

		if(!GUIIsControlItem(index))
			continue
		
		int dist
		
		if(hdir > 0)
		{
			dist = GUIItemRect[index][0] - GUIItemRect[pindex][2]
			
			if(dist < -10)
				continue
			
			dist += abs(GUIItemRect[index][1] - GUIItemRect[pindex][1])
			clamp dist<0, 800>
		}
		
		if(hdir < 0)
		{
			dist = GUIItemRect[pindex][0] - GUIItemRect[index][2]
			
			if(dist < -10)
				continue
			
			dist += abs(GUIItemRect[index][1] - GUIItemRect[pindex][1])
			clamp dist<0, 800>
		}
		
		if(vdir > 0)
		{
			dist = GUIItemRect[index][1] - GUIItemRect[pindex][3]
			
			if(dist < -10)
				continue
			
			dist += abs(GUIItemRect[index][0] - GUIItemRect[pindex][0])
			clamp dist<0, 800>
		}
		
		if(vdir < 0)
		{
			dist = GUIItemRect[pindex][1] - GUIItemRect[index][3]
			
			if(dist < -10)
				continue
			
			dist += abs(GUIItemRect[index][0] - GUIItemRect[pindex][0])
			clamp dist<0, 800>
		}
		
		if(dist < bestdist)
		{
			bestindex = index
			bestdist = dist
		}
	}
	
	if(bestindex == -1 || bestindex == pindex)
		return false

	ActButton = GUIItemIDs[bestindex]
	
	GUIActivateItem(pbutton, 0)
	GUIActivateItem(ActButton, 1)
	PlayMenuSound(1)
	return true
}

//-----------------------------------------------------------------
void InitMenu(bool music)
{
	MenuStackOff = 0
	ShowWidget(WMousePointer,true)

	if(music)
	{
		MenuMusic = GetSoundEx("MenuMusic", 1, SF_STREAM)
		//MenuMusicID = PlaySound(NULL, MenuMusic, SF_AMBIENT|SF_MUSIC)    <-- moved to open menu (due music delay on start)
		//SetSoundVolume(MenuMusicID, 0)
	}

	HButtonSelect = GetSoundEx("MenuSelect", 1, 0)
	HButtonEnter = GetSoundEx("MenuEnter", 1, 0)
	HButtonSwitch = GetSoundEx("MenuSwitch", 1, 0)
	HEscape = GetSoundEx("MenuEscape", 1, 0)
}

//-----------------------------------------------------------------
void FreeMenu()
{
	GUICleanup()
	MenuStackOff = 0
	MenuActive = ""
	ActButton = ""

	if(GUIHintWidget)
	{
		DestroyWidget(GUIHintWidget)
		GUIHintWidget = NULL
	}

	if(GUIHintInfoWidget)
	{
		DestroyWidget(GUIHintInfoWidget)
		GUIHintInfoWidget = NULL
	}

	if(MenuMusicID)
	{
		EndSound(MenuMusicID)
		ReleaseSound(MenuMusic)
	}

	ShowWidget(WMousePointer,false)
}


widget ListboxItemWidgets[64]

string ListboxPrimaryValue[64]
string ListboxSecondaryValue[64]
widget ListboxPrimaryWidget[64]
widget ListboxSecondaryWidget[64]
widget ListboxBack[64]
widget WClipArea, WScrollbarArea
widget Scrollbar[1]
widget SelectedLine[1]
widget AreaTop[1]
widget AreaBottom[1]


int ScrollbarArea[4] = {630, 280, 650, 401}
int ScrollbarRect[4] = {632, 280, 650, 401}

int ScrollbarWidth = 16
int ScrollbarHeight = 64
int LineHeight = 20
int TextAlignOffset = 10
int MouseWheelSpeed = 2

int ListboxArea[4] = {85, 280, 610, 400}
int ListboxCol0Pos[2] = {85, 250}
int ListboxCol1Pos[2] = {370, 125}
int ListboxCol2Pos[2] = {500, 125}

int ItemOffsetV[64]

//#define XBOX

#ifdef XBOX
int ListboxItemsNum = 19
#else
int ListboxItemsNum = 29
#endif

//-----------------------------------------------------------------
void SetWidTexts()
{
	int n = 0
	while(n < ListboxItemsNum)
	{
		int k1 = g_KeyDef1[n]
		int k2 = g_KeyDef2[n]

		if(ListboxPrimaryWidget[n] != NULL)
		{
			if(k1 < 0)
				SetWidgetTextEx(ListboxPrimaryWidget[n] , 0, true, OtherNames[-k1])
			else
				SetWidgetTextEx(ListboxPrimaryWidget[n] , 0, true, KeyNames[k1])

			if(k2 < 0)
				SetWidgetTextEx(ListboxSecondaryWidget[n] , 0, true, OtherNames[-k2])
			else
				SetWidgetTextEx(ListboxSecondaryWidget[n] , 0, true, KeyNames[k2])
		}
		n++
	}
}

//-----------------------------------------------------------------
void LoadListbox(string LItemNames[], widget LItemWidgets[], string LPrimaryValue[], string LSecondaryValue[], widget LPrimaryWidget[], widget LSecondaryWidget[], widget LboxBack[], widget LScrollbar[], widget LSelectedLine[], widget LAreaTop[], widget LAreaBottom[], int LboxPos[], int LboxCol0Pos[], int LboxCol1Pos[], int LboxCol2Pos[], int IOffsetV[], int LboxItemsNum)
{
	int n = 0
	int wnum = ListboxItemsNum
	ListKeyInput = 0

	int temp1 = wnum * LineHeight
	int temp2 = ListboxArea[3] - ListboxArea[1] + 1
	int temp3 = ScrollbarArea[3] - ScrollbarArea[1]
	float temp4 = temp2 / temp1
	temp4 = temp3 * temp4
	//ScrollbarHeight = temp4
	ScrollbarRect[3] = ScrollbarRect[1] + ScrollbarHeight

	WClipArea = CreateWidget(WTYPE_IMAGE, ListboxArea[0], ListboxArea[1] , 550, 120, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, 0x00ffffff, 300)

	LScrollbar[0] = CreateWidget(WTYPE_IMAGE, ScrollbarRect[0], ScrollbarRect[1], ScrollbarWidth, ScrollbarHeight, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP, ARGB(255,130,255,255), 302)
//	LoadWidgetImage(LScrollbar[0] , 0, MenuPath + "controls_scrollbar")

	WScrollbarArea = CreateWidget(WTYPE_IMAGE, ScrollbarArea[0] + 2, ScrollbarArea[1], ScrollbarWidth, 120, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, ARGB(255,30,155,155), 300)

	LSelectedLine[0] = CreateWidgetEx(WClipArea, WTYPE_IMAGE, ListboxArea[0], ScrollbarRect[1], 550, 24, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, 0xffffffff, 300)
	LoadWidgetImage(LSelectedLine[0] , 0, MenuPath + "controls_line_1")
	ShowWidget(LSelectedLine[0], 0)

	LAreaTop[0] = CreateWidget(WTYPE_IMAGE, ListboxArea[0], ListboxArea[1] - 8, 563, 7, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, 0xffffffff, 303)
	LoadWidgetImage(LAreaTop[0] , 0, MenuPath + "controls_clip_up")
	ShowWidget(LAreaTop[0], 1)

	LAreaBottom[0] = CreateWidget(WTYPE_IMAGE, ListboxArea[0], ListboxArea[1] + 121 , 563, 7, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, 0xffffffff, 303)
	LoadWidgetImage(LAreaBottom[0] , 0, MenuPath + "controls_clip_down")
	ShowWidget(LAreaBottom[0], 1)
	
//	GUIImage("controls_scrollbararea", 630, 280, 20, 128, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// scrollbar range

	while(n < wnum)
	{
		string wname

		wname = LItemNames[n]
		if(wname == "")
			return

		IOffsetV[n] = n * LineHeight
		int widposV = LboxCol0Pos[1] + IOffsetV[n]

		LboxBack[n] = CreateWidgetEx(WClipArea, WTYPE_IMAGE, LboxPos[0], widposV, 550, 24, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, 0xffffffff, 299)
		LoadWidgetImage(LboxBack[n] , 0, MenuPath + "controls_line_0")

		int WWidth = 800 - (LboxCol0Pos[0] + TextAlignOffset)
		LItemWidgets[n] = CreateWidgetEx(WClipArea, WTYPE_TEXT, LboxCol0Pos[0] + TextAlignOffset, widposV, WWidth, 10, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, GUIInactiveColor, 301)
		SetWidgetTextEx(LItemWidgets[n] , 0, true, LItemNames[n])
		SetFont(LItemWidgets[n] , MainFont)
		ShowWidget(LItemWidgets[n] , 0)

		LPrimaryWidget[n] = CreateWidgetEx(WClipArea, WTYPE_TEXT, LboxCol1Pos[0] + TextAlignOffset, widposV, 135, 25, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_NOWRAP | WF_NOFILTER, GUIInactiveColor, 301)
		ListboxPrimaryWidget[n] = ListboxPrimaryWidget[n]
		int tmpkey = g_KeyDef1_temp[n]
		if(tmpkey > 0)
			SetWidgetTextEx(LPrimaryWidget[n] , 0, true, KeyNames[tmpkey])
		else
			SetWidgetTextEx(LPrimaryWidget[n] , 0, true, OtherNames[-tmpkey])
		SetFont(LPrimaryWidget[n] , MainFont)
		if(n > ListboxItemsNum)
			ShowWidget(LPrimaryWidget[n] , 0)

		LSecondaryWidget[n] = CreateWidgetEx(WClipArea, WTYPE_TEXT, LboxCol2Pos[0] + TextAlignOffset, widposV, 135, 25, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND, GUIInactiveColor, 301)
		tmpkey = g_KeyDef2_temp[n]
		if(tmpkey > 0)
			SetWidgetTextEx(LSecondaryWidget[n] , 0, true, KeyNames[tmpkey])
		else
			SetWidgetTextEx(LSecondaryWidget[n] , 0, true, OtherNames[-tmpkey])
		SetFont(LSecondaryWidget[n] , MainFont)
		ShowWidget(LSecondaryWidget[n] , 0)

		n++
		LboxItemsNum = n
	}
	SetWidTexts()
}

//-----------------------------------------------------------------
void FreeListbox(widget w1[], widget w2[], widget w3[], widget w4[], widget w5[], widget w6[], widget w7[], widget w8[])
{
	int n
	int Wnum
	widget w
	ListKeyInput = 0

	n = 0
	Wnum = sizeof(w1)
	while(n < Wnum)
	{
		w = w1[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w1[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w2)
	while(n < Wnum)
	{
		w = w2[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w2[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w3)
	while(n < Wnum)
	{
		w = w3[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w3[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w4)
	while(n < Wnum)
	{
		w = w4[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w4[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w5)
	while(n < Wnum)
	{
		w = w5[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w5[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w6)
	while(n < Wnum)
	{
		w = w6[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w6[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w7)
	while(n < Wnum)
	{
		w7[n] = w7[n]
		w = w7[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w7[n] = NULL
		}
		n++
	}

	n = 0
	Wnum = sizeof(w8)
	while(n < Wnum)
	{
		w = w8[n]
		if(w != NULL)
		{
			DestroyWidget(w)
			w8[n] = NULL
		}
		n++
	}

	if(WScrollbarArea)
	{
		DestroyWidget(WScrollbarArea)
		WScrollbarArea = NULL	
	}

	if(WClipArea)
	{
		DestroyWidget(WClipArea)
		WClipArea = NULL
	}
}

//events flags
int		LB_evLineClick =		 1
int		LB_evLineDblClick =	 2
int		LB_evCol0Click =		 4
int		LB_evCol1Click =		 8
int		LB_evCol2Click =		 16
int		LB_evCol3Click =		 32


int		LB_Events
bool		g_LB_Update		//povoluje Update
int 		LB_ColNum
int 		LB_LinesNum
int 		LB_MaxColNum = 4
int 		LB_MaxLinesNum = 64

int		LB_Left
int		LB_Top
int		LB_Width
int		LB_Height
int		LB_Right
int		LB_Bottom

int		LB_LineHeight
int		LB_UsedHeight

int		LB_SliderWidth
int		LB_SliderHeight
int		LB_SliderLeft
int		LB_SliderTop

int		LB_SelectorWidth
int		LB_SelectorHeight
int		LB_SelectorLeft
int		LB_SelectorTop

int		LB_ColRight[4]
int		LB_ColLeft[4]

int		LB_LineVoffsets[64]
string	LB_Texts[4][64]
widget	LB_WidgetTexts[4][64]	//4 stlpce, 64 riadkov
widget	LB_WidgetImages[64]
widget	LB_Slider
widget	LB_Selector
widget	LB_Back, LB_ClipArea
widget	LB_CustomImages[8]
int		LB_CustomImagesNum
string	LB_LineBitmap

int		LB_SliderPickOffset
bool		LB_SliderPicked
float		LB_SliderActPosRate
int		LB_SelDblClick
int		LB_SelLine
int		LB_OldClicked
int		LB_LinesYOffset

//-----------------------------------------------------------------
void LB_Open(int ColNum, int height)
{
	LB_ColNum = ColNum
	LB_Height = height
	LB_Bottom = LB_Top + LB_Height
	LB_OldClicked = -1
	LB_LinesNum = 0
	LB_UsedHeight = 0
	LB_CustomImagesNum = 0
}

//-----------------------------------------------------------------
void LB_SetListBoxPos(int left, int top)
{
	LB_Left = left
	LB_Top = top
	LB_Right = LB_Left + LB_Width
	LB_Bottom = LB_Top + LB_Height
}

//-----------------------------------------------------------------
void LB_AddLineImage(string BitmapPath, int width, int height)
{
	LB_LineBitmap = BitmapPath
	LB_Width = width
	LB_Right = LB_Left + LB_Width
	LB_LineHeight = height
}

//-----------------------------------------------------------------
void LB_AddCustomImage(string BitmapPath, int left, int top, int width, int height, int color, int SortOff)
{
	LB_CustomImages[LB_CustomImagesNum] = CreateWidget(WTYPE_IMAGE, left, top, width, height, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND/* | WF_NOWRAP*/, color, 310 + SortOff)
	
	if(BitmapPath)
		LoadWidgetImage(LB_CustomImages[LB_CustomImagesNum], 0, BitmapPath)

	ShowWidget(LB_CustomImages[LB_CustomImagesNum], 1)
	LB_CustomImagesNum++
}

//-----------------------------------------------------------------
void LB_SetColWidth(int Col, int width)
{
	if(Col == 0)
	{
		LB_ColLeft[Col] = LB_Left
		LB_ColRight[Col] = LB_Left + width
		return
	}

	LB_ColLeft[Col] = LB_ColRight[Col - 1] + 1
	LB_ColRight[Col] = LB_ColLeft[Col] + width
}

//-----------------------------------------------------------------
void LB_AddSliderImage(string BitmapPath, int width, int height)
{
	LB_SliderWidth = width
	LB_SliderHeight = height

	LB_SliderLeft = LB_Right + 1

	if(LB_SliderTop == 0)	//FIXME pri viacerych listboxoch to je neziaduce
		LB_SliderTop = LB_Top

	LB_Slider = CreateWidget(WTYPE_IMAGE, LB_SliderLeft, LB_SliderTop, LB_SliderWidth, LB_SliderHeight, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND | WF_STRETCH/* | WF_NOWRAP*/, ARGB(255,130,255,255), 305)
//	LoadWidgetImage(LB_Slider , 0, BitmapPath)
	ShowWidget(LB_Slider, 1)
}

//-----------------------------------------------------------------
void LB_AddSelectorImage(string BitmapPath, int width, int height)
{
	LB_SelectorWidth = width
	LB_SelectorHeight = height

	LB_SelectorLeft = LB_Left
	LB_SelectorTop = LB_Top

	LB_Selector = CreateWidgetEx(LB_ClipArea, WTYPE_IMAGE, LB_SelectorLeft, LB_SelectorTop, LB_SelectorWidth, LB_SelectorHeight, WF_VISIBLE | WF_SOURCEALPHA | WF_ADDITIVE/* | WF_NOWRAP*/, ARGB(255,0,105,105), 302)
	
	if(BitmapPath)
		LoadWidgetImage(LB_Selector , 0, BitmapPath)

	ShowWidget(LB_Selector, 1)
}

//-----------------------------------------------------------------
void LB_AddBackgroundImage(string BitmapPath)
{
	if(!BitmapPath)
		return

	LB_ClipArea = CreateWidget(WTYPE_IMAGE, LB_Left, LB_Top, LB_Width, LB_Height, WF_VISIBLE|WF_SOURCEALPHA|WF_BLEND/* | WF_NOWRAP*/, ARGB(160,28,48,51), 300)
	LB_Back = CreateWidget(WTYPE_IMAGE, LB_Left, LB_Top, LB_Width, LB_Height, WF_VISIBLE|WF_SOURCEALPHA|WF_BLEND/* | WF_NOWRAP*/, 0x00ffffff, 301)
	LoadWidgetImage(LB_Back , 0, BitmapPath)
	ShowWidget(LB_Back, 1)
}

//-----------------------------------------------------------------
int LB_AddLine()
{
	int Temp = LB_Top + (LB_LinesNum * LB_LineHeight)
	LB_LineVoffsets[LB_LinesNum] = Temp - LB_Top

	if(LB_LineBitmap)
	{
		LB_WidgetImages[LB_LinesNum] = CreateWidgetEx(LB_ClipArea, WTYPE_IMAGE, LB_Left, Temp, LB_Width, LB_LineHeight, WF_VISIBLE | WF_SOURCEALPHA | WF_BLEND/* | WF_NOWRAP*/, 0xffffffff, 302)
		LoadWidgetImage(LB_WidgetImages[LB_LinesNum] , 0, LB_LineBitmap)
		ShowWidget(LB_WidgetImages[LB_LinesNum], 1)
	}

	for(int n = 0; n < LB_ColNum; n++)
	{
		LB_WidgetTexts[n][LB_LinesNum] = CreateWidgetEx(LB_ClipArea, WTYPE_TEXT, LB_ColLeft[n] + 10, LB_Top + (LB_LinesNum * LB_LineHeight), LB_ColRight[n] - LB_ColLeft[n], LB_LineHeight, WF_VISIBLE | WF_SOURCEALPHA | WF_ADDITIVE, 0xffffffff, 304)
		SetWidgetText(LB_WidgetTexts[n][LB_LinesNum] , 0, "no text")
		LB_Texts[n][LB_LinesNum] = "no text"
		SetFont(LB_WidgetTexts[n][LB_LinesNum] , MainFont)
		ShowWidget(LB_WidgetTexts[n][LB_LinesNum] , 1)
	}

	LB_UsedHeight += LB_LineHeight
	return LB_LinesNum++
}

//-----------------------------------------------------------------
void LB_SetLineText(int Col, int Line, string text, bool UnicodeMode)
{
	if(UnicodeMode)
		SetWidgetTextEx(LB_WidgetTexts[Col][Line], 0, true, text)
	else
		SetWidgetText(LB_WidgetTexts[Col][Line], 0, text)
}

//-----------------------------------------------------------------
void LB_SetLineColor(int Col, int Line, int color)
{
	SetWidgetTextColor(LB_WidgetTexts[Col][Line], color)
}

//-----------------------------------------------------------------
void LB_EnableEvents(bool stat)
{
	g_LB_Update = stat
}

//-----------------------------------------------------------------
string LB_GetLineText(int Col, int Line)
{
	return LB_Texts[Col][Line]
}

//-----------------------------------------------------------------
void LB_RemoveLine(int Line)
{
	LB_LinesNum--
}

//-----------------------------------------------------------------
void LB_SelectLine(int line)
{
	if(line >= 0 && line < LB_LinesNum)
		LB_SelLine = line
}

//-----------------------------------------------------------------
int LB_GetSelectedLine()
{
	return LB_SelLine
}

//-----------------------------------------------------------------
int LB_GetEvents()
{
	return LB_Events
}

//-----------------------------------------------------------------
int LB_Update()
{
	int events = 0
	int ClickedLine = -1

	float f1
	float f2
	float f3
	int n
	int VisibleHeight

	if(LB_UsedHeight < LB_Height)
	{
		LB_SliderHeight = LB_Height
		VisibleHeight = LB_UsedHeight
	}
	else
	{
		f1 = LB_Height
		f2 = LB_UsedHeight
		f3 = f1 / f2
		f3 = f3 * f1
		LB_SliderHeight = f3
		VisibleHeight = LB_Height
	}

	float MaxMoveOff = LB_UsedHeight - LB_Height
	f1 = LB_Height - LB_SliderHeight				//draha po ktorej slider chodi
	f2 = LB_SliderTop - LB_Top						//ako daleko je od zaciatku

	if(f1 == 0)
		LB_SliderActPosRate = 0
	else
		LB_SliderActPosRate = f2 / f1					//percentualna cast

	f3 = LB_SliderActPosRate * MaxMoveOff		//o tolko sa musia lajny posunut nad LB_Top
	LB_LinesYOffset = f3

	int LineActPosTop
	int TopVisEnd
	int BottomVisEnd
	int Vis

	for(n = 0; n < LB_LinesNum; n++)				//posunieme lajny
	{
		LineActPosTop = LB_Top + LB_LineVoffsets[n] - LB_LinesYOffset

		TopVisEnd = LB_Top - LB_LineHeight
		BottomVisEnd = LB_Bottom

		if(LineActPosTop < TopVisEnd || LineActPosTop > BottomVisEnd)
			Vis = 0
		else
			Vis = 1

		for(int k = 0; k < LB_ColNum; k++)
		{
			SetWidgetPos(LB_WidgetTexts[k][n], LB_ColLeft[k] + 10 - LB_Left, LineActPosTop - LB_Top, LB_ColRight[k] - LB_ColLeft[k], LB_LineHeight)
			ShowWidget(LB_WidgetTexts[k][n], Vis)
		}

		if(LB_WidgetImages[n])
		{
			SetWidgetPos(LB_WidgetImages[n], LB_Left - LB_Left, LineActPosTop - LB_Top, LB_Width, LB_LineHeight)
			ShowWidget(LB_WidgetImages[n], Vis)
		}

		if(n == LB_SelLine)
		{
			SetWidgetPos(LB_Selector, LB_Left - LB_Left, LB_Top - LB_LinesYOffset + LB_LineVoffsets[LB_SelLine] - LB_Top , LB_Width, LB_SelectorHeight)
			ShowWidget(LB_Selector, Vis)
		}
	}

	if(LB_SliderPicked == 0)	//nemame uchyteny slider
	{
		if(InRect(LB_SliderLeft - 4, LB_SliderLeft + LB_SliderWidth, LB_SliderTop, LB_SliderTop + LB_SliderHeight, MouseX, MouseY))
		{
			if(MousePress(0))			//klikli sme na slider
			{
				LB_SliderPickOffset = MouseY - LB_SliderTop
				LB_SliderPicked = true
			}
		}

		if(InRect(LB_Left, LB_Right, LB_Top, LB_Bottom - 1, MouseX, MouseY))	//sme nad polami listboxu
		{
			if(MouseWheel && LB_SliderPicked == false)
			{
				f1 = LB_Height
				f2 = LB_SliderHeight
				f3 = f2 / f1

				float ff = MouseWheel
				ff = ff * (f3 * (float)10)
				int ii = ff
				LB_SliderTop -= ii

				if(LB_SliderTop < LB_Top)
					LB_SliderTop = LB_Top

				int tmp2 = LB_Bottom - LB_SliderHeight

				if(LB_SliderTop > tmp2)
					LB_SliderTop = tmp2
			}

			int kk = LB_Top - f3		//pozicia prvej lajny
			int nn = MouseY - kk		//vyskovy rozdiel od mysi
			nn /= LB_LineHeight		//cislo lany na ktorej je mys
			int MouseOnLine = nn
			if(nn >= LB_LinesNum)
				nn = LB_LinesNum - 1

			nn *= LB_LineHeight
			nn += kk

			LB_SelDblClick -= 0.04//ftime

			for(n = 0; n < LB_ColNum; n++)
			{
				if(!LB_SliderPicked && InRect(LB_ColLeft[n], LB_ColRight[n], LB_Top, LB_Top + VisibleHeight, MouseX, MouseY))	//sme nad stlpcom n
				{
					if(LB_SelDblClick < 0)
						LB_SelDblClick = 0

					if(MousePress(0))			//klikli sme na niektory stlpec
					{
						ClearMousePress(0)
						LB_SelectLine(MouseOnLine)

						if(LB_SelDblClick > 0 && LB_SelLine == LB_OldClicked)	//dvojklick
							ClickedLine = LB_SelLine

						LB_SelDblClick = 1
						LB_OldClicked = LB_SelLine

						if(n == 0)
							events |= LB_evCol0Click

						if(n == 1)
							events |= LB_evCol1Click

						if(n == 2)
							events |= LB_evCol2Click

						if(n == 3)
							events |= LB_evCol3Click

						events |= LB_evLineClick
					}
				}
			}
		}
	}
	else	//pohybujeme sliderom
	{
		if(MousePress(0) == false)
			LB_SliderPicked = false

		LB_SliderTop = MouseY - LB_SliderPickOffset
	}

	if(LB_SliderTop < LB_Top)
		LB_SliderTop = LB_Top

	int tmp = LB_Bottom - LB_SliderHeight

	if(LB_SliderTop > tmp)
		LB_SliderTop = tmp

	// menime vysku slideru
	SetWidgetPos(LB_Slider, LB_SliderLeft, LB_SliderTop, LB_SliderWidth, LB_SliderHeight)
	
	if(LB_LinesNum <= 0)
		ShowWidget(LB_Selector, false)

	LB_Events = events
	return ClickedLine
}

//-----------------------------------------------------------------
void CloseLB()
{
	int k
	for(k = 0; k < LB_MaxLinesNum; k++)
	{
		for(int n = 0; n < LB_MaxColNum; n++)
		{
			if(LB_WidgetTexts[n][k])
				DestroyWidget(LB_WidgetTexts[n][k])

			LB_WidgetTexts[n][k] = NULL
			LB_Texts[n][k] = ""
		}

		if(LB_WidgetImages[k])
			DestroyWidget(LB_WidgetImages[k])

		LB_WidgetImages[k] = NULL
	}

	for(k = 0; k < LB_CustomImagesNum; k++)
	{
		if(LB_CustomImages[k])
		{
			DestroyWidget(LB_CustomImages[k])
			LB_CustomImages[k] = NULL
		}
	}

	if(LB_Slider)
		DestroyWidget(LB_Slider)

	LB_Slider = NULL

	if(LB_Selector)
		DestroyWidget(LB_Selector)

	LB_Selector = NULL

	if(LB_Back)
		DestroyWidget(LB_Back)

	LB_Back = NULL

	if(LB_ClipArea)	//pozor. je to parent widgetu LB_Back
		DestroyWidget(LB_ClipArea)
		
	LB_ClipArea = NULL
}

//--------------------------------------------------------------
void DeleteKeyDef(int KeyCode, int KeyPS, int TmpKeys[])
{
	int n

	for(n = 0; n < ListboxItemsNum; n++)
	{
		if(TmpKeys[n] == KeyCode)
		{
			TmpKeys[n] = 0		//zmazeme definiciu tejto klavesy

			if(KeyPS == 0)	//primary key
				SetWidgetText(ListboxPrimaryWidget[n], 0, "")

			if(KeyPS == 1)	//secondary key
				SetWidgetText(ListboxSecondaryWidget[n], 0, "")

			return
		}
	}
}

//--------------------------------------------------------------
void DupliciteKeyTest(int Key2Test, int OldDefinedKey)
{
	if(Key2Test == OldDefinedKey)
		return

	for(int n = 0; n < 256; n++)
	{
		if(g_KeyDef1_temp[n] == Key2Test)
		{
			DeleteKeyDef(g_KeyDef1_temp[n], 0, g_KeyDef1_temp)
			return
		}

		if(g_KeyDef2_temp[n] == Key2Test)
		{
			DeleteKeyDef(g_KeyDef2_temp[n], 1, g_KeyDef2_temp)
			return
		}
	}
}

//--------------------------------------------------------------
void SetItemsPosition(widget LboxItemWidgets[], widget LboxPrimaryWidget[], widget LboxSecondaryWidget[], widget LboxBack[], widget LScrollbar[], widget LSelectedLine[], int LboxCol0Pos[], int LboxCol1Pos[], int LboxCol2Pos[], int IOffsetV[], int LboxItemsNum)
{
	int n = 0
	int movement
	int offV
	int SelItemH	//Horizontaln pozicia selectnuteho itemu
	int SelItemOff
	int ListPosV
	int KeyType
	int ms	//Mouse state

	if(ListKeyInput != 0)
	{
		n = 0
		while(n < 255)
		{
			if(KeyState(n))
			{
				ClearKey(n)
				int KI = ListKeyInput - 1
				int k1 = g_KeyDef1_temp[KI]
				int k2 = g_KeyDef2_temp[KI]

				if(KeyType == 1)	//Primary
				{
					if(n == KC_ESCAPE)
					{
						if(k1 >= 0)
							SetWidgetTextEx(LboxPrimaryWidget[KI], 0, true, KeyNames[k1])
						else
						{
							k1 = -k1
							SetWidgetTextEx(LboxPrimaryWidget[KI], 0, true, OtherNames[k1])
						}
					}
					else
					{
						ClearMousePress(0)
						DupliciteKeyTest(n, k1)
						g_KeyDef1_temp[KI] = n
						SetWidgetTextEx(LboxPrimaryWidget[KI], 0, true, KeyNames[n])
					}
					SetWidgetTextColor(LboxPrimaryWidget[KI], GUIInactiveColor)
				}
				if(KeyType == 2)	//Secondary
				{
					if(n == KC_ESCAPE)
					{
						if(k2 >= 0)
							SetWidgetTextEx(LboxSecondaryWidget[KI], 0, true, KeyNames[k2])
						else
						{
							k2 = -k2
							SetWidgetTextEx(LboxSecondaryWidget[KI], 0, true, OtherNames[k2])
						}
					}
					else
					{
						ClearMousePress(0)
						DupliciteKeyTest(n, k2)
						g_KeyDef2_temp[KI] = n
						SetWidgetTextEx(LboxSecondaryWidget[KI], 0, true, KeyNames[n])
					}
					SetWidgetTextColor(LboxSecondaryWidget[KI], GUIInactiveColor)
				}
				ListKeyInput = 0
				KeyType = 0
			}
			n++
		}

		if(KeyType > 0)
		{
			ms = 0
			if(MousePress(0))					//Zmackli sme levy mouse pri editaci primary key
			{
				ClearMousePress(0)
				ms = g_MouseState[0]
			}
			if(MousePress(1))					//Zmackli sme stredni mouse pri editaci primary key
			{
				ClearMousePress(1)
				ms = g_MouseState[1]
			}
			if(MousePress(2))					//Zmackli sme pravy mouse pri editaci primary key
			{
				ClearMousePress(2)
				ms = g_MouseState[2]
			}

			if(MouseWheel > 0)			//Kolecko UP
				ms = g_MouseState[3]
			if(MouseWheel < 0)			//Kolecko DOWN
				ms = g_MouseState[4]

			for(n = 0; n < 8; n++)
			{
				if(JoyButtons[n] > 0)
					ms = g_MouseState[n + 5]
			}

			if(ms != 0)
			{
				int MI = ListKeyInput - 1
				int kk1 = g_KeyDef1_temp[MI]
				int kk2 = g_KeyDef2_temp[MI]

				if(KeyType == 1)
				{
					DupliciteKeyTest(ms, kk1)
					g_KeyDef1_temp[MI] = ms
					SetWidgetTextEx(LboxPrimaryWidget[MI], 0, true, OtherNames[-ms])
					SetWidgetTextColor(LboxPrimaryWidget[MI], GUIInactiveColor)
				}

				if(KeyType == 2)
				{
					DupliciteKeyTest(ms, kk2)
					g_KeyDef2_temp[MI] = ms
					SetWidgetTextEx(LboxSecondaryWidget[MI], 0, true, OtherNames[-ms])
					SetWidgetTextColor(LboxSecondaryWidget[MI], GUIInactiveColor)
				}

				ListKeyInput = 0
				KeyType = 0
			}
		}

		return
	}

	if(movement == 0)
	{
		if(InRect(ScrollbarArea[0], ScrollbarArea[2], ScrollbarArea[1], ScrollbarArea[3], MouseX, MouseY) == true)
		{
			if(InRect(ScrollbarRect[0], ScrollbarRect[2], ScrollbarRect[1], ScrollbarRect[3], MouseX, MouseY) == true)
			{
				movement = 1
				offV = MouseY - ScrollbarRect[1]
			}
			else
			{
				movement = 1
				offV = ScrollbarRect[3] - ScrollbarRect[1]
				offV = offV / 2
			}
		}

		if(InRect(ListboxArea[0], ListboxArea[2], ListboxArea[1], ListboxArea[3], MouseX, MouseY) == true)
		{
			n = 0
			while(n < LboxItemsNum)
			{
				SelItemH = ListPosV + IOffsetV[n]
				if(MouseY >= SelItemH)
				{
					int tmp = SelItemH + LineHeight
					if(MouseY < tmp)
					{
						SelItemOff = IOffsetV[n]
						SetWidgetPos(LSelectedLine[0], ListboxArea[0], SelItemH - ListboxArea[1], 500, 20)
						ShowWidget(LSelectedLine[0], 1)

						if(MousePress(0))	//Klikli sme na item n
						{
							if(InRect(LboxCol1Pos[0], LboxCol2Pos[0], SelItemH, tmp, MouseX, MouseY))	//Klikli sme na pole "primary key"
							{
								ClearMousePress(0)
								ListKeyInput = n + 1
								KeyType = 1
								SetWidgetText(LboxPrimaryWidget[n], 0, "???")
								SetWidgetTextColor(LboxPrimaryWidget[n], GUIActiveColor)
								break
							}
							if(InRect(LboxCol2Pos[0], ListboxArea[2], SelItemH, tmp, MouseX, MouseY))	//Klikli sme na pole "secondary key"
							{
								ClearMousePress(0)
								ListKeyInput = n + 1
								KeyType = 2
								SetWidgetText(LboxSecondaryWidget[n], 0, "???")
								SetWidgetTextColor(LboxSecondaryWidget[n], GUIActiveColor)
								break
							}
						}
						else
						{
							if(MousePress(2) || KeyState(KC_DELETE))
							{
								ClearMousePress(2)
								ClearKey(KC_DELETE)

								if(InRect(LboxCol1Pos[0], LboxCol2Pos[0], SelItemH, tmp, MouseX, MouseY))	//Klikli sme na pole "primary key"
								{
									g_KeyDef1_temp[n] = 0	//mazeme definiciu tejto klavesy
									SetWidgetText(ListboxPrimaryWidget[n], 0, "")
								}
								else
								{
									if(InRect(LboxCol2Pos[0], ListboxArea[2], SelItemH, tmp, MouseX, MouseY))	//Klikli sme na pole "secondary key"
									{
										g_KeyDef2_temp[n] = 0
										SetWidgetText(ListboxSecondaryWidget[n], 0, "")
									}
								}
							}
						}
					}
				}
				n++
			}

			if(MouseWheel != 0)
			{
				ScrollbarRect[1] = ScrollbarRect[1] - (MouseWheel * MouseWheelSpeed)
				ScrollbarRect[3] = ScrollbarRect[1] + ScrollbarHeight

				if(ScrollbarRect[1] < ScrollbarArea[1])
					ScrollbarRect[1] = ScrollbarArea[1]

				if(ScrollbarRect[3] > ScrollbarArea[3])
					ScrollbarRect[1] = ScrollbarArea[3] - ScrollbarHeight

				SetWidgetPos(LScrollbar[0], ScrollbarRect[0], ScrollbarRect[1], ScrollbarWidth, ScrollbarHeight)
			}
		}
	}
	else
	{
		ScrollbarRect[1] = MouseY - offV
		ScrollbarRect[3] = ScrollbarRect[1] + ScrollbarHeight

		if(ScrollbarRect[1] < ScrollbarArea[1])
			ScrollbarRect[1] = ScrollbarArea[1]

		if(ScrollbarRect[3] > ScrollbarArea[3])
			ScrollbarRect[1] = ScrollbarArea[3] - ScrollbarHeight

		ScrollbarRect[3] = ScrollbarRect[1] + ScrollbarHeight
		SetWidgetPos(LScrollbar[0], ScrollbarRect[0], ScrollbarRect[1], ScrollbarWidth, ScrollbarHeight)
	}

	if(g_LMB == 0)
		movement = 0

	int ListHeight = LineHeight * ListboxItemsNum
	ListHeight -= ScrollbarArea[3] - ScrollbarArea[1]
	int AreaHeight = ScrollbarArea[3] - ScrollbarArea[1] - ScrollbarHeight
	int RectInAreaPos = ScrollbarRect[1] - ScrollbarArea[1]
	float AreaRate = RectInAreaPos / AreaHeight
	float ListOffV = ListHeight * AreaRate
	ListPosV = ListboxArea[1] - ListOffV

	n = 0
	while(n < LboxItemsNum)
	{
		int pos1 = ListPosV + IOffsetV[n]
		int pos2 = ListboxArea[1] - LineHeight
		int HideValue = 0

		if(pos1 > pos2)
		{
			if(pos1 < ListboxArea[3])
				HideValue = 1
		}

		ShowWidget(LboxItemWidgets[n], HideValue)
		ShowWidget(LboxPrimaryWidget[n], HideValue)
		ShowWidget(LboxSecondaryWidget[n], HideValue)
		ShowWidget(LboxBack[n], HideValue)

      // border fade
      float alpha = 255
/*      int tempval = pos2 + (LineHeight * 2)

      // top line
      if (pos1 < tempval)
         alpha = 255 - (255 / (LineHeight * 2) * (tempval - pos1))

      // bottom line
      tempval = ListboxArea[3] - (LineHeight * 2)
      if (pos1 > tempval)
         alpha = 255 - (255 / (LineHeight * 2) * (pos1 - tempval))
*/
      // set the alphas
      int color = ARGB(alpha, 255, 255, 255)
      int textcolor = ARGB(alpha, 86, 253, 255)

      SetWidgetTextColor(LboxItemWidgets[n], textcolor);
      SetWidgetTextColor(LboxPrimaryWidget[n], textcolor);
      SetWidgetTextColor(LboxSecondaryWidget[n], textcolor);
      SetWidgetColor(LboxBack[n], color);

      int left = TextAlignOffset - ListboxArea[0]
      int top = ListPosV + IOffsetV[n] - ListboxArea[1]
      int width = 500
      int height = 25
      
		SetWidgetPos(LboxItemWidgets[n], LboxCol0Pos[0] + left, top, width, height)
		SetWidgetPos(LboxPrimaryWidget[n], LboxCol1Pos[0] + left, top, width, height)
		SetWidgetPos(LboxSecondaryWidget[n], LboxCol2Pos[0] + left, top, width, height)

		SetWidgetPos(LboxBack[n], 0, ListPosV + IOffsetV[n] - ListboxArea[1], 550, 24)
		n++
	}

	pos1 = ListPosV + SelItemOff
	HideValue = 0

	if(pos1 > pos2)
	{
		if(pos1 < ListboxArea[3])
			HideValue = 1
	}
	ShowWidget(LSelectedLine[0], HideValue)
	SetWidgetPos(LSelectedLine[0], 0, ListPosV + SelItemOff - ListboxArea[1], 550, 24)
}

void ShowSavesListbox(bool stat)
{
	if(stat == false)
	{
		LB_EnableEvents(false)
		CloseLB()
		return
	}

	LB_Open(4, 209)														//pocet stlpcov, aktivna vyska
	LB_SetListBoxPos(85, 240)											//lavy horny roh aktivnej plochy
	LB_AddLineImage("gfx/menu/controls_line_0load.tga", 546, 21)		//bitmapa pre jeden riadok
	LB_SetColWidth(0, 130)												//sirka stlpca
	LB_SetColWidth(1, 200)												//sirka stlpca
	LB_SetColWidth(2, 30)												//sirka stlpca
	LB_SetColWidth(3, 200)												//sirka stlpca
	LB_AddSliderImage(""/*"gfx/menu/controls_scrollbar.tga"*/, 16, 64)		//bitmapa pre slider. sirka, vyska
	LB_AddBackgroundImage("gfx/menu/controls_line_x.tga")			//pozadie na aktivnej ploche
	LB_AddSelectorImage(""/*"gfx/menu/controls_line_1load.tga"*/, 550, 19)	//bitmapa pre kurzor
	LB_AddCustomImage("gfx/menu/controls_clip_up.tga", 85, 232, 563 ,7, 0xffffffff, 0)
	LB_AddCustomImage("gfx/menu/controls_clip_down.tga", 85, 450, 563 ,7, 0xffffffff, 0)
	LB_AddCustomImage(""/*"gfx/menu/save_scroll_area.tga"*/, 632, 240, 16, 208, ARGB(255,30,155,155), -6)
	LB_EnableEvents(true)												//povolime update
	 
	int SaveColorNormal = ARGB(255,255,255,255)
	int SaveColorQuick = ARGB(255,127,127,127)
	int SaveColorEmpty = ARGB(255,255,255,255)

	for(int n = 0; n < sizeof(SaveFileNames); n++)
	{
		if(SaveTypes[n] != SAVETYPE_NONE)
		{
			bool Localized = false
			string MapOffName
			string SPointOffName = ""
			int LineColor = SaveColorNormal

			if(SaveTypes[n] == SAVETYPE_QUICK)
				Localized = true
			else
			{
				if(strlen(SaveFileNames[n]) == 7 && substr(SaveFileNames[n], 0, 1) == "l")
						Localized = true
			}

			if(Localized)
				MapOffName = "#" + SaveMapsNames[n]// substr(SaveFileNames[n], 0, 3)
			else
				MapOffName = SaveFileNames[n]

			if(SaveTypes[n] == SAVETYPE_AUTO)
				SPointOffName = "#save_auto"

			if(SaveTypes[n] == SAVETYPE_QUICK)
			{
				SPointOffName = "#save_quick"
				LineColor = SaveColorQuick
			}

			if(SaveTypes[n] == SAVETYPE_NORMAL)
				SPointOffName = "#save_normal"

			if(SaveTypes[n] == SAVETYPE_EMPTY)
			{
				SPointOffName = "#save_empty"
				LineColor = SaveColorEmpty
			}

			int ln = LB_AddLine()
			LB_SetLineText(0, ln, SPointOffName, true)
			LB_SetLineText(1, ln, MapOffName, Localized)
			LB_SetLineText(2, ln, " ", false)
			LB_SetLineText(3, ln, SaveMapsTimes[n], false)
			LB_SetLineColor(0, ln, LineColor)
			LB_SetLineColor(1, ln, LineColor)
			LB_SetLineColor(2, ln, LineColor)
			LB_SetLineColor(3, ln, LineColor)
		}
	}
	LB_SelectLine(g_SaveSelected)
	LB_Update()
}

//-----------------------------------------------------------------
void InitCredits()
{
	CreditsWidth = g_iWidth
	CreditsCurrentPos = 0
	CreditsFullHeight = 0
	NumLines = 0
	ResolutionScale = (float)g_iHeight / 768

//	int TitleColor = ARGB(255, 107, 107, 127)
	int TitleColor = ARGB(255, 255, 204, 0)
	int NameColor = ARGB(255, 255, 255, 255)

	CreditsBitmap("gfx/menu/titlelogo.tga", 0xffffffff, 282, 0, 256, 128)
	CreditsTextLine("#credits_text0001", MainFont, TitleColor, -1, 0, g_iWidth) //"by"
	CreditsTextLine("#credits_text0002", LargeFont, NameColor, -1, 0, g_iWidth) //"Black Element Software s.r.o."
	CreditsSpace(50)
	CreditsTextLine("#credits_text0003", MainFont, TitleColor, -1, 0, g_iWidth) //"Published by"
	CreditsTextLine("#credits_text0004", LargeFont, NameColor, -1, 0, g_iWidth) //"IDEA Games a.s."
	CreditsSpace(300)

	CreditsBitmap("gfx/menu/logo_bes.tga", 0xffffffff, 282, 0, 99, 99)
	CreditsTextLine("#credits_text0005", LargeFont, NameColor, -1, 0, g_iWidth) //"Black Element Software"
	CreditsTextLine("#credits_text0006", MainFont, NameColor, -1, 0, g_iWidth) //"www.blackelement.net"
	CreditsSpace(100)
	CreditsTextLine("#credits_text0101", LargeFont, TitleColor, -1, 0, g_iWidth) //"Project Manager"
	CreditsTextLine("#credits_text0102", LargeFont, NameColor, -1, 0, g_iWidth) //"Radek Volf"
	CreditsSpace(50)
	CreditsTextLine("#credits_text0201", LargeFont, TitleColor, -1, 0, g_iWidth) //"Lead Designer"
	CreditsTextLine("#credits_text0202", LargeFont, NameColor, -1, 0, g_iWidth) //"Radek Volf"
	CreditsSpace(50)
	CreditsTextLine("#credits_text0301", LargeFont, TitleColor, -1, 0, g_iWidth) //"Lead Programmer"
	CreditsTextLine("#credits_text0302", LargeFont, NameColor, -1, 0, g_iWidth) //"Filip Doksansky"
	CreditsSpace(50)
	CreditsTextLine("#credits_text0501", LargeFont, TitleColor, -1, 0, g_iWidth) //"Lead 3D Artist"
	CreditsTextLine("#credits_text0502", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsSpace(50)
	CreditsTextLine("#credits_text0401", LargeFont, TitleColor, -1, 0, g_iWidth) //"Lead 2D Artist"
	CreditsTextLine("#credits_text0402", LargeFont, NameColor, -1, 0, g_iWidth) //"Ales Horak"
	CreditsSpace(50)
	CreditsTextLine("#credits_text0510", LargeFont, TitleColor, -1, 0, g_iWidth) //"Original Concept"
	CreditsTextLine("#credits_text0511", LargeFont, NameColor, -1, 0, g_iWidth) //"Ondoej Neff"
	CreditsSpace(50)
	CreditsTextLine("#credits_text0520", LargeFont, TitleColor, -1, 0, g_iWidth) //"Screenwriter"
	CreditsTextLine("#credits_text0521", LargeFont, NameColor, -1, 0, g_iWidth) //"Vilma Klímov?
	CreditsSpace(50)
	CreditsTextLine("#credits_text0601", LargeFont, TitleColor, -1, 0, g_iWidth) //"Programmers"
	CreditsTextLine("#credits_text0302", LargeFont, NameColor, -1, 0, g_iWidth) //"Filip Doksansky"
	CreditsTextLine("#credits_text0602", LargeFont, NameColor, -1, 0, g_iWidth) //"Andrej Torok"
	CreditsTextLine("#credits_text0603", LargeFont, NameColor, -1, 0, g_iWidth) //"Michal Varnuska"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1001", LargeFont, TitleColor, -1, 0, g_iWidth) //"Additional Tools Programming"
	CreditsTextLine("#credits_text1002", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsTextLine("#credits_text1003", LargeFont, NameColor, -1, 0, g_iWidth) //"Milos Zajic"
	CreditsTextLine("#credits_text1004", LargeFont, NameColor, -1, 0, g_iWidth) //"Viktor Bocan"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1101", LargeFont, TitleColor, -1, 0, g_iWidth) //"Music composer"
	CreditsTextLine("#credits_text1102", LargeFont, NameColor, -1, 0, g_iWidth) //"Jaroslav Kasny"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1201", LargeFont, TitleColor, -1, 0, g_iWidth) //"Level Designers"
	CreditsTextLine("#credits_text1202", LargeFont, NameColor, -1, 0, g_iWidth) //"Ales Horak"
	CreditsTextLine("#credits_text1203", LargeFont, NameColor, -1, 0, g_iWidth) //"Rudolf Snizek"
	CreditsTextLine("#credits_text1204", LargeFont, NameColor, -1, 0, g_iWidth) //"Josef Vlach"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1206", LargeFont, TitleColor, -1, 0, g_iWidth) //Gameplay scripting
	CreditsTextLine("#credits_text1207", LargeFont, NameColor, -1, 0, g_iWidth) //Michal Svìtl?
	CreditsSpace(50)
	CreditsTextLine("#credits_text1301", LargeFont, TitleColor, -1, 0, g_iWidth) //"Modelers"
	CreditsTextLine("#credits_text1303", LargeFont, NameColor, -1, 0, g_iWidth) //"Milos Zajic"
	CreditsTextLine("#credits_text1305", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Bartonek"
	CreditsTextLine("#credits_text1304", LargeFont, NameColor, -1, 0, g_iWidth) //"Kamila Clarova"
	CreditsTextLine("#credits_text1306", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsSpace(50)
	CreditsTextLine("#credits_text2905", LargeFont, TitleColor, -1, 0, g_iWidth) //"Texture Artists"
	CreditsTextLine("#credits_text0402", LargeFont, NameColor, -1, 0, g_iWidth) //"Ales Horak"
	CreditsTextLine("#credits_text2906", LargeFont, NameColor, -1, 0, g_iWidth) //"Martin Lisec"
	CreditsTextLine("#credits_text1305", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Bartonek"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1401", LargeFont, TitleColor, -1, 0, g_iWidth) //"Character Modeling"
	CreditsTextLine("#credits_text1402", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Bartonek"
	CreditsTextLine("#credits_text1403", LargeFont, NameColor, -1, 0, g_iWidth) //"Kamila Clarova"
	CreditsTextLine("#credits_text1404", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsSpace(50)
//	CreditsTextLine("#credits_text1501", LargeFont, TitleColor, -1, 0, g_iWidth) //"Heads Modeling"
//	CreditsTextLine("#credits_text1502", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Bartonek"
//	CreditsSpace(50)
	CreditsTextLine("#credits_text1601", LargeFont, TitleColor, -1, 0, g_iWidth) //"Animators"
	CreditsTextLine("#credits_text1602", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsTextLine("#credits_text1603", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Bartonek"
	CreditsTextLine("#credits_text1604", LargeFont, NameColor, -1, 0, g_iWidth) //"Kamila Clarova"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1701", LargeFont, TitleColor, -1, 0, g_iWidth) //"Cutscenes director"
	CreditsTextLine("#credits_text1702", LargeFont, NameColor, -1, 0, g_iWidth) //"Radek Volf"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1801", LargeFont, TitleColor, -1, 0, g_iWidth) //"Cutscenes cameras & edit"
	CreditsTextLine("#credits_text1802", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsSpace(50)
	CreditsTextLine("#credits_text2001", LargeFont, TitleColor, -1, 0, g_iWidth) //"Cutscene Motion Capture Actors"
	CreditsTextLine("#credits_text2002", LargeFont, NameColor, -1, 0, g_iWidth) //"Tomas Kraucher"
	CreditsTextLine("#credits_text2003", LargeFont, NameColor, -1, 0, g_iWidth) //"Alexandra Lanska"
	CreditsTextLine("#credits_text2004", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsSpace(50)
	CreditsTextLine("#credits_text1901", LargeFont, TitleColor, -1, 0, g_iWidth) //"Ingame Motion Capture Actor"
	CreditsTextLine("#credits_text1902", LargeFont, NameColor, -1, 0, g_iWidth) //"Jan Zamecnik"
	CreditsSpace(50)
	CreditsTextLine("#credits_text2101", LargeFont, TitleColor, -1, 0, g_iWidth) //"Sound Effects"
	CreditsTextLine("#credits_text2102", LargeFont, NameColor, -1, 0, g_iWidth) //"Jaroslav Kasny"
	CreditsSpace(50)
	CreditsTextLine("#credits_text2200", LargeFont, TitleColor, -1, 0, g_iWidth) //"Cast"
	CreditsTextLine("#credits_text2201", LargeFont, NameColor, -1, 0, g_iWidth) //"Arnold - Daniel Brown"
	CreditsTextLine("#credits_text2202", LargeFont, NameColor, -1, 0, g_iWidth) //"Livia - Amy Huck"
	CreditsTextLine("#credits_text2203", LargeFont, NameColor, -1, 0, g_iWidth) //"Olivier - Jeff Smith"
	CreditsTextLine("#credits_text2204", LargeFont, NameColor, -1, 0, g_iWidth) //"Warren - Ryan James"
	CreditsTextLine("#credits_text2205", LargeFont, NameColor, -1, 0, g_iWidth) //"Bruce - DJ Ni"
	CreditsTextLine("#credits_text2206", LargeFont, NameColor, -1, 0, g_iWidth) //"Paolo - David"
	CreditsSpace(300)

	CreditsBitmap("gfx/menu/logo_idea.tga", 0xffffffff, 282, 0, 96, 86)
	CreditsTextLine("#credits_text2301", LargeFont, TitleColor, -1, 0, g_iWidth) //"IDEA Games"
	CreditsSpace(100)
	CreditsTextLine("#credits_text2401", LargeFont, TitleColor, -1, 0, g_iWidth) //"Producer"
	CreditsTextLine("#credits_text2402", LargeFont, NameColor, -1, 0, g_iWidth) //"Martin Klima"
	CreditsSpace(50)
	CreditsTextLine("#credits_text2501", LargeFont, TitleColor, -1, 0, g_iWidth) //"PR & marketing"
	CreditsTextLine("#credits_text2502", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Rydl"
	CreditsSpace(50)
	CreditsTextLine("#credits_text2601", LargeFont, TitleColor, -1, 0, g_iWidth) //"Sales"
	CreditsTextLine("#credits_text2602", LargeFont, NameColor, -1, 0, g_iWidth) //"Jiri Jakubec"
	CreditsSpace(300)
	CreditsTextLine("#credits_text2701", LargeFont, TitleColor, -1, 0, g_iWidth) //"Special thanks to"
	CreditsTextLine("#credits_text2702", LargeFont, NameColor, -1, 0, g_iWidth) //"Dennis Gustafsson, Jonas Gustavsson, Tomas Ahlstrom"
	CreditsTextLine("#credits_text2703", LargeFont, NameColor, -1, 0, g_iWidth) //"Slavomir Pavlicek"
	CreditsTextLine("#credits_text2704", LargeFont, NameColor, -1, 0, g_iWidth) //"Marek Spanel"
	CreditsTextLine("#credits_text2705", LargeFont, NameColor, -1, 0, g_iWidth) //"Paul R. Statham"
	CreditsSpace(300)
	CreditsBitmap("gfx/menu/logo_pathengine.tga", 0xffffffff, 314, 0, 170, 56)
	CreditsSpace(10)
	CreditsTextLine("#credits_text2800", LargeFont, TitleColor, -1, 0, g_iWidth) //"Agent movement"
	CreditsTextLine("#credits_text2801", LargeFont, NameColor, -1, 0, g_iWidth) //"powered by PathEngine ?www.pathengine.com"
	CreditsSpace(300)
	CreditsTextLine("#credits_text2901", LargeFont, TitleColor, -1, 0, g_iWidth) //"(C) 2006 Black Element Software s.r.o. All rights reserved."
	CreditsSpace(15)
	CreditsTextLine("#credits_text2902", MainFont, TitleColor, -1, 0, g_iWidth) //"Other products and company names mentioned herein maybe trademarks of their respective owners."
	CreditsTextLine("#credits_text2903", MainFont, TitleColor, -1, 0, g_iWidth) //"Developed by Black Element Software. All rights reserved."
	CreditsSpace(50)
	CreditsTextLine("#credits_text2904", LargeFont, NameColor, -1, 0, g_iWidth) //"www.alpha-prime.com"
	CreditsSpace(500)

	if(MenuMap)
		CreditsBackgroundColor(0x80000000)
}

//-----------------------------------------------------------------
void CloseMenu(string menu)
{
	switch(menu)
	{
	case "main_menu":
		UnloadMenuMap()
		break

	case "controls_menu":
		FreeListbox(ListboxItemWidgets, ListboxPrimaryWidget, ListboxSecondaryWidget, ListboxBack, Scrollbar, SelectedLine, AreaTop, AreaBottom)
		break
		
	case "loadgame_menu":
		ShowSavesListbox(false)
		AddSaveEmptySlot()	//pri odchode z menu musime vratit medzi sloty jeden prazdny (normalny stav)
		break

	case "savegame_menu":
		ShowSavesListbox(false)
		break

	case "ingame_menu":
		if(globalmap)
		{
			int MapHandle = SetCurrentMap(globalmap)
			EnableEnemies()
			World.PlayerControl = 1
			SetCurrentMap(MapHandle)
		}

		if(g_IMBackground)
		{
			DestroyWidget(g_IMBackground)
			g_IMBackground = NULL
		}
		break
		
	case "credits_menu":
		DestroyCredits()
		ShowWidget(WMousePointer, true)

//			ShowWidget(WView, true)
//			g_NoUpdateMenuMap = false
		break
	}
}

//-----------------------------------------------------------------
int OpenMenu(string menu)
{
	const string noyesq[2] = {"#menuitem1_no", "#menuitem1_yes"}
	const string lowhighq[2] = {"#menuitem2_low", "#menuitem2_high"}

	int ButtonPos = 240
	int ButtonLeftPos = 85
	int ButtonLeftPos2 = 300
	int ButtonLeftPos3 = 430
	int ButtonLeftPos4 = 645
	int ButtonOff = 30

	switch(menu)
	{
	case "main_menu":
		LoadMenuMap()
		LoadSaveList()	//tu sa natiahne raz a viac uz neni potreba
		
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background
		GUIImage("menufooter", g_iWidth - 512, g_iHeight - 128, 512, 128, 2, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND|WF_NOFILTER|WF_EXACTSIZE|WF_EXACTPOS, 0xffffffff)	// menu footer

      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_main", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)

		GUISetFont(LargeFont)

		GUIButton("main_new_game", "#main_new_game",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		GUIAction(GUI_ACTION_MENU, "newgame_menu")
			ButtonPos += ButtonOff
		
		GUIButton("main_continue_game", "#main_continue_game", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		ButtonPos += ButtonOff

		if(GetLastSavedSlot() == -1)	//neni tam ani jeden sejv
			GUIDisableItem("main_continue_game")

		GUIButton("main_load_game", "#main_load_game",	ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU, "loadgame_menu")
			ButtonPos += ButtonOff

		bool LoadEnabled = true
		if(SaveTypes[0] == SAVETYPE_NONE)
			LoadEnabled	= false
			
		if(SaveTypes[0] == SAVETYPE_EMPTY && SaveTypes[1] == SAVETYPE_NONE)
			LoadEnabled	= false

		if(!LoadEnabled)
			GUIDisableItem("main_load_game")

      // indent
		ButtonPos += ButtonOff
 	
		GUIButton("options_controls", "/#options_controls",			ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "controls_menu")
			ButtonPos += ButtonOff
			
		GUIButton("options_audio", "/#options_audio",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "audio_menu")
			ButtonPos += ButtonOff

		GUIButton("options_video", "/#options_video",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "video_menu")
			ButtonPos += ButtonOff

		GUIButton("options_advanced", "/#options_advanced",	ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "advanced_menu")
			ButtonPos += ButtonOff

//		GUIButton("options_language", "#options_language",	100, 370, 290, 32, 1, WF_RALIGN)
//		GUIButton("options_language", "Language",	100, 370, 290, 32, 1, WF_RALIGN)
//			string langl[7] = {"English", "Cesky", "Deutsche", "Francaise", "Italiano", "Spanish", "US english"}
//			GUIListValue(20, langl, 410, 370, 256, 32, 1, 0)

//		GUIButton("options_back", "#options_back",					200, 500, 400, 32, 1, WF_CENTER)
//			GUIAction(GUI_ACTION_BACK|GUI_ACTION_APPLYOPT)
			


//			ButtonPos += ButtonOff	// twice indent

		GUIButton("main_credits", "/#main_credits",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU, "credits_menu")
			ButtonPos += ButtonOff

		GUIButton("main_exit", "#main_exit", ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)

		float AspectFormFactor = (float)g_iHeight / (float)600	//pri WF_EXACTSIZE sa font nescaluje
		AspectFormFactor *= 15
		GUITitle(AlphaPrimeVersion, g_iWidth - 300, g_iHeight - 21 - AspectFormFactor, 100, 25, 130, WF_RALIGN|WF_NOFILTER|WF_EXACTSIZE|WF_EXACTPOS, 0xffffffff, MainFont) 

		if(!MenuMusicID)
			//SetSoundVolume(MenuMusicID, 1)
			MenuMusicID = PlaySound(NULL, MenuMusic, SF_AMBIENT|SF_MUSIC)
		   SetSoundVolume(MenuMusicID, 1)
		break


	case "ingame_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background

      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_main", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)

		GUISetFont(LargeFont)

		GUIButton("ingame_quit", "#ingame_quit",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			//GUIAction(GUI_ACTION_BACK)
			ButtonPos += ButtonOff
		
		GUIButton("ingame_save_game", "#ingame_save_game", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		    GUIAction(GUI_ACTION_MENU, "savegame_menu")
		    ButtonPos += ButtonOff
		   
//		GUIDisableItem("ingame_save_game")


		GUIButton("ingame_load_game", "#ingame_load_game",	ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU, "loadgame_menu")
			ButtonPos += ButtonOff

		bool LoadEnabled2 = true
		if(SaveTypes[0] == SAVETYPE_NONE)
			LoadEnabled2	= false
			
		if(SaveTypes[0] == SAVETYPE_EMPTY && SaveTypes[1] == SAVETYPE_NONE)
			LoadEnabled2	= false

		if(!LoadEnabled2)
			GUIDisableItem("ingame_load_game")

      // indent
		ButtonPos += ButtonOff
 	
		GUIButton("ingame_options_controls", "/"+"#options_controls",			ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "controls_menu")
			ButtonPos += ButtonOff
			
		GUIButton("ingame_options_audio", "/"+"#options_audio",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "audio_menu")
			ButtonPos += ButtonOff

		GUIButton("ingame_options_video", "/"+"#options_video",		ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "video_menu")
			ButtonPos += ButtonOff

		GUIButton("ingame_options_advanced", "/"+"#options_advanced",	ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_MENU|GUI_ACTION_GETOPT, "advanced_menu")
			ButtonPos += ButtonOff
			
			
		GUIButton("ingame_resume_game", "#ingame_resume_game", ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)

		break


	case "audio_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background

      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_audio", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
      
		GUISetFont(LargeFont)

		GUIButton("audio_msxvolume", "#audio_msxvolume",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUISlider(27, 100, "slider", ButtonLeftPos3, ButtonPos+9, 200, 24, 2, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIImage("sliderbackground", ButtonLeftPos3, ButtonPos+9, 200, 24, 1, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIHint("#audio_msxvolume")
			GUIHintInfo("#audio_msxvolumehint")
			ButtonPos += ButtonOff
			
		GUIButton("audio_efxvolume", "#audio_efxvolume",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUISlider(26, 100, "slider", ButtonLeftPos3, ButtonPos+9, 200, 24, 2, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIImage("sliderbackground", ButtonLeftPos3, ButtonPos+9, 200, 24, 1, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIHint("#audio_efxvolume")
			GUIHintInfo("#audio_efxvolumehint")
			ButtonPos += ButtonOff

		GUIButton("audio_speakers", "#audio_speakers",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			string speakers[8] = {"#menuitem4_direct", "#menuitem4_monoaural", "#menuitem4_stereo", "#menuitem4_quad", "#menuitem4_headphones", "#menuitem4_surround", "#menuitem4_51", "#menuitem4_71"}
			GUIListValue(13, speakers, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#audio_speakers")
			GUIHintInfo("#audio_speakershint")
			ButtonPos += ButtonOff

		GUIButton("audio_rate", "#audio_rate",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			//string rate[3] = {"22050", "32000", "44100"}
			string rate[3] = {"#menuitem2_low", "#menuitem2_mid", "#menuitem2_high"}
			GUIListValue(12, rate, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#audio_rate")
			GUIHintInfo("#audio_ratehint")
			ButtonPos += ButtonOff

		GUIButton("audio_occlusion", "#audio_occlusiontest",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(14, noyesq, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#audio_occlusiontest")
			GUIHintInfo("#audio_occlusiontesthint")
			ButtonPos += ButtonOff

			
		GUIButton("audio_quality", "#audio_quality",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			string audioql[3] = {"#menuitem2_low", "#menuitem2_mid", "#menuitem2_high"}
			GUIListValue(28, audioql, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#audio_quality")
			GUIHintInfo("#audio_qualityhint")
			ButtonPos += ButtonOff

			if(ItemValues[11])
				GUIDisableItem("audio_quality")
			else
				GUIEnableItem("audio_quality")
		

		GUIImageButton("audio_eax", "eaxlogo", ButtonLeftPos, ButtonPos, 128, 61, 1, WF_NOWRAP|WF_SOURCEALPHA | WF_BLEND,GUIInactiveColor)
		//GUIButton("audio_eax", "#audio_eax",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			string eax[2] = {"#menuitem1_no", "#menuitem1_yes"}
			GUIListValue(11, eax, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#audio_eax")
			GUIHintInfo("#audio_eaxhint")
			ButtonPos += ButtonOff
	
		GUIButton("audio_back", "#audio_back",	ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_BACK|GUI_ACTION_APPLYOPT)	
			
		break

	case "video_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background

      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_video", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
      
		GUISetFont(LargeFont)

	
		GUIButton("video_mode", "#video_mode",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(10, g_sVideoModes, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_mode")
			GUIHintInfo("#video_modehint")
			ButtonPos += ButtonOff

//		GUIButton("video_fsaa", "Antialiasing",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
		GUIButton("video_fsaa", "#video_fsaa",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(61, g_sFSAAModes, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_fsaa")
			GUIHintInfo("#video_fsaa_hint")
//			GUIHint("Antialiasing")
	//		GUIHintInfo("Sets full-scene anti-aliasing")
			ButtonPos += ButtonOff
			if(sizeof(g_sFSAAModes) == 0)
			{
				GUIDisableItem("video_fsaa")
			}

		GUIButton("video_bloom", "#video_bloom",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(60, noyesq, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_bloom")
			GUIHintInfo("#video_bloomhint")
			ButtonPos += ButtonOff
			if(r_maxshaderlevel == 0)
			{
				GUIDisableItem("video_bloom")
			}

		GUIButton("video_shaders", "#video_shaderlevel", ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(21, lowhighq, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_shaderlevel")
			GUIHintInfo("#video_shaderlevelhint")
			ButtonPos += ButtonOff
			if(r_maxshaderlevel == 0)
			{
				GUIDisableItem("video_shaders")
			}
			
		GUIButton("video_filtering", "#video_filtering", ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			const string filter[3] = {"#menuitem3_bilinear", "#menuitem3_trilinear", "#menuitem3_anisotropic"}
			GUIListValue(23, filter, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_filtering")
			GUIHintInfo("#video_filteringhint")
			ButtonPos += ButtonOff
			
		GUIButton("video_aniso", "#video_anisotropiclevel", ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUISlider(50, 15, "slider", ButtonLeftPos3, ButtonPos+9, 200, 24, 2, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIImage("sliderbackground", ButtonLeftPos3, ButtonPos+9, 200, 24, 1, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIHint("#video_anisotropiclevel")
			GUIHintInfo("#video_anisotropiclevelhint")
			ButtonPos += ButtonOff
            		
		GUIButton("video_texture", "#video_texture",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			const string texq[2] = {"#video_uncompressed", "#video_compressed"}
			GUIListValue(24, texq, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_texture")
			GUIHintInfo("#video_texturehint")
			ButtonPos += ButtonOff
			
		GUIButton("video_textureres", "#video_textureresolution", ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			const string texrq[3] = {"#menuitem2_high", "#menuitem2_mid", "#menuitem2_low"}
			GUIListValue(34, texrq, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#video_textureresolution")
			GUIHintInfo("#video_textureresolutionhint")
			ButtonPos += ButtonOff

		GUIButton("video_brightness", "#video_brightness",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUISlider(25, 100, "slider", ButtonLeftPos3, ButtonPos+9, 200, 24, 2, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIImage("sliderbackground", ButtonLeftPos3, ButtonPos+9, 200, 24, 1, WF_NOFILTER|WF_SOURCEALPHA|WF_BLEND)
			GUIHint("#video_brightness")
			GUIHintInfo("#video_brightnesshint")
			ButtonPos += ButtonOff

		GUIButton("video_subtitles", "#advanced_subtitles", ButtonLeftPos, ButtonPos, 290, 32, 1,  0, GUIInactiveColor)
			GUIListValue(55, noyesq, ButtonLeftPos3, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_subtitles")
			GUIHintInfo("#advanced_subtitleshint")

		GUIButton("video_back", "#video_back",	ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_BACK|GUI_ACTION_APPLYOPT)			

		if(ItemValues[23] != 2)
			GUIDisableItem("video_aniso")
		else
			GUIEnableItem("video_aniso")
		break

	case "advanced_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background

      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_advanced", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
      
		GUISetFont(LargeFont)
		
	   // first column
		GUIButton("effect_quality", "#advanced_effquality",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			string ppq[2] = {"#menuitem2_low", "#menuitem2_high"}
			GUIListValue(45, ppq, ButtonLeftPos2, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_effquality")
			GUIHintInfo("#advanced_effqualityhint")
			ButtonPos += ButtonOff

		GUIButton("advanced_shadows", "#advanced_shadows",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
//			string shadowq[3] = {"#menuitem1_no", "#menuitem2_mid", "#menuitem2_high"}
			GUIListValue(30, noyesq, ButtonLeftPos2, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_shadows")
			GUIHintInfo("#advanced_shadowshint")
			ButtonPos += ButtonOff
			ButtonPos += ButtonOff

	   int initbutpos = ButtonPos   // for init value restoring

		GUITitle("#advanced_buildings", ButtonLeftPos, ButtonPos, 200, 32, 1, 0, 0xff7f7f7f)
			ButtonPos += ButtonOff
	   
		GUIButton("advanced_reflections", "#advanced_reflections",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(31, noyesq, ButtonLeftPos2, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_reflections")
			GUIHintInfo("#advanced_reflectionshint")
			ButtonPos += ButtonOff
/*		
		GUIButton("advanced_bumpmaps", "#advanced_bumpmaps",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(32, noyesq, ButtonLeftPos2, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_bumpmaps")
			GUIHintInfo("#advanced_bumpmapshint")
			ButtonPos += ButtonOff
*/
		GUIButton("advanced_speculars", "#advanced_speculars",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
		   GUIListValue(33, noyesq, ButtonLeftPos2, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_speculars")
			GUIHintInfo("#advanced_specularshint")
			ButtonPos += ButtonOff

		// second column
		ButtonPos = initbutpos
		
		GUITitle("#advanced_terrain", ButtonLeftPos3, ButtonPos, 200, 32, 1, 0, 0xff7f7f7f)
  			ButtonPos += ButtonOff

		GUIButton("advanced_terrquality", "#advanced_terrquality",	ButtonLeftPos3, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
		   GUIListValue(37, lowhighq, ButtonLeftPos4, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_terrquality")
			GUIHintInfo("#advanced_terrqualityhint")
			ButtonPos += ButtonOff

		GUIButton("advanced_terrparallax", "#advanced_parallax",	ButtonLeftPos3, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUIListValue(35, noyesq, ButtonLeftPos4, ButtonPos, 100, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_parallax")
			GUIHintInfo("#advanced_parallaxhint")
			ButtonPos += ButtonOff

		GUIButton("advanced_terrspeculars", "#advanced_speculars",	ButtonLeftPos3, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
		   GUIListValue(36, noyesq, ButtonLeftPos4, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#advanced_speculars")
			GUIHintInfo("#advanced_specularshint")
			ButtonPos += ButtonOff

		if(ItemValues[37])
		{
			GUIEnableItem("advanced_terrparallax")
		}
		else
		{
			GUIDisableItem("advanced_terrparallax")
		}

		if(!ItemValues[21] || r_maxshaderlevel == 0)
		{
			GUIDisableItem("advanced_terrparallax")
			GUIDisableItem("advanced_terrspeculars")
			GUIDisableItem("advanced_terrquality")
		}

		GUIButton("advanced_back", "#advanced_back",	ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_BACK|GUI_ACTION_APPLYOPT)


		break	

	case "controls_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background

      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_controls", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
      
		GUISetFont(LargeFont)

      GUITitle("#controls_keyboard", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
      
		LoadListbox(KeyItems, ListboxItemWidgets, ListboxPrimaryValue, ListboxSecondaryValue, ListboxPrimaryWidget, ListboxSecondaryWidget, ListboxBack, Scrollbar, SelectedLine, AreaTop, AreaBottom, ListboxArea, ListboxCol0Pos, ListboxCol1Pos, ListboxCol2Pos, ItemOffsetV, ListboxItemsNum)

      // indent	
	   ButtonPos += ButtonOff * 6

		GUIButton("controls_mousex", "#controls_mousex",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUISlider(52, 100, "slider", ButtonLeftPos3, ButtonPos+9, 200, 24, 2, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND)
			GUIImage("sliderbackground", ButtonLeftPos3, ButtonPos+9, 200, 24, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND)
			GUIHint("#controls_mousex")
			GUIHintInfo("#controls_mousexhint")
			ButtonPos += ButtonOff
        			
		GUIButton("controls_mousey", "#controls_mousey",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			GUISlider(53, 100, "slider", ButtonLeftPos3, ButtonPos+9, 200, 24, 2, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND)
			GUIImage("sliderbackground", ButtonLeftPos3, ButtonPos+9, 200, 24, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND)
			GUIHint("#controls_mousey")
			GUIHintInfo("#controls_mouseyhint")
			ButtonPos += ButtonOff

		GUIButton("controls_invert", "#controls_invert",	ButtonLeftPos, ButtonPos, 250, 32, 1, 0, GUIInactiveColor)
			const string invert[2] = {"#menuitem1_yes", "#menuitem1_no"}
			GUIListValue(51, invert, ButtonLeftPos3, ButtonPos, 256, 32, 1, 0, GUIInactiveColor)
			GUIHint("#controls_invert")
			GUIHintInfo("#controls_inverthint")
			ButtonPos += ButtonOff

		GUIButton("controls_default", "#controls_default",	ButtonLeftPos3, 540, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_CALL, "SetDefaultSettings")
			GUIHint("#controls_default")
			GUIHintInfo("#controls_defaulthint")
			
		GUIButton("controls_back", "#controls_back",	ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
			GUIAction(GUI_ACTION_BACK|GUI_ACTION_APPLYOPT)

		break

	case "loadgame_menu":
		RemoveSaveEmptySlot()	//tu nam empty slot vadi tak ho vyhodime
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background
      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_loadgame", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
		GUISetFont(LargeFont)

		g_SaveSelected = GetLastSavedSlot()		//kurzor bude na poslednom sejvnutom slote
		
		if(g_SaveSelected == -1)					//ak tam neni ziadny sejv
			g_SaveSelected = GetEmptySaveSlot()

		ShowSavesListbox(true)

      ButtonPos += ButtonOff * 8

		GUIButton("loadgame_load", "#loadgame_load", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		    ButtonPos += ButtonOff

		GUIButton("loadgame_delete", "#loadgame_delete", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		    ButtonPos += ButtonOff
		    
		GUIButton("loadgame_back", "#loadgame_back", ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
		    GUIAction(GUI_ACTION_BACK)

		break

	case "savegame_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background
      // section title
      GUISetFont(MainFont)
      GUITitle("#savegame_save"/*"#menutitle_savegame"*/, ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
		GUISetFont(LargeFont)

		g_SaveSelected = GetEmptySaveSlot()
		ShowSavesListbox(true)
      ButtonPos += ButtonOff * 8

		GUIButton("savegame_save", "#savegame_save", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		    ButtonPos += ButtonOff

		GUIButton("savegame_delete", "#savegame_delete", ButtonLeftPos, ButtonPos, 400, 32, 1, 0, GUIInactiveColor)
		    ButtonPos += ButtonOff

		GUIButton("savegame_back", "#savegame_back", ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
		    GUIAction(GUI_ACTION_BACK)

		break
 
 #ifdef DEMO
	case "newgame_menu2":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background
		
      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_newgame", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
		GUISetFont(LargeFont)

		GUIImageButton("newgame_l01", "levels/l01_demo", 46, 208, 330, 304, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
			GUIHint("#levelname_01")
			GUIHintInfo("")

		GUIImageButton("newgame_l03", "levels/l03_demo", 421, 208, 330, 304, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_03")
			GUIHintInfo("")
		break
#endif

	case "newgame_menu":
		GUIImage("menubackground", 0, 0, 800, 600, 1, WF_NOWRAP|WF_SOURCEALPHA|WF_BLEND, 0xffffffff)	// menu background
		
      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_newgame", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)
		GUISetFont(LargeFont)

		GUIButton("newgame_easy", "#newgame_easy",	275, 220, 250, 32, 1, WF_CENTER, GUIInactiveColor)
		GUIButton("newgame_normal", "#newgame_normal",	275, 250, 250, 32, 1, WF_CENTER, GUIInactiveColor)
		GUIButton("newgame_hard", "#newgame_hard",	275, 280, 250, 32, 1, WF_CENTER, GUIInactiveColor)

/*
      // section title
      GUISetFont(MainFont)
      GUITitle("#menutitle_newgame", ButtonLeftPos, 140, 400, 32, 1, 0, 0xff0e2b2d)

		GUISetFont(LargeFont)

      int leftpos = ButtonLeftPos - 35
		GUIImageButton("newgame_l00", "levels/l00", leftpos, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
			GUIHint("#levelname_00")
			GUIHintInfo("")

		GUIImageButton("newgame_l01", "levels/l01", leftpos+70, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_01")
			GUIHintInfo("")

		GUIImageButton("newgame_l02", "levels/l02", leftpos+140, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_02")
			GUIHintInfo("")

		GUIImageButton("newgame_l03", "levels/l03", leftpos+210, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_03")
			GUIHintInfo("")

		GUIImageButton("newgame_l04", "levels/l04", leftpos+280, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_04")
			GUIHintInfo("")

		GUIImageButton("newgame_l05", "levels/l05", leftpos+350, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_05")
			GUIHintInfo("")

		GUIImageButton("newgame_l06", "levels/l06", leftpos+420, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_06")
			GUIHintInfo("")

		GUIImageButton("newgame_l07", "levels/l07", leftpos+490, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_07")
			GUIHintInfo("")
			
		GUIImageButton("newgame_l08", "levels/l08", leftpos+560, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_08")
			GUIHintInfo("")
			
		GUIImageButton("newgame_l09", "levels/l09", leftpos+630, 200, 80, 310, 1, WF_SOURCEALPHA | WF_BLEND, GUIInactiveColorImage)
		   GUIHint("#levelname_09")
			GUIHintInfo("")
*/
/*			
		GUIDisableItem("newgame_l00")
		GUIDisableItem("newgame_l01")
		GUIDisableItem("newgame_l02")
		GUIDisableItem("newgame_l03")
		GUIDisableItem("newgame_l04")
		GUIDisableItem("newgame_l05")
		GUIDisableItem("newgame_l06")
		GUIDisableItem("newgame_l07")
		GUIDisableItem("newgame_l08")
		GUIDisableItem("newgame_l09")
*/
/*			
if(g_physx)
		{
			GUIImageButton("newgame_ageia", "levels/ageia", 280, 355, 230, 180, 1, WF_SOURCEALPHA | WF_BLEND)
		}
*/		
		GUIButton("newgame_back", "#newgame_back", ButtonLeftPos, 540, 400, 32, 1, 0, GUIInactiveColor)
		GUIAction(GUI_ACTION_BACK)

		break

	case "credits_menu":
		InitCredits()
		ShowWidget(WMousePointer, false)

		GUIButton("options_back", "#options_back", 0, -32, 800, 632, 0x00ffffff, WF_CENTER)
			GUIAction(GUI_ACTION_BACK)
	break
		
	default:
		return false
	}
	
	MenuActive = menu

	GUISetMenuItems()
	if(ActButton == "")
		ActButton = GUIItemIDs[0]

	GUIActivateItem(ActButton, 1)		
	
	ClearMousePress(0)
	return true
}

//-----------------------------------------------------------------
int PopMenu(bool apply)
{
	CloseMenu(MenuActive)
	
	if(MenuStackOff >= 1)
	{
		MenuStackOff--
		ActButton = MenuStack[MenuStackOff][1]
		GUICleanup()
		
		if(MenuStack[MenuStackOff][0])
			OpenMenu(MenuStack[MenuStackOff][0])

		if(apply == false)
		{
			for(int n = 0; n < 64; n++)
				ItemValues[n] = ItemValuesBak[n]
		}
	}

	return MenuStackOff
}

//-----------------------------------------------------------------
void PushMenu(string menu)
{
	MenuStack[MenuStackOff][0] = MenuActive
	MenuStack[MenuStackOff][1] = ActButton
	MenuStackOff++
	ActButton = ""

#ifdef _XBOX
	ClearJoyButton(JB_A)
	ClearJoyButton(JB_B)
#else
	ClearKey(KC_RETURN)
	ClearKey(KC_ESCAPE)
	ClearMousePress(0)
#endif

	GUICleanup()
	if(!OpenMenu(menu))
	{
		PopMenu(true)
		return
	}

	for(int n = 0; n < 64; n++)
		ItemValuesBak[n] = ItemValues[n]
}


//-----------------------------------------------------------------
bool GUIDoAction(string item)
{
	int index = GUIGetItemIndex(item)
	//custom prvek
	if(index == -1)
		return false
		
	if(GUIItemActions[index] & GUI_ACTION_GETOPT)
	{
		GetOptions()
	}	

	if(GUIItemActions[index] & GUI_ACTION_APPLYOPT)
	{
		ApplyOptions()
		SaveConfig()
	}	

	if(GUIItemActions[index] & GUI_ACTION_BACK)
	{
		g_MenuFlash = true
		PlayMenuSound(2)
		PopMenu(true)
		return true
	}
	
	if(GUIItemActions[index] & GUI_ACTION_MENU)
	{
		g_MenuFlash = true
		PlayMenuSound(2)
		PushMenu(GUIItemActionParms[index])
		return true
	}
	
	if(GUIItemActions[index] & GUI_ACTION_CALL)
	{
		Call(NULL, GUIItemActionParms[index], "")
		return true
	}
	
	return false
}

//-----------------------------------------------------------------
string GUIUpdateMenu()
{
	if(ListKeyInput != 0)
	{
		UpdateMouse()
		return ""
	}
	
	if(ConfDialog)
		return ConfDialog.Update()

#ifdef _XBOX
	if(JoyButtonState(JB_A))
	{
		ClearJoyButton(JB_A)
#else
	if(KeyState(KC_RETURN))
	{
		ClearKey(KC_RETURN)
#endif
		if(GUIDoAction(ActButton))
			return ""

		SwitchMenuItem(ActButton, 1)
		return ActButton
	}

#ifdef _XBOX
	if(JoyButtonState(JB_B))
	{
		ClearJoyButton(JB_B)
#else
	if(KeyState(KC_ESCAPE))
	{
#endif
		ClearKey(KC_ESCAPE)
		PlayMenuSound(4)
		return "_ESCAPE"
	}

	UpdateMouse()

#ifdef _XBOX
	if(JoyButtonState(JB_DPAD_RIGHT))
	{
		ClearJoyButton(JB_DPAD_RIGHT)
#else
	if( KeyState(KC_RIGHT))
	{
#endif
		ClearKey(KC_RIGHT)
		
		if(!GUINextItem(1, 0))
			SwitchMenuItem(ActButton, 1)
		return ""
	}

#ifdef _XBOX
	if(JoyButtonState(JB_DPAD_LEFT))
	{
		ClearJoyButton(JB_DPAD_LEFT)
#else
	if( KeyState(KC_LEFT))
	{
#endif
		ClearKey(KC_LEFT)
		if(!GUINextItem(-1, 0))
			SwitchMenuItem(ActButton, -1)
		return ""
	}

#ifdef _XBOX
	if(JoyButtonState(JB_DPAD_UP))
	{
		ClearJoyButton(JB_DPAD_UP)
#else
	if( KeyState(KC_UP) == true)
	{
#endif
		ClearKey(KC_UP)

		GUINextItem(0, -1)
		return ""
	}

#ifdef _XBOX
	if(JoyButtonState(JB_DPAD_DOWN))
	{
		ClearJoyButton(JB_DPAD_DOWN)
#else
	if( KeyState(KC_DOWN) == true)
	{
		ClearKey(KC_DOWN)
#endif
		GUINextItem(0, 1)
		return ""
	}

#ifndef _XBOX
	if(MousePress(0))
	{
		int n = GUICheckMouse()
		ActButton = ""
		if(n != -1)
		{
			ActButton = GUIItemIDs[n]
			
			if(GUIDoAction(ActButton))
				return ""
				
			SwitchMenuItem(ActButton, 0)
		}
		return ActButton
	}

	if(MouseX == LMouseX)
	{
		if(MouseY == LMouseY)
			return ""
	}

	LMouseX = MouseX
	LMouseY = MouseY

	GUICheckMouse()
#endif

	return ""
}

