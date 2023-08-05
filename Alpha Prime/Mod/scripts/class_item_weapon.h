void ShotEffects(ShotStorage stor, MinClass target, vector pos, vector norm, int sparm);

const int WP_IDLE		= 1
const int WP_RUN		= 2
const int WP_SPRINT	= 3
const int WP_RELOAD	= 4
const int WP_CROUCH	= 5

//================================================================
class ShotStorageWeapon extends ShotStorage
{
	int Shoot(ExtClass other)
	{
		if(IsInherited(other, CExtClass) == false)
			return SR_NONE
/*
		if(other.ClassType == ClassTypeCreature)
		{
			ExtClass ext = shooter
			CreatureClass cc = ext.Owner
			damage *= DmgTab[shooter.ClassNum][other.ClassNum]
		}
*/
		return other.Shoot(this)
	}
}

//-----------------------------------------------------------------
	void ShotBullets(ShotStorage parms, func tracefilter)
	{
		vector To = parms.to
		int content, sparm
		ExtClass other
		float plane[4]

		parms.ID = World.ShootID++

		for(int n = 0; n < parms.NumBullets; n++)
		{
			parms.to = To	//parms.to sa musi v kazdom cykle zmenit ale tu ho stale navratime na povodnu hodnotu

			if(parms.precision != 0)
			{
				float maxerr = VectorLength(parms.to - parms.from) * parms.precision

				//presnost musi byt "narusena" v rovine kolme na smer strely
				vector right = parms.dir * UpVec
				vector up = right * parms.dir

				right = right * frand(-maxerr, maxerr)
				up = up * frand(-maxerr, maxerr)

				parms.to = parms.to + up + right
				parms.dir = parms.to - parms.from
				VectorNormalize(parms.dir)
			}

			if(TraceLineEx(DEFAULT, parms.from, parms.to, ZeroVec, ZeroVec, other, plane, content, sparm, TRACE_RAGDOLLS|TRACE_DEFAULT|TRACE_BONES, tracefilter) >= 1)
				continue

#ifdef WEAPONDEBUG
			AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, parms.from, parms.to)
#endif
	
//			int impact = SURF_NOMARKS | SURF_NOIMPACT & sparm
			int impact = SURF_NOIMPACT & sparm

			if(impact != 0)
				continue

			if(g_iTraceBone >= 64)
				parms.Bone = -1
			else
				parms.Bone = g_iTraceBone

/*			if(parms.shooter != World.Player)
			{
				float TraceDist = VectorLength(parms.to - parms.from)
				float DamageDistScale = TraceDist / 509
				clamp DamageDistScale<0, 1>
				DamageDistScale = 1 - DamageDistScale

				float OrginalDamage = parms.damage		//musime zapametat co tam bolo pretoze sa to tam musi po zavolani Shoot() vratit
				float DistanceDamage = OrginalDamage * 3 * DamageDistScale
				parms.damage *= 3
//				parms.damage += DistanceDamage
			}*/

/*			float OrginalDamage = parms.damage
			
			if(parms.shooter != World.Player)
				parms.damage *= 2*/

			if(parms.Shoot(other) == SR_HIT)
			{
				vector norm = Vector(plane[0], plane[1], plane[2])

				if(strstr(g_strTraceShader, "pipe") != -1 )
				{
					misc_particle_effector eff = new misc_particle_effector
					eff.SetParms("particle/steam_smoke.ptc", ZeroVec, true, 2)
					vector mat[4]
					vector vec1, vec2
					vec1[0] = norm[1]
					vec1[1] = norm[2]
					vec1[2] = norm[0]
					vec2 = norm * vec1
					VectorNormalize(vec2)
					vec1 = vec2 * norm

					mat[0] = vec1
					mat[1] = vec2
					mat[2] = norm
					mat[3] = norm * 0.2 + parms.to
					SetFlags(eff, TFL_FULLMATRIX)
					SetMatrix(eff, mat)
				}
                                
				if(other.ClassType == ClassTypeWorld || IsInherited(other, CRigidBodyClass) || IsInherited(other, CCreatureClass))
				{
					//shooter is always ClassTypeCreature
					if(parms.shooter.ClassNum == ClassNumPlayer)
					{
						NotifyNoises(parms.shooter, parms.to, NOISERANGE_PHYSICS, NOISE_SHOOT)
					}

					ShotEffects(parms, other, parms.to, norm, sparm)
				}
			}

//			if(parms.shooter != World.Player)
//				parms.damage = OrginalDamage
		}
	}

//-----------------------------------------------------------------
bool Projectile::CreateBody()
{
	size *= 0.5
	vector Mins = Vector(-size, -size, -size)
	vector Maxs = Vector(size, size, size)
	SetBoundBox(this, Mins, Maxs)		//podla bboxu sa vytvara RB
	SetMass(5)
	return CreateRigidBody(GEOMTYPE_SPHERE, "material/default", model)
}

//-----------------------------------------------------------------
void Projectile::Fire(ShotStorage ShotStor)
{
	SStorage = ShotStor
	damage = ShotStor.damage
	from = ShotStor.from
	target = ShotStor.to
	dir = target - from
	length = VectorNormalize(dir)

	vector Vec2
	Vec2[0] = dir[1]
	Vec2[1] = dir[2]
	Vec2[2] = dir[0]
	vector Vec3 = dir * Vec2

	vector mat[4]
	mat[0] = dir
	mat[1] = Vec2
	mat[2] = Vec3
	mat[3] = from
	SetTransform(mat)
	CreateBody()

	BeforeFire()
	ActivateBody(true)
	dBodyApplyImpulse(this, dir * speed)
	SetEventMask(this, EV_FRAME)
}

//-----------------------------------------------------------------
void Projectile::Projectile(int version)
{
	ModifyParms(version)		//zmeny kedy staci zmenit parametre nastavime tu (inac spravit novy class!)
	
	if(model)
		MainObject = GetObject(model)
		
	if(MainObject)
	{
		SelectObject(this, MainObject)
		Show(true)
	}

	InitStorage()
}

//================================================================================
class ProjectileRocket extends Projectile
{
	float timer
	int LightHandle
	float ImpactScale
	misc_effect_glow Corona
	misc_particle_effector smoke
	float FromStart
	MinClass LightStorage
	int ExplosionLook

//-----------------------------------------------------------------
	void BeforeFire()
	{
		timer = 200
		speed = 4000 * ImpactScale

		if(model == "obj/weapons/grenade.xob")
			SetObject(MainObject, "$remap 'obj/weapons/grenade' 'obj/weapons/grenade_NoFPS';$remap 'obj/weapons/grenadeLights' 'obj/weapons/grenadeLights_NoFPS';")
	}

//-----------------------------------------------------------------
	void Fire(ShotStorage ShotStor)
	{
		SStorage = ShotStor
		damage = ShotStor.damage
		from = ShotStor.from
		target = ShotStor.to
		dir = target - from
		length = VectorNormalize(dir)

		vector Vec2
		Vec2[0] = dir[1]
		Vec2[1] = dir[2]
		Vec2[2] = dir[0]
		vector Vec3 = dir * Vec2

		vector mat[4]
		mat[0] = Vec3
		mat[1] = dir
		mat[2] = Vec2
		mat[3] = from
		SetTransform(mat)
		CreateBody()

		BeforeFire()

		vector Gravity = dir * 10
		dBodySetGravity(this, Gravity)
		ActivateBody(true)
		dBodyApplyImpulse(this, Gravity * 500)
		SetEventMask(this, EV_CONTACT)
		
		//FIX!
		//dBodyInteractionLayer(this, ET_WORLD|ET_DEFAULT|ET_PLAYER|ET_NPC|ET_DOOR)
		
		if(this.IsRigidBody() && ShotStor.shooter.IsRigidBody())
		{
			ColBlockOtherBody = ShotStor.shooter
			block = dBodyCollisionBlock(this, ColBlockOtherBody)
			
			if(ShotStor.shooter.ClassType == ClassTypeCreature)	//mohol by to byt este turret
				ColBlockOtherBody.AddProjectileDependence(this)
			else
				AddToParent(ColBlockOtherBody)	//sme zavisli na tejto entite
		}

		smoke.Switch(true)
	}

//-----------------------------------------------------------------
	bool CreateBody()
	{
		SetMass(5)
		bool body = CreateRigidBody(GEOMTYPE_NONE, "", model)
		return body
	}

//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		vector origin = GetMatrix(this, 3)
		
		if(!LightHandle)
			LightHandle = AddLight(LightStorage, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 0, 128, "255 100 0")

		Show(false)

		if(smoke)
			smoke.Switch(false)

		delete Corona
		Corona = NULL

		timer = 25

		ClearEventMask(this, EV_CONTACT)
		SetEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		timer -= ftime * 100
		vector LightOff = GetMatrix(this, 0) * -30
		SetOrigin(LightStorage, GetMatrix(this, 3) + LightOff)

		if(status == 0)
		{
			RemoveColBlock(true)
			RemoveBodyAndGeom()
			ClearFlags(this, TFL_SOLID)
			World.ApplyExplosion(SStorage.shooter, GetMatrix(this, 3), 128, 300, damage, ExplosionLook)
			PlaySound(this, Sound1, SF_ONCE)
			status = 1
		}

		float rate
		vector color

		if(status == 1)
		{
			rate = timer / 25
			rate = 1 - rate
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 128, color)
			
			if(timer > 0)
				return

			status = 2
			timer = 150
		}
		
		if(status == 2)
		{
			rate = timer / 150
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 128, color)

			if(timer <= 0)
			{
				RemoveLight(LightHandle)
				LightHandle = 0
				timer = 100
				status = 3
			}
		}
		
		if(status == 3)	//musime este pockat az dohraje zvuk
		{
			if(timer <= 0)
				delete this
		}
	}

//-----------------------------------------------------------------
	void InitStorage()
	{
		LightStorage = new MinClass()
		Sound1 = GetSoundEx("Weap_Grenade_expl", 1, SF_3D)
		Corona = new misc_effect_glow()
		Corona.maxalpha = 1.0
//		Corona.minalpha = 1.0
//		Corona.flags |= 1	//vis test
		Corona.SetParms(ZeroVec, 15, "sprites/corona/flarered1.spr")
  		AddChild(this, Corona, 0)
 		
		smoke = new misc_particle_effector()
		smoke.SetParms("particle/rocket_smoke.ptc", ZeroVec, true, 1)
		AddChild(this, smoke, 2)
		SetAngles(smoke, "90 0 0")

		ClearEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void ModifyParms(int version)
	{
		switch(version)
		{
		case 1:
			model = "obj/physics/tachyonsphere.xob"
			size = 2
			ExplosionLook = 1
			break

		case 2:	//playerov raketomet
			model = "obj/weapons/rocket.xob"
			size = 2
			ExplosionLook = 1
			break
		case 3:	//heavy turret
			model = "obj/weapons/rocket.xob"
			size = 2
			ExplosionLook = 3
			break
		}
	}	

//-----------------------------------------------------------------
	void ProjectileRocket(int version)
	{
	}

//----------------------------------------------------------------------------
	void OnParentRemove()
	{
		RemoveColBlock(true)
	}

//-----------------------------------------------------------------
	void ~ProjectileRocket()
	{
		RemoveColBlock(true)

		if(Corona)
		{
			delete Corona
			Corona = NULL
		}
			
		if(smoke)
		{
			delete smoke
			smoke = NULL	
		}

		if(LightStorage)
		{
			delete LightStorage
			LightStorage = NULL	
		}

		RemoveBodyAndGeom()

		if(MainObject)
			ReleaseObject(MainObject, 0)
		
		RemoveFromParent()
	}
}



//================================================================================
class ProjectileTachyonGrenade extends Projectile
{
	dlight_dynamic DiscoLight
	int LightHandle
	misc_effect_glow Corona
	float timer

//---------------------------------------------------------------------------------
	int Shoot(ShotStorage shot)	//v superclasse sa limituje sila aby to nemalo velke rychlosti
	{
		if(shot.HurtType != HT_SHOT)
			return 0

		if(rb)
		{
			vector vec = shot.dir * shot.power
			dBodyApplyImpulseAt(this, vec, shot.to)
		}
		return 1
	}

//-----------------------------------------------------------------
	void ModifyParms(int version)
	{
		switch(version)
		{
		case 1:
			model = "obj/physics/tachyonsphere.xob"
			size = 2
			break

		case 2:
			model = "obj/weapons/grenade.xob"
			size = 2
			break
		}
	}	

//-----------------------------------------------------------------
	bool CreateBody()
	{
		SetMass(5)
		return CreateRigidBody(GEOMTYPE_NONE, "", model)
		dBodyInteractionLayer(this, ET_GRENADE)
	}

//-----------------------------------------------------------------
	void InitStorage()
	{
		Sound1 = GetSoundEx("Weap_Tachyon_expl", 1, SF_3D)
		Corona = new misc_effect_glow()
		Corona.maxalpha = 1.0
		Corona.flags |= 1	//vis test
		Corona.SetParms(ZeroVec, 30, "sprites/corona/lightblue2.spr")

		DiscoLight = new dlight_dynamic()
		DiscoLight.texture = "textures/lightcube/fluidsmall.dds"
		DiscoLight.intensity = 300
 		DiscoLight.radius = 120
 		DiscoLight._color = "0.211793 0.764687 1.000000"
 		Throw(DiscoLight, this, EV_INIT, 0)
 		DiscoLight.M_SwitchOn(this)
 		UpdateEntity(DiscoLight)
		
		misc_particle_effector effector = new misc_particle_effector()
		effector.SetParms("particle/tachyondust.ptc", ZeroVec, true, 1)
		
		AddChild(this, Corona, 0)
		AddChild(this, DiscoLight, 1)
		AddChild(this, effector, 2)
		
		timer = 400
		speed = 4000
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		timer -= ftime * 100

		if(status == 0)
		{
			if(timer <= 0)
			{
				if(World.ManipEntity == this)
				{
					CreatureClass cc = World.Player
					cc.DropManipEntity(true)
				}

				vector origin = GetMatrix(this, 3)
				DiscoLight.M_SwitchOff(this)	//vyhadzuje svetlo
//				misc_particle_effector explode = new misc_particle_effector()
//				explode.SetParms("particle/tachyonexplode.ptc", origin, true, 2)
				LightHandle = AddLight(this, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 40, 128, "255 100 0")
				Show(false)

				delete Corona
				Corona = NULL
					
				delete DiscoLight
				DiscoLight = NULL

				RemoveBodyAndGeom()
				World.ApplyExplosion(SStorage.shooter, GetMatrix(this, 3), 96, 300, damage, 6)
				PlaySound(this, Sound1, SF_ONCE)
				status = 1
				timer = 25
			}
			return
		}

		float rate
		vector color

		if(status == 1)
		{
			rate = timer / 25
			rate = 1 - rate
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 128, color)
			
			if(timer <= 0)
			{
				status = 2
				timer = 150
			}
		}
		
		if(status == 2)
		{
			rate = timer / 150
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 65, color)

			if(timer <= 0)
			{
				RemoveLight(LightHandle)
				LightHandle = 0
				timer = 100
				status = 4
			}
		}
		
		if(status == 4)	//musime este pockat az dohraje zvuk
		{
			if(timer <= 0)
				delete this
		}
	}

//-----------------------------------------------------------------
	void ProjectileTachyonGrenade(int version)
	{
	}

//-----------------------------------------------------------------
	void ~ProjectileTachyonGrenade()
	{
		if(Corona)
			delete Corona
			
		if(DiscoLight)
			delete DiscoLight

		RemoveBodyAndGeom()

		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}

//================================================================================
class ProjectileGrenade extends Projectile
{
	float timer
	int LightHandle
	float ImpactScale
	dlight_dynamic DiscoLight
	misc_effect_glow Corona
	DynamicOverlaySprite RefractionSprite 
	MinClass LightStorage

//-----------------------------------------------------------------
	void BeforeFire()
	{
		timer = 200
		speed = 4000 * ImpactScale

		if(model == "obj/weapons/grenade.xob")
			SetObject(MainObject, "$remap 'obj/weapons/grenade' 'obj/weapons/grenade_NoFPS';$remap 'obj/weapons/grenadeLights' 'obj/weapons/grenadeLights_NoFPS';")
			
		Corona = new misc_effect_glow()
		Corona.maxalpha = 1.0
//		Corona.minalpha = 1.0
//		Corona.flags |= 1	//vis test
		Corona.SetParms(ZeroVec, 15, "sprites/corona/flarered1.spr")
		AddChild(this, Corona, 0)
	}
	
//---------------------------------------------------------------------------------
	int Shoot(ShotStorage shot)	//v superclasse sa limituje sila aby to nemalo velke rychlosti
	{
		if(shot.HurtType != HT_SHOT)
			return 0

		if(rb)
		{
			vector vec = shot.dir * shot.power
			dBodyApplyImpulseAt(this, vec, shot.to)
		}
		return 1
	}

//-----------------------------------------------------------------
	bool CreateBody()
	{
		SetMass(5)
		bool body = CreateRigidBody(GEOMTYPE_NONE, "", model)
		
		if(body)
			dBodyInteractionLayer(this, ET_GRENADE)

		return body
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		timer -= ftime * 100
		SetOrigin(LightStorage, GetMatrix(this, 3) + "0 0 30")

		if(status == 0)
		{
			if(timer <= 0)
			{
				if(World.ManipEntity == this)
				{
					CreatureClass cc = World.Player
					cc.DropManipEntity(true)
				}

				vector origin = GetMatrix(this, 3)
//				misc_particle_effector explode = new misc_particle_effector()
//				explode.SetParms("particle/explosion1.ptc", origin, true, 2)
				LightHandle = AddLight(LightStorage, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 0, 128, "255 100 0")
				Show(false)
				DiscoLight.M_SwitchOff(this)	//vyhadzuje svetlo

				delete Corona
				Corona = NULL

				delete DiscoLight
				DiscoLight = NULL

				RemoveBodyAndGeom()
				ClearFlags(this, TFL_SOLID)
				World.ApplyExplosion(SStorage.shooter, GetMatrix(this, 3), 96, 300, damage, 1)
				PlaySound(this, Sound1, SF_ONCE)
				status = 1
				timer = 25				
			}
			else
				return
		}
		
		float rate
		vector color

		if(status == 1)
		{
			rate = timer / 25
			rate = 1 - rate
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 128, color)
			
			if(timer <= 0)
			{
				status = 2
				timer = 100
			}
		}
		
