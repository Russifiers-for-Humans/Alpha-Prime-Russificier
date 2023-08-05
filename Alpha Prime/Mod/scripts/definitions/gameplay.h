#ifdef DEMO
	string AlphaPrimeVersion = "Demo 1.3"						//vpravo dole v main menu
#else
	string AlphaPrimeVersion = "1.3"						//vpravo dole v main menu
#endif

// !!!!!!!! NEMENIT Start !!!!!!!!
//vyska kamery (je PlayerCapsuleExtends[crouch] * 2) + PlayerCapsuleRadius
const float PlayerCapsuleRadius = 11					//polomer kapsuly
const float PlayerCapsuleExtends[2] = {17, 6}		//od stredu kapsuly po zaciatok obluku
const float PlayerCameraOffset = 4						//od zaciatku horneho poloobluku sa kamera umiestni o tuto hodnotu vysie
const float PlayerMaxLeanAngle = 25					//maximalny uhol naklonu do stran
const float PlayerMaxSurfaceAngle = 60				//do jak strmeho kopca bude schopny vyliezt. 0 - 90
const float PlayerMaxSurfaceAngleForNormalJump = 40	//ped je na sikmejsej ploche nez toto tak uz neskace smerom hore ale v smere normaly kontaktu
const float PlayerMaxStairHeight = 19				//jak velku prekazku (schod) dokaze zdolat


const float PlayerWalkSpeed = 189		//maximalna rychlost chodze	//160
const float PlayerRunSpeed = 194
const float PlayerCrouchSpeed = 90
const float PlayerJumpPow = 14000//dwefault 10008
const float PlayerExactAimSpeed = 169


const float PlayerWalkAccel = 3500		//jak rychle bude zrychlovat z 0 na normalnu rychlost
const float PlayerRunAccel = 3000
const float PlayerCrouchAccel = 2000
const float PlayerExactAimAccel = 2000
const float PlayerJumpControlAccel = 275
const float PlayerFootstepTime = 43

// !!!!!!!! NEMENIT End !!!!!!!!

const float IdleWarAnimWaitMin = 1100
const float IdleWarAnimWaitMax = 2200


//const float DoorTriggerOverrunFront = 70		//o kolko presahuje interny trigger dveri smerom pred a za dvere (ked pozerame priamo na nich)
//const float DoorTriggerOverrunSide = 12		//o kolko presahuje interny trigger dveri smerom do stran (ked pozerame priamo na nich)
const float DoorTriggerOverrunFront = 54		//o kolko presahuje interny trigger dveri smerom pred a za dvere (ked pozerame priamo na nich)
const float DoorTriggerOverrunSide = 6		//o kolko presahuje interny trigger dveri smerom do stran (ked pozerame priamo na nich)

const float ReconSignalDistFromTrigger = 660	//maximalana vzdialenost playera od okraja hackovaceho triggeru kde este "chyta signal"

const float BulletTimeScale = 0.26			//spomalenie pre bulletTime
const float BulletTimeMax = 79		//cas po ktory vydrzi plny bullet time (v sekudach)


const float FlashLightMaxTime = 20			//vydrz baterky v sekundach (cas)
const float FlashLightCharging = 4			//za jaky cas sa baterka nabije. v sekundach (cas)

const float PlayerHealthUpSpeed = 0.5		//0 - 1  percento z playeroveho zdravia ktore sa mu pripocita za jednu sekundu doplnovania
const float PlayerHealthPumpWait = 4				//cas po ktory sa od playeroveho zranenia nebude pripocitavat health (v sekundach)

const float MineActivationDistance = 200			//vdialenost ktora je potrebna k aktivovaniu miny (item_weapon_mine)
//=========================================================================================
//const float PLAYER_DAMAGE_SCALE[3]	= {2.0, 1.5, 1.2} //globalni faktor na posileni playera.		EASY NORMAL HARD
//const float ENEMY_DAMAGE_SCALE[3]	= {0.7, 0.8, 1.0} //globalni faktor na posileni nepratel	EASY NORMAL HARD
//1.3
const float ENEMY_DAMAGE_SCALE[3]	= {0.7, 0.8, 1.0} //globalni faktor na posileni nepratel	EASY NORMAL HARD

//=========================================================================================
//Ruzne nastaveni
//=========================================================================================

const float DISTANCE_TO_CLOSECOMBAT = 55 //pri teto vzdalenosti mezi playerem a nepritelem dojde k close-hitu
													 //Nezvysovat! pokud se nezvysi i AttackDistMin

