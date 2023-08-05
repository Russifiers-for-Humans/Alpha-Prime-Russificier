shader "obj/physics/barrier_01"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/barrier_01_c"
  	normalmap "obj/physics/barrier_01_n" alpha
}

shader "obj/physics/chair_03"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/chair_03_c"
  	normalmap "obj/physics/chair_03_n" alpha
}

shader "obj/physics/chair_02"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/chair_02_c"
  	normalmap "obj/physics/chair_02_n" alpha
}

shader "tin"
{
castshadow
	specular 1.0
	reflection 0.8
	fresnel 0.301

	diffusemap "obj/physics/tinsphere"
	envmap $nearestcube
	normalmap "textures/trim/trimgrating1_n" alpha
	//diffusemap "TEXTURES/misc/tin_c"
}

shader "buggy"
{
   castshadow
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/transporter_02_c"
	normalmap "obj/vehicles/transporter_02_n" alpha
}


shader "obj/physics/pack_01"
{
   castshadow
	specular 0.2
	specularpower 0.92

	diffusemap "obj/physics/pack_01_c"
}

shader "obj/physics/offc_01"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/offc_01_c"
  	normalmap "obj/physics/offc_01_n" alpha
}

shader "obj/physics/cooling_fan_01"
{
//   surfparm trans
//   surfparm nomarks
//   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/physics/cooling_fan_01_c" alpha
  	normalmap "obj/physics/cooling_fan_01_n" alpha
}

shader "obj/physics/block_01"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/block_01_c"
  	normalmap "obj/physics/block_01_n" alpha
}

shader "obj/physics/notebook_01"
{
   castshadow
	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/physics/notebook_01_c"
  	normalmap "obj/physics/notebook_01_n" alpha
}

shader "obj/physics/folder01a"
{
   castshadow
	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/physics/folder01a_c"
  	normalmap "obj/physics/folder01_n" alpha
}

shader "obj/physics/tool_c"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/tool_c_c"
  	normalmap "obj/physics/tool_c_n" alpha
}

shader "obj/physics/tool_b"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/physics/tool_b_c" alpha
	normalmap "obj/physics/tool_b_n" alpha
}

shader "obj/physics/tool_a"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/tool_a_c"
  	normalmap "obj/physics/tool_a_n" alpha
}

shader "obj/physics/barrel01"
{
   castshadow
	specular $nmapalpha
	specularpower 0.43

	diffusemap "obj/physics/barrel01_c"
  	normalmap "obj/physics/barrel01_n" alpha
}

shader "obj/physics/gas_bottle01"
{
   castshadow
	specular $nmapalpha
	specularpower 0.43

	diffusemap "obj/physics/gas_bottle01_c"
  	normalmap "obj/physics/gas_bottle01_n" alpha
}

shader "obj/physics/case2"
{
   castshadow
	specular $nmapalpha

	diffusemap "obj/physics/case2_c"
  	normalmap "obj/physics/case2_n" alpha
}

shader "obj/physics/case1"
{
   castshadow
	specular $nmapalpha

	diffusemap "obj/physics/case01_c"
  	normalmap "obj/physics/case01_n" alpha
}

shader "obj/physics/case1b"
{
   castshadow
	specular $nmapalpha

	diffusemap "obj/physics/case01a_c"
  	normalmap "obj/physics/case01_n" alpha
}

//PhysX thousands debrises - cheapest as possible!
shader "debris"
{
	castshadow
	//specular $nmapalpha
	//cull none

	color (_eclass Color)
	blend invsrcalpha srcalpha

	diffusemap "obj/physics/debris_c"
//  	normalmap "obj/physics/debris_n" alpha
}

shader "obj/physics/debris"
{
	castshadow
	specular $nmapalpha
	cull none
	
	diffusemap "obj/physics/debris_c"
  	normalmap "obj/physics/debris_n" alpha
}

shader "obj/physics/exstbox"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physics/exstbox_c"
  	normalmap "obj/physics/exstbox_n" alpha
}

shader "obj/physics/exstinquisher"
{
   castshadow
	specular $nmapalpha

	diffusemap "obj/physics/exstinquisher_c"
  	normalmap "obj/physics/exstinquisher_n" alpha
}

shader "obj/physics/turbine_a"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/physics/turbine_a_c" alpha
	normalmap "obj/physics/turbine_a_n" alpha
}

shader "obj/physics/pillar1"
{
   castshadow
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal01_c"
  	normalmap "textures/trim/TrimMetal01_n" alpha
}

shader "obj/physics/turbine_b"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/physics/turbine_b_c" alpha
	normalmap "obj/physics/turbine_b_n" alpha
}

shader "obj/physics/turbine_c"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/turbine_c_c"
  	normalmap "obj/physics/turbine_c_n" alpha
}

shader "obj/physics/barbell2"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 1

	diffusemap "obj/physics/barbell2_c"
	normalmap "obj/physics/barbell2_n" alpha
}

shader "obj/physics/barbell1"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 1

	diffusemap "obj/physics/barbell1_c"
	normalmap "obj/physics/barbell1_n" alpha
}

