//typ striel pre partikly
const int PE_PLAYERBULLET = 0 //player ma specialni efekty bez zbufferu, ktery potvory nesmi pouzivat!!!
const int PE_PLAYERPLASMA = 1
const int PE_BULLET = 2
const int PE_PLASMA = 3
const int PE_COUNT = 4		//celkovy pocet efektu. Pri pridani dalsich je nutno zvysit podle potreby

//typ striel pre decaly
const int DE_BULLET = 0
const int DE_PLASMA = 1
const int DE_COUNT = 2		//celkovy pocet decalu. Pri pridani dalsich je nutno zvysit podle potreby

//typ striel pre ric zvuky
const int SE_BULLET = 0
const int SE_PLASMA = 1
const int SE_HAMMER = 2
const int SE_COUNT = 3		//celkovy pocet zvuku. Pri pridani dalsich je nutno zvysit podle potreby

vobject	RicParticles[PE_COUNT][12]
hshader	DecalShaders[DE_COUNT][12]
hsound	RicSounds[SE_COUNT][12]
hshader BloodDecalShaders[4]

//Tomuto poli odpovida ShellType u zbrane. ShellType se indexuje od 1 (0 znamena zadny shell)
//Podle potreby mozno zvetsovat
const string ShellObjectNames[1] = {
	"obj/weapons/shell.xob"}

//Tomuto poli odpovida StreakType u zbrane. Indexuje se od 1, (0 znamena zadny streak)
const string StreakShaderNames[5] = {
	"sprites/streak/streak1",
	"sprites/streak/streak2",
	"sprites/streak/streak3",
	"sprites/streak/streak4",
	"spr/shotstreak"}

//---------------------------------------------------------------------------------
void InitRicParticles(int PShotType)
{
	if(RicParticles[PShotType][0])	//uz to bolo inicializovane. default tam je vzdy
		return

	switch(PShotType)
	{
	case PE_BULLET:
		RicParticles[PE_BULLET][0] = GetObject("particle/ric_plastic.ptc")
		RicParticles[PE_BULLET][MATERIAL_METAL] = GetObject("particle/ric_metal.ptc")
		RicParticles[PE_BULLET][MATERIAL_IRON] = GetObject("particle/ric_iron.ptc")
//		RicParticles[PE_BULLET][MATERIAL_GLASS] = GetObject("particle/ric_plastic.ptc")
		RicParticles[PE_BULLET][MATERIAL_PLASTIC] = GetObject("")
//		RicParticles[PE_BULLET][MATERIAL_LIQUID] = GetObject("")
//		RicParticles[PE_BULLET][MATERIAL_SLIME] = GetObject("particle/physx/fluidtest.ptc")
		RicParticles[PE_BULLET][MATERIAL_BETON] = GetObject("particle/ric_beton.ptc")
		RicParticles[PE_BULLET][MATERIAL_RUBBER] = GetObject("particle/ric_rubber.ptc")
		RicParticles[PE_BULLET][MATERIAL_FLESH] = GetObject("particle/hit_blood.ptc")
                
		RicParticles[PE_BULLET][MATERIAL_GRAVEL] = GetObject("particle/ric_gravel.ptc")
		break

	case PE_PLAYERBULLET:
		RicParticles[PE_PLAYERBULLET][0] = GetObject("particle/ric_plastic.ptc")
		RicParticles[PE_PLAYERBULLET][MATERIAL_METAL] = GetObject("particle/ric_metal.ptc")
                RicParticles[PE_PLAYERBULLET][MATERIAL_IRON] = GetObject("particle/ric_iron.ptc")
//		RicParticles[PE_PLAYERBULLET][MATERIAL_GLASS] = GetObject("")
		RicParticles[PE_PLAYERBULLET][MATERIAL_PLASTIC] = GetObject("particle/ric_plastic.ptc")
//		RicParticles[PE_PLAYERBULLET][MATERIAL_LIQUID] = GetObject("")
//		RicParticles[PE_PLAYERBULLET][MATERIAL_SLIME] = GetObject("particle/physx/fluidtest.ptc")
		RicParticles[PE_PLAYERBULLET][MATERIAL_BETON] = GetObject("particle/ric_beton.ptc")
		RicParticles[PE_PLAYERBULLET][MATERIAL_RUBBER] = GetObject("particle/ric_rubber.ptc")
		RicParticles[PE_PLAYERBULLET][MATERIAL_FLESH] = GetObject("particle/hit_blood.ptc")
               
		RicParticles[PE_PLAYERBULLET][MATERIAL_GRAVEL] = GetObject("particle/ric_gravel.ptc")
		break

	case PE_PLAYERPLASMA:
		RicParticles[PE_PLAYERPLASMA][0] = GetObject("particle/ric_plastic.ptc")
		RicParticles[PE_PLAYERPLASMA][MATERIAL_METAL] = GetObject("particle/ric_metal.ptc")
                RicParticles[PE_PLAYERPLASMA][MATERIAL_IRON] = GetObject("particle/ric_iron.ptc")
//		RicParticles[PE_PLAYERPLASMA][MATERIAL_GLASS] = GetObject("")
		RicParticles[PE_PLAYERPLASMA][MATERIAL_PLASTIC] = GetObject("particle/ric_plastic.ptc")
//		RicParticles[PE_PLAYERPLASMA][MATERIAL_LIQUID] = GetObject("")
//		RicParticles[PE_PLAYERPLASMA][MATERIAL_SLIME] = GetObject("particle/physx/fluidtest.ptc")
		RicParticles[PE_PLAYERPLASMA][MATERIAL_BETON] = GetObject("particle/ric_beton.ptc")
		RicParticles[PE_PLAYERPLASMA][MATERIAL_RUBBER] = GetObject("particle/ric_rubber.ptc")
		RicParticles[PE_PLAYERPLASMA][MATERIAL_FLESH] = GetObject("particle/hit_blood.ptc")
                
//		RicParticles[PE_PLAYERPLASMA][MATERIAL_STONE] = GetObject("")

     case PE_PLASMA:

		RicParticles[PE_PLASMA][0] = GetObject("particle/ric_plastic.ptc")
		RicParticles[PE_PLASMA][MATERIAL_METAL] = GetObject("particle/ric_metal.ptc")
		RicParticles[PE_PLASMA][MATERIAL_IRON] = GetObject("particle/ric_iron.ptc")
//		RicParticles[PE_PLASMA][MATERIAL_GLASS] = GetObject("")
		RicParticles[PE_PLASMA][MATERIAL_PLASTIC] = GetObject("particle/ric_plastic.ptc")
//		RicParticles[PE_PLASMA][MATERIAL_LIQUID] = GetObject("")
//		RicParticles[PE_PLASMA][MATERIAL_SLIME] = GetObject("particle/physx/fluidtest.ptc")
		RicParticles[PE_PLASMA][MATERIAL_BETON] = GetObject("particle/ric_beton.ptc")
		RicParticles[PE_PLASMA][MATERIAL_RUBBER] = GetObject("particle/ric_rubber.ptc")
		RicParticles[PE_PLASMA][MATERIAL_FLESH] = GetObject("particle/hit_blood.ptc")
                
//		RicParticles[PE_PLASMA][MATERIAL_STONE] = GetObject("")
		break
	}
}

