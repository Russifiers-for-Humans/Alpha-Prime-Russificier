const float PATH_SEGMENT = 16

class info_path;

int CurvePath(info_path path, info_path next, vector pts[], float minsegment);
info_path FindPrev(info_path p);

#ifdef EDITOR	
	int wprec //recursion stopper
#endif

//=================================================================================
class info_path extends ExtClass
{
	vector coords
	string OnArrival
	string link
	string prev
	float FloatTest
	
//-----------------------------------------------------------------
#ifdef EDITOR	
	Shape line

	void DestroyCustomVisuals()
	{
		if(line)
		{
			line.Destroy()
			line = NULL
		}
	}

	void UpdateCustomVisuals()
	{
		DestroyCustomVisuals()

		vector curve[24]
		int numcurved

		numcurved = CurvePath(this, NULL, curve, PATH_SEGMENT)

		if(numcurved)
			line = AddDLines(0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, curve, numcurved)
			
		info_path next = FindClass(link)

		if(wprec)
			return
		
		if(next)
		{
			if(!wprec)
			{
				wprec++
				next.UpdateCustomVisuals()
				wprec--
			}
		}
		
		next = FindPrev(this)

		if(next)
		{
			if(!wprec)
			{
				wprec++
				next.UpdateCustomVisuals()
				wprec--
			}
		
			next = FindPrev(next)
	
			if(next)
			{
				if(!wprec)
				{
					wprec++
					next.UpdateCustomVisuals()
					wprec--
				}
			}
		}
	}

	void CreateCustomVisuals()
	{
		DestroyCustomVisuals()

		vector curve[24]
		int numcurved
		numcurved = CurvePath(this, NULL, curve, PATH_SEGMENT)
		if(numcurved)
			line = AddDLines(0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, curve, numcurved)
	}
#endif

//-----------------------------------------------------------------
	void M_SetLink(ExtClass other)
	{
		info_path p = FindClass(other._value)
		
		if(p && IsInherited(p, CInfoPath)
		{
			link = p._name
			p.prev = _name
		}
	}
	
//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{	
		info_path p = FindPrev(this)

		if(p)
			prev = p._name

		CallMethods(this, OnLoad)
	}
	
//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{	
		SetOrigin(this, coords)
	}
	
//-----------------------------------------------------------------
	void info_path()
	{
		SetEventMask(this, EV_INIT|EV_LOADED)
	}	
}

//-----------------------------------------------------------------
info_path FindPrev(info_path p)
{
	for(info_path ec = GetNextEntity(NULL, CInfoPath); ec != NULL; ec = GetNextEntity(ec, CInfoPath))
	{
		if(ec.link == p._name)
			return ec		
	}
	return NULL
}

//-----------------------------------------------------------------
// z aktualniho waypointu, predchoziho a dvou nasledujicich prolozi
// krivku po segmentech maximalni delky minsegment
//-----------------------------------------------------------------
int CurvePath(info_path path, info_path next, vector pts[], float minsegment)
{	
	//ending point
	if(next == NULL)
	{
		next = FindClass(path.link)
		if(next == NULL)
			return 0
	}

	vector pos0, pos1, pos2, pos3

	pos1 = GetOrigin(path)
	pos2 = GetOrigin(next)

	//at least 2 points
	pts[0] = pos1
	pts[1] = pos2

	vector vec1 = pos2 - pos1
	float length = VectorNormalize(vec1)
	
	//it's too short. Don't curve at all
	if(length <= minsegment)
		return 2

	//previous
	info_path prev
	
	if(path.prev)
		prev = FindClass(path.prev)	
	else
		prev = FindPrev(path)

	if(prev)
		pos0 = GetOrigin(prev)
	else
		pos0 = pos1 - (pos1 - pos2)

	info_path next2 = NULL
	info_path next3

	next2 = FindClass(next.link)

	if(next2)
		pos3 = GetOrigin(next2)
	else
		pos3 = pos2 + (pos2 - pos1)

	vector vec0 = pos1 - pos0
	vector vec2 = pos3 - pos2
	VectorNormalize(vec0)
	VectorNormalize(vec2)

	//is it curved enough?
	float d0 = vec0 * vec1
	float d1 = vec2 * vec2
	
	if(d0 >= 0.9999 && d1 >= 0.9999)
		return 2

	int numcurved = length + (minsegment - 1) / minsegment
	clamp numcurved<0, sizeof(pts)>

	for(int n = 0; n < numcurved; n++)
	{
		pts[n] = CatmullRom(pos0, pos1, pos2, pos3, n / (numcurved - 1))
	}
	
	return numcurved
}

const int KS_NONE = 0
const int KS_STOP = 1
const int KS_UPDATE = 2
const int KS_IDLE = 3

const int KS_FORWARD = 4
const int KS_BACKWARD = 5

//=================================================================================
class Keyframer extends _serializable
{
	float speed
	string OnForward, OnBack, OnBeginStop, OnStop

	vector CarrierOffset
	float CarrierAngle
	hsound MotorSound, StartSound, StopSound
	int StartSnd, MotorSnd, StopSnd
	bool Updated
	int status

	ExtClass Owner
	info_path Current, Next
	info_path LastWaypoint

	vector NextVec, PrevVec
	vector Curve[24]
	int NumCurved
	int CurrentSegment
	vector PathDir
	float PathLength

	float TargetVelocity
	float CurrentVelocity
	float LastVelocityTime

	bool unlockz

