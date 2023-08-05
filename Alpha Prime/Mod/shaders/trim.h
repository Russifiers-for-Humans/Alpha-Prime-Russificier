shader "textures/trim/wirestep_02"
{
	surfparm iron
	specular $nmapalpha
castshadow

	diffusemap "textures/trim/wirestep_02_c"
	normalmap "textures/trim/wirestep_02_n" alpha
}

shader "textures/trim/trimsteel"
{
	surfparm metal
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/trimsteel_c"
		qer_editorimage "textures/trim/trimsteel"
	normalmap "textures/trim/trimsteel_n" alpha
}


shader "textures/trim/trimsteelb"
{
	surfparm metal
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/trimsteelb_c"
		qer_editorimage "textures/trim/trimsteelb"
	normalmap "textures/trim/trimsteel_n" alpha
}

shader "textures/trim/trimsteel2"
{
	surfparm metal
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/trimsteel2_c"
		qer_editorimage "textures/trim/trimsteel2"
	normalmap "textures/trim/trimsteel2_n" alpha
}

shader "textures/trim/yeltrim"
{
	surfparm metal
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/yeltrim_c"
		qer_editorimage "textures/trim/yeltrim"
	normalmap "textures/trim/yeltrim_n" alpha
}

shader "textures/trim/yeltrim_noN"
{
	surfparm metal
	tesssize 144
castshadow
	diffusemap "textures/trim/yeltrim_c"
		qer_editorimage "textures/trim/yeltrim"
	
}

shader "textures/trim/yeltrim_a"
{
	surfparm metal
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/yeltrim_a_c"
		qer_editorimage "textures/trim/yeltrim"
	normalmap "textures/trim/yeltrim_n" alpha
}

shader "textures/trim/trimgrating1"
{
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/trimgrating1_c"
		qer_editorimage "textures/trim/trimgrating1"
	normalmap "textures/trim/trimgrating1_n" alpha
}

shader "textures/trim/trimgrating2"
{
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/trimgrating2_c"
		qer_editorimage "textures/trim/trimgrating2"
	normalmap "textures/trim/trimgrating2_n" alpha
}

shader "textures/trim/cable1"
{
	tesssize 36
	surfparm rubber
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe1c_c"
		qer_editorimage "textures/trim/pipe1"
	normalmap "textures/trim/pipe1_n" alpha
}


shader "textures/trim/cable1_nonsolid"
{
	tesssize 36
	surfparm nonsolid
	surfparm rubber
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe1c_c"
		qer_editorimage "textures/trim/pipe1"
	normalmap "textures/trim/pipe1_n" alpha
}

shader "textures/trim/cable2"
{
	tesssize 36
	surfparm rubber
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_04_c"
	normalmap "textures/trim/Pipe_04_n" alpha
}


shader "textures/trim/cable2_nonsolid"
{
	tesssize 32
	surfparm nonsolid
	surfparm rubber
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_04_c"
	normalmap "textures/trim/Pipe_04_n" alpha
}

shader "textures/trim/rail"
{
	tesssize 64
	surfparm iron
	specular 0.3  //$alpha
castshadow
	diffusemap "textures/trim/rail_c"

}

shader "textures/trim/rail_16"
{
	tesssize 16
	surfparm iron
	specular 0.3  //$alpha
castshadow
	diffusemap "textures/trim/rail_c"

}

shader "textures/trim/pipe1"
{
	tesssize 32
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe1_c"
		qer_editorimage "textures/trim/pipe1"
	normalmap "textures/trim/pipe1_n" alpha
}

shader "textures/trim/pipe1_t64"
{
	tesssize 64
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe1_c"
		qer_editorimage "textures/trim/pipe1"
	normalmap "textures/trim/pipe1_n" alpha
}

shader "textures/trim/pipe1b"
{
	tesssize 32
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe1b_c"
		qer_editorimage "textures/trim/pipe1b"
	normalmap "textures/trim/pipe1b_n" alpha
}

shader "textures/trim/pipe1b_t64"
{
	tesssize 64
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe1b_c"
		qer_editorimage "textures/trim/pipe1b"
	normalmap "textures/trim/pipe1b_n" alpha
}

