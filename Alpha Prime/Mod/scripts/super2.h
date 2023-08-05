int g_LMb
int g_MMb
int g_RMb

//=================================================================================
void UpdateMouseStatus()
{
	for(int n = 0; n < 8; n++)
	{
	  	if(g_JoyButtons[n] != -1)
	  	{
	  		g_JoyButtons[n] = JoyButtons[n]
	  	}
	  	else
	  	{
	  		if(JoyButtons[n] == 0)
	  			g_JoyButtons[n] = 0
	  	}
	}
	
  	if(g_LMb != -1)
  	{
  		g_LMb = LMB
  	}
  	else
  	{
  		if(LMB == 0)
  			g_LMb = 0
  	}

  	if(g_MMb != -1)
  	{
  		g_MMb = MMB
  	}
  	else
  	{
  		if(MMB == 0)
  			g_MMb = 0
  	}

  	if(g_RMb != -1)
  	{
  		g_RMb = RMB
  	}
  	else
  	{
  		if(RMB == 0)
  			g_RMb = 0
  	}
}

//=================================================================================
int ClearMousePress(int button)
{
	if(button == 0)
	{
		g_LMb = -1
		g_JoyButtons[0] = -1
	}
	
	if(button == 1)
	{
		g_MMb = -1
		g_JoyButtons[1] = -1
	}
	
	if(button == 2)
	{
		g_RMb = -1
		g_JoyButtons[2] = -1
	}
}

//=================================================================================
int MousePress(int button)
{
	switch(button)
	{
		case 0:
			if(g_LMb == 1)
				return 1
			if(g_JoyButtons[0] == 1)
				return 1
		break

		case 1:
			if(g_MMb == 1)
				return 1
			if(g_JoyButtons[1] == 1)
				return 1
		break

		case 2:
			if(g_RMb == 1)
				return 1
			if(g_JoyButtons[2] == 1)
				return 1
		break
	}
	return 0
}

//=================================================================================
int MouseUp(int button)
{
	switch(button)
	{
		case 0:
			if(g_LMb != 0 && LMB == 0)
				return 1
		break

		case 1:
			if(g_MMb != 0 && MMB == 0)
				return 1
		break

		case 2:
			if(g_RMb != 0 && RMB == 0)
				return 1
		break
	}
	return 0
}






vector OptimalCross(vector vec)
{
	const vector tmp[3] = {"1 0 0", "0 1 0", "0 0 1"}
	float dot
	float BestDot = 10000000000
	int result

	for(int n = 0; n < 3; n++)
	{
		vector v = tmp[n]
		dot = vec * v
		dot = dot * dot  //tim se zbavime znaminka. Je to daleko rychlejsi, nez podminene vetveni

		if(dot < BestDot)
		{
			BestDot = dot
			result = n
		}
	}
	return tmp[result]
}


float Degrees2Radians(float degrees)
{
   return degrees / 57.29578  
}


float AkcelMovement(float actPos, float endpos, float startpos, float speed, float akcel)
{
	float hh, gg, mm

	hh = endpos - startpos		//zaciatok
	gg = endpos - actPos			//do konca
	gg = hh - gg					//od zaciatku

	if(hh == 0)
		hh = 0.00001

	mm = gg / hh					//percentualne prejdena cast od 0 do 1
	speed = speed + (akcel * sin(mm * 3.14)) * (ftime * 35)
	
	if(endpos >= actPos)
	{
		actPos = actPos + speed
		if(actPos > endpos)
		{
			actPos = endpos
			return actPos
		}
	}

	if(endpos <= actPos)
	{
		actPos = actPos - speed
		if(actPos < endpos)
		{
			actPos = endpos
			return actPos
		}
	}

	return actPos
}

/*
//-----------------------------------------------------------------------------------------------------------
float SinusIncrement(float ActTime, float speed1, float speed2, float StartTime, float EndTime)
{
	if(ActTime <= 0.5)
		ActTime += speed1 * ftime
	else
		ActTime += speed2 * ftime

	if(ActTime >= EndTime)
		ActTime = EndTime

	float	Rate = ActTime / 1//FlashTime
	float ToAng = Rate * 180
	float res = sin(ToAng * DEG2RAD)
	
	if(res < 0)
		res = -res
		
	return res
}
*/



