	vector CurrentMatrix[4]
	bool NeedStopEvent

//-----------------------------------------------------------------
	void Stop(bool immed)
	{
		if(TargetVelocity == 0)
			return

		TargetVelocity = 0
		status = KS_STOP

		if(immed)
		{
			CurrentVelocity = 0
			NeedStopEvent = false
//			Print("Begin stop")
			CallMethods(Owner, OnBeginStop)
//			Print("Stop")
			CallMethods(Owner, OnStop)
		}
		else
		{
/*			bool StopEvent = false
			
			if(Owner)
			{
				float RealSpeed = VectorLength(GetVelocity(Owner))
				
				if(RealSpeed > 0 && status != KS_STOP)
					StopEvent = true
			}*/
	
/*			if(StopEvent)
			{
				status = KS_STOP
	
				if(StopSound)
				{
					if(!StopSnd)
						PlaySound(Owner, StopSound, SF_ONCE)
				}
				NeedStopEvent = true
				CallMethods(Owner, OnBeginStop)
			}*/
			NeedStopEvent = true
			
			if(StopSound)
			{
				if(!StopSnd)
					PlaySound(Owner, StopSound, SF_ONCE)
			}

//			Print("Begin stop")
			CallMethods(Owner, OnBeginStop)
		}
	}

//-----------------------------------------------------------------
	void Forward()
	{
		if(TargetVelocity > 0)
			return

		LastVelocityTime = -1

		TargetVelocity = speed
		StartSnd = 0
//		Print("Need Forward")
	}

//-----------------------------------------------------------------
	void Back()
	{
		if(TargetVelocity < 0)
			return

		LastVelocityTime = -1

		TargetVelocity = -speed
		StartSnd = 0
//		Print("Need Backward")
	}

//-----------------------------------------------------------------
	void SetSpeed(float sp)
	{
		speed = sp
	}

//-----------------------------------------------------------------
	void ProcessWaypoint(info_path p)
	{
		//jeste jsme ho neopustili
		if(LastWaypoint == p)
			return

		LastWaypoint = p
		CallMethods(Owner, p.OnArrival)
	}

//-----------------------------------------------------------------
	bool _Update(float vel, vector origin, float dt)
	{	
		//najdi presny bod na poloprimce
		vector from = Curve[CurrentSegment]

		float dist = PathDir * origin
		dist = PathDir * from - dist
		clamp dist<dist, 0>

		vector intersect = from + (PathDir * -dist)
		
		vector totarget
		float tdot
		
		//zjisti jestli jsme uz na konci segmentu
		if(vel > 0)
			totarget = Curve[CurrentSegment + 1] - intersect
		else
			totarget = intersect - Curve[CurrentSegment]

		VectorNormalize(totarget)
		tdot = totarget * PathDir
		
		bool nextwp = false
		//uz jsme tam, nebo jsme to dokonce prejeli
		if(tdot <= 0)
		{
			info_path nextpath
			
			if(vel > 0)
			{
				//jsme na konci krivky?
				int nc = NumCurved - 2
				if(CurrentSegment >= nc)
				{
					ProcessWaypoint(Next)
					nextpath = FindClass(Next.link)
					if(nextpath == NULL)
						return false

					nextwp = true
					Current = Next
					Next = nextpath	
					NumCurved = CurvePath(Current, Next, Curve, PATH_SEGMENT)
					CurrentSegment = 0
					NextVec = ZeroVec
					PrevVec = ZeroVec
				}
				else
					CurrentSegment++
			}
			else
			{
				//jsme na zacatku krivky?
				if(CurrentSegment <= 0)
				{
					ProcessWaypoint(Current)
					nextpath = FindClass(Current.prev)
					if(nextpath == NULL)
						return false

					nextwp = true
					Next = Current
					Current = nextpath
					NumCurved = CurvePath(Current, Next, Curve, PATH_SEGMENT)
					CurrentSegment = NumCurved - 2
					NextVec = ZeroVec
					PrevVec = ZeroVec
				}
				else
					CurrentSegment--
			}

			//vektor aktualniho segmentu
			PathDir = Curve[CurrentSegment + 1] - Curve[CurrentSegment]
			PathLength = VectorNormalize(PathDir)

			//bod intersekce uz nesouhlasi, ihned prepocitej.
			//kdybysme to nechali na dalsi krok, tak tam pak bude neprijemny skok
			from = Curve[CurrentSegment]

			dist = PathDir * origin
			dist = PathDir * from - dist
			clamp dist<dist, 0>
			intersect = from + (PathDir * -dist)
		}

		//nepotkali jsme zadny waypoint, tak to vyresetuj
		if(!nextwp)
		{
			LastWaypoint = NULL
		}
	
		//uprav uhel, aby korespondoval s trasou:
		
		//jak jsme daleko na aktualnim segmentu (0...1)
		tdot = from * PathDir
		tdot = intersect * PathDir - tdot / PathLength
		clamp tdot<0, 1>
		
		vector prevvec, nextvec
		
		vector pts[24]

		if(vel > 0)
		{
			//vezmi vektor nasledujiciho segmentu a interpoluj
			nc = CurrentSegment + 2
			if(nc < NumCurved)
			{
				nextvec = Curve[CurrentSegment + 2] - Curve[CurrentSegment + 1]
				VectorNormalize(nextvec)
			}
			else
			{
				if(NextVec == ZeroVec)
				{
					//jsme na konci krivky, musime si predpocitat dalsi usek
					if(Next)
					{
						//TODO: HMPF!!! :-P
						CurvePath(Next, NULL, pts, PATH_SEGMENT)
	
						NextVec = pts[1] - pts[0]
						VectorNormalize(NextVec)
					}
					else
					{
						NextVec = PathDir
					}
				}
				nextvec = NextVec
			}

			//interpoluj
			nextvec = nextvec - PathDir * tdot + PathDir
		}
		else
		{
			//vezmi vektor predchoziho segmentu a interpoluj
			if(CurrentSegment > 0)
			{
				prevvec = Curve[CurrentSegment] - Curve[CurrentSegment - 1]
				VectorNormalize(prevvec)
			}
			else
			{
				if(PrevVec == ZeroVec)
				{
					nextpath = FindClass(Current.prev)
					if(nextpath)
					{
						//TODO: HMPF!!! :-P
						nc = CurvePath(nextpath, Current, pts, PATH_SEGMENT)
	
						PrevVec = pts[nc - 1] - pts[nc - 2]
						VectorNormalize(PrevVec)
					}
					else
					{
						PrevVec = PathDir
					}
				}
				prevvec = PrevVec
			}
			
			tdot = 1 - tdot
			//interpoluj
			nextvec = prevvec - PathDir * tdot + PathDir
		}

		vector angs = Vector2Angles(nextvec)
		
		if(unlockz)
		{
			RollPitchYawMatrix(Vector(0, angs[1] + CarrierAngle, angs[0]), CurrentMatrix)
		}
		else
		{
			RollPitchYawMatrix(Vector(0, angs[1] + CarrierAngle, 0), CurrentMatrix)
		}

		//spocitej novou pozici z aktualni pozice + rychlosti
		float scale = vel * dt
		CurrentMatrix[3] = PathDir * scale + intersect
		return true
	}

//-----------------------------------------------------------------
	bool IsOK()
	{
		if(Current && Next)
			return true
		
		return false
	}

//-----------------------------------------------------------------
	void ProcessEvents()
	{
		if(CurrentVelocity == 0)
		{
			if(MotorSnd)
			{
				EndSound(MotorSnd)
				MotorSnd = 0
			}
			
			if(status == KS_STOP)	//uz sme zacali zastavovat
			{
				if(NeedStopEvent)
				{
					NeedStopEvent = false
					CallMethods(Owner, OnStop)
//					Print("Stop")
				}
			}
		}
		else
		{
			if(Owner)
			{
				float RealSpeed = VectorLength(GetVelocity(Owner))

				if(TargetVelocity > 0)	//je poziadavka na chod dopredu
				{
					if(status != KS_FORWARD)
					{
						if(RealSpeed > 5)	//az ked mame rychlost tak vykoname OnStart a zapiname status KS_FORWARD
						{
							status = KS_FORWARD

							if(StartSound && !StartSnd)
								StartSnd = PlaySound(Owner, StartSound, SF_ONCE)
								
							CallMethods(Owner, OnForward)
//							Print("OnForward")
						}
					}
				}

				if(TargetVelocity < 0)	//je poziadavka na chod dozadu
				{
					if(status != KS_BACKWARD)
					{
						if(RealSpeed > 5)	//az ked mame rychlost tak vykoname OnStart a zapiname status KS_FORWARD
						{
							status = KS_BACKWARD

							if(StartSound && !StartSnd)
								StartSnd = PlaySound(Owner, StartSound, SF_ONCE)
								
							CallMethods(Owner, OnBack)
//							Print("OnBackward")
						}
					}
				}

				if(MotorSound && status != KS_STOP)
				{
					if(!MotorSnd)
						MotorSnd = PlaySound(Owner, MotorSound, 0)
	
					float maxsp = fabs(speed)
					float freqrate = fabs(CurrentVelocity) / maxsp
					clamp freqrate<0, 1>
					float freq = 6000 + (freqrate * 4000)
					float vol = 0.2 + (freqrate * 0.8)
					SetSoundFrequency(MotorSnd, freq)
					SetSoundVolume(MotorSnd, LinearVol(vol))
				}
			}
		}	
		
		if(status != KS_STOP)
		{
			float RealSpeed2 = VectorLength(GetVelocity(Owner))
			
			if(RealSpeed2 == 0)
			{
				Stop(true)
//				Print("Zachranny stop")
			}
		}
	}

//-----------------------------------------------------------------
	int Update(vector pos, float dt)
	{
		ProcessEvents()

		if(!IsOK())
		{
			return KS_IDLE
		}
		
		if(CurrentVelocity < TargetVelocity)
		{
			CurrentVelocity = dt * 90 + CurrentVelocity
			clamp CurrentVelocity<CurrentVelocity, TargetVelocity>
		}
		else
		{
			if(CurrentVelocity > TargetVelocity)
			{
				CurrentVelocity = dt * -90 + CurrentVelocity
				clamp CurrentVelocity<TargetVelocity, CurrentVelocity>
			}
		}

		clamp CurrentVelocity<-speed, speed>

		if(LastVelocityTime == -1)
			LastVelocityTime = currenttime
			
		if(CurrentVelocity != 0)
			LastVelocityTime = currenttime

		float d = currenttime - LastVelocityTime

		if(d > 1.5)
		{
			return KS_IDLE
		}

		if(!_Update(CurrentVelocity, pos, dt))
		{
//			TargetVelocity = 0
//			CurrentVelocity = 0

			if(Updated)
			{
				Updated = false
				return KS_STOP
			}
			Updated = false
			return KS_NONE
		}
		Updated = true
		return KS_UPDATE
	}
	
//-----------------------------------------------------------------
	void Keyframer(ExtClass other, vector coords, float angle, hsound snd1, hsound snd2, hsound snd3)
	{	
		Owner = other
		status = KS_STOP
		info_path best = NULL
		float bestdist = 99999 * 99999
		for(info_path ec = GetNextEntity(NULL, CInfoPath); ec != NULL; ec = GetNextEntity(ec, CInfoPath))
		{
			vector off = coords - ec.coords
			float dist = off * off
			if(dist < bestdist)
			{
				bestdist = dist
				best = ec
			}
		}

		Current = best
			
		if(Current)
			Next = FindClass(Current.link)
	
		if(Current && Next)
		{
			NumCurved = CurvePath(Current, Next, Curve, PATH_SEGMENT)

			//offset od stredu trainu do pozice na kolejnici
			CarrierOffset = Current.coords - coords

			//vektor aktualniho segmentu
			PathDir = Curve[1] - Curve[0]
			PathLength = VectorNormalize(PathDir)
			CurrentSegment = 0

			vector angs = Vector2Angles(PathDir)
			CarrierAngle = angle - angs[1]
		}

		StartSound = snd1
		MotorSound = snd2
		StopSound = snd3

		LastVelocityTime = -1
	}	
}