shader "obj/physics/trashb_2"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/trashb_2_c"
  	normalmap "obj/physics/trashb_2_n" alpha
}

shader "obj/physics/t_lamp"
{
	castshadow
	specular $nmapalpha
	specularpower 0.43

	diffusemap "obj/physics/t_lamp_c"
  	normalmap "obj/physics/t_lamp_n" alpha
}

shader "obj/physics/aplayer"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/aplayer_c"
  	normalmap "obj/physics/aplayer_n" alpha
}

shader "obj/physics/enterance_panel1"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/enterance_panel1_c"
  	normalmap "obj/physics/enterance_panel1_n" alpha
}

shader "obj/physics/enterance_panel2"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/enterance_panel2_c"
  	normalmap "obj/physics/enterance_panel2_n" alpha
}

shader "obj/physics/thermos"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/thermos_c"
  	normalmap "obj/physics/thermos_n" alpha
}

shader "obj/physics/plate"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/plate_c"
  	normalmap "obj/physics/plate_n" alpha
}

shader "obj/physics/knife"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	specular $nmapalpha
	specularpower 0.43

	alphatest 16
castshadow
	diffusemap "obj/physics/knife_c"
	normalmap "obj/physics/knife_n" alpha
}

shader "obj/physics/fork"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	specular $nmapalpha
	specularpower 0.43

	alphatest 16
castshadow
	diffusemap "obj/physics/fork_c"
	normalmap "obj/physics/fork_n" alpha
}

shader "obj/physics/spoon"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	specular $nmapalpha
	specularpower 0.43

	alphatest 16
castshadow
	diffusemap "obj/physics/spoon_c"
	normalmap "obj/physics/spoon_n" alpha
}

shader "obj/physics/trashb_1"
{
	castshadow
	specular $nmapalpha
	specularpower 0.43

	diffusemap "obj/physics/trashb_1_c"
  	normalmap "obj/physics/trashb_1_n" alpha
}

shader "obj/physics/ash-cup"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/ash-cup_c"
  	normalmap "obj/physics/ash-cup_n" alpha
}

shader "obj/physics/cigarettes"
{
  castshadow
	specular $alpha
	specularpower 0.77

	diffusemap "obj/physics/cigarettes" alpha
}

shader "obj/physics/boxik_2"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/boxik_2_c"
  normalmap "obj/physics/boxik_2_n" alpha
}

shader "obj/physics/boxik_2a"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/boxik_2a_c"
  normalmap "obj/physics/boxik_2_n" alpha
}

shader "obj/physics/boxik_2b"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/boxik_2b_c"
  normalmap "obj/physics/boxik_2_n" alpha
}

shader "obj/physics/k_table"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/k_table_c"
  	normalmap "obj/physics/k_table_n" alpha
}

shader "obj/physics/boxik"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/boxik_a_c"
  	normalmap "obj/physics/boxik_a_n" alpha
}

shader "obj/physics/boxik_b"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/boxik_b_c"
  	normalmap "obj/physics/boxik_b_n" alpha
}

shader "obj/physics/boxik_c"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/boxik_c_c"
  	normalmap "obj/physics/boxik_c_n" alpha
}

shader "obj/physics/rack1"
{
  castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/rack1_c"
  	normalmap "obj/physics/rack1_n" alpha
}

shader "obj/static/bkos"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/bkos_c"
  	normalmap "obj/static/bkos_n" alpha
}

shader "obj/physics/bmic"
{
	castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/physics/bmic_c"
  	normalmap "obj/physics/bmic_n" alpha
}

shader "obj/sharedtextures/box"
{
	castshadow
	specular $nmapalpha

	diffusemap "obj/sharedtextures/box_c"
	normalmap "obj/sharedtextures/box_n" alpha
}

shader "obj/sharedtextures/can"
{
	castshadow
	specular $nmapalpha

	diffusemap "obj/sharedtextures/can_c"
	normalmap "obj/sharedtextures/can_n" alpha
}

shader "obj/sharedtextures/can2"
{
	castshadow
	specular $nmapalpha

	diffusemap "obj/sharedtextures/can2_c"
	normalmap "obj/sharedtextures/can2_n" alpha
}

shader "obj/sharedtextures/can2b"
{
	castshadow
	specular $nmapalpha

	diffusemap "obj/sharedtextures/can2b_c"
	normalmap "obj/sharedtextures/can2_n" alpha
}

shader "obj/sharedtextures/chair_01"
{
	castshadow
	specular $nmapalpha

	diffusemap "obj/sharedtextures/chair_01_c"
	normalmap "obj/sharedtextures/chair_01_n" alpha
}


shader "obj/sharedtextures/fluidblue"
{
	map nolight
	specular 1.0
	specularpower 0.23

	//blend one one
	diffusemap "obj/sharedtextures/fluidblue"
}

shader "obj/sharedtextures/fluidorange"
{
	map nolight
	specular 1.0
	specularpower 0.23

	//blend one one
	diffusemap "obj/sharedtextures/fluidorange"
}

shader "obj/sharedtextures/lightwhite"
{
	map nolight

	diffusemap "obj/sharedtextures/lightwhite"
}

