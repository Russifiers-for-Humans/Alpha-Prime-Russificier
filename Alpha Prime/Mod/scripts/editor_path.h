
//-----------------------------------------------------------------
float Floor(float f)
{
	int i = f
	return i
}
//-----------------------------------------------------------------
vector SnapVector(vector v, vector grid)
{
	vector res
	
	res[0] = Floor(v[0] / grid[0]) * grid[0]
	res[1] = Floor(v[1] / grid[1]) * grid[1]
	res[2] = Floor(v[2] / grid[2]) * grid[2]
	
	return res
}


/*
F3 		  - edit mod
SPACE		  - kamera/editace
INSERT	  - pridani bodu
DELETE	  - mazani bodu
BACKSPACE  - umazani posledniho bodu
E			  - Edit/Create mod
Enter/RMB  - ulozeni arey.
PgUp/PgDwn - Velikost gridu		
G			  - Show grid
X			  - defaultni polygon
Q			  - Point snap
F1			  - Help
*/

const string PathTitle = "Path editor"

//-----------------------------------------------------------------
string bool2a(bool in)
{
	if(in)
		return "yes"
	else
		return "no"
}
	
//-----------------------------------------------------------------
vector SplitLine(vector start, vector end, vector norm, float d)
{
	vector vec
	float d1, d2

	d1 = start * norm
	d1 = d1 - d

	d2 = end * norm 
	d2 = d2 - d

	if(d1 <= 0 && d2 <= 0)
		return ZeroVec

	if(d1 > 0 && d2 > 0)
		return ZeroVec

	d = d1 - d2

	d = d1 / d

	clamp d<0, 1>
   						
	vec = end - start
	vec = vec * d
	
	return vec + start
}

//-----------------------------------------------------------------
bool PointOnLine(vector p, vector p1, vector p2)
{
	vector v1, v2

	v1 = p - p1
	v2 = p2 - p
		
	VectorNormalize(v1)
	VectorNormalize(v2)
		
	float dot = v1 * v2

	if(dot > 0.9999)
		return true
	
	return false
}

//-----------------------------------------------------------------
bool PointOnPlane(vector pos, float plane[4])
{
	float dist = pos[0] * plane[0] + (pos[1] * plane[1]) + (pos[2] * plane[2]) - plane[3]
	
	if(fabs(dist) < 0.1)
		return true
	else
		return false
}

//-----------------------------------------------------------------
vector PlaneFromPoints(vector pnt[], int np, float plane[4])
{
	float z = pnt[1][2]
	if(pnt[0][2] == z && pnt[2][2] == z)
	{
		plane[0] = 0
		plane[1] = 0
		plane[2] = 1
		plane[3] = z
		
		return UpVec
	}

	for(int n = 0; n < np; n++)
	{
		vector p = pnt[n + 1 % np]
		
		vector v1, v2

		v1 = pnt[n] - p
		v2 = pnt[n + 2 % np] - p

		VectorNormalize(v1)
		VectorNormalize(v2)

		float d = fabs(v1 * v2)

		if(d < 0.9)
		{
			//have good enough vectors

			//cross - normal
			vector norm = v1 * v2

			//distance
			d = norm * p
			
			plane[0] = norm[0]
			plane[1] = norm[1]
			plane[2] = norm[2]
			plane[3] = d
			return norm
		}
	}

	//Shit! next try!
	for(n = 0; n < np; n++)
	{
		p = pnt[n + 1 % np]

		v1 = pnt[n] - p
		v2 = pnt[n + 2 % np] - p

		VectorNormalize(v1)
		VectorNormalize(v2)

		d = fabs(v1 * v2)

		if(d < 1.0)
		{
			//have good enough vectors

			//cross - normal
			norm = v1 * v2

			//distance
			d = norm * p
			
			plane[0] = norm[0]
			plane[1] = norm[1]
			plane[2] = norm[2]
			plane[3] = d
			return norm
		}
	}

	return ZeroVec
}

//-----------------------------------------------------------------
bool PointInPoly(vector p, vector pts[], int nump)
{
	for(int n = 0; n < nump; n++)
	{
		vector pt = pts[n]
		vector e = pts[n + 1 % nump] - pt
		VectorNormalize(e)
		vector pl = e * UpVec
		VectorNormalize(pl)
		float d = pl * pt
		
		float dist = pl * p

		if(d > dist)
			return false
	}
	
	return true
}
	

//-----------------------------------------------------------------
float Winding(vector pnt[], int np)
{
	vector center = pnt[0]
	
	for(int n = 1; n < np; n++)
		center = center + pnt[n]
	
	float div = 1 / np
	
	center = center * div

	float sum = 0
	for(n = 0; n < np; n++)
	{
		vector v1, v2, p
		
		v1 = pnt[n] - center
		v2 = pnt[n + 1] - center
		
		float a1 = FixAngle(atan2(v1[0], v1[1]) * RAD2DEG)
		float a2 = FixAngle(atan2(v2[0], v2[1]) * RAD2DEG)
		
		sum += DiffAngle(a1, a2)

	}
	
	return -sum
}

//=================================================================================
class PathArea
{
	PathArea	next
	int num
	int flags
	string name

	vector mins, maxs
	vector pos[64]
	int	indices[64]
	
	Shape points[64]
	int numpoints
	Shape lines
	Shape fill
	Shape arrow
	
	vector strip[192]
	int numv

	float plane[4]
	
	int actcolor
	
	bool notconvex
	
//-----------------------------------------------------------------
	void Fill(bool act)
	{
		if(fill)
		{
			fill.Destroy()
			fill = NULL
		}
		int color, n
		
		if(act)
		{
			color = 0x3fffff00 //selected color
			
			for(n = 0; n < numpoints; n++)
			{
				if(points[n] == NULL)
				{
					vector p = pos[n]
					points[n] = AddDShape(SHAPE_DIAMOND, 0x3fff00ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, p - "1 1 1", p + "1 1 1")
				}
			}
		}
		else
		{
			for(n = 0; n < numpoints; n++)
			{
				if(points[n])
				{
					Shape pt = points[n]
					pt.Destroy()
					points[n] = NULL
				}
			}

			if(flags & 2)
				color = 0x3f3f3fff
			else
				color = actcolor
		}
	
		fill = AddDTris(color, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_NOOUTLINE, strip, numv / 3)
	}
	
//-----------------------------------------------------------------
	void Activate(bool act)
	{
		if(!act)
		{
			actcolor = 0x3f3f3f7f
		}
		else
		{
			if(notconvex)
				actcolor = 0x3f7f0000
			else
			{
				actcolor = 0x3f007f00
			}
		}
		
		Fill(false)
	}
	
//-----------------------------------------------------------------
	void Show(bool show, bool act)
	{
		if(fill)
		{
			fill.Destroy()
			fill = NULL
		}
		
		if(arrow)
		{
			arrow.Destroy()
			arrow = NULL
		}
		
		if(lines)
		{
			lines.Destroy()
			lines = NULL
		}
int n

		if(!show)
		{
			for(n = 0; n < numpoints; n++)
			{
				if(points[n])
				{
					Shape pt = points[n]
					pt.Destroy()
					points[n] = NULL
				}
			}

			return
		}
			
		vector center = ZeroVec
		
		for(n = 0; n < numpoints; n++)
		{
			vector p = pos[n]
			center += p
			//points[n] = AddDShape(SHAPE_BBOX, 0x3fff00ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, p - "1 1 1", p + "1 1 1")
		}

		if(flags & 4)
		{
			vector dir = pos[1] - pos[0]
			dir[2] = 0
			
			VectorNormalize(dir)

			vector pdir = dir * "0 0 1"
			VectorNormalize(pdir)
			
			float scale = 1.0 / numpoints
			center = center * scale + "0 0 8"
			
			vector pts[6]
			pts[0] = center - pdir
			pts[1] = center + pdir
			pts[2] = dir * 24.0 + center
			pts[3] = dir * 18.0 + center + (pdir * 4.0)
			pts[4] = dir * 18.0 + center - (pdir * 4.0)
			pts[5] = dir * 24.0 + center
			
			arrow = AddDTris(0x3fffff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pts, 2)
		}
		lines = AddDLines(0xffff00ff, SS_NOZUPDATE|SS_DOUBLESIDE, pos, numpoints + 1)
		Fill(act)
	}
	
//-----------------------------------------------------------------
	void PathArea(vector pts[], int nump)
	{
		vector p, pnorm = PlaneFromPoints(pts, nump, plane)
		int src, inc, n
		
		if(Winding(pts, nump) > 0)
		{
			src = 0
			inc = 1
		}
		else
		{
			src = nump - 1
			inc = -1
			
			//flip plane
			float f
			f = plane[0]
			plane[0] = -f
			f = plane[1]
			plane[1] = -f
			f = plane[2]
			plane[2] = -f
			f = plane[3]
			plane[3] = -f
		}
		
		numpoints = nump

		mins = "9999 9999 9999"
		maxs = -mins
		
		for(n = 0; n < numpoints; n++)
		{
			p = pts[src]
			src += inc
			
			pos[n] = p
			
			if(p[0] < mins[0])
				mins[0] = p[0]
			if(p[1] < mins[1])
				mins[1] = p[1]
			if(p[2] < mins[2])
				mins[2] = p[2]
			
			if(p[0] > maxs[0])
				maxs[0] = p[0]
			if(p[1] > maxs[1])
				maxs[1] = p[1]
			if(p[2] > maxs[2])
				maxs[2] = p[2]
		}

		pos[numpoints] = pos[0]

		mins = mins - "1 1 2"
		maxs = maxs + "1 1 2"

		numv = PolyToTriangles(pos, numpoints, strip)

		notconvex = false

	//is it convex & planar?
		if(nump > 3)
		{
			int or = 0
			const vector up = "0 0 1"
			
			p = pos[0]
			
			float pdist = pnorm * p
			
			for(n = 0; n < nump; n++)
			{
				vector edge, plane, norm
				float d

				p = pos[n]
				
				//planar
				if(fabs(p * pnorm - pdist) > 0.1)
				{
					notconvex = true
					goto out
				}
				
				edge = p - pos[n + 1]
				
				VectorNormalize(edge)
	
				norm = edge * up
				VectorNormalize(norm)
	
				p = pos[n]
				d = norm * p
	
				for(int m = 0; m < nump; m++)
				{
					//ignore ourself
					if(abs(m - n) > 1)
					{
						p = pos[m]
						float dist = norm * p - d
			
						if(dist < -0.01)
							or |= 1
						else
						{
						 	if(dist > 0.01)
								or |= 2
						}
						
						if(or == 3)
						{
							notconvex = true
							goto out
						}
					}
				}
			}
		}
out:
		Show(true, true)
	}

//-----------------------------------------------------------------
	void ~PathArea()
	{
		Show(false, false)
	}
}