		if(status == 2)
		{
			rate = timer / 100
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 128, color)

			if(timer <= 0)
			{
				RemoveLight(LightHandle)
				LightHandle = 0
				status = 4
				timer = 100
			}
		}

		if(status == 4)	//musime este pockat az dohraje zvuk
		{
			if(timer <= 0)
				delete this
		}
	}

//-----------------------------------------------------------------
	void InitStorage()
	{
		LightStorage = new MinClass
		Sound1 = GetSoundEx("Weap_Grenade_expl", 1, SF_3D)

		DiscoLight = new dlight_dynamic()
		DiscoLight.texture = "textures/lightcube/fluidsmall.dds"
		DiscoLight.intensity = 550
 		DiscoLight.radius = 120
 		DiscoLight._color = "1 0.4 0.1"
 		Throw(DiscoLight, this, EV_INIT, 0)
 		DiscoLight.M_SwitchOn(this)
 		UpdateEntity(DiscoLight)
 		SetOrigin(DiscoLight, ZeroVec)
 
 		AddChild(this, DiscoLight, 1)	//na nule bude corona
	}

//-----------------------------------------------------------------
	void ModifyParms(int version)
	{
		switch(version)
		{
		case 1:
			model = "obj/physics/tachyonsphere.xob"
			size = 2
			break

		case 2:
			model = "obj/weapons/grenade.xob"
			size = 2
			break
		}
	}	

//-----------------------------------------------------------------
	void ProjectileGrenade(int version)
	{
	}

//-----------------------------------------------------------------
	void ~ProjectileGrenade()
	{
		if(RefractionSprite)
		{
			delete RefractionSprite
			RefractionSprite = NULL
		}

		if(Corona)
		{
			delete Corona
			Corona = NULL
		}
			
		if(DiscoLight)
		{
			delete DiscoLight
			DiscoLight = NULL
		}

		if(LightStorage)
		{
			delete LightStorage
			LightStorage = NULL	
		}

		RemoveBodyAndGeom()

		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}





const int AMMO_BULLETS1 = 1
const int AMMO_GRENADE1 = 2
const int AMMO_ROCKET1 = 3
const int AMMO_ROCKET2 = 4

//achjo :-/
const int AMMO_ROCKETBOSS = 5

class Shooter extends _serializable
{
	ExtClass Owner
//	float		Damage	//kolko to uberie energie
//	float		Power		//jaky visualny bordel to urobi (sila impaktu po zasahu napriklad)
	int		Type		//identifikator toho co z toho bude lietat.

//-------------------------------------------------------------
	void Fire(ShotStorage ShotStor)
	{
	}

//-------------------------------------------------------------
	void Saturate()
	{
	}
} 

//=============================================================
class ShooterBullets extends Shooter
{
//-------------------------------------------------------------
	void Fire(ShotStorage ShotStor)
	{
		ExtClass actor = ShotStor.shooter
		ShotBullets(ShotStor, actor.WTraceFilter)

//		if(Type == AMMO_BULLETS1)	//kazde strelivo ma svoje vlastnosti
//		{
//			Damage = WDmgWeaponTest
//			Power = 10

//			if(actor != World.Player)	//tie shotery pravdepodobne zrusim pretoze ztracaju zmysel
//				str.snd = PlaySound(str.SoundBase, g_sCommonSounds[10], 0)
//		}
	}

//-------------------------------------------------------------
	void ShooterBullets(ExtClass owner, int ammotype)
	{
		Owner = owner
		Type = ammotype	//nemylit s AmmoType u zbrane!
	}
}

//=============================================================
class ShooterProjectile extends Shooter
{
	void Fire(ShotStorage ShotStor)
	{
		vector vec1 = ShotStor.dir
		vector vec2, vec3
		vec2[0] = vec1[1]
		vec2[1] = vec1[2]
		vec2[2] = vec1[0]
		vec3 = vec1 * vec2

		vector mat[4]
		mat[0] = vec3
		mat[1] = vec1
		mat[2] = vec2
		mat[3] = ShotStor.from
		Projectile proj

		if(Type == AMMO_GRENADE1)	//kazde strelivo ma svoje vlastnosti
		{
			proj = new ProjectileTachyonGrenade(1)
			Throw(proj, World, EV_INIT, 0)
			proj.SetTransform(mat)
			proj.Fire(ShotStor)
			return
		}
		
		if(Type == AMMO_ROCKET1)
		{
			proj = new ProjectileRocket(2)
			Throw(proj, World, EV_INIT, 0)
			proj.SetTransform(mat)
			proj.Fire(ShotStor)
			return
		}

		if(Type == AMMO_ROCKET2)
		{
			proj = new ProjectileRocket(3)
			Throw(proj, World, EV_INIT, 0)
			proj.SetTransform(mat)
			proj.Fire(ShotStor)
			return
		}

		if(Type == AMMO_ROCKETBOSS)
		{
			proj = new ProjectileRocket(2)
			Throw(proj, World, EV_INIT, 0)
			proj.SetTransform(mat)
			proj.Fire(ShotStor)
			return
		}
	}

//-------------------------------------------------------------
	void Saturate()
	{
	}

//-------------------------------------------------------------
	void ShooterProjectile(ExtClass owner, int ammotype)
	{
		Owner = owner
		Type = ammotype
	}
}

//=================================================================================
class weapon_flash extends Particle
{
	float time,ttime
	int LightHandle
	MinClass LightOwner
	vobject obj
	float intens
	int toggle
	int frames
	vector adisp
	vector rangle //osa, pod kterou se plamen otaci
	misc_particle_effector particles
	int RotAxis

//-----------------------------------------------------------------
	void Switch(bool set)
	{
		if(set)
		{
//			AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, GetMatrix(LightOwner, 3) + "-1 -1 -1", GetMatrix(LightOwner, 3) + "1 1 1")
			//force light change, even if FPS is too low
			if(intens == 50)
				intens = 30
			else
				intens = 50

			frames = 0
			time = ttime
			SetFlags(this, TFL_VISIBLE)
			SetEventMask(this, EV_FRAME)

			if(LightHandle == 0)
				LightHandle = AddLight(LightOwner, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 150, 90, "255 100 0")

			if(particles)
			{
				particles.Restart()
				particles.Switch(true)
			}
			return
		}

		ClearFlags(this, TFL_VISIBLE)
		ClearEventMask(this, EV_FRAME)

		if(LightHandle)
		{
			RemoveLight(LightHandle)
			LightHandle = 0
		}
		
		if(particles)
			particles.Switch(false)
	}

//-----------------------------------------------------------------
	int EOnFrame(MinClass other, int extra)
	{
		float f = time / ttime * intens * 0.03

		if(LightHandle)
			SetLightEx(LightHandle,150 , 90, "255 100 0" * f)

		time = time - ftime
		frames++

		if(frames > 2)
		{
			if(time < 0)
				Switch(false)
		}

		float alp = intens * frand(2,4)
		clamp alp<0, 255>
		Color = ARGB(alp, 255, 255, 255)

		vector rot = adisp
		rot[RotAxis] = rot[RotAxis] + frand(-90,90)
		SetAngles(this, rot)
		return
	}

//-----------------------------------------------------------------
	void ~weapon_flash()
	{
		if(LightOwner)
			delete LightOwner

		if(particles)
		{
			delete particles
			particles = NULL	
		}

		if(obj)
			ReleaseObject(obj, 0)
	}

//-----------------------------------------------------------------
	void OnPick(ExtClass owner)
	{
		particles = new misc_particle_effector
		
		if(owner.ClassNum == ClassNumPlayer)
		{
			SetObject(obj, "$remap 'obj/weapons/weaflash1' 'obj/weapons/weaflash1_fps'")
			particles.SetParms("particle/weaponflash1player.ptc", ZeroVec, true, 0)
			SetRenderViewMask(this, 0x0001)
		}
		else
		{
			SetObject(obj)
			particles.SetParms("particle/weaponflash1.ptc", ZeroVec, true, 0)
		}

		particles.Switch(false)
		AddChild(this, particles, 4)
		SetAngles(particles, "90 0 0")
	}

//-----------------------------------------------------------------
	void weapon_flash(MinClass owner, float stime, vector adisplacement, int RotationAxis)
	{
		obj = GetObject("obj/weapons/weaflash1.xob")
		SetObject(obj)
		AddChild(owner, this, p_pivot0)
		SetAngles(this, adisplacement)
		adisp = adisplacement
		RotAxis = RotationAxis

		LightOwner = new MinClass()
		AddChild(owner, LightOwner, 3)
		SetOrigin(LightOwner, "-3 10 5")	//je treba nastavit svetlo na ine miesto
		ttime = stime
		rangle = "0 1 0"
	}
}



const int PRIMARY = 0
const int SECONDARY = 1

//=================================================================================
class WeaponClass extends ItemClass
{
	ExtClass			Spawner
	info_pivot		pivot0
	info_pivot		pivot1

	int				Ammo		//aktualny stav nabojov
	int				AmmoType
	int				AmmoPerMagazine
	int				Action
	float				ActionTime
	int				NumActions
	bool				Automatic[2]	//true = staci drzat a ono to opakovakuje fire
	int				CrossHair
	int				ShellType
	
	vobject			DummyObject	//model ked to lezi v mape ako item
	vobject			ReloadAnim, AttackAnim, AttackAnim2
	vobject			CustomReloadAnim // custom animace pro potvory
	vobject			SelectAnim, UnselectAnim, IdleAnim, AimAnim, WalkAnim
	vobject			WeapIdleAnim, WeapAttackAnim, WeapReloadAnim
	vobject			IdleVarAnims[3]
	
	int				ActionSound
	weapon_flash	Flash

	hsound			Sounds[5]
	int				Attacks[32]
	int 				NumAttacks
	float				ADelay[4] //doba zopakovani
	float				BulletRainIncrement

	CreatureClass	Owner			//override

	bool				IsPlayerWeapon
	
	int				AttackID 	//index do combat collection (jen pro steel weapons)
	int				PartFlags
	int				Part			//ktera cast utoku probiha
	int				NumParts
	
	vector			Target

#ifdef EDITOR
	void EditorOpened()
	{
		if(_name && Owner == NULL && rb)
		{
			ActivateBody(false)
			coords = atov(GetKeyValue("coords"))
			angle = atof(GetKeyValue("angle"))
			angle2 = atof(GetKeyValue("angle2"))
			angle3 = atof(GetKeyValue("angle3"))
			SetTransformEx(Vector(angle3, angle, angle2), coords)
			UpdateEntity(this)
		}
	}

//-----------------------------------------------------------------
	bool CanEdit()
	{
		if(Owner)
			return false

		return true
	}
#endif

//-----------------------------------------------------------------
	bool CanUpdateOwnerAnim()
	{
		return true
	}

//-----------------------------------------------------------------
	void SetAction(int act)
	{
		Action = act
		ActionTime = 0
		
		if(Action)
			SetEventMask(this, EV_FRAME)
		else
			ClearEventMask(this, EV_FRAME)

		if(Action)
		{
			if(Owner && MainObject)
				SetAnimSlot(this, s_walk - 1, VNULL, 10.0, 0, NULL, 30, 0)	//idle var musi skoncit aby po skonceni animacie prave zacinajucej akcie nebezal
		}
	}

//-----------------------------------------------------------------
	void SetTarget(vector targ, float precision)
	{
		Target = targ
	}

//-----------------------------------------------------------------
	void EOnExtra(ExtClass other, int extra)
	{
	}
	
//-----------------------------------------------------------------
	int ChoiceCover(WeaponClass wp)
	{
		return 0
	}
	
//-----------------------------------------------------------------
	void BeginUnselect()
	{
	}

//-----------------------------------------------------------------
	void BeginSelect()
	{
	}

//-----------------------------------------------------------------
	void EndAction()
	{
		SetAction(0)

		if(Owner.Hands)
			SetAnimSlot(Owner.Hands, s_weapon, VNULL, 10.0, 0, NULL, 0, 0)
		else
			SetAnimSlot(Owner, s_weapon, VNULL, 10.0, 0, NULL, 0, 0)
		
		if(MainObject)
			SetAnimSlot(this, 0, VNULL, 10.0, 0, NULL, 0, 0)	//vyhodime pripadny reload alebo fire anim
	}

//-----------------------------------------------------------------
	void OnActionEnd()
	{
	}

//-----------------------------------------------------------------
	void InterruptAction()
	{
		EndAction()
	}
	
//-----------------------------------------------------------------
	void SetUpgradeLevel(int l)
	{
	}

//-----------------------------------------------------------------
	void TraceStart()
	{
	}
	
//-----------------------------------------------------------------
	void TraceEnd()
	{
	}
	
//-----------------------------------------------------------------
	void ShootAction()
	{
	}

//-----------------------------------------------------------------
	void ShootAction2()
	{
	}

//-----------------------------------------------------------------
	int Reload()
	{
		return 0
	}

//-----------------------------------------------------------------
	bool CanSelect()
	{
		if(Ammo == 0 && Owner.Ammo[AmmoType] == 0)
			return false

		return true	
	}

//-----------------------------------------------------------------
	void Select()
	{
		SetAngles(this, ZeroVec)
		SetOrigin(this, ZeroVec)
		SetFlags(this, TFL_VISIBLE)
		Owner.UpdateHUD(1)

		if(MainObject)
			SetAnimSlot(this, 0, VNULL, 0, 0, NULL, 30, AF_RESET)	//vyhodime pripadny reload alebo fire anim

		BumpFrame(this, ftime, 0)
	}

//-----------------------------------------------------------------
	void Unselect()
	{
		SetAction(0)
		ClearFlags(this, TFL_VISIBLE)
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
	}

//---------------------------------------------------------------------------------------------------------
	void CanActivate(int fl, ExtClass other)
	{
		if(status != 0 || World.NumFlyingItems != 0)
			return ACT_DISABLE
		
		if(IsInherited(other, CCreatureClass))
		{
			CreatureClass cc = other
			if(cc.ClassNum == ClassNumPlayer)
			{
				WeaponClass wc = cc.GetWeaponPointer(ClassNum)
				
				if(wc && cc.Ammo[AmmoType] >= MaxAmmos[ClassNum])	
					return ACT_DISABLE
			}
		}
		return ACT_ENABLE
	}

//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		if(World.ManipEntity == this)
			return

		status = 1
		SetPickable(false)
		SetEventMask(this, EV_FRAME)
//		PlaySound(this, g_sCommonSounds[8], SF_ONCE)
		RemoveBodyAndGeom()
		World.NumFlyingItems++
	}

//---------------------------------------------------------------------------------------------------------
	void AddPickedAmmo(int num)
	{
		if(Owner)
		{
			int OwnAmmo = Owner.Ammo[AmmoType] + num
			clamp OwnAmmo<0, MaxAmmos[ClassNum]>
			Owner.Ammo[AmmoType] = OwnAmmo
		}
	}

