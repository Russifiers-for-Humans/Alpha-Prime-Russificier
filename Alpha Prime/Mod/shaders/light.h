

shader "textures/misc/barstreaks"
{
	zwrite false
	sort translucent
	// cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/misc/barstreaks"
}

shader "MediScreen01b"
{
   surfparm nolightmap
   surfparm nomarks

	map nolight

	diffusemap "obj/static/mediscreen01b"
}

shader "EnemyLights1"    //default//
{
	surfparm nomarks
	surfparm impact

	zwrite false
	sort translucent
	cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/light/enemylights1_orange"
	color CreatureClass Color2
}

shader "EnemyLights2"   //default//
{
	surfparm nomarks
	surfparm impact

	zwrite false
	sort translucent
	cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/light/enemylights2_orange"
	color CreatureClass Color2

  	pass 0
//  		tcmod SCALE 0 0 1 0.25 
 		tcmod SHIFT 0 (CreatureClass UVAnim)
}

shader "EnemyLights1_blue"
{
	surfparm nomarks
	surfparm impact

	zwrite false
	sort translucent
	cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/light/enemylights1_blue"
	color CreatureClass Color2
}

shader "EnemyLights2_blue"
{
	surfparm nomarks
	surfparm impact

	zwrite false
	sort translucent
	cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/light/enemylights2_blue"
	color CreatureClass Color2

  	pass 0
 		tcmod SHIFT 0 (CreatureClass UVAnim)
}

shader "EnemyLights1_red"
{
	surfparm nomarks
	surfparm impact

	zwrite false
	sort translucent
	cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/light/enemylights1_red"
	color CreatureClass Color2
}

shader "EnemyLights2_red"
{
	surfparm nomarks
	surfparm impact

	zwrite false
	sort translucent
	cull none
	blend one one
	specular 0
	map nolight
	
	diffusemap "textures/light/enemylights2_red"
	color CreatureClass Color2

  	pass 0
 		tcmod SHIFT 0 (CreatureClass UVAnim)
}

shader "textures/light/control_lights01_valve_02"
{
   surfparm trans
   surfparm nomarks
   surfparm noimpact
   surfparm nonsolid
  surfparm nodrop
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

  sort translucent
  map nolight

	pass
		tcmod rotate g_fTime 0.25 0.75

  blend one one//invsrcalpha srcalpha
	diffusemap
		anim 8
		"textures/light/control_lights01a"
		"textures/light/control_lights01b"
		"textures/light/control_lights01c"
		"textures/light/control_lights01d"

   qer_editorimage "textures/light/control_table_lights_01" 
}

shader "textures/light/control_lights01"
{
   surfparm trans
   surfparm nomarks
   surfparm noimpact
   surfparm nonsolid
  surfparm nodrop
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

  sort translucent
  map nolight
  
  blend one one//invsrcalpha srcalpha
	diffusemap
		anim 8
		"textures/light/control_lights01a"
		"textures/light/control_lights01b"
		"textures/light/control_lights01c"
		"textures/light/control_lights01d"

   qer_editorimage "textures/light/control_table_lights_01" 
}

shader "textures/light/batterysign1"
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

	diffusemap "textures/light/batterysign1"
}

shader "obj/lights/lightning"
{
	map nolight
	zwrite false
	cull none
	sort translucent
 	blend one one
	
	pass
		tcmod shift g_lightningU g_lightningV
	diffusemap "obj/lights/lightning"
}

shader "beacon01"
{
	specular $nmapalpha
		
	diffusemap "obj/static/beacon01_c"
  	normalmap "obj/static/beacon01_n" alpha
}

shader "beacon01_bright"
{
	map nolight
	specular 1

	pass
		tcmod rotate g_fTime10 0 0

	diffusemap "obj/static/beacon01_c" alpha
}


shader "obj/lights/Light_3"
{
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/lights/Light_3_c"
  	normalmap "obj/lights/Light_3_n" alpha
}

