/*
effect "height_fog_lmap"
{
	VertexShader vs = asm
	{
		vs.1.1
	
		dcl_position v0
		dcl_texcoord v1

		def c0, 0, 0, 0, 0 
		def c1, 1, 1, 1, 1

		; Transform position to clip space and output it
		dp4 oPos.x, v0, c2
		dp4 oPos.y, v0, c3
		dp4 oPos.z, v0, c4
		dp4 oPos.w, v0, c5
		
		mov oT0.xy, v1
		
		dp4 r9.x, v0, c22
		dp4 r9.y, v0, c23
		dp4 r9.z, v0, c24
		dp4 r9.w, v0, c25
		
		; Get height from floor
		add r10.y, r9.y, c26.x
		mul r10.y, r10.y, c26.y
		mov oT1.y, r10.y
		
		; Get range
		mov r9.y, c0.y
		add r9, r9, -c21
		dp3 r9, r9, r9
		rsq r9.w, r9.w
		rcp r9.w, r9.w
		mul oT1.x, r9.w, c26.z
	};

   technique t0
   {
   	pass p0
   	{
   		VertexShader = <vs>;
		}
	}
}
*/

/*
effect "g_addglow"
{
	texture tex;

	technique t0
	{
		pass p0
		{
			VertexShader = XYZRHW|TEX4;
			ShadeMode = Flat;
			FillMode = Solid;
			Lighting = False;
			
			ZEnable = False;
			ZWriteEnable = False;
			FogEnable = False;

      	ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
			
			TextureFactor = 0x40404040;
			StencilEnable = False;
			AlphaBlendEnable = True;
			AlphaTestEnable = False;

			BlendOp = Add;
			//SrcBlend = DestColor;//Alpha;
			SrcBlend = One;
			DestBlend = One;


	      TexCoordIndex[0] = 0;
	      TexCoordIndex[1] = 1;
	      TexCoordIndex[2] = 2;
	      TexCoordIndex[3] = 3;
			
			Texture[0] = <tex>;
			Texture[1] = <tex>;
			Texture[2] = <tex>;
			Texture[3] = <tex>;
			
      	ColorArg1[0] = Texture;
      	ColorArg1[1] = Texture;
      	ColorArg1[2] = Texture;
      	ColorArg1[3] = Texture;

      	ColorArg2[1] = Current;
      	ColorArg2[2] = Current;
      	ColorArg2[3] = Current;
      	
      	ColorOp[0] = SelectArg1;
//      	ColorOp[1] = AddSmooth;
  //    	ColorOp[2] = AddSmooth;
    //  	ColorOp[3] = AddSmooth;
      	ColorOp[1] = Add;
      	ColorOp[2] = Add;
      	ColorOp[3] = Add;
      	
      	ColorArg1[4] = Current;
      	ColorArg2[4] = TFactor;
      	ColorOp[4] = Modulate;

      	ColorOp[5] = Disable;
      	AlphaOp[0] = Disable;

			CullMode = None;
		}
	}
}

effect "g_glow"
{
	float4 col;
	
	technique t0
	{
		pass p0
		{
		
			AlphaBlendEnable = False;
			AlphaTestEnable = False;

			TextureFactor = <col>;
      	ColorArg1[0] = TFactor;
      	ColorOp[0] = SelectArg1;
      	AlphaArg1[0] = TFactor;
      	AlphaOp[0] = SelectArg1;
      	ColorOp[1] = Disable;
      	AlphaOp[1] = Disable;
		}
	}
}

effect "g_glowl"
{
	float4 col;
	texture tex0;
	
	technique t0
	{
		pass p0
		{
			AlphaBlendEnable = False;
			AlphaTestEnable = False;
	
			AddressU[0] = Clamp;
			AddressV[0] = Clamp;

	      TexCoordIndex[0] = 1;

			Texture[0]= <tex0>;
			TextureFactor = <col>;
      	ColorArg1[0] = Texture;
      	ColorOp[0] = SelectArg1;
      	ColorArg1[1] = TFactor;
      	ColorArg2[1] = Current;
      	ColorOp[0] = Modulate;
      	ColorOp[2] = Disable;
      	AlphaOp[0] = Disable;
		}
	}
}

effect "g_softshadow"
{
	dword pow;
	
	technique t0
	{
		pass p0
		{
			VertexShader = XYZRHW;
			ShadeMode = Flat;
			FillMode = Solid;
			Lighting = False;
			
			ZEnable = False;
			ZWriteEnable = False;
			FogEnable = False;

      	ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
			
			TextureFactor = <pow>;
			StencilEnable = True;
			AlphaBlendEnable = True;
			AlphaTestEnable = False;

			BlendOp = RevSubtract;
			SrcBlend = SrcAlpha;
			DestBlend = One;
			
      	ColorArg1[0] = TFactor;
      	ColorOp[0] = SelectArg1;
      	AlphaArg1[0] = TFactor;
      	AlphaOp[0] = SelectArg1;
      	ColorOp[1] = Disable;
      	AlphaOp[1] = Disable;

			StencilRef = 1;
			StencilFunc = LessEqual;
			StencilPass = DecrSat;
			StencilFail = Keep;
			StencilZFail = Keep;
			CullMode = None;
		}

		pass p1
		{
			StencilPass = Zero;
		}
	}
}
*/


/*
shader "textures/efx/gradient"
{
//	qer_trans 0.60
//   surfparm nocastshadow
   surfparm nonsolid
   surfparm trans
//   surfparm nolightmap
//	sort 30
   cull ccw

   pass 0

   pass 1
      tcmod TURB 0.2 g_fTime 0.13
      tcmod SCALE 1.0 0.8 1.0 0.8
      tcmod SHIFT g_fTime01 -8

	layer "textures/efx/gradient" alpha
	layer "obj/lamp/lampbeamnoise" alpha

	tex0 = layer 0
	tex1 = layer 1
	effect "effect_lampbeams"
}
*/