//=================================================================================
const int UV_Wrap = 0		//UV sa opakuje
const int UV_Stretch = 1	//UV sa natiahne na celu dlzku vzniknuteho modelu
int	g_DynObjNumVert	//sem si ulozime pocet vertexov. budu sa potom hodit pri replacovani v inych classoch

vector 	 g_TempVerts[4096]		//iba pre temp pouzitie
int 		 g_TempIndices[6144]
float 	 g_TempUVs[8192]

float GlassMaskUV[4][128]	//[4][16][8]

//----------------------------------------------------------------------------------------------
void CalculateGlassMaskUV()
{
//	int CurMaskBox = 0
	float orgU, orgV

	int CurUVPos = 0
	int rot
	float tolerance = 0.005//0.002

	for(int v = 0; v < 2; v++)
	{
		for(int h = 0; h < 4; h++)
		{
			rot = 0
			orgU = h / 4
			orgV = v / 2

			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
			
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
			
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			
			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			
			rot++
			CurUVPos -= 8

//--------------
			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance

			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance  
			                                          
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
			                                          
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			
			rot++
			CurUVPos -= 8
			                                      
//--------------                               
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			                                      
			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			                                          
			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance  
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
			                                          
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
			
			rot++
			CurUVPos -= 8
			                                      
//--------------                               
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
			                                      
			GlassMaskUV[rot][CurUVPos++] = orgU + 0.25 - tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			                                            
			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0.5 - tolerance
			                                            
			GlassMaskUV[rot][CurUVPos++] = orgU + 0 + tolerance
			GlassMaskUV[rot][CurUVPos++] = orgV + 0 + tolerance
		}
	}                                                 
}                                              




//SegNum 	= pocet segmentov
//size		= rozmer jednej strany boxu
//SegLength = dlzka segmentu

vobject DynamicSprite(MinClass owner, vector Verts[], int Indices[], float UVs[], float width, float height, string shader)
{
	int NumVert = 4
	g_DynObjNumVert = NumVert
	int NumFaces = 2
	int NumInd = NumFaces * 3	//pocet indexov
	vector vertex
	float HalfWidth = width * 0.5
	float HalfHeight = height * 0.5

	vertex[0] = -HalfWidth
	vertex[1] = 0
	vertex[2] = -HalfHeight
	Verts[0] = vertex
	
	vertex[0] = -HalfWidth
	vertex[1] = 0
	vertex[2] = HalfHeight
	Verts[1] = vertex
	
	vertex[0] = HalfWidth
	vertex[1] = 0
	vertex[2] = HalfHeight
	Verts[2] = vertex

	vertex[0] = HalfWidth
	vertex[1] = 0
	vertex[2] = -HalfHeight
	Verts[3] = vertex

	Indices[0] = 0
	Indices[1] = 1
	Indices[2] = 3

	Indices[3] = 1
	Indices[4] = 2
	Indices[5] = 3

	UVs[0] = 1	//0
	UVs[1] = 1   //0
	                   
	UVs[2] = 1   //0
	UVs[3] = 0   //1
	                   
	UVs[4] = 0   //1
	UVs[5] = 0   //1
                      
	UVs[6] = 0   //1
	UVs[7] = 1   //0

	int nverts[1]
	int numindices[1]
	string shaders[1]
	nverts[0] = NumVert
	numindices[0] = NumInd
	shaders[0] = shader

	vobject Obj = CreateXOB(1, nverts, numindices, shaders)
	SelectObject(owner, Obj)	
	UpdateIndices(Obj, 0, Indices)	//najprv sa musia updatovat indexy!
	UpdateVertsEx(owner, 0, Verts, UVs)
	return Obj
}

const int IndexTable0[8] = {5, 5, 6, 6, 7, 7, 4, 4}
const int IndexTable1[8] = {4, 0, 5, 1, 6, 2, 7, 3}
const int IndexTable2[8] = {0, 1, 1, 2, 2, 3, 3, 0}
const float UVTable[4] = {0, 0.5, 1, 0.5}

