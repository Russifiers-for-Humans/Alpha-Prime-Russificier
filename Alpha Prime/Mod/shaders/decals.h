shader "decals/blood"
{
	sort decal
	zwrite false

	map noambient

	blend srccolor zero
	diffusemap "textures/decals/blood1" alpha

	material GenericVertexlit
}

shader "decals/blood1"
{
	sort decal
	zwrite false

	map noambient
	specular $alpha
	
	blend srccolor zero
	diffusemap "textures/decals/blood1" alpha

	material GenericVertexlit
}

shader "decals/blood2"
{
	sort decal
	zwrite false

	map noambient
	specular $alpha

	blend srccolor zero
	diffusemap "textures/decals/blood2" alpha

	material GenericVertexlit
}

shader "decals/blood3"
{
	sort decal
	zwrite false

	map noambient
	specular $alpha

	blend srccolor zero
	diffusemap "textures/decals/blood3" alpha

	material GenericVertexlit
}

shader "decals/blood4"
{
	sort decal
	zwrite false

	map noambient
	specular $alpha

	blend srccolor zero
	diffusemap "textures/decals/blood3" alpha
	//diffusemap "textures/decals/blood4" alpha

	material GenericVertexlit
}


//--------------------------------
shader "decals/decalmetal"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.23

   blend srccolor zero
   map noambient
   alphatest 1

	diffusemap "textures/decals/hit_metal_c" clamp alpha
	normalmap "textures/decals/hit_metal_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decaliron"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.23

   blend srccolor zero
   map noambient
   alphatest 1

	diffusemap "textures/decals/hit_iron_c" clamp alpha
	normalmap "textures/decals/hit_iron_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decalbeton"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.23

   blend srccolor zero
   map noambient
   alphatest 1

	diffusemap "textures/decals/hit_beton_c" clamp alpha
	normalmap "textures/decals/hit_beton_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decalrubber"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.22

   blend srccolor zero
   map noambient
   alphatest 1

	diffusemap "textures/decals/hit_rubber_c" clamp alpha
	normalmap "textures/decals/hit_rubber_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decalplastic"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.22

   blend srccolor zero
   map noambient
   alphatest 1

	diffusemap "textures/decals/hit_plastic_c" clamp alpha
	normalmap "textures/decals/hit_plastic_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decalflesh"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.233

   blend srccolor zero
   map noambient
   alphatest 1

	diffusemap "textures/decals/hit_flesh_c" clamp alpha
	normalmap "textures/decals/hit_flesh_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decalglass"
{
	sort decal
	zwrite false
	cull none

	specular 0 //$alpha
	specularpower 0.522

	blend one srcalpha
	map noambient

	diffusemap "textures/decals/hit_glass_c" clamp alpha

	material GenericVertexlit
}

// todo

shader "decals/decalliquid"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.503

   blend invsrcalpha srcalpha

	diffusemap "textures/decals/steel1_c" clamp alpha
	normalmap "textures/decals/steel1_n" clamp alpha

	material GenericVertexlit
}

shader "decals/decalslime"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.512

   blend invsrcalpha srcalpha

	diffusemap "textures/decals/steel1_c" clamp alpha
	normalmap "textures/decals/steel1_n" clamp alpha

	material GenericVertexlit
}



shader "decals/decalstone"
{
	sort decal
	zwrite false

	specular $nmapalpha
	specularpower 0.51

	blend srccolor zero
	map noambient
	alphatest 1

	diffusemap "textures/decals/hit_stone_c" clamp alpha
	normalmap "textures/decals/hit_stone_n" clamp alpha

	material GenericVertexlit
}
/*
//---------------------
shader "decals/test"
{
	sort decal
	zwrite false
	//alphatest 1

	specular $nmapalpha
	specularpower 0.52

	//map nolight
	
	//blend invsrcalpha srcalpha
	//diffusemap "decals/burn" alpha

	//blend srccolor zero
	//diffusemap "textures/decals/scorch" clampu clampv

   blend invsrcalpha srcalpha

	diffusemap "textures/decals/hit_iron_c" clamp alpha
	normalmap "textures/decals/hit_iron_n" clamp alpha
//	diffusemap "textures/decals/test00_c" clamp alpha
//	normalmap "textures/decals/test00_n" clamp alpha

	material GenericVertexlit
}
*/

shader "decals/burn"
{
	sort decal
	zwrite false
	//alphatest 1

	map nolight
	
	//blend invsrcalpha srcalpha
	//diffusemap "decals/burn" alpha

	blend srccolor zero
	diffusemap "textures/decals/scorch" clamp alpha

	material GenericVertexlit
}

