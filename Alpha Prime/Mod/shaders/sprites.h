
// FIRE ----------------------------------
shader "particle/animfire_mask"
{
	sort translucent
	layer "textures/efx/fire" alpha
	tex0 = layer 0
   effect "effect_particlemask"
}
shader "particle/animfire"
{
	sort translucent
	layer "textures/efx/fire"
	tex0 = layer 0
   effect "effect_particle"
}


// EXPLOSION ----------------------------------
shader "particle/animexplosion_mask"
{
	sort translucent
	layer "textures/efx/fire_billowing" alpha
	tex0 = layer 0
   effect "effect_particlemask"
}
shader "particle/animexplosion"
{
	sort translucent
	layer "textures/efx/fire_billowing"
	tex0 = layer 0
   effect "effect_particle"
}


// SMOKE ----------------------------------
shader "particle/animsmoke"
{
	sort translucent
	layer "textures/efx/fire" alpha
	tex0 = layer 0
   effect "effect_particlemask"   
}

// OTHER ---------------------------------------
shader "particle/glow/angelflare"
{
	sort translucent
//   cull none

	layer "particle/sprites/angelflare" alpha
		
	tex0 = layer 0
   effect "effect_particle"
   
}

shader "particle/terraindust"
{
	sort translucent

	layer "particle/sprites/dust" alpha

	tex0 = layer 0
	effect "effect_particlemask"
}


shader "particle/glow/angelflarenb"
{
	sort translucent
//   cull none

	layer "particle/sprites/angelflare" alpha
		
	tex0 = layer 0
   effect "effect_particle_nobuff"
  
}

shader "particle/smoke/heavysmoke"
{
	sort translucent
//   cull none

	layer "particle/sprites/heavysmoke" alpha
		
	tex0 = layer 0
   effect "effect_particlemask"   
}