//=================================================================================
class PathPortal
{
	PathPortal	next

	PathArea Area[2]
	int pts[2][2]
	Shape pline1, pline2

	vector perp

//-----------------------------------------------------------------
	void Activate(int enable)
	{
		if(pline1)
		{
			pline1.Destroy()
			pline1 = NULL
		}
		if(pline2)
		{
			pline2.Destroy()
			pline2 = NULL
		}
		
		if(!enable)
			return
			
		int color
		
		vector lperp = perp * (float)0.3
		
		if(enable == 2)
		{
			color = 0xffffffff
			lperp = perp * (float)0.5
		}
		else
		{
			color = 0xff00efef
		}

		PathArea pa1, pa2
		
		pa1 = Area[0]
		pa2 = Area[1]
		
		if(!pline1)
			pline1 = AddDShape(SHAPE_LINE, color, SS_NOZUPDATE|SS_DOUBLESIDE, pa1.pos[pts[0][0]] + lperp, pa1.pos[pts[0][1]] + lperp)
		
		if(!pline2)
			pline2 = AddDShape(SHAPE_LINE, color, SS_NOZUPDATE|SS_DOUBLESIDE, pa1.pos[pts[0][0]] - lperp, pa1.pos[pts[0][1]] - lperp)
	}
		
//-----------------------------------------------------------------
	void PathPortal(PathArea pa1, int p11, int p12, PathArea pa2, int p21, int p22)
	{
		Area[0] = pa1
		Area[1] = pa2
		
		pts[0][0] = p11
		pts[0][1] = p12
		pts[1][0] = p21
		pts[1][1] = p22
		
		perp = pa1.pos[p11] - pa1.pos[p12]
		VectorNormalize(perp)
		float f = perp[0]
		perp[0] = perp[1]
		perp[1] = f
		
		Activate(true)
	}

//-----------------------------------------------------------------
	void ~PathPortal()
	{
		if(pline1)
			pline1.Destroy()
			
		if(pline2)
			pline2.Destroy()
	}
}

Shape ClipShapes[4096]
int NumClipShapes

const int UR_CREATE_AREA = 1
const int UR_DELETE_AREA = 2
const int UR_CREATE_GROUP = 3
const int UR_DELETE_GROUP = 4

class UndoRecord
{
	UndoRecord Next
	int Type
}

class PathUndoRecord extends UndoRecord
{
	PathArea Area
	void PathUndoRecord(int type)
	{
		Type = type
	}
	
	void ~PathUndoRecord()
	{
		PathArea area, next
		for(area = Area; area != NULL; area = next)
		{
			next = area.next
			delete area
		}		
/*
		PathGroup group, gnext
		for(group = Group; group != NULL; group = gnext)
		{
			next = group.Next
			delete group
		}*/
	}
}

const int EDITMODE_NONE = 0
const int EDITMODE_PATH = 1
const int EDITMODE_ENTITY = 2

//=================================================================================
class PathEditor extends MinClass
{
	PathUndoRecord UndoBuffer[32]
	int	UndoTop, UndoBottom

	void AddUndoRecord(PathUndoRecord pr)
	{
		UndoTop = UndoTop + 1 & 31
		if(UndoTop == UndoBottom)
		{
			PathUndoRecord ur, urnext
			for(ur = UndoBuffer[UndoTop]; ur != NULL; ur = urnext)
			{
				urnext = ur.Next
				delete ur
			}
		}
		UndoBuffer[UndoTop] = pr
	}

	int xpos, ypos
	int EdMouseDeltaX, EdMouseDeltaY	//su nastavavovane z GUI editoru
	int EdMouseX, EdMouseY	//su nastavavovane z GUI editoru
	int EdScreenMouseX, EdScreenMouseY
	bool EdLMBdown, EdRMBdown
	int EdKeyStates[400]
	bool CameraMode			//iba hybanie kamerou
	float CameraMovementSpeed

	int grid
	int TraceEntities

	vector cpos, cang
	
	bool Active	
	int ShowGrid	
	int Floor, SType
	
	//finalni pozice kurzoru
	vector end
	int  EdMode
	bool ClipsVisible



	vector CursorDir
	
	int mode

	bool		IsVisible
	
	vector	BakPoints[64]
	int		NumBakPoints
	
	Shape	AreaPoints[64]
	vector CurrentArea[64]
	int	NumPoints
	Shape	AreaShape
	int AreaFlags
	string AreaName

	int EditedPoint
	
	bool	bLMB, bRMB
	bool SnapPoints
	
	string			GroupNames[64]
	PathArea			First[64]
	PathPortal		FirstPortal[64]
	int				NumGroups
	
	int CurrentGroup, LastGroup

	PathArea SelectedArea
	PathPortal SelectedPortal
	
	bool AllowNonplanar

//---------------------------------------------------------------------------------
	bool IsFileWritable(string file)
	{
		int a = GetFileAttributesA(GetFilePath(file))
	
		if(a != -1 && a & 1)
			return false
		else
			return true
	}