shader "obj/lights/Light_1"
{
	specular $nmapalpha
	specularpower 0.42

	diffusemap "obj/lights/Light_1_c"
  	normalmap "obj/lights/Light_1_n" alpha
}

shader "textures/light/lightsolid1"
{
   surfparm nolightmap
   surfparm nomarks

	map nolight

	diffusemap "textures/light/lightsolid1"
}

shader "textures/light/lightline1"
{
   surfparm nolightmap
	surfparm trans
	surfparm nonsolid
   surfparm detail
   surfparm nomarks
	tesssize 144

	zwrite false
	sort translucent

	blend one srcalpha
	map nolight

	diffusemap "textures/light/lightline1"
}

shader "textures/light/lightline2"
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

	diffusemap "textures/light/lightline2"
}

shader "textures/light/lightline2b"
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

	diffusemap "textures/light/lightline2b"
}

shader "textures/light/lightline2c"
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

	diffusemap "textures/light/lightline2c"
}

shader "textures/light/lightcircle1"
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

	diffusemap "textures/light/lightcircle1"
}

shader "textures/light/lightsquare1"
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

	diffusemap "textures/light/lightsquare1"
}

shader "textures/light/lightconcrete_white"
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

shader "obj/lights/lightconcrete_orange"
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

	diffusemap "textures/light/lightconcrete_orange"
}

shader "obj/lights/lightconcrete_green"
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

	diffusemap "textures/light/lightconcrete_green"
}

shader "obj/lights/lightconcrete_red"
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

	diffusemap "textures/light/lightconcrete_red"
}

shader "textures/light/containerLight"
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

	diffusemap "textures/light/containerLight"
}

shader "textures/light/lightceil_light2"
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

	diffusemap "textures/light/lightceil_light2"
}

shader "textures/light/lightceil_light2red"
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

	diffusemap "textures/light/lightceil_light2red"
}

shader "textures/light/lightceil_light2yellow"
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

	diffusemap "textures/light/lightceil_light2yellow"
}

shader "textures/light/lightceil_light2yellowshut"
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

	diffusemap "textures/light/lightceil_light2yellowshut"
}

shader "textures/light/lightceil_light"
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

	diffusemap "textures/light/lightceil_light"
}

shader "textures/light/lightconcrete_orange"
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

	diffusemap "textures/light/lightconcrete_orange"
}


shader "textures/light/lightpanel1"
{
	qer_editorimage "textures/light/lightpanel1"
   surfparm nolightmap

	specular $alpha

	map nolight

	diffusemap "textures/light/lightpanel1_c"
}

shader "textures/light/lightpanel1b"
{
	qer_editorimage "textures/light/lightpanel1b"
   surfparm nolightmap

	specular $alpha

	map nolight

	diffusemap "textures/light/lightpanel1b_c"
}


shader "textures/light/control_table_lights"
{
   surfparm trans
   surfparm nomarks
   surfparm noimpact
   surfparm nonsolid
  surfparm nodrop
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

  sort translucent
  map nolight
  
  blend one one//invsrcalpha srcalpha
	diffusemap
		anim 1
		"textures/light/control_table_lights_01"
		"textures/light/control_table_lights_02"
		"textures/light/control_table_lights_03"

   qer_editorimage "textures/light/control_table_lights_01" 
}

shader "textures/light/control_table_lights2"
{
   surfparm trans
   surfparm nomarks
   surfparm noimpact
   surfparm nonsolid
  surfparm nodrop
  surfparm nocastshadow 
  surfparm detail
  surfparm nolightmap

  sort translucent
  map nolight
  
  blend one one//invsrcalpha srcalpha
	diffusemap
		anim 3
		"textures/light/control_table_lights_01"
		"textures/light/control_table_lights_02"
		"textures/light/control_table_lights_03"

   qer_editorimage "textures/light/control_table_lights_01" 
}