vobject SegmentedBox(MinClass owner, vector Verts[], int Indices[], float UVs[], float size, float Length, int SegNum, int UV_Type, string shader)
{
	float SegLength = Length / SegNum
	float offset = size * 0.5
	int n
	int NumVert = SegNum + 1
	NumVert *= 4		//pocet vertexov
	g_DynObjNumVert = NumVert
	int NumFaces = SegNum * 4 * 2
	int NumInd = NumFaces * 3	//pocet indexov
	vector vertex
	float lng
	int nseg = 0

	for(n = 0; n < NumVert; n += 4)	//naplnime vertexy
	{
		lng = SegLength * (float)nseg++
		
		vertex[0] = offset
		vertex[1] = -offset	
		vertex[2] = lng
		Verts[n] = vertex
		
		vertex[0] = -offset
		vertex[1] = -offset	
		vertex[2] = lng
		Verts[n + 1] = vertex
		
		vertex[0] = -offset
		vertex[1] = offset	
		vertex[2] = lng
		Verts[n + 2] = vertex

		vertex[0] = offset
		vertex[1] = offset	
		vertex[2] = lng
		Verts[n + 3] = vertex
	}
	
	nseg = 0
	int nfac = 0
	int tmp

	for(n = 0; n < NumInd; n += 3)
	{
		if(nfac > 7)
		{
			nfac = 0
			nseg++
		}

		tmp = 4 * nseg
		Indices[n] = IndexTable0[nfac] + tmp
		Indices[n + 1] = IndexTable1[nfac] + tmp
		Indices[n + 2] = IndexTable2[nfac] + tmp
		nfac++
	}

	int nvert = 0
	nseg = 0
	float UVrate
	tmp = NumVert * 2		//na kazdy vertex dva parametre na UV
	
	for(n = 0; n < tmp; n += 2)
	{
		if(nvert > 3)
		{
			nvert = 0
			nseg++
		}

		if(UV_Type == UV_Wrap)
		{
			float width = size * 2
			UVrate = SegLength * (float)nseg
			UVrate = UVrate / width
		}
		
		if(UV_Type == UV_Stretch)
		{
			UVrate = nseg						//ktory segment prave robime
			UVrate = UVrate * SegLength	//jak je to daleko od zaciatku
			UVrate = UVrate / Length		//percentualna cast z celej dlzky
		}
		
		UVrate -= 0.005						//natvrdo to skratime pretoze za UV zacinalo wrapovat
		UVs[n] = UVTable[nvert]
		UVs[n + 1] = UVrate
		nvert++
	}

	int nverts[1]
	int numindices[1]
	string shaders[1]
	nverts[0] = NumVert
	numindices[0] = NumInd
	shaders[0] = shader

	vobject Obj = CreateXOB(1, nverts, numindices, shaders)
	SelectObject(owner, Obj)	
	UpdateIndices(Obj, 0, Indices)	//najprv sa musia updatovat indexy!
	UpdateVertsEx(owner, 0, Verts, UVs)
	return Obj
}


const int TrailIndexTable0[4] = {6, 6, 5, 5}
const int TrailIndexTable1[4] = {4, 0, 7, 3}
const int TrailIndexTable2[4] = {0, 2, 3, 1}
const float TrailUVTable[4] = {0, 0, 1, 1}