	//---------------------------------------------------------------------------------
	bool GUIKeyState(int KeyCode, bool Clear)
	{
		int state = EdKeyStates[KeyCode]
		
		if(state == -1)
			return false

		if(state == 1 && Clear)
		{
			EdKeyStates[KeyCode] = -1
			return true
		}

		if(state == 1)	//moze byt este 0 a -1
			return true
		
		return false
	}

//-----------------------------------------------------------------
	bool Update3DCursor()
	{
		//virtualni fce. 
		//timto lze vypnout updatovani pozice kurzoru a gridu
		//napriklad v kombinaci s pop-up menu
		return true
	}
	
//-----------------------------------------------------------------
	PathArea FindNearestArea(vector p, vector bevel, PathArea pa)
	{
		float x, y, z
		x = p[0]
		y = p[1]
		z = p[2]
		
		if(pa == NULL)
			pa = First[CurrentGroup]

		PathArea best = NULL
		float bestdist = 9999 * 9999
		
		for(; pa != NULL; pa = pa.next)
		{
			vector mins = pa.mins - bevel
			vector maxs = pa.maxs + bevel
			
			if(x >= mins[0] && y >= mins[1] && z >= mins[2] && x <= maxs[0] && y <= maxs[1] && z <= maxs[2])
			{
				float d1,d2
				d1 = VectorLengthSq(mins - p)
				d2 = VectorLengthSq(maxs - p)
				if(d2 < d1)
					d1 = d2
				
				if(d1 < bestdist)
				{
					bestdist = d1
					best = pa
				}
			}
		}
		
		return best
	}

//-----------------------------------------------------------------
	vector SnapPoint(vector pos)
	{
		PathArea pa = NULL
		
		float gr = grid * 2
		vector bevel
		bevel[0] = gr
		bevel[1] = gr
		bevel[2] = gr
		
		while(true)
		{
			pa = FindNearestArea(pos, bevel, pa)
			
			if(!pa)
				return pos

			for(int v = 0; v < pa.numpoints; v++)
			{
				if(VectorLength(pa.pos[v] - pos) <= gr)
				{
					if(SType == 2)
					{
						pos = pa.pos[v] - "0 0 1"
					}
					else
					{
						float f = pos[2]
						pos = pa.pos[v]
						pos[2] = f
					}
					return pos
				}
			}
			
			pa = pa.next

			if(!pa)
				return pos
		}

		return pos
	}
	
//-----------------------------------------------------------------
	vector UserSnapPoint(vector pos)
	{
		vector end = pos - "0 0 1024"
		float fplane[4]
		int TraceFlags = TRACE_WORLD
		
		if(TraceEntities)
			TraceFlags |= TRACE_ENTS
			
		TraceLineEx(PLAYER, pos, end, ZeroVec, ZeroVec, NULL, fplane, NULL, NULL, TraceFlags, NULL)

		Floor = g_iTraceBrush
		SType = g_iTraceSurfaceType

		vector gr

		gr[0] = grid
		gr[1] = grid
		gr[2] = 1

		end = SnapVector(end, gr)

		if(SnapPoints)
			return SnapPoint(end)

		return end
	}

//-----------------------------------------------------------------
//timto je mozno externe zmenit pozici kamery
	void SetCam(vector pos, vector angles)
	{
		cpos = pos
		cang = angles
	}

//-----------------------------------------------------------------
	void ActivateEnts(bool enable)
	{
		ExtClass ec
		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(enable)
				ec.EditorOpened()
		}
	} 

//-----------------------------------------------------------------
	void SetEditMode(int mode)
	{
	}

//-----------------------------------------------------------------
	void SetMode(int m)
	{
		if(m != 2)
		{
			AreaFlags = 0
			AreaName = ""
		}
		
		mode = m
	}

//-----------------------------------------------------------------
	void ActivateGroup(int g, bool act)
	{
		PathArea pa
		
		for(pa = First[g]; pa != NULL; pa = pa.next)
			pa.Activate(act)
		
		PathPortal pp
		for(pp = FirstPortal[g]; pp != NULL; pp = pp.next)
			pp.Activate(act)
	}

//-----------------------------------------------------------------
	void SetCurrentGroup(int g)
	{
		if(g != CurrentGroup)
		{
			if(CurrentGroup >= 0 && CurrentGroup < NumGroups)
				ActivateGroup(CurrentGroup, false)

			CurrentGroup = g
			ActivateGroup(CurrentGroup, true)
		}
	}

//-----------------------------------------------------------------		
	vector points[4096]
	int nump
	
	int PointIndex(vector pos)
	{
		for(int n = 0; n < nump; n++)
		{
			if(VectorLengthSq(pos - points[n]) < 0.1)
				return n
		}
		
		points[nump++] = pos
		return n
	}
	
//-----------------------------------------------------------------
	PathArea AreaByNum(int num)
	{
		for(PathArea pa = First[CurrentGroup]; pa != NULL; pa = pa.next)
		{
			if(pa.num == num)
				return pa
		}
		
		return NULL
	}	
	
//-----------------------------------------------------------------
	bool CheckLoadedArea(PathArea pa)
	{
		vector strip[192]
		int numv = PolyToTriangles(pa.pos, pa.numpoints, strip)

		for(int n = 0; n < numv; n += 3)
		{
			vector p = strip[n]
			vector v1, v2, cross

			v1 = strip[n + 1] - p
			v2 = strip[n + 2] - p

			cross = v2 * v1

			float l = VectorLength(cross) * 0.5

			if(l == 0)
			{
				return false
			}

			if(cross[2] <= 0)
			{
				return false
			}
		}

		return true
	}
	
//-----------------------------------------------------------------
	void LoadArea(string tokens[], int npoints, int id, int flags, string name)
	{
		vector apts[64]
				
		for(int n = 0; n < npoints; n++)
			apts[n] = points[atoi(tokens[n])]
		
		PathArea pa = new PathArea(apts, npoints)
		
		pa.flags = flags
		pa.name = name
		
		PathArea prev
		
		prev = First[CurrentGroup]
		
		while(true)
		{
			if(prev == NULL || prev.next == NULL)
				break
			
			prev = prev.next
		}
		
		if(prev)
			prev.next = pa
		else
			First[CurrentGroup] = pa

		pa.num = id
		pa.Activate(false)
	}

//-----------------------------------------------------------------
	int RemoveFromAreas(PathArea area)
	{
		PathArea prev = NULL
		
		for(PathArea pa = First[CurrentGroup]; pa != NULL; pa = pa.next)
		{
			if(area == pa)
			{
				if(prev)
					prev.next = pa.next
				else
					First[CurrentGroup] = pa.next					
				
				pa.next = NULL
				return CurrentGroup
			}
			
			prev = pa
		}
	
		prev = NULL
		//removing from another group		
		for(int g = 0; g < NumGroups; g++)
		{
			if(g == CurrentGroup)
				continue

			for(pa = First[g]; pa != NULL; pa = pa.next)
			{
				if(area == pa)
				{
					if(prev)
						prev.next = pa.next
					else
						First[g] = pa.next					
					
					pa.next = NULL
					return g
				}
				
				prev = pa
			}
		}
		DPrint("Can't remove area!!!!")
		return -1
	}
	
//-----------------------------------------------------------------
	void DeleteArea(PathArea pa)
	{
		OnDataChange()

		int grp = RemoveFromAreas(pa)

		//remove attached portals
		PathPortal pp, next, prev = NULL
		for(pp = FirstPortal[grp]; pp != NULL; pp = next)
		{
			next = pp.next
			if(pp.Area[0] == pa || pp.Area[1] == pa)
			{
				if(prev)
					prev.next = next
				else
					FirstPortal[grp] = next

				delete pp
			}
			else
				prev = pp
		}

/*		PathUndoRecord ur = new PathUndoRecord(UR_DELETE_AREA)
		pa.Show(false, false)
		ur.Area = pa
		AddUndoRecord(ur)*/
		delete pa
	}
		
//-----------------------------------------------------------------
	bool LoadData(string name)
	{

		CurrentGroup = 0
		int f = OpenFile(name, FILEMODE_READ)
		
		if(!f)
			return false

		string line
		int narea = 0, areaid, pts = 0, linenum = 0
		int areaflags
		string areaname
		
		FGets(f, line)

		if(line != "NAV1")
		{
			CloseFile(f)
			return false
		}
		
		string tokens[100]
		
		while(true)
		{
			if(FGets(f, line) < 0)
				break
				
			linenum++

   		int ntokens = ParseString(line, tokens)
			
			if(pts)
			{
				vector pos
				pos[0] = atof(tokens[0])
				pos[1] = atof(tokens[1])
				pos[2] = atof(tokens[2])
				
				points[nump++] = pos
				pts--
				continue
			}
			
			if(narea)
			{
				if(narea >= 3)
					LoadArea(tokens, narea, areaid, areaflags, areaname)

				narea = 0
				continue
			}
			
			if(tokens[0] == "points")
			{
				pts = atoi(tokens[1])
				nump = 0
				continue
			}

			if(tokens[0] == "group")
			{
				CurrentGroup = NumGroups
				GroupNames[NumGroups++] = tokens[1]
				continue
			}

			if(tokens[0] == "area")
			{
				areaid = atoi(tokens[1])
				narea = atoi(tokens[2])
				areaflags = 0
				areaname = ""
				
				if(ntokens > 3)
					areaflags = atoi(tokens[3])

				if(ntokens > 4)
					areaname = tokens[4]

				continue
			}
			
			if(tokens[0] == "portal")
			{
				PathArea p1, p2
				p1 = AreaByNum(atoi(tokens[1]))
				p2 = AreaByNum(atoi(tokens[4]))
				int p11, p12, p21, p22
				p11 = atoi(tokens[2])
				p12 = atoi(tokens[3])
				p21 = atoi(tokens[5])
				p22 = atoi(tokens[6])
				PathPortal pp = new PathPortal(p1, p11, p12, p2, p21, p22)
				pp.next = FirstPortal[CurrentGroup]
				FirstPortal[CurrentGroup] = pp
/*				
				if(ntokens > 7)
					pp.Name = tokens[7]
*/
				continue
			}
		}
		
		
		CloseFile(f)
		
		if(NumGroups)
		{
			for(int g = 0; g < NumGroups; g++)
			{
				PathArea next
				for(PathArea pa = First[g]; pa != NULL; pa = next)
				{
					next = pa.next
					if(!CheckLoadedArea(pa))
					{
						DPrint("Bad area. Skipped!")
						DeleteArea(pa)
					}
				}
			}
			return true
		}

			
		return false
	}
	
