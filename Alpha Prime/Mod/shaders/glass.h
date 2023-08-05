shader "textures/glass/brokenglass1"
{
	surfparm trans
	surfparm glass

   cull none
	
	zwrite false

	alphatest 2

#ifdef _XBOX
	blend invsrcalpha srcalpha
	sort translucent
	diffusemap "textures/glass/brokenglass1" alpha nomipmap

#else
	sort overlay
	
	diffusemap $renderview clampu clampv
	refractmap "textures/glass/brokenglass1_n" alpha
	diffusemap2 "textures/glass/brokenglass1" alpha nomipmap

	parm0 g_RefractGlassMat0
	parm1 g_RefractGlassMat1

	material haze
#endif
}

shader "textures/glass/brokenglass2"
{
	surfparm trans
	surfparm glass

   cull none
	
	zwrite false

	alphatest 1
	map nolight

#ifdef _XBOX
	blend invsrcalpha srcalpha
	sort translucent
	diffusemap "textures/glass/brokenglass2" alpha nomipmap

#else

	sort overlay
	
	diffusemap $renderview clampu clampv
	refractmap "textures/glass/brokenglass2_n" alpha
	diffusemap2 "textures/glass/brokenglass2" alpha nomipmap

	parm0 g_RefractGlassMat0
	parm1 g_RefractGlassMat1

	material haze
#endif
}


shader "textures/glass/brokenglass3"
{
	surfparm trans
	surfparm glass
   cull none
   zwrite false

	blend invsrcalpha srcalpha
	sort translucent
	diffusemap "textures/glass/brokenglass1a" alpha
}


shader "textures/glass/brokenglass4"
{
	surfparm trans
	surfparm glass
   cull none
   zwrite false

	blend invsrcalpha srcalpha
	//sort translucent
	diffusemap2 "textures/glass/brokenglass3" alpha
	sort overlay
	
	diffusemap $renderview clampu clampv
	refractmap "textures/glass/brokenglass2_n" alpha
	//diffusemap2 "textures/glass/brokenglass2" alpha nomipmap

	parm0 g_RefractGlassMat0
	parm1 g_RefractGlassMat1

	material haze
}



// test
shader "textures/glass/mirror"
{
	surfparm glass
   mirror
   map nolight
   alphatest 1
   
	diffusemap "textures/glass/brokenglass1" clampu clampv nofilter alpha
}


shader "textures/glass/glass0"
{
	surfparm trans
	surfparm glass
	specular 1
	specularpower 0.41
	
	zwrite false
	cull none
	blend one srcalpha
	sort translucent

	diffusemap "textures/glass/glass1_c" alpha
	//normalmap "textures/glass/glass1_n"
}

shader "textures/glass/glasstrain"
{
	surfparm trans
	surfparm glass
	specular 1
	specularpower 0.41
	reflection 1.0
	
	zwrite false
	cull none
	blend one one
	sort translucent

	diffusemap "textures/glass/glass1_c" alpha
	envmap $nearestcube
}

/*
shader "textures/glass/glass0"
{
	specular 1
	specularpower 0.41
	diffusemap "textures/glass/glass1_c"// alpha
}*/

shader "textures/glass/glass0_nospec"
{
	surfparm trans
	surfparm glass
	specular 0.5
	specularpower 0.41

	zwrite false
	cull none
	//blend invsrcalpha srcalpha
	blend one srcalpha
	sort translucent

	diffusemap "textures/glass/glass1_c" alpha
	//normalmap "textures/glass/glass1_n"
}

shader "textures/glass/glass1"
{
	surfparm trans
	surfparm glass
	specular 1
	specularpower 0.41

	zwrite false
	cull none
	//blend invsrcalpha srcalpha
	blend one srcalpha
	sort translucent

	diffusemap "textures/glass/glass1_c" alpha
	//normalmap "textures/glass/glass1_n"
}

shader "textures/glass/glass2"
{
	surfparm trans
	surfparm nocastshadow
	surfparm glass
	tesssize 64

	specular 1
	specularpower 0.61
   reflection 0.4

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent
	map nolight

	diffusemap "textures/glass/glass2_c" alpha
	envmap $nearestcube
}