// new particles
shader "particle/flash/electric"
{
	sort translucent

	layer "sprites/flash/images/electric"

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/flame1"
{
	sort translucent

	layer "particle/sprites/flamesmall" alpha
		
	tex0 = layer 0
   effect "effect_particle"   
}

shader "particle/fire/flame1nb"
{
	sort translucent

	layer "particle/sprites/flamesmall" alpha
		
	tex0 = layer 0
   effect "effect_particle_nobuff"   
}

shader "particle/fire/fire1"
{
	sort translucent

	layer "particle/sprites/firesmall" alpha
	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/fire1nb"
{
	sort translucent

	layer "particle/sprites/firesmall" alpha
	tex0 = layer 0
   effect "effect_particle_nobuff"
}

shader "particle/fire/glow1"
{
	sort translucent

	layer "particle/sprites/glow" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/glow2"
{
	sort translucent

	layer "particle/sprites/glow2" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/sparks1"
{
	sort translucent

	layer "particle/sprites/spark" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/sparks1long"
{
	sort translucent

	layer "particle/sprites/sparklong" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/sparks1glow"
{
	sort translucent

	layer "particle/sprites/spark2" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/fire/sparkcloud"
{
	sort translucent

	layer "particle/sprites/sparkcloud" alpha

	tex0 = layer 0
   effect "effect_particle"
}


shader "particle/fire/sparkdensecloud"
{
	sort translucent

	layer "particle/sprites/sparkdensecloud" alpha

	tex0 = layer 0

   effect "effect_particlemaskclamp"
}


shader "particle/fire/sparks1nb"
{
	sort translucent

	layer "particle/sprites/spark" alpha

	tex0 = layer 0
   effect "effect_particle_nobuff"
}


shader "particle/fire/sparks2"
{
	sort translucent

	layer "particle/sprites/dot" alpha

	tex0 = layer 0
   effect "effect_particle"
}



shader "particle/fire/drips1"
{
	sort translucent

	layer "particle/sprites/drip" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/smoke/smoke1"
{
	sort translucent

	layer "particle/sprites/smoke" alpha

	tex0 = layer 0
   effect "effect_particlemask"
}


shader "particle/animtest"
{
	sort translucent

	layer "textures/efx/fireball01_mid" alpha

	tex0 = layer 0
   effect "effect_particlemask"
}

shader "particle/smoke/smoke1add"
{
	sort translucent

	layer "particle/sprites/smoke" alpha
		
	tex0 = layer 0
   effect "effect_particle" 
}

shader "particle/smoke/smoke2"
{
	sort translucent

	layer "particle/sprites/smoke2" alpha
		
	tex0 = layer 0
   effect "effect_particlemask"
   
}


shader "particle/smoke/smoke3"
{
	sort translucent

	layer "particle/sprites/smoke3" alpha
		
	tex0 = layer 0
   effect "effect_particlemask"
   
}

shader "particle/smoke/smoke2add"
{
	sort translucent

	layer "particle/sprites/smoke2" alpha
		
	tex0 = layer 0
   effect "effect_particle"
   
}

shader "particle/water/streamlong"
{
	sort translucent

	layer "particle/sprites/waterstreamlong" alpha

	tex0 = layer 0
   effect "effect_particlemask"
}

shader "particle/water/streamlong2"
{
	sort translucent

	layer "particle/sprites/waterstreamlong" alpha

	tex0 = layer 0
   effect "effect_particle"
}

shader "particle/misc/dot"
{
	sort translucent

	layer "particle/sprites/dot" alpha

	tex0 = layer 0
   effect "effect_particlemask"
}

shader "particle/misc/dotclamp"
{
	sort translucent

	layer "particle/sprites/dot" alpha

	tex0 = layer 0
   effect "effect_particlemaskclamp"
}


shader "particle/misc/debris1"
{
	sort translucent

	layer "particle/sprites/debris1" alpha

	tex0 = layer 0
   effect "effect_particlemaskclamp"
}


shader "particle/misc/blood1"
{
	sort translucent

	layer "particle/sprites/blood1" alpha

	tex0 = layer 0
   effect "effect_particlemask"
}

shader "particle/misc/blooddrop"
{
	sort translucent

	layer "particle/sprites/drop02" alpha

	tex0 = layer 0
   effect "effect_particlemodulate"
}

shader "particle/misc/liquidrop"
{
	sort translucent

	layer "particle/sprites/drop01" alpha

	tex0 = layer 0
   effect "effect_particleadd"
}

// *** Coronas, flares ***

shader "spr/effects/flares/lens0"
{
	sort translucent
   cull none
	
	layer "spr/effects/flares/lens0"
		
	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

shader "spr/effects/flares/lens1"
{
	sort translucent
   cull none
	
	layer "spr/effects/flares/lens1" alpha
		
	tex0 = layer 0
	tfac = Particle TFactor
   effect "effect_glow"
}


shader "sprites/corona/images/white"
{
	sort translucent
   cull none

	layer "sprites/corona/images/white" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

shader "sprites/corona/images/whitenb"
{
	sort translucent
   cull none

	layer "sprites/corona/images/white" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}


shader "sprites/corona/images/orange"
{
	sort translucent
   cull none

	layer "sprites/corona/images/orange" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

shader "sprites/corona/images/orangenb"
{
	sort translucent
   cull none

	layer "sprites/corona/images/orange" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}

shader "sprites/corona/images/green"
{
	sort translucent
   cull none

	layer "sprites/corona/images/green" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

shader "sprites/corona/images/red"
{
	sort translucent
   cull none

	layer "sprites/corona/images/red" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

shader "sprites/corona/images/redflare"
{
	sort translucent
   cull none

	layer "sprites/corona/images/redflare" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}


shader "sprites/corona/images/blue"
{
	sort translucent
   cull none

	layer "sprites/corona/images/blue" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

shader "sprites/corona/images/bluenb"
{
	sort translucent
   cull none

	layer "sprites/corona/images/blue" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}

shader "sprites/shine/images/whitesmall"
{
	sort translucent
   cull none

	layer "sprites/shine/images/whitesmall" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}

shader "sprites/shine/images/whitemed"
{
	sort translucent
   cull none

	layer "sprites/shine/images/whitemed" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}

shader "sprites/shine/images/whitemed2"
{
	sort translucent
   cull none

	layer "sprites/shine/images/white2med" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}

shader "sprites/shine/images/whitemed3"
{
	sort translucent
   cull none

	layer "sprites/shine/images/white3med" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}

shader "sprites/shine/images/orangesmall"
{
	sort translucent
   cull none

	layer "sprites/shine/images/orangesmall" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow_nobuff"
}



shader "spr/effects/glow/corona4"
{
	sort translucent
   cull none

	layer "spr/effects/glow/corona4" alpha

	tex0 = layer 0
	tfac = ExtClass TFactor
   effect "effect_glow"
}

//*** STREAKS ***
shader "sprites/streak/streak1"
{
	sort translucent
	zwrite false
	map nolight
	cull none
sort overlay


	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]
         // diffusemap2 $renderview clampu clampv
	//diffusemap3 "gfx/black" alpha nomipmap

	blend one one
	diffusemap "sprites/streak/streak1"
    
	material haze
}

shader "sprites/streak/streak2"
{
	sort translucent
	zwrite false
	map nolight
	cull none

	blend one one
	diffusemap "sprites/streak/streak2"
   
sort overlay


	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]
         // diffusemap2 $renderview clampu clampv
	//diffusemap3 "gfx/black" alpha nomipmap

	material haze
}

shader "sprites/streak/streak3"
{
	sort translucent
	zwrite false
	map nolight
	cull none

	blend one one
	diffusemap "sprites/streak/streak3"
       
sort overlay

	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]
         // diffusemap2 $renderview clampu clampv
	//diffusemap3 "gfx/black" alpha nomipmap

	material haze
}

shader "sprites/streak/streak4"
{
castshadow
	sort translucent
	zwrite false
	map nolight
	cull none

	blend one one
	diffusemap "sprites/streak/streak4"
       
sort overlay


	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]
         // diffusemap2 $renderview clampu clampv
	//diffusemap3 "gfx/black" alpha nomipmap

	material haze
}


// streak tests below

shader "obj/weapons/streak"
{
   cull none
	sort translucent
	
	layer "obj/weapons/streak" alpha

	tex0 = layer 0
	effect "nolightmap_trans_fullbright"
        

sort overlay
	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]
         // diffusemap2 $renderview clampu clampv
	//diffusemap3 "gfx/black" alpha nomipmap


	material haze
}

shader "spr/shotstreak"
{
	sort translucent
	zwrite false
	map nolight
	cull none
	blend one srcalpha
	diffusemap "obj/weapons/streak2" alpha	//"obj/weapons/streakshot"
      
sort overlay


	parm0 (DynamicOverlaySprite row0) //float[4]
	parm1 (DynamicOverlaySprite row1) //float[4]
         // diffusemap2 $renderview clampu clampv
	//diffusemap3 "gfx/black" alpha nomipmap

	material haze
}

shader "spr/uv_debug"
{
	sort translucent
	zwrite true
	map nolight
	cull none
	diffusemap "textures/debug_UV"
}

shader "spr/debug_glass"
{
   cull none
	sort translucent
	layer "textures/debug_glass" alpha

	tex0 = layer 0
	effect "nolightmap_trans"
}
