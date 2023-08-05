shader "RefractWave"
{
	surfparm trans
   cull none
	zwrite false
	sort overlay

	sort overlay
	
	diffusemap $renderview clampu clampv
	refractmap "textures/efx/Ref03dxt"

	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]

	material haze
}


shader "flameheat"
{
	surfparm trans
   cull none
	zwrite false
	sort overlay
	
	diffusemap $renderview clampu clampv
	refractmap
		anim 30
		"textures/efx/Refract_Heat_N_000"
		"textures/efx/Refract_Heat_N_001"
		"textures/efx/Refract_Heat_N_002"
		"textures/efx/Refract_Heat_N_003"
		"textures/efx/Refract_Heat_N_004"
		"textures/efx/Refract_Heat_N_005"
		"textures/efx/Refract_Heat_N_006"
		"textures/efx/Refract_Heat_N_007"
		"textures/efx/Refract_Heat_N_008"
		"textures/efx/Refract_Heat_N_009"
		"textures/efx/Refract_Heat_N_010"
		"textures/efx/Refract_Heat_N_011"
		"textures/efx/Refract_Heat_N_012"
		"textures/efx/Refract_Heat_N_013"
		"textures/efx/Refract_Heat_N_014"
		"textures/efx/Refract_Heat_N_015"
		"textures/efx/Refract_Heat_N_016"
		"textures/efx/Refract_Heat_N_017"
		"textures/efx/Refract_Heat_N_018"
		"textures/efx/Refract_Heat_N_019"
		"textures/efx/Refract_Heat_N_020"
		"textures/efx/Refract_Heat_N_021"
		"textures/efx/Refract_Heat_N_022"
		"textures/efx/Refract_Heat_N_023"
		"textures/efx/Refract_Heat_N_024"
		"textures/efx/Refract_Heat_N_025"
		"textures/efx/Refract_Heat_N_026"
		"textures/efx/Refract_Heat_N_027"
		"textures/efx/Refract_Heat_N_028"
		"textures/efx/Refract_Heat_N_029"
		"textures/efx/Refract_Heat_N_030"
		"textures/efx/Refract_Heat_N_031"
		"textures/efx/Refract_Heat_N_032"
		"textures/efx/Refract_Heat_N_033"
		"textures/efx/Refract_Heat_N_034"
		"textures/efx/Refract_Heat_N_035"
		"textures/efx/Refract_Heat_N_036"
		"textures/efx/Refract_Heat_N_037"
		"textures/efx/Refract_Heat_N_038"
		"textures/efx/Refract_Heat_N_039"
		"textures/efx/Refract_Heat_N_040"
		"textures/efx/Refract_Heat_N_041"
		"textures/efx/Refract_Heat_N_042"
		"textures/efx/Refract_Heat_N_043"
		"textures/efx/Refract_Heat_N_044"
		"textures/efx/Refract_Heat_N_045"
		"textures/efx/Refract_Heat_N_046"
		"textures/efx/Refract_Heat_N_047"
		"textures/efx/Refract_Heat_N_048"
		"textures/efx/Refract_Heat_N_049"
		"textures/efx/Refract_Heat_N_050"
		"textures/efx/Refract_Heat_N_051"
		"textures/efx/Refract_Heat_N_052"
		"textures/efx/Refract_Heat_N_053"
		"textures/efx/Refract_Heat_N_054"
		"textures/efx/Refract_Heat_N_055"
		"textures/efx/Refract_Heat_N_056"
		"textures/efx/Refract_Heat_N_057"
		"textures/efx/Refract_Heat_N_058"
		"textures/efx/Refract_Heat_N_059"
		"textures/efx/Refract_Heat_N_060"

	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]

	//diffusemap2 "gfx/black" alpha nomipmap

	material haze
}


shader "particle/test"
{
	sort translucent
   cull none

	layer "spr/corona1" alpha
//	layer "spr/corona6" alpha
//	layer "spr/ruka" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/test2"
{
	sort translucent
   cull none

	layer "spr/corona1" alpha

	tex0 = layer 0
   effect "effect_particle"
}


// *** MISC ***


shader "textures/efx/gradient"
{
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   cull ccw

	layer "textures/efx/gradient" alpha

	tex0 = layer 0

	effect "default_flame"

	//qer_trans 1.0
	qer_editorimage "textures/common/grid"
}


shader "textures/efx/gradient_doubleside"
{
	//qer_trans 0.60
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   cull none

	layer "textures/efx/gradient" alpha

	tex0 = layer 0

	effect "default_flame"
	qer_editorimage "textures/common/grid"
}

shader "textures/efx/gradientgreen"
{
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   cull none
   subdivsize 64


   pass 0
      tcmod SHIFT 0 g_fTime01

	layer "textures/efx/gradientgreen" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"

	//qer_trans 1.0
	qer_editorimage "textures/common/grid"
}


shader "textures/efx/gradient2"
{
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   //cull none
   subdivsize 64

	layer "textures/efx/gradient2" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"

	//qer_trans 1.0
	qer_editorimage "textures/common/grid"
}


shader "textures/efx/gradient2_doubleside"
{
	//qer_trans 0.60
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   cull none
   subdivsize 64

	layer "textures/efx/gradient2" alpha

	tex0 = layer 0

	//effect "default_flame"
	effect "nolightmap_trans"
	qer_editorimage "textures/common/grid"
}

shader "textures/efx/gradientyellow"
{
	//qer_trans 0.60
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   cull none
   subdivsize 64

	layer "textures/efx/gradientyellow" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"
	qer_editorimage "textures/common/grid"
}

shader "textures/efx/gradientyellow_doubleside"
{
	//qer_trans 0.60
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
	sort translucent
   cull none
   subdivsize 64

	layer "textures/efx/gradientyellow" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"
	qer_editorimage "textures/common/grid"
}

shader "textures/efx/lightstreak0"
{
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
sort overlay
	sort translucent
   //cull none
   subdivsize 64

		
	layer "textures/efx/gradient" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"

	//qer_trans 1.0
	qer_editorimage "textures/common/grid"
	
	material haze
}

shader "textures/efx/lightstreak1"
{
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
sort overlay
	sort translucent
   //cull none
   subdivsize 64

		
	layer "textures/efx/gradient2" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"

	//qer_trans 1.0
	qer_editorimage "textures/common/grid"

material haze
	
}

shader "textures/efx/lightstreak2"
{
   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
   surfparm nodrop
   surfparm nolightmap
sort overlay
	sort translucent
   //cull none
   subdivsize 64

		
	layer "textures/efx/gradient3" alpha

	tex0 = layer 0

	effect "default_flame"
	//effect "nolightmap_trans"

	//qer_trans 1.0
	qer_editorimage "textures/common/grid"
	


material haze
	
}

