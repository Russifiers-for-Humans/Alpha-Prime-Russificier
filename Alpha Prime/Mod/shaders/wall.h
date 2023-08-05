shader "textures/default"
{
	specular $nmapalpha

	diffusemap "textures/default"
	normalmap "textures/default_n"
}

shader "textures/defaulttga"
{
	specular $nmapalpha

	diffusemap "textures/default"
	normalmap "textures/defaulttga_n"
}

shader "textures/defaultdds"
{
	specular $nmapalpha

	diffusemap "textures/default"
	normalmap "textures/defaultdds_n"
}




shader "textures/wall/rock1"
{
   tesssize 512
   surfparm stone
	specular $nmapalpha
   //specularpower 0.61

	diffusemap "textures/wall/rock1_c"
	qer_editorimage "textures/wall/rock1_c"

	normalmap "textures/wall/rock1_n" alpha
}

shader "textures/wall/rock1_nonm"
{
   tesssize 512
   surfparm stone

	diffusemap "textures/wall/rock1_c"
	qer_editorimage "textures/wall/rock1_c"
}


shader "textures/wall/rock2"
{
   tesssize 512
   surfparm stone
	specular $nmapalpha
   //specularpower 0.61

	diffusemap "textures/wall/rock2_c"
	qer_editorimage "textures/wall/rock2_c"

	normalmap "textures/wall/rock2_n" alpha
}

shader "textures/wall/rock2a"
{
   tesssize 512
   surfparm stone
	specular $nmapalpha
   //specularpower 0.61

	diffusemap "textures/wall/rock2_c"
	qer_editorimage "textures/wall/rock2a_c"

	normalmap "textures/wall/rock2_n" alpha
}

shader "textures/wall/rock3"
{
   tesssize 512
   surfparm stone
	specular $nmapalpha
   //specularpower 0.61

	diffusemap "textures/wall/rock3_c"
	qer_editorimage "textures/wall/rock3_c"

	normalmap "textures/wall/rock1_n" alpha
}


shader "textures/wall/rock1_glow"
{
   tesssize 256

   surfparm trans
   surfparm nomarks
   surfparm noimpact
   surfparm nonsolid
  surfparm nodrop
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap
  surfparm slime

//alphatest 1 // zatim
sort translucent
  specular 0   //$alpha
  //specularpower 0.41
  map nolight
  
  blend one srcalpha

	diffusemap "textures/terrain/hubbardium01_c" alpha
}



shader "textures/wall/PlastWall_04_c"
{
	specular $nmapalpha
  specularpower 0.751
	diffusemap "textures/wall/PlastWall_04_c" 
	normalmap "textures/wall/PlastWall_04_n" alpha
}

shader "textures/wall/PlastWall_03_c"
{
	specular $nmapalpha
  specularpower 0.51
	diffusemap "textures/wall/PlastWall_03_c" 
	normalmap "textures/wall/PlastWall_03_n" alpha
}

shader "textures/wall/PlastWall_02_c"
{
	specular $nmapalpha
  specularpower 0.51
	diffusemap "textures/wall/PlastWall_02_c" 
	normalmap "textures/wall/PlastWall_02_n" alpha
}

shader "textures/wall/PlastWall_01_c"
{
	specular $nmapalpha
  specularpower 0.51
	diffusemap "textures/wall/PlastWall_01_c" 
	normalmap "textures/wall/PlastWall_01_n" alpha
}

shader "textures/wall/MetalWall_03_c"
{
	specular $nmapalpha
  specularpower 0.51
	diffusemap "textures/wall/MetalWall_03_c" 
	normalmap "textures/wall/MetalWall_03_n" alpha
}

shader "textures/wall/MetalWall_02_c"
{
   tesssize 144
	specular $nmapalpha
  specularpower 0.72
	diffusemap "textures/wall/MetalWall_02_c" 
	normalmap "textures/wall/MetalWall_02_n" alpha
}

shader "textures/wall/MetalWall_01_c"
{
	specular $nmapalpha
  specularpower 0.72
	diffusemap "textures/wall/MetalWall_01_c" 
	normalmap "textures/wall/MetalWall_01_n" alpha
}

shader "textures/wall/MetalWall_01b_c"
{
	specular $nmapalpha
	surfparm plastic
  //specularpower 0.52
   reflection 0.091
     
	diffusemap "textures/wall/MetalWall_01b_c" 
	normalmap "textures/wall/MetalWall_01_n" alpha
	envmap $nearestcube
}

shader "textures/wall/MetalWall_01c_c"
{
	specular $nmapalpha
  specularpower 0.72
	diffusemap "textures/wall/MetalWall_01c_c" 
	normalmap "textures/wall/MetalWall_01c_n" alpha
}

shader "textures/wall/H_Light_01"
{
	specular $nmapalpha
  specularpower 0.52
	diffusemap "textures/wall01/H_Light_01_c" 
	normalmap "textures/wall01/H_Light_01_n" alpha
}

shader "textures/wall/H_Light_01a"
{
	specular $nmapalpha
  specularpower 0.52
	diffusemap "textures/wall01/H_Light_01a_c" 
	normalmap "textures/wall01/H_Light_01_n" alpha
}

shader "textures/wall/H_MetalWall_05"
{
	specular 0.505
 
	diffusemap "textures/wall01/H_MetalWall_05_c" 
	normalmap "textures/wall01/H_MetalWall_05_n"
}



// some terrain for final level
shader "textures/terrain/ground01_256"
{
   tesssize 256
	surfparm stone

	specular $nmapalpha
	specularpower 0.505
	parallaxscale 0.032

	diffusemap "textures/terrain/ground01_c"
	normalmap "textures/terrain/ground01_n"
	heightmap "textures/terrain/ground01_h"

}

shader "textures/terrain/rockbig1_256"
{
   tesssize 128
   surfparm stone
	specular $nmapalpha
	parallaxscale 0.033
   specularpower 0.605

	diffusemap "textures/terrain/RockBig_c"
	normalmap "textures/terrain/RockBig_N" alpha
	heightmap "textures/terrain/RockBig_h"
}


