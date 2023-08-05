void g_mainstats()
{
	g_iStats++
	if(g_iStats >= 3)
		g_iStats = 0
	
}

void g_rasterstats()
{
	if(g_iWire == 0 && g_iDepthComplex == 0)
	{
		g_iWire = 1
	}
	else
	{
		if(g_iWire == 1)
		{
			g_iWire = 0
			g_iDepthComplex = 1
		}
		else
		{
			if(g_iDepthComplex == 1)
			{
				g_iDepthComplex = 0
				g_iWire = 0
			}
		}
	}
}

void showai()
{
	g_bDoShowAI = true
}

void leach()
{
	CheatNextLevel = true
}

void rasputin()
{
	CheatImmortality = true
}

void xylophone()
{
	CheatWeapons = true
}

void fodder()
{
	CheatAmmo = true
}

void quack()
{
	CheatHealth = true
}

void coke()
{
	CheatHubbardium = true
}

void pleasedaddydrawthisforme()
{
	CheatWeapons = true
	CheatImmortality = true
	CheatAmmo = true
	CheatHubbardium = true
}

void help()
{
	Print("www.alpha-prime.com")	
	Print("www.blackelement.net/forum")
}

void version()
{
	Print(String("Alpha Prime" + AlphaPrimeVersion))
}

//=================================================================================
void q()
{
	g_iMInGame = 2
}

#ifdef EDITOR
void r_makecubemaps()
{
	if(globalmap == NULL)
		return
	
	SetCurrentMap(globalmap)
	
	ctype ct = Type("info_cubemap")
	info_cubemap ec
	
	for(ec = GetNextEntity(NULL, ct); ec != NULL; ec = GetNextEntity(ec, ct))
	{
		ec.vflags = ClearFlags(ec, TFL_VISIBLE)
	}

	for(ec = GetNextEntity(NULL, ct); ec != NULL; ec = GetNextEntity(ec, ct))
	{
		UpdateCubeMap(ec, ec.size, ec._name)
	}

	for(ec = GetNextEntity(NULL, ct); ec != NULL; ec = GetNextEntity(ec, ct))
	{
		SetFlags(ec, ec.vflags)
	}
}
#else
void r_makecubemaps()
{
	if(globalmap == NULL)
		return
	
	SetCurrentMap(globalmap)
	
	ctype ct = Type("info_cubemap")
	info_cubemap ec
	
	for(ec = GetNextEntity(NULL, ct); ec != NULL; ec = GetNextEntity(ec, ct))
	{
		UpdateCubeMap(ec, ec.size, ec._name)
	}
}
#endif

#ifdef EDITOR
const bool RmapEnabled = false
#else
const bool RmapEnabled = true
#endif

#ifdef DEVELOPER
//=================================================================================
void rmap()
{
	if(!RmapEnabled)
		return

	if(g_sThisLevel == "")
	{
		Print("Can't reload map")
	}

int newmap
string fullname

	fullname = "worlds/" + g_sThisLevel + ".wrl"
	
	Print(String("*** Reloading map:" + fullname + " ***"))
	
   SetWidgetImage(WView, NULL) //avoid redraw during LoadEntities (can cause unwanted Exceptions on entities)
 
 	g_bRMap = true  

	if(globalmap != NULL)
	{
		ReloadMap(globalmap)
		//FreeMap(globalmap)
		g_iFly = false
   	//globalmap = LoadMap(fullname, true)
	}

/*
   newmap = LoadMap(fullname)
   if( newmap == NULL)
   {
   	Print("Can't found")
  	}
	else
	{
		if(globalmap != NULL)
			FreeMap(globalmap)
		
		globalmap = newmap
	}
*/
   SetWidgetImage(WView, globalmap)
  	Print("*** Reload done! ***")
}

//=================================================================================
void x(string a)
{
	g_FromConsoleCall = a
	a = ""	
}
#endif