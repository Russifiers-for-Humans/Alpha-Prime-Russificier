//****************************************************************************
//*              COMMON SHADERS                                              *
//****************************************************************************
//NEMAZAT! Testovaci objekt pro cubemapy!
shader "DebugSphere"
{
	//specular 1.0
	reflection 1.0
	//fresnel 0.501
	//reflection $nmapalpha

	diffusemap "gfx/black"
	//diffusemap "textures/nr_test/mriz_02_C"
	//normalmap "textures/nr_test/mriz_02_N" alpha
	envmap $nearestcube	
}

shader "textures/common/mirror"
{
	map nolight
	mirror

	diffusemap "gfx/black"
}

shader "debugportals"
{
	sort near
	cull none
	
#ifndef _XBOX
	effect "debug_portal"
#endif
}

shader "null"
{
}

shader "textures/REGION"
{
	surfparm nolightmap
}


shader "textures/common/alpha"
{
	qer_nocarve
	surfparm nonsolid
	effect "nolightmap"
}

shader "textures/common/pivot1"
{
	qer_nocarve
	surfparm nonsolid
	surfparm pivot
	value		0
}

shader "textures/common/pivot2"
{
	qer_nocarve
	surfparm nonsolid
	surfparm pivot
	value		1
}

shader "textures/common/pivot3"
{
	qer_nocarve
	surfparm nonsolid
	surfparm pivot
	value		2
}

shader "textures/common/pivot4"
{
	qer_nocarve
	surfparm nonsolid
	surfparm pivot
	value		3
}

shader "textures/common/nodrawnonsolid"
{
	surfparm nonsolid
	surfparm nodraw
}


shader "textures/common/nodraw"
{
	surfparm nodraw
}


shader "textures/common/clipsolid"
{
	qer_trans 0.40
	surfparm trans
	surfparm nomarks
	surfparm nodraw
}

shader "textures/common/clipcamera"
{
	qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm nonsolid
	surfparm cameraclip
	surfparm noimpact
}

shader "textures/common/clip"
{
	qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm nonsolid
	surfparm playerclip
	surfparm monsterclip
	surfparm noimpact
	surfparm detail
}

shader "textures/common/clip_playeronly"
{
   qer_editorimage "textures/common/clip"
   qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm nonsolid
	surfparm playerclip
	//surfparm monsterclip
	surfparm noimpact
	surfparm detail
}

shader "textures/common/climbclip"
{
	qer_trans 0.40
	surfparm trans
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm solid
	surfparm noimpact
	surfparm detail
}

shader "textures/common/clip_metal"
{
castshadow
	qer_editorimage "textures/common/clip"
	qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm nonsolid
	surfparm playerclip
	surfparm monsterclip
	surfparm noimpact
	surfparm detail
	surfparm metal
}

shader "textures/common/clip_stone"
{
	qer_editorimage "textures/common/clip"
	qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm nonsolid
	surfparm playerclip
	surfparm monsterclip
	surfparm noimpact
	surfparm detail
	surfparm beton
}

shader "textures/common/clipall"
{
	qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm cameraclip
	surfparm nodraw
	surfparm playerclip
	surfparm monsterclip
	surfparm noimpact
	surfparm detail
	surfparm trans
}

shader "textures/common/ladder"
{
    qer_editorimage "textures/common/slick"
    qer_trans 0.4
    surfparm nodraw
    surfparm nomarks
    surfparm nolightmap
    surfparm nonsolid
    surfparm trans
    surfparm ladder
}

shader "textures/common/monsterclip"
{
	qer_trans 0.40
	surfparm nolightmap
	surfparm nomarks
	surfparm nodraw
	surfparm monsterclip
	surfparm noimpact
	surfparm detail
	surfparm trans
}

shader "textures/common/missileclip"
{
castshadow
	qer_trans 0.40
	surfparm nodamage
	surfparm nomarks
	surfparm nodraw
	surfparm playerclip
}

shader "textures/common/cushion"
{
	qer_nocarve
	qer_trans 0.50
	surfparm nodraw
	surfparm nomarks
	surfparm nodamage
	surfparm trans
}

shader "textures/common/hint"
{
   qer_nocarve
   qer_trans 0.30
   surfparm nodraw
   surfparm hint
   surfparm nonsolid
   surfparm structural
   surfparm trans
   surfparm noimpact
}

shader "textures/common/skip"
{
   qer_nocarve
   qer_trans 0.40
   surfparm nodraw
   surfparm nonsolid
   surfparm structural
   surfparm trans
}

shader "textures/common/origin"
{
	qer_nocarve
	surfparm nodraw
	surfparm nonsolid
	surfparm origin
}

shader "textures/common/noimpact"
{
	surfparm noimpact
}

shader "textures/common/areaportal"
{
	qer_trans 0.50
	surfparm nodraw
	surfparm nonsolid
	surfparm structural
	surfparm trans
	surfparm nomarks
	surfparm areaportal
}

shader "textures/common/antiportal"
{
	qer_trans 0.50
	surfparm nodraw
	surfparm nonsolid
	surfparm structural
	surfparm nomarks
	surfparm antiportal
}

shader "textures/common/trigger"
{
	qer_trans 0.50
	qer_nocarve
	surfparm nodraw
	surfparm trans
	surfparm nocastshadow
}

shader "textures/common/caulk"
{
	surfparm nodraw
	surfparm nomarks
	surfparm nolightmap
	surfparm nocastshadow
}

shader "textures/efx/cloth"
{
   surfparm nonsolid
   surfparm trans
   surfparm nomarks
   cull none
  
   pass 0
      vcmod cloth 0.0 0.0 -24.0

   layer "textures/misc/gob_04"
   layer $lightmap	
   tex0 = layer 0
   tex1 = layer 1
   effect "lightmapped"
}

/*
Smazat
*/
shader "textures/nr_test/mriz_02" 
{
	specular $alpha
   tesssize 64
	
	diffusemap "textures/nr_test/mriz_02_C"
	normalmap "textures/nr_test/mriz_02_N"
}

shader "textures/nr_test/chodba_00_C" 
{
	specular $alpha
   tesssize 64
	
	diffusemap "textures/nr_test/chodba_00_C"
	normalmap "textures/nr_test/chodba_00_N"
}