/*
shader "textures/efx/watermud1"
{
	qer_trans 0.40
	surfparm water
	surfparm trans
   surfparm nonsolid
   surfparm nodrop
	tesssize 32
   sort 1

//	cull none


	pass 0
		vcmod SIN g_fTime 3.0 3.0 1.0
		vcmod SIN g_fTime 1.0 1.0 0.5
//		tcmod SHIFT g_fTime01 0
      tcmod SCALE 2.0 2.0 2.0 2.0

   pass 1
		vcmod SIN g_fTime 3.0 3.0 1.0
		vcmod SIN g_fTime 1.0 1.0 0.5
      tcmod turb 0.5 g_fTime 0.08
//		tcmod SHIFT g_fTime01 0


	pass 2
		vcmod SIN g_fTime 3.0 3.0 1.0
		vcmod SIN g_fTime 1.0 1.0 0.5
      tcmod turb 0.5 g_fTime 0.15
//		tcmod SHIFT g_fTime01 0

   pass 3
		vcmod SIN g_fTime 3.0 3.0 1.0
		vcmod SIN g_fTime 1.0 1.0 0.5
      tcmod turb 0.8 g_fTime01 0.06
//		tcmod SHIFT g_fTime01 0


	layer "textures/efx/watermud1" alpha
	layer $lightmap
	layer	"textures/efx/caust"

   tex0 = layer 0
   tex1 = layer 1
   tex2 = layer 2

	effect
	{
  	texture tex0;
   texture tex1;
   texture tex2;

   technique l0
   {
   	pass p0
   	{
   		ZWriteEnable = True;
         AlphaBlendEnable = True;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;

   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;
   		AddressU[1] = Clamp;
   		AddressV[1] = Clamp;

   		Texture[0] = <tex0>;
         Texture[1] = <tex1>;

         ColorArg1[0] = Texture;
         AlphaArg1[0] = Texture;
         ColorOp[0] = SelectArg1;
         AlphaOp[0] = SelectArg1;
         TexCoordIndex[0] = 0;

         ColorArg1[1] = Texture;
         ColorArg2[1] = Current;
         ColorOp[1] = Modulate;
         AlphaArg1[1] = Current;
         AlphaOp[1] = SelectArg1;
         TexCoordIndex[1] = 1;

         ColorOp[2] = Disable;
         AlphaOp[2] = Disable;
   	}

		pass p1
		{
         AlphaBlendEnable = True;
         SrcBlend  = InvSrcAlpha;
         DestBlend = InvDestColor;
         BlendOp = Add;
   		AlphaTestEnable = False;


			AddressU[0] = Wrap;
			AddressV[0] = Wrap;
			AddressU[1] = Clamp;
			AddressV[1] = Clamp;

			Texture[0] = <tex2>;
   		Texture[1] = <tex1>;

	      ColorArg1[0] = Texture;
	      ColorArg2[0] = Diffuse;
	      ColorOp[0] = SelectArg1;//Modulate;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;
	      TexCoordIndex[0] = 0;

         ColorArg1[1] = Texture;
         ColorArg2[1] = Current;
         ColorOp[1] = Modulate2x;
         AlphaArg1[1] = Current;
         AlphaOp[1] = SelectArg1;
         TexCoordIndex[1] = 1;
	      ColorOp[2] = Disable;
	      AlphaOp[2] = Disable;
		}

   	pass p2
   	{
         AlphaBlendEnable = True;
         SrcBlend  = SrcColor;
         DestBlend = One;
         BlendOp = Add;

   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;
   		AddressU[1] = Clamp;
   		AddressV[1] = Clamp;

   		Texture[0] = <tex2>;
         Texture[1] = <tex1>;

         ColorArg1[0] = Texture;
         AlphaArg1[0] = Texture;
         ColorOp[0] = SelectArg1;
         AlphaOp[0] = SelectArg1;
         TexCoordIndex[0] = 0;

         ColorArg1[1] = Texture;
         ColorArg2[1] = Current;
         ColorOp[1] = Modulate;
         AlphaArg1[1] = Current;
         AlphaOp[1] = SelectArg1;
         TexCoordIndex[1] = 1;

         ColorOp[2] = Disable;
         AlphaOp[2] = Disable;
   	}

		pass p3
		{
         AlphaBlendEnable = True;
         SrcBlend  = SrcColor;
         DestBlend = One;//DestColor;
         BlendOp = Add;
   		AlphaTestEnable = False;


			AddressU[0] = Wrap;
			AddressV[0] = Wrap;
			AddressU[1] = Clamp;
			AddressV[1] = Clamp;

			Texture[0] = <tex2>;
   		Texture[1] = <tex0>;

	      ColorArg1[0] = Texture;
	      ColorArg2[0] = Diffuse;
	      ColorOp[0] = SelectArg1;//Modulate;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;
	      TexCoordIndex[0] = 0;

         ColorArg1[1] = Texture;
         ColorArg2[1] = Current;
         ColorOp[1] = Modulate2x;
         AlphaArg1[1] = Current;
         AlphaOp[1] = SelectArg1;
         TexCoordIndex[1] = 1;

	      ColorOp[2] = Disable;
	      AlphaOp[2] = Disable;
		}

   }
}
}
*/

/*
shader "textures/efx/mud"
{
	qer_trans 0.40
	surfparm water
	surfparm trans
	tesssize 32
	cull none

//SIN casu Ux Vx amplituda
	pass 0
		vcmod SIN g_fTime 1.0 1.0 2.0
//		tcmod SHIFT g_fTime01 0
//		tcmod turb g1 g_fTime01 g2

	layer "textures/efx/mud" alpha
	layer $lightmap

	tex0 = layer 0
	tex1 = layer 1

	effect
	{
  	texture tex0;
   texture tex1;

   technique l0
   {
   	pass p0
   	{
         AlphaBlendEnable = True;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;

   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;
   		AddressU[1] = Clamp;
   		AddressV[1] = Clamp;

   		Texture[0] = <tex0>;
         Texture[1] = <tex1>;

         ColorArg1[0] = Texture;
         AlphaArg1[0] = Texture;
         ColorOp[0] = SelectArg1;
         AlphaOp[0] = SelectArg1;
         TexCoordIndex[0] = 0;

         ColorArg1[1] = Texture;
         ColorArg2[1] = Current;
         ColorOp[1] = Modulate2x;
         AlphaArg1[1] = Current;
         AlphaOp[1] = SelectArg1;
         TexCoordIndex[1] = 1;

         ColorOp[2] = Disable;
         AlphaOp[2] = Disable;
   	}
   }
   }
}
*/

/*
shader "textures/efx/water001"
{
	surfparm water
	surfparm trans
	cull none
//	tesssize 32

//SIN casu Ux Vx amplituda
	pass 0
//		vcmod SIN g_fTime 1.0 1.0 9.0
		tcmod SHIFT g_fTime01 0
//		tcmod turb g1 g_fTime01 g2

	layer "textures/efx/water001" alpha
	layer $lightmap

	tex0 = layer 0
	tex1 = layer 1

	effect
	{
  	texture tex0;
   texture tex1;

   technique l0
   {
   	pass p0
   	{
         AlphaBlendEnable = True;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;

   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;
   		AddressU[1] = Clamp;
   		AddressV[1] = Clamp;

   		Texture[0] = <tex0>;
         Texture[1] = <tex1>;

         ColorArg1[0] = Texture;
         AlphaArg1[0] = Texture;
         ColorOp[0] = SelectArg1;
         AlphaOp[0] = SelectArg1;
         TexCoordIndex[0] = 0;

         ColorArg1[1] = Texture;
         ColorArg2[1] = Current;
         ColorOp[1] = Modulate2x;
         AlphaArg1[1] = Current;
         AlphaOp[1] = SelectArg1;
         TexCoordIndex[1] = 1;

         ColorOp[2] = Disable;
         AlphaOp[2] = Disable;
   	}
   }
   }
}
*/

