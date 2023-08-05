widget WPStat
 //=================================================================================
int LoadStaticObject(string file)	//pokus. nahrajeme aj nejaky objekt okolo toho particloveho aby bolo vidiet velkosti
{
	if(World.ViewerClass)
	{
		viewer ViewerPtr = World.ViewerClass
		ViewerPtr.LoadOtherStaticObject(file)
	}
	return true
}

//=================================================================================
void PViewerUpdate()
{
	SetWidgetText(WPStat, 0, String("Particles: " + itoal(g_iStatNumParticles, 6)))
	UpdateStatKeys()
}

//=================================================================================
int PViewer(string name)
{
	CMinClass = Type("MinClass")
	CExtClass = Type("ExtClass")

	LargeFont = CreateFontEx("font_large")
	SmallFont = CreateFontEx("font_small")
	MainFont = CreateFontEx("font_main")

//	WView = CreateWidget(WTYPE_VIEW, 0, 0, 1024, 600, WF_VISIBLE, 0, 0)
	WPStat = CreateWidget(WTYPE_TEXT, 230, 20, 200, 20, WF_VISIBLE, 0xffffffff, 1)
	gOBJType = 2
	return true
}
/*
//=================================================================================
void PViewerRefresh()	//je tu uz iba koli prezeraniu z
{
	if(ViewerClass)
	{
		viewer CommonViewer = ViewerClass
		SelectObject(CommonViewer, VNULL)
		
		if(CommonViewer.PObject)
			ReleaseObject(CommonViewer.PObject, true)
				
		CommonViewer.PObject = GetObject("particle_temp.ptc")
		SelectObject(CommonViewer, CommonViewer.PObject)
	}
}
*/
int PViewerInit()
{
	r_texfilter = 2
	r_maxaniso = 16
	g_iGeomDetail = 2
	g_iBrightness = 50
	g_iEffectsVolume = 60
	g_iMusicVolume = 60
	g_iSoundQuality = 1
	r_shadow = 1
	
	g_iFullscreen = 0
	g_iDither = 0
	g_iSpeakers = 4
	g_bEAX = 0

	
//	g_iWidth = 640
//	g_iHeight = 480
//	g_iHeight -= 20
	
	g_lod = 0.21
	
	g_iFly = false
	
	//Sound
	g_fDopplerFactor = 1.5

	return true
}


_eclass GetVieverPtr()	//GUI editor z toho ziskava pointer
{
	return World.ViewerClass
}