//=================================================================================
	int Pick(CreatureClass owner)
	{
		status = 0
		Owner = owner
		int MagazineAmmo, OtherAmmo
		owner.RemoveInteractionAdept(this)

		if(Flash)
			Flash.OnPick(owner)

		local WeaponClass wc = owner.GetWeaponPointer(ClassNum)

		if(owner.ClassNum == ClassNumPlayer)
		{
			SetBoundBox(this, "-16 -16 -16", "16 16 16")	//hack - zbran musi byt porad viditelna, i kdyz je cela ve zdi			
			ClearFlags(this, TFL_SOLID)	//nesmie byt solidna. nefunguje koli tomu sialenemu boxu crouch
			SetRenderViewMask(this, 0x0001)
			
			//toto by uz nikdy nemalo nastat ale pre istotu keby sa niekde nieco zmenilo
			if(!wc && Spawner && Spawner != owner)	//ked to je enemacka zbran a player taku este nema tak si musi najprv vytvorit vlastnu koli inemu modelu, partiklom atd.
			{
				MagazineAmmo = Ammo	//enemaci zahadzuju zbrane s takym mnozstvom nabojov ktore presahuje AmmoPerMagazine
				clamp MagazineAmmo<0, AmmoPerMagazine>
				OtherAmmo = Ammo - MagazineAmmo
				wc = owner.CreateWeapon(ClassNum, OtherAmmo, MagazineAmmo)	//musi si rovnaku zbran a naboje z tejto zbrane premiestnime do zasobniku novej zbrane
				Ammo = 0
				wc.Pick(owner)
			}
		}

		//if already present, add just ammo
		if(wc)
		{
			if(Spawner != Owner)
				wc.AddPickedAmmo(Ammo)	//nech maju zbrane moznost si to managovat po svojom (granat)

			owner.PickWeapon(this)
			owner.UpdateHUD(1)	//ammo bar

#ifdef EDITOR
			if(Spawner)
				delete this
			else
				Show(false)
#else
			delete this
#endif
			return false
		}
		else	//zbran este nemame
		{
			if(!Spawner)	//iba v pripade ze zbran berieme zo zeme tak rozpocitame naboje zo zbrane do zasobniku a zbytok ownerovi. ked si zbran vytvara player/potvora tak uz to rozdelene je!!!
			{
				MagazineAmmo = Ammo	//enemaci zahadzuju zbrane s takym mnozstvom nabojov ktore presahuje AmmoPerMagazine
				clamp MagazineAmmo<0, AmmoPerMagazine>
				OtherAmmo = Ammo - MagazineAmmo
				Owner.Ammo[AmmoType] = OtherAmmo
				Ammo = MagazineAmmo
			}
		}

		if(DummyObject)	//zbran bola volne polozena v levele s dummy modelom takze selektneme normalny model
		{
			ClearFlags(this, TFL_FULLMATRIX)
			SelectModel(owner)
			Show(false)
		}

		SetAngles(this, ZeroVec)
		SetOrigin(this, ZeroVec)
		ClearEventMask(this, EV_VISIBLE | EV_FRAME)
		ClearFlags(this, TFL_VISIBLE|TFL_NOSHADOW)
		owner.PickWeapon(this)
		CallMethods(this, OnPick)
		return true
	}

//-----------------------------------------------------------------
	void EOnSoundEnd(MinClass other, int extra)
	{
		if(extra == ActionSound)
			ActionSound = 0
	}

//-----------------------------------------------------------------
	bool CanAct(int n)
	{
		if(n < 1 || n > NumActions)
			return false

		if(Action)
		{
			if(ActionTime < ADelay[Action - 1])
				return false
		}

		return true
	}

//-----------------------------------------------------------------
	void InitSounds(string names[])
	{
		for(int n = 0; n < sizeof(names); n++)
		{
			if(names[n])
				Sounds[n] = GetSoundEx(names[n], 1, SF_3D)
		}
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)		
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(Action)
			ActionTime += ftime

		BumpFrame(this, ftime, 0)
	}

//-----------------------------------------------------------------
	void EOnAnimEnd(MinClass other, int extra)
	{
		if(extra > 100)		//event bol poslany z playera (skoncila animacka ruk)
		{
			extra -= 100		//cislo slotu

			if(Action > 0)
				OnActionEnd()

			return
		}
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)	//volane iba pri vzniku z mapy
	{
		if(DummyObject)
		{
			if(ActivateDist == 0)
				ActivateDist = 50
	
			if(Definition.physics == true)	//item je definovany ako fyzikalny
			{
				SetTransformEx(Vector(angle3, angle, angle2), coords)
	
				if(CreateRigidBody(GEOMTYPE_NONE, "", Definition.model) == true)
					ActivateBody(true)
			}
			else
			{
				SetOrigin(this, coords)
				SetAngles(this, Vector(angle3, angle, angle2))
			}
			Show(true)
		}
	}

//-----------------------------------------------------------------
	void WeaponClass()
	{
		SetEventMask(this, EV_ANIMEND|EV_ONLOAD|EV_LOADED)
		SetFlags(this, TFL_VISIBLE)

		ADelay[0] = 0.2
		ADelay[1] = 0.2
		ADelay[2] = 0.2
		ADelay[3] = 0.2
	}

//-----------------------------------------------------------------
	void SetIdleAnim(string anim)
	{
		if(IdleAnim)
			ReleaseObject(IdleAnim, 0)

		IdleAnim = GetObject(anim)
	}

//-----------------------------------------------------------------
	void ~WeaponClass()
	{
		if(Flash)
			delete Flash

		if(ReloadAnim)
			ReleaseObject(ReloadAnim, 0)

		if(CustomReloadAnim)
			ReleaseObject(CustomReloadAnim, 0)

		if(SelectAnim)
			ReleaseObject(SelectAnim,0)

		if(UnselectAnim)
			ReleaseObject(UnselectAnim,0)

		if(IdleAnim)
			ReleaseObject(IdleAnim,0)
			
		if(AimAnim)
			ReleaseObject(AimAnim, 0)
			
		if(AttackAnim)
			ReleaseObject(AttackAnim, 0)
			
		if(AttackAnim2)
			ReleaseObject(AttackAnim2, 0)

		if(WeapIdleAnim)
			ReleaseObject(WeapIdleAnim, 0)

		if(WeapAttackAnim)
			ReleaseObject(WeapAttackAnim, 0)

		if(WeapReloadAnim)
			ReleaseObject(WeapReloadAnim, 0)

		for(int n = 0; n < sizeof(IdleVarAnims); n++)
		{
			if(IdleVarAnims[n])
				ReleaseObject(IdleVarAnims[n], 0)
		}

		if(pivot0)
			delete pivot0

		if(pivot1)
			delete pivot1
	}
}


//=================================================================================
class FireWeaponClass extends WeaponClass
{
	int					toload		//how much load after reload anim
	ShotStorageWeapon ShotStor		
	Shooter				Shooters[4]
	int					NumShooters
	float					KickUp[2]
	float					PrecisionFlawMax	//maximalna odchylka od presnej strelby.
	float					PrecisionFlawMin
	vector				ExactAimOffset
	vector				ExactAimAngOffset
	float					ExactAimFOV
	int					RicSndSkip

#ifdef WEAPONDEBUG
	int				DLines[32]
#endif

//-----------------------------------------------------------------
	void OnExactAimInterpolate(float rate)
	{
	}

//-----------------------------------------------------------------
	void LoadMagazine()
	{
		Ammo += toload
		Owner.Ammo[AmmoType] = Owner.Ammo[AmmoType] - toload
		
		if(Owner.Ammo[AmmoType] < 0)	//nikdy by sa nemalo stat ale istota je istota. zaporne ammo by bolo docela trapas
			Owner.Ammo[AmmoType] = 0

		toload = 0
//		PlaySound(this, Sounds[1], SF_ONCE|SF_QUARTERRANGE)
	}

//-----------------------------------------------------------------
	void ReloadAction()	//z animhooku playera
	{
		LoadMagazine()		
		Owner.UpdateHUD(1)	//ammo bar
	}

//-----------------------------------------------------------------
	bool CheckForAmmo()
	{
		if(Ammo != -1 && Ammo == 0)
		{
			PlaySound(this, Sounds[2], SF_ONCE)	//nie velmi stastne umiestnenie
			KEY(P_ATTACK, true)
//			KEY(S_ATTACK, true)
			return false
		}
		return true		
	}

//-----------------------------------------------------------------
	void OnActionEnd()	//akcia skoncila cela bez prerusenia. volane z animendu playera
	{
		int FinishedAction = Action
		EndAction()		//zrusi animacky a Action na 0
		
		if(FinishedAction == 1)	//primary fire
		{
			if(CheckForAmmo() == false)	//autoreload
			{
				if(Reload())
					SetAction(2)
				else
				{
					if(Owner.ClassNum == ClassNumPlayer)
					{
						Owner.WeaponChange(Owner.GetBestAvailableWeapon(), true)
					}	
				}
			}
			return
		}
		
		if(FinishedAction == 2)	//primary reload
		{
			SetAction(0)
			return
		}

		if(FinishedAction == 3)	//secondary fire
		{
			if(CheckForAmmo() == false)	//autoreload
			{
				if(Reload())
					SetAction(2)
			}
			return
		}
	}
	
//-----------------------------------------------------------------
	int Reload()
	{
		if(Ammo == AmmoPerMagazine)
			return false

		int a = Owner.Ammo[AmmoType]

		if(a <= 0)
			return false
	
		toload = a
		clamp toload<0, AmmoPerMagazine>

		int miss = AmmoPerMagazine - Ammo

		clamp toload<0, miss>

		if(Owner.ClassNum == ClassNumPlayer)
		{
			if(ReloadAnim)
			{
				SetAnimSlot(Owner.Hands, s_weapon - 1, VNULL, 10.0, 0, NULL, 30, AF_RESET)
				SetAnimSlot(Owner.Hands, s_weapon, ReloadAnim, 10.0, 0, NULL, 30, AF_ONCE)
				goto done
			}
		}
		else
		{
			if(CustomReloadAnim)
			{
				SetAnimSlot(Owner, s_weapon, CustomReloadAnim, 10.0, 0, BoneMasks[g_reload], 30, AF_ONCE)
				goto done
			}
		}

		LoadMagazine()
		Owner.UpdateHUD(1)	//ammo bar
		SetAction(0)

done:;			
		if(Owner && Owner.ClassNum == ClassNumPlayer)
		{
			if(WeapReloadAnim)
				SetAnimSlot(this, 0, WeapReloadAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}
		
		PlaySound(Owner, Sounds[1], SF_ONCE)
		return true
	}

//-----------------------------------------------------------------
	int PrimaryFire(vector target)
	{
		if(CheckForAmmo() == false)	//zistime ci mame strelivo na vystrel
			return false

		//infinite ammo
		if(Ammo != -1)
			Ammo--

		int sfl = ClearFlags(Owner, TFL_SOLID)
		vector targ, dir
		float prec = 0

		if(ShellType)
		{
			if(Owner.ClassNum == ClassNumPlayer)
			{
				//zvuk bude vydavat kazda druha
				World.ThrowShell(pivot1, Owner, ShellType, 30, Ammo & 1)
			}
			else
			{
				//kazdou druhou
				if(Ammo & 1)
					World.ThrowShell(pivot1, Owner, ShellType, 30, false)
			}
		}

		if(Flash && !World.CutSceneSkip)
			Flash.Switch(true)

		vector start

		if(Owner.ClassNum == ClassNumPlayer)
			start = g_CamMat[3]
		else
			start = GetMatrix(this, 3)

		dir = target - start
		VectorNormalize(dir)

		Shooter shoter = Shooters[0]
		ShotStor.shooter = Owner
		target = target - start
		VectorNormalize(target)
		target = target * 8192 + start
		ShotStor.from = start
		ShotStor.to = target
		ShotStor.dir = dir

		if(Owner.ClassNum == ClassNumPlayer)
		{
			float FlawDiff = PrecisionFlawMax - PrecisionFlawMin
		 	ShotStor.precision = PrecisionFlawMin + (FlawDiff * World.Cross.GetPrecisionScale())
		}
		else
			ShotStor.precision = PrecisionFlawMax

		if(Flash)
			ShotStor.VisualsFrom = GetMatrix(Flash, 3)
		else
			ShotStor.VisualsFrom = start

		int RicSndType = ShotStor.RicSndType
		
		if(RicSndType != -1)	//od niecoho nechcem zvuky vobec
		{
			if(RicSndSkip != 0 && rand(0, RicSndSkip))	//kazda zbran urcuje kolko ric zvukov sa bude pustat
				ShotStor.RicSndType = -1
		}

		shoter.Fire(ShotStor)

		ShotStor.RicSndType = RicSndType	//vratime co tam bolo
		SetFlags(Owner, sfl)

		if(Owner != World.Player && !World.CutSceneSkip)	//player to pusta po uspesnom Action()
		{
			if(ActionSound)
				EndSound(ActionSound)
	
			ActionSound = PlaySound(Owner, Sounds[0], SF_ONCE)
		}

		if(AttackAnim)
		{
			if(Owner.Hands)
				SetAnimSlot(Owner.Hands, s_weapon, AttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}
		
		if(Owner && Owner.ClassNum == ClassNumPlayer)
		{
			if(WeapAttackAnim)
				SetAnimSlot(this, 0, WeapAttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}

		return true
	}

//-----------------------------------------------------------------
	int SecondaryFire(vector target)
	{
		return false
	}

//-----------------------------------------------------------------
	int Action(int n, vector targ)
	{
		if(!CanAct(n))
			return false

		if(Action)				//CanAct niekedy vrati true aj ked akcia bezi a ta sa musi okamzite zrusit
			InterruptAction()

		switch(n)
		{
			case 1:
				if(PrimaryFire(targ))
				{
					SetAction(n)
					return true
				}
			break
			
			case 2:
				if(Reload())
				{
					SetAction(n)
					return true
				}
			break
			
			case 3:	//sekundarny mod
				if(SecondaryFire(targ))
				{
					SetAction(n)
					return true
				}
		}

		SetAction(0)
		return false
	}

//-----------------------------------------------------------------
	void FireWeaponClass()
	{
		pivot0 = new info_pivot
		AddChild(this, pivot0, p_pivot0)
		
		pivot1 = new info_pivot
		AddChild(this, pivot1, p_pivot1)

		ShotStor = new ShotStorageWeapon
		ShotStor.BulletRainInc = 1.0
		ShotStor.HurtType = HT_SHOT
		CrossHair = 0
	}
	
	void ~FireWeaponClass()
	{
		delete ShotStor
		ShotStor = NULL

		for(int n = 0; n < NumShooters; n++)
		{
			delete Shooters[n]
			Shooters[n] = NULL
		}
	}
}

//=================================================================================
class item_weapon_dummy extends FireWeaponClass
{
//-----------------------------------------------------------------
	bool CanSelect()
	{
		return true	
	}

//=================================================================================
	int Pick(CreatureClass owner)
	{
		status = 0
		Owner = owner
		owner.RemoveInteractionAdept(this)
		ClearFlags(this, TFL_VISIBLE|TFL_NOSHADOW)
		owner.PickWeapon(this)
		return true
	}
	
//-----------------------------------------------------------------
	void OnActionEnd()
	{
		EndAction()
	}
	
//-----------------------------------------------------------------
	int Action(int n, vector targ)
	{
		SetAction(0)
		return false
	}
	
//-----------------------------------------------------------------
	void item_weapon_dummy()
	{
		ClearEventMask(this, EV_INIT)
		ClassNum = CDummyWeapon
		AmmoType = AmmoJackHammer
		IsPlayerWeapon = true
		NumActions = 0
		CrossHair = 3	//ziadny
	}
}



//=================================================================================
class item_weapon_jackhammer extends FireWeaponClass
{
	vobject SwingAnim[2]
	vobject AttackAnims[4]

//-----------------------------------------------------------------
	bool CanSelect()
	{
		return true	
	}

//-----------------------------------------------------------------
	bool CheckForAmmo()
	{
		return true		
	}

//-----------------------------------------------------------------
	bool CanAct(int n)
	{
//		if(Action)
//			return false

		return true
	}

//-----------------------------------------------------------------
	void ShootAction()
	{
		int sfl = ClearFlags(Owner, TFL_SOLID)
		vector targ, dir = World.CameraNorm
		float prec = 0

		vector start = g_CamMat[3]
		vector target = dir * 48
		target = start + target

		Shooter shoter = Shooters[0]
		ShotStor.shooter = Owner
		ShotStor.from = start
		ShotStor.to = target
		ShotStor.dir = dir
	 	ShotStor.precision = 0
		ShotStor.VisualsFrom = start
		shoter.Fire(ShotStor)
		SetFlags(Owner, sfl)

		if(ActionSound)
			EndSound(ActionSound)

		ActionSound = PlaySound(Owner, Sounds[0], SF_ONCE)
	}

//-----------------------------------------------------------------
	void EndAction()
	{
		SetAction(0)

		if(Owner.Hands)
			SetAnimSlot(Owner.Hands, s_weapon, VNULL, 10.0, 0, NULL, 0, 0)
		else
			SetAnimSlot(Owner, s_weapon, VNULL, 10.0, 0, NULL, 0, 0)
		
		if(MainObject)
			SetAnimSlot(this, 0, VNULL, 0, 0, NULL, 30, 0)	//vyhodime pripadny reload alebo fire anim
	}

//-----------------------------------------------------------------
	int PrimaryFire(vector target)
	{
//		if(!ActionSound)
//			ActionSound = PlaySound(Owner, Sounds[0], 0)

		int content, sparm
		ExtClass other
		float plane[4]
		vobject anim = VNULL
		target = World.CameraNorm * 48
		target = World.CameraPos + target

		if(TraceLineEx(DEFAULT, World.CameraPos, target, ZeroVec, ZeroVec, other, plane, content, sparm, TRACE_RAGDOLLS|TRACE_DEFAULT|TRACE_BONES, Owner.WTraceFilter) >= 1)
		{
			anim = SwingAnim[rand(0, 2)]
			PlaySound(Owner, Sounds[1], SF_ONCE)
		}
		else
			anim = AttackAnims[rand(0, 4)]

		if(anim && Owner.Hands)
			SetAnimSlot(Owner.Hands, s_weapon, anim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)

		return true
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			
			if(spawner.ClassNum == ClassNumPlayer)
				SelectObject(this, MainObject)
			else
				SetObject(MainObject, "$remap 'hammer_fps' 'hammer';")
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	}

//-----------------------------------------------------------------
	void item_weapon_jackhammer(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		AmmoType = AmmoJackHammer
		name = "CJackHammer"
		ClassNum = World.ItemColPtr.GetItemIndex(name)	//ClassNum = CJackHammer
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)
		MainObject = GetObject("obj/weapons/hammer.xob")		//normalny model
		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)		//dummy model ked je zbran volne v mape ako item

		SelectModel(spawner)

		Shooters[0] = new ShooterBullets(this, AMMO_BULLETS1)		//primarna strelba (samopal)
		NumShooters = 1
		NumActions = 1

		PrecisionFlawMin = 0
		PrecisionFlawMax = 0
		RicSndSkip = 0

		ADelay[0] = 0.25 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 	 	//primary attack 7 shots per second
 
 		ShotStor.damage = WDmgJackHammer
		ShotStor.BulletRainInc = 5.0
 		ShotStor.power = 30
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 3.9
		ShotStor.StreakLength = 0	//nechceme streak
		ShotStor.RicDecalType = DE_PLASMA		//DE_BULLET, DE_PLASMA
		ShotStor.RicSndType = SE_HAMMER			//SE_BULLET, SE_PLASMA
		ShotStor.RicParticlesType = PE_PLAYERPLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		SetAngles(pivot0, "90 0 0")

		IdleAnim = GetObject("anm/player/hands/Hammer_Idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/Hammer_Walk.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/Pistol_reload.anm")		//player
//		AttackAnim = GetObject("anm/player/hands/Hammer_hit_01.anm")			//player
//		AttackAnim2 = GetObject("anm/player/hands/Hammer_hit_02.anm")			//player
		AttackAnims[0] = GetObject("anm/player/hands/Hammer_hit_01.anm")
		AttackAnims[1] = GetObject("anm/player/hands/Hammer_hit_02.anm")
//		AttackAnims[2] = GetObject("anm/player/hands/Hammer_hit_03.anm")
		AttackAnims[2] = GetObject("anm/player/hands/Hammer_hit_01.anm")
		AttackAnims[3] = GetObject("anm/player/hands/Hammer_hit_04.anm")
		SwingAnim[0] = GetObject("anm/player/hands/Hammer_swing_01.anm")
		SwingAnim[1] = GetObject("anm/player/hands/Hammer_swing_02.anm")
		SelectAnim = GetObject("anm/player/hands/Hammer_In.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/Hammer_Out.anm")			//player

		IdleVarAnims[0] = GetObject("anm/player/hands/JackHammer_idle_var1.anm")		//player
		IdleVarAnims[1] = GetObject("anm/player/hands/JackHammer_idle_var2.anm")		//player
		
//		WeapAttackAnim = GetObject("anm/weapons/Pistol_fire.anm")
		WeapIdleAnim = GetObject("anm/weapons/JackHammer_idle.anm")
//		WeapReloadAnim = GetObject("anm/weapons/Pistol_reload.anm")

		if(WeapIdleAnim)
			SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0)

		InitSounds(WS_WeaponHammer)
		SetEventMask(this, EV_ANIMEND)
		
		KickUp[PRIMARY] = 0		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0
		Automatic[PRIMARY] = false
		Automatic[SECONDARY] = false
	}

