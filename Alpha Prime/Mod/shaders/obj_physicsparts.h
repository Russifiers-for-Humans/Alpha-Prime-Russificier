shader "obj/physicsparts/dipper_01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physicsparts/dipper_01_c"
	normalmap "obj/physicsparts/dipper_01_n" alpha
}

shader "obj/physicsparts/poster_01"
{

	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/physicsparts/poster01_c"
}

shader "obj/physicsparts/poster_01a"
{

	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/physicsparts/poster01a_c"
}

shader "obj/physicsparts/poster_01b"
{

	specular $nmapalpha
	specularpower 0.41

	diffusemap "obj/physicsparts/poster01b_c"
}

shader "obj/physicsparts/greenhouse_01"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
	cull none   
	castshadow
	specular $nmapalpha
	specularpower 0.42

	alphatest 126

	diffusemap "obj/physicsparts/greenhouse_01_c" alpha
	normalmap "obj/physicsparts/greenhouse_01_n" alpha
}

shader "obj/physicsparts/folder01"
{
   castshadow
	specular $alpha
	specularpower 0.41
	surfparm plastic

	diffusemap "obj/physicsparts/folder01_c" alpha
}

shader "obj/physicsparts/folder01a"
{
   castshadow
	specular $alpha
	specularpower 0.41
   surfparm plastic
   
	diffusemap "obj/physicsparts/folder01a_c" alpha
}

shader "obj/physicsparts/camera02"
{
	specular $nmapalpha
	specularpower 0.42
	surfparm plastic
castshadow
	diffusemap "obj/physicsparts/camera02_c"
  	normalmap "obj/physicsparts/camera02_n" alpha
}

shader "obj/physicsparts/pipeventil"
{
	specular $nmapalpha
	specularpower 0.42
	surfparm plastic
castshadow
	diffusemap "obj/physicsparts/pipeventil_c"
  	normalmap "obj/physicsparts/pipeventil_n" alpha
}

shader "camera01"
{
	specular $nmapalpha
	surfparm plastic
castshadow
	diffusemap "obj/physicsparts/camera01_c"
	normalmap "obj/physicsparts/camera01_n" alpha
}

shader "camera01_optika"
{
	map nolight
	alphatest 128
	specular $nmapalpha
	surfparm glass
castshadow
	diffusemap "obj/physicsparts/optika_red_c"
	normalmap "obj/physicsparts/optika_n" alpha
}


shader "robot_part"
{
	castshadow
	specular $alpha
	specularpower 0.33

	diffusemap "obj/creatures/robot/robot_c" alpha
	normalmap "obj/creatures/robot/robot_n"
}


shader "glassdebris"
{
	surfparm trans
   surfparm glass
   cull none
	sort translucent

	specular 1.0
	zwrite false
	blend invsrcalpha srcalpha

	diffusemap "textures/glass/brokenglass2" alpha
}

shader "obj/physicsparts/DeviceDoor01"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physicsparts/DeviceDoor01_c"
	normalmap "obj/physicsparts/DeviceDoor01_n" alpha
}

shader "obj/physicsparts/DeviceDoor02"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physicsparts/DeviceDoor02_c"
	normalmap "obj/physicsparts/DeviceDoor02_n" alpha
}

shader "obj/physicsparts/DeviceDoor03"
{
	specular $nmapalpha
castshadow
	diffusemap "obj/physicsparts/DeviceDoor03_c"
	normalmap "obj/physicsparts/DeviceDoor03_n" alpha
}

shader "textures/wire/DeviceDoor04"
{
   surfparm trans
   surfparm nomarks
   surfparm metal
castshadow
	specular $nmapalpha
  specularpower 0.42

	alphatest 16

	diffusemap "obj/physicsparts/DeviceDoor04_c"
	normalmap "obj/physicsparts/DeviceDoor04_n" alpha
}

shader "obj/physicsparts/Board01"
{
    castshadow
	specular $nmapalpha
	specularpower 0.42
	surfparm plastic

	diffusemap "obj/physicsparts/Board01_c"
  	normalmap "obj/physicsparts/Board01_n" alpha
}

shader "obj/physicsparts/Board01a"
{
    castshadow
	specular $nmapalpha
	specularpower 0.42
	surfparm plastic

	diffusemap "obj/physicsparts/Board01a_c"
  	normalmap "obj/physicsparts/Board01_n" alpha
}
