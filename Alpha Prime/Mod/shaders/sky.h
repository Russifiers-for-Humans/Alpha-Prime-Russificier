//---------------------------------------
//sky
//--------------------------------------

// emap_sun <cervena> <zelena> <modra> <intensita> <stupne> <vyska>
// color will be normalized, so it doesn't matter what range you use
// intensity falls off with angle but not distance 100 is a fairly bright sun
// stupen  0 = from the east, 90 = north, etc.  vyska  0 = svitani /zapad, 90 = poledne


// FINAL SKYBOX SHADERS

shader "textures/sky/viewer"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP
	qer_editorimage "textures/sky/viewer/sky-5"

	//emap_surfacelight 40
//	emap_lightsubdivide 1024

	//emap_forcesunlight

	layer "textures/sky/viewer/sky"
}


shader "textures/sky/planet"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP

	emap_sun	0.32 0.28 0.1 1000 160 50
	emap_surfacelight 100
	emap_lightsubdivide 1024

	emap_forcesunlight

	layer "textures/sky/planet/sky"
	 qer_editorimage "textures/sky/planet/sky-2"
}

shader "textures/sky/afield"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP

	emap_sun	0.33 0.28 0.1 500 160 50
	emap_surfacelight 380
	emap_lightsubdivide 256
	//emap_globaltexture

	emap_forcesunlight

	layer "textures/sky/afield/sky"
	qer_editorimage "textures/sky/afield/sky-5"
}


shader "textures/sky/afieldtop"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP

	emap_sun	0.28 0.32 0.05 250 40 85
	emap_surfacelight 380
	emap_lightsubdivide 256
	//emap_globaltexture

	emap_forcesunlight

	layer "textures/sky/afieldtop/sky"
	qer_editorimage "textures/sky/afieldtop/sky-5"
}

shader "textures/sky/l01"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP

	emap_sun	0.9 0.7 0.5 1150 80 40
	emap_surfacelight 380
	emap_lightsubdivide 256
	//emap_globaltexture

	emap_forcesunlight

	layer "textures/sky/afield/sky"
	qer_editorimage "textures/sky/afield/sky-5"
}


shader "textures/sky/l04"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP

	emap_sun	0.28 0.32 0.05 250 40 85
	emap_surfacelight 550
	emap_lightsubdivide 256
	//emap_globaltexture

	emap_forcesunlight

	layer "textures/sky/afield/sky"
	qer_editorimage "textures/sky/afield/sky-5"
}

shader "textures/sky/l08"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP

	emap_sun	0.9 0.7 0.4 1600 320 28
	emap_surfacelight 380
	emap_lightsubdivide 256
	//emap_globaltexture

	emap_forcesunlight

	layer "textures/sky/afield/sky"
	qer_editorimage "textures/sky/afield/sky-5"
}


//toto je dabelsky trik, jak zamezit zbytecnemu vykreslovani ostatnich fejsu skyboxu :-)
shader "textures/sky/afield/sky-6"
{
	surfparm NODRAW
}

shader "textures/sky/afieldtop/sky-6"
{
	surfparm NODRAW
}

// *** rotosky ***

shader "textures/sky/space01"
{

	surfparm NOIMPACT
	surfparm NOLIGHTMAP
	surfparm nonsolid
	
   map nolight
   sort translucent
   
   blend one srcalpha
	pass
		tcmod rotate g_fTime001R 0.5 0.5

	diffusemap "textures/sky/space01_c" alpha

}

shader "textures/sky/space02"
{

	surfparm NOIMPACT
	surfparm NOLIGHTMAP
	surfparm nonsolid
	
   map nolight

   sort translucent

   blend one srcalpha
	pass
		tcmod rotate g_fTime01R 0.5 0.5

	diffusemap "textures/sky/space02_c" alpha
}

/*
shader "textures/sky/level01"
{
	surfparm SKY
	surfparm NOIMPACT
	surfparm NOLIGHTMAP
	qer_editorimage "textures/sky/01/sky-5"

	//emap_sun	0.28 0.28 0.24 65 250 45
	emap_surfacelight 120
	emap_lightsubdivide 1024

	emap_forcesunlight

	layer "textures/sky/32/sky"
}
*/


// FINAL FOGS

// worldspawn OnStart
//this.ChangeFog(R, G, B, Dist, 0)
// trigger
//world.ChangeFog(R, G, B, Dist, fadetime)

/*
shader "textures/sky/foglevel_parameter"
{
	surfparm	trans
	surfparm	nonsolid
	surfparm nodrop
	surfparm	fog

	fogparms  g_vFogRGB g_fFogDist
}

shader "textures/sky/foglevel00"
{
	surfparm	trans
	surfparm	nonsolid
	surfparm nodrop
	surfparm	fog

	fogparms		"0.15 0.15 0.2" 1.46

}
*/

