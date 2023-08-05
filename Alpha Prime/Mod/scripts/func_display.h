
const int DSP_INTERP_NONE = 0
const int DSP_INTERP_DIRECTLY = 1
const int DSP_INTERP_LINEAR = 2
const int DSP_INTERP_SPLINE1 = 3
const int DSP_INTERP_SPLINE2 = 4
const int DSP_INTERP_SPLINE3 = 5

const int DSP_TYPE_BITMAP = 0
const int DSP_TYPE_FONT	 = 1
const int DSP_TYPE_VIDEO = 2

const int DSP_LINESTARTOFF = 1	//flagy
const int DSP_LINEUPDATEALLWAYS = 2	//ked tento flag neni nastaveny, timelajna sa updatuje iba ked je model vidiet

const string ValFormat[3][8] = {"xpos", "ypos", "width", "height", "angle", "opacity", "", "",			//bitmap
											"xpos", "ypos", "width", "height", "angle", "opacity", "text", "",	//font
											"xpos", "ypos", "width", "height", "angle", "opacity", "", ""}			//video

//=================================================================================
class DElmKey extends _serializable
{
	DElmKey NextKey
	int interp
	float time
}

class DElmKeyFloat extends DElmKey
{
	float fval
}

class DElmKeyString extends DElmKey
{
	string sval
}

//=================================================================================
class DElm extends _serializable
{
	string ID
	int type
	string path
	int font
	int Sort
	int wflags
	DElm NextElement
	float OriginX	//0 - 1 offset k lavemu hornemu rohu bitmapy
	float OriginY
	int	color
	string ParentID
}

//==================================================================
class DCommand extends _serializable
{
	DCommand NextCommand
	string command
	float time
}

//==================================================================
class DSound extends _serializable
{
	DSound NextSound
	string ID
	hsound shandle
}

//==================================================================
class UsingInfo extends _serializable
{
	DElm NextUsingInfo
	DElm elm

	DElmKeyFloat FirstXposKey		//prvy kluc
	DElmKeyFloat FirstYposKey
	DElmKeyFloat FirstWidthKey
	DElmKeyFloat FirstHeightKey
	DElmKeyFloat FirstAngleKey
	DElmKeyFloat FirstOpacityKey
	DElmKeyFloat FirstTextKey

//------------------------------------------------------------------
	DElmKey AddKey(DElmKey key, string KeyName)
	{
		DElmKey fkey = NULL
		DElmKey LastFkey = NULL
		
		if(KeyName == "xpos")
		{
			if(!FirstXposKey)
				FirstXposKey = key

			fkey = FirstXposKey
		}

		if(KeyName == "ypos")
		{
			if(!FirstYposKey)
				FirstYposKey = key

			fkey = FirstYposKey
		}

		if(KeyName == "width")
		{
			if(!FirstWidthKey)
				FirstWidthKey = key

			fkey = FirstWidthKey
		}
			
		if(KeyName == "height")
		{
			if(!FirstHeightKey)
				FirstHeightKey = key

			fkey = FirstHeightKey
		}

		if(KeyName == "angle")
		{
			if(!FirstAngleKey)
				FirstAngleKey = key

			fkey = FirstAngleKey
		}

		if(KeyName == "opacity")
		{
			if(!FirstOpacityKey)
				FirstOpacityKey = key

			fkey = FirstOpacityKey
		}

		if(KeyName == "text")
		{
			if(!FirstTextKey)
				FirstTextKey = key

			fkey = FirstTextKey
		}

		if(fkey == key)	//iba sme nastavili first
			return NULL

		while(true)
		{
			LastFkey = fkey
			fkey = fkey.NextKey
			
			if(!fkey)
			{
				LastFkey.NextKey = key
				return LastFkey
			}
		}
	}

//------------------------------------------------------------------
	void DestroyKeyChain(DElmKey FKey)
	{
		DElmKey key = FKey
		DElmKey tmp

		while(key)
		{
			tmp = key
			key = key.NextKey
			delete tmp
		}
	}

//------------------------------------------------------------------
	void ~UsingInfo()
	{
		DestroyKeyChain(FirstXposKey)
		DestroyKeyChain(FirstYposKey)
		DestroyKeyChain(FirstWidthKey)
		DestroyKeyChain(FirstHeightKey)
		DestroyKeyChain(FirstAngleKey)
		DestroyKeyChain(FirstOpacityKey)
		DestroyKeyChain(FirstTextKey)	
	}
}

//==================================================================
class DTLine extends _serializable
{
	string ID
	int flags
	DTLine NextTLine
	UsingInfo FirstUsingInfo
	DCommand FirstCommand
	float FullTime

//------------------------------------------------------------------
	void AddUsingInfo(UsingInfo info)
	{
		if(!FirstUsingInfo)
			FirstUsingInfo = info
		
		UsingInfo inf = FirstUsingInfo
		UsingInfo LastDefined = NULL

		while(inf)
		{
			if(inf != info)
				LastDefined = inf

			inf = inf.NextUsingInfo
		}

		inf = info

		if(LastDefined)
			LastDefined.NextUsingInfo = info
	}
	
//------------------------------------------------------------------
	void AddCommand(DCommand cmd)
	{
		if(!FirstCommand)
			FirstCommand = cmd
		
		DCommand c = FirstCommand
		DCommand LastDefined = NULL

		while(c)
		{
			if(c != cmd)
				LastDefined = c

			c = c.NextCommand
		}

		c = cmd

		if(LastDefined)
			LastDefined.NextCommand = cmd
	}

//------------------------------------------------------------------
	DElm GetUsingInfoPtr(string id)
	{
		UsingInfo UInfo = FirstUsingInfo

		while(UInfo)
		{
			if(UInfo.elm.ID == id)
				return UInfo

			UInfo = UInfo.NextUsingInfo
		}
		return NULL
	}

//------------------------------------------------------------------
	void ~DTLine()
	{
		UsingInfo inf = FirstUsingInfo
		UsingInfo tmp

		while(inf)
		{
			tmp = inf
			inf = inf.NextUsingInfo
			delete tmp
		}

		DCommand cmd = FirstCommand
		DElm tmp2

		while(cmd)
		{
			tmp2 = cmd
			cmd = cmd.NextCommand
			delete tmp2
		}
	}
}

//=================================================================================
class DispAction extends _serializable
{
	string ID
	DispAction NextAction
	DCommand FirstCommand

//------------------------------------------------------------------
	void AddCommand(DCommand cmd)
	{
		if(!FirstCommand)
			FirstCommand = cmd
		
		DCommand c = FirstCommand
		DCommand LastDefined = NULL

		while(c)
		{
			if(c != cmd)
				LastDefined = c

			c = c.NextCommand
		}

		c = cmd

		if(LastDefined)
			LastDefined.NextCommand = cmd
	}

//------------------------------------------------------------------
	void ~DispAction()
	{
		DCommand cmd = FirstCommand
		DCommand tmp

		while(cmd)
		{
			tmp = cmd
			cmd = cmd.NextCommand
			delete tmp
		}
	}
}