shader "textures/trim/pipe2"
{
	tesssize 128
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe2_c"
		qer_editorimage "textures/trim/pipe2"
	normalmap "textures/trim/pipe2_n" alpha
}

shader "textures/trim/pipe2_t64"
{
	tesssize 64
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe2_c"
		qer_editorimage "textures/trim/pipe2"
	normalmap "textures/trim/pipe2_n" alpha
}

shader "textures/trim/pipe2b"
{
	tesssize 128
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe2b_c"
		qer_editorimage "textures/trim/pipe2b"
	normalmap "textures/trim/pipe2b_n" alpha
}

shader "textures/trim/pipe2b_t64"
{
	tesssize 64
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/pipe2b_c"
		qer_editorimage "textures/trim/pipe2b"
	normalmap "textures/trim/pipe2b_n" alpha
}


shader "textures/trim/Pipe_03"
{
	tesssize 32
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_03_c"
	normalmap "textures/trim/Pipe_03_n" alpha
}

// nekde nejaka demence v modelu s trubkama
shader "textures/trim/Pipe_03_c"
{
	tesssize 32
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_03_c"
	normalmap "textures/trim/Pipe_03_n" alpha
}

shader "textures/trim/Pipe_03_t64"
{
	tesssize 64
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_03_c"
	normalmap "textures/trim/Pipe_03_n" alpha
}

shader "textures/trim/Pipe_03_t12"
{
	tesssize 12
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_03_c"
	normalmap "textures/trim/Pipe_03_n" alpha
}

shader "textures/trim/Pipe_03_t40"
{
	tesssize 40
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_03_c"
	normalmap "textures/trim/Pipe_03_n" alpha
}

shader "textures/trim/Pipe_04"
{
	tesssize 32
	surfparm rubber
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_04_c"
	normalmap "textures/trim/Pipe_04_n" alpha
}

shader "textures/trim/Pipe_05"
{
	tesssize 32
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_05_c"
	normalmap "textures/trim/Pipe_05_n" alpha
}

shader "textures/trim/Pipe_06"
{
	tesssize 128
	surfparm iron
	specular $nmapalpha
castshadow
	diffusemap "textures/trim/Pipe_06_c"
	normalmap "textures/trim/Pipe_06_n" alpha
}

shader "textures/trim/TrimMetal01"
{
	surfparm metal
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal01_c"		
	normalmap "textures/trim/TrimMetal01_n" alpha
}

shader "textures/trim/TrimMetal01a"
{
	surfparm metal
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal01a_c"		
	normalmap "textures/trim/TrimMetal01_n" alpha
}

shader "textures/trim/TrimMetal02"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal02_c"		
	normalmap "textures/trim/TrimMetal02_n" alpha
}

shader "textures/trim/TrimMetal03"
{
	surfparm iron
	specular $nmapalpha


	diffusemap "textures/trim/TrimMetal03_c"		
	normalmap "textures/trim/TrimMetal03_n" alpha
}

shader "textures/trim/TrimMetal03a"
{
	surfparm iron
	specular $nmapalpha


	diffusemap "textures/trim/TrimMetal03a_c"		
	normalmap "textures/trim/TrimMetal03_n" alpha
}

shader "textures/trim/TrimMetal04"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal04_c"		
	normalmap "textures/trim/TrimMetal04_n" alpha
}

shader "textures/trim/TrimMetal04a"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal04a_c"		
	normalmap "textures/trim/TrimMetal04_n" alpha
}
	
shader "textures/trim/TrimMetal05"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal05_c"		
	normalmap "textures/trim/TrimMetal05_n" alpha
}

shader "textures/trim/TrimMetal06"
{
	surfparm metal
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal06_c"		
	normalmap "textures/trim/TrimMetal06_n" alpha
}

shader "textures/trim/TrimMetal06a"
{
	surfparm metal
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal06a_c"		
	normalmap "textures/trim/TrimMetal06_n" alpha
}

shader "textures/trim/TrimMetal07"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal07_c"		
	normalmap "textures/trim/TrimMetal07_n" alpha
}

