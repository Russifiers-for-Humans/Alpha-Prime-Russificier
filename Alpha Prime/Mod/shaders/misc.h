shader "textures/misc/bar_tabledesk"
{
	specular $nmapalpha
castshadow
	diffusemap "textures/misc/bar_tabledesk_c"
	normalmap "textures/misc/bar_tabledesk_n" alpha
}

shader "obj/misc/welldrill_01_top"
{
	specular $nmapalpha

	diffusemap "obj/misc/welldrill_01_top_c"
	normalmap "obj/misc/welldrill_01_top_n" alpha
}

shader "obj/misc/welldrill_01"
{
	specular $nmapalpha

	diffusemap "obj/misc/welldrill_01_c"
	normalmap "obj/misc/welldrill_01_n" alpha
}

shader "obj/misc/decoration_01"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
//	castshadow
	cull none
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/misc/decoration_01_c" alpha
	normalmap "obj/misc/decoration_01_n" alpha
}

shader "textures/misc/pipe_inside01"
{
	specular $nmapalpha
	specularpower 0.78

	diffusemap "textures/misc/pipe_inside01_c"
  	normalmap "textures/misc/pipe_inside01_n" alpha
}

shader "textures/misc/metal_burn"
{
	specular $nmapalpha
	specularpower 0.78

	diffusemap "textures/misc/metal_burn_c"
  	normalmap "textures/misc/metal_burn_n" alpha
}

shader "obj/misc/bottle_01_opaque"
{
   castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/misc/bottle_01_c"
	normalmap "obj/misc/bottle_01_n" alpha
}

shader "obj/misc/bottle_01_s0"
{
//   surfparm trans
//   surfparm nomarks
castshadow
	cull none
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/misc/bottle_01_s0_c" alpha
	normalmap "obj/misc/bottle_01_s0_n" alpha
}

shader "obj/misc/bottle_01_s1"
{
	cull none
	specular $nmapalpha
	specularpower 0.42
castshadow
	alphatest 126

	diffusemap "obj/misc/bottle_01_s1_c" alpha
	normalmap "obj/misc/bottle_01_s1_n" alpha
}

shader "obj/misc/bottle_01_s2"
{
	cull none
	specular $nmapalpha
	specularpower 0.42
castshadow
	alphatest 126

	diffusemap "obj/misc/bottle_01_s2_c" alpha
	normalmap "obj/misc/bottle_01_s2_n" alpha
}

shader "obj/misc/bottle_02_s3"
{
	cull none
	specular $nmapalpha
	specularpower 0.42
castshadow
	alphatest 126

	diffusemap "obj/misc/bottle_02_s3_c" alpha
	normalmap "obj/misc/bottle_02_s3_n" alpha
}

shader "obj/misc/bottle_02_s6"
{
	cull none
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/misc/bottle_02_s6_c"
	normalmap "obj/misc/bottle_02_s6_n" alpha
}

shader "obj/misc/bottle_01_l0"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0
	color 0xff0096ff
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/bottle_01_c" alpha
	normalmap "obj/misc/bottle_01_n" alpha
	envmap $nearestcube
}

shader "obj/misc/bottle_01_l1"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0
	color 0xff0018ff
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/bottle_01_c" alpha
	normalmap "obj/misc/bottle_01_n" alpha
	envmap $nearestcube
}

shader "obj/misc/bottle_01_l2"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0
	color 0xffc343ff
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/bottle_01_c" alpha
	normalmap "obj/misc/bottle_01_n" alpha
	envmap $nearestcube
}

shader "obj/misc/bottle_01_l3"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0
	color 0xffff4e00
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/bottle_01_c" alpha
	normalmap "obj/misc/bottle_01_n" alpha
	envmap $nearestcube
}

shader "obj/misc/bottle_01_l4"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0
	color 0xff00ff00
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/bottle_01_c" alpha
	normalmap "obj/misc/bottle_01_n" alpha
	envmap $nearestcube
}

shader "obj/misc/bottle_01"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/bottle_01_c" alpha
	normalmap "obj/misc/bottle_01_n" alpha
	envmap $nearestcube
}

shader "obj/misc/glass_04"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/glass_04_c" alpha
	normalmap "obj/misc/glass_04_n" alpha
	envmap $nearestcube
}

shader "obj/misc/glass_02a"
{
	surfparm trans
	specular 1
	specularpower 0.63
	reflection 1
	fresnel 1.0

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/glass_02_c" alpha
	normalmap "obj/misc/glass_02_n" alpha
	envmap $nearestcube
}

