shader "hammer_fps"
{
fresnel 0.007
reflection 0.01
	specular $nmapalpha
	specularpower 0.46

	sort overall

	diffusemap "obj/weapons/hammer_fps_c"
  	normalmap "obj/weapons/hammer_fps_n" alpha
}

shader "hammer"
{
fresnel 0.007
reflection 0.01
	specular $nmapalpha
	specularpower 0.45
castshadow
	diffusemap "obj/weapons/hammer_fps_c"
  	normalmap "obj/weapons/hammer_fps_n" alpha
}

shader "obj/weapons/energy_wall"
{

reflection 0.01
	surfparm trans
	cull none
	zwrite false
	sort overlay 2

	color _eclass Color
//	color 0x7f7fff20

	diffusemap $renderview clampu clampv
  	refractmap "textures/vp1"
  	diffusemap2 "obj/weapons/ewallshield" alpha

  	parm0 (misc_shooting_shield row0)
  	parm1 (misc_shooting_shield row1)

  	pass 0
  		tcmod SCALE 0 0 20 20 
 		tcmod SHIFT (misc_shooting_shield UVShift) 0
// 		vcmod SIN g_fTime 3.0 3.0 1.0
}

shader "obj/weapons/energy_wall_stand"
{

	specular $nmapalpha
	specularpower 0.55
	castshadow

	diffusemap "obj/weapons/energy_wall_stand_c"
  	normalmap "obj/weapons/energy_wall_stand_n" alpha
}

shader "obj/items/battery_01"
{
	specular $nmapalpha
castshadow

	diffusemap "obj/items/battery_01_c"
  	normalmap "obj/items/battery_01_n" alpha
}

shader "obj/items/hubb"
{
fresnel 0.008
	specular $nmapalpha
castshadow

	diffusemap "obj/items/hubb_c"
  	normalmap "obj/items/hubb_n" alpha
}

shader "mossberg590"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.52
	sort overall
castshadow

	diffusemap "obj/weapons/mossberg590_c"
  	normalmap "obj/weapons/mossberg590_n" alpha
}


shader "rocketlauncher"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.55
	castshadow

	diffusemap "obj/weapons/rocketlauncher_c"
  	normalmap "obj/weapons/rocketlauncher_n" alpha
}

shader "rocketlauncher_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.55
	sort overall

	diffusemap "obj/weapons/rocketlauncher_c"
  	normalmap "obj/weapons/rocketlauncher_n" alpha
}

shader "flamethrower"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.55
	castshadow

	diffusemap "obj/weapons/flamethrower_c"
  	normalmap "obj/weapons/flamethrower_n" alpha
}

shader "flamethrower_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.6
	sort overall

	diffusemap "obj/weapons/flamethrower_c"
  	normalmap "obj/weapons/flamethrower_n" alpha
}

shader "gatlingLE"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.56
	castshadow

	diffusemap "obj/weapons/gatlingLE_c"
  	normalmap "obj/weapons/gatlingLE_n" alpha
}

shader "gatlingLE_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.56
	sort overall

	diffusemap "obj/weapons/gatlingLE_c"
  	normalmap "obj/weapons/gatlingLE_n" alpha
}

shader "player_fps_hands"
{
	specular $nmapalpha
	specularpower 0.548
	sort overall
	//reflection 0.502
	//fresnel 0.7015

	diffusemap "obj/weapons/hands_c"
  	normalmap "obj/weapons/hands_n" alpha
  	envmap $nearestcube
}

shader "obj/weapons/shell"
{
	specular $nmapalpha
castshadow
fresnel 0.008
reflection 0.02
	diffusemap "obj/weapons/shell_c"
	normalmap "obj/weapons/shell_n" alpha
}

shader "obj/weapons/weaflash1_fps"
{
	zwrite false
	map nolight
	cull none
	sort overall 1

	color _eclass Color
	
	blend one srcalpha
	diffusemap "obj/weapons/weaflash1" alpha
}

