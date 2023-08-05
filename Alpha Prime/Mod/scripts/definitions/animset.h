//------------------------- -----------------------------------------------
/*
a_relax_idle			+0...7 //0 je zakladni loop
a_relax_idle_var		+0...7

a_alert_idle			+0...7 //0 je zakladni loop
a_alert_idle_var		+0...7

a_combat_idle			+0...7 //0 je zakladni loop
a_combat_idle_var		+0...7

a_spot_idle_var		+0...7
a_fight_idle_var		+0...7 //Nepouzito
a_lost_idle_var		+0...7 //Nepouzito (shortrange zbran a player nedosazitelny)

a_relax_walk			//walk
a_relax_bwalk			//backwalk

a_alert_walk
a_alert_bwalk

a_combat_walk
a_combat_bwalk

a_relax_run				//run
a_relax_brun			//backrun

a_alert_run
a_alert_brun

a_combat_run
a_combat_brun		//Neco mezi behem a chuzi, s namirenou zbrani

a_combat_run_aim	//Neco mezi behem a chuzi, s namirenou zbrani

a_relax_turn		+0 left 180' +1 right 180'
a_alert_turn		+0 left 180' +1 right 180'
a_combat_turn		+0 left 180' +1 right 180'


a_relax_strafe		+0 left, +1 right
a_alert_strafe		+0 left, +1 right
a_combat_strafe	+0 left, +1 right //Neco mezi behem a chuzi, s namirenou zbrani

a_crouch				+0 idle +1 down +2 up
a_crouch_fire		+0 +1, strileni prez prekazku

a_pain				+0...3 painy pri zasahu zpredu
a_pain_back			+0...3 painy pri zasahu zezadu


a_pain_strong		//zatim nepouzito
a_pain_inplace		//zatim nepouzito

a_combat_run_fire		//panicky uprk+strileni do stran: +0 back, +1 left, +2 right

a_jump_drop				//dopad na zem ze skoku
a_combat_railjump		//preskok 1m zabradli
a_combat_windowjump	//proskoceni oknem

a_combat_llean = 182 //+0 in, +1 idle, +2 out
a_combat_rlean = 185 //+0 in, +1 idle, +2 out

a_combat_run_jump		//skok&parakotoul?
a_combat_run_roll		//prekuleni prez prekazku?
a_combat_crawl			//podlezeni prekazky

a_combat_grenade		//hozeni granatu
*/

//------------------------------------------------------------------------
// BeginAnimSet("jmeno") zacne definice noveho animsetu