shader "obj/misc/glass_02b"
{
	specular $nmapalpha
	specularpower 0.77

	diffusemap "obj/misc/glass_02_c"
  	normalmap "obj/misc/glass_02_n" alpha
}

shader "obj/misc/glass_01"
{
	surfparm trans
	specular 1
	specularpower 0.62
	reflection 1
	fresnel 1.0

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/misc/glass_01_c" alpha
	normalmap "obj/misc/glass_01_n" alpha
	envmap $nearestcube
}

shader "obj/items/item_box"
{
	specular $nmapalpha
	specularpower 0.77
castshadow
	diffusemap "obj/items/item_box_c"
  	normalmap "obj/items/item_box_n" alpha
}

shader "obj/misc/valve_01"
{
	specular $nmapalpha
	specularpower 0.77

	diffusemap "obj/misc/valve_01_c"
  	normalmap "obj/misc/valve_01_n" alpha
}

shader "obj/misc/hall_loader"
{
	specular $nmapalpha
	specularpower 0.77

	diffusemap "obj/misc/hall_loader_c"
  	normalmap "obj/misc/hall_loader_n" alpha
}

shader "heavyTurret_light"
{
	map nolight
	pass
	tcmod shift g_fTime 0
	diffusemap "obj/weapons/grenadelights"
}

shader "heavyTurret"
{
	specular $nmapalpha
	specularpower 0.78
castshadow
	diffusemap "obj/misc/heavyTurret_c"
  	normalmap "obj/misc/heavyTurret_n" alpha
}

shader "textures/misc/ballcounter"
{
	surfparm trans
	surfparm nomarks
	surfparm noimpact
	surfparm nonsolid
	surfparm nocastshadow 
	surfparm detail
	surfparm nolightmap

	alphatest 1 // zatim
	sort translucent
	map nolight

	blend one one//invsrcalpha srcalpha

	diffusemap
		anim func_image image_frame
		"textures/misc/bball_00"
		"textures/misc/bball_01"
		"textures/misc/bball_02"
		"textures/misc/bball_03"
		"textures/misc/bball_04"
		"textures/misc/bball_05"
}
shader "Gate01"
{
   // castshadow
	specular $nmapalpha
	specularpower 0.82

	diffusemap "obj/misc/Gate01_c"
  	normalmap "obj/misc/Gate01_n" alpha
}

shader "s-debna_00_C"
{
	castshadow
	specular $alpha

	diffusemap "obj/fps/debna_00_C"
  	normalmap "obj/fps/debna_00_N"
}

shader "meqon"
{
	castshadow
	specular $alpha

	diffusemap "textures/nr_test/mriz_02_C"
	normalmap "textures/nr_test/mriz_02_N"
}

shader "vl-bigdoors"
{
	specular $alpha

	diffusemap "textures/d3test/bigdoors_c"
	normalmap "textures/d3test/bigdoors_n"
}

shader "vl-trimsteelb"
{
	specular $alpha
castshadow
	diffusemap "textures/d3test/bigdoors_c"
	normalmap "textures/d3test/bigdoors_n"
}

shader "s-bigdoors"
{
	specular $alpha

	diffusemap "textures/d3test/bigdoors_c"
	normalmap "textures/d3test/bigdoors_n"
}

shader "s-trimsteelb"
{
	specular $alpha
castshadow
	diffusemap "textures/d3test/bigdoors_c"
	normalmap "textures/d3test/bigdoors_n"
}


shader "obj/misc/analyst_head"
{
	castshadow
	specular $alpha

	diffusemap "obj/misc/analyst_head"
	normalmap "obj/misc/analyst1_body_N"
}
shader "obj/misc/analyst_view"
{
	specular $alpha

	diffusemap "obj/misc/analyst_view"
	normalmap "obj/misc/analyst1_body_N"
}
shader "obj/misc/analyst1_body"
{
	castshadow
	specular $alpha

	diffusemap "obj/misc/analyst1_body_C"
	normalmap "obj/misc/analyst1_body_N"
}

shader "textures/common/bluescreen"
{
   surfparm nolightmap
   effect "bluescreen"
}


shader "obj/misc/DvereTest"
{
	specular $alpha

	diffusemap "obj/misc/DvereTest"
  	normalmap "obj/parts/pvc00_n"
}

shader "obj/editarrow"
{
	layer "obj/weapons/streak"
	tex0 = layer 0
	effect "nolightmap"
}

