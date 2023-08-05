shader "WarenEye"
{
	surfparm flesh
	surfparm nomarks

	diffusemap "obj/creatures/waren/WarenEye"
}

shader "OlivierEye"
{
	surfparm flesh
	surfparm nomarks

	diffusemap "obj/creatures/commander/OlivierEye"
}

shader "LiviaEye"
{
	surfparm flesh
	surfparm nomarks

	diffusemap "obj/creatures/livia/LiviaEye"
}

shader "Eye"
{
	surfparm flesh
	surfparm nomarks

	diffusemap "obj/creatures/arnold/eye"
}

shader "EyeHubb"
{
	map nolight

	surfparm flesh
	surfparm nomarks

	diffusemap "obj/creatures/arnold/eyehubb"
}

shader "EyeShadowHubb"
{
	surfparm flesh
	surfparm trans
	surfparm nomarks

	specular 1
	specularpower 0.81

	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/creatures/arnold/EyeShadowHubb" alpha
}

shader "EyeShadow"
{
	surfparm flesh
	surfparm trans
	surfparm nomarks

	specular 1
	specularpower 0.8

	blend invsrcalpha srcalpha
	sort translucent

	diffusemap "obj/creatures/arnold/EyeShadow" alpha
}

shader "boss"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512

	diffusemap "obj/creatures/boss/boss_c"
	normalmap "obj/creatures/boss/boss_n" alpha
}

shader "deaddown"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/Deaddown/deaddown_c"
	normalmap "obj/creatures/Deaddown/deaddown_n" alpha
}

shader "deadupper"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/DeadUpper/DeadUpper_c"
	normalmap "obj/creatures/DeadUpper/DeadUpper_n" alpha
}

shader "dead02_head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/dead02/dead02_head_c"
	normalmap "obj/creatures/dead02/dead02_head_n" alpha
}

shader "dead02"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/dead02/dead02_c"
	normalmap "obj/creatures/dead02/dead02_n" alpha
}

shader "prospector_head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/prospector/prospector_head_c"
	normalmap "obj/creatures/prospector/prospector_head_n" alpha
}

shader "prospector"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/prospector/prospector_c"
	normalmap "obj/creatures/prospector/prospector_n" alpha
}

shader "guard_helmet_visor"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512

	reflection $nmapalpha
//	fresnel 0.501
	envmap $nearestcube

	diffusemap "obj/creatures/guard/guard_helmet_c"
	normalmap "obj/creatures/guard/guard_helmet_n" alpha
}

shader "guard_helmet"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512
	reflection $nmapalpha
	fresnel 0.501
	envmap $nearestcube

	diffusemap "obj/creatures/guard/guard_helmet_c"
	normalmap "obj/creatures/guard/guard_helmet_n" alpha
}
shader "Guard_NpcHead"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512

	diffusemap "obj/creatures/guard/Guard_NpcHead_c"
	normalmap "obj/creatures/guard/Guard_NpcHead_n" alpha
}

shader "guard"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51
	reflection $nmapalpha
	reflectiontint "90 90 90"

	fresnel 0.5
	envmap $nearestcube

	diffusemap "obj/creatures/guard/guard_c"
	normalmap "obj/creatures/guard/guard_n" alpha
}

shader "guard2"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51
	reflection $nmapalpha
	fresnel 0.5
	reflectiontint "90 90 90"
	envmap $nearestcube

	diffusemap "obj/creatures/guard/guard2_c"
	normalmap "obj/creatures/guard/guard_n" alpha
}

shader "guard2_intro"
{
	surfparm flesh
	castshadow
	noselfshadow

	cull none
	specular $nmapalpha
	specularpower 0.51
	reflection $nmapalpha
	fresnel 0.5
	reflectiontint "90 90 90"
	envmap $nearestcube

	diffusemap "obj/creatures/guard/guard2_c"
	normalmap "obj/creatures/guard/guard_n" alpha
}


shader "warenHead"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/waren/waren_Head_c"
	normalmap "obj/creatures/waren/waren_Head_n" alpha
}

shader "waren"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.411

	diffusemap "obj/creatures/waren/waren_c"
	normalmap "obj/creatures/waren/waren_n" alpha
}

shader "arnold"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.411

	diffusemap "obj/creatures/arnold/arnold_c"
	normalmap "obj/creatures/arnold/arnold_n" alpha
}
shader "arnoldHead"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/arnold/arnold_Head_c"
	normalmap "obj/creatures/arnold/arnold_Head_n" alpha
}

