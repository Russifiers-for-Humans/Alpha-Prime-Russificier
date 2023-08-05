
string WS_WeaponHammer[5] = {"","Hammer_miss01","", "", ""}				//hammer
string WS_WeaponGun[5] = {"pistol_shot01","","noammo", "", ""}				//pistol
string WS_WeaponShotgun[5] = {"ShotGun_01","","noammo", "shotgun_reload02", ""}	//shotgun
string WS_WeaponMachinegun[5] = {"gatlingLE_loop","","noammo", "gatlingLE_end", ""}	//samopal
string WS_WeaponSniperRifle[5] = {"Sniper01","","noammo", "SniperServo01", ""}			//Sniperka
string WS_WeaponRocketLauncher[5] = {"RocketLauncher_fire01","","", "", ""}
string WS_WeaponFlameThrower[5] = {"flameThrower_loop01","","noammo", "flameThrower_end01", ""}
string WS_WeaponGrenade[5] = {"","","", "", ""}			//Granat
string WS_WeaponDron[5] = {"dron_shot01","Reload_01","noammo", "", ""}			//dron
string WS_WeaponSpider[5] = {"robot_shot01","","", "", ""}			//spider

string WS_WeaponBoss[5] = {"RocketLauncher_fire01","Reload_01","noammo", "flameThrower_loop01", ""}			//BOSS


//nahoda je blbec. Nechame to tak velky, kolik je materialu, i kdyz se vsechny nepouzivaji
hsound RagdollPhysicsSounds[32]
hsound g_sCommonSounds[128]

hsound LFootstepSounds[8][32]
hsound RFootstepSounds[8][32]

const int FSNone = 0					//defaultne - zadne zvuky
const int FSPlayer = 1
const int FSHumanoidRobot = 2		//humanoidni kovovi roboti
const int FSSpiderRobot = 3
const int FSHumanoidNPC = 4		//Faggioli, Baldur apod