	void ~item_weapon_jackhammer()
	{
		int n
		
		for(n = 0; n < sizeof(SwingAnim); n++)
		{
			if(SwingAnim[n])
				ReleaseObject(SwingAnim[n], 0)
		}

		for(n = 0; n < sizeof(AttackAnims); n++)
		{
			if(AttackAnims[n])
				ReleaseObject(AttackAnims[n], 0)
		}
	}
}

//=================================================================================
class item_weapon_pistol extends FireWeaponClass
{
//-----------------------------------------------------------------
	bool CheckForAmmo()
	{
		if(Ammo != -1 && Ammo == 0)	//pistol ma len primarne strelivo ale
		{
			PlaySound(this, Sounds[2], SF_ONCE)
			return false
		}
		return true		
	}
/*
//-----------------------------------------------------------------
	int SecondaryFire(vector target)
	{
		return false

		if(CheckForAmmo() == false)	//secondary mod u pistole pouziva naboje z primary!!!!!
			return false

		Ammo--
		int sfl = ClearFlags(Owner, TFL_SOLID)

		vector targ, dir
		float prec = 0

		if(ShellType)
			World.ThrowShell(pivot1, Owner, ShellType, 30, true)

		Flash.Switch(true)

		vector start
		
		if(Owner.ClassNum == ClassNumPlayer)
			start = g_CamMat[3]
		else
			start = GetMatrix(this, 3)

		dir = target - start
		VectorNormalize(dir)

		Shooter shoter = Shooters[0] 
		ShotStor.shooter = Owner
		ShotStor.from = start
		ShotStor.to = target
		ShotStor.dir = dir
		ShotStor.VisualsFrom = GetMatrix(Flash, 3)

		if(Owner.ClassNum == ClassNumPlayer)
		{
			float FlawDiff = PrecisionFlawMax - PrecisionFlawMin
		 	ShotStor.precision = PrecisionFlawMin + (FlawDiff * World.Cross.GetPrecisionScale())
		}
		else
			ShotStor.precision = PrecisionFlawMax

		shoter.Fire(ShotStor)

		SetFlags(Owner, sfl)

		EndSound(ActionSound)
		ActionSound = PlaySound(Owner, Sounds[0], SF_ONCE)

		if(AttackAnim && Owner.Hands)
		{
			SetAnimSlot(Owner.Hands, s_weapon, AttackAnim2, 10.0, 1, NULL, 30, AF_ONCE|AF_RESET)

			if(WeapAttackAnim)
				SetAnimSlot(this, 0, WeapAttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}

		return true		
	}
*/

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			if(spawner.ClassNum == ClassNumPlayer)
				SetObject(MainObject, "$remap 'pistol' 'pistol_fps';$remap 'pistol_ammo' 'pistol_ammo_fps';$remap 'pistol_effect' 'pistol_effect_fps';")
			else
				SetObject(MainObject)
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	}

//-----------------------------------------------------------------
	void item_weapon_pistol(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		name = "CPistol"
		ClassNum = World.ItemColPtr.GetItemIndex(name)	//ClassNum = CPistol
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
			MainObject = GetObject("obj/weapons/pistol.xob")			//normalny model
		else
			MainObject = GetObject("obj/weapons/pistol_enemy.xob")

		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)		//dummy model ked je zbran volne v mape ako item
		ShellType = 1
//		AimAnim = GetObject("anm/player/hands/Pistol_aim.anm")

		SelectModel(Spawner)

		AmmoPerMagazine = PistolAmmoPerMagazine
		Ammo = AmmoPerMagazine
		AmmoType = AmmoPistol
		NumActions = 3

		Shooters[0] = new ShooterBullets(this, AMMO_BULLETS1)		//primarna strelba (samopal)
		NumShooters = 1

		PrecisionFlawMin = 0.002
		PrecisionFlawMax = 0.025
		RicSndSkip = 0

		ExactAimOffset = "-4.965 -2 0"
		ExactAimAngOffset = "0 -2 0"
		ExactAimFOV = 16

		ADelay[0] = 0.22 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 	 	//primary attack 7 shots per second
		ADelay[1] = 1.8		//reload
		ADelay[2] = 0.10 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//secondary attack

		ShotStor.damage = WDmgGun
		ShotStor.BulletRainInc = 2.44
		ShotStor.power = 10.75
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 6.8
		ShotStor.StreakWidth = 5  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 85  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 1750 * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 3
		ShotStor.RicDecalType = DE_BULLET
		ShotStor.RicSndType = SE_BULLET
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			ShotStor.RicParticlesType = PE_BULLET
		else
			ShotStor.RicParticlesType = PE_PLAYERBULLET
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "90 0 0", 1)

		IdleAnim = GetObject("anm/player/hands/Pistol_idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/Pistol_idle.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/Pistol_reload.anm")		//player
		AttackAnim = GetObject("anm/player/hands/Pistol_fire.anm")			//player
		AttackAnim2 = GetObject("anm/player/hands/Pistol_fire.anm")			//player
		SelectAnim = GetObject("anm/player/hands/Pistol_in.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/Pistol_out.anm")			//player
		CustomReloadAnim = GetObject("anm/civil_pistol/c_reload.anm")		//enemies

		IdleVarAnims[0] = GetObject("anm/player/hands/Pistol_idle_var1.anm")		//player
		IdleVarAnims[1] = GetObject("anm/player/hands/Pistol_idle_var2.anm")		//player

		WeapAttackAnim = GetObject("anm/weapons/Pistol_fire.anm")
		WeapIdleAnim = GetObject("anm/weapons/Pistol_idle.anm")
		WeapReloadAnim = GetObject("anm/weapons/Pistol_reload.anm")

		InitSounds(WS_WeaponGun)
		SetEventMask(this, EV_ANIMEND)
		
		KickUp[PRIMARY] = 0.609		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.4
		Automatic[PRIMARY] = false
		Automatic[SECONDARY] = false
	}
}

//=================================================================================
class item_weapon_machinegun extends FireWeaponClass
{
	int RotSnd
	float RotAngle
	float RotSpeed
	float MaxRotSpeed
	misc_particle_effector SmokeEffector
	float SmokePower
	float OrginalEffectTime

//-----------------------------------------------------------------
	int SecondaryFire(vector target)
	{
		return false
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			if(spawner.ClassNum == ClassNumPlayer)
			{
				SetObject(MainObject, "$remap 'gatlingLE' 'gatlingLE_fps';")

				if(WeapIdleAnim)
					SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0) 
			}
			else
				SetObject(MainObject)
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	}

//-----------------------------------------------------------------
	void SetAction(int act)
	{
/*		if(act != 1)
		{
			if(ActionSound)
			{
				EndSound(ActionSound)
				ActionSound = 0
			}
		}*/

		Action = act
		ActionTime = 0
		
		if(Action)
			SetEventMask(this, EV_FRAME)

		if(Action)
		{
			if(Owner && MainObject)
				SetAnimSlot(this, s_walk - 1, VNULL, 10.0, 0, NULL, 30, 0)	//idle var musi skoncit aby po skonceni animacie prave zacinajucej akcie nebezal
		}
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(status > 0)	//prebieha pick zbrane v superclase
			return

		MaxRotSpeed = 1440

		if(Action == 1)
		{
			SmokePower += ftime * 0.5

			RotSpeed = MaxRotSpeed
//			RotSpeed += ftime * 8000
			
			if(RotSpeed >= MaxRotSpeed)
				RotSpeed = MaxRotSpeed

			if(RotSnd)
			{
				EndSound(RotSnd)
				RotSnd = 0
			}
		}
		else
		{
			RotSpeed -= ftime * 450

			if(RotSpeed <= 0)
			{
				RotSpeed = 0
				
				if(!Action)
					ClearEventMask(this, EV_FRAME)
			}
			else
			{
				if(!KEY(P_ATTACK, false))
				{
					if(!RotSnd)
					{
						RotSnd = PlaySound(this, Sounds[3], SF_ONCE|SF_QUARTERRANGE)	
						clamp SmokePower<0, 1>
						float EffectTime = OrginalEffectTime * SmokePower
	/*
						SmokeEffector.SetParmToEffectors(EP_EFFECT_TIME, EffectTime)
	
						SmokeEffector.Restart()
						SmokeEffector.Switch(true)*/
					}

					SmokePower = 0
				}
			}
		}

		RotAngle += RotSpeed * ftime
		
		if(RotAngle > 360)
			RotAngle = RotAngle - 360

		SetBone(this, B_Rotor, Vector(0, RotAngle, 0), ZeroVec, 1)
	}

//-----------------------------------------------------------------
	void item_weapon_machinegun(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		name = "CMachinegun"
		ClassNum = World.ItemColPtr.GetItemIndex(name)
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
			MainObject = GetObject("obj/weapons/gatlingLE.xob")
		else
			MainObject = GetObject("obj/weapons/gatlingLE_enemy.xob")

		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)
		ShellType = 1

		SelectModel(Spawner)

		AmmoPerMagazine = MachinegunAmmoPerMagazine
		Ammo = AmmoPerMagazine
		AmmoType = AmmoMachinegun

		NumActions = 4
		NumShooters = 1

		Shooters[0] = new ShooterBullets(this, AMMO_BULLETS1)		//primarna strelba (samopal)
//		Shooters[1] = new ShooterProjectile(this, AMMO_GRENADE1)	//sekundarna (granat)

		PrecisionFlawMin = 0.002906
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			PrecisionFlawMax = 0.0406
		else
			PrecisionFlawMax = 0.02654
		
		RicSndSkip = 3

		ExactAimOffset = "-2.935 -1.85 0"
		ExactAimAngOffset = "0 0 0"
		ExactAimFOV = 16

		ADelay[0] = 0.09 * sqrt(sqrt(sqrt(sqrt(g_timescale))))  		//promary attack
		ADelay[1] = 1.7		//reload
		ADelay[2] = 0.85 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//secondary attack

		ShotStor.damage = WDmgMachinegun
		ShotStor.BulletRainInc = 1.3
		ShotStor.power = 9.29
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 5
		ShotStor.StreakWidth = 6  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 95  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 1800 * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 2
		ShotStor.RicDecalType = DE_PLASMA
		ShotStor.RicSndType = SE_BULLET
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			ShotStor.RicParticlesType = PE_BULLET
		else
			ShotStor.RicParticlesType = PE_PLAYERBULLET
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "90 0 0", 1)

		//PLAYER
		IdleAnim = GetObject("anm/player/hands/GatlingLE_idle.anm")
		WalkAnim = GetObject("anm/player/hands/GatlingLE_walk.anm")
		ReloadAnim = GetObject("anm/player/hands/GatlingLE_reload.anm")
		AttackAnim = GetObject("anm/player/hands/GatlingLE_fire.anm")
		CustomReloadAnim = GetObject("anm/combattest/c_reload.anm")

		SelectAnim = GetObject("anm/player/hands/GatlingLE_in.anm")
		UnselectAnim = GetObject("anm/player/hands/GatlingLE_out.anm")
		IdleVarAnims[0] = GetObject("anm/player/hands/GatlingLE_idle_var1.anm")
		IdleVarAnims[1] = GetObject("anm/player/hands/GatlingLE_idle_var2.anm")
		WeapReloadAnim = GetObject("anm/weapons/GatlingLE_reload.anm")

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
		{
			if(WeapIdleAnim)
				SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0) 
		}

		InitSounds(WS_WeaponMachinegun)
		SetEventMask(this, EV_ANIMEND)

		SmokeEffector = new misc_particle_effector
		SmokeEffector.SetParms("particle/GatlingLE_smoke.ptc", "-2 11 4", true, 1)
		SmokeEffector.Switch(false)
		GetEffectorParm(SmokeEffector, 0, EP_EFFECT_TIME, OrginalEffectTime)
		AddChild(this, SmokeEffector, 4)

		KickUp[PRIMARY] = 0.203		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.402
		Automatic[PRIMARY] = true
		Automatic[SECONDARY] = false
	}

//-----------------------------------------------------------------
	void ~item_weapon_machinegun()
	{
		if(SmokeEffector)
		{
			delete SmokeEffector
			SmokeEffector = NULL
		}
	}
}