shader "liv"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.41

	reflection $nmapalpha
	reflectiontint "90 90 90"
	fresnel 0.5
	envmap $nearestcube

	diffusemap "obj/creatures/Livia/Liv_C"
	normalmap "obj/creatures/Livia/Liv_N" alpha
}

shader "liv_head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.4

	diffusemap "obj/creatures/Livia/Liv_Head_C"
	normalmap "obj/creatures/Livia/Liv_Head_N" alpha
}

shader "liv_rohovina"
{
	surfparm flesh

	cull none
	specular 0
	blend invsrcalpha srcalpha
	sort translucent


	diffusemap "obj/creatures/Livia/Liv_Head_C" alpha
	normalmap "obj/creatures/Livia/Liv_Head_N"
}

shader "Technicist"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/Technicist/Technicist_c"
	normalmap "obj/creatures/Technicist/Technicist_n" alpha
}

shader "Technicist2"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/Technicist/Technicist_a_c"
	normalmap "obj/creatures/Technicist/Technicist_n" alpha
}

shader "Technicist_Head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/Technicist/Technicist_Head_c"
	normalmap "obj/creatures/Technicist/Technicist_Head_n" alpha
}

shader "Technicist2Head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/Technicist2/Technicist2Head_c"
	normalmap "obj/creatures/Technicist2/Technicist2Head_n" alpha
}

shader "Technicist3Head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/Technicist3/Technicist3Head_c"
	normalmap "obj/creatures/Technicist3/Technicist3Head_n" alpha
}

shader "Technicist4Head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/Technicist4/Technicist4Head_c"
	normalmap "obj/creatures/Technicist4/Technicist4Head_n" alpha
}

shader "LightTrooperHelmet"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.511

	diffusemap "obj/creatures/lighttrooper/helmet_c"
	normalmap "obj/creatures/lighttrooper/helmet_n" alpha
}

//TODO: je tento suface pouzit, kdyz ma na hlave helmu?
shader "LightTrooperHead"
{
	surfparm flesh
	castshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/lighttrooper/lighttrooperHead_c"
	normalmap "obj/creatures/lighttrooper/lighttrooperHead_n" alpha
}

shader "LightTrooper"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512

	diffusemap "obj/creatures/lighttrooper/lighttrooper_c"
	normalmap "obj/creatures/lighttrooper/lighttrooper_n" alpha
}

shader "LightTrooper2"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512

//	reflection $nmapalpha
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/lighttrooper/lighttrooper2_c"
	normalmap "obj/creatures/lighttrooper/lighttrooper_n" alpha
}

shader "LightTrooperHelmet2"
{
	surfparm metal
	//castshadow
	//noselfshadow
	specular $nmapalpha
	specularpower 0.511

//	reflection $nmapalpha
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/lighttrooper/helmet2_c"
	normalmap "obj/creatures/lighttrooper/helmet_n" alpha
}

shader "LightTrooper3"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.512

	diffusemap "obj/creatures/lighttrooper/lighttrooper3_c"
	normalmap "obj/creatures/lighttrooper/lighttrooper3_n" alpha
}

shader "LightTrooperHelmet3"
{
	surfparm metal
	//castshadow
	//noselfshadow
	specular $nmapalpha
	specularpower 0.512

	diffusemap "obj/creatures/lighttrooper/helmet3_c"
	normalmap "obj/creatures/lighttrooper/helmet3_n" alpha
}
shader "heavycommander"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.37

//	reflection 0.251
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/heavycommander/heavycommander_c"
	normalmap "obj/creatures/heavycommander/heavycommander_n" alpha
}

shader "heavycommander2"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.511

//	reflection 0.251
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/heavycommander/heavycommander_c"
	normalmap "obj/creatures/heavycommander/heavycommander_n" alpha
}

shader "heavycommanderHead"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/heavycommander/heavycommanderHead_c"
	normalmap "obj/creatures/heavycommander/heavycommanderHead_n" alpha
}

shader "heavycommander2Head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/heavycommander2/heavycommander2Head_c"
	normalmap "obj/creatures/heavycommander2/heavycommander2Head_n" alpha
}

shader "CommanderHead"
{
	surfparm flesh
	castshadow
	noselfshadow
	specular $nmapalpha
	specularpower 0.75

//	reflection 0.25
//	fresnel 0.5
//	envmap $nearestcube

	diffusemap "obj/creatures/commander/commanderHead_c"
	normalmap "obj/creatures/commander/commanderHead_n" alpha
}