//=================================================================================
class DispProgram extends _serializable
{
	string ID
	DispProgram NextProgram
	DTLine FirstTLine
	DElm FirstElement
	DispAction FirstAction
	DSound FirstSound
	int CanvasX, CanvasY
	float fps	//virtualne fps na ktorom program pobezi
	int CanvasFlags

//------------------------------------------------------------------
	void AddAction(DispAction action)
	{
		if(!FirstAction)
			FirstAction = action
			
		DispAction st = FirstAction
		DispAction LastDefined = NULL

		while(st)
		{
			if(st != action)
				LastDefined = st

			st = st.NextAction
		}

		st = action
		
		if(LastDefined)
			LastDefined.NextAction = action
	}

//------------------------------------------------------------------
	void AddSound(DSound snd)
	{
		if(!FirstSound)
			FirstSound = snd
			
		DSound sn = FirstSound
		DSound LastDefined = NULL

		while(sn)
		{
			if(sn != snd)
				LastDefined = sn

			sn = sn.NextSound
		}

		sn = snd
		
		if(LastDefined)
			LastDefined.NextSound = snd
	}

//------------------------------------------------------------------
	void AddTimeline(DTLine tline)
	{
		if(!FirstTLine)
			FirstTLine = tline
			
		DTLine tl = FirstTLine
		DTLine LastDefined = NULL

		while(tl)
		{
			if(tl != tline)
				LastDefined = tl

			tl = tl.NextTLine
		}

		tl = tline
		
		if(LastDefined)
			LastDefined.NextTLine = tline
	}

//------------------------------------------------------------------
	DElm GetElementPtr(string id)
	{
		DElm elm = FirstElement

		while(elm)
		{
			if(elm.ID == id)
				return elm

			elm = elm.NextElement
		}
		return NULL
	}

//------------------------------------------------------------------
	void AddElement(DElm element)
	{
		if(!FirstElement)
			FirstElement = element
		
		DElm elm = FirstElement
		DElm LastDefined = NULL

		while(elm)
		{
			if(elm != element)
				LastDefined = elm

			elm = elm.NextElement
		}

		elm = element

		if(LastDefined)
			LastDefined.NextElement = element
	}

//------------------------------------------------------------------
	void DispProgram()
	{
		CanvasX = 800	//default keby niekto nenastavil nic aby bolo nieco vidiet
		CanvasY = 600
	}

//------------------------------------------------------------------
	void ~DispProgram()
	{
		DTLine tline = FirstTLine
		DTLine tmp

		while(tline)
		{
			tmp = tline
			tline = tline.NextTLine
			delete tmp
		}

		DElm elm = FirstElement
		DElm tmp3

		while(elm)
		{
			tmp3 = elm
			elm = elm.NextElement
			delete tmp3
		}

		DispAction st = FirstAction
		DispAction tmp4

		while(st)
		{
			tmp4 = st
			st = st.NextAction
			delete tmp4
		}

		DSound sn = FirstSound
		DSound tmp5

		while(sn)
		{
			tmp5 = sn
			sn = sn.NextSound
			delete tmp5
		}
	}
}