//=================================================================================
class item_weapon_shotgun extends FireWeaponClass
{
	int CurReloadAnim
	vobject ReloadAnim2, ReloadAnim3	//ReloadAnim - nabeh, ReloadAnim2 nabijanie (loop), ReloadAnim3 - dobeh

//-----------------------------------------------------------------
	bool CanAct(int n)
	{
		if(n < 1 || n > NumActions)
			return false

		if(Action)
		{
			if(Action == 2 && n == 2)
				return false

			if(Action == 2 && Ammo > 0)	//reload sa musi dat prerusit pokial mame aspon jeden naboj
				return true

			if(ActionTime < ADelay[Action - 1])
				return false
		}

		return true
	}

//-----------------------------------------------------------------
	void EndAction()
	{
		SetAction(0)
		CurReloadAnim = 0

		if(Owner.Hands)
			SetAnimSlot(Owner.Hands, s_weapon, VNULL, 20.0, 0, NULL, 0, 0)

		if(MainObject)
			SetAnimSlot(this, 0, VNULL, 0, 0, NULL, 30, 0)	//vyhodime pripadny reload alebo fire anim
	}

//-----------------------------------------------------------------
	void PlayLastReloadAnim()
	{
		SetAnimSlot(Owner.Hands, s_weapon, ReloadAnim3, 0, 0, NULL, 30, AF_ONCE)

		if(Owner && Owner.ClassNum == ClassNumPlayer)
		{
			if(WeapReloadAnim)	//s poslednym kusom animujeme aj zbran
				SetAnimSlot(this, 0, WeapReloadAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}

		PlaySound(this, Sounds[4], SF_ONCE)			
	}

//-----------------------------------------------------------------
	void OnActionEnd()	//akcia skoncila cela bez prerusenia. volane z animendu playera
	{
		int FinishedAction = Action

		if(FinishedAction == 1)	//primary fire
		{
			EndAction()		//zrusi animacky a Action na 0
			
			if(CheckForAmmo() == false)	//autoreload
			{
				if(Reload())
					SetAction(2)
				else
				{
					if(Owner.ClassNum == ClassNumPlayer)
					{
						Owner.WeaponChange(Owner.GetBestAvailableWeapon(), true)
					}
				}
			}
			return
		}
		
		if(FinishedAction == 2)	//primary reload
		{
			if(CurReloadAnim == 1)	//skoncila prva animacka (nabeh)
			{
				if(Ammo < AmmoPerMagazine && Owner.Ammo[AmmoType] > 0)	//ak potrebujeme dalej dobijat a mame z coho
				{
					SetAnimSlot(Owner.Hands, s_weapon, ReloadAnim2, 0, 0, NULL, 30, AF_ONCE)
					CurReloadAnim = 2
				}
				else
				{
					PlayLastReloadAnim()
					CurReloadAnim = 3
				}

				return				
			}

			if(CurReloadAnim == 2)	//skoncila druha animacka (loop)
			{
				if(Ammo < AmmoPerMagazine && Owner.Ammo[AmmoType] > 0)	//ak potrebujeme dalej dobijat a mame z coho
					SetAnimSlot(Owner.Hands, s_weapon, ReloadAnim2, 0, 0, NULL, 30, AF_ONCE)
				else
				{
					PlayLastReloadAnim()
					CurReloadAnim = 3
				}
				return				
			}

			if(CurReloadAnim == 3)	//skoncila tretia animacka (dobeh)
			{
				CurReloadAnim = 0
				EndAction()		//zrusi animacky a Action na 0
				return				
			}

			SetAction(0)
			return
		}
	}

//-----------------------------------------------------------------
	int Reload()
	{
		if(Ammo == AmmoPerMagazine)
			return false

		int a = Owner.Ammo[AmmoType]

		if(a <= 0)
			return false

		if(ReloadAnim && Owner.Hands)
		{
			SetAnimSlot(Owner.Hands, s_weapon - 1, VNULL, 10.0, 0, NULL, 30, AF_RESET)
			SetAnimSlot(Owner.Hands, s_weapon, ReloadAnim, 10.0, 0, NULL, 30, AF_ONCE)
			CurReloadAnim = 1
		}
		else
		{
			toload = a
			clamp toload<0, AmmoPerMagazine>
			int miss = AmmoPerMagazine - Ammo
			clamp toload<0, miss>
			LoadMagazine()
			Owner.UpdateHUD(1)	//ammo bar
			SetAction(0)
		}

		PlaySound(Owner, Sounds[1], SF_ONCE)
		return true
	}

//-----------------------------------------------------------------
	void ReloadAction()	//preposlane z animhooku playera
	{
		toload = 1			//maly hack spracialne koli shotgunu
		LoadMagazine()		
		toload = 0			//load magazine sa vola este na konci celej akcie ale tam uz bude toload = 0
		Owner.UpdateHUD(1)	//ammo bar
		PlaySound(Owner, Sounds[3], SF_ONCE)
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			if(spawner.ClassNum == ClassNumPlayer)
			{
				SetObject(MainObject, "$remap 'obj/weapons/Shotgun' 'obj/weapons/Shotgun_fps';$remap 'obj/weapons/Shotgun_effect' 'obj/weapons/Shotgun_effect_fps';")
				
				if(WeapIdleAnim)
					SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0)
			}
			else
				SetObject(MainObject)
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	}

//-----------------------------------------------------------------
	void item_weapon_shotgun(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		name = "CShotgun"
		ClassNum = World.ItemColPtr.GetItemIndex(name)	//ClassNum = CMachineGun
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)
		
		if(!spawner || spawner.ClassNum == ClassNumPlayer)
			MainObject = GetObject("obj/weapons/shotgun.xob")
		else
			MainObject = GetObject("obj/weapons/shotgun_enemy.xob")

		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)
		ShellType = 1
//		AimAnim = GetObject("anm/player/hands/ShotGun_aim.anm")

		SelectModel(Spawner)

		AmmoPerMagazine = ShotgunAmmoPerMagazine
		Ammo = AmmoPerMagazine
		AmmoType = AmmoShotgun

		NumActions = 3
		NumShooters = 1

		Shooters[0] = new ShooterBullets(this, AMMO_BULLETS1)		//primarna strelba (samopal)

		PrecisionFlawMin = 0.0441
		PrecisionFlawMax = 0.0979
		RicSndSkip = 5

		ExactAimOffset = "-2.935 -1 0"
		ExactAimAngOffset = "10 0 0"
		ExactAimFOV = 16

		ADelay[0] = 1 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) //primary attack
		ADelay[1] = 5			//reload

		ShotStor.damage = WDmgShotgun
		ShotStor.BulletRainInc = 3.975
		ShotStor.power = 6.45
		ShotStor.NumBullets = 8
		ShotStor.MarkSize = 3.32
		ShotStor.StreakWidth = 4  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 81  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 1700  * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 3
		ShotStor.RicDecalType = DE_BULLET
		ShotStor.RicSndType = SE_BULLET
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			ShotStor.RicParticlesType = PE_BULLET
		else
			ShotStor.RicParticlesType = PE_PLAYERBULLET
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "90 0 0", 1)

		IdleAnim = GetObject("anm/player/hands/ShotGun_Idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/ShotGun_walk.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/ShotGun_reload_01.anm")			//player
		ReloadAnim2 = GetObject("anm/player/hands/ShotGun_reload_02.anm")			//player
		ReloadAnim3 = GetObject("anm/player/hands/ShotGun_reload_03.anm")			//player

		CustomReloadAnim = GetObject("anm/shotgun/c_reload.anm")		//enemies

		AttackAnim = GetObject("anm/player/hands/ShotGun_Fire.anm")			//player
		SelectAnim = GetObject("anm/player/hands/ShotGun_in.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/ShotGun_out.anm")			//player
		IdleVarAnims[0] = GetObject("anm/player/hands/ShotGun_Idle_var1.anm")		//player
		IdleVarAnims[1] = GetObject("anm/player/hands/ShotGun_Idle_var2.anm")		//player

		WeapIdleAnim = GetObject("anm/weapons/ShotGun_idle.anm")
		WeapAttackAnim = GetObject("anm/weapons/ShotGun_Fire.anm")
		WeapReloadAnim = GetObject("anm/weapons/ShotGun_reload_03.anm")	//pusta sa iba s ReloadAnim3 na rukach

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
		{
			if(WeapIdleAnim)
				SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0)
		}

		InitSounds(WS_WeaponShotgun)
		SetEventMask(this, EV_ANIMEND)

		KickUp[PRIMARY] = 0.877 	//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.255
		Automatic[PRIMARY] = false
		Automatic[SECONDARY] = false
	}

//-----------------------------------------------------------------
	void ~item_weapon_shotgun()
	{	
		if(ReloadAnim2)
		{
			ReleaseObject(ReloadAnim2, 0)
			ReloadAnim2 = VNULL
		}

		if(ReloadAnim3)
		{
			ReleaseObject(ReloadAnim3, 0)
			ReloadAnim3 = VNULL
		}
	}
}

//=================================================================================
class item_weapon_sniperrifle extends FireWeaponClass
{
//-----------------------------------------------------------------
	void OnExactAimInterpolate(float rate)
	{
		if(rate >= 0.5)
			CrossHair = 5	//ziadny kurzor
		else
			CrossHair = 0

		SetBone(this, B_Sniper_Display, Vector(0, 0, rate * 90), ZeroVec, 1)
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze

			if(spawner.ClassNum == ClassNumPlayer)
			{
				SetObject(MainObject, "$remap 'obj/weapons/SniperRifle' 'obj/weapons/SniperRifle_fps';$remap 'obj/weapons/SniperRifle_CrossEff' 'obj/weapons/SniperRifle_CrossEff_fps';$remap 'obj/weapons/SniperRifle_Cross' 'obj/weapons/SniperRifle_Cross_fps';")
				
				if(WeapIdleAnim)
					SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0) 
			}
			else
				SetObject(MainObject, "$remap 'obj/weapons/SniperRifle_Display' 'null';")
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	} 

//-----------------------------------------------------------------
	void item_weapon_sniperrifle(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		name = "CSniperRifle"
		ClassNum = World.ItemColPtr.GetItemIndex(name)	//ClassNum = CMachineGun
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)
		
		if(!spawner || spawner.ClassNum == ClassNumPlayer)
			MainObject = GetObject("obj/weapons/SniperRifle.xob")
		else
			MainObject = GetObject("obj/weapons/SniperRifle_enemy.xob")

		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)
		ShellType = 1
//		AimAnim = GetObject("anm/player/hands/MachineGun_aim.anm")

		SelectModel(Spawner)

		AmmoPerMagazine = SniperRifleAmmoPerMagazine
		Ammo = AmmoPerMagazine
		AmmoType = AmmoSniperRifle

		NumActions = 4
		NumShooters = 1

		Shooters[0] = new ShooterBullets(this, AMMO_BULLETS1)		//primarna strelba (samopal)

		PrecisionFlawMin = 0.00002
		PrecisionFlawMax = 0.19
		RicSndSkip = 0

		//temporalne vypnute nez jirka predela animacku idlu (LCD lezlo do ruky)
		ExactAimOffset = "-1.935 -4.325 5" //"-4 -2 1"  l/r n/d
		ExactAimAngOffset = "0 0 0" //"0 0 0"  
		ExactAimFOV = 50 //8

		ADelay[0] = 0.5 * sqrt(sqrt(sqrt(sqrt(g_timescale))))  		//primary attack
		ADelay[1] = 1.7		//reload
		ADelay[2] = 0.7 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//secondary attack

		ShotStor.damage = WDmgSniperRifle
		ShotStor.BulletRainInc = 6.5
		ShotStor.power = 28.35
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 9.3
		ShotStor.StreakWidth = 9  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 180  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 2000  * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 4
		ShotStor.RicDecalType = DE_PLASMA
		ShotStor.RicSndType = SE_PLASMA
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			ShotStor.RicParticlesType = PE_BULLET
		else
			ShotStor.RicParticlesType = PE_PLAYERBULLET
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "90 0 0", 1)

		IdleAnim = GetObject("anm/player/hands/SniperRifle_idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/SniperRifle_walk.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/SniperRifle_reload.anm")			//player
		AttackAnim = GetObject("anm/player/hands/SniperRifle_fire.anm")			//player
		AttackAnim2 = GetObject("anm/player/hands/SniperRifle_fire.anm")			//player
		SelectAnim = GetObject("anm/player/hands/SniperRifle_in.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/SniperRifle_out.anm")			//player
		IdleVarAnims[0] = GetObject("anm/player/hands/SniperRifle_idle.anm")		//player
		IdleVarAnims[1] = GetObject("anm/player/hands/SniperRifle_idle.anm")		//player

		WeapIdleAnim = GetObject("anm/weapons/SniperRifle_Idle.anm")
		WeapReloadAnim = GetObject("anm/weapons/SniperRifle_reload.anm")
//		ProjectX/anm/weapons/SniperRifle_Open.anm

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
		{
			if(WeapIdleAnim)
				SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0) 
		}

		InitSounds(WS_WeaponSniperRifle)
		SetEventMask(this, EV_ANIMEND)

		KickUp[PRIMARY] = 0.76		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.02
		Automatic[PRIMARY] = false
		Automatic[SECONDARY] = false
	}
}


//=================================================================================
class item_weapon_grenade extends FireWeaponClass
{
	ProjectileGrenade ActionProj
	bool FireButton
	float ThrowPow
	bool OffScreenPos
	bool AfterThrow

//-----------------------------------------------------------------
	bool CanUpdateOwnerAnim()
	{
		return 1 - OffScreenPos
	}

//-----------------------------------------------------------------
	void Unselect()
	{
		SetAction(0)
		ClearFlags(this, TFL_VISIBLE)
		
		if(ActionProj)
		{
			delete ActionProj
			ActionProj = NULL	
		}
	}

//-----------------------------------------------------------------
	void SetAction(int act)
	{
		Action = act
		ActionTime = 0

		if(Action)
			SetEventMask(this, EV_FRAME)
		else
			ClearEventMask(this, EV_FRAME)

		if(Action)
		{
			if(Owner && MainObject)
				SetAnimSlot(this, s_walk - 1, VNULL, 10.0, 0, NULL, 30, 0)	//idle var musi skoncit aby po skonceni animacie prave zacinajucej akcie nebezal
		}

		ThrowPow = 0
		OffScreenPos = false
		bool ThrowBar = false
		FireButton = false

		if(Action == 1)
			ThrowBar = true

		if(Owner)
			Owner.SetThrowBar(ThrowPow, ThrowBar)
	}

//-----------------------------------------------------------------
	void Fire()
	{
		ShotStor.shooter = Owner
		ShotStor.dir = World.CameraNorm//GetMatrix(this, 1)
		ShotStor.from = GetMatrix(this, 3)
		ShotStor.from += g_CamMat[0] * 7
		ShotStor.from += World.CameraNorm * 10
		ShotStor.to = ShotStor.from
		ShotStor.to += ShotStor.dir * 1000000
		ShotStor.precision = 0//PrecisionFlawMax * World.Cross.GetPrecisionScale()
/*		ShotStor.RicDecalType = DE_PLASMA		//DE_BULLET, DE_PLASMA	//zeby sme do toho systemu nemontovali granaty?
		ShotStor.RicSndType = SE_BULLET			//SE_BULLET, SE_PLASMA
		ShotStor.RicParticlesType = PE_BULLET	//PE_BULLET, PE_PLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)*/

		Ammo--
		clamp Ammo<0, GrenadeAmmoPerMagazine>
		Owner.UpdateHUD(1)	//ammo bar

		RemoveChild(this, ActionProj)
		ActionProj.ImpactScale = ThrowPow
		ActionProj.Fire(ShotStor)
		SetFlags(ActionProj, TFL_USER6)

		UpdateEntity(ActionProj)
		ActionProj = NULL
		ThrowPow = 0
		
/*		if(Owner.ClassNum == ClassNumPlayer && Ammo == 0 && Owner.Ammo[AmmoType] == 0)
		{
			InterruptAction()
			Owner.WeaponChange(Owner.GetBestAvailableWeapon(), true)
		}	*/
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(!Owner)
			return

		if(Action == 1)
		{
			if(AfterThrow == false)
			{
				ThrowPow += ftime
				clamp ThrowPow<0, 1>	
				int FireButtonBefore = FireButton
				FireButton = KEY(P_ATTACK, false)

				if(FireButtonBefore != 0 && FireButton == 0)
				{
					OffScreenPos = false
					AfterThrow = true
				}
			}

			if(Owner)
			{
				if(ThrowPow)
					Owner.SetThrowBar(ThrowPow, true)
				else
					Owner.SetThrowBar(0, false)
			}
		}
	}

//---------------------------------------------------------------------------------------------------------
	void AddPickedAmmo(int num)
	{
		if(Ammo == 0)
		{
			Ammo = 1
			num -= 1
		}

		int OwnAmmo = Owner.Ammo[AmmoType] + num
		clamp OwnAmmo<0, MaxAmmos[ClassNum]>
		Owner.Ammo[AmmoType] = OwnAmmo
	}

//-----------------------------------------------------------------
	bool CanSelect()
	{
		if(Ammo == 0 && Owner.Ammo[AmmoType] == 0)
			return false

		return true	
	}

//---------------------------------------------------------------------------------------------------------
	void Activate(int fl, ExtClass other)
	{
		if(other != World.Player)
			return

		CreatureClass cc = other
		WeaponClass wc = cc.GetWeaponPointer(ClassNum)

		if(wc)
		{
			int AllAmmo = cc.Ammo[AmmoGrenade] + Ammo
	
			if(AllAmmo >= GrenadeMaximumAmmo)	//neberieme to ked viac nemozeme zobrat
				return
		}

		if(World.ManipEntity == this)
			return
		
		status = 1
		SetPickable(false)
		RemoveBodyAndGeom()
		SetEventMask(this, EV_FRAME)
		World.NumFlyingItems++
//		PlaySound(this, g_sCommonSounds[1], SF_ONCE)
	}	

//-----------------------------------------------------------------
	void ReloadAction()	//z animhooku playera
	{
		LoadMagazine()		
		Owner.UpdateHUD(1)	//ammo bar
	
		if(!ActionProj)
		{
			ActionProj = new ProjectileGrenade(2)
			Throw(ActionProj, this, EV_INIT, 0)
			ActionProj.damage = ShotStor.damage
			ClearFlags(ActionProj, TFL_FULLMATRIX)
			AddChild(this, ActionProj, 1)
			SetOrigin(ActionProj, Vector(-1.984,0,0))	//musi sa naofsetit pretoze taziste ma v strede koli fyzike
		}

		EndSound(ActionSound)
		ActionSound = PlaySound(Owner, Sounds[0], SF_ONCE)
	}
 
//-----------------------------------------------------------------
	void ShootAction()	//z animhooku playera
	{
		Fire()

		if(Owner.ClassNum == ClassNumPlayer && Ammo == 0 && Owner.Ammo[AmmoType] == 0)	//uz tu musime menit zbran aby to nevyzeralo dementne
		{
			InterruptAction()
			Owner.WeaponChange(Owner.GetBestAvailableWeapon(), true)
		}
	}

//-----------------------------------------------------------------
	void ShootAction2()
	{
		if(AfterThrow == false)	//ak sme este nepustili LMB tak animacka zostane visiet v tomto bode
			OffScreenPos = true
	}

//-----------------------------------------------------------------
	int Reload()
	{
		if(Ammo == AmmoPerMagazine)
			return false

		int a = Owner.Ammo[AmmoType]
		toload = a
		clamp toload<0, AmmoPerMagazine>
		int miss = AmmoPerMagazine - Ammo

		clamp toload<0, miss>

		if(ReloadAnim)
		{
			SetAnimSlot(Owner.Hands, s_weapon - 1, VNULL, 10.0, 0, NULL, 30, AF_RESET)
			SetAnimSlot(Owner.Hands, s_weapon, ReloadAnim, 10.0, 0, NULL, 30, AF_ONCE)

			if(Owner && Owner.ClassNum == ClassNumPlayer)
			{
				if(WeapReloadAnim)
					SetAnimSlot(this, 0, WeapReloadAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
			}
		}
		else
		{
			LoadMagazine()
			Owner.UpdateHUD(1)	//ammo bar
			SetAction(0)
		}

		PlaySound(Owner, Sounds[1], SF_ONCE)
		return true
	}

//-----------------------------------------------------------------
	int PrimaryFire(vector target)
	{
		if(ThrowPow)
			return false

		if(CheckForAmmo() == false)	//zistime ci mame strelivo na vystrel
			return false

		ThrowPow = 0
		OffScreenPos = false
		AfterThrow = false
		FireButton = true
		
		if(AttackAnim && Owner.Hands)
		{
			SetAnimSlot(Owner.Hands, s_weapon, AttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)

			if(Owner || Owner.ClassNum == ClassNumPlayer)
			{
				if(WeapAttackAnim)
					SetAnimSlot(this, 0, WeapAttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
			}
		} 

		return true
	}
	
//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			SelectObject(this, VNULL)
			ClearFlags(this, TFL_SOLID)
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	} 

//-----------------------------------------------------------------
	void item_weapon_grenade(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		name = "CGrenade"
		ClassNum = World.ItemColPtr.GetItemIndex(name)	//ClassNum = CMachineGun
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)
		MainObject = VNULL//GetObject("obj/weapons/grenade.xob")
		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)

