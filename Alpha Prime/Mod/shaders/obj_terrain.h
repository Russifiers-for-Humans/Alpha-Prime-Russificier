//basic default texture
shader "terrain/default"
{
	surfparm stone

	diffusemap "textures/terrain/default"
}


//kvuli radiantu
shader "textures/terrain/default"
{
	surfparm stone

	diffusemap "textures/terrain/default"
}

shader "textures/terrain/rockbig1"
{
   tesssize 512
   surfparm stone
	specular $nmapalpha
	//parallaxscale 0.028
   specularpower 0.6

	diffusemap "textures/terrain/RockBig_c"
	normalmap "textures/terrain/RockBig_N" alpha
	//heightmap "textures/terrain/RockBig_h"
}

//big rocks 
shader "terrain/rockbig1"
{
	tesssize 512
	surfparm stone
	specular $nmapalpha
	//parallaxscale 0.027
	specularpower 0.6

	diffusemap "textures/terrain/RockBig_c"
	normalmap "textures/terrain/RockBig_N" alpha
}


//small rocks
shader "terrain/ground01"
{
	surfparm beton

	specular $nmapalpha
	specularpower 0.5
	parallaxscale 0.036

	diffusemap "textures/terrain/ground01_c"
	normalmap "textures/terrain/ground01_n" alpha
	heightmap "textures/terrain/ground01_h"

}



//sand
shader "terrain/ground02"
{
	surfparm gravel
	surfparm dust

	specular $nmapalpha
	specularpower 0.08
//	parallaxscale 0.0053

	diffusemap "textures/terrain/ground02_c"
	normalmap "textures/terrain/ground02_n" alpha
//	heightmap "textures/terrain/ground02_h"
}


//dirty rocks?
shader "terrain/rock02"
{
	surfparm stone

	specular $nmapalpha
	specularpower 0.63
	parallaxscale 0.015


	diffusemap "textures/terrain/rock02_c"
	normalmap "textures/terrain/rock02_n"
	heightmap "textures/terrain/rock02_h" alpha
}


//large rocks 
shader "terrain/rock03"
{
	surfparm stone

	specular $nmapalpha
	specularpower 0.8
	parallaxscale 0.035


	diffusemap "textures/terrain/rock03_c"
	normalmap "textures/terrain/rock03_n" alpha
	heightmap "textures/terrain/rock03_h"
}


//road, need different
shader "terrain/rock_road"
{
	surfparm beton

	specular $nmapalpha
	specularpower 0.75

	parallaxscale 0.0193

	diffusemap "textures/terrain/road_c"
	normalmap "textures/terrain/road_n" alpha
	heightmap "textures/terrain/road_h"
}



shader "terrain/hubbardium01"
{
	surfparm slime

	diffusemap "textures/terrain/hubbardium01_c"
}