// *** STATIC DISPLAYS

shader "textures/misc/display01static"
{
//castshadow 
	surfparm trans
	surfparm nomarks
	surfparm noimpact
	surfparm nonsolid
	surfparm castshadow 
	surfparm detail
	surfparm nolightmap

	sort translucent
	map nolight

	blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/display01_bcg" alpha
}

shader "textures/misc/display02static"
{
//castshadow
	surfparm trans
	surfparm nomarks
	surfparm noimpact
	surfparm nonsolid
	surfparm castshadow 
	surfparm detail
	surfparm nolightmap

	sort translucent
	map nolight

	blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/display02_bcg" alpha
}

shader "textures/misc/display03static"
{
//castshadow
	surfparm trans
	surfparm nomarks
	surfparm noimpact
	surfparm nonsolid
	surfparm castshadow 
	surfparm detail
	surfparm nolightmap

	sort translucent
	map nolight

	blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/display03_bcg" alpha
}

shader "textures/misc/display04static"
{
//castshadow
	surfparm trans
	surfparm nomarks
	surfparm noimpact
	surfparm nonsolid
	surfparm castshadow 
	surfparm detail
	surfparm nolightmap

	sort translucent
	map nolight

	blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/display04_bcg" alpha
}

shader "textures/misc/display05static"
{
//castshadow
	surfparm trans
	surfparm nomarks
	surfparm noimpact
	surfparm nonsolid
	surfparm castshadow 
	surfparm detail
	surfparm nolightmap

	sort translucent
	map nolight

	blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/display05_bcg" alpha
}


// *** STATIC DECALS ***

shader "textures/decals/blood01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 72

   sort mark
   alphatest 1
   map noambient
	
	blend srccolor zero
	diffusemap "textures/decals/blood1" alpha
}

shader "textures/decals/blood02"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 72

   sort mark
   alphatest 1
   map noambient
	
	blend srccolor zero
	diffusemap "textures/decals/blood2" alpha
}

shader "textures/decals/blood03"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 72

   sort mark
   alphatest 1  
   map noambient
	
	blend srccolor zero
	diffusemap "textures/decals/blood3" alpha
}

shader "textures/decals/blood04"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 72

   sort mark
   alphatest 1
   map noambient
	
	blend srccolor zero
	diffusemap "textures/decals/blood4" alpha
}

shader "textures/decals/DecDirt_01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 72

   sort mark
   alphatest 1
   blend invsrcalpha srcalpha

	diffusemap "textures/decals/DecDirt_01_c" alpha
}

shader "textures/decals/DecDirt_02"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 72

   sort mark
   alphatest 1
   blend invsrcalpha srcalpha

	diffusemap "textures/decals/DecDirt_02_c" alpha
}

shader "textures/decals/DecDirt_03"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   tesssize 72

   sort mark
   alphatest 1
   //specular $alpha
  
   blend invsrcalpha srcalpha

	diffusemap "textures/decals/DecDirt_03_c" alpha
	normalmap "textures/decals/DecDirt_03_n" alpha
}

shader "textures/decals/DecDirt_04"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   tesssize 36

   sort mark
   alphatest 1
  
   blend invsrcalpha srcalpha

	diffusemap "textures/decals/DecDirt_04_c" alpha
}

shader "textures/decals/DecOil_01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   tesssize 72

   sort mark
   specular $alpha
  
   blend invsrcalpha srcalpha

	diffusemap "textures/decals/DecOil_01_c" alpha
	normalmap "textures/decals/DecOil_01_n" alpha
}


// *** SIGNS *** 

shader "textures/misc/sign_storagea"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_storagea" alpha
}

shader "textures/misc/sign_storageb"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_storageb" alpha
}

shader "textures/misc/sign_quartersa"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_quartersa" alpha
}

shader "textures/misc/sign_freightcontrola"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_freightcontrola" alpha
}


shader "textures/misc/sign_fridge1"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_fridge1" alpha
}

shader "textures/misc/sign_fridge2"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_fridge2" alpha
}

shader "textures/misc/sign_relaxa"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/sign_relaxa" alpha
}


shader "textures/misc/sign_keepout"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

sort mark

  map nolight
  
  blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/sign_keepout"
}

shader "textures/misc/sign_keepoutcross"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

sort mark
  map nolight
  
  blend one one//invsrcalpha srcalpha

	diffusemap "textures/misc/sign_keepoutcross"
}



shader "textures/misc/stripe_white"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/stripe_white" alpha
}