shader "obj/weapons/weaflash1"
{
	zwrite false
	map nolight
	cull none
	sort translucent

	color _eclass Color
	
	blend one srcalpha
	diffusemap "obj/weapons/weaflash1" alpha
}

/*
Shadery pro zbrane jako itemy. Tj. neobsahuji depthhack
*/
shader "mineLight2"
{
	map nolight
	pass
	tcmod shift g_fTime 0
	diffusemap "obj/weapons/grenadelights"
}

shader "mineLight"
{
	map nolight
	pass
	color _eclass Color
	tcmod shift g_fTime 0
	diffusemap "obj/weapons/mineLight"
}

shader "mine"
{
fresnel 0.008
	specular $nmapalpha
	specularpower 0.55
castshadow
	diffusemap "obj/weapons/mine_c"
  	normalmap "obj/weapons/mine_n" alpha
}

shader "obj/items/grenade"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.555
 castshadow
	diffusemap "obj/weapons/grenade_c"
  	normalmap "obj/weapons/grenade_n" alpha
}

shader "obj/items/grenadeLights"
{
	map nolight
	pass
	tcmod shift g_fTime 0
	diffusemap "obj/weapons/grenadelights"
}

shader "obj/weapons/machinegun_service"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.55
	castshadow

	diffusemap "obj/weapons/machinegun_service_c"
  	normalmap "obj/weapons/machinegun_service_n" alpha
}

shader "obj/weapons/machinegun"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.56
	castshadow

	diffusemap "obj/weapons/machinegun_c"
  	normalmap "obj/weapons/machinegun_n" alpha
}

shader "obj/weapons/machinegun_effect"
{
	zwrite false
	alphatest 1 // zatim
	sort translucent
	map nolight

	blend one one
	diffusemap "obj/weapons/machinegun_effect"
  	
}

shader "pistol"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.17
castshadow
	diffusemap "obj/weapons/pistol_c"
  	normalmap "obj/weapons/pistol_n" alpha
}


shader "pistol_ammo"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
castshadow
	diffusemap "obj/weapons/pistol_ammo_c"
  	normalmap "obj/weapons/pistol_ammo_n" alpha
}

shader "pistol_ammo_bright"
{
fresnel 0.008
reflection 0.01
	zwrite false
	sort translucent
	map nolight

	diffusemap "obj/weapons/pistol_ammo_c"
  	normalmap "obj/weapons/pistol_ammo_n" alpha
}

shader "pistol_effect"
{
	alphatest 1
	sort translucent
	zwrite false
	map nolight
	
	blend one one
	diffusemap "obj/weapons/pistol_effect"
}

shader "obj/items/recon"
{
fresnel 0.007
	specular $nmapalpha
	specularpower 0.55
castshadow	
	diffusemap "obj/items/recon_c"
  	normalmap "obj/items/recon_n" alpha
}


/*
Shadery pro zbrane do ruky. Tj. obsahuji depthhack
*/

//JackHammer body
shader "obj/weapons/JackHammer_fps"
{

	specular $nmapalpha
	specularpower 0.56
	
	sort overall
 
	diffusemap "obj/weapons/JackHammer_c"
  	normalmap "obj/weapons/JackHammer_n" alpha
}

//JackHammer hroty
shader "obj/weapons/JackHammer_Hroty_Fps"
{
fresnel 0.007
reflection 0.01
//	specular $nmapalpha
//	specularpower 0.56
	sort overall

//	zwrite false
	blend zero srcalpha
	color _eclass Color
	map nolight

	diffusemap "obj/weapons/JackHammer_c"
//  	normalmap "obj/weapons/JackHammer_n" alpha
}