		SelectModel(Spawner)

		AmmoPerMagazine = GrenadeAmmoPerMagazine
		Ammo = AmmoPerMagazine
		AmmoType = AmmoGrenade

		NumActions = 2
		NumShooters = 1
	
		Shooters[0] = new ShooterProjectile(this, AMMO_GRENADE1)	//sekundarna (granat)

		PrecisionFlawMin = 0
		PrecisionFlawMax = 0.0425
		RicSndSkip = 0

		ExactAimOffset = ZeroVec
		ExactAimAngOffset = ZeroVec
		ExactAimFOV = 0

		ADelay[0] = 3000 * sqrt(sqrt(sqrt(sqrt(g_timescale))))  		//primary attack
		ADelay[1] = 2.89			//reload
		ADelay[2] = 0.85 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//secondary attack

		ShotStor.damage = WDmgGrenade
		ShotStor.power = 23
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 28
		ShotStor.StreakWidth = 3  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 60  * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 5000
		ShotStor.StreakShader = 0
                ShotStor.RicDecalType = DE_PLASMA

//		Flash = new weapon_flash(this, "obj/weapons/weaflash1.xob", 0.1, "90 0 0", 1)

		IdleAnim = GetObject("anm/player/hands/Grenade_idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/Grenade_walk.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/Grenade_in.anm")			//player
		AttackAnim = GetObject("anm/player/hands/Grenade_fire.anm")			//player
//		AttackAnim2 = GetObject("anm/player/hands/machinegun_fire_secondary.anm")			//player
		SelectAnim = GetObject("anm/player/hands/Grenade_in.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/Grenade_out.anm")			//player
//		IdleVarAnims[0] = GetObject("anm/player/hands/machinegun_idle_var1.anm")		//player
//		IdleVarAnims[1] = GetObject("anm/player/hands/machinegun_idle_var2.anm")		//player

		InitSounds(WS_WeaponGrenade)
		SetEventMask(this, EV_ANIMEND)

		KickUp[PRIMARY] = 0.25		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.4
		Automatic[PRIMARY] = true
		Automatic[SECONDARY] = false
	}
}

const float MineFlyOffset = 5	//o kolko sa dvihne ked sa da do pohybu

//-----------------------------------------------------------------
class item_weapon_mine extends RigidBodyClass
{
	vector coords
	float angle, angle2, angle3
	float damage
	float health	//asi iba pre plamenomet
	string model
	string activatesound
	string explosionsound
	vobject MainObject
	int status
	dJoint AngLimJoint
	hsound ActivateSound
	hsound ExplosionSound
	int LightHandle
	dlight_dynamic DiscoLight
	misc_particle_effector MotorParticles
	float timer
	vector CtrlAngles
	hsound MotorSound
	int MotorSnd
	float UVPos, UVPos2	//pouzito v shaderu
	bool NeedExplosion

#ifdef EDITOR
	void EditorOpened()
	{
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		angle2 = atof(GetKeyValue("angle2"))
		angle3 = atof(GetKeyValue("angle3"))
		vector Angs = Vector(angle3, angle, angle2)

		if(GetParent() == NULL)
			SetTransformEx(Angs, coords)
		
		Show(true)
		UpdateEntity(this)
	}

	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		if(GetParent() == NULL)	//normalny stav
			SetTransformEx(Angs, Pos)
		else											//zrejme je na niecom naparentena
		{
			ClearFlags(this, TFL_FULLMATRIX)
			SetOrigin(this, Pos)	
			SetAngles(this, Angs)
		}

		UpdateMainVisualizer()
		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}
#endif

//-----------------------------------------------------------------
	void CleanUpEffects()
	{
		if(DiscoLight)
		{
			delete DiscoLight
			DiscoLight = NULL
		}

		if(LightHandle)
		{
			RemoveLight(LightHandle)
			LightHandle = 0
		}
		
		if(MotorParticles)
		{
			delete MotorParticles
			MotorParticles = NULL
		}
	}

//-----------------------------------------------------------------
	bool CanLogIn()
	{
		if(status == 0)
			return true
		
		return false
	}

//-----------------------------------------------------------------
	void LogIn()
	{
		SetEventMask(this, EV_FRAME)
		ClearEventMask(this, EV_VISIBLE)
		SetMass(1)
		CtrlAngles[0] = 0//45
		CtrlAngles[1] = 0
		CtrlAngles[2] = 0
		status = 10
		
		if(!MotorSnd)
			MotorSnd = PlaySound(this, MotorSound, SF_HALFRANGE)
			
		SetEAXEnvironment("$current", "Bullet", 100)
			
		if(!MotorParticles)
		{
			MotorParticles = new misc_particle_effector()
			MotorParticles.SetParms("particle/mine_drive.ptc", ZeroVec, true, 0)
			AddChild(this, MotorParticles, 0)
		}
		
		SetBone(this, B_mine, ZeroVec, Vector(MineFlyOffset, 0, 0), 1)
	}

//-----------------------------------------------------------------
	void LogOut()
	{
		ClearEventMask(this, EV_FRAME)
		SetEventMask(this, EV_VISIBLE)
		SetMass(50)
		status = 0
		
		if(MotorSnd)
		{
			EndSound(MotorSnd)
			MotorSnd = 0
		}
		
		SetEAXEnvironment("$current", "Bullet", 0)
		SetBone(this, B_mine, ZeroVec, ZeroVec, 1)
		CleanUpEffects()
	}

//-----------------------------------------------------------------
	void RemoveJoints()
	{
		if(AngLimJoint)
		{
			dJointDestroy(AngLimJoint)
			AngLimJoint = 0
		}
	}

//-----------------------------------------------------------------
	void StartExplosion()
	{
		CreatureClass pla = World.Player

		if(World.ManipEntity == this)
			pla.DropManipEntity(true)

		if(status == 10)
			pla.InterruptHacking(false)
		
		pla.RemoveHackAdept(this)
		if(HackInterf)	//ked uz niesme pod playerovou kontrolou tak by tam zostavali visiet widgety
		{
			delete HackInterf
			HackInterf = NULL
		}

		ActivateBody(false)
		Show(false)
		vector origin = GetMatrix(this, 3)
//		misc_particle_effector explode = new misc_particle_effector()
//		explode.SetParms("particle/explosion1.ptc", origin, true, 2)
		
		if(!LightHandle)
			LightHandle = AddLight(this, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 40, 128, "255 100 0")

		if(DiscoLight)
			DiscoLight.M_SwitchOff(this)	//vyhadzuje svetlo

		if(MotorParticles)
			MotorParticles.Switch(false)

		if(MotorSnd)
		{
			EndSound(MotorSnd)
			MotorSnd = 0
		}

		ClearFlags(this, TFL_SOLID)
		World.ApplyExplosion(this, GetMatrix(this, 3), 96, 300, damage, 1)
		PlaySound(this, ExplosionSound, SF_ONCE|SF_AMBIENT)
		SetEventMask(this, EV_FRAME)
		status = 4
		timer = 25
		NeedExplosion = false
	}

//-----------------------------------------------------------------
	int Shoot(ShotStorage shot)
	{
		if(shot.HurtType == HT_SHOT)
		{
			if(status < 4)
				StartExplosion()

			return 1
		}

		if(shot.HurtType == HT_FLAME)
		{
			if(status < 4)
			{
				health -= ftime * 150
				
				if(health <= 0)
					StartExplosion()
			}
			return 1
		}
		return 0
	}

//-----------------------------------------------------------------
	void EOnVisible(MinClass other, int extra)
	{
		if(status == 0)	//cakame na playera az sa priblizi
		{
			vector pos = GetMatrix(this, 3)
			vector PlayerPos = GetMatrix(World.Player, 3)
			vector dir = PlayerPos - pos
			float dist = VectorNormalize(dir)
			UVPos +=	ftime
			UVPos2 = UVPos

			if(dist < MineActivationDistance)
			{
				ClearEventMask(this, EV_VISIBLE)				//budeme zrychlovat a na naraz explodujeme
				SetEventMask(this, EV_FRAME)
				DiscoLight = new dlight_dynamic()
				DiscoLight.texture = "textures/lightcube/fluidsmall.dds"
				DiscoLight.intensity = 500
		 		DiscoLight.radius = 120
		 		DiscoLight._color = "1 0.4 0.1"
		 		Throw(DiscoLight, this, EV_INIT, 0)
		 		DiscoLight.M_SwitchOn(this)
		 		UpdateEntity(DiscoLight)
		 		SetOrigin(DiscoLight, "0 0 16")
		 		AddChild(this, DiscoLight, 1)		
				PlaySound(this, ActivateSound, SF_ONCE)	//ked dohraje zvuk, spusti sa OnFrame

				if(!LightHandle)
					LightHandle = AddLight(this, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 255, 16, "255 100 100")
				
				Color = 0x000000ff
				UVPos = 0
				SetMass(1)
				timer = 100
				status = 1
				
				CreatureClass pla = World.Player
				pla.RemoveHackAdept(this)	//odteraz sa do toho nemoze hackovat a entitta bude coskoro zmazana takze to musime playerovi vyhodit zo zoznamu adeptov
				if(HackInterf)	//ked uz niesme pod playerovou kontrolou tak by tam zostavali visiet widgety
				{
					delete HackInterf
					HackInterf = NULL
				}
			}
		}
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		vector Vel
		float VelLng
		float freqrate
		float freq

		if(NeedExplosion)
		{
			StartExplosion()
			return
		}

		if(status == 10)	//sme nalogovani
		{
			CtrlAngles[0] = CtrlAngles[0] - (g_fMouseSpeedV * (float)MouseDeltaY * 3)
			CtrlAngles[1] = CtrlAngles[1] - (g_fMouseSpeedH * (float)MouseDeltaX * 3)
			CtrlAngles[2] = 0
			
			if(CtrlAngles[0] > 80)
				CtrlAngles[0] = 80

			if(CtrlAngles[0] < -20)
				CtrlAngles[0] = -20

			vector tmpang = CtrlAngles
			float ff = tmpang[0]
			tmpang[0] = -ff

			vector LookMat[4]
			vector CameraMat[4]
			RollPitchYawMatrix2(CtrlAngles, LookMat)
			RollPitchYawMatrix2(tmpang, CameraMat)
			vector Pos = GetMatrix(this, 3)
			LookMat[3] = Pos

			vector Forward = LookMat[0]
			vector Left = LookMat[1]
			vector AAForward = Left * "0 0 1"
			vector CameraOffset = Forward * -128
			CameraMat[3] = Pos + Vector(0, 0, 6 + MineFlyOffset) //+ CameraOffset
			vector Force = ZeroVec

			if(KEY(LEFT, false))
				Force = Force + Left

			if(KEY(RIGHT, false))
				Force = Force - Left

			if(KEY(MOVE_F, false))
				Force = Force + AAForward

			if(KEY(MOVE_B, false))
				Force = Force - AAForward

			float ForceLng = VectorNormalize(Force)
			
			if(ForceLng == 0)
			{
				Vel = GetVelocity(this)
				Force = -Vel * 0.002
			}

			Force = Force * 10
			dBodyApplyForce(this, Force)
			SetCameraEx(1, CameraMat)
			
			if(MotorSnd)
			{
				Vel = GetVelocity(this)
				VelLng = VectorLength(Vel)
				freqrate = VelLng / 400
				clamp freqrate<0, 1>
				freq = 10000 + (freqrate * 30000)
				SetSoundFrequency(MotorSnd, freq)
			}
			
			UVPos +=	ftime * 1
			UVPos2 += ftime * 2
			return	
		}

		timer -= ftime * 100

		if(status == 1)
		{
			if(timer <= 0)
			{
				timer = 100
				status = 2
				
				if(!MotorSnd)
					MotorSnd = PlaySound(this, MotorSound, SF_HALFRANGE)
				
				if(!MotorParticles)
				{
					MotorParticles = new misc_particle_effector()
					MotorParticles.SetParms("particle/mine_drive.ptc", ZeroVec, true, 0)
					AddChild(this, MotorParticles, 0)
				}
			}
			return
		}

		if(status == 2)
		{
			float HeightOffset
			if(timer <= 0)
			{
				timer = 0
				HeightOffset = MineFlyOffset
				status = 3
			}
			else
			{
				float StatusProgress = timer / 100
				StatusProgress = 1 - StatusProgress
				StatusProgress = StatusProgress * StatusProgress
				HeightOffset = MineFlyOffset * StatusProgress
			}

			UVPos +=	ftime * 10
			UVPos2 += ftime * 2
			SetBone(this, B_mine, ZeroVec, Vector(HeightOffset, 0, 0), 1)

			if(MotorParticles)
				SetOrigin(MotorParticles, Vector(0, 0, HeightOffset))
			return
		}

		if(status == 3)	//priblizujeme sa k playerovi
		{
			UVPos +=	ftime * 10
			UVPos2 += ftime * 4
			vector pos = GetMatrix(this, 3)
			vector PlayerPos = GetMatrix(World.Player, 3)
			PlayerPos[2] = pos[2]
			vector dir = PlayerPos - pos
			float dist = VectorNormalize(dir)
			dBodyApplyForce(this, dir * 20)

			if(MotorSnd)
			{
				Vel = GetVelocity(this)
				VelLng = VectorLength(Vel)
				freqrate = VelLng / 400
				clamp freqrate<0, 1>
				freq = 10000 + (freqrate * 30000)
				SetSoundFrequency(MotorSnd, freq)
			}
			return
		}

		float rate
		vector color

		if(status == 4)	//vybuch prva faza
		{
			rate = timer / 25
			rate = 1 - rate
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 128, color)
			
			if(timer <= 0)
			{
				status = 5
				timer = 150
			}
			return
		}

		if(status == 5)	//vybuch druha faza
		{
			rate = timer / 150
			color = "255 100 0"
			color = color * rate
			SetLightEx(LightHandle, 0, 65, color)

			if(timer <= 0)
			{
				CleanUpEffects()
				RemoveJoints()
				RemoveBodyAndGeom()
				ClearEventMask(this, EV_ALL)
				ClearFlags(this, TFL_VISIBLE)
			}
		}
	}

//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		float KE = dBodyGetLastKineticEnergy(this)
		float ke = dBodyGetKineticEnergy(this)
		float KinDiff = KE - ke

		if(KinDiff > 5)
		{
			NeedExplosion = true
			SetEventMask(this, EV_FRAME)
		}
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetPickable(true)

		if(!model)
			model = "obj/weapons/mine.xob"

		if(!activatesound)
			activatesound = "Disp_effect_31"

		if(damage == 0)
			damage = WDmgMine

		health = 50	//pouzito iba pre plamenomet

		MainObject = GetObject(model)

		if(MainObject)
			SetObject(MainObject, _materialremap)

		SetMass(50)
		SetTransformEx(Vector(angle3, angle, angle2), coords)
		SetAngles(this, Vector(angle3, angle, angle2))

		if(CreateRigidBody(GEOMTYPE_NONE, "", model) == true)
		{
			AngLimJoint = dJointCreateAngularLimit(World, "0 0 1", this, "0 0 1", 10 * DEG2RAD, 0)
			ActivateBody(true)
		}

		ActivateSound = GetSoundEx(activatesound, 1, SF_3D)
		ExplosionSound = GetSoundEx("Weap_Grenade_expl", 1, SF_3D)
		MotorSound = GetSoundEx("mine01", 1, SF_3D|SF_FREQCONTROL)

		vector mins, maxs
		GetBoundBox(this, mins, maxs)
		maxs[2] = maxs[2] + MineFlyOffset
		SetBoundBox(this, mins, maxs)
	}