shader "textures/misc/stripe_blue"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/stripe_blue" alpha
}

shader "textures/decals/papers1"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/papers_01_c" alpha
}

shader "textures/decals/papers2"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/papers_02_c" alpha
}

shader "textures/misc/posters1"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/posters1" alpha
}

shader "textures/misc/posters2"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/posters2" alpha
}

shader "textures/misc/posters3"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/misc/posters3" alpha
}

shader "textures/decals/Num_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/Num_01_c" alpha
}

shader "textures/decals/Num_01a"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark

  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/Num_01a_c" alpha
}

shader "textures/decals/Num_02"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/Num_02_c" alpha
}

shader "textures/decals/Num_02a"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

sort mark

  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/Num_02a_c" alpha
}

shader "textures/decals/SignCaution_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark

  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/SignCaution_01_c" alpha
}

shader "textures/decals/SignCaution_02"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark

  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/SignCaution_02_c" alpha
}

shader "textures/decals/SignStep_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark

  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/SignStep_01_c" alpha
}

shader "textures/decals/SignStep_02"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/SignStep_02_c" alpha
}

shader "textures/decals/TrimYellow_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/TrimYellow_01_c" alpha
}

shader "textures/decals/TrimYellow_02"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/TrimYellow_02_c" alpha
}

shader "textures/decals/TrimRed_01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   sort mark
   blend invsrcalpha srcalpha
	diffusemap "textures/decals/TrimRed_01_c" alpha
}

shader "textures/decals/TrimBlue_01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   sort mark
   blend invsrcalpha srcalpha
	diffusemap "textures/decals/TrimBlue_01_c" alpha
}

shader "textures/decals/TrimGreen_01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   sort mark
   blend invsrcalpha srcalpha
	diffusemap "textures/decals/TrimGreen_01_c" alpha
}

shader "textures/decals/TrimBlack_01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   sort mark
   blend invsrcalpha srcalpha
	diffusemap "textures/decals/TrimBlack_01_c" alpha
}

shader "textures/decals/TrimWhite_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/TrimWhite_01_c" alpha
}


shader "textures/decals/Blood_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

tesssize 72

sort mark

  //specular $alpha
  
   blend invsrcalpha srcalpha

	//diffusemap "textures/decals/blood1" alpha
	diffusemap "textures/decals/DecBlood_01_c" alpha
	//normalmap "textures/decals/DecBlood_01_n" alpha
}

shader "textures/decals/Blood_02"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

tesssize 72

sort mark
  //specular $alpha
  
  blend invsrcalpha srcalpha
   //diffusemap "textures/decals/blood2" alpha
	diffusemap "textures/decals/DecBlood_02_c" alpha
	//normalmap "textures/decals/DecBlood_02_n" alpha
}

shader "textures/decals/Waste_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

tesssize 72

sort mark
  //specular $alpha
  
  blend invsrcalpha srcalpha
	diffusemap "textures/decals/Waste_01_c" alpha
	//normalmap "textures/decals/Waste_01_n" alpha
}

shader "textures/decals/Waste_02"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail

tesssize 72

sort mark
  //specular $alpha
  
  blend invsrcalpha srcalpha
	diffusemap "textures/decals/Waste_02_c" alpha
	normalmap "textures/decals/Waste_02_n" alpha
}

shader "textures/misc/warnlight_pressure"
{
   surfparm trans
   surfparm nomarks
   surfparm noimpact
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

  sort translucent
  map nolight
  
  blend one one//invsrcalpha srcalpha
	diffusemap
		anim 1
		"textures/misc/warnlight_pressurea"
		"textures/misc/warnlight_pressureb"

   qer_editorimage "textures/misc/warnlight_pressurea" 
}



shader "textures/backgrounds/Industry01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail

   tesssize 72

   sort translucent
  //specular $alpha
  
  blend invsrcalpha srcalpha
  map nolight
  alphatest 24

	diffusemap "textures/backgrounds/Industry01_c" alpha
//	normalmap "textures/backgrounds/Industry01_n" alpha
}

shader "textures/misc/font01"
{
   surfparm trans
   surfparm nonsolid
   surfparm nocastshadow 
   surfparm detail
   sort mark

   blend invsrcalpha srcalpha

	diffusemap "textures/misc/font01" alpha
}

shader "textures/decals/SignDoNotBlock_01"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark

  
  blend invsrcalpha srcalpha

	diffusemap "textures/decals/SignDoNotBlock_01_c" alpha
}
