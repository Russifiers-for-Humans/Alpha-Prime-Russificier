
bool IsHidden(ExtClass other)
{
	if(!other)
		return false

	if(other.Hidden || other.CategoryHidden)
		return true

	return false
}

bool TrcFilter5(ExtClass other)
{
	if(other._name == "")
		return false

	if(!IsInherited(other, CExtClass))
		return false

	if(IsHidden(other))
		return false

	if(other.CanEdit() == false)		//nieco sa editovat neda
		return false

	return true
}

ExtClass PointEntity(vector TargetPoint)
{
	ExtClass NoModelEnts[4096]
	int NoModelEntsNum
	vector Mins, Maxs
	NoModelEntsNum = 0
	ExtClass ext

	for(ext = GetNextEntity(NULL, CExtClass); ext != NULL; ext = GetNextEntity(ext, CExtClass))
	{
		if(ext._name)
		{
			ext.EngineFlags = GetFlags(ext)
			SetFlags(ext, TFL_OBBCOLLISION | TFL_SOLID)
			GetBoundBox(ext, Mins, Maxs)
			
			bool ContainModel = true
			
			if(Mins == ZeroVec && Maxs == ZeroVec)	//entity bez objemu budeme zistovat dodatocne
				ContainModel = false
			else
			{
				if(ClassName(ext) == "misc_particle_effector")	//snad jedina vynimka
					ContainModel = false				
			}
			
			if(ContainModel == false)
				NoModelEnts[NoModelEntsNum++] = ext
		}
	}

  	ExtClass best = NULL
	float bestdist = 9999999
	int cont, sparm
  	float norm[4], cfrac
  	bool Hidden

	vector from = World.CameraPos
	vector to = TargetPoint//g_CamNorm * (float)8192 + from
   	
	//vector mins, maxs
	cfrac = TraceLineEx(DEFAULT, from, to, ZeroVec, ZeroVec, best, norm, NULL, NULL, TRACE_WORLD|TRACE_ENTS|TRACE_RAGDOLLS|TRACE_BONES, &TrcFilter5)
//	AddDShape(SHAPE_BBOX, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, to - "0.1 0.1 0.1", to + "0.1 0.1 0.1")

	if(cfrac < 1 && best != World)
		bestdist = VectorLength(from - to)
	else
		best = NULL

//vratime flagy
	for(ext = GetNextEntity(NULL, CExtClass); ext != NULL; ext = GetNextEntity(ext, CExtClass))
	{
		if(ext._name)
			SetFlags(ext, ext.EngineFlags)
	}

	ExtClass ec = NULL
	Hidden = true
	int CurArrPos = -1

	while(Hidden && CurArrPos < NoModelEntsNum)
	{
		ec = NoModelEnts[++CurArrPos]//GetNextEntity(ec, CExtClass)
				
		if(!ec)
			break
				
		Hidden = IsHidden(ec)
	}
	
	float x = g_iWidth * 0.5
	float y = g_iHeight * 0.5
	
	float length = bestdist

	while(CurArrPos < NoModelEntsNum)
	{
		ec = NoModelEnts[CurArrPos++]
		vector min, max
		GetBoundBox(ec, min, max)
		vector org = GetMatrix(ec, 3)

		if(min == ZeroVec)
			min = "-8 -8 -8"

		if(max == ZeroVec)
			max = "8 8 8"
		
		min += org
		max += org

		if(ec != World && ec.DefPtr && ec._name != "" && !ec.NotEditable && IsBoxVisible(min, max, 1))
		{
			float dist
	
			if(ec.DefPtr.VisualsType == "box" || ec.DefPtr.VisualsType == "diamond" || ec.DefPtr.VisualsType == "pyramid")
			{
				if(ec.DefPtr.VisualsSize == "?")	//z modelu
					GetBoundBox(ec, min, max)
				else				//z definicie v xml
				{
					string tokens[16]
					int NumTok = ParseString(ec.DefPtr.VisualsSize, tokens)
					float sx = atof(tokens[0])
					float sy = atof(tokens[1])
					float sz = atof(tokens[2])
					min = Vector(sx, sy, sz)
					sx = atof(tokens[3])
					sy = atof(tokens[4])
					sz = atof(tokens[5])
					max = Vector(sx, sy, sz)
				}

				if(min == ZeroVec)
					min = "-8 -8 -8"
		
				if(max == ZeroVec)
					max = "8 8 8"
				
				min += org
				max += org
	
				dist = IntersectRayBox(from, to, min, max)
			}
	
			if(ec.DefPtr.VisualsType == "sphere")
			{
				float radius = atof(ec.DefPtr.VisualsSize)
				dist = IntersectRaySphere(from, to, org, radius)
			}

			if(dist > 0)
			{
				vector mid = to - from * dist + from
				
				dist *= length
				if(dist < bestdist)
				{
					bestdist = dist
					best = ec
				}
			}
		}

		Hidden = true

		while(Hidden && CurArrPos < NoModelEntsNum)
		{
			ec = NoModelEnts[CurArrPos]
		
			if(!ec)
				break
				
			Hidden = IsHidden(ec)
			
			if(Hidden)
				CurArrPos++
		}
	}

	if(best == World)
		return NULL

	return best
}


//-----------------------------------------------------------------
Shape WayBoxShapes[1024]
int NumWayBoxes
ExtClass BoxOwners[1024]
Shape WayLineShapes[1024]
int NumWayLines
ExtClass WayEnts[1024]
int WayWidgets[1024]
int NumWayEnts

//-----------------------------------------------------------------
vector GetEntityCenter(ExtClass ec)
{
	vector min,max
	
	GetBoundBox(ec, min, max)
	return min + max * (float)0.5 + GetOrigin(ec)
}

//-----------------------------------------------------------------
void UpdateEntityLinks(bool show)
{
	return
}

//-----------------------------------------------------------------
void UpdateWayEnts()
{
	return
}
