const int s_death		= 0	//overrides everything
const int s_pain		= 1	//overrides everything
const int s_weapon	= 2	//reloads,kickbacks,attacks etc.

const int s_cutscene	= 3	//3,4

const int s_custom	= 6
const int s_idlevar	= 7
const int s_walk		= 8	//walk/run

//bone groups
const int g_all = 1
const int g_reload = 2
const int g_headhands = 3
const int g_smallpain = 4
const int g_gesture = 5
const int g_confirmgesture = 6
const int g_legs = 7

//anim states
const int s_default = 0
const int s_relaxed = 0
const int s_alert = 1
const int s_combat = 2

//base movement anim types
const int t_default = 0
const int t_idle = 1
const int t_walk = 2
const int t_bwalk = 3

const int t_run = 4
const int t_brun = 5
const int t_lstrafe = 6
const int t_rstrafe = 7
const int t_run_aim = 8 //running, firing to front

const int t_run_fb = 9 //running, firing behind back
const int t_run_fl = 10 //running, firing to left
const int t_run_fr = 11 //running, firing to right

const int t_idle_fire = 12 //special anim when firing. For flamethrower for example

//anims

//IDLE
const int a_relax_idle = 100
const int a_relax_idle_var = 108

const int a_alert_idle = 116
const int a_alert_idle_var = 124

const int a_combat_idle = 132
const int a_combat_idle_var = 140

//IDLE special
const int a_spot_idle_var = 148
const int a_fight_idle_var = 156
const int a_lost_idle_var = 164

const int a_combat_idle_fire = 168

//WALK
const int a_relax_walk = 10
const int a_alert_walk = 12
const int a_combat_walk = 14

//RUN
const int a_relax_run = 16
const int a_relax_brun = 17
const int a_alert_run = 18
const int a_alert_brun = 19
const int a_combat_run = 20
const int a_combat_brun = 21
const int a_combat_run_aim = 22

//TURN (left 180degrees, right 180degrees)
const int a_relax_turn = 28
const int a_alert_turn = 30
const int a_combat_turn = 32

//BACKWALK
const int a_relax_bwalk = 38
const int a_alert_bwalk = 39
const int a_combat_bwalk = 40

//STRAFE
const int a_relax_strafe = 42
const int a_alert_strafe = 44
const int a_combat_strafe = 46

//PAIN
const int a_pain = 48
const int a_pain_strong = 52
const int a_pain_inplace = 60
const int a_pain_back = 64

//DEATH
const int a_death = 68
const int a_death_strong = 72
const int a_death_inplace = 80

//CROUCH
const int a_crouch = 88
const int a_crouch_fire = 91 //+1

const int a_combat_run_fire = 96 //back, left, right

const int a_jump_drop = 179
const int a_combat_railjump = 180
const int a_combat_windowjump = 181
const int a_combat_crawl = 182

const int a_combat_llean = 184 //in, idle, out
const int a_combat_rlean = 187 //in, idle, out

const int a_combat_grenade = 200 //up, down, left, right
const int a_combat_grenade_crouch = 204 //up, left, right

const int a_combat_closecombat = 210 //+5

const int a_combat_cover = 220 //armored enemies don't run. Just cover

//gestures
const int a_act_left_go = 225
const int a_act_right_go = 226
const int a_act_back_go = 227
const int a_act_forward_go = 228
const int a_act_hold = 229

const int a_act_look_left = 235
const int a_act_look_right = 236

//human body pivots
const int p_lhand = 0
const int p_rhand = 1
const int p_rhand2 = 2

const int p_weapon0 = 3
const int p_weapon1 = 4
const int p_weapon2 = 5
const int p_weapon3 = 6
const int p_camera = 7

//any purpose pivots
const int p_pivot0 = 0
const int p_pivot1 = 1
const int p_pivot2 = 2
const int p_pivot3 = 3
const int p_pivot4 = 4
const int p_pivot5 = 5
const int p_pivot6 = 6
const int p_pivot7 = 7

//Human body skeleton. Common bones
const int B_SCENE_ROOT		= 0
const int B_Layer1			= 1
const int B_Common_Body	= 2	//priblizne stred trupu
const int B_Common_Head	= 3