//=================================================================================
class DisplayCollection extends _serializable
{
	DispProgram FirstProgram

//------------------------------------------------------------------
	int ConvertInterpType(string s)
	{
		if(s == ".")
			return DSP_INTERP_DIRECTLY

		if(s == "1")
			return DSP_INTERP_LINEAR

		if(s == "2")
			return DSP_INTERP_SPLINE1

		if(s == "3")
			return DSP_INTERP_SPLINE2

		if(s == "4")
			return DSP_INTERP_SPLINE3
		
		return DSP_INTERP_LINEAR
	}

//------------------------------------------------------------------
	void AddProgram(DispProgram prg)
	{
		if(!FirstProgram)
			FirstProgram = prg
		
		DispProgram dp = FirstProgram
		DispProgram LastDefined = NULL

		while(dp)
		{
			if(dp != prg)
				LastDefined = dp

			dp = dp.NextProgram
		}

		dp = prg

		if(LastDefined)
			LastDefined.NextProgram = prg
	}

//------------------------------------------------------------------
	DispProgram LoadProgram(string prg)
	{
		DispProgram prog = FirstProgram

		while(prog)
		{
			if(prog.ID == prg)
				return prog

			prog = prog.NextProgram
		}

		tparse tp = BeginParse(prg)

		if(!tp)
			return NULL

		prog = new DispProgram
		prog.ID = prg
		AddProgram(prog)

		string tokens[32]
		int n = 0
		int k, r, g, b
		int BlendType
		int TextureAlpha
		int InterpOff
		int MaxKeys
		string VarName
		bool Commands = false
		DElm elm = NULL
		DTLine tline = NULL
		DCommand cmd = NULL
		DSound snd = NULL
		DispAction action = NULL
		DElmKeyFloat Key = NULL
		DElmKeyFloat KeyLastDefined = NULL
		DElmKeyString KeyString = NULL
		UsingInfo uinfo = NULL
		int NumTokens = ParseLine(tp, n++, tokens)

		while(NumTokens)
		{
			if(tokens[0] == "CANVAS")
			{
				prog.CanvasX = atoi(tokens[1])
				prog.CanvasY = atoi(tokens[2])
				prog.fps = atof(tokens[3])
				
				if(tokens[4] != "alpha")
					prog.CanvasFlags |= WF_NOALPHA

				if(prog.fps == 0)
					prog.fps = 30

				goto next
			}

			if(tokens[0] == "BITMAP" || tokens[0] == "TEXT" || tokens[0] == "VIDEO")
			{
				elm = new DElm
				prog.AddElement(elm)
				elm.ID = tokens[1]
				elm.ParentID = tokens[2]
				
				if(tokens[0] == "BITMAP")
					elm.type = DSP_TYPE_BITMAP
				else
				{
					if(tokens[0] == "TEXT")
						elm.type = DSP_TYPE_FONT
					else
						elm.type = DSP_TYPE_VIDEO
				}
					
				elm.path = RemoveQuotes(tokens[3])
				elm.Sort = atoi(tokens[4])
			
				if(tokens[5] == "stretch")
					elm.wflags |= WF_STRETCH

				if(tokens[5] == "rotate")
					elm.wflags |= WF_CUSTOMUV | WF_NOWRAP

				if(tokens[6] == "0")
					elm.wflags |= WF_NOFILTER

				elm.OriginX = atof(tokens[7])
				elm.OriginY = atof(tokens[8])
				int FlipU = atoi(tokens[9])
				int FlipV = atoi(tokens[10])
				
				if(FlipU)
					elm.wflags |= WF_FLIPU

				if(FlipV)
					elm.wflags |= WF_FLIPV

				BlendType = atoi(tokens[11])
				TextureAlpha = atoi(tokens[12])
				clamp BlendType<0, 2>
				
				if(BlendType == 1)
					elm.wflags |= WF_BLEND

				if(BlendType == 2)
					elm.wflags |= WF_ADDITIVE
				
				if(BlendType && TextureAlpha)
					elm.wflags |= WF_SOURCEALPHA
				
				r = atoi(tokens[13])
				g = atoi(tokens[14])
				b = atoi(tokens[15])
				elm.color = ARGB(0, r, g, b)

				if(tokens[0] == "TEXT")
				{
					elm.font = atoi(tokens[16])
	
					if(tokens[17] == "right")
						elm.wflags |= WF_RALIGN
	
					if(tokens[17] == "center")
						elm.wflags |= WF_CENTER
	
					if(tokens[18] == "center")
						elm.wflags |= WF_VCENTER
				}

				elm = NULL
				goto next
			}

			if(tokens[0] == "SOUND")
			{
				snd = new DSound
				prog.AddSound(snd)
				snd.ID = tokens[1]
				snd.shandle = GetSoundEx(RemoveQuotes(tokens[2]), 1, SF_3D)
				snd = NULL
				goto next
			}

			if(tokens[0] == "USING")
			{
				uinfo = new UsingInfo
				tline.AddUsingInfo(uinfo)
				uinfo.elm = prog.GetElementPtr(tokens[1])
				uinfo = NULL
				goto next
			}

			if(tokens[0] == "TIMELINE")
			{
				elm = NULL
				Commands = false
				tline = new DTLine
				prog.AddTimeline(tline)
				tline.ID = tokens[1]
				
				if(tokens[2] == "OFF")
					tline.flags |= DSP_LINESTARTOFF

				goto next
			}

			if(tokens[0] == "COMMANDS")
			{
				elm = NULL
				Commands = true
				action = NULL
				goto next
			}

			if(tokens[0] == "ACTION")
			{
				elm = NULL
				Commands = false
				action = new DispAction
				prog.AddAction(action)
				action.ID = tokens[1]
				goto next
			}

			if(action)
			{
				cmd = new DCommand
				action.AddCommand(cmd)
				cmd.time = -1	//podla toho spozname statecommand
				
				for(k = 0; k < NumTokens; k++)
					cmd.command += tokens[k] + " "

				goto next
			}

			if(Commands)
			{
				cmd = new DCommand
				tline.AddCommand(cmd)
				cmd.time = atof(tokens[0])
				
				if(tokens[1] != "JUMP")
					tline.flags |= DSP_LINEUPDATEALLWAYS	//iny command nez JUMP znamena ze v displeji musi bezat OnFrame event koli korektnym casom (zvuky a pod)

				for(k = 1; k < NumTokens; k++)
					cmd.command += tokens[k] + " "

				goto next
			}

			if(tokens[0] == "KEYS")
			{
				Commands = false
				uinfo = tline.GetUsingInfoPtr(tokens[1])
				goto next
			}

			if(uinfo)		//riadok ktory definuje kluce
			{
				if(uinfo.elm.type == DSP_TYPE_BITMAP)
				{
					MaxKeys = 6							//kolko to ma klucov
					InterpOff = 7						//token s tymto indexom je inerpolacia pre xpos
				}

				if(uinfo.elm.type == DSP_TYPE_FONT)
				{
					MaxKeys = 7
					InterpOff = 8
				}

				float ActTime = atof(tokens[0])

				if(ActTime > tline.FullTime)
					tline.FullTime = ActTime

				
				for(k = 0; k < MaxKeys; k++)
				{
					if(tokens[k + 1] != ".")		//token1. ma sa vytvorit nejaky kluc
					{
						VarName = ValFormat[uinfo.elm.type][k]
						
						if(VarName == "text")
							Key = new DElmKeyString
						else
							Key = new DElmKeyFloat

						KeyLastDefined = uinfo.AddKey(Key, VarName)
						Key.time = ActTime
						Key.interp = ConvertInterpType(tokens[k + InterpOff])
	
						if(tokens[k + 1] == "x")	//ma to byt kopia posledneho
						{
							if(KeyLastDefined)
							{
								if(VarName == "text")
								{
									KeyString = KeyLastDefined
									string sv = KeyString.sval
									KeyString = Key
									KeyString.sval = sv
								}
								else
									Key.fval = KeyLastDefined.fval
							}
						}
						else
						{
							if(VarName == "text")
							{
								KeyString = Key
								KeyString.sval = tokens[k + 1]
							}
							else
								Key.fval = atof(tokens[k + 1])
						}
					}
				}
				goto next
			}
next:
			NumTokens = ParseLine(tp, n++, tokens)
		}

		EndParse(tp)
		return prog
	}

//------------------------------------------------------------------
	void ~DisplayCollection()
	{
		DispProgram prg = FirstProgram
		DispProgram tmp

		while(prg)
		{
			tmp = prg
			prg = prg.NextProgram
			delete tmp
		}
	}
}

//=================================================================================
class DWidgetStorage extends _serializable
{
	DWidgetStorage NextWStorage
	widget wdg
	DElm DefElm
	float CurXpos, CurYpos, CurWidth, CurHeight
	string CurString

	void ~DWidgetStorage()
	{
		if(wdg)
		{
			DestroyWidget(wdg)
			wdg = NULL
		}
	}
}

//=================================================================================
class DWidgetContainer extends _serializable		//pre ucely ked z inej entity pridavame do displeja widgety. aby sa mazali predtym nez sa zmaze hlavny parent widget
{
	DWidgetContainer next
	widget wdg

	void ~DWidgetContainer()
	{
		if(wdg)
		{
			DestroyWidget(wdg)
			wdg = NULL
		}
	}
}

//=================================================================================
class DElmHelp extends _serializable
{
	DWidgetStorage storage
	UsingInfo DefInfo		//pointer na definiciu elementu
	DElmHelp NextElmHelp
	DElmKeyFloat CurXposKey
	DElmKeyFloat CurYposKey
	DElmKeyFloat CurWidthKey
	DElmKeyFloat CurHeightKey
	DElmKeyFloat CurAngleKey
	DElmKeyFloat CurOpacityKey
	DElmKeyString CurTextKey
	bool visible
}

//=================================================================================
class DTLineHelp extends _serializable
{
	DTLineHelp NextLineHelp
	DElmHelp	FirstElmHelp
	DTLine	DefLine	//pointer na definiciu
	DCommand ActCommand
	float CurTime
	int	status
	int	flags
}

const int DSP_STOP = 0
const int DSP_PLAY = 1

float DisplaySkip

//==================================================================
class func_display extends ExtClass
{
	int flags
	string program
	DispProgram prg
	widget CanvasWdg
	bool OnScreen
	int status
	DTLineHelp		FirstTLineHelp
	DWidgetStorage	FirstWStorage
	DWidgetContainer FirstExternWidget	//ked mame display a pridavame k nemu z nejakej inej entity widget
	int LastUpdateFrame
	int LastUpdatedFrame