//=================================================================================
class func_train extends RigidBodyClass
{
	vector coords
	float angle, angle2, angle3
	float speed
	string _materialremap
	string model
	string motorsound, startsound, stopsound
	string OnForward, OnBack, OnBeginStop, OnStop

	vobject MainObject
	vector CarrierOffset
	float CarrierAngle

	hsound MotorSound, StartSound, StopSound
	int MotorSnd, StartSn
	bool Updated

	Keyframer keyframer

	Constraint		ManipPointConstraint1, ManipPointConstraint2, ManipPointConstraint3
	Constraint		ManipAngConstraint1, ManipAngConstraint2, ManipAngConstraint3

#ifdef EDITOR
	void EditorOpened()
	{
		ActivateBody(false)
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		angle2 = atof(GetKeyValue("angle2"))
		angle3 = atof(GetKeyValue("angle3"))
		SetTransformEx(Vector(angle3, angle, angle2), coords)
		UpdateEntity(this)
	}

	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		SetTransformEx(Angs, Pos)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}
#endif

//---------------------------------------------------------------------------------
	void ActivateBody(bool stat)
	{
		if(!rb)
			return

		if(stat)
		{
			dBodyActive(this, true, true)
			SetEventMask(this, EV_CONTACT|EV_SIMULATE)
			
			if(impactsounds)
				SetEventMask(this, EV_SOUNDEND)	//koli zvukom
		}
		else
		{
			dBodyActive(this, false, false)
			SetVelocity(this, ZeroVec)
			dBodySetAngularVelocity(this, ZeroVec)
			
			ClearEventMask(this, EV_CONTACT|EV_SIMULATE)
			ClearEventMask(this, EV_SOUNDEND)	//koli zvukom
		}
	}

//-----------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		if(other._value == "true")
			keyframer.Stop(true)
		else
			keyframer.Stop(false)
	}

//-----------------------------------------------------------------
	void M_Forward(ExtClass other)
	{
		ActivateBody(true)	//zapina aj OnSimulate
		keyframer.Forward()
	}

//-----------------------------------------------------------------
	void M_Back(ExtClass other)
	{
		ActivateBody(true)	//zapina aj OnSimulate
		keyframer.Back()
	}

//-----------------------------------------------------------------
	void M_SetSpeed(ExtClass other)
	{
		keyframer.SetSpeed(atof(other._value))
	}
	
//-----------------------------------------------------------------
	void EOnTick(MinClass other, int extra)
	{
		if(KeyState(KC_NUMPAD4))
		{
			ClearKey(KC_NUMPAD4)
			M_Forward(this)
		}
		else
		{
			if(KeyState(KC_NUMPAD5))
			{
				ClearKey(KC_NUMPAD5)
				_value = "false"
				M_Stop(this)
			}
			else
			{
				if(KeyState(KC_NUMPAD6))
				{
					ClearKey(KC_NUMPAD6)
					M_Back(this)
				}
			}
		}
	}

	void UpdateConstraints()
	{
		vector mat[4]
		GetMatrix4(this, mat)
		vector TargetPoint = keyframer.CurrentMatrix[3]
		vector ErrOff = mat[3] - TargetPoint
		float ErrDot
		ErrOff = ErrOff * 0.5
		
		if(ManipPointConstraint1)
		{
			ManipPointConstraint1.setPoint(TargetPoint)
			ManipPointConstraint1.setConstraintError(ErrOff[0])
			dConstraintInsert(ManipPointConstraint1)
		}

		if(ManipPointConstraint2)
		{
			ManipPointConstraint2.setPoint(TargetPoint)
			ManipPointConstraint2.setConstraintError(ErrOff[1])
			dConstraintInsert(ManipPointConstraint2)
		}

		if(ManipPointConstraint3)
		{
			ManipPointConstraint3.setPoint(TargetPoint)
			ManipPointConstraint3.setConstraintError(ErrOff[2])
			dConstraintInsert(ManipPointConstraint3)
		}
//--
		vector rot = RotationVectorTo(keyframer.CurrentMatrix, mat)

		if(ManipAngConstraint1)
		{
			vector axis = ManipAngConstraint1.getAxis1()
			ErrDot = ManipAngConstraint1.getAxis1() * rot
			ManipAngConstraint1.setPoint(TargetPoint)
			ManipAngConstraint1.setConstraintError(ErrDot)
			dConstraintInsert(ManipAngConstraint1)
		}

		if(ManipAngConstraint2)
		{
			ErrDot = ManipAngConstraint2.getAxis1() * rot
			ManipAngConstraint2.setPoint(TargetPoint)
			ManipAngConstraint2.setConstraintError(ErrDot)
			dConstraintInsert(ManipAngConstraint2)
		}

		if(ManipAngConstraint3)
		{
			ErrDot = ManipAngConstraint3.getAxis1() * rot
			ManipAngConstraint3.setPoint(TargetPoint)
			ManipAngConstraint3.setConstraintError(ErrDot)
			dConstraintInsert(ManipAngConstraint3)
		}
	}