//
/*
shader "obj/k4l4/bankaliq2"
{
//	surfparm nolightmap 
	surfparm trans
	sort 10
   cull none
   
	layer alpha
	   anim 15
		"obj/fx/magic01_001"
		"obj/fx/magic01_002"
		"obj/fx/magic01_003"
		"obj/fx/magic01_004"
		"obj/fx/magic01_005"
		"obj/fx/magic01_006"
		"obj/fx/magic01_007"
		"obj/fx/magic01_008"
		"obj/fx/magic01_009"
		"obj/fx/magic01_010"
		"obj/fx/magic01_011"
		"obj/fx/magic01_012"
		"obj/fx/magic01_013"
		"obj/fx/magic01_014"
		"obj/fx/magic01_015"
	
   tex0 = layer 0
   effect "liquid_small"
}


shader "obj/decors/semafor_alpha"
{
	cull none
	layer "obj/ldecors/semafor_schod" alpha
	tex0 = layer 0
	effect "nolightmap_clip"
}



shader "obj/k4l4/bankaliq1"
{
//	surfparm nolightmap 
	surfparm trans
	sort 10
   cull none
   
	layer alpha
	   anim 15
		"obj/fx/magic01_001b"
		"obj/fx/magic01_002b"
		"obj/fx/magic01_003b"
		"obj/fx/magic01_004b"
		"obj/fx/magic01_005b"
		"obj/fx/magic01_006b"
		"obj/fx/magic01_007b"
		"obj/fx/magic01_008b"
		"obj/fx/magic01_009b"
		"obj/fx/magic01_010b"
		"obj/fx/magic01_011b"
		"obj/fx/magic01_012b"
		"obj/fx/magic01_013b"
		"obj/fx/magic01_014b"
		"obj/fx/magic01_015b"
	
   tex0 = layer 0
   effect "liquid_small_trans"
}

shader "obj/k4l4/bankaliq3"
{
  
	layer alpha
	   anim 15
		"obj/fx/magic01_001"
		"obj/fx/magic01_002"
		"obj/fx/magic01_003"
		"obj/fx/magic01_004"
		"obj/fx/magic01_005"
		"obj/fx/magic01_006"
		"obj/fx/magic01_007"
		"obj/fx/magic01_008"
		"obj/fx/magic01_009"
		"obj/fx/magic01_010"
		"obj/fx/magic01_011"
		"obj/fx/magic01_012"
		"obj/fx/magic01_013"
		"obj/fx/magic01_014"
		"obj/fx/magic01_015"
	
   layer "textures/env/water"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

// K3l2
shader "obj/k3l6/labor_tank"
{
   layer "obj/k3l6/labor_tank.jpg" alpha

   tex0 = layer 0
   colr = int g_iKanystrColor

	effect
	{
	   texture tex0;
	   dword colr;
	   
	   technique t0
	   {
	   	//nejdriv nepruhlednou cast kontejneru (nebude ovlivnena vstupni barvou)
	   	pass p0 //prvni pruchod (polygon se kresli poprve)
	   	{
	   		ZWriteEnable = True;	//zapisuj do zbufferu
	         AlphaBlendEnable = False;	//neblenduj (kreslime nepruhlednou cast)
	   		AlphaTestEnable = True;	//a nastav test na hodnotu alfy
				AlphaFunc = Greater;	//alfa musi byt vetsi
				AlphaRef = 250;	//nez 250
	  
				AddressU[0] = Wrap; //textura se opakuje kolem dokola (Clamp)
				AddressV[0] = Wrap;
	   		Texture[0] = <tex0>; //priradime texturu kontejneru pro 1. stage
		      TexCoordIndex[0] = 0; //pouzijeme prvni par UV souradnic(pouziva se temer vzdy)
	           
	         ColorArg1[0] = Texture; //prvni vstupni argument je texel z textury
	         ColorArg2[0] = Diffuse; //druhy je svetlo dopadajici na objekt
	         ColorOp[0] = Modulate2x; //vynasob je (a jeste 2x navic)
	
	         AlphaArg1[0] = Texture; //alfu jen propust skrz prvni stage (na vystupu se kontroluje)
	         AlphaOp[0] = SelectArg1;
	           
	         ColorOp[1] = Disable; //dalsi stage nepouzivame
	         AlphaOp[1] = Disable;
	   	}
	
			//kreslime pruhlednou cast kontejneru, kterou navic nasobime barvou ze skriptu
	   	pass p1 //druhy pruchod (polygon se kresli podruhe)
	   	{
	   		ZWriteEnable = False; //pruhledne body se NESMI zapisovat do zbufferu!
	
	         AlphaBlendEnable = True; //zapni alfa blending
	         SrcBlend  = SrcAlpha; //nastavime alfablending na P = T * SourceAlfa + P * (1 - SourceAlfa)
	         DestBlend = InvSrcAlpha;
	         BlendOp = Add;
	         
				AlphaFunc = Lessequal;	//alfa musi byt mensi nebo stejna nez 250
				TextureFactor = <colr>; //nastav temporary register (TextureFactor) na RGB hodnotu ze skriptu
	
												//prvni stage je stejna jako v predchozim passu, tak ji neopakujem
	
	         ColorArg1[1] = Current; //v dalsi stage vynasobime vysledny pixel jeste hodnotou ze skriptu
	         ColorArg2[1] = TFactor; //odkaz na (TextureFactor) register
	         ColorOp[1] = Modulate; //nasobeni
	          
	         ColorOp[2] = Disable; //ostatni stage nepouzijem
	   	}
	   }
   }
}

shader "obj/k1l3/kohoutek"
{	
	cull none
   layer "obj/k1l3/kohoutek"

   tex0 = layer 0
	effect "nolightmap_clip"

}

shader "obj/k1l3/airradar"
{	
	cull none
   layer "obj/k1l3/airradar"

   tex0 = layer 0
	effect "nolightmap"

}

shader "obj/k1l3/radaralpha"
{	
	cull none
   layer "obj/k1l3/radaralpha"

   tex0 = layer 0
	effect "nolightmap_clip"

}


//core k3l5

shader "textures/misc/server_box_01_vl"
{	
	cull ccw
   layer "textures/misc/server_box_01"

   tex0 = layer 0
	effect "nolightmap"

}

shader "textures/steel/nosnik_vl"
{	
	cull ccw
   layer "textures/steel/nosnik"

   tex0 = layer 0
	effect "nolightmap"

}


shader "textures/steel/pas_stroj_05_vl"
{	
	cull ccw
   layer "textures/steel/pas_stroj_05"

   tex0 = layer 0
	effect "nolightmap"

}


shader "textures/steel/trubka1c_vl"
{	
	cull ccw
   layer "textures/steel/trubka1c"

   tex0 = layer 0
	effect "nolightmap"

}


shader "textures/wire/kanal_021_vl"
{	
	cull ccw
   layer "textures/wire/kanal_02"

   tex0 = layer 0
	effect "nolightmap"

}

*/

