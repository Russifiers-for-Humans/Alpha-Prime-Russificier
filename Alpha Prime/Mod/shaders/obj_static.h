
shader "Medifurniture01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/Medifurniture01_c"
	normalmap "obj/static/Medifurniture01_n" alpha

}

shader "MediScreen01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/MediScreen01_c"
	normalmap "obj/static/MediScreen01_n" alpha

}

shader "MortuaryFridge01b"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/MortuaryFridge01b_c"
	normalmap "obj/static/MortuaryFridge01b_n" alpha

}

shader "MortuaryFridge01a"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/MortuaryFridge01a_c"
	normalmap "obj/static/MortuaryFridge01a_n" alpha

}


shader "MortuaryTable01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/MortuaryTable01_c"
	normalmap "obj/static/MortuaryTable01_n" alpha

}

shader "HospitalLight01"
{
	specular $nmapalpha

	diffusemap "obj/static/HospitalLight01_c"
	normalmap "obj/static/HospitalLight01_n" alpha

}


shader "HospitalBed01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/HospitalBed01_c"
	normalmap "obj/static/HospitalBed01_n" alpha

}

shader "obj/static/tower01"
{
	specularpower 0.52
castshadow
	diffusemap "obj/static/tower01_c"

}


shader "obj/static/AutomatCookies2"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/AutomatCookies2_c"
	normalmap "obj/static/AutomatCookies2_n" alpha
}

shader "obj/static/AutomatCookies1"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/AutomatCookies1_c"
	normalmap "obj/static/AutomatCookies1_n" alpha
}

shader "obj/static/WaterTank01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/WaterTank01_c"
	normalmap "obj/static/WaterTank01_n" alpha
}

shader "obj/static/LightTrooperHelmet"
{
	surfparm flesh
	castshadow
	specular $nmapalpha
	specularpower 0.52
	surfparm nomarks

	diffusemap "obj/creatures/lighttrooper/helmet_c"
  	normalmap "obj/creatures/lighttrooper/helmet_n" alpha
}

shader "obj/static/boot_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/boot_01_c"
	normalmap "obj/static/boot_01_n" alpha
}
shader "obj/static/shoe_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/shoe_01_c"
	normalmap "obj/static/shoe_01_n" alpha
}

shader "obj/static/guard_helmet_visor"
{
	surfparm flesh
	specular $nmapalpha
	specularpower 0.52
//	surfparm nomarks
castshadow
	reflection $nmapalpha
	fresnel 0.5
	envmap $nearestcube

	diffusemap "obj/creatures/guard/guard_helmet_c"
  	normalmap "obj/creatures/guard/guard_helmet_n" alpha
}





shader "obj/static/bar_stool_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/bar_stool_01_c"
	normalmap "obj/static/bar_stool_01_n" alpha
}

shader "obj/static/bed_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/bed_01_c"
	normalmap "obj/static/bed_01_n" alpha
}

shader "obj/static/deadLeg"
{
	surfparm flesh
	castshadow
	specular $nmapalpha
	specularpower 0.52
	surfparm nomarks
	
	diffusemap "obj/static/DeadLeg_c"
  	normalmap "obj/static/DeadLeg_n" alpha
}

shader "obj/static/valve_02"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/valve_02_c"
	normalmap "obj/static/valve_02_n" alpha
}

shader "deadhead"
{
	surfparm flesh
	castshadow
	specular $nmapalpha
	specularpower 0.52
	surfparm nomarks
	
	diffusemap "obj/static/deadhead_c"
  	normalmap "obj/static/deadhead_n" alpha
}


shader "obj/static/t_table"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/t_table_c"
	normalmap "obj/static/t_table_n" alpha
}

shader "obj/static/device_04"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/device_04_c"
	normalmap "obj/static/device_04_n" alpha
}

shader "obj/static/generator2"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/generator2_c"
	normalmap "obj/static/generator2_n" alpha
}