//-----------------------------------------------------------------
	void ShowClips(bool show)
	{
		Shape shp
		
		if(show)
			NumClipShapes = Brushes2Shape(C_DETAIL|C_PLAYERCLIP, 0x80, 0, 0, ClipShapes)
		else
		{
			for(int n = 0; n < NumClipShapes; n++)
			{
				shp = ClipShapes[n]
				
				if(shp)
					shp.Destroy()
			}
			NumClipShapes = 0		
		}
	}

//-----------------------------------------------------------------
	void SaveData(string name)
	{
		string fname = GetFilePath(name)
		CopyFileA(fname, fname + ".bak", false)

		LastGroup = CurrentGroup
		int f = OpenFile(name, FILEMODE_WRITE)
		
		FPrintln(f, "NAV1")

		//prepare
		PathArea pa
		PathPortal pp
		int a, p, np
		
		nump = 0	
		int narea[256], nport[256]
		
		for(int g = 0; g < NumGroups; g++)
		{
			a = 0
			np = 0
			for(pa = First[g]; pa != NULL; pa = pa.next)
			{
				pa.num = a++
				for(p = 0; p < pa.numpoints; p++)
					pa.indices[p] = PointIndex(pa.pos[p])
			}
			
			for(pp = FirstPortal[g]; pp != NULL; pp = pp.next)
				np++
				
			narea[g] = a
			nport[g] = np;
		}
		
		FPrintln(f, String("points " + itoa(nump)))
			
		for(p = 0; p < nump; p++)
			FPrintln(f, vectoa(points[p]))

		for(g = 0; g < NumGroups; g++)
		{
			FPrintln(f, String("group " + GroupNames[g] + " " + itoa(narea[g]) + " " + itoa(nport[g])))

			for(pa = First[g]; pa != NULL; pa = pa.next)
			{
				string arealine = "area " + itoa(pa.num) + " " + itoa(pa.numpoints) + " " + itoa(pa.flags)
				
				if(pa.name)
					arealine += " " + pa.name
					
				FPrintln(f, arealine)
				
				for(p = 0; p < pa.numpoints; p++)
					FPrint(f, String(itoa(pa.indices[p]) + " "))

				FPrintln(f, "")
			}

			for(pp = FirstPortal[g]; pp != NULL; pp = pp.next)
			{
				PathArea p1, p2
				p1 = pp.Area[0]
				p2 = pp.Area[1]
				
				string pstr
				pstr = "portal "
				pstr += itoa(p1.num) + " " + itoa(pp.pts[0][0]) + " " + itoa(pp.pts[0][1]) + " "
				pstr += itoa(p2.num) + " " + itoa(pp.pts[1][0]) + " " + itoa(pp.pts[1][1])
				
/*
				if(pp.Name)
					pstr += " " + pp.Name
*/
				FPrintln(f, pstr)
			}

		}
		
		CloseFile(f)
		Print(String("File " + name + " is saved."))
	}

//-----------------------------------------------------------------
	void RemoveGroup(int g)
	{
		PathArea pa, next
		
		for(pa = First[g]; pa != NULL; pa = next)
		{
			next = pa.next
			delete pa
		}
		First[g] = NULL
		
		PathPortal pp, pnext
		for(pp = FirstPortal[g]; pp != NULL; pp = pnext)
		{
			pnext = pp.next
			delete pp
		}
		FirstPortal[g] = NULL
		
		for(int n = g; n < NumGroups; n++)
		{
			FirstPortal[n] = FirstPortal[n + 1]
			First[n] = First[n + 1]
			GroupNames[n] = GroupNames[n + 1]
		}
		NumGroups--
	}

//-----------------------------------------------------------------
	void CleanupAll()
	{
		while(NumGroups > 0)
			RemoveGroup(0)

		UpdateAreaGroupList(GroupNames, NumGroups)
		SelectedArea = NULL
		SelectedPortal = NULL
		Floor = NULL
	}

//-----------------------------------------------------------------
	int EffVolume
	int MusicVol

	void Activate(bool enable)
	{
		string name = World.ThisLevelDir + World.ThisLevelName + ".nav"
		ActivateEnts(enable)
		SetEditMode(EdMode)
			
		SetCurrentAreaAccesibility(false)
		
		if(enable)
		{
			EffVolume = g_iEffectsVolume
			MusicVol = g_iMusicVolume
		
			g_iEffectsVolume = 0
			g_iMusicVolume = 0

			SetMode(1)

			SetEventMask(this, EV_FRAME)
			Active = true
			World.PlayerControl = 0

			if(!LoadData(name))
			{
				CurrentGroup = 0
				GroupNames[0] = "Default"		
				NumGroups = 1
			}
			else
			{
				if(LastGroup >= NumGroups)
					LastGroup = 0
				CurrentGroup = -1
				SetCurrentGroup(LastGroup)
			}

			UpdateAreaGroupList(GroupNames, NumGroups)
			UpdateGroupSelection(GroupNames[CurrentGroup]) 
			ShowClips(ClipsVisible)
		}
		else
		{
			g_iEffectsVolume = EffVolume
			g_iMusicVolume = MusicVol
			ClearEventMask(this, EV_FRAME)
			Active = false
			World.PlayerControl = 1
			
//			SaveData(name)
			CleanupAll()
/*		
			World.LoadNAV(name, true)
			EnableEnemies()
			ShowClips(false)
*/
			ExtClass ec
			for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
			{
				if(ec._name && ec.DefPtr)	//musime vyhodit pripadne shapes
					ec.DestroyEditorVisuals()
			}
		}

//		SetAreasVis()
	}
	
//-----------------------------------------------------------------
	void DrawGrid(int show, vector end)
	{
		if(show == 0)
			return

		int ncolor, fcolor

		if(show == 1)
		{
			ncolor = 0x4f000000
			fcolor = 0xff202020
		}
		else
		{
			ncolor = 0x4fffffff
			fcolor = 0xffd0d0d0
		}

		const float tsize = 256, tsize2 = 128

		int lnum = tsize / grid + 1
		
		float sx = end[0] - tsize2
		
		int mid = lnum / 2
		
		vector v1, v2
		
		v1[2] = end[2] + 0.5
		v2[2] = v1[2]
		
		v1[1] = end[1] - tsize2
		v2[1] = end[1] + tsize2

		vector lines[520]
		int nl = 0

		for(int x = 0; x < lnum; x++)
		{
			v1[0] = sx
			v2[0] = sx				

			if(!x & 1)
			{
				lines[nl++] = v1
				lines[nl++] = v2
			}
			else
			{
				lines[nl++] = v2
				lines[nl++] = v1
			}

			sx = sx + grid
		}

		AddDLines(ncolor, SS_NOZBUFFER|SS_ONCE|SS_NOZUPDATE|SS_TRANSP, lines, nl)

		nl = 0

		float sy = end[1] - tsize2
		
		v1[0] = end[0] - tsize2
		v2[0] = end[0] + tsize2
		
		for(int y = 0; y < lnum; y++)
		{
			v1[1] = sy
			v2[1] = sy
	
			if(y & 1)
			{
				lines[nl++] = v1
				lines[nl++] = v2
			}
			else
			{
				lines[nl++] = v2
				lines[nl++] = v1
			}

			sy = sy + grid
		}
		AddDLines(ncolor, SS_NOZBUFFER|SS_ONCE|SS_NOZUPDATE|SS_TRANSP, lines, nl)

		v1 = Vector(end[0] - tsize2, end[1], end[2])
		v2 = Vector(end[0] + tsize2, end[1], end[2])		
		AddDShape(SHAPE_LINE, fcolor, SS_NOZBUFFER|SS_ONCE|SS_NOZUPDATE|SS_TRANSP, v1, v2)

		v1 = Vector(end[0], end[1] - tsize2, end[2])
		v2 = Vector(end[0], end[1] + tsize2, end[2])		
		AddDShape(SHAPE_LINE, fcolor, SS_NOZBUFFER|SS_ONCE|SS_NOZUPDATE|SS_TRANSP, v1, v2)			
	}
		
	bool GetAreasVis()
	{
		return true
	}