shader "Commander"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.37

	// reflection 0.25
	// fresnel 0.501
	// envmap $nearestcube

	diffusemap "obj/creatures/commander/commander_c"
	normalmap "obj/creatures/commander/commander_n" alpha
}

shader "HeavyTrooper"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.37

//	reflection 0.251
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/Heavytrooper/Heavytrooper_c"
	normalmap "obj/creatures/Heavytrooper/Heavytrooper_n" alpha
}

shader "HeavyTrooper2"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.37

//	reflection 0.251
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/Heavytrooper/Heavytrooper_a_c"
	normalmap "obj/creatures/Heavytrooper/Heavytrooper_n" alpha
}

shader "HeavyTrooperHelmet"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.511

//	reflection $nmapalpha
//	fresnel 0.501
//	envmap $nearestcube

	diffusemap "obj/creatures/lighttrooper/helmet_c"
	normalmap "obj/creatures/lighttrooper/helmet_n" alpha
}

shader "HeavyTrooperHelmet2"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.511

	diffusemap "obj/creatures/lighttrooper/helmet2_c"
	normalmap "obj/creatures/lighttrooper/helmet_n" alpha
}

shader "player_eyeR"
{
	specular $nmapalpha
	specularpower 0.8
	surfparm nomarks

	diffusemap "obj/creatures/morphhead/eye_c"
	normalmap "obj/creatures/morphhead/eye_n" alpha
}
shader "player_eyeL"
{
	specular $nmapalpha
	specularpower 0.8
	surfparm nomarks

	diffusemap "obj/creatures/morphhead/eye_c"
	normalmap "obj/creatures/morphhead/eye_n" alpha
}
shader "player_body"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.511

	diffusemap "obj/creatures/player/player_body_c"
	normalmap "obj/creatures/player/player_body_n" alpha
}
shader "player_head"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.5

	diffusemap "obj/creatures/player/player_head_c"
	normalmap "obj/creatures/player/player_head_n" alpha
}
shader "player_hand"
{
	surfparm flesh
	specular $nmapalpha
	specularpower 0.51

	diffusemap "obj/creatures/player/player_hand_c"
	normalmap "obj/creatures/player/player_hand_n" alpha
}

shader "servicebot_01"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.53

	diffusemap "obj/creatures/servicebot01/servicebot_01_c" alpha
	normalmap "obj/creatures/servicebot01/servicebot_01_n" alpha
}

shader "servicebot_01_Wire"
{
	surfparm metal
	surfparm nomarks

	specular $nmapalpha
	cull none
	alphatest 126
	specularpower 0.52

	diffusemap "obj/creatures/servicebot01/servicebot_01_c" alpha
	normalmap "obj/creatures/servicebot01/servicebot_01_n" alpha
}

shader "servicebot_02"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha
	specularpower 0.531

	diffusemap "obj/creatures/servicebot01/servicebot_01b_c" alpha
	normalmap "obj/creatures/servicebot01/servicebot_01b_n" alpha
}

shader "servicebot_02_Wire"
{
	surfparm metal
	surfparm nomarks

	specular $nmapalpha
	cull none
	alphatest 126
	specularpower 0.52

	diffusemap "obj/creatures/servicebot01/servicebot_01b_c" alpha
	normalmap "obj/creatures/servicebot01/servicebot_01b_n" alpha
}


shader "obj/creatures/corpses/pants"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/corpses/pants_c"
	normalmap "obj/creatures/corpses/pants_n" alpha
}
shader "obj/creatures/corpses/shirt"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/corpses/shirt_c"
	normalmap "obj/creatures/corpses/shirt_n" alpha
}
shader "obj/creatures/corpses/skin"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/corpses/skin_c"
	normalmap "obj/creatures/corpses/skin_n" alpha

}
shader "obj/creatures/corpses/pants_service"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/corpses/pants_service_c"
	normalmap "obj/creatures/corpses/pants_service_n" alpha
}

shader "obj/creatures/corpses/shirt_service"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/corpses/shirt_service_c"
	normalmap "obj/creatures/corpses/shirt_service_n" alpha
}

shader "obj/creatures/corpses/hat_service"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/corpses/hat_service_c"
	normalmap "obj/creatures/corpses/hat_service_n" alpha
}