//-----------------------------------------------------------------
	void EOnSimulate(MinClass other, float dt)
	{
		vector origin = GetMatrix(this, 3)
		
		int rs = keyframer.Update(origin, dt)

		if(rs != KS_IDLE)
		{
			dBodyActive(this, true, true)

			if(dBodyIsActive(this))
				UpdateConstraints()
		}

		switch(rs)
		{
		case KS_IDLE:
			ActivateBody(false)	//vypina aj OnSimulate
			return

		case KS_STOP:
			keyframer.Stop(true)
			return

		case KS_NONE:
			return

//		case KS_UPDATE:
//				UpdateConstraints()
		}
	}

//-----------------------------------------------------------------
	void DeleteConstraints()
	{
		if(ManipPointConstraint1)
		{
			ManipPointConstraint1.destroy()
			ManipPointConstraint1 = NULL
		}
		
		if(ManipPointConstraint2)
		{
			ManipPointConstraint2.destroy()
			ManipPointConstraint2 = NULL
		}
		
		if(ManipPointConstraint3)
		{
			ManipPointConstraint3.destroy()
			ManipPointConstraint3 = NULL
		}

		if(ManipAngConstraint1)
		{
			ManipAngConstraint1.destroy()
			ManipAngConstraint1 = NULL	
		}

		if(ManipAngConstraint2)
		{
			ManipAngConstraint2.destroy()
			ManipAngConstraint2 = NULL	
		}
		
		if(ManipAngConstraint3)
		{
			ManipAngConstraint3.destroy()
			ManipAngConstraint3 = NULL	
		}		
	}

//-----------------------------------------------------------------
	void CreateConstraints()
	{
		if(!ManipPointConstraint1)
		{
			ManipPointConstraint1 = dConstraintCreatePoint()
			ManipPointConstraint2 = dConstraintCreatePoint()
			ManipPointConstraint3 = dConstraintCreatePoint()
			ManipAngConstraint1 = dConstraintCreateAngular()
			ManipAngConstraint2 = dConstraintCreateAngular()
			ManipAngConstraint3 = dConstraintCreateAngular()
	
			ManipPointConstraint1.setBody1(World)
			ManipPointConstraint1.setBody2(this)
			ManipPointConstraint2.setBody1(World)
			ManipPointConstraint2.setBody2(this)
			ManipPointConstraint3.setBody1(World)
			ManipPointConstraint3.setBody2(this)
	
			ManipPointConstraint1.setAxis("1 0 0")
			ManipPointConstraint2.setAxis("0 1 0")
			ManipPointConstraint3.setAxis("0 0 1")
	//--
			ManipAngConstraint1.setBody1(World)
			ManipAngConstraint1.setBody2(this)
			ManipAngConstraint2.setBody1(World)
			ManipAngConstraint2.setBody2(this)
			ManipAngConstraint3.setBody1(World)
			ManipAngConstraint3.setBody2(this)
	
			ManipAngConstraint1.setAxis1("1 0 0")
			ManipAngConstraint2.setAxis1("0 1 0")
			ManipAngConstraint3.setAxis1("0 0 1")
			
			ManipAngConstraint1.setAxis2("1 0 0")
			ManipAngConstraint2.setAxis2("0 1 0")
			ManipAngConstraint3.setAxis2("0 0 1")
		}	
	}
	