	float skipcount
	float skiptime

//------------------------------------------------------------------
	void M_Enable(ExtClass other)
	{
		flags &= -1 ^ 1
	}

//------------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		flags |= 1
	}

//------------------------------------------------------------------
	void AddWidgetStorage(DWidgetStorage wid)
	{
		wid.NextWStorage = FirstWStorage
		FirstWStorage = wid
	}

//------------------------------------------------------------------
	void AddExternWidget(widget wdg)
	{
		DWidgetContainer cont = new DWidgetContainer
		cont.wdg = wdg
		cont.next = FirstExternWidget
		FirstExternWidget = cont
	}

//------------------------------------------------------------------
	DWidgetStorage GetWidgetStorage(string ID)
	{
		for(DWidgetStorage st = FirstWStorage; st != NULL; st = st.NextWStorage)
		{
			if(st.DefElm.ID == ID)
				return st
		}
		return NULL
	}

//------------------------------------------------------------------
	DElmHelp GetTimeLineHelpPtr(string id)
	{
		DTLineHelp TlineHlp = FirstTLineHelp

		while(TlineHlp)
		{
			if(TlineHlp.DefLine.ID == id)
				return TlineHlp

			TlineHlp = TlineHlp.NextLineHelp
		}
		return NULL
	}

//------------------------------------------------------------------
	DElmHelp GetElementHelpPtr(string id)
	{
		DTLineHelp TlineHlp = FirstTLineHelp

		while(TlineHlp)
		{
			DElmHelp ElmHelp = TlineHlp.FirstElmHelp
		
			while(ElmHelp)
			{
				if(ElmHelp.storage.DefElm.ID == id)
					return ElmHelp
	
				ElmHelp = ElmHelp.NextElmHelp
			}

			TlineHlp = TlineHlp.NextLineHelp
		}
		return NULL
	}

//------------------------------------------------------------------
	void SetTimeLinePos(DTLineHelp TlineHlp, float Time, bool WriteToWigets)
	{
		TlineHlp.CurTime = Time
		TlineHlp.ActCommand = TlineHlp.DefLine.FirstCommand
		DCommand cmd = TlineHlp.DefLine.FirstCommand

		while(cmd)	//nastavime ActCommand na najblizsi ktory je na rade
		{
			if(cmd.time >= Time)
			{
				TlineHlp.ActCommand = cmd
				break
			}
			cmd = cmd.NextCommand
		}
		
		UsingInfo inf
		DElmHelp ElmHelp = TlineHlp.FirstElmHelp
		DElmKey key
		
		while(ElmHelp)
		{
			inf = ElmHelp.DefInfo

			key = inf.FirstXposKey
			ElmHelp.CurXposKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurXposKey = key
				key = key.NextKey	
			}

			key = inf.FirstYposKey
			ElmHelp.CurYposKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurYposKey = key
				key = key.NextKey	
			}

			key = inf.FirstWidthKey
			ElmHelp.CurWidthKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurWidthKey = key
				key = key.NextKey	
			}

			key = inf.FirstHeightKey
			ElmHelp.CurHeightKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurHeightKey = key
				key = key.NextKey	
			}

			key = inf.FirstAngleKey
			ElmHelp.CurAngleKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurAngleKey = key
				key = key.NextKey	
			}

			key = inf.FirstOpacityKey
			ElmHelp.CurOpacityKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurOpacityKey = key
				key = key.NextKey	
			}

			key = inf.FirstTextKey
			ElmHelp.CurTextKey = key
			while(key && key.time <= Time)
			{
				ElmHelp.CurTextKey = key
				key = key.NextKey	
			}

//---------------- nastavime hodnoty klucov do widgetov ---------------------
			if(WriteToWigets)
			{
				ShowWidget(ElmHelp.storage.wdg, ElmHelp.visible)	//musime updatovat viditelnost

				if(ElmHelp.CurTextKey)	//nemusi mat ani prvy kluc
				{
					DElmKeyString StrKey = ElmHelp.CurTextKey
					ElmHelp.storage.CurString = StrKey.sval
					SetWidgetTextEx(ElmHelp.storage.wdg, 0, 1, "#" + StrKey.sval)
				}
	
				if(ElmHelp.CurXposKey && ElmHelp.CurYposKey && ElmHelp.CurWidthKey && ElmHelp.CurHeightKey)	//prve kluce pozicii a rozmerov su v postate povinne aby to malo ine rozmery nez nulove
				{
					float offsetx = fabs(ElmHelp.storage.CurWidth) * inf.elm.OriginX
					float offsety = fabs(ElmHelp.storage.CurHeight) * inf.elm.OriginY
					ElmHelp.storage.CurXpos = ElmHelp.CurXposKey.fval
					ElmHelp.storage.CurYpos = ElmHelp.CurYposKey.fval
					ElmHelp.storage.CurWidth = ElmHelp.CurWidthKey.fval
					ElmHelp.storage.CurHeight = ElmHelp.CurHeightKey.fval
					SetWidgetPos(ElmHelp.storage.wdg, ElmHelp.storage.CurXpos - offsetx, ElmHelp.storage.CurYpos - offsety, ElmHelp.storage.CurWidth, ElmHelp.storage.CurHeight)	// a konecne aplikujeme interpolovane hodnoty
				}
				
				if(ElmHelp.CurOpacityKey)
				{
					int col = inf.elm.color
					col |= (int)ElmHelp.CurOpacityKey.fval << 24
		
					if(inf.elm.type == DSP_TYPE_FONT)					//u textovych widgetov menime farbu textu a u zbytku farbu pozadia
						SetWidgetTextColor(ElmHelp.storage.wdg, col)
					else
						SetWidgetColor(ElmHelp.storage.wdg, col)
				}
	
				if(ElmHelp.CurAngleKey)
				{
					float UVs[8]
					GetRotatedSpriteUVs(UVs, ElmHelp.CurAngleKey.fval)
					SetWidgetUV(ElmHelp.storage.wdg, UVs)
				}
			}

			ElmHelp = ElmHelp.NextElmHelp
		}
	}

//------------------------------------------------------------------
	void ResetToStartPos()
	{
		DTLineHelp TlineHlp = FirstTLineHelp

		while(TlineHlp)
		{
			bool UpdateWidgetsUsingTimeline = false

			if(!TlineHlp.DefLine.flags & DSP_LINESTARTOFF)	//moze byt na zaciatku vypnuta
			{
				TlineHlp.status = 1
				UpdateWidgetsUsingTimeline = true
			}
				
			SetTimeLinePos(TlineHlp, 0, UpdateWidgetsUsingTimeline)

			DElmHelp ElmHlp = TlineHlp.FirstElmHelp
			while(ElmHlp)
			{
				if(ElmHlp.DefInfo.elm.type == DSP_TYPE_VIDEO)
				{
					PlayVideo(ElmHlp.storage.wdg, VP_PLAY)
					PlayVideo(ElmHlp.storage.wdg, VP_REPEAT)
				}
				ElmHlp = ElmHlp.NextElmHelp
			}
			TlineHlp = TlineHlp.NextLineHelp
		}
	}