//-----------------------------------------------------------------
vobject SegmentedCrossPlanes(MinClass owner, vector Verts[], int Indices[], float UVs[], float size, float Length, int SegNum, int UV_Type, string shader)
{
	float SegLength = Length / SegNum
	float offset = size * 0.5
	int n
	int NumVert = SegNum + 1
	NumVert *= 4							//pocet vertexov
	g_DynObjNumVert = NumVert
	int NumFaces = SegNum * 2 * 2
	int NumInd = NumFaces * 3			//pocet indexov
	vector vertex
	float lng
	int nseg = 0

	for(n = 0; n < NumVert; n += 4)	//naplnime vertexy
	{
		lng = SegLength * (float)nseg++
		
		vertex[0] = offset
		vertex[1] = 0
		vertex[2] = lng
		Verts[n] = vertex
		
		vertex[0] = 0
		vertex[1] = -offset	
		vertex[2] = lng
		Verts[n + 1] = vertex
		
		vertex[0] = -offset
		vertex[1] = 0
		vertex[2] = lng
		Verts[n + 2] = vertex

		vertex[0] = 0
		vertex[1] = offset	
		vertex[2] = lng
		Verts[n + 3] = vertex
	}
	
	nseg = 0
	int nfac = 0
	int tmp

	for(n = 0; n < NumInd; n += 3)
	{
		if(nfac > 3)	//mame jeden segment (4 faces)
		{
			nfac = 0
			nseg++
		}

		tmp = 4 * nseg
		Indices[n] = TrailIndexTable0[nfac] + tmp
		Indices[n + 1] = TrailIndexTable1[nfac] + tmp
		Indices[n + 2] = TrailIndexTable2[nfac] + tmp
		nfac++
	}

	int nvert = 0
	nseg = 0
	float UVrate
	tmp = NumVert * 2		//na kazdy vertex dva parametre na UV
	
	for(n = 0; n < tmp; n += 2)
	{
		if(nvert > 3)
		{
			nvert = 0
			nseg++
		}

		if(UV_Type == UV_Wrap)
		{
			UVrate = SegLength * (float)nseg
			UVrate = UVrate / size
		}
		
		if(UV_Type == UV_Stretch)
		{
			UVrate = nseg						//ktory segment prave robime
			UVrate = UVrate * SegLength	//jak je to daleko od zaciatku
			UVrate = UVrate / Length		//percentualna cast z celej dlzky
		}
		
		UVrate -= 0.005						//natvrdo to skratime pretoze za UV zacinalo wrapovat
		UVs[n] = TrailUVTable[nvert]
		UVs[n + 1] = UVrate
		nvert++
	}

	int nverts[1]
	int numindices[1]
	string shaders[1]
	nverts[0] = NumVert
	numindices[0] = NumInd
	shaders[0] = shader

	vobject Obj = CreateXOB(1, nverts, numindices, shaders)
	SelectObject(owner, Obj)	
	UpdateIndices(Obj, 0, Indices)	//najprv sa musia updatovat indexy!
	UpdateVertsEx(owner, 0, Verts, UVs)
}

vobject StreakModel(string shader)
{
	const int indices[18] = {0, 3, 2, 2, 1, 0, 1, 2, 7, 7, 4, 1, 4, 7, 6, 6, 5, 4}
	const int numindices[1] = {18}
	const int nverts[1] = {8}

	string shaders[1]
	shaders[0] = shader

	vobject Obj = CreateXOB(1, nverts, numindices, shaders)
	UpdateIndices(Obj, 0, indices)	//najprv sa musia updatovat indexy!
	return Obj
}

vobject SimpleSpriteModel(string shader)
{
	const int indices[18] = {0, 1, 2, 2, 3, 0}
	const int numindices[1] = {6}
	const int nverts[1] = {4}

	string shaders[1]
	shaders[0] = shader

	vobject Obj = CreateXOB(1, nverts, numindices, shaders)
	UpdateIndices(Obj, 0, indices)	//najprv sa musia updatovat indexy!
	return Obj
}

//=================================================================================
class ShotStreak extends MinClass
{
	vobject MainObject
	vector Start, End
	vector dir
	float WayLength
	float VirtualLength
	float FullLength
	float HalfWidth
	float Speed
	float CurPos
	int snd
	int ShaderNum
//	MinClass SoundBase