//Human body NEW skeleton
const int B_Hips						= 2
const int B_Head						= 3
const int B_Spine						= 4
const int B_Spine1					= 5
const int B_Spine2					= 6
const int B_Neck						= 7
const int B_Pelvis					= 8
const int B_LeftShoulderBlade		= 9
const int B_LeftShoulder			= 10
const int B_LeftArm					= 11
const int B_LeftArmRoll				= 12
const int B_LeftForeArm				= 13
const int B_LeftForeArmRoll		= 14
const int B_LeftHand					= 15
const int B_LeftHandPivot			= 16

const int B_LeftHandThumb1			= 17
const int B_LeftHandThumb2			= 18
const int B_LeftHandIndex1			= 19
const int B_LeftHandIndex2			= 20
const int B_LeftHandMiddle1		= 21
const int B_LeftHandMiddle2		= 22


const int B_RightShoulderBlade	= 23
const int B_RightShoulder			= 24
const int B_RightArm					= 25
const int B_RightArmRoll			= 26
const int B_RightForeArm			= 27
const int B_RightForeArmRoll		= 28
const int B_RightHand				= 29
const int B_RightHandPivot			= 30

const int B_RightHandThumb1		= 31
const int B_RightHandThumb2		= 32
const int B_RightHandIndex1		= 33
const int B_RightHandIndex2		= 34
const int B_RightHandMiddle1		= 35
const int B_RightHandMiddle2		= 36

const int B_LeftUpLeg				= 37
const int B_LeftUpLegRoll			= 38
const int B_LeftLeg					= 39
const int B_LeftLegRoll				= 40
const int B_LeftFoot					= 41
const int B_LeftBackFoot			= 42 //FIXME: chybicka, duplicitni nazev
const int B_LeftToeBase				= 42
const int B_RightUpLeg				= 43
const int B_RightUpLegRoll			= 44
const int B_RightLeg					= 45
const int B_RightLegRoll			= 46
const int B_RightFoot				= 47
const int B_RightToeBase			= 48

const int B_LeftHandThumb3			= 49
const int B_LeftHandMiddle3		= 50
const int B_LeftHandIndex3			= 51

const int B_LeftFingerRoll			= 52
const int B_LeftHandPinky1			= 53
const int B_LeftHandPinky2			= 54
const int B_LeftHandPinky3			= 55
const int B_LeftHandRing1			= 56
const int B_LeftHandRing2			= 57
const int B_LeftHandRing3			= 58
const int B_RightHandThumb3		= 59
const int B_RightHandMiddle3		= 60
const int B_RightHandIndex3		= 61
const int B_RightFingerRoll		= 62
const int B_RightHandPinky1		= 63

const int B_RightHandPinky2		= 64
const int B_RightHandPinky3		= 65

const int B_RightHandRing1		= 66
const int B_RightHandRing2		= 67
const int B_RightHandRing3		= 68

//Human body OLD skeleton
const int B_Spine01			= B_Common_Body
const int B_Hlava				= B_Common_Head
const int B_Skeleton_Root	= 4
const int B_Spine02			= 5
const int B_Krk				= 6
const int B_L_Rameno			= 7
const int B_L_Rameno_2		= 8
const int B_L_Zarameni		= 9
const int B_L_Zarameni_2	= 10
const int B_L_Predlokti		= 11
const int B_L_Predlokti_2	= 12
const int B_L_Zapesti		= 13
const int B_R_Rameno			= 14
const int B_R_Rameno_2		= 15
const int B_R_Zarameni		= 16
const int B_R_Zarameni_2	= 17
const int B_R_Predlokti		= 18
const int B_R_Predlokti_2	= 19
const int B_R_Zapesti		= 20
const int B_L_Kycle			= 21
const int B_L_Stehno			= 22
const int B_L_Lejtko			= 23
const int B_L_Noha			= 24
const int B_R_Kycle			= 25
const int B_R_Stehno			= 26
const int B_R_Lejtko			= 27
const int B_R_Noha			= 28
const int B_Spine03			= 29
const int B_Celist			= 30