//------------------------------------------------------------------
	void OnStopProgram()
	{
		DTLineHelp TlineHlp = FirstTLineHelp

		while(TlineHlp)
		{
			SetTimeLinePos(TlineHlp, 0, false)
			
			DElmHelp ElmHlp = TlineHlp.FirstElmHelp
			while(ElmHlp)
			{
				if(ElmHlp.DefInfo.elm.type == DSP_TYPE_VIDEO)
					PlayVideo(ElmHlp.storage.wdg, VP_STOP)

				ElmHlp = ElmHlp.NextElmHelp
			}
			TlineHlp = TlineHlp.NextLineHelp
		}
	}

//------------------------------------------------------------------
	bool NeedAnyTimelineUpdateAllways()
	{
		DTLineHelp TlineHlp = FirstTLineHelp
		bool NeedUpdateAllways = false

		while(TlineHlp)
		{
			if(TlineHlp.DefLine.flags & DSP_LINEUPDATEALLWAYS && TlineHlp.status != 0)	//ma sa updatovat vzdy pretoze je aktivna a ma nejake commandy okrem jumpu
			{
				NeedUpdateAllways = true
				break
			}

			TlineHlp = TlineHlp.NextLineHelp
		}
		return NeedUpdateAllways
	}

//------------------------------------------------------------------
	void M_Play(ExtClass other)
	{
		if(!prg)
			return

		status = DSP_PLAY
		
		if(NeedAnyTimelineUpdateAllways() == true)	//optimalizujeme. neupdatujeme cas z OnFrame ale iba z OnVisible ked v displeji nebezi nic doleziteho co musi prebiehat v spravnom case
			SetEventMask(this, EV_FRAME)
		else
			ClearEventMask(this, EV_FRAME)

		SetEventMask(this, EV_VISIBLE)
		ResetToStartPos()
	}

//------------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		if(!prg)
			return

		status = DSP_STOP
		ClearEventMask(this, EV_FRAME|EV_VISIBLE)
		OnStopProgram()
	}

//------------------------------------------------------------------
	void SetProgram()
	{
		if(!prg)
		{
			DPrint("Error loading program " + program + " to entity " + _name)
			return
		}

		skiptime = 1 / prg.fps
		int AddFlags = 0

		if(!CanvasWdg)	//hlavny widget
		{
			int WdgType = WTYPE_RTTEXTURE
			int WdgXpos = 0
			int WdgYpos = 0
	
			if(OnScreen == true)	//na obrazovku
			{
				WdgType = WTYPE_IMAGE
				
				if(ClassName(Owner) == "viewer")
				{
					WdgXpos = g_iWidth - prg.CanvasX / 2
					WdgYpos = g_iHeight - prg.CanvasY / 2
					AddFlags = WF_VISIBLE | WF_EXACTPOS | WF_EXACTSIZE
				}
				else
					AddFlags = WF_VISIBLE
			}
			else
				AddFlags = WF_VISIBLE | WF_EXACTPOS | WF_EXACTSIZE

			CanvasWdg = CreateWidget(WdgType, WdgXpos, WdgYpos, prg.CanvasX, prg.CanvasY, prg.CanvasFlags | AddFlags , 0x00000000, 0)

			if(CanvasWdg && Owner && OnScreen == false)
				SetGUIWidget(Owner, 0, CanvasWdg)	//!!!!!!!!!!!
		}

		DElm elm = prg.FirstElement
		DWidgetStorage wid
		
		while(elm)		//vytvorime schranky pre widgety aj s obsahom
		{
			wid = new DWidgetStorage
			wid.DefElm = elm
			AddWidgetStorage(wid)

			int wtype
			int Font = LargeFont
			
			if(elm.type == DSP_TYPE_BITMAP)
				wtype = WTYPE_IMAGE

			if(elm.type == DSP_TYPE_FONT)
				wtype = WTYPE_TEXT

			if(elm.type == DSP_TYPE_VIDEO)
				wtype = WTYPE_VIDEO

			widget WParent = NULL
			
			if(elm.ParentID == "none")
				WParent = CanvasWdg
			else
			{
				DWidgetStorage ws = GetWidgetStorage(elm.ParentID)
				if(ws)
					WParent = ws.wdg
			}
	
			if(!WParent)
				WParent = CanvasWdg

			if(OnScreen == false || ClassName(Owner) == "viewer")	//je to na entite v surface
				AddFlags = WF_EXACTPOS | WF_EXACTSIZE
			else			//je to na screene
				AddFlags = 0
				
			wid.wdg = CreateWidgetEx(WParent, wtype, 20, 20, 256, 256, elm.wflags | AddFlags , elm.color | 0xff000000, elm.Sort)

			if(wid.wdg)
			{
				if(elm.path)
					LoadWidgetImage(wid.wdg, 0, elm.path)

				if(elm.type == DSP_TYPE_FONT)
				{
					if(elm.font == 1)
						Font = LargeFont
					else
					{
						if(elm.font == 2)
							Font = MainFont
						else
							Font = SmallFont	
					}

					SetFont(wid.wdg, Font)
					SetWidgetTextEx(wid.wdg, 0, 1, "")
				}
				
/*				if(elm.wflags == WF_CUSTOMUV)
				{
					float UVs[8]
					GetRotatedSpriteUVs(UVs, 0)
					SetWidgetUV(wid.wdg, UVs)
				}*/
			}
			elm = elm.NextElement
		}

		DTLine tline = prg.FirstTLine
		DTLineHelp tlineHlp = NULL
		DTLineHelp TLineH = NULL

		while(tline)						//vytvorime pomocne instancie k definovanym timelajnam
		{
			tlineHlp = new DTLineHelp
			tlineHlp.DefLine = tline	//hned si ulozime definiciu ktora k tomu patri

			if(TLineH)
				TLineH.NextLineHelp = tlineHlp

			if(!FirstTLineHelp)
				FirstTLineHelp = tlineHlp

			UsingInfo uinfo = tline.FirstUsingInfo
			DElmHelp elmHlp = NULL
			DElmHelp ElmH = NULL
			DWidgetStorage st

			while(uinfo)					//pomocne instancie k elementom
			{
				elmHlp = new DElmHelp
				elmHlp.storage = GetWidgetStorage(uinfo.elm.ID)	//hned si ulozime definiciu ktora k tomu patri
				elmHlp.DefInfo = uinfo
				elmHlp.visible = true

				if(ElmH)
					ElmH.NextElmHelp = elmHlp
					
				if(!tlineHlp.FirstElmHelp)
					tlineHlp.FirstElmHelp = elmHlp

				ElmH = elmHlp
				uinfo = uinfo.NextUsingInfo
			}

			TLineH = tlineHlp
			tline = tline.NextTLine
		}
	}