//---------------------------------------------------------------------------------
void InitDecalDefs()
{
	BloodDecalShaders[0] = GetShader("decals/blood1")
	BloodDecalShaders[1] = GetShader("decals/blood2")
	BloodDecalShaders[2] = GetShader("decals/blood3")
	BloodDecalShaders[3] = GetShader("decals/blood4")
	
	DecalShaders[DE_BULLET][0] = GetShader("decals/decaliron")
	DecalShaders[DE_BULLET][MATERIAL_METAL] = GetShader("decals/decalmetal")
	DecalShaders[DE_BULLET][MATERIAL_IRON] = GetShader("decals/decaliron")
	DecalShaders[DE_BULLET][MATERIAL_GLASS] = GetShader("decals/decalglass")
	DecalShaders[DE_BULLET][MATERIAL_PLASTIC] = GetShader("decals/decalplastic")
	DecalShaders[DE_BULLET][MATERIAL_LIQUID] = GetShader("decals/decalliquid")
	DecalShaders[DE_BULLET][MATERIAL_SLIME] = GetShader("decals/decalslime")
	DecalShaders[DE_BULLET][MATERIAL_BETON] = GetShader("decals/decalbeton")
	DecalShaders[DE_BULLET][MATERIAL_RUBBER] = GetShader("decals/decalrubber")
	DecalShaders[DE_BULLET][MATERIAL_FLESH] = GetShader("decals/decalflesh")
	DecalShaders[DE_BULLET][MATERIAL_STONE] = GetShader("decals/decalstone")

	DecalShaders[DE_PLASMA][0] = GetShader("decals/decaliron")
	DecalShaders[DE_PLASMA][MATERIAL_METAL] = GetShader("decals/decalmetal")
	DecalShaders[DE_PLASMA][MATERIAL_IRON] = GetShader("decals/decaliron")
	DecalShaders[DE_PLASMA][MATERIAL_GLASS] = GetShader("decals/decalglass")
	DecalShaders[DE_PLASMA][MATERIAL_PLASTIC] = GetShader("decals/decalplastic")
	DecalShaders[DE_PLASMA][MATERIAL_LIQUID] = GetShader("decals/decalliquid")
	DecalShaders[DE_PLASMA][MATERIAL_SLIME] = GetShader("decals/decalslime")
	DecalShaders[DE_PLASMA][MATERIAL_BETON] = GetShader("decals/decalbeton")
	DecalShaders[DE_PLASMA][MATERIAL_RUBBER] = GetShader("decals/decalrubber")
	DecalShaders[DE_PLASMA][MATERIAL_FLESH] = GetShader("decals/decalflesh")
	DecalShaders[DE_PLASMA][MATERIAL_STONE] = GetShader("decals/decalstone")
	
}