//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{	
		CreateConstraints()
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{	
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{	
		if(speed == 0)
			speed = 100

		if(!model)
			model = "obj/vehicles/transporter_01_body.xob"

		if(impactsounds == 3)	//user do kluca nic nenastavil tak my to chceme defaultne vypnute
			impactsounds = 0

		MainObject = GetObject(model)
		SetObject(MainObject, _materialremap)

		SetTransformEx(Vector(angle3, angle, angle2), coords)
		SetOrigin(this, coords)

		if(startsound)
			StartSound = GetSoundEx(startsound, 1, SF_3D)

		if(motorsound)
			MotorSound = GetSoundEx(motorsound, 1, SF_3D|SF_FREQCONTROL)

		if(stopsound)
			StopSound = GetSoundEx(stopsound, 1, SF_3D)

		keyframer = new Keyframer(this, coords, angle, StartSound, MotorSound, StopSound)
		
		keyframer.OnForward = OnForward
		keyframer.OnBack = OnBack
		keyframer.OnBeginStop = OnBeginStop
		keyframer.OnStop = OnStop
		keyframer.SetSpeed(speed)

		if(CreateRigidBody(GEOMTYPE_NONE, "", model))
		{
			dBodyInteractionLayer(this, ET_DOOR)

			if(keyframer.IsOK())
			{
				ActivateBody(true)	//zapina aj OnSimulate
				SetEventMask(this, EV_TICK)		
			}
			else
				ActivateBody(false)
			
			CreateConstraints()
			dBodySetGravity(this, ZeroVec)
		}
		
		SetEventMask(this, EV_LOADED|EV_ONLOAD)
		ClearFlags(this, TFL_TOUCHTRIGGERS) //Isn't good idea
	}
	
//-----------------------------------------------------------------
	void func_train()
	{
		ClassType = ClassTypeFuncTrain
		SetEventMask(this, EV_INIT)
		SetFlags(this, TFL_VISIBLE|TFL_SOLID|TFL_NOSHADOW)
		SetPickable(false)
		
		if(ActivateMethod == ACT_ENTITY_DEFAULT)
			ActivateMethod = ACT_NONE
	}	

//-----------------------------------------------------------------
	void ~func_train()
	{
		DeleteConstraints()

		delete keyframer
		//destroy action
		ClearEventMask(this, EV_SIMULATE)
		RemoveBodyAndGeom()

		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}

//=================================================================================
class func_keyframer extends ExtClass
{
	vector coords
	float angle, angle2, angle3
	float speed
	string motorsound, startsound, stopsound
	string OnForward, OnBack, OnBeginStop, OnStop

	hsound MotorSound, StartSound, StopSound
	int MotorSnd

	int flags
	Keyframer keyframer

#ifdef EDITOR
	void EditorOpened()
	{
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		angle2 = atof(GetKeyValue("angle2"))
		angle3 = atof(GetKeyValue("angle3"))
		SetMatrixEx(this, Vector(angle3, angle, angle2), coords)
		UpdateEntity(this)
	}

	void OnPositionChangingInEditor(vector pos, vector angs)
	{
		SetMatrixEx(this, angs, pos)
		UpdateMainVisualizer()

		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()
	}
#endif

//-----------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		if(other._value == "true")
			keyframer.Stop(true)
		else
			keyframer.Stop(false)
	}

//-----------------------------------------------------------------
	void M_Forward(ExtClass other)
	{
		SetEventMask(this, EV_FRAME)
		keyframer.Forward()
	}

//-----------------------------------------------------------------
	void M_Back(ExtClass other)
	{
		SetEventMask(this, EV_FRAME)
		keyframer.Back()
	}

//-----------------------------------------------------------------
	void M_SetSpeed(ExtClass other)
	{
		keyframer.SetSpeed(atof(other._value))
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		vector origin = GetMatrix(this, 3)

		int rs = keyframer.Update(origin, ftime)
		
		switch(rs)
		{
		case KS_IDLE:
			SetVelocity(this, ZeroVec)
			ClearEventMask(this, EV_FRAME)
			return

		case KS_STOP:
			keyframer.Stop(true)
			return

		case KS_NONE:
			return

		case KS_UPDATE:
			//TODO: behave according entities attached to us
			//dBodySetTargetFrame(this, keyframer.CurrentMatrix, dt, true)
			SetMatrix(this, keyframer.CurrentMatrix)
		}
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{	
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{	
		if(speed == 0)
			speed = 100

		SetFlags(this, TFL_FULLMATRIX)
		SetMatrixEx(this, Vector(angle3, angle, angle2), coords)

		if(startsound)
			StartSound = GetSoundEx(startsound, 1, SF_3D)

		if(motorsound)
			MotorSound = GetSoundEx(motorsound, 1, SF_3D|SF_FREQCONTROL)

		if(stopsound)
			StopSound = GetSoundEx(stopsound, 1, SF_3D)

		keyframer = new Keyframer(this, coords, angle, StartSound, MotorSound, StopSound)
		
		keyframer.OnForward = OnForward
		keyframer.OnBack = OnBack
		keyframer.OnBeginStop = OnBeginStop
		keyframer.OnStop = OnStop
		keyframer.SetSpeed(speed)

		if(flags & 1)
			keyframer.unlockz = true

		if(keyframer.IsOK())
		{
			SetEventMask(this, EV_FRAME)
		}
	}
	
//-----------------------------------------------------------------
	void func_keyframer()
	{
		SetEventMask(this, EV_INIT|EV_LOADED)
	}	
	
	void ~func_keyframer()
	{
		delete keyframer
	}
}

//=================================================================================
class info_cubemap extends ExtClass
{
	vector coords
	int size

#ifdef EDITOR
	int vflags
	vobject sphere

	void AfterReloadInEditor()		//Bolo uz volane init aj loaded. editor s tym skoncil
	{
		UpdateEntity(this)
		UpdateCubeMap(this, size, _name)
	}

	void DestroyCustomVisuals()
	{
		if(sphere)
		{
			ReleaseObject(sphere, 0)
			sphere = VNULL
		}
	}

	void CreateCustomVisuals()
	{
		DestroyCustomVisuals()
		sphere = GetObject("obj/debug/DebugSphere.xob")
		SelectObject(this, sphere)
		SetFlags(this, TFL_VISIBLE)
	}	
#endif

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{	
		SetOrigin(this, coords)
		clamp size<0, 6>
	}

//-----------------------------------------------------------------
	void ~info_envcube()
	{
	}

	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
		SetEventMask(this, EV_INIT|EV_LOADED)
	}
	
//-----------------------------------------------------------------
	void info_envcube()
	{
		SetEventMask(this, EV_INIT|EV_LOADED)
	}	
}

//=================================================================================
class info_null extends ExtClass
{
	float angle, angle2, angle3
	vector coords
	int	extra
	string linkname

/*
#ifdef EDITOR
	vobject obj

//-----------------------------------------------------------------
	void DebugVisualize(bool stat)
	{
		if(stat)
		{
			if(!obj)
	      	obj = GetObject("obj/pointing_arow.xob")
	      	
	      SelectObject(this, obj)
	      SetFlags(this, TFL_VISIBLE | TFL_OBBCOLLISION | TFL_SOLID)
   	}
   	else
   		ClearFlags(this, TFL_VISIBLE | TFL_OBBCOLLISION | TFL_SOLID)
	}

//-----------------------------------------------------------------
	void OnPositionChangingInEditor(vector Pos, vector Angs)
	{
		SetOrigin(this, coords)
		SetAngles(this, Angs)
	}
//-----------------------------------------------------------------
	void ~info_null()
	{
		if(obj)
			ReleaseObject(obj, 0)
	}
#endif
*/
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
      ClassType = ClassTypeInfo
      ClassNum = ClassNumNull
      SetOrigin(this, coords)

      SetAngles(this, Vector(angle3, angle, angle2))
	}	

	void info_null()
	{
		SetEventMask(this, EV_INIT|EV_LOADED)
	}
}

//=================================================================================
class info_notnull extends ExtClass
{
	vector coords
}

//=================================================================================
// nulova entita, jen pro odecitani orientace pivotu
class info_pivot extends _eclass
{
}

//=================================================================================
ExtClass FindClassByLink(string link)
{
	if(link == "")
		return NULL

	info_null ec = FindClass(link)
	if(ec == NULL)
	{
		ec = GetNextEntity(NULL, CInfoNull)
	
		while(ec)
		{
			if(ec.linkname == link)
				break
	
			ec = GetNextEntity(ec, CInfoNull)
		}
	}
	return ec
}

//=================================================================================
class info_script extends ExtClass
{
string OnScript
string Parms[16]
int Lengths[16]
int NumParms
vector coords
string script

//-----------------------------------------------------------------
	void M_Exec(ExtClass other)
	{
		if(other._value)
		{
			CallMethods(other, PrepareScript(script, other._value, Parms, Lengths, NumParms))
			return
		}

		CallMethods(other, script)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(ExtClass other, int extra)
	{
		SetOrigin(this, coords)	

		triminplace(OnScript)
		
		NumParms = 0
	
		int s = strstr(OnScript, "(")
		
		if(s == 0)
		{
			s = strstr(OnScript, ")")
			if(s < 0)
				return
			
			string str = substr(OnScript, 1, s - 1)
			script = substr(OnScript, s + 1, strlen(OnScript) - s - 1)

			while(true)
			{
				s = strstr(str, ",")
				
				if(s >= 0)
				{
					string p = substr(str, 0, s)
					Lengths[NumParms] = triminplace(p)
					Parms[NumParms++] = p
					str = substr(str, s + 1, strlen(str) - s - 1)
				}
				else
				{
					Lengths[NumParms] = triminplace(str)
					Parms[NumParms++] = str
					break
				}
			}
			return
		}
		script = OnScript
	}
}

//=================================================================================
class info_switch extends ExtClass
{
	string OnExec1, OnExec2, OnExec3, OnExec4
	string OnExec5, OnExec6, OnExec7, OnExec8
	
	string OnChange, BeforeExec, AfterExec
	
	string execs[8]
	
	int selected
	int execsnum
	