//------------------------------------------------------------------	
	void RemoveProgram()
	{
		DTLineHelp tlineHlp = FirstTLineHelp
		DTLineHelp tmp

		DElmHelp ElmHlp
		DElmHelp etmp

		while(tlineHlp)
		{
			tmp = tlineHlp

			ElmHlp = tmp.FirstElmHelp
			etmp = NULL

			while(ElmHlp)
			{
				etmp = ElmHlp
				ElmHlp = ElmHlp.NextElmHelp
				delete etmp
			}

			tlineHlp = tlineHlp.NextLineHelp
			delete tmp
		}

		FirstTLineHelp = NULL
		prg = NULL
	}

//------------------------------------------------------------------
	void ShowTimeLine(DTLineHelp TlineHlp, bool vis)
	{
		DElmHelp ElmHelp = TlineHlp.FirstElmHelp

		while(ElmHelp)
		{
			ElmHelp.visible = vis
			ShowWidget(ElmHelp.storage.wdg, vis)
			ElmHelp = ElmHelp.NextElmHelp
		}
	}

//------------------------------------------------------------------
	void M_SetProgram(ExtClass other)
	{
		M_Stop(this)
		RemoveProgram()
		program = other._value
		prg = World.DispColPtr.LoadProgram(program)
		SetProgram()
		M_Play(this)
	}

//------------------------------------------------------------------
	void RealizeCommand(DTLineHelp TlineHlp, DCommand cmd)
	{
		string tokens[8]
		int nump = ParseString(cmd.command, tokens)
		float ftmp = 0
		DTLineHelp TargLine
		string first = tokens[0]

		if(first == "JUMP")
		{
			if(TlineHlp.status == 2)	//hack. vynechavame JUMP aby to na konci skoncilo
				return

			if(nump == 2)
				ftmp = atof(tokens[1])

			SetTimeLinePos(TlineHlp, ftmp, false)
			TlineHlp.status = 1
			return
		}

		if(first == "SND")
		{
			DSound snd = prg.FirstSound
			while(snd)
			{
				if(snd.ID == tokens[1])
				{
					MinClass Own = Owner
					
					if(!Own)
						Own = this

					if(snd.shandle)
					{
#ifdef EDITOR
						if(!World.EditorActive)	//ked editujeme tak su zvuky neziaduce.
#endif
						PlaySound(Own, snd.shandle, SF_ONCE)
					}
					return
				}
				snd = snd.NextSound
			}
			return
		}

		if(first == "STATUS")
		{
			if(atoi(tokens[1]) > 0)
				ActionNum = 1
			else
				ActionNum = 0

			return	
		}

		if(first == "TLINE")
		{
			DTLineHelp line = GetTimeLineHelpPtr(tokens[1])

			if(line)
			{
				if(tokens[2] == "START")
				{
					SetTimeLinePos(line, 0, true)
					line.status = 1
				}

				if(tokens[2] == "STOP")
				{
					if(line.status != 0)
						SetTimeLinePos(line, line.CurTime, true)	//okamzite musime do widget ktore pouziva vrazit aktualny stav pretoze dalsie timelajny uz potom mozu s tymi widgetami nieco robit a neskor by sme neboli schopni rozpoznat ktory widget ma mat jake parametre

					line.status = 0
				}

				if(tokens[2] == "ENDSTOP")
					line.status = 2				//tento status je hack na vynechanie posledneho JUMPu

				if(tokens[2] == "JUMP")
					SetTimeLinePos(line, atof(tokens[3]), false)
				
				if(tokens[2] == "HIDE")
					ShowTimeLine(line, false)					//skryje grafiku ktoru timelajna pouziva

				if(tokens[2] == "SHOW")
					ShowTimeLine(line, true)
			}
			return
		}

		if(first == "WIDGET")
		{
			DElmHelp elm = GetElementHelpPtr(tokens[1])
			
			if(elm)
			{
				if(tokens[2] == "HIDE")
				{
					ShowWidget(elm.storage.wdg, false)
					elm.visible = false
				}

				if(tokens[2] == "SHOW")
				{
					ShowWidget(elm.storage.wdg, true)
					elm.visible = true
				}
			}
			return	
		}
	}

//------------------------------------------------------------------
	void RunAction(string ActionName)
	{
		if(!prg)
		{
			string ErrMsg = "Display RunAction error. Missing .dpl file " + program + " ?"
			DPrint(ErrMsg)
			return
		}
					
		DispAction action = prg.FirstAction
		
		while(action)
		{
			if(action.ID == ActionName)
			{
				DCommand cmd = action.FirstCommand
				
				while(cmd)
				{
					RealizeCommand(NULL, cmd)
					cmd = cmd.NextCommand	
				}
				break
			}
			action = action.NextAction	
		}

		if(NeedAnyTimelineUpdateAllways() == true)	//optimalizujeme. neupdatujeme cas z OnFrame ale iba z OnVisible ked v displeji nebezi nic doleziteho co musi prebiehat v spravnom case
			SetEventMask(this, EV_FRAME)
		else
			ClearEventMask(this, EV_FRAME)
		
		SetEventMask(this, EV_VISIBLE)
	}
	
//------------------------------------------------------------------
	void M_RunAction(ExtClass other)
	{
		RunAction(other._value)
	}

//------------------------------------------------------------------
	void RunActionFromIndex(int index)	//iba koli vieweru na debug ucely
	{
		string StateName
		int count = 0
		DispAction action = prg.FirstAction
		
		while(action)
		{
			if(count == index)
			{
				DCommand cmd = action.FirstCommand
				
				while(cmd)
				{
					RealizeCommand(NULL, cmd)
					cmd = cmd.NextCommand
				}	
				return
			}
			count++
			action = action.NextAction	
		}
	}

//------------------------------------------------------------------
	float InterpBetwen(DElmKeyFloat CurrentKey, DElmKeyFloat NextKey, float TLineTime)
	{
		float RateBetwen
		float TimeBetwen
		float ReativeTime
		float ValueOffset

//		if(!NextKey || NextKey.interp <= DSP_INTERP_DIRECTLY)
//			return CurrentKey.fval

		float CurrentVal = CurrentKey.fval
		float CurrentTime = CurrentKey.time
		
		TimeBetwen = NextKey.time - CurrentTime
		ValueOffset = NextKey.fval - CurrentVal
		ReativeTime = TLineTime - CurrentTime					//cas ktory prebehol od pozicie na CurrentKey
		RateBetwen = ReativeTime / TimeBetwen						//percentualna pozicia medzi klucmi

		if(NextKey.interp == DSP_INTERP_SPLINE1)		//z pomala do rychla
			RateBetwen = RateBetwen * RateBetwen * RateBetwen
		else
		{
			if(NextKey.interp == DSP_INTERP_SPLINE2)		//z rychla do pomala
			{
				float flip = 1 - RateBetwen
				RateBetwen = flip * flip * flip
				RateBetwen = 1 - RateBetwen
			}
			else
			{
				if(NextKey.interp == DSP_INTERP_SPLINE3)		//z pomala do rychla, konci pomaly (vysek z sinusovky)
				{
					RateBetwen = 0.25 + (RateBetwen * 0.5)
					RateBetwen = sin(RateBetwen * M_PI2) - 1
					RateBetwen *= -0.5
				}
			}
		}

		return ValueOffset * RateBetwen + CurrentVal	//momentalna hodnota
	}