shader "obj/static/HealthDevice"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/HealthDevice_c"
	normalmap "obj/static/HealthDevice_n" alpha
}

shader "obj/static/OxygenDevice"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/OxygenDevice_c"
	normalmap "obj/static/OxygenDevice_n" alpha
}

shader "obj/static/target_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/target_01_c"
	normalmap "obj/static/target_01_n" alpha
}

shader "obj/static/automat_01_light"
{
	specular 0.5
   map nolight
   
	diffusemap "obj/static/automat_01_c"
}

shader "obj/static/automat_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/automat_01_c"
	normalmap "obj/static/automat_01_n" alpha
}

shader "obj/static/automat_01a_light"
{
	specular 0.5
   map nolight
   
	diffusemap "obj/static/automat_01a_c"
}

shader "obj/static/automat_01a"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/automat_01a_c"
	normalmap "obj/static/automat_01_n" alpha
}

shader "obj/static/cdisplay_01_light"
{
	specular 0.5
   map nolight
   
	diffusemap "obj/static/cdisplay_01_c"
}

shader "obj/static/cdisplay_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/cdisplay_01_c"
	normalmap "obj/static/cdisplay_01_n" alpha
}

shader "obj/static/casing_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/casing_01_c"
	normalmap "obj/static/casing_01_n" alpha
}

shader "obj/static/diagscanner_02"
{
	specular $nmapalpha

	diffusemap "obj/static/diagscanner_02_c"
	normalmap "obj/static/diagscanner_02_n" alpha
}

shader "obj/static/locker1"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/locker1_c"
	normalmap "obj/static/locker1_n" alpha
}

shader "obj/static/locker1a"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/locker1a_c"
	normalmap "obj/static/locker1_n" alpha
}

shader "obj/static/rack3"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/rack3_c"
	normalmap "obj/static/rack3_n" alpha
}

shader "obj/static/forge_low_cabels"
{
   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm rubber
castshadow
	specular $alpha

	cull none
	alphatest 16


	diffusemap "obj/static/Forge_low_cabels_c" alpha
	normalmap "obj/static/Forge_low_cabels_n"
}

shader "obj/static/forge_low03"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/Forge_low03_c"
	normalmap "obj/static/Forge_low03_n" alpha
}

shader "obj/static/forge_low02"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/Forge_low02_c"
	normalmap "obj/static/Forge_low02_n" alpha
}

shader "obj/static/forge_low"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/Forge_low01_c"
	normalmap "obj/static/Forge_low01_n" alpha
}


shader "obj/static/engine3"
{
	specular $nmapalpha

	diffusemap "obj/static/engine3_c"
	normalmap "obj/static/engine3_n" alpha
}

shader "obj/static/engine3a"
{
	specular $nmapalpha

	diffusemap "obj/static/engine3a_c"
	normalmap "obj/static/engine3_n" alpha
}

shader "obj/static/devices_01-03"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/devices_01-03_c"
	normalmap "obj/static/devices_01-03_n" alpha
}

shader "obj/static/electro_case02_lights"
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
		"obj/static/electro_case02_lights01_c"
		"obj/static/electro_case02_lights02_c"
		"obj/static/electro_case02_lights03_c"
	
   qer_editorimage "obj/static/electro_case02_lights" 
}

shader "obj/static/electro_case02"
{

castshadow
	diffusemap "obj/static/electro_case02_c"
	normalmap "obj/static/electro_case02_n" alpha
}


shader "obj/static/electro_case01_lights"
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
		"obj/static/electro_case01_lights01_c"
		"obj/static/electro_case01_lights02_c"
		"obj/static/electro_case01_lights03_c"
		"obj/static/electro_case01_lights04_c"

   qer_editorimage "obj/static/electro_case01_lights" 
}


shader "obj/static/electro_case01"
{
castshadow
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/static/electro_case01_c"
	normalmap "obj/static/electro_case01_n" alpha
}