/*
shader "obj/weapons/JackHammer_Hroty_Fps"
{
fresnel 0.007
reflection 0.01
	sort overall
//	map nolight

	layer "obj/weapons/JackHammer_c" alpha
	tex0 = layer 0
	tfac = _eclass TFactor
   effect "JackHammerHroty"
}
*/
//JackHammer effectobject1 - (velky blesk)
shader "textures/light/lights_01"
{
   surfparm lightmap
	surfparm trans
	surfparm nonsolid
   surfparm detail
   surfparm nomarks

	cull none
	zwrite false
	sort overall 1

	blend one srcalpha
	map nolight

	diffusemap "textures/light/lights_01"
}

//JackHammer effectobject2 (maly blesk)
shader "textures/light/lights_02"
{
   surfparm lightmap
	surfparm trans
	surfparm nonsolid
   surfparm detail
   surfparm nomarks
	
	cull none
	zwrite false
	sort overall 1

	blend one srcalpha
	map nolight

	diffusemap "textures/light/lights_02"
}

//JackHammer corona
shader "sprites/corona/JackHammer"
{
fresnel 0.007
reflection 0.01
	sort overall
   cull none
castshadow
	layer "obj/weapons/hammerflare" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

/*
shader "obj/weapons/JackHammer_Hroty_Fps"
{
fresnel 0.007
reflection 0.01
	sort overall
//	cull none

	layer "obj/weapons/JackHammer_c"
	tex0 = layer 0
	tfac = _eclass TFactor
	effect "JackHammerHroty"
}
*/
shader "obj/weapons/grenade"
{
fresnel 0.007
reflection 0.01
	specular $nmapalpha
	specularpower 0.555
	sort overall
 
	diffusemap "obj/weapons/grenade_c"
  	normalmap "obj/weapons/grenade_n" alpha
}

shader "obj/weapons/grenadeLights"
{
	map nolight
	sort overall
	pass
	tcmod shift g_fTime 0
	diffusemap "obj/weapons/grenadelights"
}

shader "obj/weapons/grenade_NoFPS"
{
fresnel 0.007
reflection 0.01
	specular $nmapalpha
	specularpower 0.555
 castshadow
	diffusemap "obj/weapons/grenade_c"
  	normalmap "obj/weapons/grenade_n" alpha
}

shader "obj/weapons/grenadeLights_NoFPS"
{
	map nolight
	pass
	tcmod shift g_fTime 0
	diffusemap "obj/weapons/grenadelights"
}

shader "obj/weapons/machinegun_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.56
	sort overall

	diffusemap "obj/weapons/machinegun_c"
  	normalmap "obj/weapons/machinegun_n" alpha
}

shader "obj/weapons/machinegun_effect_fps"
{
	zwrite false
	sort overall 1
	map nolight

	blend one one
	diffusemap "obj/weapons/machinegun_effect"
}

shader "pistol_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.17
	sort overall

	diffusemap "obj/weapons/pistol_c"
  	normalmap "obj/weapons/pistol_n" alpha
}

shader "pistol_ammo_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha

	diffusemap "obj/weapons/pistol_ammo_c"
  	normalmap "obj/weapons/pistol_ammo_n" alpha
}

shader "pistol_effect_fps"
{
	zwrite false
	map nolight
	sort overall 1
	
	blend one one
	diffusemap "obj/weapons/pistol_effect"
}

shader "obj/weapons/Shotgun"
{
fresnel 0.0073
reflection 0.01
	specular $nmapalpha
	specularpower 0.55
	castshadow
	diffusemap "obj/weapons/ShotGun_c"
  	normalmap "obj/weapons/ShotGun_n" alpha
}

shader "obj/weapons/Shotgun_fps"
{
fresnel 0.0073
reflection 0.01
	specular $nmapalpha
	specularpower 0.55
	sort overall
	diffusemap "obj/weapons/ShotGun_c"
  	normalmap "obj/weapons/ShotGun_n" alpha
}

// SNIPER RIFLE ------------------------
shader "obj/weapons/SniperRifle_fps"
{
fresnel 0.008
reflection 0.01
	specular $nmapalpha
	specularpower 0.554
	sort overall
	diffusemap "obj/weapons/SniperRifle_c"
  	normalmap "obj/weapons/SniperRifle_n" alpha
}