//-----------------------------------------------------------------
	void ShowAreas(bool show)
	{
		IsVisible = show

		for(int n = 0; n < NumGroups; n++)
		{
			for(PathArea pa = First[n]; pa != NULL; pa = pa.next)
				pa.Show(show, false)

			for(PathPortal pp = FirstPortal[n]; pp != NULL; pp = pp.next)
				pp.Activate(show)
		}
	}

//-----------------------------------------------------------------
	void SetAreasVis()
	{
		if(EdMode == EDITMODE_ENTITY)
			ShowAreas(GetAreasVis())
		
		if(EdMode == EDITMODE_PATH)
			ShowAreas(true)	

		if(EdMode == EDITMODE_NONE)
			ShowAreas(false)
	}

//-----------------------------------------------------------------
	void OnSavePathsFromGUI()
	{
		if(!World.EditorActive)
			return

		string NavFile = World.ThisLevelDir + World.ThisLevelName + ".nav"

		if(IsFileWritable(NavFile) == true)
			SaveData(NavFile)
		else
			RequestDialog("Navigation file " + NavFile + " is not writable!", "write error", GUI_DIALOG_OK)
	}

//-----------------------------------------------------------------
	PathArea FindArea(vector p, PathArea pa)
	{
		float x, y, z
		x = p[0]
		y = p[1]
		z = p[2]
		
		if(pa == NULL)
			pa = First[CurrentGroup]

		for(; pa != NULL; pa = pa.next)
		{
			float d = pa.plane[0] * x + (pa.plane[1] * y) + (pa.plane[2] * z)
			
			if(fabs(d - pa.plane[3]) > 2)
				continue

			if(x < pa.mins[0] && y < pa.mins[1] && x > pa.maxs[0] && y > pa.maxs[1])
				continue

			if(PointInPoly(p, pa.pos, pa.numpoints))
				return pa
		}
		
		return NULL
	}

//-----------------------------------------------------------------
	PathArea PickArea(vector from, vector dir, bool allgroups)
	{
		float bestdist = 99999
		PathArea bestarea = NULL

		int grp
		
		if(allgroups)
			grp = 0
		else
			grp = CurrentGroup

		while(grp < NumGroups)
		{
			for(PathArea pa = First[grp]; pa != NULL; pa = pa.next)
			{
				vector plane
				float planedist
				
				plane[0] = pa.plane[0]
				plane[1] = pa.plane[1]
				plane[2] = pa.plane[2]
				planedist = pa.plane[3]

				float v0 = plane * from - planedist
	
				if(v0 >= bestdist)
					continue
	
				float vd = plane * dir
				if(vd == 0) //paralel direction
					continue
	
				float scale = v0 / vd
				
				if(scale > 0)
					continue
	
				vector intersect = from + (dir * -scale)
	
				if(PointInPoly(intersect, pa.pos, pa.numpoints))
				{
					bestarea = pa
					bestdist = v0
				}
			}
			
			if(!allgroups)
				break
			
			grp++
		}
			
		return bestarea
	}

//-----------------------------------------------------------------
	void UpdatePoint(int p, vector pos, int color)
	{
		if(AreaPoints[p])
		{
			Shape pt = AreaPoints[p]
			pt.Destroy()
			AreaPoints[p] = NULL
		}
			
		CurrentArea[p] = pos
		
		if(color)
			color = 0x70ffff4f
		else
			color = 0x7000ffff
			
		AreaPoints[p] = AddDShape(SHAPE_DIAMOND, color, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, pos + "-2 -2 -2", pos + "2 2 2")
	}
	
//-----------------------------------------------------------------
	void EditArea(PathArea pa)
	{
		for(int n = 0; n < pa.numpoints; n++)
		{
			UpdatePoint(n, pa.pos[n], false)
			BakPoints[n] = pa.pos[n]
		}
		
		AreaFlags = pa.flags
		AreaName = pa.name
		NumPoints = pa.numpoints
		NumBakPoints = NumPoints
//Print(String("NumPoints: " + itoa(NumPoints)))
		DeleteArea(pa)
	}
	
//-----------------------------------------------------------------
	void CleanupCurrentArea()
	{
		if(AreaShape)
		{
			AreaShape.Destroy()
			AreaShape = NULL
		}
		
		for(int n = 0; n < 64; n++)
		{
			Shape pt = AreaPoints[n]
			if(pt)
			{
				pt.Destroy()
				AreaPoints[n] = NULL
			}
		}
		NumPoints = 0
	}
	
//-----------------------------------------------------------------
	void MergeGroup(int grp1, int grp2)
	{
		PathArea pa, next
		for(pa = First[grp2]; pa != NULL; pa = next)
		{
			next = pa.next
			pa.next = First[grp1]
			First[grp1] = pa
		}
		First[grp2] = NULL

		PathPortal pp, pnext
		for(pp = FirstPortal[grp2]; pp != NULL; pp = pnext)
		{
			pnext = pp.next
			pp.next = FirstPortal[grp1]
			FirstPortal[grp1] = pp
		}
		
		FirstPortal[grp2] = NULL

		RemoveGroup(grp2)
	}
	
//-----------------------------------------------------------------
	int AddGroup(string name)
	{
		FirstPortal[NumGroups] = NULL
		First[NumGroups] = NULL
		GroupNames[NumGroups++] = name
		
		return NumGroups - 1
	}

//-----------------------------------------------------------------
	bool EntIsWritable()
	{
		return IsFileWritable(World.ThisLevelDir + World.ThisLevelName + ".ent")
	}

//-----------------------------------------------------------------
	bool PrfIsWritable()
	{
	}

//-----------------------------------------------------------------
	bool IsConnected(PathArea p1, PathArea p2)
	{
		PathPortal pp
		for(pp = FirstPortal[CurrentGroup]; pp != NULL; pp = pp.next)
		{
			if(pp.Area[0] == p1 && pp.Area[1] == p2)
				return true
			if(pp.Area[0] == p2 && pp.Area[1] == p1)
				return true
		}
		
		return false
	}

//-----------------------------------------------------------------
	void DeletePoint(int p)
	{
		NumPoints--
		
		Shape pt = AreaPoints[p]
		if(pt)
		{
			pt.Destroy()
			AreaPoints[p] = NULL
		}

		for(int n = p; n < NumPoints; n++)
		{
			AreaPoints[n] = AreaPoints[n + 1]
			CurrentArea[n] = CurrentArea[n + 1]
		}
		AreaPoints[NumPoints] = NULL
	}
	
//-----------------------------------------------------------------
	void AddPoint(vector pos)
	{
		UpdatePoint(NumPoints, pos, false)
		NumPoints++
	}
	
//-----------------------------------------------------------------
	void UnEditPoint()
	{
		if(EditedPoint >= 0)
		{
			UpdatePoint(EditedPoint, CurrentArea[EditedPoint], false)
			EditedPoint = -1
		}
	}

//-----------------------------------------------------------------
	void EditPoint(int p)
	{
		UnEditPoint()
		EditedPoint = p
		UpdatePoint(p, CurrentArea[p], true)
	}
	
//-----------------------------------------------------------------
	void RemovePoint()
	{
		if(NumPoints)
		{
			NumPoints--
			Shape pt = AreaPoints[NumPoints]
			
			if(pt)
			{
				pt.Destroy()
				AreaPoints[NumPoints] = NULL
			}
		}
	}