/*
//-----------------------

shader "obj/misc/pneu"
{
   cull none
	layer "obj/misc/pneu"
	tex0 = layer 0
	effect "nolightmap"
}

shader "obj/k3l4/vozik_pradlo"
{

   layer "obj/k3l4/vozik_pradlo" alpha
   layer $lightmap

   tex0 = layer 0
   tex1 = layer 1

   effect "nolightmap_clip"
}

shader "obj/misc/ponk"
{

   cull none

   layer "obj/misc/ponk"
   layer $lightmap

   tex0 = layer 0
   tex1 = layer 1

   effect "nolightmap"
}




// monitory
effect "monitor_display"
   {

   texture tex0;
   texture tex1;
   technique t2
   {
   	pass p0
   	{
   		ZWriteEnable = False;
         AlphaBlendEnable = False;
   		AlphaTestEnable = False;

			AddressU[0] = Wrap;
			AddressV[0] = Wrap;
   		Texture[0] = <tex0>;
	      TexCoordIndex[0] = 0;

         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = SelectArg1;//Modulate2x;

         ColorOp[1] = Disable;
         AlphaOp[0] = Disable;
   	}

   	pass p1
   	{
   		ZWriteEnable = True;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;
         AlphaBlendEnable = True;
//   		AlphaTestEnable = False;

   		Texture[0] = <tex1>;
	      TexCoordIndex[0] = 0;

         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = Modulate2x;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;

         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
}

shader "obj/k3l4/monitor"
{

	layer "obj/k3l4/monitor1disp"
	layer "obj/k3l4/monitor"

	pass 0
		tcmod SHIFT g_fTime001 0
      tcmod SCALE 1.0 1.0 3.0 3.0
   pass 1

	tex0 = layer 0
	tex1 = layer 1

   effect "monitor_display"
}

shader "obj/k3l4/monitor2"
{

	layer "obj/k3l4/monitor2disp"
	layer "obj/k3l4/monitor"

	pass 0
		tcmod SHIFT g_fTime01 g_fTime
      tcmod SCALE 1.0 1.0 3.0 3.0
   pass 1

	tex0 = layer 0
	tex1 = layer 1

   effect "monitor_display"
}

shader "obj/k3l4/monitor3"
{

	layer "obj/k3l4/monitor3disp"
	layer "obj/k3l4/monitor"

	pass 0
		tcmod SHIFT g_fTime g_fTime
//      tcmod SCALE 1.0 1.0 3.0 3.0
   pass 1

	tex0 = layer 0
	tex1 = layer 1

   effect "monitor_display"
}

//mechanismezecky

shader "obj/misc/desta"
{
   cull none
	layer "obj/misc/desta"
	tex0 = layer 0
	effect "nolightmap_clip"
}

shader "obj/k3l5a/cryo2rantl"
{
   cull none
	layer "obj/k3l5a/cryo2panel"
	tex0 = layer 0
	effect "nolightmap_clip"
}

shader "obj/k3l5a/cryo2panel"
{

	pass 0
		tcmod SHIFT g_fTime01 0.585
   pass 1

	layer "obj/k3l5a/cryo2panel"
	tex0 = layer 0

	effect
   {

   texture tex0;
   technique t2
   {
   	pass p0
   	{
   		ZWriteEnable = False;
         AlphaBlendEnable = False;
   		AlphaTestEnable = False;

			AddressU[0] = Wrap;
			AddressV[0] = Wrap;
   		Texture[0] = <tex0>;
	      TexCoordIndex[0] = 0;

         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = SelectArg1;//Modulate2x;

         ColorOp[1] = Disable;
         AlphaOp[0] = Disable;
   	}

   	pass p1
   	{
   		ZWriteEnable = True;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;
         AlphaBlendEnable = True;
//   		AlphaTestEnable = False;

   		Texture[0] = <tex0>;
	      TexCoordIndex[0] = 0;

         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = Modulate2x;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;

         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
   }

}

// armatury

shader "obj/armatury/ar01_koleno"
{
	layer "textures/steel/kov"
	tex0 = layer 0
	effect "nolightmap"
}

shader "obj/armatury/ar01_kolenorez"
{
	layer "textures/steel/strojlidumil_02b"
	tex0 = layer 0
	effect "nolightmap"
}

shader "obj/armatury/ar01_koncovka"
{
	layer "textures/steel/kov"
	tex0 = layer 0
	effect "nolightmap"
}

shader "obj/armatury/ar01_koncovkarez"
{
	layer "textures/steel/strojlidumil_02b"
	tex0 = layer 0
	effect "nolightmap"
}

// ammo

shader "obj/ammo/223rem50"
{	
   layer "obj/ammo/223rem_50"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/308win50"
{	
   layer "obj/ammo/308win_50"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/9mm25"
{	
   layer "obj/ammo/9mm_25"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/9mm100"
{	
   layer "obj/ammo/9mm_100"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/45acp25"
{	
   layer "obj/ammo/45acp_25"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/45acp100"
{	
   layer "obj/ammo/45acp_100"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/12brok25"
{	
   layer "obj/ammo/12brok_25"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/12brokslug10"
{	
   layer "obj/ammo/12brokslug_10"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/12brok100"
{	
   layer "obj/ammo/12brok_100"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/ammo/clipsg55x30"
{	
   layer "obj/ammo/clip_sg55x_30"
   layer "textures/env/metal"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/ammo/9mmclip17"
{	
   layer "obj/ammo/clip_9mm_17"
   layer "textures/env/metal2"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/ammo/9mmclip30"
{	
   layer "obj/ammo/clip_9mm_30"
   layer "textures/env/metal2"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/ammo/p245clip6"
{	
   layer "obj/ammo/clip_9mm_17"
   layer "textures/env/metal2"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/ammo/223rem300"
{	
   layer "obj/ammo/223rem_300"
   layer "textures/env/metal"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}

shader "obj/ammo/ft_canyster"
{	
   layer "obj/ammo/ft_canyster"
   layer "textures/env/keram"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}

shader "obj/ammo/gl_grenade"
{	
   layer "obj/ammo/gl_grenade"
   layer "textures/env/keram"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}



// small things


shader "obj/k4l2/seno"
{	
   cull none
   layer "obj/k4l2/seno"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/misc/vetrak"
{	
   cull none
   layer "obj/misc/vetrak"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/misc/regal"
{	
//		castshadow
   cull none
   layer "obj/misc/regal"

   tex0 = layer 0
	effect "nolightmap_clip"
}

shader "obj/misc/popelnik"
{
	layer "obj/misc/popelnik"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/misc/umyvadlo"
{
	layer "obj/misc/umyvadlo"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/misc/bojler"
{
	layer "obj/misc/bojler"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}


shader "obj/misc/musle"
{
	layer "obj/misc/musle"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/misc/musleble"
{
	layer "obj/misc/musleble"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/misc/zachod"
{
	layer "obj/misc/zachod"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/misc/zachodble"
{
	layer "obj/misc/zachodble"
   layer "textures/env/dlazky"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}



shader "obj/misc/hasicak_mobilni"
{	
   layer "obj/misc/hasicak_mobilni"
   layer "textures/env/keram"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}

shader "obj/misc/hasicak_mensi"
{	
   layer "obj/misc/hasicak_mensi"
   layer "textures/env/keram"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}

shader "obj/misc/hasicak_vetsi"
{	
   layer "obj/misc/hasicak_vetsi"
   layer "textures/env/keram"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}

shader "obj/k4l2/stit"
{	
   layer "obj/k4l2/stit"
   layer "textures/env/metal"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"
}

shader "obj/k4l4/stit2"
{	
   layer "obj/k4l4/stit2"
   layer "textures/env/metal2"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_mask"

}


shader "obj/k4l2/krutinoha"
{
   cull none
	layer "obj/k4l2/krutinoha"
	tex0 = layer 0
	effect "nolightmap"
}

shader "obj/k4l4/nadoba"
{	
   layer "obj/k4l4/nadoba"
   layer "textures/env/metal"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"

}

shader "obj/k4l4/banka_krystal"
{	
	surfparm alphashadow
	surfparm trans
	cull none
	sort 10

   layer "obj/k4l4/banka_krystal"
   layer "textures/env/glass"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_trans"

}

shader "obj/k4l4/banka"
{	
	surfparm alphashadow
	surfparm trans
	cull none
	sort 10

   layer "obj/k4l4/banka"
   layer "textures/env/glass"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_trans"

}

shader "obj/k4l4/banka2"
{	
	surfparm alphashadow
	surfparm trans
	cull none
	sort 10

   layer "obj/k4l4/banka2"
   layer "textures/env/glass"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_trans"

}


// furniture

shader "obj/misc/lekarnicka_stara"
{	
	cull none
   layer "obj/misc/lekarnicka_stara"

   tex0 = layer 0
	effect "nolightmap"

}

shader "obj/misc/lekarnicka_stara_glass"
{	
	surfparm trans
	cull none
	sort 10

   layer "obj/misc/lekarnicka_stara"
   layer "textures/env/glass2"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_trans"

}

shader "obj/k4l2/trun"
{
	surfparm alphashadow
   surfparm trans
   cull none
	layer "obj/k4l2/trun" alpha

	tex0 = layer 0
	effect "nolightmap_clip"
}

// mechanisms

shader "obj/k4l4/picka"
{	
	surfparm alphashadow
	surfparm trans
   cull none

	layer "obj/k4l4/picka" alpha
	tex0 = layer 0

	effect "nolightmap_clip"
}


// misc
shader "obj/k4l2/praporec_static"
{	
	cull none
	
   layer "obj/k4l2/praporec" alpha
   tex0 = layer 0
	effect "nolightmap_clip"
}
	
shader "obj/k4l2/praporec"
{	
	cull none
   layer "obj/k4l2/praporec" alpha
   tex0 = layer 0
	effect "nolightmap_clip"
}

shader "obj/k4l4/vlajka"
{	
	cull none
   layer "obj/k4l4/vlajka"
   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/k4l4/vlajka_static"
{	
	cull none
   layer "obj/k4l4/vlajka"
   tex0 = layer 0
	effect "nolightmap"

}



shader "obj/creatures/vlajecka"
{	
	surfparm trans
	cull none

	layer "obj/creatures/vlajecka" alpha
	tex0 = layer 0

//	effect "nolightmap_trans_fullbright"
	effect "nolightmap_clip"
}

shader "obj/creatures/vlajecka1"
{	
	surfparm trans
	cull none

	layer "obj/creatures/vlajecka1" alpha
	tex0 = layer 0

//	effect "nolightmap_trans_fullbright"
	effect "nolightmap_clip"

}

shader "obj/creatures/vlajecka2"
{	
	surfparm trans
	cull none

	layer "obj/creatures/vlajecka2" alpha
	tex0 = layer 0

//	effect "nolightmap_trans_fullbright"
	effect "nolightmap_clip"

}

shader "obj/creatures/vlajecka3"
{	
	surfparm trans
	cull none

	layer "obj/creatures/vlajecka3" alpha
	tex0 = layer 0

//	effect "nolightmap_trans_fullbright"
	effect "nolightmap_clip"

}


shader "obj/misc/hodiny_vt"
{	
//	surfparm trans
   layer "obj/misc/hodinyhruc"
   layer "obj/misc/hodinymruc"
   layer "obj/misc/hodinyvruc"

	pass 0  // hours
		tcmod rotate g_fHHRotate 0.5 0.5

	pass 1  // mins
		tcmod rotate g_fHMRotate 0.5 0.5

	pass 2  // seconds
		tcmod rotate g_fHSRotate 0.5 0.5

   tex0 = layer 0
   tex1 = layer 1
   tex2 = layer 2

	effect 
   {
   texture tex0;
   texture tex1;
   texture tex2;

   technique t2
   {
   	pass p0
   	{
   		ZWriteEnable = True;
         AlphaBlendEnable = False;
			AlphaFunc = Greater;
			AlphaRef = 127;
   		AlphaTestEnable = True;
   		
   		Texture[0] = <tex0>;
	      TexCoordIndex[0] = 0;
           
         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = Modulate2x;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;
           
         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}

   	pass p1
   	{
   		Texture[0] = <tex1>;
	      TexCoordIndex[0] = 0;
           
         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = Modulate2x;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;
           
         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}

   	pass p2
   	{
   		Texture[0] = <tex2>;
	      TexCoordIndex[0] = 0;
           
         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         ColorOp[0] = Modulate2x;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;
           
         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
   }
}

shader "obj/k1l3/fuel_tower_nadrz_vybuch"
{	
   cull none
   layer "obj/k1l3/fuel_tower_nadrz_vybuch"

   tex0 = layer 0
	effect "nolightmap"
}

shader "obj/k1l3/fireaxe_glass"
{	
   surfparm trans
   sort 2
   
   layer "obj/k1l3/fireaxe" alpha
   layer "textures/env/glass"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env_trans_mask"
}

shader "obj/k1l3/fuel_tower"
{	
   cull none
   surfparm alphashadow
   surfparm trans
   
   layer "obj/k1l3/fuel_tower" alpha

   tex0 = layer 0
	effect "nolightmap_clip"
}

// ************
shader "obj/misc/hodiny_glass"
{	
   layer "obj/misc/hodiny"
   layer "textures/env/glass"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}

shader "obj/misc/lahev"
{	
   cull none
   layer "obj/misc/lahev" 
   layer "textures/env/glass"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}


shader "obj/k5l0/chain"
{	
	surfparm trans
	cull none

	layer "obj/k5l0/chain" alpha
	tex0 = layer 0

//	effect "nolightmap_trans_fullbright"
	effect "nolightmap_clip"

}
*/