const int B_L_Stehno_2		= 43
const int B_L_Noha_2			= 44
const int B_R_Stehno_2		= 45
const int B_R_Noha_2			= 46

//Player Fingers
const int B_L_Palec			= 31
const int B_L_Palec_2		= 32
const int B_L_Prsty			= 33
const int B_L_Ukazovacek	= 34
const int B_L_Ukazovacek_2	= 35
const int B_L_Prsty_2		= 36
const int B_R_Palec			= 37
const int B_R_Palec_2		= 38
const int B_R_Prsty			= 39
const int B_R_Ukazovacek	= 40
const int B_R_Ukazovacek_2	= 41
const int B_R_Prsty_2		= 42

//Human - pivots
const int B_CameraPivot		= 57
const int B_L_Ruka_Pivot	= 58
const int B_R_Ruka_Pivot	= 59
const int B_Weapon_P4		= 60
const int B_Weapon_P3		= 61
const int B_Weapon_P2		= 62
const int B_Weapon_P1		= 63

//FPS HANDS -------------------------------
//Player FPS Hands Fingers
const int B_L_Palec1			= 31
const int B_L_Palec2			= 32
const int B_L_Ukaz1			= 33
const int B_L_Ukaz2			= 34
const int B_L_Prost1			= 35
const int B_L_Prost2			= 36
const int B_L_Prst1			= 37
const int B_L_Prst2			= 38
const int B_L_Mali1			= 39
const int B_L_Mali2			= 40
const int B_R_Palec1			= 41
const int B_R_Palec2			= 42
const int B_R_Ukaz1			= 21
const int B_R_Ukaz2			= 22
const int B_R_Prost1			= 23
const int B_R_Prost2			= 24
const int B_R_Prst1			= 25
const int B_R_Prst2			= 26
const int B_R_Mali1			= 27
const int B_R_Mali2			= 28



//SCIFI ROBOT -----------------------------
const int B_Podvozek			= B_Common_Body 
const int B_SkeletonRoot	= 4
const int B_RF_KycelKloub  = 7
const int B_RF_Kycel       = 8
const int B_RF_Stehno      = 9
const int B_RF_Lejtko		= 10
const int B_RF_Noha			= 11
const int B_LF_KycelKloub  = 12
const int B_LF_Kycel       = 13
const int B_LF_Stehno      = 14
const int B_LF_Lejtko		= 15
const int B_LF_Noha			= 16
const int B_RB_KycelKloub  = 17
const int B_RB_Kycel       = 18
const int B_RB_Stehno      = 19
const int B_RB_Lejtko		= 20
const int B_RB_Noha			= 21
const int B_LB_KycelKloub  = 22
const int B_LB_Kycel       = 23
const int B_LB_Stehno      = 24
const int B_LB_Lejtko		= 25
const int B_LB_Noha			= 26

const int B_Body				= B_Common_Head
const int B_Kridlo_R			= 4
const int B_Kridlo_D			= 5
const int B_Kridlo_L			= 6
const int B_Rameno_L			= 7
const int B_Antena_T			= 8
const int B_Hlava2			= 27
const int B_P_Fire			= 28

//gun
const int B_fixed				= 2
const int B_Fixed				= 2
const int B_P_Shell			= 3
const int B_Ammo				= 4
const int B_Zaver				= 5
const int B_Rotor				= 6


//plamenomet
const int B_P_Flame			= 10
const int B_L_Ammo			= 11
const int B_R_Ammo			= 12
const int B_Flame_rod		= 13

//raketomet
const int B_RL_TL				= 10
const int B_RL_TR				= 11
const int B_RL_BL				= 12
const int B_RL_BR				= 13
const int B_dolni				= 14
const int B_horni				= 15
const int B_horni_2			= 16

//sniperka
const int B_Sniper_Display = 4
const int B_Sniper_Ammo = 5

//shotgun
const int B_Shot_Slider = 4

//mina
const int B_mine = 31

const int B_OxDev_pist = 4
const int B_OxDev_fixed = 5
const int B_OxygenDevice_lwo = 6

