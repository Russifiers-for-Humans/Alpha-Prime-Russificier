
shader "textures/floor/wireclip"
{
	specular 1.0

	cull none
	alphatest 128
	
	diffusemap "textures/floor/wireclip_c"
	normalmap "textures/floor/wireclip_n"
}

shader "textures/floor/wiretrans"
{
	specular 1.0

	cull none
	alphatest 128

	diffusemap "textures/floor/wiretrans_c"
	normalmap "textures/floor/wiretrans_n"
}

//FIDO pokusy. Nemazat
shader "textures/floor/floorX"
{
	specular 1.0
	specularpower 0.483
	heightscale 0.104
	qer_editorimage "textures/floor/floor1"

	diffusemap "textures/heighttest/brickwall01_d"
	normalmap "textures/heighttest/brickwall01_n" alpha
	heightmap "textures/heighttest/brickwall01_h"
}

shader "textures/floor/floor1"
{
	specular $nmapalpha
	specularpower 0.483
	qer_editorimage "textures/floor/floor1"
	diffusemap "textures/floor/floor1_c"
	normalmap "textures/floor/floor1_n" alpha
}

shader "textures/floor/floor1b"
{
	specular $nmapalpha
  specularpower 0.482
	diffusemap "textures/floor/floor1b_c"
		qer_editorimage "textures/floor/floor1b"
	normalmap "textures/floor/floor1b_n" alpha
}

shader "textures/floor/floor1c"
{
	specular $nmapalpha
  specularpower 0.483
	diffusemap "textures/floor/floor1c_c"
		qer_editorimage "textures/floor/floor1c"
	normalmap "textures/floor/floor1c_n" alpha
}

shader "textures/floor/floor1c_a"
{
	specular $nmapalpha
  specularpower 0.483
	diffusemap "textures/floor/floor1c_a_c"
		qer_editorimage "textures/floor/floor1c_a"
	normalmap "textures/floor/floor1c_n" alpha
}

shader "textures/floor/floor2"
{
	specular $nmapalpha
  specularpower 0.533
  surfparm plastic

	diffusemap "textures/floor/floor2_c"
		qer_editorimage "textures/floor/floor2"
	normalmap "textures/floor/floor2_n" alpha
}

shader "textures/floor/floor2a"
{
	specular $nmapalpha
  specularpower 0.533
	diffusemap "textures/floor/floor2a_c"
		qer_editorimage "textures/floor/floor2"
	normalmap "textures/floor/floor2_n" alpha
}

shader "textures/floor/floor2b"
{
	specular $nmapalpha
  specularpower 0.533
	diffusemap "textures/floor/floor2b_c"
		qer_editorimage "textures/floor/floor2b"
	normalmap "textures/floor/floor2b_n" alpha
}

shader "textures/floor/floor2c"
{
	specular $nmapalpha
  specularpower 0.533
  surfparm rubber

	diffusemap "textures/floor/floor2c_c"
		qer_editorimage "textures/floor/floor2c"
	normalmap "textures/floor/floor2c_n" alpha
}

shader "textures/floor/floor2d"
{
	specular $nmapalpha
  specularpower 0.533
	diffusemap "textures/floor/floor2d_c"
		qer_editorimage "textures/floor/floor2d_c"
	normalmap "textures/floor/floor2_n" alpha
}

shader "textures/floor/floor3"
{
	specular $nmapalpha
  specularpower 0.533
	diffusemap "textures/floor/floor3_c"
	normalmap "textures/floor/floor3_n" alpha
}

shader "textures/floor/floor3a"
{
	specular $nmapalpha
  specularpower 0.533
	diffusemap "textures/floor/floor3a_c"
	normalmap "textures/floor/floor3_n" alpha
}

shader "textures/floor/ConcreteFloor_01"
{
   surfparm beton
	specular $nmapalpha
	diffusemap "textures/floor/ConcreteFloor_01_c"
		qer_editorimage "textures/floor/ConcreteFloor_01_c"
	normalmap "textures/floor/ConcreteFloor_01_n" alpha
}

// for debrises only!!!
shader "textures/floor/ConcreteFloor_02_pieces"
{
//castshadow
   surfparm beton
   surfparm castshadow
	specular $nmapalpha
	diffusemap "textures/floor/ConcreteFloor_02_c"
		qer_editorimage "textures/floor/ConcreteFloor_02_c"
	normalmap "textures/floor/ConcreteFloor_02_n" alpha
}