const float DIST_TO_IMMED_REACTION = 99 //pod tuto vzdalenost reaguje okamzite, jinak ma trochu dlouhe vedeni,
													//v zavislosti na vzdalenosti, aby mel player sanci nakouknout a hned
													//zmizet aniz by vzbudil pozornost

const float MIN_OBSTACLE_SIZE			= 10 //pod tuto velikost (plati pro vsechny osy) se nevytvari prekazka pro patfinder

//do jake vzdalenosti jsou slyset nasledujici ruchy
const float NOISERANGE_FOOTSTEP		= 200 //kroky behu (chuze slyset neni)
const float NOISERANGE_THUD 			= 250 //dopad ze skoku
const float NOISERANGE_SHOOT			= 580 //vystrel
const float NOISERANGE_ATTACK			= 320 //
const float NOISERANGE_PHYSICS		= 200 //dopad fyzikalniho objektu
const float NOISERANGE_FLAME			= 385

//=========================================================================================
// Ammo
//=========================================================================================

const int PistolAmmoPerMagazine = 10
const int ShotgunAmmoPerMagazine = 9
const int MachinegunAmmoPerMagazine = 31
const int SniperRifleAmmoPerMagazine = 5
const int RocketLauncherAmmoPerMagazine = 2
const int FlameThrowerAmmoPerMagazine = 500
const int GrenadeAmmoPerMagazine = 1

//v akom poradi sa ma pokusat volit zbrane ked v tej ktoru drzime nam dosli naboje
const int BestWeapChoices[9][7] = {	4,3,2,5,7,6,1,	//CDummyWeapon			0
												4,3,2,5,7,6,8,	//CJackHammer			1
												3,4,5,7,1,8,6,	//CPistol				2
												4,5,7,2,1,8,6,	//CShotgun				3
												3,5,7,2,1,8,6,	//CMachinegun			4
												4,3,7,2,1,8,6,	//CSniperRifle			5
												4,3,5,7,2,1,8,	//CRocketLauncher		6
												4,3,5,2,1,8,6,	//CFlameThrower		7
												4,3,5,7,2,1,6}	//CGrenade				8

//naboje do playerovych zbrani na zaciatku hry. kedze na zaciatku ma len kladivo tak uz to neni aktualne
const int PistolAmmoOnStart = 60
const int ShotgunAmmoOnStart = 26
const int MachinegunAmmoOnStart = 17
const int SniperRifleAmmoOnStart = 5
const int RocketLauncherAmmoOnStart = 4
const int FlameThrowerAmmoOnStart = 400
const int GrenadeAmmoOnStart = 2
const int GrenadeMaximumAmmo = 10

//mnozstvo nabojov v krabickach najdenych v levloch	EASY NORMAL HARD
const int AmmoGunQuantity[3] =				{24, 20, 8}
const int AmmoShotgunQuantity[3] = 			{19, 12, 5}
const int AmmoMachinegunQuantity[3]	= 		{60, 50, 16}
const int AmmoSniperRifleQuantity[3] = 	{12, 10, 4}
const int AmmoRocketLauncherQuantity[3] = {4, 2, 1}
const int AmmoFlameThrowerQuantity[3] = 	{720, 600, 200}

//naboje v zbraniach po zabitych enemakoch			 EASY NORMAL HARD
const int DroppedAmmoGunQuantityMin[3]	=					{5, 4, 2}
const int DroppedAmmoGunQuantityMax[3]	=					{10, 10, 4}

const int DroppedAmmoShotgunQuantityMin[3] = 			{5, 3, 2}
const int DroppedAmmoShotgunQuantityMax[3] = 			{8, 6, 5}

const int DroppedAmmoMachinegunQuantityMin[3] = 		{5, 2, 1}
const int DroppedAmmoMachinegunQuantityMax[3] = 		{19, 16, 7}

const int DroppedAmmoSniperRifleQuantityMin[3] =		{1, 1, 1}
const int DroppedAmmoSniperRifleQuantityMax[3] =		{8, 5, 3}

const int DroppedAmmoRocketLauncherQuantityMin[3] =	{2, 1, 1}
const int DroppedAmmoRocketLauncherQuantityMax[3] =	{4, 4, 3}

const int DroppedAmmoFlameThrowerQuantityMin[3] =		{1, 1, 1}
const int DroppedAmmoFlameThrowerQuantityMax[3] =		{230, 210, 60}