shader "obj/static/data_case"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/data_case_c"
	normalmap "obj/static/data_case_n" alpha
}



shader "obj/static/canister"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/canister_c"
	normalmap "obj/static/canister_n" alpha
}




shader "obj/static/control_board_02"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/control_board_02_c"
	normalmap "obj/static/control_board_02_n" alpha
}

shader "obj/static/control_board_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/control_board_01_c"
	normalmap "obj/static/control_board_01_n" alpha
}

shader "obj/static/control_board_map"
{
	specular $nmapalpha

	diffusemap "obj/static/control_board_map_c"
	normalmap "obj/static/control_board_map_n" alpha
}

shader "obj/static/seat_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/seat_01_c"
	normalmap "obj/static/seat_01_n" alpha
}

shader "obj/static/armchair_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/armchair_01_c"
	normalmap "obj/static/armchair_01_n" alpha
}

shader "obj/static/armchair_01a"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/armchair_01a_c"
	normalmap "obj/static/armchair_01_n" alpha
}

shader "obj/static/rlg"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/Railing_c"
	normalmap "obj/static/Railing_n" alpha
}

shader "obj/static/holder_hscreen_01"
{
	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/static/holder_hscreen_01_c"
  	normalmap "obj/static/holder_hscreen_01_n" alpha
}

shader "obj/static/holder_01"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/holder_01_c"
  	normalmap "obj/static/holder_01_n" alpha
}

shader "obj/static/control_table_04"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/control_table_04_c"
	normalmap "obj/static/control_table_04_n" alpha
}

shader "obj/static/control_table_04a"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/control_table_04a_c"
	normalmap "obj/static/control_table_04a_n" alpha
}

shader "obj/static/control_table_03"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/control_table_03_c"
	normalmap "obj/static/control_table_03_n" alpha
}


shader "obj/static/control_table_01"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/control_table_01_c"
  	normalmap "obj/static/control_table_01_n" alpha
}

shader "obj/static/washing_stand_01"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/washing_stand_01_c"
  	normalmap "obj/static/washing_stand_01_n" alpha
}

shader "obj/static/urinal_01"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/urinal_01_c"
  	normalmap "obj/static/urinal_01_n" alpha
}

shader "obj/static/health_box"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/health_box_c"
  	normalmap "obj/static/health_box_n" alpha
}

shader "obj/static/crane01"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/crane01_c"
  	normalmap "obj/static/crane01_n" alpha
}

shader "obj/static/emergency_kit_1"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/emergency_kit_1_c"
  	normalmap "obj/static/emergency_kit_1_n" alpha
}

shader "obj/static/Venting4"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/Venting4_c"
  	normalmap "obj/static/Venting4_n" alpha
}

shader "obj/static/Venting3"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/Venting3_c"
  	normalmap "obj/static/Venting3_n" alpha
}

shader "obj/static/console01"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/console01_c"
  	normalmap "obj/static/console01_n" alpha
}

shader "obj/static/engine1"
{
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/static/engine01_c"
  	normalmap "obj/static/engine01_n" alpha
}

shader "obj/columns/column_04"
{
	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/columns/column_04_c"
  	normalmap "obj/columns/column_04_n" alpha
}

shader "obj/static/switch_device_1"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/switch_device_1_c"
  	normalmap "obj/static/switch_device_1_n" alpha
}

shader "obj/static/fire_alarm"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/Alarm01_c"
  	normalmap "obj/static/Alarm01_n" alpha
}

shader "obj/static/Control1"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/Control01_c"
  	normalmap "obj/static/Control01_n" alpha
}

shader "obj/static/Control1a"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/Control01a_c"
  	normalmap "obj/static/Control01a_n" alpha
}

shader "obj/static/Switch01"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/Switch01_c"
  	normalmap "obj/static/Switch01_n" alpha
}

shader "obj/lights/Light_2"
{
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/lights/Light_2_c"
  	normalmap "obj/lights/Light_2_n" alpha
}

