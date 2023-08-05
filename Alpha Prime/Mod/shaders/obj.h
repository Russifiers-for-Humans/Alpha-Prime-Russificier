shader "obj/hackbox"
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

	diffusemap "obj/hackbox"
}

//UNIVERZALNI SHADER ( POUZE PRO TESTOVACI UCELY!!!!!!!!!!)
shader "kov"
{
	layer "textures/steel/kov"
	tex0 = layer 0
	effect "nolightmap"
}


// efx - vybuch granatometu

shader "obj/fx/explo"
{	
	cull none
   layer "obj/fx/explo"
   
   //pass 0
      //vcmod SCALE g_iScaleTcMod

   tex0 = layer 0
	tfac = (_eclass TFactor)
   effect "effect_glow"

}
shader "obj/weapons/future/future"
{

	layer "obj/weapons/future/future"
	tex0 = layer 0
	effect "nolightmap"
}



// vehicles

shader "obj/vehicles/buggy_01"
{
	castshadow
	specular $nmapalpha
	specularpower 0.23
	diffusemap "obj/vehicles/buggy_01_c"
	normalmap "obj/vehicles/buggy_01_n" alpha
}

shader "obj/vehicles/truck_01"
{
	castshadow
	specular $nmapalpha
	specularpower 0.23
	diffusemap "obj/vehicles/truck_01_c"
	normalmap "obj/vehicles/truck_01_n" alpha
}

shader "obj/vehicles/dropship_01a"
{
	specular $nmapalpha
	specularpower 0.23
	diffusemap "obj/vehicles/dropship_01a_c"
	normalmap "obj/vehicles/dropship_01a_n" alpha
}

shader "obj/vehicles/dropship_01b"
{
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/dropship_01b_c"
	normalmap "obj/vehicles/dropship_01b_n" alpha
}

shader "obj/vehicles/plates"
{
castshadow
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/dropship_plate_c"
	normalmap "obj/vehicles/dropship_plate_n" alpha
}

shader "obj/vehicles/transporter_01"
{
//	castshadow
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/transporter_01_c"
	normalmap "obj/vehicles/transporter_01_n" alpha
}

shader "obj/vehicles/transporter_02"
{
//	castshadow
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/transporter_02_c"
	normalmap "obj/vehicles/transporter_02_n" alpha
}

shader "obj/vehicles/transporter_03"
{
	castshadow
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/transporter_03_c"
	normalmap "obj/vehicles/transporter_03_n" alpha
}

shader "obj/vehicles/loader"
{
	castshadow
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/loader_c"
	normalmap "obj/vehicles/loader_n" alpha
}

shader "obj/vehicles/capsule_01"
{
	specular $nmapalpha
	specularpower 0.53
	diffusemap "obj/vehicles/capsule_01_c"
	normalmap "obj/vehicles/capsule_01_n" alpha
}

shader "obj/vehicles/capsule_decal"
{
   surfparm trans
   surfparm nonsolid
  surfparm nocastshadow 
  surfparm detail


sort mark
  
  blend invsrcalpha srcalpha

	diffusemap "obj/vehicles/capsule_decal" alpha
}