//maximalne ammo pre jednotlive zbrane
const int MaxAmmos[9] = {	1,		//CDummyWeapon			0
									1,		//CJackHammer			1
									250,	//CPistol				2
									120,	//CShotgun				3
									370,	//CMachinegun			4
									50,	//CSniperRifle			5
									20,	//CRocketLauncher		6
									9999999,	//CFlameThrower		7
									10}		//CGrenade				8

//=========================================================================================
// Health
//=========================================================================================

const float PlayerMaxHealth				= 6000
const int	MedikitHealth[3]				= {4000, 2800, 2000}	//EASY NORMAL HARD
const float HealthDeviceHealth[3]		= {7500, 5500, 2000}	//EASY NORMAL HARD
const float HealthPumpUpSpeed				= 1200	//kolik jednotek kysliku pribudne za jednu sekundu cerpani z entity func_health_device
const float	PlayerMaxOxygen				= 90	//plny zasobnik kysliku
const float OxygenPumpUpSpeed				= 20	//kolik jednotek kysliku pribudne za jednu sekundu cerpani z entity func_oxygen
const float OxygenDownSpeeds[3]			= {1.5, 1.0, 0.7}	//jak rychle mu klesa zasoba kysliku v nedychatelnej atmosfere. jednotky za sekundu  {MaximumToxicity, MediumToxicity, MinimumToxicity}
const float NoOxygenHurtTime				= 1	//ked nema co dychat a nema zasobu kysliku tak bude zranovany v tomto intervale (v sekundach)
const float NoOxygenHealthDown			= 30	//vzdy ked bude zraneny tym ze nema kyslik, uberie sa mu tato ciastka

const float PlayerDropDownHurtHeight	= 140	 //vyska od ktorej to pri pade playera zacina zranovat. (skok z tejto vysky zacina zranovat)
const float PlayerDropDownHurtRange		= 190	//PlayerDropDownHurtHeight + tato hodnota je vyska ktora znamena zabite playera aj ked ma plnu health

/*
const float FaggioliHealth = 5000
const float WarrenHealth = 5000
const float NPCcommanderHealth = 5000
const float Dron1Health = 600
const float HeavySpiderHealth = 7000
const float ServiceBot1Health = 2000 //servisak
const float ServiceBot2Health = 2000 //cerveny agresivni servisak
const float SecuritySpiderHealth = 6000

const float TechnicianHealth		= 900
const float GuardHealth				= 1800
const float LightTrooperHealth	= 3500
const float HeavyTrooperRLHealth	= 7000

//plamenometcik musi mit podstatne vetsi vydrz, nez cokoliv jineho
//protoze se pohybuje pomalu, ma kratky dostrel a musi byt videt, jak se i obcas kryje
//nez ho player udola. Ted sel sejmout behem 10sec
const float HeavyTrooperFTHealth	= 9000

const float CommanderHealth		= 6000

const float BossHealth		= 100000
*/

// 1.3 version
const float ThrowDamage = 200 //kolik sebere zasah vrzenym predmetem
const float FaggioliHealth = 4100
const float WarrenHealth = 4100
const float NPCcommanderHealth = 1500
const float Dron1Health = 700
const float HeavySpiderHealth = 6500
const float ServiceBot1Health = 1800//servisak
const float ServiceBot2Health = 1900 //cerveny agresivni servisak
const float SecuritySpiderHealth = 3800

const float TechnicianHealth		= 980
const float GuardHealth				= 1500
const float LightTrooperHealth	= 2900
const float HeavyTrooperRLHealth	= 5000

//plamenometcik musi mit podstatne vetsi vydrz, nez cokoliv jineho
//protoze se pohybuje pomalu, ma kratky dostrel a musi byt videt, jak se i obcas kryje
//nez ho player udola. Ted sel sejmout behem 10sec
const float HeavyTrooperFTHealth	= 9000

const float CommanderHealth		= 4000

const float BossHealth		= 100000

//=========================================================================================
// Weapon/Monster damage
//=========================================================================================

//rana zbrani do drzky. Mela by byt HODNE silna, aby se radeji player k nepratelum moc nesral
const float WDmgCloseHit = 1900

const float WDmgJackHammer	= 1700
const float WDmgGun	= 760
const float WDmgShotgun	= 590 //nasobeno poctem broku, takze je ta hodnota v realu 8x vyssi!

const float WDmgMachinegun	= 490
const float WDmgSniperRifle = 2500
const float WDmgRocketLauncher = 1800
const float WDmgFlameThrower = 60
const float WDmgGrenade = 3350
const float WDmgTachyonGrenade = 5580

const float WDmgTurret	= 250
const float WDmgHeavyTurret = 3000
const float WDmgMine = 3500