/*
// zombie civil---- prozatimni shader

shader "obj/creatures/zombie_civil/kalhoty"
{
	castshadow
	
	layer
		anim skin1
		"obj/creatures/zombie_civil/kalhoty"
		"obj/creatures/zombie_civil/kalhoty2"
		"obj/creatures/zombie_civil/kalhoty3"
		
	tex0 = layer 0

	effect "creature"
}

shader "obj/creatures/zombie_civil/kosile"
{
	castshadow
	
	layer
		anim skin2
		"obj/creatures/zombie_civil/kosile"
		"obj/creatures/zombie_civil/kosile2"
		"obj/creatures/zombie_civil/kosile3"
		
	tex0 = layer 0

	effect "creature"
}

shader "obj/creatures/zombie_civil/kosile_pasek2"
{
	castshadow
	
	layer
		anim skin2
		"obj/creatures/zombie_civil/kosile_pasek"
		"obj/creatures/zombie_civil/kosile_pasek2"
		"obj/creatures/zombie_civil/kosile_pasek3"
		
	tex0 = layer 0

	effect "creature"
}


shader "obj/creatures/zombie_civil/kuze"
{
	castshadow
	
	layer
		anim skin3
		"obj/creatures/zombie_civil/kuze"
		"obj/creatures/zombie_civil/kuze2"
		"obj/creatures/zombie_civil/kuze3"
		"obj/creatures/zombie_civil/kuze4"
		
	tex0 = layer 0

	effect "creature"
}
*/



