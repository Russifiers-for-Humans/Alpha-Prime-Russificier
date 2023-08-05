// pro pokusy a testovani

shader "livia"
{
	surfparm flesh
	//castshadow
	specular $nmapalpha
	specularpower 0.51
	surfparm nomarks
	
	diffusemap "obj/creatures/Liv_head_c"
  	normalmap "obj/creatures/Liv_head_n" alpha
}


shader "warenhead2"
{
	surfparm flesh
	//castshadow
	specular $nmapalpha
	specularpower 0.52
	surfparm nomarks
	
	diffusemap "obj/creatures/warenhead_c"
  	normalmap "obj/creatures/warenhead_n" alpha
}

shader "com1"
{
	surfparm flesh
	//castshadow
	specular $nmapalpha
	specularpower 0.52
	surfparm nomarks
	
	diffusemap "obj/creatures/Com1_c"
  	normalmap "obj/creatures/Com1_n" alpha
}


shader "dead_head"
{
	surfparm flesh
	castshadow
	specular $nmapalpha
	specularpower 0.52
	
	diffusemap "obj/creatures/dead/dead_head_c"
  	normalmap "obj/creatures/dead/dead_head_n" alpha
}


shader "dead"
{
	surfparm flesh
	castshadow
	specular $nmapalpha
	specularpower 0.52

	diffusemap "obj/creatures/dead/dead_c"
  	normalmap "obj/creatures/dead/dead_n" alpha
}

// !!@@##

shader "lcd02"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physics/lcd02_c"
  	normalmap "obj/physics/lcd02_n" alpha
}

shader "lcd01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physics/lcd01_c"
  	normalmap "obj/physics/lcd01_n" alpha
}
shader "lcd_display"
{
   surfparm detail
   surfparm nomarks
castshadow
	specular 0.2
	specularpower 0.32

	map nolight

#ifdef _XBOX
	pass
		tcmod shift 0.0 g_fTime10

	diffusemap "obj/static/usedisp01_c"
#else
	diffusemap $gui0
#endif
}

shader "lcd_display2"
{
   surfparm detail
   surfparm nomarks
castshadow
	specular 0.2
	specularpower 0.32

	map nolight

#ifdef _XBOX
	pass
		tcmod shift 0.0 g_fTime10

	diffusemap "obj/static/usedisp01_c"
#else
	diffusemap $renderview
#endif
}

shader "elevator-big-01"
{
	castshadow
	specular $nmapalpha

	diffusemap "obj/elevators/e_big01_c"
  	normalmap "obj/elevators/e_big01_n" alpha
}
shader "elevator-big-01-lights1"
{
	//castshadow
   map nolight
   color g_iColorElevatorBig01_L1
   
	diffusemap "obj/elevators/e_big01_c"
  	normalmap "obj/elevators/e_big01_n" alpha
}
shader "elevator-big-01-lights2"
{
	//castshadow
   map nolight
   color g_iColorElevatorBig01_L2
   
	diffusemap "obj/elevators/e_big01_c"
  	normalmap "obj/elevators/e_big01_n" alpha
}
shader "elevator-big-01-display"
{
   surfparm detail
   surfparm glass

	specular 0.2
	specularpower 0.32

	map nolight

#ifdef _XBOX
	pass
		tcmod shift 0.0 g_fTime10

	diffusemap "textures/glass/lcd1_c"
#else
	diffusemap $gui0
#endif
}

shader "obj/static/girder1ax"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
 castshadow  
	specular $nmapalpha
	specularpower 0.42

	cull none
	alphatest 16

	diffusemap "obj/static/girder1ax_c"
	normalmap "obj/static/girder1ax_n" alpha
}

shader "obj/physics/bota_1"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/physics/bota_1_c"
  	normalmap "obj/physics/bota_1_n" alpha
}


shader "container01"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.32
	diffusemap "obj/physics/container01_c"
  	normalmap "obj/physics/container01_n"
}