shader "obj/weapons/SniperRifle_Display"
{
fresnel 0.008
reflection 0.01
	map nolight
	sort overall
	diffusemap $renderview clampu clampv
}

shader "obj/weapons/SniperRifle_CrossEff_fps"
{
	map nolight
	sort overall
	blend one srcalpha
	pass
		tcmod rotate g_fTime01 0 0
	diffusemap "obj/weapons/SniperRifle_crosseff" clampu clampv
}

shader "obj/weapons/SniperRifle_Cross_fps"
{
fresnel 0.008
	map nolight
	sort overall
	blend one srcalpha
	diffusemap "obj/weapons/SniperRifle_cross"
}

shader "obj/weapons/SniperRifle"
{
fresnel 0.008
reflection 0.01
castshadow
	specular $nmapalpha
	specularpower 0.554
	diffusemap "obj/weapons/SniperRifle_c"
  	normalmap "obj/weapons/SniperRifle_n" alpha
}

shader "obj/weapons/SniperRifle_CrossEff"
{
	map nolight
	blend one srcalpha
	pass
		tcmod rotate g_fTime01 0 0
	diffusemap "obj/weapons/SniperRifle_crosseff" clampu clampv
}

shader "obj/weapons/SniperRifle_Cross"
{
	map nolight
	blend one srcalpha
	diffusemap "obj/weapons/SniperRifle_cross"
}

// SNIPER RIFLE ITEM -------------------
shader "obj/items/SniperRifle"
{
fresnel 0.008
reflection 0.01
castshadow
	specular $nmapalpha
	specularpower 0.554
	diffusemap "obj/weapons/SniperRifle_c"
  	normalmap "obj/weapons/SniperRifle_n" alpha
}

shader "obj/weapons/Shotgun_effect"
{
	zwrite false
	alphatest 1 // zatim
	sort translucent
	map nolight

	blend one one
	diffusemap "obj/weapons/machinegun_effect"
}

shader "obj/weapons/Shotgun_effect_fps"
{
	zwrite false
	sort overall 1
	map nolight

	blend one one
	diffusemap "obj/weapons/machinegun_effect"
}


//----------------------

shader "health01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/items/health01_c"
  	normalmap "obj/items/health01_n" alpha
}

shader "health01_bright"
{
	specular 0.5


	diffusemap "obj/items/health01_c"
}



shader "hubb"
{
fresnel 0.007
	specular $nmapalpha
castshadow
	diffusemap "obj/items/hubb_c"
  	normalmap "obj/items/hubb_n" alpha
}

shader "hubb_bright"
{
	specular $nmapalpha
	map nolight
	
fresnel 0.007
	diffusemap "obj/items/hubb_c"
  	normalmap "obj/items/hubb_n" alpha
}

shader "hubb_empty"
{
	surfparm trans
	specular 1
	specularpower 0.45
	reflection 0.3
	fresnel 1.001
castshadow
	zwrite false
	cull none
	blend invsrcalpha srcalpha
	//blend one srcalpha
	sort translucent

	diffusemap "textures/glass/glass1_c" alpha
	envmap $nearestcube
}



shader "rcdevice"
{
fresnel 0.008
	specular $nmapalpha
	sort overall
castshadow
	diffusemap "obj/static/usedisp01_c"
  	normalmap "obj/static/usedisp01_n" alpha
}


shader "rcdevice_display"
{
	surfparm trans
   surfparm detail
   surfparm nomarks

	specular 0.2
	specularpower 0.35
	zwrite false
	sort overall 1

	blend invsrcalpha srcalpha
	map nolight

#ifdef _XBOX
	pass
		tcmod shift 0.0 g_fTime10

	diffusemap "obj/static/usedisp01_c"
#else
	diffusemap $gui0 alpha
#endif
}