shader "textures/glass/glass2_ref"
{
	surfparm trans
	surfparm nocastshadow
	surfparm glass
	tesssize 64

	specular 1
	specularpower 0.61

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	map nolight
	sort overlay

	diffusemap $renderview clampu clampv
	refractmap "textures/glass/glass_ref_n" alpha

	parm0 g_RefractGlassMat0
	parm1 g_RefractGlassMat1
}

	















shader "textures/glass/glass2lowspec"
{
	surfparm trans
	surfparm nocastshadow
	surfparm glass
	tesssize 64

	specular 0.5
	specularpower 0.61
   reflection 0.4

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent
	map nolight

	diffusemap "textures/glass/glass2_c" alpha
	envmap $nearestcube
}

shader "textures/glass/glassblack"
{
//castshadow
	surfparm trans
	surfparm castshadow
	tesssize 64

	specular 1
	specularpower 0.41
   reflection 0.2

	diffusemap "textures/glass/glassblack_c" alpha
	envmap $nearestcube
}


shader "textures/glass/lcd_disconnect"
{
	surfparm glass
castshadow
	specular $alpha     
	specularpower 0.72
	
	map nolight

	pass
		tcmod shift g_fNoiseU g_fNoiseV

	diffusemap "gfx/remotecontrol/noise"
}

shader "textures/glass/lcd1"
{
	surfparm glass
castshadow
	specular $alpha     
	specularpower 0.72
	
	map nolight
	//blend one one //co je to za demenci? Proc to bylo pruhledny??
   //sort translucent

	diffusemap $renderview clampu clampv
}

shader "textures/glass/lcd1notrans"
{
	surfparm glass
	//castshadow
	specular $alpha     
	specularpower 0.72
	
	map nolight
	//blend one one
  // sort translucent

	diffusemap $renderview clampu clampv
}


shader "textures/glass/lcd2"
{
	surfparm glass
castshadow
	surfparm trans
   surfparm detail
   surfparm nomarks

	specular 0.2	//$alpha
	specularpower 0.32

	zwrite false
	//cull none
	//blend invsrcalpha srcalpha
	blend one one
	sort translucent

	map nolight

	pass
		tcmod shift 0.0 g_fTime10

#ifdef _XBOX
	diffusemap "textures/glass/lcd1_c"
#else
	diffusemap $gui0  //$renderview
#endif
}

shader "textures/glass/monitor1"
{
	surfparm glass
	surfparm trans
   surfparm detail
   surfparm nomarks
castshadow
	specular 0.2	//$alpha
	specularpower 0.32

	//zwrite false
	//cull none
	//blend invsrcalpha srcalpha
	//blend one one
	//sort translucent

	map nolight

#ifdef _XBOX
	pass
		tcmod shift 0.0 g_fTime10

	diffusemap "textures/glass/lcd1_c"
#else
	diffusemap $gui0  //$renderview
#endif
}

shader "textures/glass/haze1"
{
	zwrite false

	//blend one one
	sort overlay

	map nolight

//	pass
	//	tcmod shift g_fTime g_fTime

	diffusemap $renderview clampu clampv
	refractmap "textures/vp1"

	parm0 g_RefractGlassMat0
	parm1 g_RefractGlassMat1

	material haze
}

shader "textures/glass/hazetwirl1"
{

#ifdef _XBOX
	surfparm trans
	specular 1
	specularpower 0.42

	zwrite false
	cull none
	//blend invsrcalpha srcalpha
	blend one srcalpha
	sort translucent

	diffusemap "textures/glass/glass1_c" alpha

#else
	zwrite false

	//blend one one
	sort overlay

	map nolight

	pass
	tcmod shift g_fTime g_fTime

	diffusemap $renderview clampu clampv
	refractmap "textures/vp1"
//	refractmap "textures/common/bump" bump

	parm0 g_RefractGlassMat0
	parm1 g_RefractGlassMat1

	material haze
#endif
}