/*
shader "obj/creatures/zombie/zombie_body"
{
	castshadow
	layer "obj/creatures/zombie/zombie_body"
	tex0 = layer 0

	effect "creature"
}
*/

/*	
//Miner zombie
shader "obj/creatures/miner_zombie/miner_zomb_legs"
{
	castshadow
	layer "obj/creatures/miner_zombie/miner_zomb_legs"
	tex0 = layer 0

	effect "creature"
}

shader "obj/creatures/miner_zombie/miner_zomb"
{
	castshadow
	layer "obj/creatures/miner_zombie/miner_zomb"
	tex0 = layer 0

	effect "creature"
}

shader "obj/creatures/miner_zombie/miner_brasna"
{
	castshadow
	layer "obj/creatures/miner_zombie/miner_brasna"
	tex0 = layer 0

	effect "creature"	
}

shader "obj/creatures/miner_zombie/miner_cap"
{
	castshadow
	cull none

	layer "obj/creatures/miner_zombie/miner_cap"
	tex0 = layer 0

	effect "creature"	
}
	
shader "obj/creatures/miner_zombie/miner_zomb_head1"
{
	castshadow
	layer
		anim skin3
			"obj/creatures/miner_zombie/miner_zomb_head1"
			"obj/creatures/miner_zombie/miner_zomb_head2"

	tex0 = layer 0

	effect "creature"	
}
			
// pro minera - zombii: zare oci
shader "obj/creatures/miner_zombie/miner_zombie_eyeshine"
{
	layer "obj/creatures/miner_zombie/miner_zombie_eyeshine" alpha
	tex0 = layer 0

effect
{
	texture tex0;
   
   technique t0
   {
   	pass p0
   	{
   		ZWriteEnable = True;
         AlphaBlendEnable = True;
         SrcBlend  = SrcAlpha;
   		AlphaTestEnable = False;
         DestBlend = One;
         BlendOp = Add;
         AlphaTestEnable = True;
         AlphaRef = 127;
         AlphaFunc = Greater;
         
   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;
   		
   		Texture[0] = <tex0>;
           
         ColorArg1[0] = Texture;
         ColorArg2[0] = Texture;
         AlphaArg1[0] = Texture;
         ColorOp[0] = Add;
         AlphaOp[0] = SelectArg1;
         TexCoordIndex[0] = 0;
           
         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
}
}

shader "obj/creatures/ghost/ghost"
{
	layer "obj/creatures/ghost/ghost" alpha
	tex0 = layer 0

	effect "creature"
}

shader "obj/creatures/ghost/ghost_trans"
{
	sort 31
	layer "obj/creatures/ghost/ghost" alpha
	layer "textures/env/metal"
	layer "obj/creatures/ghost/ghost_gradient" alpha
	tex0 = layer 0
	tenv = layer 1
	tex1 = layer 2

	effect
{

   texture tex0;
   texture tenv;
   texture tex1;
     
   technique e0
   {

      pass p0
      {
   		ZWriteEnable = False;
         AlphaBlendEnable = True;
         SrcBlend = SrcAlpha;
         DestBlend = One;
         BlendOp = Add;

   		Texture[1] = <tex1>;
   		Texture[0] = <tenv>;
   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;


	      ColorArg1[0] = Texture;
	      ColorArg2[0] = Diffuse;
	      ColorOp[0] = Modulate4x;
	      TexCoordIndex[0] = CameraSpaceReflectionVector;

	      ColorArg1[1] = Current;
	      AlphaArg1[1] = Texture;
	      AlphaOp[1] = SelectArg1;
	      ColorOp[1] = SelectArg1;
	      TexCoordIndex[1] = 0;

	      ColorOp[2] = Disable;
	      AlphaOp[2] = Disable;
      }

   	pass p1
   	{
         AlphaBlendEnable = True;
   		AlphaTestEnable = False;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;
         
   		Texture[0] = <tex0>;
	      TexCoordIndex[0] = 0;

           
         ColorArg1[0] = Texture;
         ColorArg2[0] = Diffuse;
         AlphaArg1[0] = Texture;
         ColorOp[0] = Modulate2x;
         AlphaOp[0] = SelectArg1;
           
         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
	}
}

*/

/*
// darklord - zare oci
shader "obj/creatures/darklord/darklordeyes"
{

   
	layer "obj/creatures/darklord/darklordeyes" alpha
	tex0 = layer 0

effect
{
	texture tex0;

   technique t0
   {

   	pass p0
   	{
   		ZWriteEnable = True;
         AlphaBlendEnable = True;
         SrcBlend  = SrcAlpha;
   		AlphaTestEnable = False;
         DestBlend = One;
         BlendOp = Add;
         AlphaTestEnable = False;
         AlphaRef = 127;
         AlphaFunc = Greater;

   		AddressU[0] = Wrap;
   		AddressV[0] = Wrap;
   		
   		Texture[0] = <tex0>;

         ColorArg1[0] = Texture;
         ColorArg2[0] = Texture;
         AlphaArg1[0] = Texture;
         ColorOp[0] = Add;
         AlphaOp[0] = SelectArg1;
         TexCoordIndex[0] = 0;

         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
}
}
*/

/*

//shader "obj/creatures/darklord/darklord"
effect
{
   texture tex0;
	
   technique t0
   {
   	//1st pass, render only depth
   	pass p0
   	{
   		ZWriteEnable = True;
			ColorWriteEnable = 0;
      	
         ColorArg1[0] = Current;
         ColorOp[0] = SelectArg1;

         ColorOp[1] = Disable;
         AlphaOp[0] = Disable;
   	}

   	//2nd pass, render colors to the same depth
   	pass p1
   	{
   		ZWriteEnable = False;
         AlphaBlendEnable = True;
   		AlphaTestEnable = False;
         SrcBlend  = SrcAlpha;
         DestBlend = InvSrcAlpha;
         BlendOp = Add;
			ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
      	Texture[0] = <tex0>;
	      TexCoordIndex[0] = 0;
         AlphaArg1[0] = Texture;
         AlphaOp[0] = SelectArg1;
         ColorArg1[0] = Texture;
         ColorArg2[0] = Current;
         ColorOp[0] = Modulate; // Modulate2x;

         ColorOp[1] = Disable;
         AlphaOp[1] = Disable;
   	}
   }
}
*/

/*

shader "obj/lamp/redlight"
{
   cull none

	layer "obj/lamp/redlight"

	tex0 = layer 0
	effect "nolightmap_fullbright"
	

}


shader "obj/k4l4/vaza"
{	
   layer "obj/k4l4/vaza"
   layer "textures/env/keram"

   tex0 = layer 0
	tenv = layer 1
	effect "nolightmap_env"
}


shader "obj/lamp/lampa4_mrizka"
{
	sort 1
	surfparm nocastshadow
	layer "obj/lamp/lampa4_mrizka" alpha
	tex0 = layer 0
	effect "nolightmap_clip"
}

*/