	vector coords
	
//-----------------------------------------------------------------
	void M_Random(ExtClass other)
	{
		if(other._value)
		{
			int s = strstr(other._value, ",")
			
			if(s >= 0)
			{
				int lo, hi
				
				lo = atoi(substr(other._value, 0, s))
				hi = atoi(substr(other._value, s + 1, strlen(other._value) - s - 1))
				
				if(lo >= 1 && lo <= 8 && hi >= 1 && hi <= 8 && lo <= hi)
				{
					selected = rand(lo, hi + 1)
					CallMethods(this, OnChange)
				}
			}
		}
	}
	
//-----------------------------------------------------------------
	void M_Switch(ExtClass other)
	{
		int n = atoi(other._value)
		if(n >= 1 && n <= 8)
		{
			selected = n
			CallMethods(this, OnChange)
		}
	}

//-----------------------------------------------------------------
	void M_SwitchInc(ExtClass other)
	{
		int n = atoi(other._value)
		n = selected + n
		
		if(n > execsnum)
			n = 1
			
		if(n < 1)
			n = execsnum
		
		if(n >= 1 && n <= 8)
		{
			selected = n
			CallMethods(this, OnChange)
		}
	}
	
//-----------------------------------------------------------------
	void M_Exec(ExtClass other)
	{
		int sel = selected
		
		if(other._value)
		{
			int n = atoi(other._value)
			if(n >= 1 && n <= 8)
				sel = n
		}
		
		CallMethods(this, BeforeExec)		
		CallMethods(this, execs[sel - 1])
		CallMethods(this, AfterExec)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(ExtClass other, int extra)
	{
		SetOrigin(this, coords)	

		if(selected == 0)
			selected = 1

		if(execsnum == 0)
			execsnum = 8
		
		execs[0] = OnExec1
		execs[1] = OnExec2
		execs[2] = OnExec3
		execs[3] = OnExec4
		execs[4] = OnExec5
		execs[5] = OnExec6
		execs[6] = OnExec7
		execs[7] = OnExec8
	}
}

//=================================================================================
class info_counter extends ExtClass
{
	string OnCount, OnIncrement, OnDecrement, OnFail, OnCountFail
	int flags
	int count, cnt, cnt2
	int parm
	int LastParm
	vector coords
	MinClass Entities[20]

	void ClearList()
	{
		cnt2 = 0
		LastParm = 0
		
		for(int n = 0; n < sizeof(Entities); n++)
			Entities[n] = NULL
	}

	void CountTest()
	{
		if(cnt2 == count)	//ak uz bol pokus z kazdej entity ale nebolo to spravne poradie
		{
			ClearList()
			CallMethods(this, OnCountFail)
		}
	}

	void M_Increment(ExtClass other)
	{
		parm = atoi(other._value)

		bool IsInList = false
		for(int n = 0; n < cnt2; n++)
		{
			if(Entities[n] == other)
				IsInList = true
		}
		
		if(!IsInList)
			Entities[cnt2++] = other
			
		if(flags & 2)	//pripad ked napriklad potrebujeme postlacat viac tlacitok v spravnom poradi
		{
			if(parm > 0)
			{
				if(parm < LastParm)
				{
					CallMethods(this, OnFail)
					CountTest()
					return
				}
				else
				{
					int tmp = LastParm + 1
											
					if(parm != tmp)
					{
						CallMethods(this, OnFail)
						CountTest()
						return
					}
					else
						LastParm	= parm
				}
			}
		}
		
		CallMethods(this, OnIncrement)
		
		if(++cnt == count)
		{
			CallMethods(this, OnCount)
			return
		}

		int ii = count + 1
		if(cnt == ii && flags & 1)
		{
			cnt = 0
			LastParm = 0
		}
		
		CountTest()
	}
	
	void M_Decrement(ExtClass other)
	{
		cnt--
		CallMethods(this, OnDecrement)
				
		if(cnt < 0 && flags & 1)
		{
			cnt = count
			CallMethods(this, OnCount)
		}
	}

	void M_SetCounter(ExtClass other)
	{
		cnt = atoi(other._value)
		ClearList()
	}

	void M_Refresh(ExtClass other)
	{
		if(cnt == count)
			CallMethods(this, OnCount)
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

	void EOnInit(ExtClass other, int extra)
	{
		SetOrigin(this, coords)	
	}
}

//=================================================================================
class info_portal extends ExtClass
{
	string OnEnable, OnDisable
	
	void M_Enable(ExtClass other)
	{
		World.pPathFinder.SwitchPortal(_name, true)
		CallMethods(this, OnEnable)
	}
	
	void M_Disable(ExtClass other)
	{
		World.pPathFinder.SwitchPortal(_name, false)
		CallMethods(this, OnDisable)
	}	

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

	void info_portal()
	{
	}	
}

//=================================================================================
class info_connection extends ExtClass
{
vector	coords
float 	angle
string OnArrival
int type

//-----------------------------------------------------------------
#ifdef EDITOR	
	Shape line

	void DestroyCustomVisuals()
	{
		if(line)
		{
			line.Destroy()
			line = NULL
		}
	}

	void CreateCustomVisuals()
	{
		DestroyCustomVisuals()
		
		ExtClass next = FindClass(link)
		
		if(next)
		{
			vector from = GetMatrix(this, 3) + UpVec
			
			line = AddDBridgeArrow(from, GetMatrix(next, 3) + UpVec, 4.0, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP)
		}
	}

	void UpdateCustomVisuals()
	{
		DestroyCustomVisuals()
		CreateCustomVisuals()

		if(wprec == 0)
		{
			for(local info_connection ec = GetNextEntity(NULL, CInfoConnection); ec != NULL; ec = GetNextEntity(ec, CInfoConnection))
			{
				if(ec != this)
				{
					if(ec.link == _name)
					{
						wprec++
						ec.UpdateCustomVisuals()
						wprec--
					}
				}
			}
		}
	}
	
#endif

/*
	void M_Enable(ExtClass other)
	{
		World.pPathFinder.SwitchPortal(_name, true)
		CallMethods(this, OnEnable)
	}
	
	void M_Disable(ExtClass other)
	{
		World.pPathFinder.SwitchPortal(_name, false)
		CallMethods(this, OnDisable)
	}	
*/
//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetEventMask(this, EV_LOADED)
      SetOrigin(this, coords)
      SetAngles(this, Angles(angle))
      UpdateEntity(this)
	}

//-----------------------------------------------------------------
	void info_connection()
	{
	}	
}

//=================================================================================
class info_waypoint extends ExtClass
{
vector coords, floor
float angle
int flags
int used
string firstsound

int cost
float coverfov
float radius
float enemyradius
float enemyfov
float reusetime
string aitype

string OnArrival, OnDeparture

float lastusetime
int tcost //per search cost

vector facevec, rightvec
int node

#ifdef EDITOR	
	Shape line

	void UpdateCustomVisuals()
	{
		DestroyCustomVisuals()
		CreateCustomVisuals()

		int rec
		
		if(wprec == 0)
		{
			for(local info_waypoint ec = GetNextEntity(NULL, CInfoWaypoint); ec != NULL; ec = GetNextEntity(ec, CInfoWaypoint))
			{
				if(ec != this)
				{
					if(ec.link == _name)
					{
						wprec++
						ec.UpdateCustomVisuals()
						wprec--
					}
				}
			}
		}
	}

	void DestroyCustomVisuals()
	{
		if(line)
		{
			line.Destroy()
			line = NULL
		}
	}