//---------------------------------------------------------------------------------
void InitRicSounds(int RSndType)
{
	if(RSndType == -1 || RicSounds[RSndType][0])
		return

	switch(RSndType)
	{
	case SE_BULLET:
		RicSounds[SE_BULLET][0] = GetSoundEx("Rics_Metal", 0, 0)
		RicSounds[SE_BULLET][MATERIAL_METAL] = GetSoundEx("Rics_Metal", 0, 0)
		RicSounds[SE_BULLET][MATERIAL_IRON] = GetSoundEx("Rics_Metal", 0, 0)
		RicSounds[SE_BULLET][MATERIAL_GLASS] = GetSoundEx("Rics_Metal", 0, 0)
//		RicSounds[SE_BULLET][MATERIAL_PLASTIC] = GetSoundEx("", 0, 0)
//		RicSounds[SE_BULLET][MATERIAL_LIQUID] = GetSoundEx("", 0, 0)
//		RicSounds[SE_BULLET][MATERIAL_SLIME] = GetSoundEx("", 0, 0)
//		RicSounds[SE_BULLET][MATERIAL_BETON] = GetSoundEx("", 0, 0)
//		RicSounds[SE_BULLET][MATERIAL_RUBBER] = GetSoundEx("", 0, 0)
		RicSounds[SE_BULLET][MATERIAL_FLESH] = GetSoundEx("Rics_Flesh", 0, 0)
//		RicSounds[SE_BULLET][MATERIAL_STONE] = GetSoundEx("", 0, 0)
		RicSounds[SE_BULLET][MATERIAL_GRAVEL] = GetSoundEx("Rics_PISEK", 0, 0)
		break

	case SE_PLASMA:
		RicSounds[SE_PLASMA][0] = GetSoundEx("Rics_Metal", 0, 0)
		RicSounds[SE_PLASMA][MATERIAL_METAL] = GetSoundEx("Rics_Metal", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_IRON] = GetSoundEx("", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_GLASS] = GetSoundEx("", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_PLASTIC] = GetSoundEx("", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_LIQUID] = GetSoundEx("", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_SLIME] = GetSoundEx("", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_BETON] = GetSoundEx("", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_RUBBER] = GetSoundEx("", 0, 0)
		RicSounds[SE_PLASMA][MATERIAL_FLESH] = GetSoundEx("Rics_Flesh", 0, 0)
//		RicSounds[SE_PLASMA][MATERIAL_STONE] = GetSoundEx("", 0, 0)
		RicSounds[SE_PLASMA][MATERIAL_GRAVEL] = GetSoundEx("Rics_PISEK", 0, 0)
		break

	case SE_HAMMER:
		RicSounds[SE_HAMMER][0] = GetSoundEx("Rics_MetalHammer", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_METAL] = GetSoundEx("Rics_MetalHammer", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_IRON] = GetSoundEx("Rics_MetalHammer", 0, 0)
//		RicSounds[SE_HAMMER][MATERIAL_GLASS] = GetSoundEx("", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_PLASTIC] = GetSoundEx("Rics_BetonHammer", 0, 0)
//		RicSounds[SE_HAMMER][MATERIAL_LIQUID] = GetSoundEx("", 0, 0)
//		RicSounds[SE_HAMMER][MATERIAL_SLIME] = GetSoundEx("", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_BETON] = GetSoundEx("Rics_BetonHammer", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_RUBBER] = GetSoundEx("Rics_BetonHammer", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_FLESH] = GetSoundEx("Rics_Flesh", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_STONE] = GetSoundEx("Rics_Gravel", 0, 0)
		RicSounds[SE_HAMMER][MATERIAL_GRAVEL] = GetSoundEx("Rics_PISEK", 0, 0)
		break
	}
}