shader "textures/trim/TrimMetal08"
{
	surfparm beton
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal08_c"		
	normalmap "textures/trim/TrimMetal08_n" alpha
}

shader "textures/trim/TrimMetal09"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal09_c"		
	normalmap "textures/trim/TrimMetal09_n" alpha
}

shader "textures/trim/TrimMetal10"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal10_c"		
	normalmap "textures/trim/TrimMetal10_n" alpha
}

shader "textures/trim/TrimMetal10a"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal10a_c"		
	normalmap "textures/trim/TrimMetal10_n" alpha
}

shader "textures/trim/TrimMetal10b"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal10b_c"		
	normalmap "textures/trim/TrimMetal10b_n" alpha
}

shader "textures/trim/TrimMetal10c"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/TrimMetal10c_c"		
	normalmap "textures/trim/TrimMetal10b_n" alpha
}

shader "textures/trim/TrimMetal11"
{
	surfparm iron
	specular $nmapalpha
	tesssize 64

	diffusemap "textures/trim/TrimMetal11_c"		
	normalmap "textures/trim/TrimMetal11_n" alpha
}

shader "textures/trim/TrimMetal12"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal12_c"		
	normalmap "textures/trim/TrimMetal12_n" alpha
}

shader "textures/trim/TrimMetal13"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
	surfparm alphashadow

	specular $nmapalpha
  specularpower 0.43
	alphatest 16

	diffusemap "textures/trim/TrimMetal13_c"
	normalmap "textures/trim/TrimMetal13_n" alpha
}

shader "textures/trim/TrimMetal13a"
{

   surfparm trans
   surfparm detail
   surfparm nomarks
   surfparm metal
	surfparm alphashadow
 
	specular $nmapalpha
  specularpower 0.43
	alphatest 16

	diffusemap "textures/trim/TrimMetal13a_c"
	normalmap "textures/trim/TrimMetal13_n" alpha
}

shader "textures/trim/TrimMetal14"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal14_c"		
	normalmap "textures/trim/TrimMetal14_n" alpha
}

shader "textures/trim/TrimMetal14a"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal14a_c"		
	normalmap "textures/trim/TrimMetal14_n" alpha
}

shader "textures/trim/TrimMetal1401"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/TrimMetal1401_c"		
	normalmap "textures/trim/TrimMetal1401_n" alpha
}

shader "textures/trim/E_TrimMetal01"
{
	surfparm metal
	specular $nmapalpha

	diffusemap "textures/trim/E_TrimMetal01_c"		
	normalmap "textures/trim/E_TrimMetal01_n" alpha
}

shader "textures/trim/E_TrimMetal02"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/E_TrimMetal02_c"		
	normalmap "textures/trim/E_TrimMetal02_n" alpha
}

shader "textures/trim/E_TrimMetal03"
{
	surfparm iron
	specular $nmapalpha
   tesssize 128

	diffusemap "textures/trim/E_TrimMetal03_c"		
	normalmap "textures/trim/E_TrimMetal03_n" alpha
}

shader "textures/trim/H_MetalTrim_02"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/H_MetalTrim_02_c"		
	normalmap "textures/trim/H_MetalTrim_02_n" alpha
}

shader "textures/trim/H_MetalTrim_02_envspec"
{

	surfparm iron
	specular $nmapalpha
   reflection 0.0506
	diffusemap "textures/trim/H_MetalTrim_02_c"		
	normalmap "textures/trim/H_MetalTrim_02_n" alpha
	envmap $nearestcube
}


shader "textures/trim/H_MetalTrim_02b"
{
	surfparm iron
	specular $nmapalpha

	diffusemap "textures/trim/H_MetalTrim_02_c"		
	normalmap "textures/trim/H_MetalTrim_02b_n" alpha
}

shader "textures/trim/H_MetalTrim_03"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/H_MetalTrim_03_c"		
	normalmap "textures/trim/H_MetalTrim_03_n" alpha
}

shader "textures/trim/H_MetalTrim_04"
{
	surfparm iron
	specular $nmapalpha
	tesssize 128

	diffusemap "textures/trim/H_MetalTrim_04_c"		
	normalmap "textures/trim/H_MetalTrim_04_n" alpha
}