//---------------------------------------------------------------------------------
void LoadSounds()
{
	LoadSoundSet("Common", g_sCommonSounds, true)

	RagdollPhysicsSounds[MATERIAL_METAL] = GetSoundEx("physic_metal01", 0, 0)
	RagdollPhysicsSounds[MATERIAL_PLASTIC] = GetSoundEx("physic_metal01", 0, 0)
	RagdollPhysicsSounds[MATERIAL_IRON] = GetSoundEx("physic_metal01", 0, 0)
	RagdollPhysicsSounds[MATERIAL_RUBBER] = GetSoundEx("physic_metal01", 0, 0)
	RagdollPhysicsSounds[MATERIAL_FLESH] = GetSoundEx("flesh_hard01", 0, 0)

	LFootstepSounds[FSPlayer][MATERIAL_METAL] = GetSoundEx("fs_01a", 0, 0)
	RFootstepSounds[FSPlayer][MATERIAL_METAL] = GetSoundEx("fs_01b", 0, 0)
   LFootstepSounds[FSPlayer][MATERIAL_IRON] = GetSoundEx("fsIron_01a", 0, 0)
   RFootstepSounds[FSPlayer][MATERIAL_IRON] = GetSoundEx("fsIron_01b", 0, 0)
//	LFootstepSounds[FSPlayer][MATERIAL_GLASS] = GetSoundEx("blabla", 0, 0)
//	RFootstepSounds[FSPlayer][MATERIAL_GLASS] = GetSoundEx("blabla", 0, 0)
   LFootstepSounds[FSPlayer][MATERIAL_PLASTIC] = GetSoundEx("fsPlast_01a", 0, 0)
   RFootstepSounds[FSPlayer][MATERIAL_PLASTIC] = GetSoundEx("fsPlast_01b", 0, 0)
	LFootstepSounds[FSPlayer][MATERIAL_BETON] = GetSoundEx("fsBeton_01a", 0, 0)
	RFootstepSounds[FSPlayer][MATERIAL_BETON] = GetSoundEx("fsBeton_01b", 0, 0)
   LFootstepSounds[FSPlayer][MATERIAL_RUBBER] = GetSoundEx("fsPlast_01a", 0, 0)
   RFootstepSounds[FSPlayer][MATERIAL_RUBBER] = GetSoundEx("fsPlast_01b", 0, 0)
	LFootstepSounds[FSPlayer][MATERIAL_GRAVEL] = GetSoundEx("fsOut_02a", 0, 0)
	RFootstepSounds[FSPlayer][MATERIAL_GRAVEL] = GetSoundEx("fsOut_02b", 0, 0)
	LFootstepSounds[FSPlayer][MATERIAL_STONE] = GetSoundEx("fsOut_01a", 0, 0)
	RFootstepSounds[FSPlayer][MATERIAL_STONE] = GetSoundEx("fsOut_01b", 0, 0)

	LFootstepSounds[FSHumanoidRobot][MATERIAL_METAL] = GetSoundEx("HRobot_fstep01", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_METAL] = GetSoundEx("HRobot_fstep01", 0, 0)
	LFootstepSounds[FSHumanoidRobot][MATERIAL_IRON] = GetSoundEx("fsIron_02", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_IRON] = GetSoundEx("fsIron_02", 0, 0)
/*	LFootstepSounds[FSHumanoidRobot][MATERIAL_GLASS] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_GLASS] = GetSoundEx("blabla", 0, 0)
	*/
	LFootstepSounds[FSHumanoidRobot][MATERIAL_PLASTIC] = GetSoundEx("HRobot_fstep01", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_PLASTIC] = GetSoundEx("HRobot_fstep01", 0, 0)
	LFootstepSounds[FSHumanoidRobot][MATERIAL_BETON] = GetSoundEx("HRobot_fstep01", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_BETON] = GetSoundEx("HRobot_fstep01", 0, 0)
	LFootstepSounds[FSHumanoidRobot][MATERIAL_RUBBER] = GetSoundEx("fsPlast_01a", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_RUBBER] = GetSoundEx("fsPlast_01b", 0, 0)
	LFootstepSounds[FSHumanoidRobot][MATERIAL_STONE] = GetSoundEx("HRobot_fstep01", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_STONE] = GetSoundEx("HRobot_fstep01", 0, 0)
	LFootstepSounds[FSHumanoidRobot][MATERIAL_GRAVEL] = GetSoundEx("fsOut_02a", 0, 0)
	RFootstepSounds[FSHumanoidRobot][MATERIAL_GRAVEL] = GetSoundEx("fsOut_02b", 0, 0)
/*
	LFootstepSounds[FSSpiderRobot][MATERIAL_METAL] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_METAL] = GetSoundEx("blabla", 0, 0)
	LFootstepSounds[FSSpiderRobot][MATERIAL_IRON] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_IRON] = GetSoundEx("blabla", 0, 0)
	LFootstepSounds[FSSpiderRobot][MATERIAL_GLASS] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_GLASS] = GetSoundEx("blabla", 0, 0)
	LFootstepSounds[FSSpiderRobot][MATERIAL_PLASTIC] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_PLASTIC] = GetSoundEx("blabla", 0, 0)
	LFootstepSounds[FSSpiderRobot][MATERIAL_BETON] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_BETON] = GetSoundEx("blabla", 0, 0)
	LFootstepSounds[FSSpiderRobot][MATERIAL_RUBBER] = GetSoundEx("blabla", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_RUBBER] = GetSoundEx("blabla", 0, 0)
*/
	LFootstepSounds[FSSpiderRobot][MATERIAL_STONE] = GetSoundEx("fsOut_01a", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_STONE] = GetSoundEx("fsOut_01b", 0, 0)
	LFootstepSounds[FSSpiderRobot][MATERIAL_GRAVEL] = GetSoundEx("fsOut_02a", 0, 0)
	RFootstepSounds[FSSpiderRobot][MATERIAL_GRAVEL] = GetSoundEx("fsOut_02b", 0, 0)
}