const int B_HlthDev_pist = 4
const int B_HlthDev_fixed = 5

//handle
const int B_SD1_HANDLE = 4

//capsule
const int B_CPSL01_L = 4
const int B_CPSL01_R = 5


const int B_airdoor4_base = 4
const int B_airdoor4_lock = 5

const int B_loader_arm1_L = 4
const int B_loader_arm2_L = 5
const int B_loader_arm3_L = 6
const int B_loader_arm4_L = 7
const int B_loader_arm2pist2_L = 8
const int B_loader_arm2pist1_L = 9
const int B_loader_arm1_R = 10
const int B_loader_arm2_R = 11
const int B_loader_arm3_R = 12
const int B_loader_arm4_R = 13
const int B_loader_arm2pist2_R = 14
const int B_container = 15
const int B_loader_arm2pist1_R = 16
const int B_HLOAD_con1 = 17

const int B_crane = 18
const int B_arm_BASE = 19

const int B_bedna01 = 4
const int B_bedna02 = 5
const int B_bedna03 = 6
const int B_bedna04 = 7
const int B_bedna05 = 8
const int B_bedna06 = 9

const int B_meteorBONE1 = 4
const int B_meteorBONE2 = 5
const int B_meteorBONE3 = 6

const int B_GunHead = 4
const int B_GunBody = 5

const int B_safedoor_base = 4
const int B_safedoor_rot = 5
const int B_safedoor_lock = 6

const int B_TRSP_door = 4

//facial anim morphs
const int B_BrowsDown_L		= 4
const int B_BrowsDown_R		= 5
const int B_BrowsUp_L		= 6
const int B_BrowsUp_R		= 7
const int B_BrowsUpSide_L	= 8
const int B_BrowsUpSide_R	= 9
const int B_Mrkani_L		= 10
const int B_Mrkani_R		= 11
const int B_Privreni_L		= 12
const int B_Privreni_R		= 13
const int B_Angry		= 14
const int B_Sad			= 15
const int B_AI			= 16
const int B_BMP			= 17
const int B_CDG			= 18
const int B_E			= 19
const int B_FV			= 20
const int B_O			= 21
const int B_QW			= 22
const int B_U			= 23
const int B_T			= 24