	void Update()
	{
		vector vec1 = dir * World.CameraNorm
		VectorNormalize(vec1)
		vector off = vec1 * HalfWidth
		vector spos, epos
		float tmpf

		if(CurPos >= VirtualLength)	//cele to skoncilo. mazeme
			delete this
				
		if(CurPos <= FullLength)	//zaciatok. iba roztiahneme streak na plnu velkost
		{
			spos = Start
			epos = dir * CurPos
			epos = Start + epos
			tmpf = CurPos / FullLength
		}
		else
		{
			if(CurPos >= WayLength)		//koniec. postupne stiahneme dlzku na nulu
			{
				tmpf = VirtualLength - CurPos
				spos = dir * tmpf
				spos = End - spos
				epos = End
			}
			else	//normalny stav
			{
				epos = dir * CurPos
				epos = Start + epos
				spos = epos - (dir * FullLength)
			}
		}

		vector v
		
//		epos - zaciatok streaku (leti ako prvy)
//		spos - koniec streaku (leti ako druhy)
		v = World.CameraPos
 		float SPosLng = VectorLength(epos - v)	//dlzka k prvemu bodu
 		vector SposDir = spos - v					//vector k druhemu bodu
 		VectorNormalize(SposDir)
 		vector spos2 = v + (SposDir * SPosLng)	//akoby druhy bod ale posunieme ho do rovnakej vzdialenosti ako je ten prvy

 		vector axis1 = spos2 - epos
 		VectorNormalize(axis1)
 		vector ToCameraDir = v - epos
 		VectorNormalize(ToCameraDir)
 		vector axis2 = axis1 * ToCameraDir							//osa kolma na axis1

		float FullWidth = HalfWidth * 2
		axis1 = axis1 * FullWidth
		axis2 = axis2 * HalfWidth
		vector vrt[8]

		v = epos + axis2
		vrt[0] = v - axis1
		vrt[1] = v
		v = epos - axis2
		vrt[2] = v
		vrt[3] = v - axis1
		v = spos + axis2
		vrt[4] = v
		vrt[5] = v + axis1
		v = spos - axis2
		vrt[6] = v + axis1
		vrt[7] = v

		const float StreakUVs[16] = {0, 0, 0, 0.5, 1, 0.5, 1, 0, 0, 0.5, 0, 1, 1, 1, 1, 0.5}

		UpdateVertsEx(this, 0, vrt, StreakUVs)
	}

	void EOnFrame(MinClass other, int extra)
	{
		CurPos += ftime * Speed
		Update()
	}

	void ShotStreak(vector start, vector end, float width, float length, float speed, int Shader)
	{
		if(width == 0)
			width = 1

		if(length == 0)
			length = 80

		Shader--
		ShaderNum = Shader

		int num = World.NumStreaks[Shader]
		if(num)
		{
			MainObject = World.Streaks[Shader][--num]
			World.NumStreaks[Shader] = num
		}
		else
			MainObject = StreakModel(StreakShaderNames[Shader])
	
		SelectObject(this, MainObject)
	
		SetFlags(this, TFL_VISIBLE)
		SetEventMask(this, EV_FRAME)

//		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, start, end)

		dir = end - start
		WayLength = VectorNormalize(dir)
		FullLength = length
		
		if(FullLength > WayLength)
			FullLength = WayLength

		VirtualLength = WayLength + FullLength
		Start = start
		End = end
		HalfWidth = width * 0.5
		CurPos = 0
		Speed = speed
		
		Update()
//		SoundBase = new MinClass
	}

	void ~ShotStreak()
	{
		int num = World.NumStreaks[ShaderNum]
		if(num < 32)
		{
			World.Streaks[ShaderNum][num++] = MainObject
			World.NumStreaks[ShaderNum] = num
		}
		else
		{
			SetObject(VNULL)
			ReleaseObject(MainObject, true)
		}

		if(snd)
			EndSound(snd)
		
//		delete SoundBase
	}
}