shader "obj/physicsparts/TransCart"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/physicsparts/TransCart_c" alpha
  	normalmap "obj/physicsparts/TransCart_n" alpha
}

shader "obj/lights/walltest"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/lights/walltest_c" alpha
  	normalmap "obj/lights/walltest_n" alpha
}

shader "obj/lights/walltest2"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/lights/walltest_c" alpha
  	normalmap "obj/lights/walltest_n" alpha
}

shader "obj/items/HeadSet_light"
{
   surfparm nolightmap
	surfparm trans
	surfparm nonsolid
   surfparm detail
   surfparm nomarks

	zwrite false
	sort translucent

	blend one srcalpha
	map nolight

	diffusemap "textures/light/lightconcrete_white"
} 

shader "obj/items/HeadSet"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/items/HeadSet_c" alpha
  	normalmap "obj/items/HeadSet_n" alpha
}



shader "obj/misc/ZachrannyModul"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/misc/ZachrannyModul_c" alpha
  	normalmap "obj/misc/ZachrannyModul_n" alpha
}

shader "obj/physicsparts/sentrygun"
{
	castshadow
	specular $alpha     
	specularpower 0.77

	diffusemap "obj/physicsparts/sentrygun_c"
  	normalmap "obj/physicsparts/sentrygun_n"
}

shader "obj/static/cryodevice"
{
	surfparm metal
	specular $alpha
	specularpower 0.77
castshadow
	diffusemap "obj/static/cryodevice_c"
  	normalmap "obj/static/cryodevice_n"
}

shader "obj/static/cryodevice_glass"
{
	surfparm glass
	surfparm trans
	surfparm nocastshadow

	specular 1
	specularpower 0.42
   reflection 0.8
   map nolight
   
   blend invsrcalpha srcalpha

	diffusemap "textures/glass/cryo1" alpha
	envmap $nearestcube
}

shader "obj/elevators/elevator_01"
{
	specular $alpha
	specularpower 0.72

	diffusemap "obj/elevators/elevator_01_c"
  	normalmap "obj/elevators/elevator_01_n"
}


shader "obj/elevators/elevator_01_Display"
{
   surfparm detail
   surfparm glass

	specular 0.2	//$alpha
	specularpower 0.32

//	blend invsrcalpha srcalpha

	map nolight

#ifdef _XBOX
	pass
		tcmod shift 0.0 g_fTime10

	diffusemap "textures/glass/lcd1_c"
#else
	diffusemap $gui0
#endif
}

shader "obj/misc/zabradli_01"
{
	specular $alpha
castshadow
	diffusemap "obj/misc/zabradli_01_c"
	normalmap "obj/misc/zabradli_01_n"
}

shader "obj/columns/svetlosloup"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/columns/svetlosloup_c" alpha
  	normalmap "obj/columns/svetlosloup_n" alpha
}


/*    -------------  
	 j_test
       -------------   */	 

shader "obj/creatures/ruka"
{
	//castshadow
	specular $nmapalpha

	specularpower 0.52

	diffusemap "obj/creatures/ruka_c" alpha
  	normalmap "obj/creatures/ruka_n" alpha
}

shader "technicist_01_head"
{
	surfparm flesh
//	castshadow
	specular $nmapalpha
	specularpower 0.77
	surfparm nomarks

	diffusemap "obj/creatures/technicist_01_head_c"
  	normalmap "obj/creatures/technicist_01_head_n" alpha
}

shader "romale"
{
	surfparm flesh
//	castshadow
	specular $nmapalpha
	specularpower 0.77
	surfparm nomarks

	diffusemap "obj/creatures/prospector_head_c"
  	normalmap "obj/creatures/prospector_head_n" alpha
}

shader "Technik2_head"
{
	surfparm flesh
//	castshadow
	specular $nmapalpha
	specularpower 0.77
	surfparm nomarks

	diffusemap "obj/creatures/Technik2_head_c"
  	normalmap "obj/creatures/Technik2_head_n" alpha
}