shader "SecuritySpider01"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/securityspider01/securityspider01_c"
	normalmap "obj/creatures/securityspider01/securityspider01_n" alpha
}


shader "dron01"
{
	surfparm metal
	 castshadow //zatim, nez se opravi geometrie
	//noselfshadow
	specular $nmapalpha

	specularpower 0.54

	diffusemap "obj/creatures/dron01/dron01_c" alpha
	normalmap "obj/creatures/dron01/dron01_n" alpha
}

shader "dron01_bright"
{
	surfparm nomarks
	map nolight
	color creature_dron iColorBright

	diffusemap "obj/creatures/dron01/dron01_c" alpha
}

shader "dron01_flare1"
{
	zwrite false
	sort translucent
	cull none
	blend one srcalpha
	map nolight
	color creature_dron iColorFlare1
	diffusemap "textures/efx/coronalens" alpha
}
shader "dron01_flare2"
{
	zwrite false
	sort translucent
	cull none
	blend one srcalpha
	map nolight
	color creature_dron iColorFlare2

	diffusemap "textures/efx/coronalens" alpha
}
shader "dron01_flare3"
{
	zwrite false
	sort translucent
	cull none
	blend one srcalpha
	map nolight
	color creature_dron iColorFlare3

	diffusemap "textures/efx/coronalens" alpha
}
shader "dron01_eye"
{
	zwrite false
	sort translucent
	cull none
	blend one srcalpha
	map nolight
	color creature_dron iColorEye

	pass
		tcmod rotate g_fTime 0 0

	diffusemap "textures/efx/coronalens" alpha
}
shader "dron01_engine"
{
	zwrite false
	sort translucent
	cull none
	blend one srcalpha

	map nolight

	color creature_dron iColorEngine

	diffusemap "textures/efx/coronalens" alpha
}

shader "dron01_wires"
{
	surfparm nomarks
	cull none
	alphatest 128

	diffusemap "obj/creatures/dron01/dron01_c" alpha
}

shader "dron01_hit"
{
	surfparm nomarks
	zwrite false
	sort translucent
	cull none
	blend one one
	color _eclass Color

	map nolight
	diffusemap "obj/creatures/dron01/hitglare"
}

shader "heavyspider01_hit"
{
	surfparm nomarks
	zwrite false
	sort translucent
	cull none
	blend one one
	color _eclass Color

	map nolight
	diffusemap "obj/creatures/heavyspider01/hitglare"
}


shader "baldur_head"
{
	surfparm flesh
	specular $alpha
castshadow
	diffusemap "obj/creatures/baldur/baldur_head_c" alpha
}

shader "baldur_shirt"
{
	surfparm flesh
	specular $nmapalpha
	diffusemap "obj/creatures/baldur/baldur_shirt_c"
	normalmap "obj/creatures/baldur/baldur_shirt_n" alpha
}

shader "analyst2"
{
	castshadow
	noselfshadow

	specular $nmapalpha

	diffusemap "obj/creatures/analyst2/analyst2_c" alpha
	normalmap "obj/creatures/analyst2/analyst2_n" alpha
}
shader "analyst2_view"
{
	surfparm trans
	specular 1
	specularpower 0.9

	zwrite false
	cull none
	blend invsrcalpha srcalpha
	//blend one srcalpha
	sort translucent

	diffusemap "obj/creatures/analyst2/analyst2_c" alpha
	//normalmap "textures/glass/glass1_n"
}



shader "heavyspider01head"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha

	specularpower 0.301
	diffusemap "obj/creatures/heavyspider01/heavyspider01head_c"
	normalmap "obj/creatures/heavyspider01/heavyspider01head_n"
}



shader "heavyspider01legs"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $alpha
	specularpower 0.321

	diffusemap "obj/creatures/heavyspider01/heavyspider01legs_c"
	normalmap "obj/creatures/heavyspider01/heavyspider01legs_n"
}

shader "robotheadtest"
{
	surfparm metal
	castshadow
	noselfshadow

	specular $nmapalpha

	specularpower 0.30
	diffusemap "obj/creatures/robot/robothead_c"
//	diffusemap "obj/creatures/robot/grey"
	normalmap "obj/creatures/robot/robothead_n"
}


shader "spacesuit"
{
	surfparm flesh
	castshadow
	noselfshadow

	specular $alpha
	specularpower 0.33

	diffusemap "obj/creatures/spacesuit/spacesuit_c"
	normalmap "obj/creatures/spacesuit/spacesuit_n"
}