//-----------------------------------------------------------------
	void InsertPointAtPos(int pos, vector p)
	{
		UnEditPoint()
		
		for(int n = NumPoints - 1; n >= pos; n--)
		{
			AreaPoints[n + 1] = AreaPoints[n]
			CurrentArea[n + 1] = CurrentArea[n]
		}

		AreaPoints[pos] = NULL
		UpdatePoint(pos, p, true)
		EditedPoint = pos
		NumPoints++
	}
	
//-----------------------------------------------------------------
	int InsertPoint(vector p)
	{
		vector p1, p2, v1, v2

		CurrentArea[NumPoints] = CurrentArea[0]

		int bestpoint = -1
		float bestdot = 0
		
		for(int n = 0; n < NumPoints; n++)
		{
			p1 = CurrentArea[n]
			p2 = CurrentArea[n + 1]
			
			v1 = p - p1
			v2 = p2 - p
			
			VectorNormalize(v1)
			VectorNormalize(v2)
			
			float dot = v1 * v2
			
			if(dot > bestdot)
			{
				bestpoint = n + 1
				bestdot = dot
			}
		}

		if(bestpoint >= 0)
			InsertPointAtPos(bestpoint, p)

		return bestpoint
	}
	
//-----------------------------------------------------------------
	void MergeArea()
	{
		bool merge = false
again:;
		PathArea ppa
		
		vector pos, pos2
		
		pos2 = CurrentArea[0]
		
		for(int n = 0; n < NumPoints; n++)
		{
			pos = pos2
			pos2 = CurrentArea[n + 1]

			for(ppa = First[CurrentGroup]; ppa != NULL; ppa = ppa.next)
			{
				if(PointOnPlane(pos, ppa.plane) && PointOnPlane(pos2, ppa.plane))
				{
					for(int p = 0; p <= ppa.numpoints; p++)
					{
						if(PointOnLine(ppa.pos[p], pos, pos2))
						{
							if(!merge)
							{
								if(RequestDialog("Connect area?", PathTitle, GUI_DIALOG_YES_NO) == GUI_REQUEST_YES)
									merge = true
								else
									return
							}
							
							InsertPointAtPos(n + 1, ppa.pos[p])
							goto again
						}
					}
				}
			}
		}
	}

//-----------------------------------------------------------------
	void AddArea()
	{	
		OnDataChange()
		MergeArea()
		
		PathArea ppa, pa = new PathArea(CurrentArea, NumPoints)
		
		pa.flags = AreaFlags
		pa.name = AreaName
		pa.Activate(true)

		vector pos, pos2
		
		pos2 = pa.pos[0]
		
		for(int n = 0; n < pa.numpoints; n++)
		{
			pos = pos2
			pos2 = pa.pos[n + 1]

			ppa = NULL

			while(true)
			{
				ppa = FindArea(pos, ppa)

				if(ppa == NULL)
					goto out

				if(ppa != pa && PointOnPlane(pos, ppa.plane))
				{
					for(int p = 0; p <= ppa.numpoints; p++)
					{
						//match points?
						if(VectorLengthSq(pos - ppa.pos[p]) < 0.1)
						{
							int pprev = p - 1
							if(pprev < 0)
								pprev = ppa.numpoints - 1
					
							if(VectorLengthSq(pos2 - ppa.pos[pprev]) < 0.1)
							{
								//portal!
								int pthis = p
								if(pthis == ppa.numpoints)
									pthis--

								PathPortal pp = new PathPortal(pa, n, n + 1, ppa, pthis, pprev)

								pp.next = FirstPortal[CurrentGroup]
								FirstPortal[CurrentGroup] = pp
							}
						}
					}
				}
				ppa = ppa.next
				
				if(ppa == NULL)
					goto out
			}
			out:
		}
		
		pa.next = First[CurrentGroup]
		First[CurrentGroup] = pa

		CleanupCurrentArea()
	}
	
//-----------------------------------------------------------------
/*
		PathUndoRecord Next
	void AddUndoRecord(PathUndoRecord pr)
	{
		UndoTop = UndoTop + 1 & 31
		if(UndoTop == UndoBottom)
		{
			PathUndoRecord ur, urnext
			for(ur = UndoBuffer[UndoTop]; ur != NULL; ur = urnext)
			{
				urnext = ur.Next
				delete ur
			}
		}
		UndoBuffer[UndoTop] = pr
	}
*/
//-----------------------------------------------------------------
	int FindPoint(vector pos)
	{
		for(int n = 0; n < NumPoints; n++)
		{
			if(VectorLength(CurrentArea[n] - pos) < 8)
				return n
		}
		
		return -1
	}
	
//-----------------------------------------------------------------
	bool CheckArea(int np)
	{
		if(np <= 2)
			return true

		float plane[4]
		vector p1, p2, q1, q2

		vector norm = PlaneFromPoints(CurrentArea, np, plane)

		if(norm == ZeroVec)
			return false

		if(np == 3)
			return true

		float dist, d = plane[3]
		
		if(!AllowNonplanar)
		{
			//check planarity
			for(int n = 0; n < np; n++)
			{
				p1 = CurrentArea[n]
				dist = p1 * norm - d
				
				if(fabs(dist) > 0.1)
					return false
			}
		}

		bool res = true
		//check crossing lines
		int lp = np// - 1
		for(n = 0; n < lp; n++)
		{
			//make plane from edge
			p1 = CurrentArea[n]
			p2 = CurrentArea[n + 1]
			vector edge = p2 - p1

			VectorNormalize(edge)
			vector pl = edge * norm
			d = p1 * pl
			
			//split all lines and search for cross
			for(int m = 0; m < lp; m++)
			{
				//skip us and neighbors
				if(abs(m - n) <= 1)
					continue

				q1 = CurrentArea[m]
				q2 = CurrentArea[m + 1]
				
				vector mid = SplitLine(q1, q2, pl, d)
				
				if(mid == ZeroVec)
					continue
			
				if(PointOnLine(mid, p1, p2))
				{
					AddDShape(SHAPE_DIAMOND, 0xffff0000, SS_ONCE|SS_DOUBLESIDE, mid + "-1 -1 -1", mid + "1 1 1")
					res = false
				}
			}
		}
		
		return res
	}
		
//-----------------------------------------------------------------
	bool NotAxial(vector p1, vector p2)
	{
		vector v = p2 - p1
		
		int n = 0
		
		if(v[0])
			n++
			
		if(v[1])
			n++
			
		if(v[2])
			n++
		
		if(n > 1)
			return true
		else
			return false
	}
		
//-----------------------------------------------------------------
	void UpdateArea()
	{
		if(AreaShape)
		{
			AreaShape.Destroy()
			AreaShape = NULL
		}
		
		if(NumPoints >= 1)
		{
			int color
			
			int lastp
			if(mode == 1) //new area
				lastp = NumPoints + 1
			else
				lastp = NumPoints
			
			AllowNonplanar = false
			//na krivce dovol cokoliv
			if(NumPoints >= 2 && SType == 2)
				AllowNonplanar = true

			if(CheckArea(lastp))
				color = 0xffffffff
			else
				color = 0xffff0000

			CurrentArea[lastp] = CurrentArea[0]
			AreaShape = AddDLines(color, SS_NOZUPDATE|SS_DOUBLESIDE, CurrentArea, lastp + 1)
			
			if(color == 0xffff0000)
				return

			for(int n = 0; n < lastp; n++)
			{
				if(NotAxial(CurrentArea[n], CurrentArea[n + 1]))
				{
					AddDShape(SHAPE_LINE, 0xff00ff00, SS_ONCE|SS_NOZUPDATE|SS_DOUBLESIDE, CurrentArea[n], CurrentArea[n + 1])
				}
			}				
		}
	}