int P1 = 16
int P2 = 17
//3,14159265358979
//-----------------------------------------------------------------
void InitSkeletons()
{
	//full skeleton
/*
   BeginGroup(g_reload)
		Bone(B_SCENE_ROOT)
		Bone(B_Layer1)
		Bone(B_Hips)
		Bone(B_Head)
		Bone(B_Spine)
		Bone(B_Spine1)
		Bone(B_Spine2)
		Bone(B_Neck)
		Bone(B_Pelvis)
		Bone(B_LeftShoulderBlade)
		Bone(B_LeftShoulder)
		Bone(B_LeftArm)
		Bone(B_LeftArmRoll)
		Bone(B_LeftForeArm)
		Bone(B_LeftForeArmRoll)
		Bone(B_LeftHand)
		Bone(B_LeftHandPivot)
		Bone(B_RightShoulderBlade)
		Bone(B_RightShoulder)
		Bone(B_RightArm)
		Bone(B_RightArmRoll)
		Bone(B_RightForeArm)
		Bone(B_RightForeArmRoll)
		Bone(B_RightHand)
		Bone(B_RightHandPivot)
		Bone(B_LeftUpLeg)
		Bone(B_LeftUpLegRoll)
		Bone(B_LeftLeg)
		Bone(B_LeftLegRoll)
		Bone(B_LeftFoot)
		Bone(B_LeftToeBase)
		Bone(B_RightUpLeg)
		Bone(B_RightUpLegRoll)
		Bone(B_RightLeg)
		Bone(B_RightLegRoll)
		Bone(B_RightFoot)
		Bone(B_RightToeBase)
*/


   BeginGroup(g_headhands)
		Bone(B_Head)
		Bone(B_Neck)
		Bone(B_LeftShoulderBlade)
		Bone(B_LeftShoulder)
		Bone(B_LeftArm)
		Bone(B_LeftArmRoll)
		Bone(B_LeftForeArm)
		Bone(B_LeftForeArmRoll)
		Bone(B_LeftHand)
		Bone(B_RightShoulderBlade)
		Bone(B_RightShoulder)
		Bone(B_RightArm)
		Bone(B_RightArmRoll)
		Bone(B_RightForeArm)
		Bone(B_RightForeArmRoll)
		Bone(B_RightHand)
		Bone(B_RightHandPivot)

   BeginGroup(g_reload)
		Bone(B_Head)
		Bone(B_Spine)
		Bone(B_Spine1)
		Bone(B_Spine2)
		Bone(B_Neck)
		Bone(B_LeftShoulderBlade)
		Bone(B_LeftShoulder)
		Bone(B_LeftArm)
		Bone(B_LeftArmRoll)
		Bone(B_LeftForeArm)
		Bone(B_LeftForeArmRoll)
		Bone(B_LeftHand)
		Bone(B_RightShoulderBlade)
		Bone(B_RightShoulder)
		Bone(B_RightArm)
		Bone(B_RightArmRoll)
		Bone(B_RightForeArm)
		Bone(B_RightForeArmRoll)
		Bone(B_RightHand)
		Bone(B_RightHandPivot)

   BeginGroup(g_gesture)
		Bone(B_Head)
		Bone(B_Spine)
		Bone(B_Spine1)
		Bone(B_Spine2)
		Bone(B_Neck)
		Bone(B_LeftShoulderBlade)
		Bone(B_LeftShoulder)
		Bone(B_LeftArm)
		Bone(B_LeftArmRoll)
		Bone(B_LeftForeArm)
		Bone(B_LeftForeArmRoll)
		Bone(B_LeftHand)
		Bone(B_RightShoulderBlade)
		Bone(B_RightShoulder)
		Bone(B_RightArm)
		Bone(B_RightArmRoll)
		Bone(B_RightForeArm)
		Bone(B_RightForeArmRoll)
		Bone(B_RightHand)
		Bone(B_RightHandPivot)

   BeginGroup(g_confirmgesture)
		Bone(B_Head)
		Bone(B_Spine)
		Bone(B_Spine1)
		Bone(B_Spine2)
		Bone(B_Neck)

   BeginGroup(g_smallpain)
		Bone(B_Head)
		Bone(B_Spine)
		Bone(B_Spine1)
		Bone(B_Spine2)
		Bone(B_Neck)
//		Bone(B_Pelvis)      -- pevlis se nesmi pouzivat pro upper body protoze je nadrazenej oboum noham (panev)
		Bone(B_LeftShoulderBlade)
		Bone(B_LeftShoulder)
		Bone(B_LeftArm)
		Bone(B_LeftArmRoll)
		Bone(B_LeftForeArm)
		Bone(B_LeftForeArmRoll)
		Bone(B_LeftHand)
		Bone(B_LeftHandPivot)
		Bone(B_RightShoulderBlade)
		Bone(B_RightShoulder)
		Bone(B_RightArm)
		Bone(B_RightArmRoll)
		Bone(B_RightForeArm)
		Bone(B_RightForeArmRoll)
		Bone(B_RightHand)
		Bone(B_RightHandPivot)

   BeginGroup(g_all)
   	SetAllBones()

   BeginGroup(g_legs)
		Bone(B_SCENE_ROOT)
		Bone(B_Layer1)
		Bone(B_Spine)
		Bone(B_Spine1)
		Bone(B_Spine2)
		Bone(B_Hips)
		Bone(B_Pelvis)
		Bone(B_LeftUpLeg)
		Bone(B_LeftUpLegRoll)
		Bone(B_LeftLeg)
		Bone(B_LeftLegRoll)
		Bone(B_LeftFoot)
		Bone(B_LeftToeBase)
		Bone(B_RightUpLeg)
		Bone(B_RightUpLegRoll)
		Bone(B_RightLeg)
		Bone(B_RightLegRoll)
		Bone(B_RightFoot)
		Bone(B_RightToeBase)
}