//------------------------------------------------------------------
	void UpdateElements()
	{
		if(!r_display) //engine ma zakazany renderovani displays
			return
	
		if(skipcount != 0)
			return

		if(g_iViewFrame <= LastUpdateFrame)
			return

		LastUpdateFrame = g_iViewFrame

		DTLine tline
		DTLineHelp TlineHlp = FirstTLineHelp
		float TimeRate

		DElmKeyFloat CurrentKey
		DElmKeyFloat NextKey
		DElmKeyFloat TempKey

		float angle
		float opacity
		string text

		while(TlineHlp)								//prebehneme vsetky timelajny
		{
			tline = TlineHlp.DefLine

			if(!TlineHlp.status)						//ale iba tie ktore maju bezat
			{
				TlineHlp = TlineHlp.NextLineHelp
				continue
			}

			float curtime = TlineHlp.CurTime

			UsingInfo Uinfo
			DElmHelp ElmHelp = TlineHlp.FirstElmHelp
			DWidgetStorage ElmHelpStorage

			while(ElmHelp)
			{
				Uinfo = ElmHelp.DefInfo
				ElmHelpStorage = ElmHelp.storage
//--- xpos
				TempKey = ElmHelp.CurXposKey
				while(TempKey && TempKey.time < curtime)	//cas mohol ubehnut a medzitym sa elementy neupdatovali takze to musime dobehnut a spravne nastavit current a next
				{
					ElmHelp.CurXposKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurXposKey	//nemusi mat ani prvy kluc! (nema ziadne)

				if(CurrentKey)
				{
					NextKey = CurrentKey.NextKey		//ked nema NextKey tak Current je posledny
					ElmHelpStorage.CurXpos = CurrentKey.fval	//default z aktualneho
	
					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurXposKey = CurrentKey
							ElmHelpStorage.CurXpos = CurrentKey.fval	//default z aktualneho
						}
						if(NextKey && NextKey.interp > DSP_INTERP_DIRECTLY)
							ElmHelpStorage.CurXpos = InterpBetwen(CurrentKey, NextKey, curtime)
					}
				}

//---	ypos
				TempKey = ElmHelp.CurYposKey
				while(TempKey && TempKey.time < curtime)
				{
					ElmHelp.CurYposKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurYposKey	//xpos

				if(CurrentKey)
				{
					NextKey = CurrentKey.NextKey
					ElmHelpStorage.CurYpos = CurrentKey.fval	//default z aktualneho

					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurYposKey = CurrentKey
							ElmHelpStorage.CurYpos = CurrentKey.fval	//default z aktualneho
						}
						if(NextKey && NextKey.interp > DSP_INTERP_DIRECTLY)
							ElmHelpStorage.CurYpos = InterpBetwen(CurrentKey, NextKey, curtime)
					}
				}

//---	width
				TempKey = ElmHelp.CurWidthKey
				while(TempKey && TempKey.time < curtime)
				{
					ElmHelp.CurWidthKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurWidthKey	//xpos

				if(CurrentKey)
				{
					NextKey = CurrentKey.NextKey
					ElmHelpStorage.CurWidth = CurrentKey.fval	//default z aktualneho

					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurWidthKey = CurrentKey
							ElmHelpStorage.CurWidth = CurrentKey.fval	//default z aktualneho
						}
						if(NextKey && NextKey.interp > DSP_INTERP_DIRECTLY)
							ElmHelpStorage.CurWidth = InterpBetwen(CurrentKey, NextKey, curtime)
					}
				}

//---	height
				TempKey = ElmHelp.CurHeightKey
				while(TempKey && TempKey.time < curtime)
				{
					ElmHelp.CurHeightKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurHeightKey	//xpos

				if(CurrentKey)
				{
					NextKey = CurrentKey.NextKey
					ElmHelpStorage.CurHeight = CurrentKey.fval	//default z aktualneho

					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurHeightKey = CurrentKey
							ElmHelpStorage.CurHeight = CurrentKey.fval	//default z aktualneho
						}
						if(NextKey && NextKey.interp > DSP_INTERP_DIRECTLY)
							ElmHelpStorage.CurHeight = InterpBetwen(CurrentKey, NextKey, curtime)
					}
				}

//---	angle
				TempKey = ElmHelp.CurAngleKey
				while(TempKey && TempKey.time < curtime)
				{
					ElmHelp.CurAngleKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurAngleKey
				angle = 6666	//defaultne nic nemenime

				if(CurrentKey)
				{
					angle = CurrentKey.fval
					NextKey = CurrentKey.NextKey

					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurAngleKey = CurrentKey
							angle = CurrentKey.fval	//default z aktualneho
						}
						if(NextKey && NextKey.interp > DSP_INTERP_DIRECTLY)
							angle = InterpBetwen(CurrentKey, NextKey, curtime)
					}
				}

//---	opacity
				TempKey = ElmHelp.CurOpacityKey
				while(TempKey && TempKey.time < curtime)
				{
					ElmHelp.CurOpacityKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurOpacityKey
				opacity = 6666	//defaultne nic nemenime

				if(CurrentKey)
				{
					opacity = CurrentKey.fval
					NextKey = CurrentKey.NextKey
					
					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurOpacityKey = CurrentKey
							opacity = CurrentKey.fval	//default z aktualneho
						}
						if(NextKey && NextKey.interp > DSP_INTERP_DIRECTLY)
							opacity = InterpBetwen(CurrentKey, NextKey, curtime)
					}
				}

//---	text
				TempKey = ElmHelp.CurTextKey
				while(TempKey && TempKey.time < curtime)
				{
					ElmHelp.CurTextKey = TempKey
					TempKey = TempKey.NextKey
				}

				CurrentKey = ElmHelp.CurTextKey

				if(CurrentKey)
				{
					NextKey = CurrentKey.NextKey
	
					if(NextKey)
					{
						if(curtime > NextKey.time)
						{
							CurrentKey = NextKey
							NextKey = CurrentKey.NextKey
							ElmHelp.CurTextKey = CurrentKey
						}
					}
					DElmKeyString StrKey = CurrentKey

					if(ElmHelpStorage.CurString != StrKey.sval)
					{
						ElmHelpStorage.CurString = StrKey.sval
						SetWidgetTextEx(ElmHelpStorage.wdg, 0, 1, "#" + StrKey.sval) //vzdy z posledneho kluca
					}
				}