	void CreateCustomVisuals()
	{
		DestroyCustomVisuals()

		ExtClass next = FindClass(link)
		
		if(next)
		{
			vector from = GetMatrix(this, 3) + UpVec
			
			line = AddDArrow(from, GetMatrix(next, 3) + UpVec, 4.0, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP)
		}
	}
	
#endif
//-----------------------------------------------------------------
	void M_Enable(ExtClass other)
	{
		used &= 0xbfffffff
	}
	
//-----------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		used |= 0x40000000
	}

//-----------------------------------------------------------------
	CPosition GetWPos()
	{
		if(WPosition)
			return WPosition
		
		WPosition = new CPosition

		if(!World.pPathFinder.PositionForPoint(floor + "0 0 32", WPosition))
		{
			delete WPosition
			WPosition = NULL
		}

		return WPosition
	}

//-----------------------------------------------------------------
	bool TraceFilter(ExtClass other)
	{
		if(IsInherited(other, CExtClass) == true)
			return true
			
		return false
	}

//-----------------------------------------------------------------
	void Init()
	{
		if(!_name || substr(_name, 0, 1) == "@")
			return

#ifdef DEVELOPER
		ExtClass other = FindClass(link)
		if(other)
		{
			if(other.ClassType != CTypeInfoWaypoint)
			{
  				DPrint("info_waypoint " + _name + " linked to: " + other._name)
				ErrorNum++
			}
		}
#endif

		if(flags & 0x10000)
		{
			floor = coords
		}
		else
		{
			floor = coords - "0 0 128"
			float plane[4]
			
	//		TraceLineEx(PLAYER, coords + "0 0 4", floor, ZeroVec, ZeroVec, NULL, plane, NULL, NULL, TRACE_DEFAULT, NULL)
			if(TraceLineEx(PLAYER, coords + "0 0 4", floor, ZeroVec, ZeroVec, NULL, plane, NULL, NULL, TRACE_DEFAULT, NULL) >= 1)
			{
				DPrint("info_waypoint " + _name + " too high above floor")
				ErrorNum++
			}
		}

//Print(String(_name + Vector2String(floor)))

      SetOrigin(this, coords)
      SetAngles(this, Angles(angle))
      UpdateEntity(this)

		if(WPosition)
		{
			delete WPosition
      	WPosition = NULL
		}

      //is it disabledonstart?
      if(flags & 0x40000000)
      	used |= 0x40000000

		facevec = Yaw2Vector(angle)
		rightvec = facevec * UpVec
/*
		vector coord = coords + "0 0 16"
			AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_TRANSP, coord, facevec * 32 + coord)
			AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_TRANSP, coord, leftvec * 32 + coord)
	*/	
/*
		vector coord = coords + "0 0 16"
		if(flags & 1)
		{
			AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_TRANSP, coord - "3 3 3", coord + "3 3 3")
			AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_TRANSP, coord, facevec * 32 + coord)
		}
		else
		{
			if(flags & 2)
			{
				AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZUPDATE|SS_TRANSP, coord - "3 3 3", coord + "3 3 3")
				AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_TRANSP, coord, facevec * 32 + coord)
			}
		}*/
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		Init()
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetEventMask(this, EV_LOADED)		 

		if(coverfov == 0)
			coverfov = cos(45 * DEG2RAD)
		else
			coverfov = cos(coverfov * 0.5 * DEG2RAD)
		
		if(reusetime == 0)
			reusetime = 500
		
		lastusetime = currenttime * 100 - 1000

		if(enemyradius == 0)
			enemyradius = 64

		if(enemyfov)
			enemyfov = cos(enemyfov * 0.5 * DEG2RAD)
	}
	
//-----------------------------------------------------------------
	void info_waypoint()
	{
		ClassType = CTypeInfoWaypoint
	}

//-----------------------------------------------------------------
	void ~info_waypoint()
	{
		if(node)
		{
			World.pPathFinder.UnlinkWaypoint(this, node)
		}
	}	
}

//=================================================================================
class info_squad extends ExtClass
{
vector coords
float angle
string members
int flags
string OnActivate, OnKill, OnKillOff, OnLastMan
int Alive

bool bActivated, bAttacked
CreatureClass Members[64]
int NumMembers

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra);
	void OnMemberActivate(CreatureClass cc);	
	void OnMemberAttacked(CreatureClass cc, CreatureClass attacker, vector pos);
	void OnMemberKilled(CreatureClass cc);
	//seeks for visible actor somewhere in facevec direction from actor 'fromactor'
	CreatureClass FindCommander(CreatureClass fromactor, ExtClass target);
	
	void M_Activate(ExtClass other);	

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetOrigin(this, coords)
		SetAngles(this, Angles(angle))
		SetEventMask(this, EV_LOADED)
	}
}

//=================================================================================
class info_wpgroup extends ExtClass
{
vector coords
float angle
string waypoints

info_waypoint	PWaypoints[64]
int NumWaypoints

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		string str = waypoints
		int lastgroup = -1
		info_waypoint lastwaypoint = NULL
		
		NumWaypoints = 0
		while(true)
		{
			string wpname

			if(ParseStringEx(str, wpname) != 3)
				break
			
			info_waypoint wp = FindClass(wpname)
			
			if(wp == NULL)
			{
				DPrint("info_wpgroup '" + _name + "' contains nonexistent waypoint '" + wpname + "'")
				ErrorNum++
			}
			else
			{
				if(!IsInherited(wp, CInfoWaypoint))
				{
					DPrint("info_wpgroup '" + _name + "' contains '" + wpname + "' of type '" + ClassName(wp) + "'")
					ErrorNum++
				}
				else
				{
					wp.GetWPos()

					if(wp.WPosition)
					{
						if(lastgroup != -1 && wp.WPosition.group != lastgroup)
						{
							DPrint("info_wpgroup '" + _name + "' contains waypoint '" + wpname + "' from different nav-group than " + lastwaypoint._name)
							ErrorNum++
						}
						lastgroup = wp.WPosition.group
						lastwaypoint = wp
					}

					PWaypoints[NumWaypoints++] = wp
					if(NumWaypoints >= 64)
					{
						DPrint("info_wpgroup '" + _name + "' has too much waypoints")
						ErrorNum++
						break
					}					
				}
			}

			if(ParseStringEx(str, wpname) == 0)
				break

			if(wpname != ",")
			{
				DPrint("info_wpgroup '" + _name + "' syntax error '" + wpname + "' of type '" + ClassName(wp) + "'")
				ErrorNum++
				break
			}
		}

		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetOrigin(this, coords)
		SetAngles(this, Angles(angle))
		SetEventMask(this, EV_LOADED)
	}
	
//-----------------------------------------------------------------
	void info_wpgroup()
	{
		ClassType = CTypeInfoWpGroup
	}
}

//=================================================================================
class info_sound extends ExtClass
{
int flags
vector coords
string sound

float RndWaitMin			
float RndWaitMax
float RandomTime
hsound Sound
int Psound
bool repeat

float NextVolume
float FadeSpeed
float CurrentVolume

//-----------------------------------------------------------------

	void Play()
	{
   	int sflags = 0
   	
      if(flags & 1)
         sflags = sflags | SF_ONCE

      if(flags & 2)
         sflags = sflags | SF_AMBIENT

      if(flags & 8)
         sflags = sflags | SF_HALFRANGE
      else
		{
	      if(flags & 16)
	         sflags = sflags | SF_QUARTERRANGE
		}

		if(repeat)	//repeat play
		{
	      if(!flags & 1)
	         sflags = sflags | SF_ONCE

			RandomTime = frand(RndWaitMin, RndWaitMax)
			SetEventMask(this, EV_TIMER)
	     	SetTimer(this, this, RandomTime, EV_TIMER, 0, true)
		}

		Psound = PlaySound(this, Sound, sflags)
		
		//toto je umyslne. Viditelne entity ziskavaji vyhodu rychleho testovani slysitelnosti
		SetFlags(this, TFL_VISIBLE)
   }