//=================================================================================
class LaserStreak extends MinClass
{
	vector pos1, pos2
	float HalfWidth
	vobject MainObject
	
//---------------------------------------------------------------------------------
	void Show(bool show)
	{
		if(show)
			SetFlags(this, TFL_VISIBLE)
		else
			ClearFlags(this, TFL_VISIBLE)
	}
	
//---------------------------------------------------------------------------------
	void SetPos(vector p1, vector p2)
	{
		pos1 = p1
		pos2 = p2
	}

//---------------------------------------------------------------------------------
	void Update()
	{
		vector v = World.CameraPos
 		float SPosLng = VectorLength(pos1 - v)	//dlzka k prvemu bodu
 		vector SposDir = pos2 - v					//vector k druhemu bodu
 		VectorNormalize(SposDir)
 		vector spos2 = v + (SposDir * SPosLng)	//akoby druhy bod ale posunieme ho do rovnakej vzdialenosti ako je ten prvy

 		vector axis1 = spos2 - pos1
 		VectorNormalize(axis1)
 		vector ToCameraDir = v - pos1
 		VectorNormalize(ToCameraDir)
 		vector axis2 = axis1 * ToCameraDir							//osa kolma na axis1

		float FullWidth = HalfWidth * 2
		axis1 = axis1 * FullWidth
		axis2 = axis2 * HalfWidth
		vector vrt[8]

		v = pos1 + axis2
		vrt[0] = v - axis1
		vrt[1] = v
		v = pos1 - axis2
		vrt[2] = v
		vrt[3] = v - axis1
		v = pos2 + axis2
		vrt[4] = v
		vrt[5] = v + axis1
		v = pos2 - axis2
		vrt[6] = v + axis1
		vrt[7] = v

		const float StreakUVs[16] = {0, 0, 0, 0.5, 1, 0.5, 1, 0, 0, 0.5, 0, 1, 1, 1, 1, 0.5}
		UpdateVertsEx(this, 0, vrt, StreakUVs)
	}

//---------------------------------------------------------------------------------
	void LaserStreak(vector p1, vector p2, float width, string Shader)
	{
		pos1 = p1
		pos2 = p2

		if(width == 0)
			width = 1

		HalfWidth = width * 0.5

		if(!Shader)
			Shader = "spr/shotstreak" 

		MainObject = StreakModel(Shader)
		SelectObject(this, MainObject)
		SetFlags(this, TFL_VISIBLE)
		Update()
	}

//---------------------------------------------------------------------------------
	void ~LaserStreak()
	{
		SetObject(VNULL)
		ReleaseObject(MainObject, 0)
	}
}




const float FlameThrowerRefract = 0.008

