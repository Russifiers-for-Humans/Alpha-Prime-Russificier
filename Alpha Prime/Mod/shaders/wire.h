shader "textures/wire/cables1"
{
   tesssize 32

   surfparm nonsolid
   surfparm trans
   surfparm nomarks
   surfparm alphashadow

	specular $nmapalpha
  specularpower 0.81

	alphatest 32

	diffusemap "textures/wire/cables1_c" alpha
		qer_editorimage "textures/wire/cables1"
	normalmap "textures/wire/cables1_n" alpha
}

shader "textures/wire/cables2"
{
   tesssize 32

   surfparm nonsolid
   surfparm trans
   surfparm nomarks
   surfparm alphashadow
   cull none

	specular $nmapalpha
	specularpower 0.81

	alphatest 64

	diffusemap "textures/wire/cables2_c" alpha
	qer_editorimage "textures/wire/cables2"
	normalmap "textures/wire/cables2_n" alpha
}

shader "textures/wire/wirestep1"
{
   surfparm trans
   surfparm nomarks
   surfparm iron
   surfparm alphashadow

	specular $nmapalpha
  specularpower 0.41

	alphatest 16

	diffusemap "textures/wire/wirestep1_c"
		qer_editorimage "textures/wire/wirestep1"
	normalmap "textures/wire/wirestep1_n" alpha
}

shader "textures/wire/wiredfloor1"
{
   surfparm trans
   surfparm nomarks
   surfparm iron
   surfparm alphashadow

	specular $nmapalpha
  specularpower 0.41

	alphatest 16

	diffusemap "textures/wire/wiredfloor1_c" alpha
		qer_editorimage "textures/wire/wiredfloor1"
	normalmap "textures/wire/wiredfloor1_n" alpha
}

shader "textures/wire/wiredfloor1a"
{
   surfparm trans
   surfparm nomarks
   surfparm iron
   surfparm alphashadow

	specular $alpha
  specularpower 0.41

	alphatest 16

	diffusemap "textures/wire/wiredfloor1a_c" alpha
		qer_editorimage "textures/wire/wiredfloor1a"
	normalmap "textures/wire/wiredfloor1_n" alpha
}

shader "textures/wire/fencing"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
   surfparm castshadow
 
	specular $nmapalpha
  specularpower 0.41
	cull none
	alphatest 16

	diffusemap "textures/wire/fencing_c"
		qer_editorimage "textures/wire/fencing"
	normalmap "textures/wire/fencing_n" alpha
}

shader "textures/wire/lamela1"
{
	qer_editorimage "textures/wire/lamela1"
   surfparm trans
   surfparm detail
   surfparm metal
   surfparm alphashadow

	specular $alpha

	cull none
	alphatest 5

	diffusemap "textures/wire/lamela1_c" alpha
	normalmap "textures/wire/lamela1_n"
}

shader "textures/wire/Fencing_02"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
   surfparm castshadow
  
	specular $nmapalpha
  specularpower 0.41
	alphatest 16

	diffusemap "textures/wire/Fencing_02_c"
	normalmap "textures/wire/Fencing_02_n" alpha
}

shader "textures/wire/Fencing_03"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
   surfparm castshadow
 
	specular $nmapalpha
  specularpower 0.41
	alphatest 100

	diffusemap "textures/wire/Fencing_03_c"
	normalmap "textures/wire/Fencing_03_n" alpha
}

shader "textures/wire/ventilator_01"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
   castshadow
	specular $nmapalpha
  specularpower 0.41
	alphatest 16

	diffusemap "textures/wire/ventilator_01_c"
	normalmap "textures/wire/ventilator_01_n" alpha
}

shader "textures/wire/ventilator_01a"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
   castshadow
	specular $nmapalpha
  specularpower 0.41
	alphatest 16

	diffusemap "textures/wire/ventilator_01a_c"
	normalmap "textures/wire/ventilator_01_n" alpha
}