/*
shader "Gfx/hud/compass"
{
	sort 30
	emap_globaltexture
	
	layer "Gfx/hud/compass" alpha
	layer "Gfx/hud/compass_arrow" alpha
	layer "Gfx/hud/compass_quest1" alpha
	layer "Gfx/hud/compass_quest2" alpha
	layer "Gfx/hud/compass_quest3" alpha
	layer "Gfx/hud/compass_quest4" alpha
	layer "Gfx/hud/compass_quest5" alpha
	layer "Gfx/hud/compass_quest0" alpha	//Cil od demona
	
	pass 0
		tcmod rotate 3.14 0.5 0.5
	pass 1
		tcmod rotate g_fCompBack 0.5 0.5
	pass 2
		tcmod rotate g_fQuest0 0.5 0.5
	pass 3
		tcmod rotate g_fQuest1 0.5 0.5
	pass 4
		tcmod rotate g_fQuest2 0.5 0.5
	pass 5
		tcmod rotate g_fQuest3 0.5 0.5
	pass 6
		tcmod rotate g_fQuest4 0.5 0.5
	pass 7
		tcmod rotate g_fQuest5 0.5 0.5
		
   tex0 = layer 0
   tex1 = layer 1
   tex2 = layer 2
   tex3 = layer 3
   tex4 = layer 4
   tex5 = layer 5
   tex6 = layer 6
   tex7 = layer 7
   
   alph0 = int g_QuestAlpha0
   alph1 = int g_QuestAlpha1
   alph2 = int g_QuestAlpha2
   alph3 = int g_QuestAlpha3
   alph4 = int g_QuestAlpha4
   alph5 = int g_QuestAlpha5

	effect
	{
	   texture tex0;
	   texture tex1;
	   texture tex2;
	   texture tex3;
	   texture tex4;
	   texture tex5;
	   texture tex6;
	   texture tex7;
	   
	   dword alph0;
	   dword alph1;
	   dword alph2;
	   dword alph3;
	   dword alph4;
	   dword alph5;
	   
	   technique l0
	   {
	   	pass p0
	   	{
	   		ZWriteEnable = False;
	         AlphaBlendEnable = True;
	         SrcBlend  = SrcAlpha;
	         DestBlend = InvSrcAlpha;
	         BlendOp = Add;

      		AddressU[0] = Clamp;
      		AddressV[0] = Clamp;

	   		AlphaTestEnable = True;
				AlphaFunc = Greater;
				AlphaRef = 0;
	         
	   		Texture[0] = <tex0>;
		      TexCoordIndex[0] = 0;
		      TexCoordIndex[1] = 1;
				
	         ColorArg1[0] = Texture;
	         AlphaArg1[0] = Texture;
	         ColorOp[0] = SelectArg1;
	         AlphaOp[0] = SelectArg1;

	         ColorOp[1] = Disable;
	         AlphaOp[1] = Disable;
	   	}
	   	pass p1
	   	{
	   		Texture[0] = <tex1>;
	   		AlphaRef = 0;
	   	}
	   	pass p2
	   	{
	   		Texture[0] = <tex2>;
	   		AlphaRef = <alph0>;
	   	}
	   	pass p3
	   	{
	   		Texture[0] = <tex3>;
	   		AlphaRef = <alph1>;
	   	}
	   	pass p4
	   	{
	   		Texture[0] = <tex4>;
	   		AlphaRef = <alph2>;
	   	}
	   	pass p5
	   	{
	   		Texture[0] = <tex5>;
	   		AlphaRef = <alph3>;
	   	}
	   	pass p6
	   	{
	   		Texture[0] = <tex6>;
	   		AlphaRef = <alph4>;
	   	}
	   	pass p7
	   	{
	   		Texture[0] = <tex7>;
	   		AlphaRef = <alph5>;
	   	}
	   }
	}
}
*/

/*
shader "OBJ/CREATURES/dcera/dcera_head"
{
	castshadow
	cull ccw

	layer "OBJ/CREATURES/dcera/dcera_head"
		
	tex0 = layer 0
	tfac0 = int g_TransColor
	
	effect
	{
		texture tex0;
	   DWORD tfac0;  
		
	   technique t0
	   {
	   	pass p0
	   	{
	   		ZWriteEnable = True;
	         AlphaBlendEnable = true;
	         SrcBlend  = SrcAlpha;
	   		AlphaTestEnable = False;
	         DestBlend = InvSrcAlpha;
	         BlendOp = Add;
	         TextureFactor = <tfac0>;

      		AddressU[0] = Clamp;
      		AddressV[0] = Clamp;
            TexCoordIndex[0] = 0;	
      		Texture[0] = <tex0>;
            AlphaArg1[0] = TFactor;	
				ColorArg1[0] = Texture;
         	ColorArg2[0] = Diffuse;
         	ColorOp[0] = Modulate2x;
         	AlphaOp[0] = SelectArg1;

            ColorOp[1] = Disable;
            AlphaOp[1] = Disable;
	   	}
	   }
	}
}

shader "OBJ/CREATURES/dcera/dcera_hair"
{
	castshadow
	cull ccw

	layer "OBJ/CREATURES/dcera/dcera_hair"
		
	tex0 = layer 0
	tfac0 = int g_TransColor
	
	effect
	{
		texture tex0;
	   DWORD tfac0;  
		
	   technique t0
	   {
	   	pass p0
	   	{
	   		ZWriteEnable = True;
	         AlphaBlendEnable = true;
	         SrcBlend  = SrcAlpha;
	   		AlphaTestEnable = False;
	         DestBlend = InvSrcAlpha;
	         BlendOp = Add;
	         TextureFactor = <tfac0>;

      		AddressU[0] = Clamp;
      		AddressV[0] = Clamp;
            TexCoordIndex[0] = 0;	
      		Texture[0] = <tex0>;
            AlphaArg1[0] = TFactor;	
				ColorArg1[0] = Texture;
         	ColorArg2[0] = Diffuse;
         	ColorOp[0] = Modulate2x;
         	AlphaOp[0] = SelectArg1;

            ColorOp[1] = Disable;
            AlphaOp[1] = Disable;
	   	}
	   }
	}
}

shader "OBJ/CREATURES/dcera/dcera_body"
{
	castshadow
	cull ccw

	layer "OBJ/CREATURES/dcera/dcera_body"
		
	tex0 = layer 0
	tfac0 = int g_TransColor
	
	effect
	{
		texture tex0;
	   DWORD tfac0;  
		
	   technique t0
	   {
	   	pass p0
	   	{
	   		ZWriteEnable = True;
	         AlphaBlendEnable = true;
	         SrcBlend  = SrcAlpha;
	   		AlphaTestEnable = False;
	         DestBlend = InvSrcAlpha;
	         BlendOp = Add;
	         TextureFactor = <tfac0>;

      		AddressU[0] = Clamp;
      		AddressV[0] = Clamp;
            TexCoordIndex[0] = 0;	
      		Texture[0] = <tex0>;
            AlphaArg1[0] = TFactor;	
				ColorArg1[0] = Texture;
         	ColorArg2[0] = Diffuse;
         	ColorOp[0] = Modulate2x;
         	AlphaOp[0] = SelectArg1;

            ColorOp[1] = Disable;
            AlphaOp[1] = Disable;
	   	}
	   }
	}
}
*/