//-----------------------------------------------------------------
	void item_weapon_mine();

//-----------------------------------------------------------------
	void ~item_weapon_mine()
	{
		CleanUpEffects()
		RemoveJoints()
		RemoveBodyAndGeom()

		if(MainObject)
		{
			ReleaseObject(MainObject, 0)
			MainObject = VNULL
		}
	}
}


//=================================================================================
class MineInterface extends HackInterface
{
	item_weapon_mine mine
	func_display BackDisplay

//---------------------------------------------------------------------------------	
	bool CanLogin()
	{
		if(CurHackingTime != HackingTime)
			return false

		if(mine.status == 0)
			return true

		return false
	}

//---------------------------------------------------------------------------------	
	bool NeedLogOut()
	{
		if(mine.status == 10)
			return false

		return true
	}

//---------------------------------------------------------------------------------	
	void LogIn()
	{
		mine.LogIn()
		
		if(BackDisplay)
		{
			ShowWidget(BackDisplay.CanvasWdg, true)
			BackDisplay.RunAction("login")
		}
	}

//---------------------------------------------------------------------------------	
	void LogOut()
	{
		mine.LogOut()
		
		if(BackDisplay)
			ShowWidget(BackDisplay.CanvasWdg, false)
	}

//---------------------------------------------------------------------------------	
	vector GetVisualPos()
	{
		return GetMatrix(mine, 3)
	}

//---------------------------------------------------------------------------------	
	MinClass GetMainEntity()
	{
		return mine
	}

//---------------------------------------------------------------------------------	
	bool SetHackPreviewCamera()
	{
		vector mat[4]
		GetMatrix4(mine, mat)
		vector pos = mat[3] + Vector(0, 0, 6/* + MineFlyOffset*/)

		vector rotmat[3]
		RollPitchYawMatrix2("0 0 0", rotmat)
		
		vector resmat[4]
		MatrixMultiply3(mat, rotmat, resmat)
		
		resmat[3] = pos
		SetCameraEx(1, resmat)
		return true
	}

//---------------------------------------------------------------------------------	
	void OnFrame()
	{
		if(BackDisplay)
		{
			BackDisplay.UpdateTime()
			BackDisplay.UpdateElements()
		}
	}

//---------------------------------------------------------------------------------	
	void BeforeInit()
	{
		string NameIndex = itoa(++World.NumMines)
		name = "ExplosiveMine" + " " + NameIndex
		IconName = "AM" + NameIndex
		description = "Attack mine"
		IconType = 2
	}

//---------------------------------------------------------------------------------	
	void MineInterface(MinClass owner)
	{
		mine = owner
		
		if(World.MineDisplay == NULL)	//drzime iba jeden spolocny
			World.MineDisplay = new func_display(mine, "displays/turret00_overlay.dpl", true)
		
		BackDisplay = World.MineDisplay
		ShowWidget(BackDisplay.CanvasWdg, false)
	}

//---------------------------------------------------------------------------------	
	void ~MineInterface()
	{
		//Tady ne!! uz to maze world!
		//delete BackDisplay
		//BackDisplay = NULL
	}
}

//-----------------------------------------------------------------
void item_weapon_mine::item_weapon_mine()
{
	SetFlags(this, TFL_VISIBLE)
	SetEventMask(this, EV_VISIBLE|EV_CONTACT)
	Color = 0x0000ff00
	HackInterf = new MineInterface(this)
}




//=================================================================================
class item_weapon_rocketlauncher extends FireWeaponClass
{
//-----------------------------------------------------------------
	int PrimaryFire(vector target)
	{
		if(CheckForAmmo() == false)	//zistime ci mame strelivo na vystrel
			return false

		Ammo--
		int sfl = ClearFlags(Owner, TFL_SOLID)
		vector targ, dir
		float prec = 0

		if(Flash)
			Flash.Switch(true)

		vector start

//		if(Owner.ClassNum == ClassNumPlayer)
//			start = g_CamMat[3]
//		else
			start = GetMatrix(this, 3)

		dir = target - start
		VectorNormalize(dir)

		Shooter shoter = Shooters[0]
		ShotStor.shooter = Owner
		target = target - start
		VectorNormalize(target)
		target = target * 8192 + start
		ShotStor.from = start
		ShotStor.to = target
		ShotStor.dir = dir

		if(Owner.ClassNum == ClassNumPlayer)
		{
			float FlawDiff = PrecisionFlawMax - PrecisionFlawMin
		 	ShotStor.precision = PrecisionFlawMin + (FlawDiff * World.Cross.GetPrecisionScale())
		}
		else
			ShotStor.precision = PrecisionFlawMax
			

		if(Flash)
			ShotStor.VisualsFrom = GetMatrix(Flash, 3)
		else
			ShotStor.VisualsFrom = start

		shoter.Fire(ShotStor)
		SetFlags(Owner, sfl)

		if(Owner != World.Player)	//player to pusta po uspesnom Action()
		{
			if(ActionSound)
				EndSound(ActionSound)
	
			ActionSound = PlaySound(Owner, Sounds[0], SF_ONCE)
		}

		if(AttackAnim)
		{
			if(Owner.Hands)
				SetAnimSlot(Owner.Hands, s_weapon, AttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}
		
		if(Owner && Owner.ClassNum == ClassNumPlayer)
		{
			if(WeapAttackAnim)
				SetAnimSlot(this, 0, WeapAttackAnim, 0, 0, NULL, 30, AF_ONCE|AF_RESET)
		}

//		World.Quake(16, 2, 2, 8, 8, 1)	
		return true
	}

//-----------------------------------------------------------------
	int SecondaryFire(vector target)
	{
		return false
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			if(spawner.ClassNum == ClassNumPlayer)
			{
				SetObject(MainObject, "$remap 'rocketlauncher' 'rocketlauncher_fps';")
				
				if(WeapIdleAnim)
					SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0)
			}
			else
				SetObject(MainObject)
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	}

//-----------------------------------------------------------------
	void item_weapon_rocketlauncher(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true

		name = "CRocketLauncher"
		ClassNum = World.ItemColPtr.GetItemIndex(name)
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
			MainObject = GetObject("obj/weapons/rocketlauncher.xob")			//normalny model
		else
			MainObject = GetObject("obj/weapons/rocketlauncher_enemy.xob")

		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)
		ShellType = 1
//		AimAnim = GetObject("anm/player/hands/MachineGun_aim.anm")

		SelectModel(Spawner)

		AmmoPerMagazine = RocketLauncherAmmoPerMagazine
		Ammo = AmmoPerMagazine
		AmmoType = AmmoRocketLauncher

		NumActions = 4
		NumShooters = 1
		Shooters[0] = new ShooterProjectile(this, AMMO_ROCKET1)

		PrecisionFlawMin = 0.004
		PrecisionFlawMax = 0.016
		RicSndSkip = 3

		ExactAimOffset = "0 0 0"
		ExactAimAngOffset = "0 0 0"
		ExactAimFOV = -1	//tymto sa definuje ze u zbrane nechceme ExactAim mod vobec

		ADelay[0] = 2.4 * sqrt(sqrt(sqrt(sqrt(g_timescale))))  		//primary attack
		ADelay[1] = 2.0		//reload
		ADelay[2] = 0.9 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//secondary attack

		ShotStor.damage = WDmgRocketLauncher
		ShotStor.power = 10
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 202
		ShotStor.StreakWidth = 25 * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakLength = 5100 * sqrt(sqrt(1 / g_timescale))
		ShotStor.StreakSpeed = 185  * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 4
		ShotStor.RicDecalType = DE_BULLET
		ShotStor.RicSndType = SE_BULLET
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			ShotStor.RicParticlesType = PE_BULLET
		else
			ShotStor.RicParticlesType = PE_PLAYERBULLET
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flash = new weapon_flash(this, 0.1, "90 0 0", 1)
//Replacing $/ProjectX/anm/player/hands/RocketLauncher_fire.anm
//Getting $/ProjectX/anm/player/hands/RocketLauncher_reload.anm


		IdleAnim = GetObject("anm/player/hands/rocketlauncher_idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/rocketlauncher_idle.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/rocketlauncher_reload.anm")			//player
		AttackAnim = GetObject("anm/player/hands/rocketlauncher_fire.anm")			//player
		SelectAnim = GetObject("anm/player/hands/rocketlauncher_in.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/rocketlauncher_out.anm")			//player
		IdleVarAnims[0] = GetObject("anm/player/hands/rocketlauncher_idle.anm")		//player
		IdleVarAnims[1] = GetObject("anm/player/hands/rocketlauncher_idle.anm")		//player

		//WeapAttackAnim = GetObject("anm/weapons/rocketlauncher_fire.anm") 
		WeapIdleAnim = GetObject("anm/weapons/rocketlauncher_idle.anm")
		WeapReloadAnim = GetObject("anm/weapons/rocketlauncher_reload.anm")

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
		{
			if(WeapIdleAnim)
				SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0) 
		}

		InitSounds(WS_WeaponRocketLauncher)
		SetEventMask(this, EV_ANIMEND)

		KickUp[PRIMARY] = 0.55		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.5
		Automatic[PRIMARY] = false
		Automatic[SECONDARY] = false
	}

//-----------------------------------------------------------------
	void ~item_weapon_rocketlauncher()
	{
	}
}



const float FlameHurtLength = 160.3	//5m
const vector FlameLightColor = "240 140 40"
const int NumPlayerFlameHurtSamples = 15

int LastFlameID
//=================================================================================
class item_weapon_flamethrower extends FireWeaponClass
{
	misc_particle_effector Flame
	misc_particle_effector Burner
        misc_particle_effector SmokeEffector
	DynamicOverlaySprite RefractionSprite
        float SmokePower
        float OrginalEffectTime
	float FiringCounter, FiringLightCounter
	int LightHandle
	float fAmmo	//tu musi byt ammo ako float
	bool Firing
	vector HurtPtsOffsetsStart[NumPlayerFlameHurtSamples]
	vector HurtPtsOffsetsEnd[NumPlayerFlameHurtSamples]
	int NumFlameHurtSamples
	float FlameEmitorVelocities[8]
	vector FlameEmitorConeAngles[8]
	float FlameEmitorBirthRates[8]
	int NumFlameEmitors
	MinClass LightOwner
	vector LightOffset

//-----------------------------------------------------------------
	void Show(bool show)
	{
		if(show)
			SetFlags(this, TFL_VISIBLE)
		else
			ClearFlags(this, TFL_VISIBLE)
			
		Burner.Show(show)
	}

//-----------------------------------------------------------------
	int Action(int n, vector targ)
	{
		if(!CanAct(n))
			return false

//		if(Action)					//toto je u plamenometu v rukach potvory neziaduce
//			InterruptAction()

		switch(n)
		{
			case 1:
				if(PrimaryFire(targ))
				{
					SetAction(n)
					return true
				}
			break
			
			case 2:
				if(Reload())
				{
					SetAction(n)
					return true
				}
			break
			
			case 3:	//sekundarny mod
				if(SecondaryFire(targ))
				{
					SetAction(n)
					return true
				}
		}

		SetAction(0)
		return false
	}

//-----------------------------------------------------------------
	int SecondaryFire(vector target)
	{
		return false
	}

//-----------------------------------------------------------------
	void SelectModel(ExtClass spawner)
	{
		if(spawner)	//berieme to rovno do ruk pretoze entita nebola vytvorena z mapy
		{
			DummyObject = VNULL	//system itemov objekt po skonceni mapy maze
			if(spawner.ClassNum == ClassNumPlayer)
			{
				SetObject(MainObject, "$remap 'flamethrower' 'flamethrower_fps';")
				
				if(WeapIdleAnim)
					SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0)
			}
			else
				SetObject(MainObject)
		}
		else	//bude lezat v levele ako fyzikalny objekt
			SelectObject(this, DummyObject)
	}

//-----------------------------------------------------------------
	void SetAction(int act)
	{
		Action = act
		ActionTime = 0
		
		if(Action)
			SetEventMask(this, EV_FRAME)

		if(Action)
		{
			if(Owner && MainObject)
				SetAnimSlot(this, s_walk - 1, VNULL, 10.0, 0, NULL, 30, 0)	//idle var musi skoncit aby po skonceni animacie prave zacinajucej akcie nebezal
		}
	}

//-----------------------------------------------------------------
	void LoadMagazine()
	{
		Ammo += toload
		Owner.Ammo[AmmoType] = Owner.Ammo[AmmoType] - toload
		toload = 0
		fAmmo = Ammo
	}

//-----------------------------------------------------------------
/*
to je asi nejaky spatny vtip, ne?
Flame neni solid
this neni solid
a u Owner se da TFL_SOLID vypnout!

	bool FlameTraceFilter(MinClass other)
	{
		if(other == Flame || other == this || other == Owner)
			return false
		
		return true
	}
*/

	vector HurtLineEnd
	float LastNotifyTime

//-----------------------------------------------------------------
	void UpdateBurning(bool FirstTime)
	{
		if(Ammo != -1)
		{
			fAmmo -= ftime * 50
			Ammo = fAmmo
			clamp Ammo<0, FlameThrowerAmmoPerMagazine>
		}

		Owner.UpdateHUD(1)	//ammo bar

		vector HurtLineStart
		vector IdealHurtLineEnd
		vector IdealHurtLineVec
		vector HurtLineVec
		vector FlameMat[4]
		GetMatrix4(Flame, FlameMat)
		HurtLineStart = FlameMat[3]
		IdealHurtLineEnd = FlameMat[2] * FlameHurtLength	//6m
		IdealHurtLineEnd = HurtLineStart + IdealHurtLineEnd

		if(FirstTime)
			HurtLineEnd = IdealHurtLineEnd

		IdealHurtLineVec = IdealHurtLineEnd - HurtLineStart
		HurtLineVec = HurtLineEnd - HurtLineStart

		if(!FirstTime)
		{
			vector HurtLineOff = IdealHurtLineEnd - HurtLineEnd
			float TimeStep = 20 * (ftime - (ftime * 0.5))		//TimeStep = 20 * ftime by malo byt spravne ale na nizkom fps nefunguje :-(
			vector HurtLineStep = HurtLineOff * TimeStep
			HurtLineEnd = HurtLineEnd + HurtLineStep
		}

		vector HurtLineDir = HurtLineVec
		float HurtLineLng = VectorNormalize(HurtLineDir)

		vector HurtSpace[4]
		GetAroundMatrixOfDir(HurtLineDir, HurtSpace)
		HurtSpace[3] = HurtLineStart
//		ShowMatrix(HurtSpace)

		vector CurLightPos = VectorMatrixMultiply4(HurtSpace, LightOffset)
		SetOrigin(LightOwner, CurLightPos)


		//scare monsters
		if(Owner == World.Player)
		{
			//limits notification frequency
			float df = currenttime - LastNotifyTime
			if(df > 1.0)
			{
				LastNotifyTime = currenttime
				NotifyNoises(Owner, HurtLineEnd, NOISERANGE_FLAME, NOISE_FLAME)
			}
		}

		//AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, HurtLineStart, HurtLineEnd)

		int content, sparm
		ExtClass other
		float plane[4]
		vector SampleStart, SampleEnd

		LastFlameID++
		int ofl = ClearFlags(Owner, TFL_SOLID)
		float LongestHurtRate = -1
		float AverageHurtRate = 0
	
		for(int s = 0; s < NumFlameHurtSamples; s++)
		{
			SampleStart = VectorMatrixMultiply4(HurtSpace, HurtPtsOffsetsStart[s])
			SampleEnd = VectorMatrixMultiply4(HurtSpace, HurtPtsOffsetsEnd[s])
			float HurtRate = TraceLineEx(DEFAULT, SampleStart, SampleEnd, ZeroVec, ZeroVec, other, plane, content, sparm, TRACE_RAGDOLLS|TRACE_DEFAULT|TRACE_BONES, NULL)
			AverageHurtRate += HurtRate

			if(HurtRate < 1)
			{
				if(HurtRate > LongestHurtRate)
					LongestHurtRate = HurtRate

				if(other != World && IsInherited(other, CExtClass))
				{
					//nezranuj 2x tim samym plamenem
					if(other.FlameID != LastFlameID)
					{
						other.FlameID = LastFlameID
	
						HurtRate *= HurtRate		//pri konci zranuje menej ale smerom k zbrani to rychle narasta
						HurtRate = 1 - HurtRate
			
						ShotStor.shooter = Owner
						ShotStor.from = SampleStart
						ShotStor.to = SampleEnd
						ShotStor.dir = HurtLineDir
						ShotStor.precision = 0
						ShotStor.VisualsFrom = SampleStart
			
						HurtRate *= 100 * ftime
						ShotStor.power = HurtRate * 200
						float DamageBefore = ShotStor.damage
						ShotStor.damage *= HurtRate
						ShotStor.Bone = -1
						other.Shoot(ShotStor)
						ShotStor.damage = DamageBefore
					}
				}
			}
/*			AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, SampleStart - "0.2 0.2 0.2", SampleStart + "0.2 0.2 0.2")
			AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, SampleEnd - "0.2 0.2 0.2", SampleEnd + "0.2 0.2 0.2")
			AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, SampleStart, SampleEnd)*/
		}
		
		if(LongestHurtRate == -1)
			LongestHurtRate = 1
			
		AverageHurtRate /= (float)NumFlameHurtSamples
	
		SetFlags(Owner, ofl)
	
/*		
		ExtClass HurtAdepts[16]
		int HurtAdeptsNum = VisEntities(HurtLineStart, HurtLineDir, 15, FlameHurtLength, HurtAdepts, 16, TFL_VISIBLE)

		ExtClass Adept
		for(int n = 0; n < HurtAdeptsNum; n++)
		{
			Adept = HurtAdepts[n]
			
			if(Adept == this || Adept == Owner || Adept == Flame || Adept == Burner)
				continue

			if(Owner.Hands && Adept == Owner.Hands)
				continue

			if(!IsInherited(Adept, CExtClass))
				continue

//			if(!E2EVisibility(Flame, Oss, Adept, GetMatrix(Adept, 3), TRACE_VISTEST|TRACE_DETAIL|TRACE_ENTS|TRACE_PASSTRANSLUCENT))
//				continue

			vector pos = GetMatrix(Adept, 3)
			vector HurtDir = pos - HurtLineStart
			float Dist = VectorNormalize(HurtDir)
			float HurtRate = Dist / FlameHurtLength
			clamp HurtRate<0, 1>
			HurtRate = 1 - HurtRate

			ShotStor.shooter = Owner
			ShotStor.from = HurtLineStart
			ShotStor.to = pos//HurtLineEnd
			ShotStor.dir = HurtDir//HurtLineDir
			ShotStor.precision = 0
			ShotStor.VisualsFrom = HurtLineStart

			ShotStor.power = 200 * HurtRate * 100 * ftime// * HurtRate3	
			ShotStor.damage = WDmgFlameThrower * HurtRate * 100 * ftime// * HurtRate3
			ShotStor.Bone = -1
			Adept.Shoot(ShotStor)
		}*/

		float StrLen = FlameHurtLength// * 20000
		vector StrPos = HurtLineDir * StrLen
		StrPos = HurtLineStart + StrPos
		RefractionSprite.SetPos(HurtLineStart, StrPos)
		RefractionSprite.Update()

		for(int n = 0; n < NumFlameEmitors; n++)
		{
			float fval = FlameEmitorVelocities[n] * AverageHurtRate
			SetEffectorParm(Flame, n, EP_VELOCITY, fval)
			
			vector cang = FlameEmitorConeAngles[n]
			float flip = 1 - AverageHurtRate
			float ca = cang[1] + (cang[1] * flip * 4)

			if(n == 0)
				ca *= 0.5

			vector vval = Vector(cang[0], ca, cang[2])
			SetEffectorParm(Flame, n, EP_CONEANGLE, vval)
		}

//		vector vec = Vector(LongestHurtRate * FlameHurtLength, 0, 0)
//		SetEffectorParm(Flame, -1, EP_KILLBOX, vec)	//KillShapeSize

//		SetLightEx(LightHandle, 0, 128, color)
//		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, HurtLineStart, IdealHurtLineEnd)
//		AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, HurtLineStart, HurtLineEnd)
	}