shader "textures/floor/ConcreteFloor_02"
{
   surfparm beton
	specular $nmapalpha
	diffusemap "textures/floor/ConcreteFloor_02_c"
		qer_editorimage "textures/floor/ConcreteFloor_02_c"
	normalmap "textures/floor/ConcreteFloor_02_n" alpha
}

shader "textures/floor/ConcreteFloor_03"
{
   surfparm beton
	specular $nmapalpha
	diffusemap "textures/floor/ConcreteFloor_03_c"
	normalmap "textures/floor/ConcreteFloor_03_n" alpha
}

shader "textures/floor/MetalFloor_01"
{
	specular $nmapalpha
  specularpower 0.534
	diffusemap "textures/floor/MetalFloor_01_c"
	normalmap "textures/floor/MetalFloor_01_n" alpha
}

shader "textures/floor/MetalFloor_02"
{
	specular $nmapalpha
  specularpower 0.534
	diffusemap "textures/floor/MetalFloor_02_c"
	normalmap "textures/floor/MetalFloor_02_n" alpha
}

shader "textures/floor/MetalFloor_02a"
{
	specular $nmapalpha
  specularpower 0.534
	diffusemap "textures/floor/MetalFloor_02a_c"
	normalmap "textures/floor/MetalFloor_02a_n" alpha
}

shader "textures/floor/MetalFloor_03"
{
	specular $nmapalpha
  specularpower 0.534
	diffusemap "textures/floor/MetalFloor_03_c"
	normalmap "textures/floor/MetalFloor_03_n" alpha
}

shader "textures/floor/MetalFloor_03a"
{
	specular $nmapalpha
  specularpower 0.534
	diffusemap "textures/floor/MetalFloor_03a_c"
	normalmap "textures/floor/MetalFloor_03_n" alpha
}

shader "textures/floor/E_ConcreteFloor_01"
{
	specular $nmapalpha
  specularpower 0.51
   surfparm beton

	diffusemap "textures/floor/E_ConcreteFloor_01_c"
	normalmap "textures/floor/ConcreteFloor_02_n" alpha
}

shader "textures/floor/E_MetalFloor_01"
{
	specular $nmapalpha
  specularpower 0.534
	diffusemap "textures/floor/E_MetalFloor_01_c"
	normalmap "textures/floor/E_MetalFloor_01_n" alpha
}

shader "textures/floor/E_MetalFloor_02"
{
	specular $nmapalpha
  specularpower 0.534
  tesssize 128
  
	diffusemap "textures/floor/E_MetalFloor_02_c"
	normalmap "textures/floor/E_MetalFloor_02_n" alpha
}



shader "textures/floor/H_MetalFloor_01"
{
	specular $nmapalpha
	tesssize 144
  
	diffusemap "textures/floor/H_MetalFloor_01_c"
	normalmap "textures/floor/H_MetalFloor_01_n" alpha

}

shader "textures/floor/H_MetalFloor_01a"
{
	specular $nmapalpha
   reflection 0.13
	tesssize 144
   
	diffusemap "textures/floor/H_MetalFloor_01a_c"
	normalmap "textures/floor/H_MetalFloor_01_n" alpha
	envmap $nearestcube
}

shader "textures/floor/H_MetalFloor_01a_withspec"
{
	specular $nmapalpha
   reflection 0.13
   
	diffusemap "textures/floor/H_MetalFloor_01a_c"
	normalmap "textures/floor/H_MetalFloor_01_n" alpha
	envmap $nearestcube
}

shader "textures/floor/Trash"
{
	specular $nmapalpha
  specularpower 0.51
  tesssize 256
  
	diffusemap "textures/floor/Trash_01_c"
	normalmap "textures/floor/Trash_01_n" alpha
}

shader "textures/floor/new_floor01"
{
	specular $nmapalpha
  specularpower 0.53
  tesssize 144
  
	diffusemap "textures/floor/new_floor01_c"
	normalmap "textures/floor/new_floor01_n" alpha
}

shader "textures/floor/new_floor02"
{
	specular $nmapalpha
  specularpower 0.53
  tesssize 144
  
	diffusemap "textures/floor/new_floor02_c"
	normalmap "textures/floor/new_floor02_n" alpha
}