shader "obj/static/grideriron1"
{
	specular $nmapalpha
	specularpower 0.43
castshadow
	diffusemap "obj/static/grideriron1_c"
  	normalmap "obj/static/grideriron1_n" alpha
}

shader "obj/static/generdevice"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/generdevice01_c"
  	normalmap "obj/static/generdevice01_n" alpha
}

shader "obj/static/generdevice_a"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "obj/static/generdevice01a_c"
  	normalmap "obj/static/generdevice01a_n" alpha
}

// meteorites!!
shader "obj/misc/meteorite"
{
	specular $nmapalpha

	surfparm nocastshadow

 map nolight
   
	diffusemap "obj/misc/meteorite_c"
 	normalmap "obj/misc/meteorite_n" alpha
}


// meteorites!!
shader "obj/misc/meteorite_NORM"
{
	specular $nmapalpha
	specularpower 0.11

	diffusemap "obj/misc/meteorite_c"
 	normalmap "obj/misc/meteorite_n" alpha
}


shader "obj/misc/meteorite_terrain"
{
	castshadow
	specular $nmapalpha
	specularpower 0.41

	diffusemap "textures/misc/meteorite_terrain_c"
 	normalmap "obj/misc/meteorite_n" alpha
}



shader "obj/static/rock1"
{
	specular $nmapalpha

	surfparm nocastshadow

   map nolight
   
	diffusemap "obj/misc/meteorite_c"
  	normalmap "obj/misc/meteorite_n" alpha
}


/*
shader "obj/static/rock1"
{
	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/static/rock1_c"
  	normalmap "obj/static/rock1_n" alpha
}
*/

shader "obj/static/fitness2"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
   
	castshadow
	specular $nmapalpha
	specularpower 0.41
castshadow
	alphatest 1

	diffusemap "obj/static/fitness2_c"
	normalmap "obj/static/fitness2_n" alpha
}

shader "obj/static/fitness1"
{
	castshadow
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/fitness1_c"
  	normalmap "obj/static/fitness1_n" alpha
}

shader "obj/static/generator1"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/Generator01_c"
  	normalmap "obj/static/Generator01_n" alpha
}

shader "obj/static/generator1a"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/Generator01a_c"
  	normalmap "obj/static/Generator01a_n" alpha
}

shader "obj/static/generator1b"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/Generator01b_c"
  	normalmap "obj/static/Generator01_n" alpha
}

shader "usable_display01"
{
	specular $nmapalpha

	diffusemap "obj/static/usedisp01_c"
  	normalmap "obj/static/usedisp01_n" alpha
}

shader "usable_display01_display"
{
   surfparm detail
   surfparm nomarks

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


shader "obj/static/cigamach"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/cigamach_c"
  	normalmap "obj/static/cigamach_n" alpha
}

shader "obj/static/cigamach_light"
{
	specular 0.5
   map nolight

	diffusemap "obj/static/cigamach_c"
}

shader "controltable01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/controltable01_c"
  	normalmap "obj/static/controltable01_n" alpha
}

shader "obj/static/girder1a"
{
	specular $nmapalpha
	specularpower 0.41
castshadow
	diffusemap "obj/static/girder1a_c"
  	normalmap "obj/static/girder1a_n" alpha
}

shader "obj/physics/floorpanel1"
{
	specular $nmapalpha
	specularpower 0.42
castshadow
	diffusemap "TEXTURES/floor/floor1c_c"
  	normalmap "TEXTURES/floor/floor1c_n" alpha
}

shader "display_ram01"
{
	specular $nmapalpha

	diffusemap "obj/static/display01_c"
	normalmap "obj/static/display01_n" alpha
}

shader "ceilgirder01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/ceilgirder01_c"
	normalmap "obj/static/ceilgirder01_n" alpha
}

shader "window01"
{
	specular $nmapalpha

	diffusemap "obj/static/window01_c"
	normalmap "obj/static/window01_n" alpha
}