//oltar knezky v k2l2a
shader "textures/special/oltar1_1"
{
	cull ccw

	layer "textures/special/oltar1_1" alpha
	layer "textures/special/oltar1_2" alpha
	layer $lightmap
	
	tex0 = layer 0
	tex1 = layer 1
	tex2 = layer 2
	
	tfac0 = _eclass TFactor
	tfac1 = func_wall_morph TexFactor2
	
	effect "effect_morph_lightmap"
}


/*
shader "textures/special/gates_teleport"
{
	layer "textures/special/gates_teleport"
	layer $lightmap
	layer "textures/special/gates_teleport_glow" alpha
  
   tex0 = layer 0
   tex1 = layer 1
   tex2 = layer 2
	tfac = int g_iShaderTFactor

   effect 
	{
	   texture tex0;
	   texture tex1;
	   texture tex2;
	   DWORD tfac;
	
		technique e4
	   {
	   	pass p0
	   	{
	   		ZWriteEnable = True;
	         AlphaBlendEnable = False;
	   		AlphaTestEnable = False;
	
	         TextureTransformFlags[0] = Disable;
				AddressU[0] = Wrap;
				AddressV[0] = Wrap;
				AddressU[1] = Clamp;
				AddressV[1] = Clamp;
	
	   		Texture[0] = <tex0>;
	         Texture[1] = <tex1>;
		      TexCoordIndex[0] = 0;
		      TexCoordIndex[1] = 1;
	           
	         ColorArg1[0] = Texture;
	         ColorOp[0] = SelectArg1;
	           
	         ColorArg1[1] = Texture;
	         ColorArg2[1] = Current;
	         ColorOp[1] = Modulate2x;
	          
	         ColorOp[2] = Disable;
	         AlphaOp[0] = Disable;
	   	}
	
	   	pass p1
	   	{
	   		ZWriteEnable = False;
	         AlphaBlendEnable = True;
	   		AlphaTestEnable = False;
	         SrcBlend  = SrcAlpha;
	         DestBlend = One;
	         BlendOp = Add;
	         
	   		AddressU[0] = Wrap;
	   		AddressV[0] = Wrap;
	   		
	   		Texture[0] = <tex2>;
	         TextureFactor = <tfac>;
	           
	         ColorArg1[0] = Texture;
	            ColorArg1[0] = Texture;
	            AlphaArg1[0] = Texture;
	            AlphaArg2[0] = TFactor;
	            ColorOp[0] = SelectArg1;
	            AlphaOp[0] = Modulate2x;
	            TexCoordIndex[0] = 0;
	           
	         ColorOp[1] = Disable;
	         AlphaOp[1] = Disable;
	   	}
	   }
	}
}
*/

// healthbar
/*
shader "spr/misc/enemy_healthbar"
{
	sort 2
	cull none
	
	layer "Spr/misc/enemy_healthbar" alpha
	
	tex0 = layer 0
	tfac = g_iHealthFactor
	
effect
{
	texture tex0;
	dword tfac;
	
   technique t0
   {
      	pass p0
      	{
            AlphaBlendEnable = True;
      		AlphaTestEnable = True;
            SrcBlend  = SrcAlpha;
            DestBlend = InvSrcAlpha;
            BlendOp = Add;
            AlphaFunc = Greater;
			   AlphaRef = <tfac>;
			   
      		AddressU[0] = Clamp;
      		AddressV[0] = Clamp;
      		
      		MinFilter[0] = Point;
      		MagFilter[0] = Point;
      		
      		Texture[0] = <tex0>;
            ColorArg1[0] = Texture;
            AlphaArg1[0] = Texture;
            ColorOp[0] = SelectArg1;
            AlphaOp[0] = SelectArg1;
            TexCoordIndex[0] = 0;

            ColorOp[1] = Disable;
            AlphaOp[1] = Disable;
      	}
      	
      	pass _nodraw
      	{
      		MinFilter[0] = Linear;
      		MagFilter[0] = Linear;
      	}
    }
}
}
*/


/*
shader "textures/window/vit_tem"
{
   //surfparm alphashadow
   surfparm glass
   //emap_surfacelight 450

   layer "textures/window/vit_tem"
   layer $lightmap

   tex0 = layer 0
   tex1 = layer 1
   effect "lightmapped"
}
*/
/*
shader "textures/window/vit_pan"
{
   //surfparm alphashadow
   surfparm glass
   //emap_surfacelight 550

   layer "textures/window/vit_pan" alpha
   layer "textures/env/vitraglass"
   layer $lightmap

   tex0 = layer 0
   tex1 = layer 1
   tex2 = layer 2
   effect "lightmapped_window"
}
*/


// 5 shaderu pro zmenu obrazu v k2l2b
shader "textures/window/obraz1"
{
	cull ccw
   qer_image textures/window/obraz1

	layer "textures/window/obraz_ram"
	layer "textures/window/obraz1"
	layer $lightmap
	
	tex0 = layer 0
	tex1 = layer 1
	tex2 = layer 2

	tfac0 = _eclass TFactor
	tfac1 = func_wall_morph TexFactor2
	
	effect "effect_morph_lightmap2"
}
/*
shader "textures/window/obraz2"
{
	cull ccw
	qer_image textures/window/obraz2

	layer "textures/window/obraz_ram"
	layer "textures/window/obraz2"
	layer $lightmap
	
	tex0 = layer 0
	tex1 = layer 1
	tex2 = layer 2

	tfac0 = int g_MorphColor0
	tfac1 = int g_MorphColor1

	effect "effect_morph_lightmap"
}

shader "textures/window/obraz3"
{
	cull ccw
	qer_image textures/window/obraz3

	layer "textures/window/obraz_ram"
	layer "textures/window/obraz3"
	layer $lightmap
	
	tex0 = layer 0
	tex1 = layer 1
	tex2 = layer 2

	tfac0 = int g_MorphColor0
	tfac1 = int g_MorphColor1
	
	effect "effect_morph_lightmap"
}

shader "textures/window/obraz4"
{
	cull ccw
	qer_image textures/window/obraz4

	layer "textures/window/obraz_ram"
	layer "textures/window/obraz4"
	layer $lightmap
	
	tex0 = layer 0
	tex1 = layer 1
	tex2 = layer 2

	tfac0 = int g_MorphColor0
	tfac1 = int g_MorphColor1
	
	effect "effect_morph_lightmap"
}

shader "textures/window/obraz5"
{
	cull ccw
	qer_image textures/window/obraz5

	layer "textures/window/obraz_ram"
	layer "textures/window/obraz5"
	layer $lightmap
	
	tex0 = layer 0
	tex1 = layer 1
	tex2 = layer 2

	tfac0 = int g_MorphColor0
	tfac1 = int g_MorphColor1
	
	effect "effect_morph_lightmap"
}
*/