const float WDmgFaggioli = 150
const float WDmgWarren = 150
const float WDmgNPCcommander = 150
const float WDmgSpiderRobot	= 60
const float WDmgDron	= 10
const float WDmgServiceBot1	= 100
const float WDmgServiceBot2	= 189
const float WDmgSecuritySpider1 = 120

const float WDmgTechnician		= 160
const float WDmgGuard1			= 200 //shotgun
const float WDmgGuard2			= 175 //machinegun
const float WDmgLightTrooper	= 190 //machinegun
const float WDmgLightTrooper2	= 200 //shotgun
const float WDmgLightTrooper3	= 335 //sniperrifle

const float WDmgHeavyTrooperRL	= 1950
const float WDmgHeavyTrooperFT	= 48

const float WDmgCommander		= 250

const float WDmgEnemyGrenade = 2400

const float WDmgBoss = 2000 //rockets
const float WDmgBoss2 = 1000 //sperma/second

//=========================================================================================
// Definice zranovaci tabulky pro jednotlive typy nepratel
//=========================================================================================
float HumanHurtFactors[64]

void InitHurtFactors()
{
	//HumanHurtFactors
	HumanHurtFactors[B_Hips] = 0.95

//	HumanHurtFactors[B_Head] = 2.0
//	HumanHurtFactors[B_Neck] = 2.0
//1.3
	HumanHurtFactors[B_Head] = 3.5
	HumanHurtFactors[B_Neck] = 2.7

        HumanHurtFactors[B_Hips] = 0.8

	HumanHurtFactors[B_Spine] = 1.2
	HumanHurtFactors[B_Spine1] = 0.9
	HumanHurtFactors[B_Spine2] = 1.25
	HumanHurtFactors[B_Pelvis] = 1.5
	HumanHurtFactors[B_LeftShoulderBlade] = 0.7
	HumanHurtFactors[B_LeftShoulder] = 0.75
	HumanHurtFactors[B_LeftArm] = 0.85
	HumanHurtFactors[B_LeftArmRoll] = 0.89
	HumanHurtFactors[B_LeftForeArm] = 0.95
	HumanHurtFactors[B_LeftForeArmRoll] = 1.15
	HumanHurtFactors[B_LeftHand] = 0.87
        HumanHurtFactors[B_LeftHandPivot] = 0.82
	HumanHurtFactors[B_LeftHandThumb1] = 0.78
	HumanHurtFactors[B_LeftHandThumb2] = 0.78
	HumanHurtFactors[B_LeftHandIndex1] = 0.78
	HumanHurtFactors[B_LeftHandIndex2] = 0.78
	HumanHurtFactors[B_LeftHandMiddle1] = 0.78
	HumanHurtFactors[B_LeftHandMiddle2] = 0.78
	HumanHurtFactors[B_RightShoulderBlade] = 0.85
	HumanHurtFactors[B_RightShoulder] = 0.9
	HumanHurtFactors[B_RightArm] = 0.85
	HumanHurtFactors[B_RightArmRoll] = 0.7
	HumanHurtFactors[B_RightForeArm] = 0.7
	HumanHurtFactors[B_RightForeArmRoll] = 1.15
	HumanHurtFactors[B_RightHand] = 0.79
	HumanHurtFactors[B_RightHandPivot] = 0.7
	HumanHurtFactors[B_RightHandThumb1] = 0.7
	HumanHurtFactors[B_RightHandThumb2] = 0.7
	HumanHurtFactors[B_RightHandIndex1] = 0.7
	HumanHurtFactors[B_RightHandIndex2] = 0.7
	HumanHurtFactors[B_RightHandMiddle1] = 0.7
	HumanHurtFactors[B_RightHandMiddle2] = 0.7
	HumanHurtFactors[B_LeftUpLeg] = 0.9
	HumanHurtFactors[B_LeftUpLegRoll] = 1.2
	HumanHurtFactors[B_LeftLeg] = 0.98
	HumanHurtFactors[B_LeftLegRoll] = 1.0
	HumanHurtFactors[B_LeftFoot] = 0.8
	HumanHurtFactors[B_LeftToeBase] = 0.7
	HumanHurtFactors[B_RightUpLeg] = 0.9
	HumanHurtFactors[B_RightUpLegRoll] = 1.2
	HumanHurtFactors[B_RightLeg] = 0.98
	HumanHurtFactors[B_RightLegRoll] = 1.0
	HumanHurtFactors[B_RightFoot] = 0.8
	HumanHurtFactors[B_RightToeBase] = 0.7
}