//=================================================================================
class DynamicOverlaySprite extends MinClass
{
	vector pos1, pos2
	float HalfWidth, Width
	vobject MainObject
	float row0[4]
	float row1[4]
	float Scale, DistanceScale

//---------------------------------------------------------------------------------
	void Show(bool show)
	{
		if(show)
			SetFlags(this, TFL_VISIBLE)
		else
			ClearFlags(this, TFL_VISIBLE)
	}
	
//---------------------------------------------------------------------------------
	void SetPos(vector p1, vector p2)
	{
		pos1 = p1
		pos2 = p2
	}
	
//---------------------------------------------------------------------------------
	void ScaleRefract(float scale)
	{
		Scale = scale
		row0[0] = FlameThrowerRefract * scale * DistanceScale
		row1[1] = row0[0]
	}

//---------------------------------------------------------------------------------
	void Update()
	{
		vector OrginalDir = pos2 - pos1						//pos2 je dalej od kamery
		float OrginalDist = VectorNormalize(OrginalDir)
		float LookScale = World.CameraNorm * OrginalDir
		float LookScale2 = fabs(LookScale)
		LookScale2 = 1 - LookScale2
		LookScale += 1
		LookScale = 2 - LookScale * 0.5
		
		vector v = World.CameraPos
 		float SPosLng = VectorLength(pos1 - v)	//dlzka k prvemu bodu
 		vector SposDir = pos2 - v					//vector k druhemu bodu
 		float SPos2Lng = VectorNormalize(SposDir)
 		vector vpos2 = v + (SposDir * SPosLng)
 		
/*		AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, pos1, spos2)
		AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, pos1 - "0.5 0.5 0.5", pos1 + "0.5 0.5 0.5")
		AddDShape(SHAPE_BBOX, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, spos2 - "0.5 0.5 0.5", spos2 + "0.5 0.5 0.5")
*/
 		vector axis1 = vpos2 - pos1
 		VectorNormalize(axis1)
 		vector ToCameraDir = v - pos1
 		VectorNormalize(ToCameraDir)
 		vector axis2 = axis1 * ToCameraDir							//osa kolma na axis1

		float LookOff2 = LookScale2 * HalfWidth
		float LookOff = LookScale * Width
//		Print(LookScale)

//		float LookScale3 = LookScale

		float wd1 = 5 + LookOff												//dozadu
		float wd2 = 10 + LookOff2 + LookOff								//do stran
		float wd3 = 10 + LookOff2 + LookOff								//dopredu

		vector off0 = axis1 * wd1	//dozadu
		vector off1 = axis2 * wd2	//do stran
		vector off2 = axis1 * wd3	//dopreru
/*		AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, pos2, pos2 + off0)
		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, pos2, pos2 + off1)

		AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, pos1, pos1 + off0)
		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, pos1, pos1 + off1)*/
//		axis1 = axis1 * HalfWidth
//		axis2 = axis2 * HalfWidth
		
		vector vrt[4]
		vrt[0] = pos1 + off1 - off0
		vrt[1] = pos1 - off1 - off0
		vrt[2] = vpos2 - off1 + off2
		vrt[3] = vpos2 + off1 + off2

		const float StreakUVs[8] = {1,1, 1,0, 0,0, 0,1}
		UpdateVertsEx(this, 0, vrt, StreakUVs)
		
		clamp SPosLng<0, 1024>
		float DistFactor = 1 - (SPosLng / 1024)
		DistanceScale = 0.2 + (0.8 * DistFactor)

		row0[0] = FlameThrowerRefract * Scale * DistanceScale
		row1[1] = row0[0]
	}

//---------------------------------------------------------------------------------
	void SetParms(vector p1, vector p2, float width)
	{
		pos1 = p1
		pos2 = p2

		if(width == 0)
			width = 1

		Width = width
		HalfWidth = width * 0.5
	}
	
	
//---------------------------------------------------------------------------------
	void DynamicOverlaySprite(vector p1, vector p2, float width, string Shader)
	{
		SetParms(p1, p2, width)

		if(!Shader)
			Shader = "spr/shotstreak" 

		MainObject = SimpleSpriteModel(Shader)
		SelectObject(this, MainObject)
		SetFlags(this, TFL_VISIBLE)
		Update()

		row0[0] = FlameThrowerRefract
		row0[1] = 0.0
		row0[2] = 0.0
		row0[3] = 0.0

		row1[0] = 0.0
		row1[1] = FlameThrowerRefract
		row1[2] = 0.0
		row1[3] = 0.0
		
		Scale = 1.0

		// m00 m01 m02 0
		// m10 m11 m12 0
	}

//---------------------------------------------------------------------------------
	void ~DynamicOverlaySprite()
	{
		SetObject(VNULL)
		ReleaseObject(MainObject, 0)
	}
}





vector GetTracPos(MinClass ent, vector from, vector dir)
{
	vector mins, maxs
	vector start = from
	vector end = from + dir
	float Plane[4]
	ExtClass TargEnt = NULL
	
	if(ent)
		GetBoundBox(ent, mins, maxs)
	else
	{
		mins = ZeroVec
		maxs = ZeroVec
	}
	
	TraceLineEx(PLAYER, start, end, mins, maxs, TargEnt, Plane, NULL, NULL, TRACE_WORLD, NULL)
//	AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, start, end)
	return end
}

bool IsBrushEntity(ExtClass ent)
{
	string mdl
	if(GetValue(ent, "model", 0, mdl))
	{
		if(strlen(mdl) > 1 && substr(mdl, 0, 1) == "*")
		{
			return true
		}
	}
	
	return false
}