shader "window01a"
{
	specular $nmapalpha

	diffusemap "obj/static/window01a_c"
	normalmap "obj/static/window01_n" alpha
}

shader "window01b"
{
	specular $nmapalpha

	diffusemap "obj/static/window01b_c"
	normalmap "obj/static/window01_n" alpha
}

shader "obj/sharedtextures/panels"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/sharedtextures/panels_c"
	normalmap "obj/sharedtextures/panels_n" alpha
}

shader "obj/static/venting"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/venting_c"
	normalmap "obj/static/venting_n" alpha
}

shader "obj/static/ventingfan"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/ventingfan_c"
	normalmap "obj/static/ventingfan_n" alpha
}

shader "obj/static/hatch_b"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/hatch_b_c"
	normalmap "obj/static/hatch_n" alpha
}

shader "obj/static/hatch"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/static/hatch_c"
	normalmap "obj/static/hatch_n" alpha
}

// *** LIGHTS ***

shader "obj/Lights/LightCeil_01"
{
	specular $alpha
	specularpower 0.623

	diffusemap "obj/Lights/LightCeil_01_c"
  	normalmap "obj/Lights/LightCeil_01_n"
}

shader "obj/Lights/LightCeil_02"
{
	specular $alpha
	specularpower 0.623

	diffusemap "obj/Lights/LightCeil_02_c"
  	normalmap "obj/Lights/LightCeil_02_n"
}

shader "obj/Lights/LightCeil_03"
{
	specular $alpha
	specularpower 0.623

	diffusemap "obj/Lights/LightCeil_03_c"
  	normalmap "obj/Lights/LightCeil_03_n"
}

shader "obj/lights/LightCeil_04"
{
	specular $nmapalpha

	specularpower 0.623

	diffusemap "obj/lights/LightCeil_04_c" alpha
  	normalmap "obj/lights/LightCeil_04_n" alpha
}


// *** STATIC ***
shader "obj/static/container02b"
{
	specular $nmapalpha
castshadow
	specularpower 0.42

	diffusemap "obj/static/container02b_c" alpha
  	normalmap "obj/static/container02b_n" alpha
}

shader "obj/static/container02a"
{
	specular $nmapalpha
castshadow
	specularpower 0.42

	diffusemap "obj/static/container02a_c" alpha
  	normalmap "obj/static/container02a_n" alpha
}

// *** COLUMNS ***

shader "obj/columns/column_01"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/column_01_c"
  normalmap "obj/columns/column_01_n"
}

shader "obj/columns/column_01b"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/column_01b_c"
  normalmap "obj/columns/column_01_n"
}

shader "obj/columns/column_02"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/column_02_c"
  normalmap "obj/columns/column_02_n"
}

shader "obj/columns/column_02a"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/column_02a_c"
  normalmap "obj/columns/column_02_n"
}

shader "obj/columns/column_03"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/column_03_c"
  normalmap "obj/columns/column_03_n"
}

shader "obj/columns/column_03_wire"
{
castshadow
   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal

	specular $nmapalpha

	cull none
	alphatest 2

	diffusemap "textures/wire/fencing_c"
	normalmap "textures/wire/fencing_n" alpha
}

shader "obj/columns/Column_05"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/Column_05_c"
  normalmap "obj/columns/Column_05_n"
}

shader "obj/columns/Column_05a"
{

	specular $nmapalpha
	//specularpower 0.76
  specularpower 0.62
	diffusemap "obj/columns/Column_05a_c"
  normalmap "obj/columns/Column_05_n"
}


shader "obj/sharedtextures/hose"
{
castshadow
	specular 0.504

	diffusemap "obj/sharedtextures/hose_c"
}

shader "obj/sharedtextures/rail"
{
	specular 0.504

	diffusemap "obj/sharedtextures/rail_c"
}

shader "obj/sharedtextures/rail2"
{
	specular 0.504

	diffusemap "obj/sharedtextures/rail2_c"
}