//---	drawing
				float offsetx = fabs(ElmHelpStorage.CurWidth) * Uinfo.elm.OriginX
				float offsety = fabs(ElmHelpStorage.CurHeight) * Uinfo.elm.OriginY

				SetWidgetPos(ElmHelpStorage.wdg, ElmHelpStorage.CurXpos - offsetx, ElmHelpStorage.CurYpos - offsety, ElmHelpStorage.CurWidth, ElmHelpStorage.CurHeight)	// a konecne aplikujeme interpolovane hodnoty
				
				if(opacity != 6666)
				{
//					if(ElmHelp.storage.DefElm.ID == "error")
//						Print(opacity)
					int col = Uinfo.elm.color
					col |= (int)opacity << 24

					if(Uinfo.elm.type == DSP_TYPE_FONT)					//u textovych widgetov menime farbu textu a u zbytku farbu pozadia
						SetWidgetTextColor(ElmHelpStorage.wdg, col)
					else
						SetWidgetColor(ElmHelpStorage.wdg, col)
				}

				if(angle != 6666)
				{
					float UVs[8]
					GetRotatedSpriteUVs(UVs, angle)
					SetWidgetUV(ElmHelpStorage.wdg, UVs)
				} 

				ShowWidget(ElmHelpStorage.wdg, ElmHelp.visible)

				ElmHelp = ElmHelp.NextElmHelp
			}
			
			TlineHlp = TlineHlp.NextLineHelp
		}
	}

//------------------------------------------------------------------
	void UpdateTime()	//updatuje sa kazdy frame aby casy timelajn bezali vzdy
	{
		if(!r_display) //engine ma zakazany renderovani displays
			return

		LastUpdatedFrame = World.ActualFrame

		float VirtFtime
		skipcount += ftime
		
		if(skipcount >= skiptime)
		{
			VirtFtime = skipcount
			skipcount = 0
		}
		else
			return

		DTLine tline
		DTLineHelp TlineHlp = FirstTLineHelp

		while(TlineHlp)								//prebehneme vsetky timelajny
		{
			tline = TlineHlp.DefLine

			if(!TlineHlp.status)						//ale iba tie ktore maju bezat
			{
				TlineHlp = TlineHlp.NextLineHelp
				continue
			}

			float step = 100 * VirtFtime + TlineHlp.CurTime
			float curtime = step
			TlineHlp.CurTime = curtime

			UsingInfo Uinfo
			DElmHelp ElmHelp = TlineHlp.FirstElmHelp

			if(TlineHlp.CurTime >= tline.FullTime)
			{
				if(TlineHlp.status != 2)	//2 = ignorujeme pripadny jump v RealizeCommand
					TlineHlp.status = 0		//deaktivujeme timelajnu. pokial je na konci command repeat tak to moze znova zapnut!
			}

			while(TlineHlp.ActCommand && TlineHlp.CurTime >= TlineHlp.ActCommand.time)	//ak su vobec pouzite commandy a niesme na konci
			{
				DCommand CurCmd = TlineHlp.ActCommand
				TlineHlp.ActCommand = TlineHlp.ActCommand.NextCommand	//prepneme na dalsi. musi to byt pre realize pretoze ten to pravdepodobne zase zmeni
				RealizeCommand(TlineHlp, CurCmd)
			}
			
			if(TlineHlp.CurTime >= tline.FullTime)
			{
				if(TlineHlp.status != 1)	//timelajna tu konci takze okamzite musime do widget ktore pouziva vrazit aktualny stav pretoze dalsie timelajny uz potom mozu s tymi widgetami nieco robit a neskor by sme neboli schopni rozpoznat ktory widget ma mat jake parametre
					SetTimeLinePos(TlineHlp, tline.FullTime, true)

				if(TlineHlp.status == 2)	//koniec hacku na vynechanie JUMPu
					TlineHlp.status = 0
			}
			
			TlineHlp = TlineHlp.NextLineHelp
		}

		TlineHlp = FirstTLineHelp
		int NumActiveTimeLines = 0	//pokial ziadna timelajna nebezi tak to stopneme

		while(TlineHlp)
		{
			if(TlineHlp.status > 0)
				NumActiveTimeLines++
			
			TlineHlp = TlineHlp.NextLineHelp
		}

		if(NumActiveTimeLines == 0)
			M_Stop(this)
	}
	
//------------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)	//preposielane z entity ktorej displej patri
	{
		if(status == DSP_PLAY)	//ked je displej stopnuty tak cas v ziadnom pripade neupdatujeme
		{
			if(LastUpdatedFrame < World.ActualFrame)	//funguje optimalizacia kedy cas neupdatujeme z OnFrame ale iba odtial
				UpdateTime()
		}

		UpdateElements()
		return 0
	}

//------------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(LastUpdatedFrame < World.ActualFrame)	//aby sa to nahodou nevolalo 2x
			UpdateTime()
	}

//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(ActionNum != 0)
			return ACT_DISABLE
		
		if(flags & 1)				//je disablovany
			return ACT_DISABLE

		return ACT_ENABLE
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//------------------------------------------------------------------	
	void func_display(ExtClass owner, string ProgramFile, bool OnScr)
	{
		Owner = owner
		program = ProgramFile
		OnScreen = OnScr
		
		if(owner && ClassName(Owner) != "viewer")
		{
			SetName(owner._name + "_disp")
		}

		if(!program)
			program = "displays/TestProgram.dpl"
		
		prg = World.DispColPtr.LoadProgram(program)

		if(!prg)
			return

		SetProgram()
		M_Play(this)
		
		//aby se na zacatku displeje trochu rozsynchronizovaly a nebezely v ten samy frame,
		//jinak se cela optimalizace s frameskipem projevi jako pravidelne zadrhavani.
		skipcount = DisplaySkip
		DisplaySkip = DisplaySkip + 0.02
		if(DisplaySkip >= 0.2)
			DisplaySkip = 0
	}

//------------------------------------------------------------------	
	void ~func_display()
	{
		RemoveProgram()
		
		DWidgetStorage st = FirstWStorage
		DWidgetStorage cur

		while(st)
		{
			cur = st
			st = st.NextWStorage
			delete cur
		}

		DWidgetContainer ew = FirstExternWidget	//widgety ktore sme do displeja pridali z inej entity musime zrusit uz teraz
		DWidgetContainer CurEw

		while(ew)
		{
			CurEw = ew
			ew = ew.next
			delete CurEw
		}

		if(CanvasWdg)
		{
			DestroyWidget(CanvasWdg)
			CanvasWdg = NULL
		}
	}
}