//-----------------------------------------------------------------
	void DefaultPoly(vector end)
	{
		//defaultni polygon
		vector pts1[64], pts2[64], pts3[64], pts[]
		float planes[16][4]
		bool bounds[16]
		
		int nump = GetBrushFaces(Floor, planes)

		for(int n = 0; n < nump; n++)
		{
			if(fabs(planes[n][2]) < 0.2)
				bounds[n] = true
			else
				bounds[n] = false
		}

		vector to
		float plane[4]

//debug
		vector left, right, top, bottom

		end = end + "0 0 1"

		left = end + "-1024 0 0"
		TraceLine(NULL, end, left, NULL, plane, NULL, NULL)
		right = end + "1024 0 0"
		TraceLine(NULL, end, right, NULL, plane, NULL, NULL)
		top = end + "0 1024 0"
		TraceLine(NULL, end, top, NULL, plane, NULL, NULL)
		bottom = end + "0 -1024 0"
		TraceLine(NULL, end, bottom, NULL, plane, NULL, NULL)
		
		to = left
		to[1] = top[1]
		pts1[0] = to - "0 0 1"
		to = right
		to[1] = top[1]
		pts1[1] = to - "0 0 1"
		to = right
		to[1] = bottom[1]
		pts1[2] = to - "0 0 1"
		to = left
		to[1] = bottom[1]
		pts1[3] = to - "0 0 1"

		int num2, num = 4
		
		for(n = 0; n < nump; n++)
		{
			if(bounds[n])
			{
				plane[0] = planes[n][0]
				plane[1] = planes[n][1]
				plane[2] = planes[n][2]
				plane[3] = planes[n][3]
				ClipPoly(pts1, num, pts3, num2, pts2, num, plane)
				
				pts = pts2
				pts2 = pts1
				pts1 = pts
			}
		}

		for(n = 0; n < num; n++)
			AddPoint(pts1[n])
		
		SetMode(2)
	}

//-----------------------------------------------------------------
	int GetGroupIndex(string group)
	{
		for(int n = 0; n < NumGroups; n++)
		{
			if(group == GroupNames[n])	
				return n
		}
		return -1
	}

//-----------------------------------------------------------------
	void OnChangeGroupGromGUI(string group)
	{
		int GroupIndex = GetGroupIndex(group)
		
		if(GroupIndex == -1)
			return

		SetCurrentGroup(GroupIndex)
		PrintAreaEditInfo(5, "Group      : " + GroupNames[CurrentGroup])

		if(CurrentGroup >= 0)
		{
			PathArea pa = First[CurrentGroup]
			if(pa)
			{
				vector pos = pa.mins + pa.maxs * (float)0.5
				SetCam(pos + "0 0 32", cang)
			}
		}
	}

//-----------------------------------------------------------------
	void OnAddGroupGromGUI()
	{
		string val = ""
		if(TextEntryDialog(val, "New group"))
		{
			int res = AddGroup(val)
			SetCurrentGroup(res)
			UpdateAreaGroupList(GroupNames, NumGroups)
			UpdateGroupSelection(GroupNames[CurrentGroup]) 
		}
	}
	
//-----------------------------------------------------------------
	void OnDeleteGroupGromGUI(string group)
	{
		int GroupIndex = GetGroupIndex(group)
		
		if(GroupIndex == -1)
			return

		if(GroupIndex == CurrentGroup)
			CurrentGroup = GroupIndex - 1
		
		clamp CurrentGroup<0, CurrentGroup>

		RemoveGroup(GroupIndex)
		UpdateAreaGroupList(GroupNames, NumGroups)
		UpdateGroupSelection(GroupNames[CurrentGroup])
	}
	
//-----------------------------------------------------------------
	void OnRenameGroupGromGUI(string group)
	{
		int GroupIndex = GetGroupIndex(group)
		
		if(GroupIndex == -1)
			return

		string val = ""
		if(TextEntryDialog(val, "Rename group") && val)
		{
			GroupNames[GroupIndex] = val
			UpdateAreaGroupList(GroupNames, NumGroups)
			UpdateGroupSelection(GroupNames[GroupIndex])
		}
	}

//-----------------------------------------------------------------
	void OnMergeGroupGromGUI(string group1, string group2)
	{
		int GroupIndex1 = GetGroupIndex(group1)
		
		if(GroupIndex1 == -1)
			return

		int GroupIndex2 = GetGroupIndex(group2)
		
		if(GroupIndex2 == -1)
			return

		MergeGroup(GroupIndex1, GroupIndex2)
		UpdateAreaGroupList(GroupNames, NumGroups)
		
		for(int n = 0; n < NumGroups; n++)
		{
			if(GroupNames[n] == group1)
			{
				CurrentGroup = -1
				SetCurrentGroup(n)
				UpdateGroupSelection(GroupNames[CurrentGroup]) 
				break
			}
		}
	}

//-----------------------------------------------------------------
void SplitArea(PathArea pa, int i1, int i2)
{
	vector area1[64]
	int narea1 = 0
	
	NumPoints = 0
	
	for(int n = 0; n < pa.numpoints; n++)
	{
		//1st half
		if(n <= i1 || n >= i2)
		{
			area1[narea1++] = pa.pos[n]
		}

		//2nd half
		if(n >= i1 && n <= i2)
		{
			CurrentArea[NumPoints++] = pa.pos[n]
		}
	}

	DeleteArea(pa)

	AddArea()
	
	for(n = 0; n < narea1; n++)
		CurrentArea[n] = area1[n]
	
	NumPoints = narea1
	AddArea()
}

//-----------------------------------------------------------------
void TrySplitArea(vector p1, vector p2)
{
	for(PathArea pa = First[CurrentGroup]; pa != NULL; pa = pa.next)
	{
		int i1 = -1, i2 = -1
		
		for(int n = 0; n < pa.numpoints; n++)
		{
			vector p = pa.pos[n]
			
			if(p == p1)
				i1 = n

			if(p == p2)
				i2 = n
		}

		//that's the one!?
		if(i1 >= 0 && i2 >= 0)
		{
			if(i1 > i2)
			{
				int tmp = i2
				i2 = i1
				i1 = tmp
			}

			tmp = pa.numpoints - 1
			if(i2 == tmp && i1 == 0)
				return

			tmp = i2 - i1
			if(tmp < 2)
				return
			 
			SplitArea(pa, i1, i2)
			SetMode(1)
			return
		}
	}
}

//-----------------------------------------------------------------
void OnDeleteSelectedAreaFromGUI()
{
	if(!World.EditorActive)
		return

	if(EdMode != EDITMODE_PATH)	//osetrenie nutnej prasarny. v GUI mame iba jedno delete a tak vola toto ale aj EntityEditoru OnDeleteSelectionFromGUI()
		return

	if(SelectedArea)
	{
		if(RequestDialog("Delete area?", PathTitle, GUI_DIALOG_YES_NO) == GUI_REQUEST_YES)
		{
			DeleteArea(SelectedArea)
			SelectedArea = NULL
		}
	}
}

	vector CursorPos