//-----------------------------------------------------------------
	void BurningStart()
	{
		Firing = true
		UpdateBurning(true)
		Flame.Restart()
		Flame.Switch(true)
		Burner.Switch(false)
		RefractionSprite.Show(true)
		
		if(!LightHandle)
			LightHandle = AddLight(LightOwner, 0, LFL_POINT | LFL_DYNAMIC | LFL_CHEAP, 100, 128, ZeroVec)

		SetEventMask(this, EV_FRAME)

		if(AttackAnim)
		{
			if(Owner.Hands)
				SetAnimSlot(Owner.Hands, s_weapon, AttackAnim, 0, 0, NULL, 30, AF_RESET)
		}
		
		if(Owner && Owner.ClassNum == ClassNumPlayer)
		{
			if(WeapAttackAnim)
				SetAnimSlot(this, 0, WeapAttackAnim, 5, 0, NULL, 30, AF_ONCE|AF_RESET)
		}
		
		if(Owner != World.Player)
			ActionSound = PlaySound(Owner, Sounds[0], 0)
	}

//-----------------------------------------------------------------
	void BurningEnd()
	{
		Firing = false
		EndAction()
		FiringCounter = 0
//		ClearEventMask(this, EV_FRAME)
		Flame.Switch(false)
		Burner.Switch(true)
//		RefractionSprite.Show(false)

/*		if(LightHandle)
		{
			RemoveLight(LightHandle)
			LightHandle = 0
		}*/

		if(Owner.Hands)
			SetAnimSlot(Owner.Hands, s_weapon, VNULL, 30, 0, NULL, 0, 0)
		else
			SetAnimSlot(Owner, s_weapon, VNULL, 1, 0, NULL, 0, 0)

		if(ActionSound)
		{
			EndSound(ActionSound)
			ActionSound = 0
		}
	}

//-----------------------------------------------------------------
	void InterruptAction()
	{
//		if(Owner != World.Player)
//		{
			BurningEnd()
			Burner.Switch(false)
			RefractionSprite.Show(false)
			ClearEventMask(this, EV_FRAME)

			if(LightHandle)
			{
				RemoveLight(LightHandle)
				LightHandle = 0
			}
//		}
	}

//-----------------------------------------------------------------
	int PrimaryFire(vector target)
	{
		if(CheckForAmmo() == false)	//zistime ci mame strelivo na vystrel
			return false

		if(FiringCounter == 0)	//ohen nehori
			BurningStart()

		FiringCounter = 0.2
		return true
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(status > 0)	//prebieha pick zbrane v superclase
			return

		if(Firing)	//on frame pobezi az kym nezhasne svetlo
		{
                        SmokePower += ftime * 0.8

			FiringLightCounter += ftime * 10
			
			if(FiringLightCounter >= 1)
				FiringLightCounter = 1	
		}
		else
		{
			FiringLightCounter -= ftime * 5
			
			if(FiringLightCounter <= 0)
			{
                                clamp SmokePower<0, 1>
						float EffectTime = OrginalEffectTime * SmokePower
	/*
						SmokeEffector.SetParmToEffectors(EP_EFFECT_TIME, EffectTime)
	
						SmokeEffector.Restart()
						SmokeEffector.Switch(true)*/
				FiringLightCounter = 0
				
				if(!Action)	//mohol by bezat reload
					ClearEventMask(this, EV_FRAME)
				
				if(LightHandle)
				{
					RemoveLight(LightHandle)
					LightHandle = 0
				}
                                SmokePower = 0
				RefractionSprite.Show(false)
			}
		}

		if(LightHandle)
			SetLightEx(LightHandle, 100, 128, FlameLightColor * FiringLightCounter)
			
		RefractionSprite.ScaleRefract(FiringLightCounter)
		
//		UpdateBurning(false)

		if(FiringCounter > 0)
		{
			FiringCounter -= ftime
			
			if(FiringCounter <= 0)
			{
				FiringCounter = 0
				BurningEnd()
			}
			UpdateBurning(false)
			
			if(Ammo == 0)
				BurningEnd()
		}
	}

//-----------------------------------------------------------------
	void SetFlameBirthRateScale(float scale)
	{
		for(int n = 0; n < NumFlameEmitors; n++)
		{
			float fval = FlameEmitorBirthRates[n] * scale
			SetEffectorParm(Flame, n, EP_BIRTH_RATE, fval)
		}
	}

//-----------------------------------------------------------------
	void Select()
	{
		SetAngles(this, ZeroVec)
		SetOrigin(this, ZeroVec)
		SetFlags(this, TFL_VISIBLE)
		Owner.UpdateHUD(1)
		SetEventMask(this, EV_FRAME)
		fAmmo = Ammo
		Burner.Switch(true)

		if(MainObject)
			SetAnimSlot(this, 0, VNULL, 0, 0, NULL, 30, AF_RESET)	//vyhodime pripadny reload alebo fire anim

		BumpFrame(this, 0, 0)
	}

//-----------------------------------------------------------------
	void BeginUnselect()
	{
		Flame.Switch(false)
		Burner.Switch(false)
//		RefractionSprite.Show(false)
	}

//-----------------------------------------------------------------
	void EndAction()
	{
		if(Action != 1)	//pri fire zostava loopovat attack animacka
		{
			if(Owner.Hands)
				SetAnimSlot(Owner.Hands, s_weapon, VNULL, 30, 0, NULL, 0, 0)
			else
				SetAnimSlot(Owner, s_weapon, VNULL, 1, 0, NULL, 0, 0)
		}

		if(MainObject)
			SetAnimSlot(this, 0, VNULL, 0, 0, NULL, 30, 0)	//vyhodime pripadny reload alebo fire anim

		SetAction(0)
	}

//-----------------------------------------------------------------
	void item_weapon_flamethrower(ExtClass spawner)
	{
		Spawner = spawner
		IsPlayerWeapon = true
		name = "CFlameThrower"
		ClassNum = World.ItemColPtr.GetItemIndex(name)
		Definition = World.ItemColPtr.GetItemFromIndex(ClassNum)
		string FlameEffector, BurnerEfector

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
		{
			MainObject = GetObject("obj/weapons/flamethrower.xob")
			FlameEffector = "particle/flamethrower_fire.ptc"
			BurnerEfector = "particle/flamethrower_burner.ptc"
			NumFlameHurtSamples = NumPlayerFlameHurtSamples
		}
		else
		{
			MainObject = GetObject("obj/weapons/flamethrower_enemy.xob")
			FlameEffector = "particle/flamethrower_fireenemy.ptc"
			BurnerEfector = "particle/flamethrower_burner.ptc"
			NumFlameHurtSamples = 4	//enemakom staci menej
		}

		DummyObject = World.ItemColPtr.GetObjectFromIndex(ClassNum)
		ShellType = 1
//		AimAnim = GetObject("anm/player/hands/MachineGun_aim.anm")

		SelectModel(Spawner)

		AmmoPerMagazine = FlameThrowerAmmoPerMagazine
		Ammo = AmmoPerMagazine
		fAmmo = Ammo	//u plamenometu musime pracovat s floatami
		AmmoType = AmmoFlameThrower

		NumActions = 2
		NumShooters = 1

		Shooters[0] = new ShooterBullets(this, AMMO_BULLETS1)		//primarna strelba (samopal)

		PrecisionFlawMin = 0.012
		PrecisionFlawMax = 0.042
		RicSndSkip = 3

		ExactAimOffset = "0 0 0"
		ExactAimAngOffset = "-0.6 -0.8 0"
		ExactAimFOV = 5

		ADelay[0] = 0.16 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//primary attack
		ADelay[1] = 2.7		//reload
		ADelay[2] = 0.88 * sqrt(sqrt(sqrt(sqrt(g_timescale)))) 		//secondary attack

		ShotStor.damage = WDmgFlameThrower	//damage musi byt podla fps pretoze zranujeme OnFrame
		ShotStor.BulletRainInc = 3.38
                ShotStor.power = 120
		ShotStor.HurtType = HT_FLAME
		ShotStor.NumBullets = 1
		ShotStor.MarkSize = 70
		ShotStor.StreakWidth = 10
		ShotStor.StreakLength = 300
		ShotStor.StreakSpeed = 1000 * sqrt(sqrt(sqrt(sqrt(g_timescale))))
		ShotStor.StreakShader = 4
		ShotStor.RicDecalType = DE_BULLET		//DE_BULLET, DE_PLASMA
		ShotStor.RicSndType = -1			//SE_BULLET, SE_PLASMA
		if(spawner && spawner.ClassNum != ClassNumPlayer)
			ShotStor.RicParticlesType = PE_BULLET
		else
			ShotStor.RicParticlesType = PE_PLAYERBULLET	//PE_BULLET, PE_PLASMA
		InitRicParticles(ShotStor.RicParticlesType)
		InitRicSounds(ShotStor.RicSndType)

		Flame = new misc_particle_effector()
		Flame.SetParms(FlameEffector, ZeroVec, true, 0)
		Flame.Switch(false)

		Burner = new misc_particle_effector()
		Burner.SetParms(BurnerEfector, ZeroVec, true, 0)
		Burner.Switch(false)

                SmokeEffector = new misc_particle_effector
		SmokeEffector.SetParms("particle/GatlingLE_smoke.ptc", "-2 11 4", true, 1)
		SmokeEffector.Switch(false)
		GetEffectorParm(SmokeEffector, 0, EP_EFFECT_TIME, OrginalEffectTime)
		AddChild(this, SmokeEffector, 4)

		Flame.AnimFlags |= 2
		Burner.AnimFlags |= 2

		AddChild(this, Flame, 0)
		SetAngles(Flame, "180 0 0")

		AddChild(this, Burner, 1)
		SetAngles(Burner, "0 0 -90")
		
		RefractionSprite = new DynamicOverlaySprite(ZeroVec, ZeroVec, 80, "flameheat")
		RefractionSprite.Show(false)

		IdleAnim = GetObject("anm/player/hands/flamethrower_idle.anm")				//player
		WalkAnim = GetObject("anm/player/hands/flamethrower_walk.anm")				//player
		ReloadAnim = GetObject("anm/player/hands/flamethrower_reload.anm")			//player
		AttackAnim = GetObject("anm/player/hands/flamethrower_fire.anm")			//player
		SelectAnim = GetObject("anm/player/hands/flamethrower_in.anm")				//player
		UnselectAnim = GetObject("anm/player/hands/flamethrower_out.anm")			//player
		IdleVarAnims[0] = GetObject("anm/player/hands/flamethrower_idle.anm")		//player
		IdleVarAnims[1] = GetObject("anm/player/hands/flamethrower_idle.anm")		//player

//		WeapIdleAnim = GetObject("anm/weapons/machinegun_idle.anm")
		WeapReloadAnim = GetObject("anm/weapons/flamethrower_reload.anm")

		if(!spawner || spawner.ClassNum == ClassNumPlayer)
		{
			if(WeapIdleAnim)
				SetAnimSlot(this, 1, WeapIdleAnim, 0, 0, NULL, 30, 0) 
		}

		InitSounds(WS_WeaponFlameThrower)
		SetEventMask(this, EV_ANIMEND)

/*		SmokeEffector = new misc_particle_effector
		SmokeEffector.SetParms("particle/GatlingLE_smoke.ptc", "-2 11 4", true, 1)
		SmokeEffector.Switch(false)
		GetEffectorParm(SmokeEffector, 0, EP_EFFECT_TIME, OrginalEffectTime)
		AddChild(this, SmokeEffector, 4)*/

		KickUp[PRIMARY] = 0.022		//ako to dviha zbran pri strelbe
		KickUp[SECONDARY] = 0.001
		Automatic[PRIMARY] = true
		Automatic[SECONDARY] = false

		int NumAroundSamples = NumFlameHurtSamples - 1
		float angstep = 360 / (float)NumAroundSamples
		float sangle = 0
		vector vec
		float StartHurtRadius = 0.5
		float EndHurtRadius = 49.5

		HurtPtsOffsetsStart[0] = ZeroVec
		HurtPtsOffsetsEnd[0] = Vector(FlameHurtLength, 0, 0)

		for(int s = 1; s < NumFlameHurtSamples; s++)
		{
			float CurAng = sangle * DEG2RAD
			vec[1] = sin(CurAng)
			vec[2] = cos(CurAng)
			vec[0] = 0
			vector off = vec * StartHurtRadius
//			off[0] = off[0] - 20
			HurtPtsOffsetsStart[s] = off
			
			off = vec * EndHurtRadius
			off[0] = FlameHurtLength
			HurtPtsOffsetsEnd[s] = off
			sangle += angstep
		}

		NumFlameEmitors = GetEmitors(Flame, NULL, 0)
		float fval
		vector vval
		for(int n = 0; n < NumFlameEmitors; n++)
		{
			GetEffectorParm(Flame, n, EP_VELOCITY, fval)
			FlameEmitorVelocities[n] = fval
			
			GetEffectorParm(Flame, n, EP_CONEANGLE , vval)
			FlameEmitorConeAngles[n] = vval
			
			GetEffectorParm(Flame, n, EP_BIRTH_RATE , fval)
			FlameEmitorBirthRates[n] = fval
		}
		
		LightOwner = new MinClass
		LightOffset = Vector(FlameHurtLength * 0.75, 0, 0)
	}

//-----------------------------------------------------------------
	void ~item_weapon_flamethrower()
	{
		if(ActionSound)
		{
			EndSound(ActionSound)
			ActionSound = 0
		}

		if(RefractionSprite)
		{
			delete RefractionSprite
			RefractionSprite = NULL	
		}
		
		if(Flame)
		{
			delete Flame
			Flame = NULL
		}
		
		if(Burner)
		{
			delete Burner
			Burner = NULL	
		}
		
		if(LightOwner)
		{
			delete LightOwner
			LightOwner = NULL	
		}
/*		if(SmokeEffector)
		{
			delete SmokeEffector
			SmokeEffector = NULL
		}*/
	}
}