//------------------------------------------------------------------------
//spravi maticu z uhlov a pozicie z LW
//vstup v metroch(LW) vystup v enforce units
vector LWmat[4]
void MakeMatrixFromLWParms(vector angles, vector pos)
{
	float H = angles[2]
	float P = angles[1]
	float B = angles[0]
	B = -B
	
	float ch = cos(H * DEG2RAD)
	float sh = sin(H * DEG2RAD)
	float cp = cos(P * DEG2RAD)
	float sp = sin(P * DEG2RAD)
	float cb = cos(B * DEG2RAD)
	float sb = sin(B * DEG2RAD)
	
	vector mh[3]
	vector mp[3]
	vector mb[3]

	mh[0] = Vector(ch, 0, sh)
	mh[1] = Vector(0, 1, 0)
	mh[2] = Vector(-sh, 0, ch)
	
	mp[0] = Vector(1, 0, 0)
	mp[1] = Vector(0, cp, -sp)
	mp[2] = Vector(0, sp, cp)
	
	mb[0] = Vector(cb, sb, 0)
	mb[1] = Vector(-sb, cb, 0)
	mb[2] = Vector(0, 0, 1)
	
	vector mat[3]
	vector res[3]
	MatrixMultiply3(mb, mp, mat)
	MatrixMultiply3(mat, mh, res)
	
	pos = pos * 32
	
	LWmat[0] = res[0]
	LWmat[1] = res[1]
	LWmat[2] = res[2]
	LWmat[3] = pos
}


int Modulo(int a, int b)	//zbytok po deleni
{
	float c = b
	int d = a / b
	float e = a / c
	float f = e - d
	float g = c * f
	int h = g
	return h
}

//----------------------------------------------------------------------------
vector GetDirectionFromAngle(float ang)
{
	if(ang == -1)
		return UpVec
		
	if(ang == -2)
		return "0 0 -1"

	return Angles2Vector(Vector(0, ang, 0))
}

//----------------------------------------------------------------------------
vector FixAngles(vector vec)
{
	vec[0] = FixAngle(vec[0])
	vec[1] = FixAngle(vec[1])
	vec[2] = FixAngle(vec[2])

	return vec
}

float LinearBlend(float CurValue, float TargetValue, float speed)
{
	if(CurValue != TargetValue)
	{
		if(TargetValue == 1)
			CurValue += ftime * speed
		else
			CurValue -= ftime * speed

		clamp CurValue<0, 1>
	}
	return CurValue
}

//---------------------------------------------------------------------------------	
float GetAngularOffset(float TargAngle, float CurAngle)
{
	TargAngle = FixAngle(TargAngle)
	CurAngle = FixAngle(CurAngle)
	float off, dir
	
	if(TargAngle > CurAngle)
	{
		off = TargAngle - CurAngle
		dir = 1
	}
	else
	{
		off = CurAngle - TargAngle
		dir = -1
	}

	if(off <= 180)
		return off * dir
	else
		return 360 - off * -dir
}

//---------------------------------------------------------------------------------	
vector EntityAnglesToCameraAngles(vector angles)
{
	vector CamAngs = angles
	float flip = CamAngs[0]
	CamAngs[0] = -flip
	return CamAngs
}

void ShowMatrixRich(vector mat[4], int ShapeFlags)
{
	vector start = mat[3]
	vector end
	
	end = mat[0] * 10
	end = start + end
	AddDShape(SHAPE_LINE, 0xffff0000, ShapeFlags, start, end)
	end = mat[1] * 10
	end = start + end
	AddDShape(SHAPE_LINE, 0xff00ff00, ShapeFlags, start, end)
	end = mat[2] * 10
	end = start + end
	AddDShape(SHAPE_LINE, 0xff0000ff, ShapeFlags, start, end)
}

void ShowMatrix(vector mat[4])
{
	ShowMatrixRich(mat, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP)
}

void ShowMatrixOnce(vector mat[4])
{
	ShowMatrixRich(mat, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE)
}

void GetAroundMatrixOfDir(vector dir, vector mat[3])
{
	vector up = "0 0 1"
	vector left = "1 0 0"
	vector pvec
	
	float UpDot = fabs(dir * up)
	float LeftDot = fabs(dir * left)

	if(UpDot < LeftDot)
		pvec = dir * up
	else
		pvec = dir * left

	mat[0] = dir
	mat[1] = dir * pvec
	mat[2] = dir * mat[1]
}

vector GetRandPosInAABB(MinClass ent)
{
	vector mins, maxs
	GetCBoundBox(ent, mins, maxs)
	float off0 = frand(mins[0], maxs[0])
	float off1 = frand(mins[1], maxs[1])
	float off2 = frand(mins[2], maxs[2])
	return Vector(off0, off1, off2)
}