//-----------------------------------------------------------------
	void EveryFrame()	//virtualna funkcia pre entity editor
	{
		switch(mode)
		{
		case 1:
			PrintAreaEditInfo(0, "Mode       : Create area")
			break
		case 2:
			PrintAreaEditInfo(0, "Mode       : Edit area")
			break
		}

		PrintAreaEditInfo(1, "Points     : " + itoa(NumPoints))
		PrintAreaEditInfo(2, "Edit point : " + itoa(EditedPoint))
		PrintAreaEditInfo(3, "Grid size  : " + itoa(grid))
		PrintAreaEditInfo(4, "Snap points: " + bool2a(SnapPoints))
		PrintAreaEditInfo(6, "Coords     : " + vectoa(end))
		PrintAreaEditInfo(7, "entities as ground : " + bool2a(TraceEntities))

		if(GUIKeyState(wx_CONTROL, false))
		{
			if(GUIKeyState(wx_L, true))
			{
				CleanupAll()
				if(!LoadData(World.ThisLevelDir + World.ThisLevelName + ".nav"))
				{
					CurrentGroup = 0
					GroupNames[0] = "Default"		
					NumGroups = 1
					UpdateAreaGroupList(GroupNames, NumGroups)
					UpdateGroupSelection(GroupNames[CurrentGroup]) 
				}
				else
				{
					if(LastGroup >= NumGroups)
						LastGroup = 0
					CurrentGroup = -1
					SetCurrentGroup(LastGroup)
					UpdateAreaGroupList(GroupNames, NumGroups)
					UpdateGroupSelection(GroupNames[CurrentGroup]) 
				}
			}
		}

		vector pos, dir
	
		if(CameraMode == 0)
		{	
			switch(mode)
			{
			case 1:
				EditedPoint = -1
				CurrentArea[NumPoints] = end
				
				if(NumPoints == 0)
					AllowNonplanar = false
					
				if(EdLMBdown)
				{
					if(!bLMB)
					{
						bLMB = true
						
						//don't add duplicities
						if(FindPoint(end) == -1)
							AddPoint(end)
					}
				}
				else
					bLMB = false
	
				if(GUIKeyState(wx_BACK, true))
					RemovePoint()

/*				if(GUIKeyState(wx_DELETE, true))
				{
					if(SelectedArea)
					{
						if(RequestDialog("Delete area?", PathTitle, GUI_DIALOG_YES_NO) == GUI_REQUEST_YES)
						{
							DeleteArea(SelectedArea)
							SelectedArea = NULL
						}
					}
				}*/


				if(NumPoints == 0 && GUIKeyState(wx_X, true))
				{
					DefaultPoly(end)
					OnDataChange()
				}
				break
			
			case 2:
				if(EdLMBdown)
				{
					int pnt = FindPoint(end)
											
					if(pnt >= 0 && pnt != EditedPoint)
					{
						UnEditPoint()
						EditedPoint = pnt
					}

					if(EditedPoint >= 0)
						UpdatePoint(EditedPoint, end, true)
				}
				
				if(GUIKeyState(wx_1, true))
				{
					if(EditedPoint >= 0)
					{
						int p = EditedPoint
						UnEditPoint()
						EditedPoint = p + 1
						
						if(EditedPoint == NumPoints)
							EditedPoint = 0
					}
					else
						EditedPoint = 0
					
					EditPoint(EditedPoint)
				}
					
				if(GUIKeyState(wx_INSERT, true))
				{
					InsertPoint(end)
				}
				
				if(GUIKeyState(wx_DELETE, true))
				{
					if(EditedPoint >= 0)
					{
						DeletePoint(EditedPoint)

						//deleted whole area, switch to New mode
						if(NumPoints == 0)
							SetMode(1)
						else
						{
							//edit next point
							if(EditedPoint >= NumPoints)
								EditedPoint = 0
							
							EditPoint(EditedPoint)
						}		
					}	
				}
				break
			}

			if(GUIKeyState(wx_Q, true))
			{
				SnapPoints = true - SnapPoints
			}

			if(GUIKeyState(wx_T, true))
				TraceEntities = 1 - TraceEntities

			if(NumPoints == 0)
			{
				PathArea sel

				if(GUIKeyState(wx_SHIFT, false))
					sel = PickArea(CursorPos, CursorDir, true)
				else
					sel = PickArea(CursorPos, CursorDir, false)

				if(SelectedArea)
					SelectedArea.Fill(false)

				SelectedArea = sel

				if(sel)
				{
					sel.Fill(true)
					SetAreaFlagsInGUI(sel.flags)
					SetAreaNameInGUI(sel.name)
				}
				else
				{
					AreaFlags = 0
					AreaName = ""
					SetAreaFlagsInGUI(0)
					SetAreaNameInGUI("")
				}
			}

			if(GUIKeyState(wx_E, true))
			{
				if(NumPoints == 0)
				{
					if(SelectedArea)
					{
						SetAreaFlagsInGUI(SelectedArea.flags)
						SetAreaNameInGUI(SelectedArea.name)
						EditArea(SelectedArea)
						SelectedArea = NULL
						//tady si zkopirovat AreaName, AreaFlags

						SetCurrentAreaAccesibility(true)
						EditPoint(0)
						SetMode(2)
						OnDataChange()
						
						EdLMBdown = false
					}
				}
				else
				{
					if(mode != 2)
					{
						SetMode(2)
						EditPoint(0)
					}
					else
					{
						SetMode(1)
						UnEditPoint()
					}
				}
			}

			if(NumPoints)
			{
				if(GUIKeyState(wx_RETURN, true))
				{
					OnDataChange()
					if(NumPoints == 2)
						TrySplitArea(CurrentArea[0], CurrentArea[1])
					else
					{
						if(NumPoints > 2 && CheckArea(NumPoints))
						{
								//tady prepsat Areaname, AreaFlags
							AreaFlags = GetAreaFlags()
							AreaName = GetAreaNameInGUI()
							AddArea()
							AreaFlags = 0
							AreaName = ""
							SetCurrentAreaAccesibility(false)
							SetMode(1)
						}
					}
				}

				if(EdRMBdown)
				{
					OnDataChange()
					if(!bRMB)
					{
						bRMB = true
						
						if(NumPoints == 2)
							TrySplitArea(CurrentArea[0], CurrentArea[1])
						else
						{
							if(NumPoints > 2 && CheckArea(NumPoints))
							{
								AreaFlags = GetAreaFlags()
								AreaName = GetAreaNameInGUI()
								//tady prepsat Areaname, AreaFlags
								AddArea()
								AreaFlags = 0
								AreaName = ""
								SetCurrentAreaAccesibility(false)
								SetMode(1)
							}
						}
					}
				}
				else
					bRMB = false
			}

			if(GUIKeyState(wx_ESCAPE, false))
			{
				GUIKeyState(wx_ESCAPE, true)

				CleanupCurrentArea()
				
				if(mode == 2)
				{	
					if(NumBakPoints)
					{
						for(int n = 0; n < NumBakPoints; n++)
							AddPoint(BakPoints[n])

						AddArea()
					}
					SetMode(1)
					NumBakPoints = 0
				}
				SelectedArea = NULL
			}
			else
			{
				if(GUIKeyState(wx_N, true) && SelectedArea && SelectedArea.flags & 2)
				{
					string name = SelectedArea.name
					if(TextEntryDialog(name, "Portal name"))
						SelectedArea.name = name
				}
				
				if(!EdRMBdown)
					bRMB = false						
			}
		}

//		UpdateArea()

		if(GroupNames[CurrentGroup])
			PrintAreaEditInfo(5, "Group      : " + GroupNames[CurrentGroup])
	}

//-----------------------------------------------------------------
	void MainNotebookChangedFromGUI(int OldPage, int NewPage)
	{
		if(NewPage == 0)		//entity editor menu
			SetEditMode(EDITMODE_ENTITY)
		else
		{
			if(NewPage == 1)	//path edit menu
				SetEditMode(EDITMODE_PATH)
			else
				SetEditMode(EDITMODE_NONE)
		}		
	}

//-----------------------------------------------------------------
	void BeforeQuitFromGUI()
	{
		if(!World.EditorActive)	//nemozeme sejvovat v playmode
			return
	}

	//-----------------------------------------------------------------
	void OnGridSizeChangeFromGUI(int size)
	{
		grid = size
	}

//-----------------------------------------------------------------
	void OnGridTypeChange(int type)
	{
		clamp type<0, 2>
		ShowGrid = type
	}

//-----------------------------------------------------------------
	void OnCameraSpeedChangeFromGUI(float speed, int dd)
	{
		CameraMovementSpeed = speed
	}

//-----------------------------------------------------------------
	void MainUpdateFromGUI()	//volane z GUI editoru iba ked je potreba
	{
		if(!World.EditorActive)
			return

		if(CameraMode == 0)
		{	
			if(Update3DCursor())
			{
				CursorPos = UnprojectVector(EdMouseX, EdMouseY, CursorDir)
				end = CursorDir * (float)8192 + CursorPos
				float plane[4]
				int TraceFlags = TRACE_WORLD
				
				if(TraceEntities)
					TraceFlags |= TRACE_ENTS

				float f = TraceLineEx(PLAYER, CursorPos, end, ZeroVec, ZeroVec, NULL, plane, NULL, NULL, TraceFlags, NULL)				

				if(EdMode == EDITMODE_PATH)	//area editor
				{
					end = end + "0 0 1"
					end = UserSnapPoint(end)
					end = end + "0 0 1"
				}
			}
		}

//---
		
		if(EdMode == EDITMODE_PATH)
			EveryFrame()
	}
	
//-----------------------------------------------------------------
	void PathEditor()
	{
		CurrentGroup = 0
		GroupNames[0] = "Default"		
		SnapPoints = true

		int PageSelected = GetNotebookActivePage()
		
		if(PageSelected == 0)
		{
			EdMode = EDITMODE_ENTITY		//entity edit
			ShowGrid = 1
		}
		else
		{
			if(PageSelected == 1)
			{
				EdMode = EDITMODE_PATH		//entity edit
				ShowGrid = 1
			}
			else
				EdMode = EDITMODE_NONE
		}	

		IsVisible = false
	}

//-----------------------------------------------------------------
	void ~PathEditor()
	{
		while(UndoTop != UndoBottom)
		{
			UndoTop = UndoTop - 1 & 31
	
			if(UndoBuffer[UndoTop])
				delete UndoBuffer[UndoTop]
		}
		
		ShowClips(false)
		g_iEffectsVolume = EffVolume
		g_iMusicVolume = MusicVol
		CleanupAll()
	}	
}