	void EOnFrame(MinClass other, int extra)
	{
		CurrentVolume += ftime * FadeSpeed
		
		if(FadeSpeed > 0)
		{
			if(CurrentVolume >= NextVolume)
			{
				CurrentVolume = NextVolume
				ClearEventMask(this, EV_FRAME)
			}
			SetSoundVolume(Psound, LinearVol(CurrentVolume))
			return
		}
		
		if(FadeSpeed < 0)
		{
			if(CurrentVolume <= NextVolume)
			{
				CurrentVolume = NextVolume
				ClearEventMask(this, EV_FRAME)
			}
			SetSoundVolume(Psound, LinearVol(CurrentVolume))
		}
	}

	void M_SetVolume(ExtClass other)
	{
		CurrentVolume = atof(other._value)
		
		if(CurrentVolume > 100)
			CurrentVolume = 100
			
		CurrentVolume *= 0.01
		SetSoundVolume(Psound, LinearVol(CurrentVolume))
	}

	void M_VolumeFade(ExtClass other)
	{
		string parms[8]
		int nump = ParseString(other._value, parms)

		if(nump != 3)
			return

		NextVolume = atof(parms[0])

		if(NextVolume > 100)
			NextVolume = 100

		NextVolume *= 0.01
		FadeSpeed = atof(parms[2])
		FadeSpeed *= 0.01
		
		if(FadeSpeed == 0)
			FadeSpeed = 0.01
			
		FadeSpeed = 1 / FadeSpeed

		if(NextVolume < CurrentVolume)
			FadeSpeed = -FadeSpeed
		
		SetEventMask(this, EV_FRAME)
	}

	void M_Play(ExtClass other)
	{
		Play()
	}
	
	void M_Stop(ExtClass other)
	{
		if(Psound && !repeat)	//nahodne zvuky nechame dohrat
		{
			EndSound(Psound)
			Psound = 0	
		}
		
		ClearEventMask(this, EV_TIMER)
	}

	vector RandomPos()
	{
		vector RndVec
		RndVec[0] = frand(-1, 1)
		RndVec[1] = frand(-1, 1)
		RndVec[2] = frand(-1, 1)
		VectorNormalize(RndVec)
		RndVec = RndVec * frand(20, 200)
		return RndVec
	}
	
//-----------------------------------------------------------------
	void EOnTimer(MinClass other, int extra)
	{
     	if(flags & 4)
		{
	     	if(!flags & 2)
	     	{
				vector Spos = GetOrigin(World.Player) + RandomPos()
				SetOrigin(this, Spos)
			}
		}

     	Play()
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
      SetOrigin(this, coords)
		UpdateEntity(this)
		
		if(RndWaitMin > 0 || RndWaitMax > 0)
		{
			repeat = true
			
	      if(RndWaitMin == 0)
	      	RndWaitMin = 200
	
	      if(RndWaitMax == 0)
	      	RndWaitMax = 300
		}
        
		int gf
		
		if(flags & 2)
			gf = 0
		else
			gf = SF_3D
		
		if(sound)
			Sound = GetSoundEx(sound, 1, gf)

		CurrentVolume = 1
	}
}

//==============================================================
class info_hack extends ExtClass
{
	vector coords
	string OnLogin, OnLogout
	string name, description
	string HackCameraFrom
	float hackingtime
	float logintime
	int	flags
	info_null HackCameraOwner

//--------------------------------------------------------------
	void M_Enable(ExtClass other)
	{
		status = 0
	}

//--------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		status = 1
	}

//--------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//--------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		SetOrigin(this, coords)
		HackCameraOwner = FindClass(HackCameraFrom)

		if(!HackCameraOwner)	//ak nechame 0 = nesizujeme kameru = okamzity login a logout
			logintime = 0

		if(hackingtime == 0)
			hackingtime = 200

		HackInterf.HackingTime = hackingtime * 0.01	//v interface to mame v sekundach ale v entite dodrzime standard v milisekundach

		if(HackCameraOwner && !IsInherited(HackCameraOwner, CInfoNull))
		{
			HackCameraOwner = NULL
			DPrint("Bad entity type in key HackCameraFrom in " + _name + ". Only info_null is allowed!")
		}
	}

//--------------------------------------------------------------
	void info_hack();
}

//==============================================================
class HackInterfaceCustom extends HackInterface
{
	info_hack Owner
	int NumLogins

//--------------------------------------------------------------
	bool CanHacking()
	{
		if(NumLogins > 0 && Owner.flags & 1)	//once flag
			return false
		
		if(Owner.status == 1)
			return false

		return true
	}

//---------------------------------------------------------------------------------	
	int CanLogin()
	{
		if(CurHackingTime != HackingTime)
			return false

		return 1 - Owner.status
	}

//---------------------------------------------------------------------------------	
	bool NeedLogOut()	//ked entita uz nechce aby bol player nalogovany
	{
		return false
	}

//--------------------------------------------------------------
	void LogIn()
	{
		NumLogins++
		CallMethods(Owner, Owner.OnLogin)
	}

//--------------------------------------------------------------
	void LogOut()
	{
		CallMethods(Owner, Owner.OnLogout)
	}

//---------------------------------------------------------------------------------	
	float GetMaxLoginTime()
	{
		return Owner.logintime
	}

//---------------------------------------------------------------------------------	
	bool SetCameraFromNull(int camera)
	{
		if(!Owner.HackCameraOwner)
			return false

		info_null camown = Owner.HackCameraOwner
		vector mat[4]
		RollPitchYawMatrix2(Vector(-camown.angle3, camown.angle, camown.angle2) + EarthquakeAng, mat)
		mat[3] = GetMatrix(camown, 3) + EarthquakeOff
		SetCameraEx(camera, mat)
		return true
	}

//---------------------------------------------------------------------------------	
	void OnFrame()
	{
		SetCameraFromNull(0)
	}

//---------------------------------------------------------------------------------	
	bool SetHackPreviewCamera()
	{
		return SetCameraFromNull(1)
	}

//---------------------------------------------------------------------------------	
	void BeforeInit()
	{
		name = Owner.name
		description = Owner.description
//		IconName = "C" + NameIndex
		IconType = 2
	}

//--------------------------------------------------------------
	void HackInterfaceCustom(MinClass owner)
	{
		Owner = owner
	}
}

void info_hack::info_hack()
{
	SetEventMask(this, EV_INIT|EV_LOADED)
	HackInterf = new HackInterfaceCustom(this)
}




//=================================================================================
class info_impacter extends ExtClass
{
	float angle, angle2, angle3
	vector coords
	string ImpactTarget
	string OnLoad
	ExtClass Target

//-----------------------------------------------------------------
	void M_Impact(ExtClass other)
	{
		if(!Target)
			return

		float impactpower = atof(other._value)
		vector force = GetMatrix(this, 0) * impactpower
		dBodyApplyImpulseAt(Target, force, GetMatrix(this, 3))
	}

//-------------------------------------------------------------------------
	void EOnLoaded(ExtClass other, int extra)
	{
		Target = FindClass(ImpactTarget)
		
		if(Target && !Target.IsRigidBody())
		{
			DPrint(String("info_impacter: ") + Target._name + String(" without rigid body."))
			Target = NULL
		}
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
      ClassType = ClassTypeInfo
      ClassNum = ClassNumNull
      SetOrigin(this, coords)
      SetAngles(this, Vector(angle3, angle, angle2))
	}	

//-----------------------------------------------------------------
	void info_impacter()
	{
		SetEventMask(this, EV_INIT|EV_LOADED)
	}
}