// Anim(slot, "animace" [, fps]) prida animaci do animsetu
// Import("animset") naimportuje do aktualniho animsetu jiny animset. Pouze ty animace, ktere jsme jeste nezadali!
//-----------------------------------------------------------------
void WorldClass::InitAnimSets()
{
//-----------------------------------------------------------------------
	BeginAnimSet("HeavySpider")
//-----------------------------------------------------------------------
	ApplyAnimFlags(ASF_AIMING)
	Anim(a_relax_idle + 0,"anm/heavyspider01/r_idle.anm")
	Anim(a_relax_idle_var + 0,"anm/heavyspider01/r_idle_var1.anm")
	Anim(a_relax_idle_var + 1,"anm/heavyspider01/r_idle_var2.anm")
	Anim(a_relax_idle_var + 2,"anm/heavyspider01/r_idle_var3.anm")

	Anim(a_alert_idle + 0, "anm/heavyspider01/a_idle.anm")
	Anim(a_alert_idle_var + 0, "anm/heavyspider01/a_idle_var1.anm")
	Anim(a_alert_idle_var + 1, "anm/heavyspider01/a_idle_var2.anm")
	Anim(a_alert_idle_var + 2, "anm/heavyspider01/a_idle_var3.anm")
	Anim(a_alert_idle_var + 3, "anm/heavyspider01/a_idle_var4.anm")

	Anim(a_combat_idle + 0, "anm/heavyspider01/c_idle.anm")
	//Anim(a_combat_idle_var + 0, "anm/heavyspider01/a_idle_var1.anm")

	Anim(a_fight_idle_var + 0, "anm/heavyspider01/a_idle_var1.anm")
	Anim(a_spot_idle_var + 0, "anm/heavyspider01/a_idle_var2.anm")
	Anim(a_lost_idle_var + 0, "anm/heavyspider01/a_idle_var3.anm")
	
	Anim(a_relax_walk + 0,"anm/heavyspider01/r_walk.anm")
	Anim(a_alert_walk + 0,"anm/heavyspider01/a_walk.anm")
	Anim(a_combat_walk + 0,"anm/heavyspider01/c_walk.anm")

	Anim(a_relax_bwalk + 0,"anm/heavyspider01/r_backwalk.anm")
	Anim(a_alert_bwalk + 0,"anm/heavyspider01/a_backwalk.anm")
	Anim(a_combat_bwalk + 0,"anm/heavyspider01/c_backwalk.anm")

	Anim(a_relax_run + 0,"anm/heavyspider01/r_walk.anm")
	Anim(a_alert_run + 0,"anm/heavyspider01/a_walk.anm")
	Anim(a_combat_run + 0,"anm/heavyspider01/c_walk.anm")
	Anim(a_combat_run_aim + 0,"anm/heavyspider01/c_walk.anm")

	Anim(a_relax_strafe + 0,"anm/heavyspider01/r_strafel.anm")
	Anim(a_relax_strafe + 1,"anm/heavyspider01/r_strafer.anm")
	Anim(a_alert_strafe + 0,"anm/heavyspider01/a_strafel.anm")
	Anim(a_alert_strafe + 1,"anm/heavyspider01/a_strafer.anm")
	Anim(a_combat_strafe + 0,"anm/heavyspider01/c_strafel.anm")
	Anim(a_combat_strafe + 1,"anm/heavyspider01/c_strafer.anm")

	Anim(a_relax_turn + 0,"anm/heavyspider01/r_rotate90l.anm")
	Anim(a_relax_turn + 1,"anm/heavyspider01/r_rotate90r.anm")
	Anim(a_alert_turn + 0,"anm/heavyspider01/a_rotate90l.anm")
	Anim(a_alert_turn + 1,"anm/heavyspider01/a_rotate90r.anm")
	Anim(a_combat_turn + 0,"anm/heavyspider01/c_rotate90l.anm")
	Anim(a_combat_turn + 1,"anm/heavyspider01/c_rotate90r.anm")

//-----------------------------------------------------------------------
	BeginAnimSet("Dron")
//-----------------------------------------------------------------------
const vector mvec = "100 0 0"
const vector mvec2 = "40 0 0"
const vector mvec3 = "0 40 0"

	ApplyAnimFlags(ASF_AIMING)
	Anim(a_relax_idle + 0,"anm/dron01/r_idle.anm")
	Anim(a_relax_idle_var + 0, "anm/dron01/r_idlevar_1.anm")

	Anim(a_alert_idle + 0, "anm/dron01/a_idle.anm")
	Anim(a_alert_idle_var + 0, "anm/dron01/a_idlevar_1.anm")
	Anim(a_alert_idle_var + 1, "anm/dron01/a_idlevar_2.anm")
	Anim(a_alert_idle_var + 2, "anm/dron01/a_idlevar_3.anm")

	Anim(a_combat_idle + 0, "anm/dron01/c_idle.anm")
	Anim(a_combat_idle_var + 0, "anm/dron01/c_idlevar_1.anm")
	
	Anim(a_fight_idle_var + 0, "anm/dron01/c_idlevar_1.anm")
	Anim(a_spot_idle_var + 0, "anm/dron01/a_idlevar_2.anm")
	Anim(a_lost_idle_var + 0, "anm/dron01/a_idlevar_1.anm")
	
	Anim(a_relax_walk + 0,"anm/dron01/r_walk.anm", 30, mvec2)
	Anim(a_alert_walk + 0,"anm/dron01/a_walk.anm", 30, mvec2)
	Anim(a_combat_walk + 0,"anm/dron01/c_walk.anm",30, mvec2)
	
	Anim(a_relax_bwalk + 0,"anm/dron01/r_walk.anm", 30, -mvec2)
	Anim(a_alert_bwalk + 0,"anm/dron01/a_walk.anm", 30, -mvec2)
	Anim(a_combat_bwalk + 0,"anm/dron01/c_walk.anm", 30, -mvec2)

	Anim(a_relax_run + 0,"anm/dron01/r_walk.anm", 30, mvec)
	Anim(a_alert_run + 0,"anm/dron01/a_walk.anm", 30, mvec)
	Anim(a_combat_run + 0,"anm/dron01/c_walk.anm", 30, mvec)
	Anim(a_combat_run_aim + 0,"anm/dron01/c_walk.anm", 30, mvec)

	Anim(a_relax_strafe + 0,"anm/dron01/r_walk.anm", 30, mvec3)
	Anim(a_relax_strafe + 1,"anm/dron01/r_walk.anm", 30, -mvec3)
	Anim(a_alert_strafe + 0,"anm/dron01/a_walk.anm", 30, mvec3)
	Anim(a_alert_strafe + 1,"anm/dron01/a_walk.anm", 30, -mvec3)
	Anim(a_combat_strafe + 0,"anm/dron01/c_walk.anm",30, mvec3)
	Anim(a_combat_strafe + 1,"anm/dron01/c_walk.anm",30, -mvec3)

	Anim(a_pain + 0,"anm/dron01/pain_01.anm", 30, "-100 0 0")
	Anim(a_pain + 1,"anm/dron01/pain_02.anm", 30, "-100 0 0")
	Anim(a_pain + 2,"anm/dron01/pain_03.anm", 30, "-100 0 0")
	Anim(a_pain + 3,"anm/dron01/pain_04.anm", 30, "-100 0 0")
	Anim(a_pain_inplace + 0,"anm/dron01/pain_05.anm")
	Anim(a_pain_inplace + 1,"anm/dron01/pain_06.anm")
	Anim(a_pain_inplace + 0,"anm/dron01/pain_05.anm")
	Anim(a_pain_inplace + 1,"anm/dron01/pain_06.anm")

//-----------------------------------------------------------------------
BeginAnimSet("HumanoidNPC")
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/civil_pistol/R_Idle.anm")
	Anim(a_relax_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_relax_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_relax_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_alert_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_alert_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_alert_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_alert_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_combat_idle + 0, "anm/CombatTest/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_spot_idle_var + 0, "anm/combattest/c_disturb_l.anm")
	Anim(a_spot_idle_var + 1, "anm/combattest/c_disturb_r.anm")
	
	Anim(a_relax_walk, "anm/CombatTest/R_WalkF.anm")
	Anim(a_alert_walk, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_walk, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_bwalk, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/CombatTest/R_RunF.anm")
	Anim(a_alert_run, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/CombatTest/C_RunF.anm")
	Anim(a_combat_run_aim, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_turn + 0,"anm/player/r_rotate90l.anm")
	Anim(a_relax_turn + 1,"anm/player/r_rotate90r.anm")
	Anim(a_alert_turn + 0,"anm/player/r_rotate90l.anm")
	Anim(a_alert_turn + 1,"anm/player/r_rotate90r.anm")
	Anim(a_combat_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/CombatTest/C_Crouch_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/CombatTest/C_Crouch_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/CombatTest/C_Crouch_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch_fire + 0, "anm/CombatTest/C_Crouch_fire_1h.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch_fire + 1, "anm/CombatTest/C_Crouch_fire_2h.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_act_left_go, "anm/act/Act_RifleC_Commands_LeftGo.anm")
	Anim(a_act_right_go, "anm/act/Act_RifleC_Commands_RightGo.anm")
	Anim(a_act_back_go, "anm/act/Act_RifleC_Commands_BackGo.anm")
	Anim(a_act_forward_go, "anm/act/Act_RifleC_Commands_AllGo02.anm")
	Anim(a_act_hold, "anm/act/Act_RifleC_Commands_Hold.anm")
	Anim(a_act_look_left, "anm/act/Act_RifleC_Look_Left.anm")
	Anim(a_act_look_right, "anm/act/Act_RifleC_Look_Right.anm")

//-----------------------------------------------------------------------
	BeginAnimSet("ServiceBot")
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0,"anm/humanoidTest/r_idle.anm")
	Anim(a_alert_idle + 0, "anm/humanoidTest/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle + 0, "anm/humanoidTest/c_idle.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_walk + 0,"anm/humanoidTest/r_walk.anm")
	Anim(a_alert_walk + 0,"anm/humanoidTest/c_walk.anm")
	Anim(a_combat_walk + 0,"anm/humanoidTest/c_walk.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk + 0,"anm/humanoidTest/r_backwalk.anm")
	Anim(a_alert_bwalk + 0,"anm/humanoidTest/c_backwalk.anm")
	Anim(a_combat_bwalk + 0,"anm/humanoidTest/c_backwalk.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run + 0,"anm/humanoidTest/r_run.anm")
	Anim(a_alert_run + 0,"anm/humanoidTest/a_run.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run + 0,"anm/humanoidTest/c_run.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_aim + 0,"anm/humanoidTest/c_run.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/humanoidTest/c_strafel.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/humanoidTest/c_strafer.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/humanoidTest/c_strafel.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/humanoidTest/c_strafer.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/humanoidTest/c_strafel.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/humanoidTest/c_strafer.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_alert_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/humanoidtest/c_crouch_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/humanoidtest/c_crouch_down.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/humanoidtest/c_crouch_up.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain + 0,"anm/humanoidtest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 1,"anm/humanoidtest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 2,"anm/humanoidtest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 3,"anm/humanoidtest/c_pain01.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain_back + 0,"anm/humanoidtest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 1,"anm/humanoidtest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 2,"anm/humanoidtest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 3,"anm/humanoidtest/c_pain02.anm")
		AnimFlags(ASF_AIMING)

//-----------------------------------------------------------------------
	BeginAnimSet("SecuritySpider")
//-----------------------------------------------------------------------
	ApplyAnimFlags(ASF_AIMING)
	Anim(a_relax_idle + 0,"anm/securityspider01/idle.anm")
	Anim(a_relax_idle_var + 0,"anm/securityspider01/idle_var1.anm")
	Anim(a_relax_idle_var + 1,"anm/securityspider01/idle_var2.anm")

	Anim(a_alert_idle + 0, "anm/securityspider01/idle.anm")
	Anim(a_alert_idle_var + 0, "anm/securityspider01/idle_var1.anm")
	Anim(a_alert_idle_var + 1, "anm/securityspider01/idle_var2.anm")

	Anim(a_combat_idle + 0, "anm/securityspider01/idle.anm")

	Anim(a_fight_idle_var + 0, "anm/securityspider01/idle_var1.anm")
	Anim(a_spot_idle_var + 0, "anm/securityspider01/idle_var2.anm")
	Anim(a_lost_idle_var + 0, "anm/securityspider01/idle_var1.anm")
	
	Anim(a_relax_walk + 0,"anm/securityspider01/r_walk.anm")
	Anim(a_alert_walk + 0,"anm/securityspider01/c_walk.anm")
	Anim(a_combat_walk + 0,"anm/securityspider01/c_walk.anm")

	Anim(a_relax_bwalk + 0,"anm/securityspider01/r_backwalk.anm")
	Anim(a_alert_bwalk + 0,"anm/securityspider01/c_backwalk.anm")
	Anim(a_combat_bwalk + 0,"anm/securityspider01/c_backwalk.anm")

	Anim(a_relax_run + 0,"anm/securityspider01/r_walk.anm")
	Anim(a_alert_run + 0,"anm/securityspider01/c_walk.anm")
	Anim(a_combat_run + 0,"anm/securityspider01/c_walk.anm")
	Anim(a_combat_run_aim + 0,"anm/securityspider01/c_walk.anm")

	Anim(a_relax_strafe + 0,"anm/securityspider01/c_strafel.anm")
	Anim(a_relax_strafe + 1,"anm/securityspider01/c_strafer.anm")
	Anim(a_alert_strafe + 0,"anm/securityspider01/c_strafel.anm")
	Anim(a_alert_strafe + 1,"anm/securityspider01/c_strafer.anm")
	Anim(a_combat_strafe + 0,"anm/securityspider01/c_strafel.anm")
	Anim(a_combat_strafe + 1,"anm/securityspider01/c_strafer.anm")

	Anim(a_relax_turn + 0,"anm/securityspider01/c_rotate90l.anm")
	Anim(a_relax_turn + 1,"anm/securityspider01/c_rotate90r.anm")
	Anim(a_alert_turn + 0,"anm/securityspider01/c_rotate90l.anm")
	Anim(a_alert_turn + 1,"anm/securityspider01/c_rotate90r.anm")
	Anim(a_combat_turn + 0,"anm/securityspider01/c_rotate90l.anm")
	Anim(a_combat_turn + 1,"anm/securityspider01/c_rotate90r.anm")

//-----------------------------------------------------------------------
//-------- MACHINE GUN --------------------------------------------------
	BeginAnimSet("LightTrooper")	
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_relax_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_relax_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_relax_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_alert_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_alert_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_alert_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_alert_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_combat_idle + 0, "anm/CombatTest/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle_var + 0,"anm/CombatTest/C_Idle_Var_01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle_var + 1,"anm/CombatTest/C_Idle_Var_02.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_spot_idle_var + 0, "anm/combattest/c_disturb_l.anm")
	Anim(a_spot_idle_var + 1, "anm/combattest/c_disturb_r.anm")

	Anim(a_spot_idle_var + 2, "anm/act/Act_Rifle_RelaxToCombatAim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_walk, "anm/CombatTest/R_WalkF.anm")
	Anim(a_alert_walk, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_walk, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_bwalk, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/CombatTest/R_RunF.anm")
	Anim(a_alert_run, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/CombatTest/C_RunF.anm")
	Anim(a_combat_run_aim, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_brun, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_brun, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_alert_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/CombatTest/C_Crouch_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/CombatTest/C_Crouch_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/CombatTest/C_Crouch_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch_fire + 0, "anm/CombatTest/C_Crouch_fire_1h.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch_fire + 1, "anm/CombatTest/C_Crouch_fire_2h.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_run_fire + 0, "anm/CombatTest/C_RunF_BAim_Panic.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_fire + 1, "anm/CombatTest/C_RunF_LAim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_fire + 2, "anm/CombatTest/C_RunF_RAim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain + 0,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 1,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 2,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 3,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain_back + 0,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 1,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 2,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 3, "anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)

//NEW
	Anim(a_jump_drop, "anm/CombatTest/C_Railing_jump_drop.anm")
	Anim(a_combat_railjump, "anm/CombatTest/C_Railing_jump.anm")
	Anim(a_combat_windowjump, "anm/CombatTest/C_window_Roll.anm")
	Anim(a_combat_crawl, "anm/act/Act_RifleC_PodlejzaniDveri.anm")
	
	Anim(a_combat_llean + 0, "anm/CombatTest/C_LeanL_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 1, "anm/CombatTest/C_LeanL_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 2, "anm/CombatTest/C_LeanL_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 0, "anm/CombatTest/C_LeanR_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 1, "anm/CombatTest/C_LeanR_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 2, "anm/CombatTest/C_LeanR_Out.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_grenade + 0, "anm/CombatTest/C_Grenade.anm")
	Anim(a_combat_grenade + 1, "anm/CombatTest/C_Grenade_L.anm")
	Anim(a_combat_grenade + 2, "anm/CombatTest/C_Grenade_R.anm")
	Anim(a_combat_grenade + 3, "anm/CombatTest/C_Grenade_Down.anm")

	Anim(a_combat_grenade_crouch + 0, "anm/CombatTest/C_Crouch_Grenade_Up.anm")
	Anim(a_combat_grenade_crouch + 1, "anm/CombatTest/C_Crouch_Grenade_L.anm")
	Anim(a_combat_grenade_crouch + 2, "anm/CombatTest/C_Crouch_Grenade_R.anm")

	Anim(a_combat_closecombat + 0, "anm/CombatTest/C_CloseHit_1.anm")
	Anim(a_combat_closecombat + 1, "anm/CombatTest/C_CloseHit_4.anm")
	Anim(a_combat_closecombat + 2, "anm/CombatTest/C_CloseHit_5.anm")

	Anim(a_act_left_go, "anm/act/Act_RifleC_Commands_LeftGo.anm")
	Anim(a_act_right_go, "anm/act/Act_RifleC_Commands_RightGo.anm")
	Anim(a_act_back_go, "anm/act/Act_RifleC_Commands_BackGo.anm")
	Anim(a_act_forward_go, "anm/act/Act_RifleC_Commands_AllGo02.anm")
	Anim(a_act_hold, "anm/act/Act_RifleC_Commands_Hold.anm")
	Anim(a_act_look_left, "anm/act/Act_RifleC_Look_Left.anm")
	Anim(a_act_look_right, "anm/act/Act_RifleC_Look_Right.anm")

//-------- ROCKET LAUNCHER ----------------------------------------------
	BeginAnimSet("HeavyTrooperRL")	
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_relax_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_relax_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_relax_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_alert_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_alert_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_alert_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_alert_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_combat_idle + 0, "anm/CombatTest/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle_var + 0,"anm/CombatTest/C_Idle_Var_01.anm")
	Anim(a_combat_idle_var + 1,"anm/CombatTest/C_Idle_Var_02.anm")
	Anim(a_combat_idle_fire, "anm/CombatTest/c_idle.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_spot_idle_var + 0, "anm/combattest/c_disturb_l.anm")
	Anim(a_spot_idle_var + 1, "anm/combattest/c_disturb_r.anm")

	Anim(a_relax_walk, "anm/CombatTest/R_WalkF.anm")
	Anim(a_alert_walk, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_walk, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_bwalk, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/CombatTest/R_RunF.anm")
	Anim(a_alert_run, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/CombatTest/C_RunF.anm")
	Anim(a_combat_run_aim, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/CombatTest/C_Crouch_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/CombatTest/C_Crouch_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/CombatTest/C_Crouch_Out.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_brun, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_brun, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_alert_turn + 0,"anm/CombatTest/C_Turn180L.anm")
	Anim(a_alert_turn + 1,"anm/CombatTest/C_Turn180R.anm")

	Anim(a_combat_turn + 0,"anm/CombatTest/C_Turn180L.anm")
	Anim(a_combat_turn + 1,"anm/CombatTest/C_Turn180R.anm")

	Anim(a_pain + 0,"anm/CombatTest/c_pain01.anm")
	Anim(a_pain + 1,"anm/CombatTest/c_pain01.anm")
	Anim(a_pain + 2,"anm/CombatTest/c_pain01.anm")
	Anim(a_pain + 3,"anm/CombatTest/c_pain01.anm")

	Anim(a_pain_back + 0,"anm/CombatTest/c_pain02.anm")
	Anim(a_pain_back + 1,"anm/CombatTest/c_pain02.anm")
	Anim(a_pain_back + 2,"anm/CombatTest/c_pain02.anm")
	Anim(a_pain_back + 3, "anm/CombatTest/c_pain02.anm")

//NEW
	Anim(a_jump_drop, "anm/CombatTest/C_Railing_jump_drop.anm")
	Anim(a_combat_railjump, "anm/CombatTest/C_Railing_jump.anm")
	Anim(a_combat_windowjump, "anm/CombatTest/C_window_Roll.anm")
	Anim(a_combat_crawl, "anm/act/Act_RifleC_PodlejzaniDveri.anm")

	Anim(a_combat_llean + 0, "anm/CombatTest/C_LeanL_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 1, "anm/CombatTest/C_LeanL_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 2, "anm/CombatTest/C_LeanL_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 0, "anm/CombatTest/C_LeanR_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 1, "anm/CombatTest/C_LeanR_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 2, "anm/CombatTest/C_LeanR_Out.anm")
		AnimFlags(ASF_AIMING)

//-----------------------------------------------------------------------
	BeginAnimSet("HeavyTrooperFT")	
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/heavytrooperft/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_idle + 0, "anm/heavytrooperft/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle + 0, "anm/heavytrooperft/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle_fire, "anm/heavytrooperft/c_idle_fire.anm")
			AnimFlags(ASF_AIMING)

	Anim(a_relax_walk, "anm/heavytrooperft/C_RunF.anm")
	Anim(a_alert_walk, "anm/heavytrooperft/C_RunF.anm")
	Anim(a_combat_walk, "anm/heavytrooperft/C_RunF.anm")

	Anim(a_relax_bwalk, "anm/heavytrooperft/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_bwalk, "anm/heavytrooperft/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/heavytrooperft/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/heavytrooperft/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_run, "anm/heavytrooperft/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/heavytrooperft/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_aim, "anm/heavytrooperft/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/heavytrooperft/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_brun, "anm/heavytrooperft/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_brun, "anm/heavytrooperft/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/heavytrooperft/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/heavytrooperft/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/heavytrooperft/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/heavytrooperft/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/heavytrooperft/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/heavytrooperft/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_run_fire + 0, "anm/heavytrooperft/C_RunF.anm")
	Anim(a_combat_run_fire + 1, "anm/CombatTest/C_RunF_LAim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_fire + 2, "anm/CombatTest/C_RunF_RAim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_jump_drop, "anm/CombatTest/C_Railing_jump_drop.anm")
	Anim(a_combat_railjump, "anm/CombatTest/C_Railing_jump.anm")
	Anim(a_combat_windowjump, "anm/CombatTest/C_window_Roll.anm")
	Anim(a_combat_crawl, "anm/act/Act_RifleC_PodlejzaniDveri.anm")

	Anim(a_combat_llean + 0, "anm/CombatTest/C_LeanL_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 1, "anm/CombatTest/C_LeanL_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 2, "anm/CombatTest/C_LeanL_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 0, "anm/CombatTest/C_LeanR_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 1, "anm/CombatTest/C_LeanR_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 2, "anm/CombatTest/C_LeanR_Out.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_cover + 0, "anm/HeavyTrooperft/C_Cover_In.anm")
	Anim(a_combat_cover + 1, "anm/HeavyTrooperft/C_Cover_Idle.anm")
	Anim(a_combat_cover + 2, "anm/HeavyTrooperft/C_Cover_Out.anm")

//------------- PISTOL --------------------------------------------------
	BeginAnimSet("Technician_pistol")	
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/civil_pistol/R_Idle.anm")
	Anim(a_alert_idle + 0, "anm/civil_pistol/R_Idle.anm")
	Anim(a_combat_idle + 0, "anm/civil_pistol/c_idle.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_spot_idle_var + 0, "anm/combattest/c_disturb_l.anm")
	Anim(a_spot_idle_var + 1, "anm/combattest/c_disturb_r.anm")

	Anim(a_relax_walk, "anm/civil_pistol/R_WalkF.anm")
	Anim(a_alert_walk, "anm/civil_pistol/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_walk, "anm/civil_pistol/C_RunF.anm")

	Anim(a_relax_bwalk, "anm/civil_pistol/C_2RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_bwalk, "anm/civil_pistol/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/civil_pistol/C_2RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/civil_pistol/C_RunF.anm")
	Anim(a_alert_run, "anm/civil_pistol/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/civil_pistol/C_RunF.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_aim, "anm/civil_pistol/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/civil_pistol/C_2RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_brun, "anm/civil_pistol/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_brun, "anm/civil_pistol/C_2RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/civil_pistol/C_2RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/civil_pistol/C_2RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/civil_pistol/C_2RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/civil_pistol/C_2RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/civil_pistol/C_2RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/civil_pistol/C_2RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/civil_pistol/C_Crouch_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/civil_pistol/C_Crouch_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/civil_pistol/C_Crouch_Out.anm")
		AnimFlags(ASF_AIMING)
	
	Anim(a_combat_run_fire + 0, "anm/civil_pistol/C_RunF.anm")
	Anim(a_combat_run_fire + 1, "anm/civil_pistol/C_RunF_AimL.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_fire + 2, "anm/civil_pistol/C_RunF_AimR.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain + 0,"anm/civil_pistol/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 1,"anm/civil_pistol/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 2,"anm/civil_pistol/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 3,"anm/civil_pistol/c_pain01.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain_back + 0,"anm/civil_pistol/c_pain02.anm")
	Anim(a_pain_back + 1,"anm/civil_pistol/c_pain02.anm")
	Anim(a_pain_back + 2,"anm/civil_pistol/c_pain02.anm")
	Anim(a_pain_back + 3, "anm/civil_pistol/c_pain02.anm")

	Anim(a_jump_drop, "anm/CombatTest/C_Railing_jump_drop.anm")
	Anim(a_combat_railjump, "anm/CombatTest/C_Railing_jump.anm")
	Anim(a_combat_windowjump, "anm/CombatTest/C_window_Roll.anm")
	Anim(a_combat_crawl, "anm/act/act_pistolc_podlejzanidveri.anm")

	Anim(a_combat_llean + 0, "anm/CombatTest/C_LeanL_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 1, "anm/CombatTest/C_LeanL_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 2, "anm/CombatTest/C_LeanL_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 0, "anm/CombatTest/C_LeanR_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 1, "anm/CombatTest/C_LeanR_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 2, "anm/CombatTest/C_LeanR_Out.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_alert_turn + 0,"anm/civil_pistol/C_Turn180L.anm",45)
	Anim(a_alert_turn + 1,"anm/civil_pistol/C_Turn180R.anm",45)

	Anim(a_combat_turn + 0,"anm/civil_pistol/C_Turn180L.anm",45)
	Anim(a_combat_turn + 1,"anm/civil_pistol/C_Turn180R.anm",45)


//-----------------------------------------------------------------------
//-------- SHOT GUN -----------------------------------------------------
	BeginAnimSet("Guard")	
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_relax_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_relax_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_relax_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")
	Anim(a_alert_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_alert_idle_var + 0,"anm/CombatTest/R_Idle_Var01.anm")
	Anim(a_alert_idle_var + 1,"anm/CombatTest/R_Idle_Var02.anm")
	Anim(a_alert_idle_var + 2,"anm/CombatTest/R_Idle_Var03.anm")

	Anim(a_combat_idle + 0, "anm/shotgun/c_idle.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_spot_idle_var + 0, "anm/shotgun/c_disturb_l.anm")
	Anim(a_spot_idle_var + 1, "anm/shotgun/c_disturb_r.anm")

	Anim(a_relax_walk, "anm/CombatTest/R_WalkF.anm")
	Anim(a_alert_walk, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_walk, "anm/shotgun/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_bwalk, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/shotgun/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/CombatTest/R_RunF.anm")
	Anim(a_alert_run, "anm/CombatTest/A_WalkF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/CombatTest/C_RunF.anm")
	Anim(a_combat_run_aim, "anm/shotgun/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_brun, "anm/CombatTest/A_WalkB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_brun, "anm/shotgun/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/shotgun/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/shotgun/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/shotgun/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/shotgun/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/shotgun/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/shotgun/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_alert_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/CombatTest/C_Crouch_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/CombatTest/C_Crouch_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/CombatTest/C_Crouch_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch_fire + 0, "anm/CombatTest/C_Crouch_fire_1h.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch_fire + 1, "anm/CombatTest/C_Crouch_fire_2h.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_run_fire + 0, "anm/CombatTest/C_RunF_BAim_Panic.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_fire + 1, "anm/CombatTest/C_RunF_LAim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run_fire + 2, "anm/CombatTest/C_RunF_RAim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain + 0,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 1,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 2,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 3,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain_back + 0,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 1,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 2,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 3, "anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)

//NEW
	Anim(a_combat_railjump, "anm/CombatTest/C_Railing_jump.anm")
	Anim(a_combat_windowjump, "anm/CombatTest/C_window_Roll.anm")
	Anim(a_jump_drop, "anm/CombatTest/C_Railing_jump_drop.anm")
	Anim(a_combat_crawl, "anm/act/Act_RifleC_PodlejzaniDveri.anm")

	Anim(a_combat_llean + 0, "anm/CombatTest/C_LeanL_In.anm")
	Anim(a_combat_llean + 1, "anm/CombatTest/C_LeanL_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 2, "anm/CombatTest/C_LeanL_Out.anm")
	Anim(a_combat_rlean + 0, "anm/CombatTest/C_LeanR_In.anm")
	Anim(a_combat_rlean + 1, "anm/CombatTest/C_LeanR_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 2, "anm/CombatTest/C_LeanR_Out.anm")

	Anim(a_combat_grenade + 0, "anm/CombatTest/C_Grenade.anm")
	Anim(a_combat_grenade + 1, "anm/CombatTest/C_Grenade_L.anm")
	Anim(a_combat_grenade + 2, "anm/CombatTest/C_Grenade_R.anm")
	Anim(a_combat_grenade + 3, "anm/CombatTest/C_Grenade_Down.anm")

	Anim(a_combat_grenade_crouch + 0, "anm/CombatTest/C_Crouch_Grenade_Up.anm")
	Anim(a_combat_grenade_crouch + 1, "anm/CombatTest/C_Crouch_Grenade_L.anm")
	Anim(a_combat_grenade_crouch + 2, "anm/CombatTest/C_Crouch_Grenade_R.anm")

	Anim(a_combat_closecombat + 0, "anm/CombatTest/C_CloseHit_1.anm")
	Anim(a_combat_closecombat + 1, "anm/CombatTest/C_CloseHit_4.anm")
	Anim(a_combat_closecombat + 2, "anm/CombatTest/C_CloseHit_5.anm")

	Anim(a_act_left_go, "anm/act/Act_RifleC_Commands_LeftGo.anm")
	Anim(a_act_right_go, "anm/act/Act_RifleC_Commands_RightGo.anm")
	Anim(a_act_back_go, "anm/act/Act_RifleC_Commands_BackGo.anm")
	Anim(a_act_forward_go, "anm/act/Act_RifleC_Commands_AllGo02.anm")
	Anim(a_act_hold, "anm/act/Act_RifleC_Commands_Hold.anm")
	Anim(a_act_look_left, "anm/act/Act_RifleC_Look_Left.anm")
	Anim(a_act_look_right, "anm/act/Act_RifleC_Look_Right.anm")

//-----------------------------------------------------------------------
BeginAnimSet("Player")
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/player/hackIdle.anm")

	Anim(a_combat_idle + 0, "anm/CombatTest/c_idle.anm")
	Anim(a_combat_idle + 1, "anm/civil_pistol/c_idle.anm")

	Anim(a_combat_walk, "anm/CombatTest/C_RunF_Aim.anm")
//	Anim(a_combat_walk+1, "anm/CombatTest/C_RunF_Aim.anm")
	Anim(a_combat_walk + 1, "anm/civil_pistol/C_RunF_Aim.anm")

	Anim(a_combat_bwalk, "anm/CombatTest/C_RunB_Aim.anm")
//	Anim(a_combat_bwalk+1, "anm/CombatTest/C_RunB_Aim.anm")
	Anim(a_combat_bwalk + 1, "anm/civil_pistol/C_RunB_Aim.anm")
 
	Anim(a_combat_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
	Anim(a_combat_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")

	Anim(a_crouch + 0, "anm/CombatTest/C_Crouch_Idle.anm")
	Anim(a_crouch + 1, "anm/CombatTest/C_Crouch_In.anm")
	Anim(a_crouch + 2, "anm/CombatTest/C_Crouch_Out.anm")
                              
                              
//-----------------------------------------------------------------------
//-------- MACHINE GUN --------------------------------------------------
	BeginAnimSet("SecurityBot")	//servicebot cervenej - combat
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/CombatTest/R_Idle.anm")
	Anim(a_alert_idle + 0, "anm/CombatTest/c_idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle + 0, "anm/CombatTest/c_idle.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_spot_idle_var + 0, "anm/combattest/c_disturb_l.anm")
	Anim(a_spot_idle_var + 1, "anm/combattest/c_disturb_r.anm")

	Anim(a_relax_walk, "anm/CombatTest/R_WalkF.anm")
	Anim(a_alert_walk, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_walk, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_bwalk, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_bwalk, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/CombatTest/R_RunF.anm")
	Anim(a_alert_run, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_run, "anm/CombatTest/C_RunF.anm")
	Anim(a_combat_run_aim, "anm/CombatTest/C_RunF_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/CombatTest/R_WalkB.anm")
	Anim(a_alert_brun, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_brun, "anm/CombatTest/C_RunB_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/CombatTest/C_RunL_Aim.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/CombatTest/C_RunR_Aim.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_alert_turn + 0,"anm/CombatTest/C_Turn180L.anm")
	Anim(a_alert_turn + 1,"anm/CombatTest/C_Turn180R.anm")

	Anim(a_combat_turn + 0,"anm/CombatTest/C_Turn180L.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_turn + 1,"anm/CombatTest/C_Turn180R.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_crouch + 0, "anm/CombatTest/C_Crouch_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 1, "anm/CombatTest/C_Crouch_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_crouch + 2, "anm/CombatTest/C_Crouch_Out.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain + 0,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 1,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 2,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain + 3,"anm/CombatTest/c_pain01.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_pain_back + 0,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 1,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 2,"anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_pain_back + 3, "anm/CombatTest/c_pain02.anm")
		AnimFlags(ASF_AIMING)

//NEW
	Anim(a_jump_drop, "anm/CombatTest/C_Railing_jump_drop.anm")
	Anim(a_combat_railjump, "anm/CombatTest/C_Railing_jump.anm")
	Anim(a_combat_windowjump, "anm/CombatTest/C_window_Roll.anm")
	Anim(a_combat_crawl, "anm/act/Act_RifleC_PodlejzaniDveri.anm")
	
	Anim(a_combat_llean + 0, "anm/CombatTest/C_LeanL_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 1, "anm/CombatTest/C_LeanL_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_llean + 2, "anm/CombatTest/C_LeanL_Out.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 0, "anm/CombatTest/C_LeanR_In.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 1, "anm/CombatTest/C_LeanR_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_rlean + 2, "anm/CombatTest/C_LeanR_Out.anm")
		AnimFlags(ASF_AIMING)

//-------- SHOT GUN -----------------------------------------------------
	BeginAnimSet("Boss")	
//-----------------------------------------------------------------------
	Anim(a_relax_idle + 0, "anm/boss/C_Idle.anm")
	Anim(a_relax_idle_var + 0,"anm/boss/C_Idle_Var1.anm")
	Anim(a_relax_idle_var + 1,"anm/boss/C_Idle_Var1.anm")
	Anim(a_relax_idle_var + 2,"anm/boss/C_Idle_Var1.anm")

	Anim(a_alert_idle + 0, "anm/boss/C_Idle.anm")
	Anim(a_alert_idle_var + 0,"anm/boss/C_Idle_Var1.anm")
	Anim(a_alert_idle_var + 1,"anm/boss/C_Idle_Var1.anm")
	Anim(a_alert_idle_var + 2,"anm/boss/C_Idle_Var1.anm")

	Anim(a_combat_idle + 0, "anm/boss/C_Idle.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_idle_var + 0,"anm/boss/C_Idle_Var1.anm")
	Anim(a_combat_idle_var + 1, "anm/boss/C_Fire_secondary.anm")

	Anim(a_relax_walk, "anm/boss/C_WalkF.anm")
	Anim(a_alert_walk, "anm/boss/C_WalkF.anm")
	Anim(a_combat_walk, "anm/boss/C_WalkF.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_bwalk, "anm/boss/C_WalkB.anm")
	Anim(a_alert_bwalk, "anm/boss/C_WalkB.anm")
	Anim(a_combat_bwalk, "anm/boss/C_WalkB.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_run, "anm/boss/C_WalkF.anm")
	Anim(a_alert_run, "anm/boss/C_WalkF.anm")
	Anim(a_combat_run, "anm/boss/C_WalkF.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_combat_run_aim, "anm/boss/C_WalkF.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_brun, "anm/boss/C_WalkB.anm")
	Anim(a_alert_brun, "anm/boss/C_WalkB.anm")
	Anim(a_combat_brun, "anm/boss/C_WalkB.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_strafe + 0,"anm/boss/C_WalkL.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_relax_strafe + 1,"anm/boss/C_WalkR.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 0,"anm/boss/C_WalkL.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_alert_strafe + 1,"anm/boss/C_WalkR.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 0,"anm/boss/C_WalkL.anm")
		AnimFlags(ASF_AIMING)
	Anim(a_combat_strafe + 1,"anm/boss/C_WalkR.anm")
		AnimFlags(ASF_AIMING)

	Anim(a_relax_turn + 0,"anm/boss/C_Turn180L.anm")
	Anim(a_relax_turn + 1,"anm/boss/C_Turn180R.anm")

	Anim(a_alert_turn + 0,"anm/boss/C_Turn180L.anm")
	Anim(a_alert_turn + 1,"anm/boss/C_Turn180R.anm")

	Anim(a_combat_turn + 0,"anm/boss/C_Turn180L.anm")
	Anim(a_combat_turn + 1,"anm/boss/C_Turn180R.anm")

/*
	Anim(a_combat_run_fire + 0, "anm/boss/C_RunF_BAim_Panic.anm")
	Anim(a_combat_run_fire + 1, "anm/boss/C_RunF_LAim.anm")
	Anim(a_combat_run_fire + 2, "anm/boss/C_RunF_RAim.anm")
*/
	Anim(a_combat_closecombat + 0, "anm/boss/C_Fire_secondary.anm")
	Anim(a_combat_closecombat + 1, "anm/boss/C_Fire_secondary.anm")
	Anim(a_combat_closecombat + 2, "anm/boss/C_Fire_secondary.anm")

}
