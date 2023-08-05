class CreatureClass;
class info_cutscene;
class func_display;
class DisplayCollection;
class RigidBodyClass;
class ItemCollection;

int ErrorNum
int g_Subtitles
bool g_Viewer

vector g_vIdentMatrix[4] = {"1 0 0", "0 1 0", "0 0 1", "0 0 0"}
/*
vector[] OffsetMatrix(vector off)
{
	vector mat[4] = {"1 0 0", "0 1 0", "0 0 1"}
	
	mat[3] = off
	return mat
}
*/

vector g_CamMat[4]

int TRACE_DEFAULT

float LinearVol(float val)
{
	float tmp = 1 - (val * val * val * val)
	tmp = tmp * 0.5
	return val + tmp
}

//-----------------------------------------------------------------
vector Angles(int angle)
{
//-1 up
//-2 down
// 0 - 359 angle

vector out

	out = ZeroVec

	if( angle >= 0)
	{
		out[YAW] = angle
	}
	else
	{
		if( angle == -1)
			out[0] = 90
		else
			out[0] = 270
	}

	return out
}

vector atov(string str)
{
	string tokens[3]
	string tmp
	int pos
	vector vv = ZeroVec

	if(strlen(str) < 5)	//neni to vector
		return ZeroVec

	pos = strstr(str, " ")
	if(pos <= 0)
		return vv

	vv[0] = atof(substr(str, 0 , pos))
	str = substr(str, pos + 1, strlen(str) - 1 - pos)

	pos = strstr(str, " ")
	if(pos <= 0)
		return vv

	vv[1] = atof(substr(str, 0 , pos))
	str = substr(str, pos + 1, strlen(str) - 1 - pos)

	vv[2] = atof(str)

	return vv
}


string vectoa(vector vec)
{
	float f0 = vec[0]
	float f1 = vec[1]
	float f2 = vec[2]

	return ftoa(f0) + " " + ftoa(f1) + " " + ftoa(f2)
}

bool atob(string str)
{
	if(str == "true")
		return true
		
	return false
}

int StringToColor(string str)
{
	vector rgb = atov(str)
	float r = rgb[0] * (float)255
	float g = rgb[1] * (float)255
	float b = rgb[2] * (float)255
	return ARGB(63, r, g, b)
}

string RemoveChars(string str, string sample)
{
	string chr
	string buffer
	int lenght = strlen(str)

	if(lenght == 0)
		return str

	if(strlen(sample) != 1)
		return str

	for(int n = 0; n < lenght; n++)
	{
		chr = substr(str, n, 1)
		if(chr != sample)
			buffer = buffer + chr
	}

	string result = buffer
	buffer = ""
	return result
}

string ReplaceChars(string str, string OldChar, string NewChar)
{
	string chr
	string buffer
	int lenght = strlen(str)

	if(lenght == 0)
		return str

	if(strlen(OldChar) != 1)
		return str

	if(strlen(NewChar) != 1)
		return str

	for(int n = 0; n < lenght; n++)
	{
		chr = substr(str, n, 1)
		if(chr == OldChar)
			chr = NewChar

		buffer = buffer + chr
	}

	string result = buffer
	buffer = ""
	return result
}

void GetRotatedSpriteUVs(float UVarray[], float angle)
{
	float RadAngle = angle * DEG2RAD

	float s = sin(RadAngle) * 0.5
	float c = cos(RadAngle) * 0.5
	float is = -s
	float ic = -c

	UVarray[0] = ic + s + 0.5
	UVarray[1] = is - c + 0.5	
	UVarray[2] = c + s + 0.5
	UVarray[3] = s - c + 0.5
	UVarray[4] = c - s + 0.5
	UVarray[5] = s + c + 0.5
	UVarray[6] = ic - s + 0.5
	UVarray[7] = is + c + 0.5
}

vector IntersectLines(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	float a1,b1,c1,a2,b2,c2,det_inv,m1,m2
	float aa,bb

	aa = x1 - x0

	if (aa != 0)
	   m1 = y1 - y0 / (x1 - x0)
	else
	   m1 = 99999999999999   // close enough to infinity
	
	aa = x3 - x2

	if (aa != 0)
	   m2 = y3 - y2 / (x3 - x2)
	else
	   m2 = 99999999999999   // close enough to infinity
	
	a1 = m1
	a2 = m2
	
	b1 = -1
	b2 = -1
	
	c1 = y0 - (m1 * x0)
	c2 = y2 - (m2 * x2)
	
	aa = a1 * b2
	bb = a2 * b1
	det_inv = 1 / (aa - bb)
	
	float xi, yi
	aa = b1 * c2
	bb = b2 * c1
	xi = aa - bb * det_inv
	
	aa = a2 * c1
	bb = a1 * c2
	yi = aa - bb * det_inv
	return Vector(xi, yi, 0)
}

//--------------------------------------------------------------
bool InRect(int left ,int right ,int top ,int bottom ,int X ,int Y)
{
	if(X < left || X > right || Y < top || Y > bottom)
		return false

	return true
}

//--------------------------------------------------------------
bool InRectEx(int left ,int top ,int width ,int height ,int X ,int Y)
{
	int right = left + width
	int bottom = top + height

	if(X < left || X > right || Y < top || Y > bottom)
		return false

	return true
}

//--------------------------------------------------------------
vector ComputeInertiaForNullBody(float mass)
{
//	float a = mass * 10
	return Vector(mass, mass, mass)
}

//==================================================================
class Particle extends _eclass
{
	cache		16
	int      ClassType
	//float    ScaleTcMod
}

//==================================================================
class MinClass extends _eclass
{
	cache 	16
	int      ClassType
	int      ClassNum
	int		status

//-----------------------------------------------------------------
	void MinClass()
	{
		SetEventMask(this, EV_INIT)
	}
}

const int FADEOUT_START = 1
const int FADEOUT_END = 2
const int FADEIN_START = 3
const int FADEIN_END  = 4

const int ACT_DISABLE	= 0
const int ACT_ENABLE	= 1
const int ACT_BLOCKED	= 2

const int ACT_ENTITY_DEFAULT = 0		//user nenastavil nic tak entita nastavuje to co bude pravdepodobne vyhovovat
const int ACT_NONE = 1					//zakazane aktivovanie
const int ACT_USEKEY = 2
const int ACT_USEKEY_DISP = 3
const int ACT_USEKEY_DISTANCE = 4
const int ACT_DISTANCE_ONLY = 5

vector GetCenterOfEntity(MinClass ent)
{
	vector Pos, mins, maxs
	GetBoundBox(ent, mins, maxs)	//GetCBoundBox(ent, mins, maxs)

	vector offset = mins + maxs * (float)0.5
	vector matrix[4]

	GetMatrix4(ent, matrix)

	Pos = VectorMatrixMultiply4(matrix, offset)	//musime akceptovat transformaciu
/*
	vector Pos, mins, maxs
	Pos = GetMatrix(ent, 3)
	GetBoundBox(ent,mins,maxs)
	mins = Pos + mins
	maxs = Pos + maxs
	Pos = mins + maxs
	Pos = Pos * (float)0.5*/
	return Pos
}


class ItemDef extends _serializable
{
	string ID
	string name
	string description
	string model
	vobject obj
	bool physics
	bool pickable

//---------------------------------------------------------------------------
	void ~ItemDef()
	{
		if(obj)
		{
			ReleaseObject(obj, 0)
			obj = VNULL
		}
	}
}

class ItemCollection extends _serializable
{
	ItemDef Items[256]
	int	  ItemsNum

//---------------------------------------------------------------------------
	int GetItemIndex(string id)
	{
		if(!id)
			return -1

		ItemDef CurItem

		for(int n = 0; n < ItemsNum; n++)
		{
			CurItem = Items[n]

			if(CurItem.ID == id)
				return n
		}
		return -1
	}

//---------------------------------------------------------------------------
	ItemDef GetItem(string id)
	{
		int index = GetItemIndex(id)

		if(index == -1)
			return NULL

		return Items[index]
	}

//---------------------------------------------------------------------------
	ItemDef GetItemFromIndex(int index)
	{
		if(index >= ItemsNum)
			return NULL
			
		if(index == -1)
			return NULL

		return Items[index]
	}

//---------------------------------------------------------------------------
	vobject GetObjectFromID(string id)
	{
		ItemDef CurItem = GetItem(id)

		if(!CurItem)
			return VNULL

		if(CurItem.obj == VNULL)
			CurItem.obj = GetObject(CurItem.model)

		return CurItem.obj
	}

//---------------------------------------------------------------------------
	vobject GetObjectFromIndex(int index)
	{
		ItemDef CurItem = GetItemFromIndex(index)

		if(!CurItem)
			return VNULL

		if(CurItem.obj == VNULL)
			CurItem.obj = GetObject(CurItem.model)

		return CurItem.obj
	}

//---------------------------------------------------------------------------
	void LoadDefinitions()
	{
		tparse tp = BeginParse("scripts/definitions/items_def.h")

		if(!tp)
			return

		string tokens[10]
		int index
		int n
		int line = 0
		int MaxItems = sizeof(Items)
		ItemDef CurDef = NULL
		int MaxWeapons = 10					//prvych 10 su zbrane
		ItemsNum = MaxWeapons

		for(n = 0; n < MaxWeapons; n++)	//rezervovane na zbrane
			Items[n] = new ItemDef

		n = 0
		while(ParseLine(tp, n++, tokens))
		{
			if(ItemsNum >= MaxItems)
				continue

			if(tokens[0] == "WEAPON")
			{
				int WeapIndex = -1
				string value = tokens[1]
				GetValue(NULL, value, 0, WeapIndex)
				CurDef = Items[WeapIndex]
				CurDef.ID = tokens[1]
				continue
			}

			if(tokens[0] == "ITEM")
			{
				CurDef = new ItemDef
				Items[ItemsNum++] = CurDef
				CurDef.ID = tokens[1]
				continue
			}

			if(tokens[0] == "name")
			{
				if(CurDef)
					CurDef.name = RemoveQuotes(tokens[1])
				continue
			}

			if(tokens[0] == "model")
			{
				if(CurDef)
					CurDef.model = RemoveQuotes(tokens[1])
				continue
			}

			if(tokens[0] == "physics")
			{
				if(CurDef)
					CurDef.physics = atoi(tokens[1])
				continue
			}

			if(tokens[0] == "pickable")
			{
				if(CurDef)
					CurDef.pickable = atoi(tokens[1])
				continue
			}
		}
		EndParse(tp)
	}

//---------------------------------------------------------------------------
	void ItemCollection()
	{
		LoadDefinitions()

#ifdef EDITOR
		string AllItemNames[256]
		int AllItemNamesNum = 0
		ItemDef current

		for(int n = 0; n < ItemsNum; n++)
		{
			current = Items[n]

			if(current.name)
				AllItemNames[AllItemNamesNum++] = current.ID
		}

		SetItemNames(AllItemNames, AllItemNamesNum)
#endif
	}

//---------------------------------------------------------------------------
	void ~ItemCollection()
	{
		ItemDef CurItem
		for(int n = 0; n < ItemsNum; n++)
		{
			CurItem = Items[n]
			delete CurItem
			Items[n] = NULL
		}
	}
}

const float HG_OpacitySelected = 255
const float HG_OpacityUnselected = 70

//---------------------------------------------------------------------------------	
class HackInterface extends _eclass
{
	MinClass Owner
	int IconType
	string name
	string IconName
	string description
	widget WSymbol, WSymbol2, WLabel, WArrow, WProgress, WProgressBack
	float SelectTargetOpacity		//select znamena nafadovat do tejto hodnoty
	float SelectOpacity				//aktualna hodnota
	float GlobalVisibilityScale	//ovlada player podla viditelnosti Hack GUI
	float FlashScale					//koli selectu potrebujeme flashnut
	float FadeScaleTarget			//cielova hodnota
	float FadeScale					//normalny fade vidime/nevidime
	float DistanceScale
	float Xpos, Ypos
	float RotDir
	float RotAngle
	float HackingTime					//cas potrebny na zalogovanie (bar)
	float CurHackingTime				//jak dlho uz logujeme
	float ProgressFlickTimer		//ked je progres na konci tak text na nom blika
	int	ProgressTextVis

//---------------------------------------------------------------------------------	
	bool CanHacking()
	{
		return true
	}

//---------------------------------------------------------------------------------	
	int CanLogin()
	{
		if(CurHackingTime == HackingTime)
			return true
		else
			return false
	}

//---------------------------------------------------------------------------------	
	bool NeedLogOut()	//ked entita uz nechce aby bol player nalogovany
	{
		return false
	}

//---------------------------------------------------------------------------------	
	void LogIn()
	{
	}

//---------------------------------------------------------------------------------	
	void LogOut()
	{
	}

//---------------------------------------------------------------------------------	
	float GetMaxLoginTime()
	{
		return -1	//neobmedzeny
	}

//---------------------------------------------------------------------------------	
	bool SetHackPreviewCamera()
	{
		return false
	}

//---------------------------------------------------------------------------------	
	void Select(bool select)
	{
		if(select)
		{
			SelectTargetOpacity = HG_OpacitySelected
			FlashScale = 0
			
			if(CurHackingTime < HackingTime)	//to co sme uz raz nahackovali nebudeme hackovat znova
				CurHackingTime = 0
/*
			if(MouseDeltaX > 0)
				RotDir = -1
			else
				RotDir = 1
*/
         RotDir = -1

		}
		else
			SelectTargetOpacity = HG_OpacityUnselected
	}

//---------------------------------------------------------------------------------	
	void Show(bool show)
	{
		ShowWidget(WSymbol, show)
		ShowWidget(WSymbol2, show)
		ShowWidget(WLabel, show)
		ShowWidget(WArrow, show)
		ShowWidget(WProgress, show)
		ShowWidget(WProgressBack, show)
	}
	
//---------------------------------------------------------------------------------	
	void SetVisibility(float rate)
	{
		GlobalVisibilityScale = rate
	}

//---------------------------------------------------------------------------------	
	void FadeIn()
	{
		FadeScaleTarget = 1
	}

//---------------------------------------------------------------------------------	
	void FadeOut()
	{
		FadeScaleTarget = 0
	}

//---------------------------------------------------------------------------------	
	bool IsVisible()
	{
		if(FadeScaleTarget == 0)
			return false
		
		return true
	}

//---------------------------------------------------------------------------------	
	vector GetVisualPos()
	{
		return GetMatrix(Owner, 3)
	}

//---------------------------------------------------------------------------------	
	MinClass GetMainEntity()
	{
		return Owner
	}

//---------------------------------------------------------------------------------	
	vector OnFrontPlane(vector Point)
	{
		vector CameraVector = g_CamMat[1]
		vector CameraPos = g_CamMat[3]

		float d = CameraVector * CameraPos
	
		float d2 = CameraVector * Point
		d2 = -d2
	
		return CameraVector * d2 + Point
	}

//---------------------------------------------------------------------------------	
	void UpdateVisualizers(vector PlayerPos);

//---------------------------------------------------------------------------------	
	void OnFrame()
	{
	}

//---------------------------------------------------------------------------------	
	void BeforeInit()
	{
	}

	void HackInterface(MinClass owner);

//---------------------------------------------------------------------------------	
	void ~HackInterface()
	{
		if(WSymbol)
		{
			DestroyWidget(WSymbol)
			WSymbol = NULL
		}

		if(WSymbol2)
		{
			DestroyWidget(WSymbol2)
			WSymbol2 = NULL
		}

		if(WLabel)
		{
			DestroyWidget(WLabel)
			WLabel = NULL
		}

		if(WArrow)
		{
			DestroyWidget(WArrow)
			WArrow = NULL
		}
		
		if(WProgress)
		{
			DestroyWidget(WProgress)
			WProgress = NULL
		}
		
		if(WProgressBack)
		{
			DestroyWidget(WProgressBack)
			WProgressBack = NULL
		}
	}
}

//----------------------------------------------------------------
//Shoot() response
const int SR_NONE		= 0
const int SR_HIT		= 1
const int SR_COVERED	= 2
const int SR_KILL		= 3

const int HT_SHOT = 0
const int HT_EXPLOSION = 1
const int HT_IMPACT = 2
const int HT_FLAME = 3
const int HT_HURT_TRIGGER = 4

//----------------------------------------------------------------
class ShotStorage extends _serializable
{
	int		HurtType				//zakladny typ zranenia. strela, vybuch, zranenie fyzikou, plamen
	MinClass shooter
	CreatureClass	target		//pri zranovani fyzikou potrebujeme vediet koho budeme zranovat pretoze priamo z contact eventu to neni mozne
	vector	from					//u vybuchu je to pozicia vybuchu
	vector	VisualsFrom			//trasujeme z kamery ale streak poleti od zbrane
	vector	to
	vector	dir
	float		power					//sila pre impact/sila po vizualnej stranke
	float		damage				//kolko energie to zoberie
	float		BulletRainInc		//o kolik bude zvysovat bullet rain counter u AI jednotek (default == 1.0)
	int		NumBullets
	float		precision
	float		MarkSize				//velkost decalu
	int		RicDecalType		//typ strely pre decal				DE_BULLET, DE_PLASMA
	int		RicSndType			//typ strely pre RicSound			SE_BULLET, SE_PLASMA
	int		RicParticlesType	//typ strely pre RicParticles		PE_BULLET, PE_PLASMA
	float		StreakWidth
	float		StreakSpeed
	float		StreakLength
	float		CameraHitFactor	//jak to sekne kamerou ked player dostane zasah
	int		StreakShader
	int		ID						//viz. World::ShootID
	int		Bone					//copy of g_iTraceBone after TraceLineEx
	float		radius				//pre vybuch
	float		timer					//pri zranovani fyzikou pouzivame pre delay jak casto entitu target zranime
	float		timer2				//pri zranovani fyzikou pouzivame pre maximalny cas po ktory bude entita ktora toto vlastni zranovat
	int		flags					//pouziva sa pri zranovani fyzikou
//----------------------------------------------------------------
	int Shoot(MinClass other);
}

#ifdef EDITOR
class EntityDef
{
	string ClassName
	string Keys[70]
	string Events[30]
	string Color
	int 	 NumKeys
	int 	 NumEvents
	string VisualsType
	string VisualsSize

	void AddKey(string key)		//volane z GetEntityDefinitionsFromXML()
	{
		if(NumKeys >= sizeof(Keys))
			return

		Keys[NumKeys++] = key
	}

	void AddEvent(string event)
	{
		if(NumEvents >= sizeof(Events))
			return

		Events[NumEvents++] = event
	}
	
	void SetColor(string color)
	{
		Color = color
	}

	void SetDefaultVisuals(string type, string size)
	{
		VisualsType = type
		VisualsSize = size
	}
}

class EntityDefManager
{
	EntityDef EntDefs[200]
	int		 EntDefsNum

//--------------------------------------------------------------------------------
	void AddDefinition(EntityDef def)
	{
		EntDefs[EntDefsNum++] = def
	}

//--------------------------------------------------------------------------------
	EntityDef GetDefinition(string ClassName)
	{
		EntityDef Current
		for(int d = 0; d < EntDefsNum; d++)
		{
			Current = EntDefs[d]

			if(Current.ClassName == ClassName)
				return Current
		}
		return NULL
	}

//--------------------------------------------------------------------------------
	int GetClassNames(string array[])
	{
		EntityDef Current
		
		for(int d = 0; d < EntDefsNum; d++)
		{
			Current = EntDefs[d]
			array[d] = Current.ClassName
		}
		return EntDefsNum
	}

//--------------------------------------------------------------------------------
	void CleanUp()
	{
		for(int d = 0; d < EntDefsNum; d++)
		{
			delete EntDefs[d]
			EntDefs[d] = NULL
		}
		EntDefsNum = 0
	}

//--------------------------------------------------------------------------------
	void LoadDefinitions()
	{
		CleanUp()
		string ClassNames[200]
		int Num = GetClassNamesFromXML(ClassNames)

		EntityDef Current
		for(int n = 0; n < Num; n++)
		{
			Current = new EntityDef
			Current.ClassName = ClassNames[n]
			GetEntityDefinitionsFromXML(Current, Current.ClassName)	//do toho classu mi nasypeme definicie
			AddDefinition(Current)
		}		
	}

//--------------------------------------------------------------------------------	
	void EntityDefManager()
	{
		LoadDefinitions()
	}

//--------------------------------------------------------------------------------	
	void ~EntityDefManager()
	{
		CleanUp()
	}
}

EntityDefManager DefManager
#endif

//predchozi entita pri FindClass
MinClass PrevFind, DefFind

class ExtClass;
ExtClass FindClass(string name);

//=================================================================================
class ExtClass extends MinClass
{
	string	_name, _value, _state
	string	_userevents, _userstates
	string	_userevent0, _userevent1, _userevent2, _userevent3
	string	_surfaces

	string	parent			//kluc
	ExtClass Owner
	ExtClass FirstChild, NextChild,Parent
	int		Dependencies	// if(Dependencies > 0) - entita nemoze byt za behu zmazana pretoze na nej visi nejaka funkcnost
	
	string   link

	int 		UseClass
	float		ActivateDist
	int		ActivateMethod
	string	OnSuccess, OnFail, OnLoad

	string   UseName
	int		ActionNum

	string	display
	ExtClass Display
	
	HackInterface HackInterf

	string prefab

	int		FlameID	//filtrovani opakovaneho detekovani plamenem


//-----------------------------------------------------------------
// AI, jak pro CreatureClass, tak pro staticke a dynamicke prekazky.
//-----------------------------------------------------------------
	CPosition WPosition
	int WAFrame
	CAgent agent //kolizni primitiv pro PathEngine
	CPosition GetWPos();
	void UpdateAgent(bool set);

//-----------------------------------------------------------------
	void Show(bool show)
	{
		if(show)
			SetFlags(this, TFL_VISIBLE)
		else
			ClearFlags(this, TFL_VISIBLE)
	}

#ifdef EDITOR
	int DefXMLPtr			//pointer na element s definiciami klucov atd.
	EntityDef DefPtr		//kopia z XML pre veci ktore casto pouzivame

	string EKeyValues[40]
	int 	 EKeyTypes[40]
	bool	 CategoryHidden
	int	 CategoryHiddenFlags
	bool	 Hidden
	int	 HiddenFlags
	bool	 NotEditable
	int	 EngineFlags
	Shape	 MainVisShape
	string	ParentName	//entita ktora je parentom z AddChild()
	string 	ChildsNames[16]
	int		ChildsNamesNum

	ExtClass GetEventualOwner()
	{
		return this	
	}

//---------------------------------------------------------------------
	string GetKeyValue(string key)
	{
		for(int n = 0; n < DefPtr.NumKeys; n++)
		{
			if(DefPtr.Keys[n] == key)
				return EKeyValues[n]
		}
		return ""
	}

//---------------------------------------------------------------------
	void NullKeyValue(string key)
	{
		for(int n = 0; n < DefPtr.NumKeys; n++)
		{
			if(DefPtr.Keys[n] == key)
				EKeyValues[n] = ""
		}
	}

//---------------------------------------------------------------------
	void EditorOpened()
	{
	}

//---------------------------------------------------------------------
	bool CanEdit()				//niektore entity nemozno editovat vecsinou po nejakej udalosti. napriklad player ked zoberie zbran
	{
		return true
	}

//---------------------------------------------------------------------
	void CreateCustomVisuals()
	{
	}

//---------------------------------------------------------------------
	void UpdateCustomVisuals()
	{
	}

//---------------------------------------------------------------------
	void DestroyCustomVisuals()
	{
	}

//---------------------------------------------------------------------
	void UpdateMainVisualizer()
	{
		UpdateCustomVisuals()
		if(MainVisShape)
		{
			vector mat[4]
			GetMatrix4(this, mat)
			MainVisShape.SetMatrix(mat)
		}
	}

//---------------------------------------------------------------------
	void RefreshInEditor()	//volame z editoru namiesto UpdateEntity() aby sa rekurzivne refreshovalo vsetko potrebne
	{
		UpdateEntity(this)
		UpdateMainVisualizer()
		
		if(ChildsNamesNum > 0)
		{
			local ExtClass Child

			for(local int n = 0; n < ChildsNamesNum; n++)
			{
				Child = FindClass(ChildsNames[n])

				if(Child)
					Child.RefreshInEditor()	//ideme do rekurzivy
			}
		}
	}

//---------------------------------------------------------------------
	ExtClass FindParentRoot()
	{
		ExtClass ext = GetParent()
		
		if(!ext)
			return NULL

		while(true)
		{
			ExtClass parent = ext.GetParent()
			if(parent == NULL)
				return ext

			ext = parent
		}
	}

//---------------------------------------------------------------------
	void AfterReloadInEditor() //volane pote, co se entita zmenila/premistila a editor ji reloadnul
	{
	}

//---------------------------------------------------------------------
	void OnPositionChangingInEditor(vector Pos, vector Angs)	//volane kazdy frame sa meni pozicia pomocou editoru
	{
		SetOrigin(this, Pos)
		SetAngles(this, Angs)
		
		ExtClass root = FindParentRoot()	//musime updatovat zhora
		
		if(root)
			root.RefreshInEditor()	//namiesto UpdateEntity(this)
		else
			RefreshInEditor()

		UpdateMainVisualizer()
	}

//---------------------------------------------------------------------
	void CreateEditorVisuals()
	{
		if(CategoryHidden || Hidden)
			return

		CreateCustomVisuals()
		
		if(!MainVisShape)
		{
			UpdateEntity(this)
			vector mat[4]
			GetMatrix4(this, mat)
			int ShapeColor = StringToColor(DefPtr.Color)

			vector dsmin,dsmax
			if(DefPtr.VisualsType != "sphere")
			{
				if(DefPtr.VisualsSize == "?")	//z modelu
				{
					GetBoundBox(this, dsmin, dsmax)

					if(dsmin == ZeroVec && dsmax == ZeroVec)	//zrejme este nema priradeny model takze default pre taky pripad
					{
						dsmin = "-8 -8 -8"
						dsmax = "8 8 8"
					}					
				}
				else				//z definicie v xml
				{
					string tokens[16]
					int NumTok = ParseString(DefPtr.VisualsSize, tokens)
					float x = atof(tokens[0])
					float y = atof(tokens[1])
					float z = atof(tokens[2])
					dsmin = Vector(x, y, z)
					x = atof(tokens[3])
					y = atof(tokens[4])
					z = atof(tokens[5])
					dsmax = Vector(x, y, z)
				}

				dsmin = dsmin - "-0.04 -0.04 -0.04"
				dsmax = dsmax - "0.04 0.04 0.04"
			}
			
			switch(DefPtr.VisualsType)
			{
			case "diamond":
				MainVisShape = AddDShape(SHAPE_DIAMOND, ShapeColor, SS_NOZUPDATE|SS_TRANSP, dsmin, dsmax)
				break

			case "pyramid":
				MainVisShape = AddDShape(SHAPE_PYRAMID, ShapeColor, SS_NOZUPDATE|SS_TRANSP, dsmin, dsmax)
				break

			case "box":
				MainVisShape = AddDShape(SHAPE_BBOX, ShapeColor, SS_NOZUPDATE|SS_TRANSP, dsmin, dsmax)
				break
		
			case "sphere":
				MainVisShape = AddDSphere(ShapeColor, SS_NOZUPDATE|SS_TRANSP, ZeroVec, atof(DefPtr.VisualsSize))
				break
			}

			if(MainVisShape)
				MainVisShape.SetMatrix(mat)
		}
	} 

//---------------------------------------------------------------------
	void DestroyEditorVisuals()
	{
		if(MainVisShape)
		{
			MainVisShape.Destroy()
			MainVisShape = NULL
		}
		DestroyCustomVisuals()
	}
/*
//---------------------------------------------------------------------
	void SetVisualsStyle(int style)	//0=normal 1=selected 2=undercursor
	{
		string DefColor = DefPtr.Color
		vector rgb = atov(DefPtr.Color)
		float r = rgb[0] * (float)255
		float g = rgb[1] * (float)255
		float b = rgb[2] * (float)255
		float Alpha
		int NewColor

		if(style == 0)
		{
			Alpha = 63
			NewColor = ARGB(Alpha,r,g,b)
		}

		if(style == 1)
		{
			Alpha = 63
			r += 32
			g += 32
			b += 32
			clamp r<0, 255>
			clamp g<0, 255>
			clamp b<0, 255>
			NewColor = ARGB(Alpha,r,g,b)
		}

		if(style == 2)
		{
			Alpha = 15
			NewColor = ARGB(Alpha,0,255,255)//0x0f00ffff//ARGB(Alpha,r,g,b)
		}

		DestroyEditorVisuals()
		string DefColor = DefPtr.Color
		vector rgb = atov(DefPtr.Color)
		float r = rgb[0] * (float)255
		float g = rgb[1] * (float)255
		float b = rgb[2] * (float)255
		r += 64
		g += 64
		b += 64
		clamp r<0, 255>
		clamp g<0, 255>
		clamp b<0, 255>
		DefPtr.Color = ftoa(r) + " " + ftoa(g) + " " + ftoa(b)
		CreateEditorVisuals()
		DefPtr.Color = DefColor
	}
*/
//---------------------------------------------------------------------
	void HideInEditor(bool hide)
	{
		if(hide)
		{
			HiddenFlags = GetFlags(this)
			ClearFlags(this, TFL_VISIBLE | TFL_SOLID)
			Hidden = true				
		}
		else
		{
			ClearFlags(this, -1)
			SetFlags(this, HiddenFlags)
			Hidden = false
		}
	}

//---------------------------------------------------------------------
	void HideByCategory(bool hide)
	{
		if(hide)
		{
			CategoryHiddenFlags = GetFlags(this)
			ClearFlags(this, TFL_VISIBLE | TFL_SOLID)
			CategoryHidden = true
		}
		else
		{
			ClearFlags(this, -1)
			SetFlags(this, CategoryHiddenFlags)
			CategoryHidden = false
		}
	}
#endif

//-----------------------------------------------------------------
	bool WTraceFilter(ExtClass other, vector raypos, vector raydir)
	{
		return false
	}

//-----------------------------------------------------------------
	void OnParentRemove()
	{
		//virtualni fce, zavolana pro kazdy child, jehoz Parent se maze
	}
	
//-----------------------------------------------------------------
// user funkce, ktera nas prida do seznamu childu parent
//-----------------------------------------------------------------
	void AddToParent(ExtClass parent)
	{
		Parent = parent
		
		NextChild = parent.FirstChild
		parent.FirstChild = this
	}
	
//-----------------------------------------------------------------
// user funkce, ktera nas odstrani ze seznamu childu parenta
//-----------------------------------------------------------------
	void RemoveFromParent()
	{
		if(Parent == NULL)
			return
			
		ExtClass childs = Parent.FirstChild
		
		//to by se nemelo stat
		if(childs == NULL)
			return

		//pokud jsme prvni na seznamu
		if(childs == this)
		{
			Parent.FirstChild = NextChild
			return
		}
		
		while(childs)
		{
			if(childs.NextChild == this)
			{
				childs.NextChild = NextChild
				return
			}

			childs = childs.NextChild
		}
		
		//nenasli jsme se v seznamu. To by se nemelo stat :-/
	}
	
//-----------------------------------------------------------------
	void OnRemove()
	{
		//my sami muzeme mit parent.
		RemoveFromParent()
		
		ExtClass next
		for(ExtClass child = FirstChild; child != NULL; child = next)
		{
			//odzalohuj si pointer na dalsi child, protoze retezec se mezitim muze zmenit (child se sam smaze)
			next = child.NextChild
			//informuj childs o tom, ze budem smazani
			child.OnParentRemove()
		}
	}
	

//-----------------------------------------------------------------
	//pro animhooky volajici metody
	void Exec(string parm);
	
	void M_Exec(ExtClass other)
	{
		Exec(other._value)
	}
	
//-----------------------------------------------------------------
	void M_SetCamera(ExtClass other);

//-----------------------------------------------------------------
// virtualni metody, aby si entity osetrily zacatek/konec cutsceny
//-----------------------------------------------------------------
	void BeginCutscene()
	{
	}
	
//-----------------------------------------------------------------
	void EndCutscene()
	{
	}
	
//-----------------------------------------------------------------
// pro zvuky spoustene dynamickou metodou
//-----------------------------------------------------------------
	int P_Sounds[8]
	int NumPSounds

//-----------------------------------------------------------------
//DYNAMICKE METODY:
//-----------------------------------------------------------------
	void M_State(ExtClass other)
	{
		string tokens[16]
		int t = ParseString(_userstates, tokens)

		for(int n = 0; n < t; n++)
		{
			if(tokens[n] == ",")
				continue

			if(tokens[n] == other._value)
			{
				_state = other._value
				return
			}
		}

		DPrint("Entity '" + _name + "' doesn't have state '" + other._value + "'")
		ErrorNum++
	}

//-----------------------------------------------------------------
	void OnParent(ExtClass parent)	//je to volane entite ktora bola naparentena na inu entity z ingame skriptu (M_Parent())
	{
	}

//-----------------------------------------------------------------
	void M_Parent(ExtClass other);
	void M_Use(ExtClass other);

//-----------------------------------------------------------------
	void M_Solid(ExtClass other)
	{
		SetFlags(this, TFL_SOLID)
	}

//-----------------------------------------------------------------
	void M_NonSolid(ExtClass other)
	{
		ClearFlags(this, TFL_SOLID)
	}

//-----------------------------------------------------------------
	void M_EndSound(ExtClass other)
	{
		if(!NumPSounds) //zvuky uz skoncily
			return

		int n = atoi(other._value)

		if(P_Sounds[n])
		{
			EndSound(P_Sounds[n])
			P_Sounds[n] = 0
			NumPSounds--
		}
	}

//-----------------------------------------------------------------
	void M_Sound(ExtClass other)
	{
		int n, p = strstr(other._value, ",")
		string name
		int pf, lf = SF_3D
		//simplest case
		if(p == -1)
		{
			pf = SF_ONCE
			n = 0
			name = other._value
		}
		else
		{
			string val, parm

			pf = 0

			n = atoi(substr(other._value, 0, p))

			val = substr(other._value, p + 1, strlen(other._value) - p - 1)

			while(p != -1)
			{
				p = strstr(val, ",")

				if(p == -1)
					parm = val
				else
				{
					parm = substr(val, 0, p)
					val = substr(val, p + 1, strlen(val) - p - 1)
				}

				triminplace(parm)
				triminplace(val)

				strlower(parm)
				switch(parm)
				{
					case "once":
						pf |= SF_ONCE
					break

					case "ambient":
						pf |= SF_AMBIENT
						lf = 0
					break

					case "half":
						pf |= SF_HALFRANGE
					break

					case "quart":
						pf |= SF_QUARTERRANGE
					break

					default:
						name = parm
					break
				}
			}
		}

		if(P_Sounds[n])
			EndSound(P_Sounds[n])
		else
			NumPSounds++

		P_Sounds[n] = PlaySound(this, GetSoundEx(name, 1, lf), pf)

		if(pf & SF_ONCE)
			SetEventMask(this, EV_SOUNDEND)
	}
/*
	void SetFade(float fade)
	{
		TFactor[3] = fade
	}

	void FadeHandler(int FadeEvent)
	{
	}

	void TDynamicFade(local int FadeTime, local float FTarget)
	{
		local int n
		local float FadeRate

		//SetRenderMode(this, TSWITCH_TRANSP)
		SetSortBias(this, 1)

		if(FTarget == 1)	//FadeIn
		{
			SetFlags(this, TFL_VISIBLE)
			FadeHandler(FADEIN_START)
		}
		else
			FadeHandler(FADEOUT_START)

		if(FadeTime == 0)
			FadeTime = 1

		for(n = 0; n < FadeTime; n += 5)
		{
			FadeRate = (float)n / (float)FadeTime

			if(!FTarget)
				FadeRate = 1 - FadeRate

			SetFade(FadeRate)
			Sleep(5)
		}

		if(FTarget == 1)
		{
			SetRenderMode(this, 0)
			SetSortBias(this, 0)
			FadeHandler(FADEIN_END)
		}
		else
		{
			ClearFlags(this, TFL_VISIBLE)
			FadeHandler(FADEOUT_END)
		}

		SetFade(FTarget)
	}

	void M_FadeIn(ExtClass other)
	{
		thread TDynamicFade(atoi(other._value), 1)
	}

	void M_FadeOut(ExtClass other)
	{
		thread TDynamicFade(atoi(other._value), 0)
	}
*/
//-----------------------------------------------------------------
	void M_Show(ExtClass other)
	{
		Show(true)
	}
//-----------------------------------------------------------------
	void M_Hide(ExtClass other)
	{
		Show(false)
	}

//-----------------------------------------------------------------
//virtualni fce, jen pro CreatureClass
	void PickItem(int item, bool RealyPick)
	{
	}

	void AddItem(MinClass it)
	{
	}

//-----------------------------------------------------------------
	void Activate(int fl, ExtClass other)		//aktivacia sa podarila. hrac stlacil use. zavola sa predtym nez sa zacne volat ActivateProcess()
	{
	}

	void ActivateFail(int fl, ExtClass other)	//aktivacia sa nepodarila. na vypis hlasok co k tomu potrebujes atd.
	{
	}

//-----------------------------------------------------------------
	void ActivateProcess(int fl, ExtClass other)	//pokial CanActivate() vracia ACT_ENABLE a hrac drzi USE tak je to volane kazdy frame
	{
	}

//-----------------------------------------------------------------
	void ActivateProcessEnd()	//skoncilo volanie ActivateProcess()
	{
	}

//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(Display && Display.CanActivate(0, this) == ACT_ENABLE)
			return ACT_ENABLE

		return ACT_DISABLE
	}

//-----------------------------------------------------------------
	int IsShot(vector rayorigin, vector raydirection)
	{
		return false
	}

//-----------------------------------------------------------------
	int Shoot(ShotStorage shot)
	{
		return true
	}

//-----------------------------------------------------------------
	void EOnSoundEnd(MinClass other, int extra)
	{
		if(!NumPSounds)
			return

		for(int n = 0; n < sizeof(P_Sounds); n++)
		{
			if(P_Sounds[n] == extra)
			{
				P_Sounds[n] = 0
				NumPSounds--
				return
			}
		}
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		if(Display)
			Throw(Display, other, EV_VISIBLE, extra)

		return 0
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra);
	void EOnInit(MinClass other, int extra);

//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{
		if(_name)
		{
			AddEntityToHashTable(this, _name)
		}
	}
	
//-----------------------------------------------------------------
	void ExtClass()
	{
		TFactor[0] = 1
		TFactor[1] = 1
		TFactor[2] = 1
		TFactor[3] = 1
		SetEventMask(this, EV_LOADED|EV_ONLOAD)

		if(_name)
		{
			AddEntityToHashTable(this, _name)
		}
	}

//-----------------------------------------------------------------
	void SetName(string name)
	{
		if(this == PrevFind)
			PrevFind = DefFind

		if(_name)
		{
			RemoveEntityFromHashTable(this, _name)
		}
		
		_name = name
		
		if(name)
		{
			AddEntityToHashTable(this, _name)
		}
	}
	
//-----------------------------------------------------------------
	void ~ExtClass()
	{
		OnRemove()	//Informuje entity na tomto zavisle ze sa maze

		if(HackInterf)
		{
			delete HackInterf
			HackInterf = NULL
		}

		if(agent)
			agent.Destroy()

		if(WPosition)
			delete WPosition

		//pokud mazes cachovanou entitu, tak premaz pointer
		if(this == PrevFind)
			PrevFind = DefFind

		if(_name)
			RemoveEntityFromHashTable(this, _name)

		if(Display)
		{
			delete Display
			Display = NULL
		}

#ifdef EDITOR
		ExtClass child
		int n

		for(n = 0; n < ChildsNamesNum; n++)
		{
			child = FindClass(ChildsNames[n])
			
			if(child)
			{
				child.ParentName = ""
				UpdateEntity(child)
			}
		}

		DestroyEditorVisuals()
#endif
	}
}

//----------------------------------------------------------------
void ShotStorage::Shoot(MinClass other)
{
	ExtClass oth = other

	if(IsInherited(oth, CExtClass) == false)
		return SR_NONE

	return oth.Shoot(this)
}

//=================================================================================
ExtClass FindClass(string name)
{
	if(!name)
		return NULL

	ExtClass ec = PrevFind

	//optimalizace - pokud hledame opakovane stejnou entitu, tak nehledej znova
	//je to bezpecne, i kdyz se entity mazou
	if(ec._name == name)
		return ec

	ec = FindEntityInHashTable(name)

	if(ec)
		PrevFind = ec

	return ec
}
//=================================================================================
int CallStackPos

//=================================================================================
string CallUserEvents(ExtClass ec, string method)
{
	if(ec._userevents)
	{
		string events = ec._userevents
		int n, p, c

		p = 0

		while(events)
		{
			string ev

			c = strstr(events, ",")
			if(c != -1)
			{
				ev = trim(substr(events, 0, c))
				events = substr(events, c + 1, strlen(events) - c - 1)
			}
			else
			{
				ev = trim(events)
				events = ""
			}

			if(ev == method)
				goto found

			p++
		}
	}

	return ""

found:;
	switch(p)
	{
	case 0:
		return ec._userevent0
	case 1:
		return ec._userevent1
	case 2:
		return ec._userevent2
	case 3:
		return ec._userevent3
	}
}

//=================================================================================
int ParseParm(string str, int p)
{
	int len = strlen(str)
	int count = 1
	p++

	while(p < len && count > 0)
	{
		string char = substr(str, p, 1)
		if(char == "(")
			count++
		else
		{
			if(char == ")")
			{
				count--

				if(count == 0)
					return p
			}
		}
		p++
	}

	return -1
}

//=================================================================================
#ifdef DEVELOPER
string g_sDebugStack[32]
int g_iDebugStackBottom, g_iDebugStackTop

void FSMDebugger(ExtClass owner, string ename, string mname, bool show)
{
	string text = itoa(CallStackPos) + " : " + owner._name + " -> " + ename + "." + mname

	if(owner._value)
		text += "(" + owner._value + ")"

	g_sDebugStack[g_iDebugStackTop] = text

	g_iDebugStackTop = g_iDebugStackTop + 1 & 31
	if(g_iDebugStackTop == g_iDebugStackBottom)
		g_iDebugStackBottom = g_iDebugStackBottom + 1 & 31

	if(show)
	{
		string list[32]
		int cnt = 0;
	
		for(int n = g_iDebugStackBottom; n != g_iDebugStackTop; )
		{
			list[cnt++] = g_sDebugStack[n]
			n = n + 1 & 31
		}
	
		if(cnt)
		{
			ShowCursor(true)
			SetMousePos(30, 410)
	
			if(ListBox(list, cnt, cnt - 1, "FSM debugger") == -1)
				g_bDebugFSM = 0
			ShowCursor(false)
		}
	}
}
#endif

//=================================================================================
void _CallMethods(local ExtClass owner, local string methods)
{
	local string ename = "this"
	local ExtClass ec = owner
	local int p
	local bool skipping = false
	string token, token2

	while(true)
	{
		int type = ParseStringEx(methods, token)

		//end of string
		if(type == 0)
			break

		switch(token)
		{		
		//state condition
		case "#":
			ParseStringEx(methods, token2)
			ParseStringEx(methods, token)

			if(token != ":")
			{
				DPrint("Syntax error in state declaration'" + methods + "' on entity '" + owner._name + "'")
				ErrorNum++
			}
			if(token2 == "end")
				continue

			//skip whole state
			if(token2 != owner._state || skipping)
			{
				p = strstr(methods, "#")

				if(p == -1)
					return //there are not other states

				methods = substr(methods, p, strlen(methods) - p)
			}
			continue

		case "break":
			skipping = true
			break

		case "WaitFor":
			ParseStringEx(methods, token)
			ParseStringEx(methods, token2)
			ParseStringEx(methods, token)
			
			if(token != ")")
			{
				DPrint("Syntax error in WaitFor '" + methods + "' on entity '" + owner._name + "'")
				ErrorNum++
				return
			}

			if(token2 == "this")
				ec = owner
			else
			{
				if(token2 == "disp")
					ec = owner.Display
				else
					ec = FindClass(token2)
			}

			if(ec == NULL)
			{
				DPrint("Entity '" + token2 + "' does not exists in WaitFor on '" + owner._name + "'")
				ErrorNum++
			}
			else
				Wait(ec.ActionNum == 0)
			break

		case "debug":
#ifdef DEVELOPER
			g_bDebugFSM = 1
#endif
			break

		case "Sleep":
			ParseStringEx(methods, token)

			if(ParseStringEx(methods, ename) != 4)
			{
				DPrint("No number in Sleep '" + ename + "' on entity '" + owner._name + "'")
				ErrorNum++
				return
			}
			
			ParseStringEx(methods, token)

			if(token != ")")
			{
				DPrint("Syntax error in Sleep '" + methods + "' on entity '" + owner._name + "'")
				ErrorNum++
				return
			}

			Sleep(atoi(ename))
			break

		case "Print":
			ParseStringEx(methods, token)

			if(token != "(")
			{
				DPrint("Syntax error Print '" + token + "' on entity '" + owner._name + "'")
				ErrorNum++
				return
			}

			p = strstr(methods, ")")
			if(p == -1)
			{
				DPrint("Syntax error Print '" + methods + "' on entity '" + owner._name + "'")
				ErrorNum++
				return
			}
			
#ifdef DEBUG_PRINT
			token2 = substr(methods, 0, p)
			methods = substr(methods, p + 1, strlen(methods) - p - 1)
			
			ExtClass ptext
			if(token2 == "this")
				ptext = owner
			else
			 	ptext = FindClass(token2)

			if(ptext)
				Print(String(owner._name + ".Print Entity:" + ptext._name + " State:" + ptext._state))
			else
				Print(String(owner._name + ".Print " + token2))
#else
			//just skip
			methods = substr(methods, p + 1, strlen(methods) - p - 1)
#endif
			break

		default:
			if(token != ".")
			{
				ename = token
				
				ParseStringEx(methods, token)

				if(token != ".")
				{
					DPrint("Syntax error in method '" + methods + "' on entity '" + owner._name + "'")
					ErrorNum++
					return
				}

				ParseStringEx(methods, token2)

				if(ename == "this")
					ec = owner
				else
					ec = FindClass(ename)
			}
			else
			{
				ParseStringEx(methods, token2)
			}

			//parametry
			if(strlen(methods) && substr(methods, 0, 1) == "(")
			{
				p = 1
				int len = strlen(methods)
				int count = 1
	
				while(p < len)
				{
					string char = substr(methods, p, 1)
					if(char == "(")
						count++
					else
					{
						if(char == ")")
						{
							count--
			
							if(count == 0)
								goto done
						}
					}
					p++
				}

				DPrint("Syntax error in method '" + token2 + "' on entity '" + owner._name + "'")
				ErrorNum++
				continue
	done:;
				owner._value = trim(substr(methods, 1, p - 1))
				methods = substr(methods, p + 1, strlen(methods) - p - 1)
			}
			else
				owner._value = ""

			if(ec == NULL)
			{
				DPrint("Entity '" + ename + "' does not exists in method call on '" + owner._name + "'")
				ErrorNum++
			}
			else
			{
				if(CallStackPos >= 31)
				{
					DPrint("Stack overflow!")
					ErrorNum++
#ifdef DEVELOPER
					FSMDebugger(owner, ename, token2, true)
#endif
					return
				}
	
				CallStackPos++
	
#ifdef DEVELOPER
				FSMDebugger(owner, ename, token2, g_bDebugFSM)
#endif

				if(!Call(ec, "M_" + token2, owner))
				{
					string ev = CallUserEvents(ec, token2)

					if(!ev)
					{
						DPrint("Entity " + ename + " (" + ClassName(ec) + ") have no method '" + token2 + "'")
						ErrorNum++
					}
					else
						thread _CallMethods(ec, ev)
				}
				CallStackPos--
			}
		}

		//should be ';'
		if(ParseStringEx(methods, token) == 0)
		{
			//valid end
			return
		}

		if(token != ";")
		{
			DPrint("Command is terminated by '" + token + "' in method call on '" + owner._name + "'")
			ErrorNum++
		}
	}
}

//-----------------------------------------------------------------
void CallMethods(ExtClass owner,string methods)
{
	if(methods != "")
		thread _CallMethods(owner, methods)
}

//=================================================================================
class LightClass extends ExtClass
{
	vector   coords
	vector   _color
	int      style
	int      intensity
	int      radius
	int      corona
	int      glow
	HLIGHT   LightHandle
	int		effect_parm

//-----------------------------------------------------------------
	void Switch(int onoff)
	{
	}

//-----------------------------------------------------------------
	void Init(int clnum, int flags)
	{
		float f = _color * _color
		if( f == 0)
		  _color = "255 255 255"

		if( intensity == 0)
			intensity = 300

		if( radius == 0)
			radius = intensity

		ClassType = ClassTypeLight
		ClassNum = clnum
//		SetOrigin(this, coords)

		LightHandle = AddLight(this, style, flags, intensity, radius, _color)
	}
}

bool g_MenuFlash


//=================================================================================
class SpeechManager extends _serializable
{
	CreatureClass Speakers[ST_SIZEOF][SPEAKERS_COUNT]
	float SpeakTime[ST_SIZEOF][SPEAKERS_COUNT]
	bool Emit[ST_SIZEOF][SPEAKERS_COUNT]
	float Dist[ST_SIZEOF][SPEAKERS_COUNT]

	bool Say(CreatureClass owner, int type);
	void Update();
}

/*
speaker.Clear()
speaker.AddDialog(livie, SayID4, 100)
speaker.AddDialog(livie, SayID5, 100)
speaker.AddDialog(livie, SayID6, 100)
speaker.Play()
speaker.PlayIngame()
speaker.Stop()
*/

//-----------------------------------------------------------------
class IngameDialog extends _serializable
{
	string			ID
	string			WhoSay
	float				OverTime
	bool				transmition
	IngameDialog	next
}

const int SP_NONE = 0
const int SP_MINICUTSCENE = 1
const int SP_INGAME = 2

//=================================================================================
class SpeakTransmitter extends ExtClass
{
	DialogPanel DlgPanel
	SubtitlesDialog Subtitles
	IngameDialog FirstDialog, LastDialog, PlayingDialog
	int InGameDialogSound
	float DialogOverTime
	int PlayMode
	ExtClass SoundPos

//-----------------------------------------------------------------
	void Show(bool show)
	{
		DlgPanel.Show(show)
	}
	
//-----------------------------------------------------------------
	void StopInGameDialogSound()
	{
		if(InGameDialogSound)
		{
			EndSound(InGameDialogSound)	
			InGameDialogSound = 0
		}
	}

//-----------------------------------------------------------------
	bool PlayInGameDialogSound(MinClass owner, string sample, int flags)
	{
		StopInGameDialogSound()
		hsound snd = GetSoundEx(sample, 0, flags)

		if(!snd)
		{
			DPrint("Can't found dialog sample " + sample)
			ErrorNum++
			return false
		}

		InGameDialogSound = PlaySound(owner, snd, flags)
		return true
	}

//-----------------------------------------------------------------
	bool PlayDialog(IngameDialog dialog, int mode)
	{
		int SoundFlags = SF_ONCE
		int IconIndex = GetSayCharacterImageIndex(dialog.WhoSay)

		if(IconIndex == -1)	//nebolo zadane meno toho kto hovori ale nazov entity z ktorej ma hrat zvuk
		{
			SoundPos = FindClass(dialog.WhoSay)

			if(!SoundPos)
			{
				DPrint(String("speaker: Bad WhoSay parameter in dialog ") + dialog.ID)
				ErrorNum++
				return false
			}
			
			SoundFlags |= SF_3D
			SetOrigin(this, GetMatrix(SoundPos, 3))
		}
		else
		{
			SoundFlags |= SF_AMBIENT
			SoundPos = NULL
		}

		if(dialog.transmition)
		{
			DlgPanel.SetDialogText("#Transmition_Received", dialog.WhoSay) 
			DlgPanel.Open()
		}
		else
			DlgPanel.Close()	//keby bol nahodou otvoreny predchadzajucim dialogom

		if(dialog.WhoSay != "tutorial")	//tutorialove hlasky su bez zvukov
			PlayInGameDialogSound(this, dialog.ID, SoundFlags)

		Subtitles.SetText("#" + dialog.ID)
		Subtitles.Show(true)
		DialogOverTime = dialog.OverTime
		PlayMode	= mode
		return true
	}

//-----------------------------------------------------------------
	void AddDialog(IngameDialog dialog)
	{
		if(!FirstDialog)
		{
			FirstDialog = dialog
			LastDialog = dialog
		}
		else
		{
			if(!LastDialog)
			{
				LastDialog = FirstDialog
				IngameDialog CurrentDialog = FirstDialog
		
				while(CurrentDialog)
				{
					LastDialog = CurrentDialog
					CurrentDialog = CurrentDialog.next
				}
			}
	
			LastDialog.next = dialog
			LastDialog = dialog
		}
	}

//----------------------------------------------------------------
	void ClearDialogs()
	{
		LastDialog = FirstDialog
		IngameDialog CurrentDialog = FirstDialog

		while(CurrentDialog)
		{
			LastDialog = CurrentDialog
			CurrentDialog = CurrentDialog.next
			delete LastDialog
			LastDialog = NULL
		}
		
		FirstDialog = NULL
		LastDialog = NULL
		PlayingDialog = NULL
	}
	
//-----------------------------------------------------------------
	IngameDialog GetNextDialog()
	{
		if(!PlayingDialog)
			PlayingDialog = FirstDialog
		else
			PlayingDialog = PlayingDialog.next

		return PlayingDialog
	}
	
//----------------------------------------------------------------
	void Stop()
	{
		DlgPanel.Close()
		Subtitles.Show(false)
		StopInGameDialogSound()
		ClearDialogs()
		PlayMode	= SP_NONE
		PlayingDialog = NULL
		ActionNum = 0
	}

//----------------------------------------------------------------
	bool Play(int mode)	//0=zastavi hru 1=hraje ingame bez prerusenia hry
	{
		if(PlayMode != SP_NONE)
			return false

		IngameDialog ToPlay = GetNextDialog()

		if(!ToPlay)
			return false

		if(!PlayDialog(ToPlay, mode))
			return false
	
		ActionNum = 1		//aby fungovalo WaitFor()
		return true
	}

//-----------------------------------------------------------------
	void OnFrame()	//preposiela world
	{
		if(PlayMode && PlayingDialog)	//ak je vobec zobrazeny nejaky dialog
		{
			if(SoundPos)
				SetOrigin(this, GetMatrix(SoundPos, 3))
				
			bool NeedClose = false

			if(PlayingDialog.transmition)	//vysielacka
			{
				if(!InGameDialogSound && DlgPanel.status == 2 && DialogOverTime >= 0)
					NeedClose = true
			}
			else
			{
				if(!InGameDialogSound && DialogOverTime >= 0)	//DialogOverTime = -1 = ma zostat otvoreny
					NeedClose = true				
			}
			
			if(NeedClose)
			{
				DialogOverTime -= ftime * 100
				
				if(DialogOverTime <= 0)
				{
					DialogOverTime = 0
					IngameDialog NextToPlay = GetNextDialog()
			
					if(NextToPlay && NextToPlay.ID == "ScriptCall")	//moze tam byt aj dialog ktory slusi iba na zavolanie niecoho do skriptu
					{
						CallMethods(this, NextToPlay.WhoSay)
						NextToPlay = GetNextDialog()
					}

					if(!NextToPlay || !PlayDialog(NextToPlay, PlayMode))
						Stop()
				}
			}
		}
		DlgPanel.OnFrame()	
	}

//-----------------------------------------------------------------
	void BeginCutscene()
	{
		Stop()
	}

//-----------------------------------------------------------------
	void AddDialogFromGame(string ParmsString, bool transmition)
	{
		string Parms[8]
		int NumParms = ParseString(ParmsString, Parms)

		if(NumParms < 3 || NumParms > 5)
		{
			DPrint("Say:wrong num of parameters " + _name + ".Say")
			ErrorNum++
			return
		}

		IngameDialog dialog = new IngameDialog
		dialog.ID = Parms[0]
		dialog.WhoSay = Parms[2]
		dialog.OverTime = 50		//defaultne zostava viditelny pol sekundy po dohrani samplu		
		dialog.transmition = transmition
		
		if(NumParms >= 5)
			dialog.OverTime = atof(Parms[4])

		AddDialog(dialog)		
	}

//-----------------------------------------------------------------
	void M_AddDialog(ExtClass other)	//speaker.AddDialog(text, KtoHovori, cas)
	{
		AddDialogFromGame(other._value, false)
	}

//-----------------------------------------------------------------
	void M_AddTransDialog(ExtClass other)	//speaker.AddTransDialog(text, KtoHovori, cas)
	{
		AddDialogFromGame(other._value, true)
	}

//---------------------------------------------------------------------------------
	void M_AddCall(ExtClass other)
	{
		IngameDialog dialog = new IngameDialog
		dialog.ID = "ScriptCall"
		dialog.WhoSay = other._value
		dialog.OverTime = 0
		dialog.transmition = false
		AddDialog(dialog)
	}

//-----------------------------------------------------------------
	void M_Say(ExtClass other)	//speaker.Say(text, KtoHovori, cas)
	{
		Stop()
		AddDialogFromGame(other._value, false)
		
		if(!FirstDialog)
		{
			DPrint(String("speaker: no dialogs to playing. caller is ") + other._name)
			ErrorNum++
			return
		}
		
		if(!Play(SP_INGAME))
			return
	}

//-----------------------------------------------------------------
	void M_SayTrans(ExtClass other)	//speaker.Say(text, KtoHovori, cas)
	{
		Stop()
		AddDialogFromGame(other._value, true)
		
		if(!FirstDialog)
		{
			DPrint(String("speaker: no dialogs to playing. caller is ") + other._name)
			ErrorNum++
			return
		}
		
		if(!Play(SP_INGAME))
			return
	}
	
//-----------------------------------------------------------------
	void M_Play(ExtClass other)	//speaker.Play(text, KtoHovori, cas)
	{
		if(!FirstDialog)
		{
			DPrint(String("speaker: no dialogs to playing. caller is ") + other._name)
			ErrorNum++
			return
		}

//		if(!Play(SP_MINICUTSCENE))	//spusti to player
//			return

		ExtClass player = FindClass("player")
		player.Activate(2, this)		//player spusti minicutscenu a tieto dialogy okamzite jak bude na zemi a odlozi zbran
		ActionNum = 1						//musi byt aj tu pretoze sa tu nevola play a az player ho zavola s malym spozdenim
	}

//-----------------------------------------------------------------
	void M_PlayIngame(ExtClass other)	//speaker.PlayIngame(text, KtoHovori, cas)
	{
		if(!FirstDialog)
		{
			DPrint(String("speaker: no dialogs to playing. caller is ") + other._name)
			ErrorNum++
			return
		}
		
		if(!Play(SP_INGAME))
			return
	}

//---------------------------------------------------------------------------------
	void M_Stop(ExtClass other)
	{
		Stop()
	}

//----------------------------------------------------------------
	void M_Clear(ExtClass other)
	{
		ClearDialogs()
		PlayMode	= SP_NONE
	}

//-----------------------------------------------------------------
	void EOnSoundEnd(MinClass other, int extra)
	{
		if(extra == InGameDialogSound)
		{
			InGameDialogSound = 0
			return
		}
	}

//-----------------------------------------------------------------
	void EOnLoad(ExtClass other, int extra)
	{
		InGameDialogSound = 0	//ak nieco pred sejvom hralo tak simulujeme ze zvuk dohral nech sa pusti nasledujuci dialog
	}

//---------------------------------------------------------------------------------
	void SpeakTransmitter()
	{
		SetName("speaker")
		DlgPanel = new DialogPanel(17, -100)
		DlgPanel.SetPos(17, -DlgPanel.GetHeight())
		Subtitles = new SubtitlesDialog(0, 418, 800, 128)
		SetEventMask(this, EV_SOUNDEND|EV_ONLOAD)
	}

//---------------------------------------------------------------------------------
	void ~SpeakTransmitter()
	{
		ClearDialogs()

		if(DlgPanel)
		{
			delete DlgPanel
			DlgPanel = NULL
		}
		
		if(Subtitles)
		{
			delete Subtitles
			Subtitles = NULL	
		}
	}
}

class SunFlare extends ExtClass
{
	vobject MainObject
	string model
	bool IsVisible

	void Switch(bool stat)
	{
		if(stat)
			SetFlags(this, TFL_VISIBLE)
		else
			ClearFlags(this, TFL_VISIBLE)

		IsVisible = stat
	}

	void SetAlpha(float targetval)
	{
		float spd = ftime * 4

		if(TFactor[3] > targetval)
		{
			TFactor[3] = TFactor[3] - spd

			if(TFactor[3] < targetval)
			{
				TFactor[3] = targetval

				if(IsVisible && targetval == 0)
					Switch(false)
			}
		}

		if(TFactor[3] < targetval)
		{
			TFactor[3] = TFactor[3] + spd

			if(TFactor[3] > targetval)
				TFactor[3] = targetval
		}

//		g_vSunColor
//		g_fSunIntensity
	}

	bool SunTracFilter(ExtClass other)
	{
		if(other.ClassType == ClassTypeCreature && other.ClassNum == ClassNumPlayer)
			return false

		return true
	}

	void Update();

	void SunFlare(int type)
	{
		if(type == 1)
			model = "spr/effects/flares/lens0.spr"

		if(type == 2)
			model = "spr/effects/flares/lens1.spr"

		MainObject = GetObject(model)
		SelectObject(this, MainObject)
	}

	void ~SunFlare()
	{
		if(MainObject)
			ReleaseObject(MainObject, 0)
	}
}

//=================================================================================
const int ShellTypes = 4
const int ShellBufferSize = 16

class music_player;

class WorldClass extends ExtClass
{
	_eclass	ViewerClass
#ifdef EDITOR
	bool		WorkbenchActive
	bool		EditorActive
	MinClass Editor
	int		PrefabsIncrement
	string	ThisLevelName
	string	ThisLevelDir
#endif

	string name			//pracovni nazev levlu kteremu odpovida oficialni alias v strings.h
	CreatureClass	Player
	MinClass			ManipEntity
	float				BulletTimeFade
	
	music_player	MusicPlayer
	ItemCollection ItemColPtr
	DisplayCollection DispColPtr
	func_display	TurretDisplay1
	func_display	CameraDisplay
	func_display	EnemyDisplay
	func_display	VehicleLoaderDisplay
	func_display	MineDisplay
	func_display	ScreenDisplay
	TasklistDialog	TaskList
	InfoDialog		InfDialog
	HackInfoDialog	HackInfDialog
	CrossHairClass Cross
	SpeakTransmitter Transmitter
	
	vector			CameraPos
	vector			CameraAngs
	vector			CameraNorm
            		
	int				NumTurrets1
	int				NumMines
	int				NumVehicleLoaders
	int				NumVehicleBuggys
	int				NumCameras
	int				NumDronEnemies
	int				NumSpiderEnemies
	int				NumFlyingItems

	bool				CutScene
	bool				CutSceneSkip
	bool				PlayerControl
	bool				ShowCursor
	int				ActualFrame

	int				ShootID //counter unikatniho ID vystrelu
									//slouzi pro AI, by odfiltrovalo vice zasahu napr. brokovnici
	
	float		viewdist
//	float		playerscale, enemyscale

	int DlgSound[4]
	string DlgID[4]
	int NumDlgSound
	int NumAnswers, CurrentAnswer
	bool QuestionActive// = false

	float pfov
	float InventoryScale		//otvoreny inventory = 1, zavreny = 0

	float FlashTime
	float FlashCount
	float FlashSpeed1, FlashSpeed2
	vector FlashColor
	bool	FlashNeedSkip
//	string FlSpriteName

	PathFinder pPathFinder

	string OnStart

	SpeechManager SManager

	ExtClass Cameras[32]
	int CameraIndex
	
	//circular buffer for shells
	MinClass		Shells[ShellTypes][ShellBufferSize]
	int			ShellTop[ShellTypes]
	vobject		ShellObjects[ShellTypes]
	
	vobject		Streaks[8][32]
	int			NumStreaks[8]
	
	SunFlare flare
	int flaretype

//-----------------------------------------------------------------
	void OnCutSceneSkip()
	{
		if(FlashCount > 0)
			FlashNeedSkip = true
	}

//-----------------------------------------------------------------
	void ThrowShell(MinClass pivot, MinClass owner, int type, float speed, bool sound);

//------------------------------------------------------------
	void Quake(float TimeMax, float Power, float Speed, float FadeInTime, float FadeOutTime, int Priority)
	{
	}

	void ApplyExplosion(MinClass actor, vector pos, float radius, float power, float damage, int type)
	{
	}
	
bool DialogVisible
float DialogTimer

//-----------------------------------------------------------------
	void CloseCutSceneDialog()
	{
		ShowWidget(WDialogText,false)
		DialogVisible = false
	}

//-----------------------------------------------------------------
	int Dialog(MinClass owner, string id, int flags)
	{
		for(int n = 0; n < 4; n++)
			SetWidgetText(WDialogText, n, "")

		SetWidgetTextEx(WDialogText, 0, 1, "#" + id)

		hsound snd = GetSoundEx(id, 0, SF_STREAM|SF_NOERROR)

		if(!snd)
		{
			ShowWidget(WDialogText,true)
			DialogVisible = true
			DialogTimer = 3.0
		
			//DPrint("Can't found dialog sample " + id)
			//ErrorNum++
			return 0
		}

		ShowWidget(WDialogText, g_Subtitles)
		DialogVisible = g_Subtitles
		DialogTimer = 1.5

		int hsnd = PlaySound(owner, snd, flags | SF_ONCE)

		DlgID[NumDlgSound] = id
		DlgSound[NumDlgSound++] = hsnd
		NumDlgSound &= 3
		return hsnd
	}

//-----------------------------------------------------------------
	bool IsDialogActive(string dlg)
	{
		for(int n = 0; n < sizeof(DlgID); n++)
		{
			if(DlgID[n] == dlg)
				return true
		}

		return false
	}

//-----------------------------------------------------------------
	void StopDialogs()
	{
		for(int n = 0; n < sizeof(DlgSound); n++)
		{
			if(DlgSound[n])
			{
				EndSound(DlgSound[n])
				DlgSound[n] = 0
			}
		}
	}

//------------------------------------------------------------
	void ProcessFlash()
	{
		if(FlashCount <= 0.5)
			FlashCount += FlashSpeed1 * ftime
		else
			FlashCount += FlashSpeed2 * ftime

		if(FlashCount >= FlashTime)
			FlashCount = FlashTime

		float	FlashRate = FlashCount / 1//FlashTime
		float ToAng = FlashRate * 180

		float res = sin(ToAng * DEG2RAD)
		if(res < 0)
			res = -res

		if(FlashCount >= FlashTime || FlashNeedSkip)
		{
			if(FlashTime == 1 || FlashNeedSkip)
			{
				FlashTime = 0
				res = 0
				ShowWidget(WFlash, false)
				FlashNeedSkip = false
			}
		}

		float a = res * 255
		float r = FlashColor[0]
		float g = FlashColor[1]
		float b = FlashColor[2]
		SetWidgetColor(WFlash, ARGB(a, r, g, b))
	}

//-----------------------------------------------------------------
	void FullscreenFlash(float speed1, float speed2, vector color, float StartTime, float EndTime)
	{
		SetWidgetColor(WFlash, 0x00ffffff)
		ShowWidget(WFlash, true)
		FlashTime = EndTime
		FlashSpeed1 = speed1 * 0.01
		FlashSpeed2 = speed2 * 0.01
		FlashCount = StartTime
		FlashColor = color
	}

//-----------------------------------------------------------------
	void M_Flash(ExtClass other)
	{
		string ParmsAll = ReplaceChars(other._value, ",", " ")
		string Parms[8]
		int NumParms = ParseString(ParmsAll, Parms)

		if(NumParms != 2)
		{
			DPrint("Flash:wrong num of parameters " + _name + ".Flash")
			ErrorNum++
			return
		}

		float speed1 = atof(Parms[0])
		float speed2 = atof(Parms[1])
		vector color = "255 255 255"

		FullscreenFlash(speed1, speed2, color, 0, 1)
	}

//-----------------------------------------------------------------
	void M_ShowScreenDisplay(ExtClass other);
//-----------------------------------------------------------------
	void M_HideScreenDisplay();

bool FirstFrame
//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(!FirstFrame)
		{
			FirstFrame = true
			CallMethods(this, OnStart)
		}
		
		ActualFrame = extra
		g_FPS = 1 / ftime

		if(g_Viewer)
			return

		Transmitter.OnFrame()
		TaskList.OnFrame()
		InfDialog.OnFrame()

		if(DialogVisible)
		{
			if(DlgSound[0] == 0 && DlgSound[1] == 0 && DlgSound[2] == 0 && DlgSound[3] == 0)
			{
				DialogTimer -= ftime
				if(DialogTimer <= 0)
				{
					DialogTimer = 0
					DialogVisible = false
					ShowWidget(WDialogText, false)
				}
			}
		}

/*		if(g_MenuFlash)
		{
			g_MenuFlash = false
			FullscreenFlash(1600, 60, "0.2 0.2 0.3", 0, 1)
		}*/
		
		if(FlashTime > 0)	//je mozne ze sprite je viditelny
			ProcessFlash()

		SManager.Update()
	}

//-----------------------------------------------------------------
	void EOnSoundEnd(MinClass other, int extra)
	{
		for(int n = 0; n < 4; n++)
		{
			if(DlgSound[n] == extra)
			{
				DlgSound[n] = 0
				DlgID[n] = ""
			}
		}
	}

#ifdef DEVELOPER
//-----------------------------------------------------------------
	void M_DebPrint(ExtClass other)
	{
		InfDialog.ShowMessage(other._value)
	}
#endif

//---------------------------------------------------------------------------------
	void LoadNAV(string name, bool free)
	{
	}

	void SetCurrent();

//-----------------------------------------------------------------
	void EOnLoad(MinClass other, int extra)
	{
		SetCurrent()
		DefFind = this
		PrevFind = this
		CallStackPos = 0
	}

	int ErrorFrame
	int ErrorLine
	widget ErrorWidget
	bool nopvsreported
//-----------------------------------------------------------------
	void CreateErrorWidget()
	{
		if(!ErrorWidget)
		{
			ErrorWidget = CreateWidget(WTYPE_TEXT, 150, 20, 500, 200, WF_BLEND|WF_VISIBLE, 0x7f7f7f7f, 300)
			LoadWidgetImage(ErrorWidget, 0, "gfx/black.tga")
			SetWidgetTextColor(ErrorWidget, 0xffffffff)
		}
	}

//-----------------------------------------------------------------
	void EOnWorldProcess(MinClass other, int extra)
	{
		SetCurrent()
		DefFind = this
		PrevFind = this

		currenttime = GetWorldTime() * 0.001
		//viewdist = 512
		SetCameraFarPlane(0, viewdist)
		//g_fVisDist = 512
		

		bool test = false
		
#ifdef DEVELOPER
 		test = true
#endif

#ifndef EDITOR
		if(test && ViewerClass == NULL)
		{
			if(ErrorNum > 0)
			{
				//let this crap run for a while
				ErrorFrame++
				if(ErrorFrame == 5)
				{
					ErrorNum = 0

					//if(ComputerName != "fido")
					//{
						CreateErrorWidget()
						SetWidgetText(ErrorWidget, ErrorLine++, "Level obsahuje vazne chyby ve skriptovani, ci definici entit.")
						SetWidgetText(ErrorWidget, ErrorLine++, "Funkcnost bude pravdepodobne omezena, ci zcela chybna")
						SetWidgetText(ErrorWidget, ErrorLine++, "Reportujte na Mantis jako 'Level design - skript' problem")
						SetWidgetText(ErrorWidget, ErrorLine++, "a pripojte obsah souboru error.log")
						SetWidgetText(ErrorWidget, ErrorLine++, "")
					//}
				}
			}

			if(d_nopvs && !nopvsreported)
			{
				nopvsreported = true
				CreateErrorWidget()
				SetWidgetText(ErrorWidget, ErrorLine++, "Level nema zkompilovanou viditelnost a neni v provozuschopnem stavu.")
				SetWidgetText(ErrorWidget, ErrorLine++, "Reportujte na Mantis jako 'Level design - geometrie' problem.")
				SetWidgetText(ErrorWidget, ErrorLine++, "")
			}
		}
#endif
	}

//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		dBodyInteractionLayer(this, ET_WORLD)
	}

//-----------------------------------------------------------------
// Definition of SpeechSet. Must be here because of serialization process
//-----------------------------------------------------------------
SpeechSet SpeechSets[32]
int SpeechSetCount
int CSType

//-----------------------------------------------------------------
void BeginSpeechSet(string ss)
{
	SpeechSetCount++
	SpeechSets[SpeechSetCount] = new SpeechSet(ss)
}

//-----------------------------------------------------------------
void SpeechType(int type)
{
	CSType = type
}

//-----------------------------------------------------------------
void Speech(string sample)
{
	SpeechSet ss = SpeechSets[SpeechSetCount]
	
	ss.SampleNames[CSType][ss.Counts[CSType]] = sample
	ss.Counts[CSType] = ss.Counts[CSType] + 1
}

void InitSpeechSet();

//-----------------------------------------------------------------
// Definition of animsets. Must be here because of serialization process
//-----------------------------------------------------------------
AnimSet	AnimSets[MAX_ANIMSETS]
int 		NumAnimSets

//-----------------------------------------------------------------
void Import(string aset)
{
	AnimSet set
	
	for(int s = 0; s < NumAnimSets; s++)
	{
		set = AnimSets[s]
		
		if(set.Name == aset)
			break
	}
	
	if(s == NumAnimSets)
	{
		DPrint("Can't found animset " + aset)
		ErrorNum++
		return
	}
	
	for(int n = 0; n < MAX_ANIMSLOTS; n++)
	{
		if(!CurrentAnimSet.AnimNames[n])
		{
			CurrentAnimSet.AnimNames[n] = set.AnimNames[n]
			CurrentAnimSet.FPS[n] = set.FPS[n]
		}
	}
}

//-----------------------------------------------------------------
void BeginAnimSet(string name)
{
	CurrentAnimSet = new AnimSet(name)
	AnimSets[NumAnimSets++] = CurrentAnimSet
	CurrentAnimFlags = 0
}

//-----------------------------------------------------------------
void Anim(int id, string name, int fps = 0, vector motion = "0 0 0")
{
	CurrentAnimID = id
	CurrentAnimSet.AnimNames[id] = name
	CurrentAnimSet.FPS[id] = fps
	CurrentAnimSet.Motion[id] = motion
	CurrentAnimSet.Flags[id] = CurrentAnimFlags
}

//-----------------------------------------------------------------
void AnimFlags(int flags)
{
	CurrentAnimSet.Flags[CurrentAnimID] = flags
}

//-----------------------------------------------------------------
void ApplyAnimFlags(int flags)
{
	CurrentAnimFlags |= flags
}

//-----------------------------------------------------------------
void ClearAnimFlags(int flags)
{
	flags = flags ^ 0xffffffff
	CurrentAnimFlags &= flags
}

//-----------------------------------------------------------------
void InitAnimSets();
//-----------------------------------------------------------------
void CleanupAnimSets()
{
	for(int s = 0; s < NumAnimSets; s++)
	{
		AnimSet set = AnimSets[s]
		if(set.Counter)
		{
			set.Counter = 0
			
			for(int n = 0; n < MAX_ANIMSLOTS; n++)
			{
				if(set.Anims[n])
				{
					ReleaseObject(set.Anims[n], 0)
					set.Anims[n] = VNULL
				}
			}
		}
		
		delete set
	}
}

//-----------------------------------------------------------------
	void WorldClass()
	{
		SpeechSetCount = -1
		CSType = -1

		for(int n = 0; n < sizeof(ShellObjectNames); n++)
		{
			ShellObjects[n] = GetObject(ShellObjectNames[n])
		}

		CameraIndex = 2
		PlayerControl = true
		ShowCursor = true
		ActualFrame = 0

		SetCurrent()
		DefFind = this
		PrevFind = this

		SetCameraFarPlane(0, viewdist)

		SetEventMask(this, EV_ONLOAD|EV_INIT|EV_SOUNDEND|EV_WORLDPROCESS)
		CallStackPos = 0

/*		if(playerscale == 0)
			playerscale = 1.0

		if(enemyscale == 0)
			enemyscale = 1.0*/

		if(!g_Viewer)
		{
			Transmitter = new SpeakTransmitter()
			InfDialog = new InfoDialog(250, 430)
			TaskList = new TasklistDialog(Transmitter.DlgPanel, InfDialog, -512, 100)
			TaskList.SetPos(-TaskList.width, 17)
			Cross = new CrossHairClass
			HackInfDialog = new HackInfoDialog(510, 400)
			SManager = new SpeechManager

			if(flaretype > 0)
				flare = new SunFlare(flaretype)
		}
		InitAnimSets()
	}

	void ~WorldClass()
	{
		if(ErrorWidget)
		{
			DestroyWidget(ErrorWidget)
			ErrorWidget = NULL
			ErrorFrame = 0
			ErrorNum = 0
		}

		for(int n = 0; n < sizeof(ShellObjectNames); n++)
		{
			if(ShellObjects[n])
			{
				ReleaseObject(ShellObjects[n], 0)
				ShellObjects[n] = VNULL
			}
		}

		for(int t = 0; t < sizeof(StreakShaderNames); t++)
		{
			for(n = 0; n < NumStreaks[t]; n++)
			{
				ReleaseObject(Streaks[t][n], true)
				Streaks[t][n] = VNULL
			}
		}

		if(flare)
		{
			delete flare
			flare = NULL
		}

		if(SManager)
		{
			delete SManager
			SManager = NULL
		}
		
		if(Transmitter)
		{
			delete Transmitter
			Transmitter = NULL	
		}

		if(TurretDisplay1)
		{
			delete TurretDisplay1
			TurretDisplay1 = NULL
		}

		if(MineDisplay)
		{
			delete MineDisplay
			MineDisplay = NULL	
		}

		if(CameraDisplay)
		{
			delete CameraDisplay
			CameraDisplay = NULL
		}

		if(EnemyDisplay)
		{
			delete EnemyDisplay
			EnemyDisplay = NULL
		}
		
		if(ScreenDisplay)
		{
			delete ScreenDisplay
			ScreenDisplay = NULL	
		}
		
		if(InfDialog)
		{
			delete InfDialog
			InfDialog = NULL	
		}
		
		if(TaskList)
		{
			delete TaskList
			TaskList = NULL	
		}

		if(Cross)
		{
			delete Cross
			Cross = NULL	
		}

		if(HackInfDialog)
		{
			delete HackInfDialog
			HackInfDialog = NULL	
		}
/*		
		if(MessagePanel)
		{
			delete MessagePanel
			MessagePanel = NULL	
		}*/
	}
}


WorldClass World

void HackInterface::HackInterface(MinClass owner)
{
	Owner = owner
	BeforeInit()

	HackingTime = 2
	SelectOpacity = HG_OpacityUnselected
	SelectTargetOpacity = HG_OpacityUnselected
	RotAngle = frand(0, 360)

	WSymbol = CreateWidget(WTYPE_IMAGE, 0, 0, 200, 200, WF_SOURCEALPHA | WF_BLEND|WF_CUSTOMUV|WF_NOWRAP|WF_ADDITIVE, 0xffffffff, 2)
	WSymbol2 = CreateWidget(WTYPE_IMAGE, 0, 0, 200, 200, WF_SOURCEALPHA | WF_BLEND|WF_CUSTOMUV|WF_NOWRAP|WF_ADDITIVE, 0xffffffff, 1)
	WLabel = CreateWidget(WTYPE_TEXT, 0, 0, 240, 32, WF_SOURCEALPHA | WF_BLEND|WF_ADDITIVE, 0xffffffff, 3)
	WProgress = CreateWidget(WTYPE_TEXT, 0, 0, 32, 32, WF_BLEND|WF_CENTER|WF_VCENTER, 0xffffffff, 4)
	SetFont(WProgress, MainFont)
//	LoadWidgetImage(WProgress, 0, "gfx/remotecontrol/titlegrad.tga")
	SetWidgetTextOffset(WProgress, 2, -1)
//	SetWidgetTextColor(WProgress, 0xff000000)
//	SetWidgetColor(WProgress, 0xffffffff)

	WProgressBack = CreateWidget(WTYPE_IMAGE, 0, 0, 32, 32, WF_BLEND, 0xffffffff, 3)
	WArrow = CreateWidget(WTYPE_IMAGE, 0, 0, 32, 32, WF_SOURCEALPHA|WF_BLEND|WF_CUSTOMUV|WF_NOWRAP|WF_ADDITIVE, 0xffffffff, 4)
	LoadWidgetImage(WSymbol, 0, "gfx/remotecontrol/spot_camera.tga")
	LoadWidgetImage(WSymbol2, 0, "gfx/remotecontrol/spot_camera.tga")
	LoadWidgetImage(WLabel, 0, "gfx/remotecontrol/cross_d.tga")
	LoadWidgetImage(WArrow, 0, "gfx/remotecontrol/direction.tga")
	SetFont(WLabel, MainFont)

	if(name)
		SetWidgetTextEx(WLabel, 0, 1, "#" + name)
	else
		SetWidgetText(WLabel, 0, "NoNameDevice")

	SetWidgetTextOffset(WLabel, 30, -2)
}

//-----------------------------------------------------------------
void ExtClass::M_Use(ExtClass other)
{
	int CanActiv = CanActivate(0, World.Player)
	
	if(CanActiv == ACT_ENABLE)	//najprvs skusame ci sa to neda aktivovat
		CallMethods(this, OnSuccess)
	else
		CallMethods(this, OnFail)
}

//-----------------------------------------------------------------
void ExtClass::EOnInit(MinClass other, int extra)
{
	if(UseName)
		UseClass = World.ItemColPtr.GetItemIndex(UseName)
}

//-----------------------------------------------------------------
void SunFlare::Update()
{
	if(g_iIsOutdoor == false)
	{
		if(IsVisible)
			Switch(false)

		return
	}

	vector CamPos = World.CameraPos
	vector SunPos = CamPos + (g_vSunDirection * (float)1000000)
	float dot = World.CameraNorm * g_vSunDirection
	/*
	vector from = CamPos
	vector to = World.CameraNorm * 100
	to = from + to
	
	if(KeyState(KC_K))
		AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, from, to)
*/

	if(dot >= 0.6)
	{
		float Plane[4]
		int cont, surf
		vector end = SunPos

//			TraceLineEx(PLAYER, CamPos, end, ZeroVec, ZeroVec, NULL, Plane, cont, surf, TRACE_ENTS|TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH, SunTracFilter)
		TraceLineEx(DEFAULT, CamPos, end, ZeroVec, ZeroVec, NULL, Plane, cont, surf, TRACE_WORLD|TRACE_DETAIL|TRACE_PATCH, SunTracFilter)
//			Print(g_strShader)

		if(surf & SURF_SKY)
		{
			if(IsVisible == false)
				Switch(true)

			dot -= 0.6
			dot *= 2.2	//4.2

			if(dot > 1)
				dot = 1

			SetAlpha(dot)
		}
		else
			SetAlpha(0)

//			AddDShape(SHAPE_BBOX, 0xffff0000, SS_NOZBUFFER|SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, FinePos - "3 3 3", FinePos + "3 3 3")

//		Print(CamPos)
		SetOrigin(this, CamPos + g_vSunDirection)
		UpdateEntity(this)
		return
	}

	if(IsVisible)
		Switch(false)
}

const float ArrayCircleDist = 100	//jak daleko je sipka od stredu entity v 2D
const float HG_SelectorHalfWidth = 75			//polomer vizualizeru
const float HG_SelectorHalfHeight = 75
const float HG_SelectorWidth = 150
const float HG_SelectorHeight = 150
const float HG_LabelOffsetX = 50				//label entity od stredu selectoru
const float HG_LabelOffsetY = 50
const float HG_LabelWidth = 180
const float HG_LabelHeight = 32
const float HG_ProgressOffsetX = 26
const float HG_ProgressOffsetY = 20
const float HG_ProgressWidth = 100
const float HG_ProgressHeight = 16

//---------------------------------------------------------------------------------	
void HackInterface::UpdateVisualizers(vector PlayerPos)
{
	float FinalFlashScale
	RotAngle += ftime * 30	//pomaly rotujeme dookola

	float diff = SelectTargetOpacity - SelectOpacity
	SelectOpacity += diff * ftime * 5
	clamp SelectOpacity<0, 255>

	diff = FadeScaleTarget - FadeScale
	FadeScale += diff * ftime * 5

	//vector orig = GetMatrix(Owner, 3)
	//if(!IsBoxVisible(orig, orig, 0))
		//return

	vector CameraLeft = g_CamMat[0]
	vector CameraForward = g_CamMat[1]
	vector CameraUp = g_CamMat[2]

	vector pos3D = GetVisualPos()
	vector PlaOrg = g_CamMat[3]//PlayerPos
//		pos3D[2] = PlaOrg[2]
	vector DirBetw = pos3D - PlaOrg
	float DistFromCamera = VectorNormalize(DirBetw)
	
	DistanceScale = DistFromCamera - 256
	clamp DistanceScale<0, 256>
	DistanceScale /= 512
	clamp DistanceScale<0, 1>
	DistanceScale = 1 - DistanceScale
	DistanceScale = 0.3 + (DistanceScale * 0.7)

	float DotLeft = CameraLeft * DirBetw
	float DotUp = CameraUp * DirBetw
	float DotForward = CameraForward * DirBetw

	bool InScreen = false

	if(DotForward < 0 && DotUp > -0.5 && DotUp < 0.5 && DotLeft > -0.6 && DotLeft < 0.6)	//je v obraze
		InScreen = true
	
	float xx = 400 + (DotLeft * -580)
	float yy = 300 + (DotUp * -580)

	vector v1 = Vector(400, 300, 0)
	vector v2 = Vector(xx, yy, 0)
	vector Dir2D = v2 - v1
	float Dist = VectorNormalize(Dir2D)
	vector angles = Vector2Angles(Dir2D)
	float rotang = angles[1]
	
	float UVs[8]
	vector pos

	if(rotang >= 37 && rotang < 143)	//spodna strana
	{
		pos = IntersectLines(-10,590,810,590,  400,300,xx,yy)

		if(pos[0] >= -10 && pos[0] <= 810)	//pretina spodnu cast
		{
			xx = pos[0]
			yy = pos[1]
		}
	}
	else
	{
		if(rotang >= 143 && rotang < 217)	//lava strana
		{
			pos = IntersectLines(10,-10,10,610,  400,300,xx,yy)
		
			if(pos[1] >= -10 && pos[1] <= 610)	//pretina lavu cast
			{
				xx = pos[0]
				yy = pos[1]
			}
		}
		else
		{
			if(rotang >= 217 && rotang < 323)	//horna strana
			{
				pos = IntersectLines(-10,10,810,10,  400,300,xx,yy)

				if(pos[0] >= -10 && pos[0] <= 810)	//pretina hornu cast
				{
					xx = pos[0]
					yy = pos[1]
				}
			}
			else	//prava strana
			{
				pos = IntersectLines(790,-10,790,610,  400,300,xx,yy)

				if(pos[1] >= -10 && pos[1] <= 610)	//pretina pravu cast
				{
					xx = pos[0]
					yy = pos[1]
				}
			}
		}	
	}

	float ArrowTransp
	vector pp = ProjectVector(GetMainEntity(), ZeroVec)

	if(pp[2] < 1)
	{
		Xpos = pp[0]	//pozicia na screene
		Ypos = pp[1]
		float InvScaleX = 800 / g_iWidth
		float InvScaleY = 600 / g_iHeight
		float VirtPosX = Xpos * InvScaleX	//pozicia na virtualnom platne 800x600
		float VirtPosY = Ypos * InvScaleY
	
		vector vv1 = Vector(VirtPosX, VirtPosY, 0)
		vector vv2 = Vector(xx, yy, 0)
		float lng = VectorLength(vv1 - vv2)
		clamp lng<0, ArrayCircleDist>
		float ArrowFadeRate = lng / ArrayCircleDist
	
		if(InRect(10 ,790 ,10 ,590 ,VirtPosX ,VirtPosY))
			ArrowFadeRate = 0
		
		ArrowTransp = ArrowFadeRate * FadeScale * 255
	
	//	v1 = Vector(400, 300, 0)
		v2 = Vector(xx, yy, 0)
		Dir2D = v2 - v1
		Dist = VectorNormalize(Dir2D)
	
		Dist -= 1 - ArrowFadeRate * ArrayCircleDist
		Dir2D = Dir2D * Dist
		xx = 400 + Dir2D[0]
		yy = 300 + Dir2D[1]

		float LabelLeft = VirtPosX + (HG_LabelOffsetX * DistanceScale)
		float LabelTop = VirtPosY + (HG_LabelOffsetY * DistanceScale)
		float ProgressRate = CurHackingTime / HackingTime
		float ProgressWidth = ProgressRate * HG_ProgressWidth
		float SymbolHalfWidth = HG_SelectorHalfWidth * DistanceScale
		float SymbolHalfHeight = HG_SelectorHalfHeight * DistanceScale
		float SymbolWidth = HG_SelectorWidth * DistanceScale
		float SymbolHeight = HG_SelectorHeight * DistanceScale

		SetWidgetPos(WSymbol, VirtPosX - SymbolHalfWidth, VirtPosY - SymbolHalfHeight, SymbolWidth, SymbolHeight)
		SetWidgetPos(WLabel, LabelLeft, LabelTop, HG_LabelWidth, HG_LabelHeight)
		SetWidgetPos(WProgressBack, LabelLeft + HG_ProgressOffsetX, LabelTop + HG_ProgressOffsetY, HG_ProgressWidth, HG_ProgressHeight)
		SetWidgetPos(WProgress, LabelLeft + HG_ProgressOffsetX, LabelTop + HG_ProgressOffsetY, ProgressWidth, HG_ProgressHeight)

		if(ProgressRate == 1)
		{
			ProgressFlickTimer += ftime
			
			if(ProgressFlickTimer > 0.2)
			{
				ProgressTextVis = 1 - ProgressTextVis
				ProgressFlickTimer = 0

				if(ProgressTextVis)
					SetWidgetTextEx(WProgress, 0, 1, "#READY")
				else
					SetWidgetText(WProgress, 0, "")
			}
		}

		FlashScale += ftime
		clamp FlashScale<0, 1>
		float flip = 1 - FlashScale
		FinalFlashScale = flip * flip
		FinalFlashScale = 1 - FinalFlashScale

		float RadiusExtend = 120 * FinalFlashScale
		float Width = SymbolWidth + (RadiusExtend * DistanceScale)
		float Height = SymbolHeight + (RadiusExtend * DistanceScale)
		float left = VirtPosX - (Width * 0.5)
		float top = VirtPosY - (Height * 0.5)
		SetWidgetPos(WSymbol2, left, top, Width, Height)
 
		GetRotatedSpriteUVs(UVs, FinalFlashScale * 175 * RotDir + RotAngle)
		SetWidgetUV(WSymbol, UVs)
		SetWidgetUV(WSymbol2, UVs)

		float SelectOpacityRate = SelectOpacity - HG_OpacityUnselected / (HG_OpacitySelected - HG_OpacityUnselected)
		float LabelOp = SelectOpacity * FadeScale * GlobalVisibilityScale
		float ProgressOp = LabelOp// * SelectOpacityRate
		float ProgressBackOp = ProgressOp * 0.25

		int LabelColor = AWHITE(LabelOp)
		SetWidgetColor(WSymbol, LabelColor)
		SetWidgetColor(WLabel, LabelColor)
		SetWidgetTextColor(WLabel, LabelColor)
		SetWidgetColor(WProgress, ARGB(ProgressOp, 136, 241, 254))
		SetWidgetTextColor(WProgress, ARGB(ProgressOp, 0, 0, 0))
		SetWidgetColor(WProgressBack, ARGB(ProgressBackOp, 136, 241, 254))

		float alp2 = 1 - FinalFlashScale * FadeScale * GlobalVisibilityScale * 255
		SetWidgetColor(WSymbol2, AWHITE(alp2))
	}
	else
	{
		ArrowTransp = FadeScale * 255	//na odvratenej strane sipku vidime vzdy
		SetWidgetColor(WSymbol, 0x00000000)
		SetWidgetColor(WLabel, 0x00000000)
		SetWidgetTextColor(WLabel, 0x00000000)
		SetWidgetColor(WProgress, 0x00000000)
		SetWidgetTextColor(WProgress, 0x00000000)
		SetWidgetColor(WProgressBack, 0x00000000)
	}

	SetWidgetColor(WArrow, AWHITE(ArrowTransp))
	SetWidgetPos(WArrow, xx - 16, yy - 16, 32, 32 )
	GetRotatedSpriteUVs(UVs, -rotang)
	SetWidgetUV(WArrow, UVs)
}

//-----------------------------------------------------------------
void ExtClass::M_Parent(ExtClass other)
{
	string Value = other._value
	string Parms[8]
	int NumParms = ParseString(Value, Parms)

	if(NumParms == 1)
	{
		int p = atoi(other._value) - 1
		if(p >= -1 && p < 8)
		{
			SetOrigin(other, ZeroVec)
			if(!AddChild(this, other, p))
			{
				DPrint("Can't parent " + other._name + " on pivot " + other._value + " of " + _name)
				ErrorNum++
			}
			else
				other.OnParent(this)	//informujeme child ze bol naparenteny
		}
		return
	}

	if(NumParms >= 3)
	{
		ExtClass ChildEnt = FindClass(Parms[0])
		
		if(!ChildEnt)
		{
			DPrint("Can't parent " + Parms[0] + " on " + _name + ". Entity " + Parms[0] + " not exist")
			ErrorNum++
			return
		}
		
		int pt = atoi(Parms[2]) - 1
		if(pt >= -1 && pt < 8)
		{
			UpdateEntity(this)
			UpdateEntity(ChildEnt)
			vector ChildPos = GetMatrix(ChildEnt, 3)
			vector mat[3]
			GetMatrix4(this, mat)
			vector ParentOff = ZeroVec

			if(NumParms == 5 && Parms[4] == "keepoffset")
			{
				ParentOff = ChildPos - GetMatrix(this, 3)
				ParentOff = VectorMatrixInvMultiply3(mat, ParentOff)
			}
	
			if(AddChild(this, ChildEnt, pt))
			{
				SetOrigin(ChildEnt, ParentOff)
				UpdateEntity(this)
				ChildEnt.OnParent(this)
#ifdef EDITOR
				bool ChildFound = false
				for(int c = 0; c < ChildsNamesNum; c++)
				{
					if(ChildsNames[c] == ChildEnt._name)
						ChildFound = true
				}
				
				if(!ChildFound)
					ChildsNames[ChildsNamesNum++] = ChildEnt._name	//nemozeme si ukladat p

				ChildEnt.ParentName = _name
#endif	
			}				
			else
			{
				DPrint("Can't parent " + ChildEnt._name + " on pivot " + Parms[2] + " of " + _name)
				ErrorNum++
			}
		}
	}
}

//-----------------------------------------------------------------
void WorldClass::SetCurrent()
{	
	World = this
}

//-----------------------------------------------------------------
void ExtClass::UpdateAgent(bool set)
{
	if(!set)
	{
		if(agent)
		{
			agent.Destroy()
			agent = NULL
		}
	}
	else
	{	
		int ff = GetFlags(this)
		
		if(ff & TFL_TRIGGER || !ff & TFL_SOLID)
		{
			if(agent)
			{
				agent.Destroy()
				agent = NULL
			}
			return
		}

		vector mins, maxs
		GetBoundBox(this, mins, maxs)
		vector mat[4]
		GetMatrix4(this, mat)
		agent = World.pPathFinder.AgentFromOBB(agent, mat, mins, maxs)
	}
}

//-----------------------------------------------------------------
CPosition ExtClass::GetWPos()
{
	if(WAFrame == g_iViewFrame)
		return WPosition

	if(WPosition == NULL)
		WPosition = new CPosition

	if(!World.pPathFinder.PositionForPoint(GetOrigin(this), WPosition))
	{
		delete WPosition
		WPosition = NULL
	}
	
	WAFrame = g_iViewFrame

	return WPosition
}

//-----------------------------------------------------------------
void ExtClass::Exec(string parm)
{
	CallMethods(this, parm)
}


void NotifyNoises(ExtClass actor, vector origin, float radius, int noises);

//-----------------------------------------------------------------
void SetPosInCamAxis(MinClass other, vector RelCoords, vector angles, float scal)
{
	float f
	vector Off
	Off[1] = RelCoords[2]//vzdalenost od kamery
	f = RelCoords[0]
	Off[0] = -f			//offset x
	f = RelCoords[1]
	Off[2] = -f			//offset y

	vector mat2[4]
	RollPitchYawMatrix(angles, mat2)	//rotacia okolo vlastnej osi

	vector res[4]

	mat2[3] = Off
	MatrixMultiply4(g_CamMat, mat2, res)

	if(scal == 1.0)
	{
		SetMatrix(other, res)
		return
	}
	
	vector scale[4]
	scale[0][0] = scal
	scale[1][1] = scal
	scale[2][2] = scal

	vector res2[4]
	MatrixMultiply4(res, scale, res2)

	SetMatrix(other, res2)
}

void CalcMatrixFromCamera()
{
	vector CameraAng
	
	CameraAng[1] = World.CameraAngs[1] + 90.0
	CameraAng[2] = World.CameraAngs[0] * -1

	RollPitchYawMatrix(CameraAng, g_CamMat)
	g_CamMat[3] = World.CameraPos
}

//=============================================================================
class JointClass extends ExtClass
{
	vector coords
	float angle, angle2, angle3
	int flags
	string body1, body2
	RigidBodyClass Body1, Body2
	dJoint handle
	JointClass NextJoint

//---------------------------------------------------------------------------------	
	void OnBreak(dJoint JointHandle)
	{
		if(JointHandle != handle && NextJoint)
			NextJoint.OnBreak(JointHandle)
	}

//---------------------------------------------------------------------------------
	bool Create()
	{
		return false
	}

//---------------------------------------------------------------------------------
	void Destroy()
	{
		if(handle)
		{
			dJointDestroy(handle)	
			handle = 0
		}
	}

//---------------------------------------------------------------------------------	
	bool FindBodies();


#ifdef EDITOR
	void EditorOpened()
	{
		Destroy()
	}
#endif

//---------------------------------------------------------------------------------	
	void EOnLoaded(ExtClass other, int extra);


//---------------------------------------------------------------------------------	
	void EOnInit(ExtClass other, int extra)
	{
		SetOrigin(this, coords)
		SetAngles(this, Vector(angle3, angle, angle2))
	}

//---------------------------------------------------------------------------------	
	void ~JointClass()
	{
		Destroy()
	}
}

//=================================================================================
class RichJoint extends JointClass
{
	string OnBreak, OnBreakAngLimit, OnBreakAngSpring
	float BreakThreshold, AngLimitThreshold, AngSpringThreshold
	float angstiffness
	float angdamping
	dJoint AngLimitHandle
	dJoint AngSpringHandle

//---------------------------------------------------------------------------------	
	void M_BreakAngularLimit(ExtClass other)
	{
		if(AngLimitHandle)
		{
			dJointDestroy(AngLimitHandle)
			AngLimitHandle = 0
		}
	}

//---------------------------------------------------------------------------------	
	void M_BreakAngularSpring(ExtClass other)
	{
		if(AngSpringHandle)
		{
			dJointDestroy(AngSpringHandle)
			AngSpringHandle = 0
		}
	}

//---------------------------------------------------------------------------------
	void Destroy()
	{
		if(handle)
		{
			dJointDestroy(handle)	
			handle = 0
		}

		if(AngLimitHandle)
		{
			dJointDestroy(AngLimitHandle)
			AngLimitHandle = 0
		}
		
		if(AngSpringHandle)
		{
			dJointDestroy(AngSpringHandle)
			AngSpringHandle = 0
		}
	}

//---------------------------------------------------------------------------------	
	void M_Break(ExtClass other)
	{
		Destroy()
	}

//---------------------------------------------------------------------------------	
	void OnBreak(dJoint JointHandle)
	{
		bool IsMyJoint = false

		if(JointHandle == handle)	//ak sa uthne hlavny joint tak s nim rusime aj limity
		{
			if(AngSpringHandle)
			{
				dJointDestroy(AngSpringHandle)
				AngSpringHandle = 0
				CallMethods(this, OnBreakAngSpring)
			}
			
			if(AngLimitHandle)
			{
				dJointDestroy(AngLimitHandle)
				AngLimitHandle = 0
				CallMethods(this, OnBreakAngLimit)
			}

			handle = 0
			IsMyJoint = true
			CallMethods(this, OnBreak)
		}
		else
		{
			if(JointHandle == AngLimitHandle)
			{
				AngLimitHandle = 0
				IsMyJoint = true
				CallMethods(this, OnBreakAngLimit)
			}
			else
			{
				if(JointHandle == AngSpringHandle)
				{
					AngSpringHandle = 0
					IsMyJoint = true
					CallMethods(this, OnBreakAngSpring)
				}
			}
		}

		if(!IsMyJoint && NextJoint)
			NextJoint.OnBreak(JointHandle)
	}
}

//=================================================================================
class joint_fixed extends RichJoint
{
	float anglimit

//---------------------------------------------------------------------------------
	bool Create()
	{
		bool IntBlock = false
		bool FixOnlyDist = false
		bool AngularLimit = false
		bool AngularSpring = false

		if(flags & 1)
			IntBlock = true

		if(flags & 2)
			FixOnlyDist = true

		if(flags & 4)
			AngularLimit = true
			
		if(flags & 8)
			AngularSpring = true

		if(FixOnlyDist)
		{
			handle = dJointCreateBall(Body1, Body2, coords, IntBlock, BreakThreshold)

			if(AngularLimit)
			{
				UpdateEntity(this)
				vector Body1Pos = GetMatrix(Body1, 3)
				vector Body2Pos = GetMatrix(Body2, 3)
				vector axis = Body1Pos - Body2Pos
				VectorNormalize(axis)
				clamp anglimit<-360, 360>
				AngLimitHandle = dJointCreateAngularLimit(Body1, axis, Body2, axis, anglimit * DEG2RAD, AngLimitThreshold)
			}

			if(AngularSpring)
				AngSpringHandle = dJointCreateAngularSpringDamper(Body1, axis, Body2, axis, angstiffness, angdamping, AngSpringThreshold)	
		}
		else
			handle = dJointCreateFixed(Body1, Body2, coords, IntBlock, BreakThreshold)

		return true
	}

//--------------------------------------------------------------------------------------
	void EOnLoaded(ExtClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}
}

//=================================================================================
class joint_hinge extends RichJoint
{
	float anglimitmin, anglimitmax
	float springangle

//---------------------------------------------------------------------------------
	bool Create()
	{
		bool IntBlock = false
		bool AngularLimit = false
		bool AngularSpring = false
		
		if(flags & 1)
			IntBlock = true

		if(flags & 2)
			AngularLimit = true
			
		if(flags & 4)
			AngularSpring = true

		UpdateEntity(this)
		vector Body1Pos, Body2Pos
		vector mat[4]
		GetMatrix4(this, mat)
		vector axis = mat[0]

		handle = dJointCreateHinge(Body1, Body2, coords, axis, IntBlock, BreakThreshold)

		vector AngAxis1 = mat[2]	//prva osa vzdycky tato kolmica na hlavnu osu
		vector AngAxis2

		if(AngularLimit)
		{
			clamp anglimitmin<-360, 0>
			clamp anglimitmax<0, 360>
			clamp springangle<anglimitmin, anglimitmax>		//nema zmysel davat pruzinu mimo limit
			
			float rangeangle = fabs(anglimitmin) + anglimitmax	//kolo stupov volnosti tam bude
			float jointangle = rangeangle * 0.5 * DEG2RAD		//polovicu dame do jointu
			float offsetangle = 0

			if(rangeangle > 0)
			{	
				float angminrate = fabs(anglimitmin) / rangeangle		//percentualne jedna strana voci celemu rozsahu
				float angmaxrate = fabs(anglimitmax) / rangeangle		//percentualne druha strana voci celemu rozsahu
				float offsetrate = angmaxrate - angminrate
				offsetangle = offsetrate * rangeangle
				offsetangle *= 0.5 * DEG2RAD
			}
			
			AngAxis2 = VectorMatrixMultiply3(mat, Vector(0,sin(offsetangle),cos(offsetangle)))
			AngLimitHandle = dJointCreateAngularLimit(Body1, AngAxis1, Body2, AngAxis2, jointangle, AngLimitThreshold)
		}

		if(AngularSpring)
		{
			float radangle = springangle * DEG2RAD
			AngAxis2 = VectorMatrixMultiply3(mat, Vector(0,sin(radangle),cos(radangle)))
			AngSpringHandle = dJointCreateAngularSpringDamper(Body1, AngAxis1, Body2, AngAxis2, angstiffness, angdamping, AngSpringThreshold)	
		}

		return true
	}

//--------------------------------------------------------------------------------------
	void EOnLoaded(ExtClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}
}

//=================================================================================
class joint_sliding extends RichJoint
{
	float linstiffness
	float lindamping
	float linlimitmin, linlimitmax
	float springoffset
	float springlength
	float anglimitmin, anglimitmax
	float springangle
	float LinLimitThreshold, LinSpringThreshold
	string OnBreakLinLimit, OnBreakLinSpring


	dJoint LinLimitHandle
	dJoint LinSpringHandle

//---------------------------------------------------------------------------------	
//---------------------------------------------------------------------------------	
	void OnBreak(dJoint JointHandle)
	{
		bool IsMyJoint = false

		if(JointHandle == handle)	//ak sa uthne hlavny joint tak s nim rusime aj limity
		{
			if(AngSpringHandle)
			{
				dJointDestroy(AngSpringHandle)
				AngSpringHandle = 0
				CallMethods(this, OnBreakAngSpring)
			}

			if(AngLimitHandle)
			{
				dJointDestroy(AngLimitHandle)
				AngLimitHandle = 0
				CallMethods(this, OnBreakAngLimit)
			}
			
			if(LinSpringHandle)
			{
				dJointDestroy(LinSpringHandle)
				LinSpringHandle = 0
				CallMethods(this, OnBreakLinSpring)
			}

			if(LinLimitHandle)
			{
				dJointDestroy(LinLimitHandle)
				LinLimitHandle = 0
				CallMethods(this, OnBreakLinLimit)
			}

			handle = 0
			IsMyJoint = true
			CallMethods(this, OnBreak)
		}
		else
		{
			if(JointHandle == AngLimitHandle)
			{
				AngLimitHandle = 0
				IsMyJoint = true
				CallMethods(this, OnBreakAngLimit)
			}
			else
			{
				if(JointHandle == AngSpringHandle)
				{
					AngSpringHandle = 0
					IsMyJoint = true
					CallMethods(this, OnBreakAngSpring)
				}
				else
				{
					if(JointHandle == LinLimitHandle)
					{
						LinLimitHandle = 0
						IsMyJoint = true
						CallMethods(this, OnBreakLinLimit)
					}
					else
					{
						if(JointHandle == LinSpringHandle)
						{
							LinSpringHandle = 0
							IsMyJoint = true
							CallMethods(this, OnBreakLinSpring)
						}
					}	
				}
			}
		}

		if(!IsMyJoint && NextJoint)
			NextJoint.OnBreak(JointHandle)
	}

//---------------------------------------------------------------------------------	
	void M_BreakLinearLimit(ExtClass other)
	{
		if(LinLimitHandle)
		{
			dJointDestroy(LinLimitHandle)
			LinLimitHandle = 0
		}
	}

//---------------------------------------------------------------------------------	
	void M_BreakLinearSpring(ExtClass other)
	{
		if(LinSpringHandle)
		{
			dJointDestroy(LinSpringHandle)
			LinSpringHandle = 0
		}
	}

//---------------------------------------------------------------------------------
	bool Create()
	{
		bool IntBlock = false
		bool LinearLimit = false
		bool LinearSpring = false
		bool AngularLimit = false
		bool AngularSpring = false
		
		if(flags & 1)
			IntBlock = true

		if(flags & 2)
			LinearLimit = true

		if(flags & 4)
			LinearSpring = true

		if(flags & 8)
			AngularLimit = true

		if(flags & 16)
			AngularSpring = true

		UpdateEntity(this)
		vector mat[4]
		GetMatrix4(this, mat)
		vector axis = mat[0]

		handle = dJointCreateSliding(Body1, Body2, coords, axis, IntBlock, BreakThreshold)

		vector point1, point2

		if(Body1 == World)
			point1 = GetMatrix(Body2, 3)
		else
		{
			if(Body2 == World)
				point1 = GetMatrix(Body1, 3)
			else
				point1 = GetMatrix(Body2, 3)
		}
		
		if(LinearLimit)
		{
			clamp linlimitmin<-999999, 0>
			clamp linlimitmax<0, 999999>
	
			float distance = linlimitmax + fabs(linlimitmin)

			vector off1 = axis * -linlimitmax * 0.5
			point2 = point1 + off1
			
			vector off2 = axis * linlimitmin * 0.5
			point1 = point1 + off2

			LinLimitHandle = dJointCreateLinearLimit(Body1, point1, Body2, point2, distance * 0.5, LinLimitThreshold)
		}

		if(LinearSpring)
		{
			float halfoffset = springoffset * 0.5
			vector off = axis * halfoffset
			vector p1 = point1 + off
			vector p2 = point1 - off
			LinSpringHandle = dJointCreateSpringDamper(Body1, p1, Body2, p2, linstiffness, lindamping, springlength, LinSpringThreshold)
		}

		vector AngAxis1 = mat[2]	//prva osa vzdycky tato kolmica na hlavnu osu
		vector AngAxis2

		if(AngularLimit)
		{
			clamp anglimitmin<-360, 0>
			clamp anglimitmax<0, 360>
			clamp springangle<anglimitmin, anglimitmax>		//nema zmysel davat pruzinu mimo limit
			
			float rangeangle = fabs(anglimitmin) + anglimitmax	//kolo stupov volnosti tam bude
			float jointangle = rangeangle * 0.5 * DEG2RAD		//polovicu dame do jointu
			float offsetangle = 0

			if(rangeangle > 0)
			{	
				float angminrate = fabs(anglimitmin) / rangeangle		//percentualne jedna strana voci celemu rozsahu
				float angmaxrate = fabs(anglimitmax) / rangeangle		//percentualne druha strana voci celemu rozsahu
				float offsetrate = angmaxrate - angminrate
				offsetangle = offsetrate * rangeangle
				offsetangle *= 0.5 * DEG2RAD
			}
			
			AngAxis2 = VectorMatrixMultiply3(mat, Vector(0,sin(offsetangle),cos(offsetangle)))
			AngLimitHandle = dJointCreateAngularLimit(Body1, AngAxis1, Body2, AngAxis2, jointangle, AngLimitThreshold)
		}

		if(AngularSpring)
		{
			float radangle = springangle * DEG2RAD
			AngAxis2 = VectorMatrixMultiply3(mat, Vector(0,sin(radangle),cos(radangle)))
			AngSpringHandle = dJointCreateAngularSpringDamper(Body1, AngAxis1, Body2, AngAxis2, angstiffness, angdamping, AngSpringThreshold)	
		}

		return true
	}

//---------------------------------------------------------------------------------
	void Destroy()
	{
		if(handle)
		{
			dJointDestroy(handle)	
			handle = 0
		}
		
		if(LinLimitHandle)
		{
			dJointDestroy(LinLimitHandle)
			LinLimitHandle = 0
		}

		if(LinSpringHandle)
		{
			dJointDestroy(LinSpringHandle)
			LinSpringHandle = 0
		}

		if(AngLimitHandle)
		{
			dJointDestroy(AngLimitHandle)
			AngLimitHandle = 0
		}
		
		if(AngSpringHandle)
		{
			dJointDestroy(AngSpringHandle)
			AngSpringHandle = 0
		}
	}

//--------------------------------------------------------------------------------------
	void EOnLoaded(ExtClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}
}

//======================================================================================
class joint_motor extends JointClass
{
	dJoint AngularMotorHandle
	float MoveVel, RotVel
	float MoveAccel, RotAccel

//--------------------------------------------------------------------------------------
	void WakeUpBodies();

//--------------------------------------------------------------------------------------
	void M_SetMoving(ExtClass other)
	{
		if(!handle)
			return

		string Parms[8]
		int NumParms = ParseString(other._value, Parms)

		if(NumParms >= 1)
			MoveVel = atof(Parms[0])

		if(NumParms >= 2)
			MoveAccel = atof(Parms[2])

		dJointSetMotor(handle, MoveVel, MoveAccel)
		WakeUpBodies()
	}

//--------------------------------------------------------------------------------------
	void M_SwitchMoveDir(ExtClass other)
	{
		if(!handle)
			return

		MoveVel = -MoveVel
		dJointSetMotor(handle, MoveVel, MoveAccel)
		WakeUpBodies()
	}

//--------------------------------------------------------------------------------------
	void M_SetRotating(ExtClass other)
	{
		if(!AngularMotorHandle)
			return

		string Parms[8]
		int NumParms = ParseString(other._value, Parms)

		if(NumParms >= 1)
			RotVel = atof(Parms[0])

		if(NumParms >= 2)
			RotAccel = atof(Parms[2])

		dJointSetMotor(AngularMotorHandle, RotVel, RotAccel)
		WakeUpBodies()
	}

//--------------------------------------------------------------------------------------
	void M_SwitchRotateDir(ExtClass other)
	{
		if(!AngularMotorHandle)
			return

		RotVel = -RotVel
		dJointSetMotor(AngularMotorHandle, RotVel, RotAccel)
		WakeUpBodies()
	}

//--------------------------------------------------------------------------------------
	bool Create()
	{
		bool IsLinear = false
		bool IsAngular = false

		if(flags & 1)
			IsLinear = true

		if(flags & 2)
			IsAngular = true

		UpdateEntity(this)
		vector axis = GetMatrix(this, 0)
		
		vector v1 = GetMatrix(this, 3)
		vector v2 = axis * 100
		v2 = v1 + v2

		if(IsLinear)
			handle = dJointCreateLinearMotor(Body1, Body2, axis, 0, 0)
			
		if(IsAngular)
			AngularMotorHandle = dJointCreateAngularMotor(Body1, Body2, axis, 0, 0)
	}

//--------------------------------------------------------------------------------------
	void Destroy()
	{
		if(handle)
		{
			dJointDestroy(handle)	
			handle = 0
		}

		if(AngularMotorHandle)
		{
			dJointDestroy(AngularMotorHandle)	
			AngularMotorHandle = 0
		}	
	}

//--------------------------------------------------------------------------------------
	void EOnLoaded(ExtClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}
}

//======================================================================================
class RigidBodyClass extends ExtClass
{
	float mass
	int	InterLayer
	int impactsounds
	float weaponscale
	//float impactdamage		//kolko to ma uberat
	ShotStorage PhysHurtInfo
	bool pickable
	float _pathblock //-1 znamena zadna prekazka pri pathfindingu.
						  // 0 znamena "tvrdou" prekazkou
						  // Pokud >0, je to mekka prekazka, ktera se muze dat prekonat

	string _materialremap

	dGeom geom
	bool	rb //geom pro identifikaci Rigidbody nemuzeme pouzit, protoze geometrie muze byt interni z modelu

	vector Matrix[4]
	int LastSoundFrame
	float LastNotifyTime //kdy jsme naposledy informovali AI o hluku
	JointClass FirstJoint
	
	int NumContacts

//---------------------------------------------------------------------------------
	void AddJointDependence(JointClass joint)
	{
		if(!FirstJoint)
			FirstJoint = joint
		else
		{
			JointClass CurrentJoint = FirstJoint
			JointClass LastJoint = NULL
			
			while(CurrentJoint)
			{
				if(CurrentJoint == joint)	//tento joint uz je v retazci. pridalo ho tam druhe body
					return

				LastJoint = CurrentJoint
				CurrentJoint = CurrentJoint.NextJoint	
			}
			LastJoint.NextJoint = joint
		}
	}

//---------------------------------------------------------------------------------
	void EOnJointBreak(class other, int extra)	//ked sa utrhne joint. volane obidvom spojenym body
	{
		if(FirstJoint)	//informujeme jointy ktore ovladaju toto body
			FirstJoint.OnBreak(extra)
	}

//-----------------------------------------------------------------
	void OnParent(ExtClass parent)	//boli sme naparenteni z ingame scriptu
	{
		vector Angles = GetAngles(this)
		ClearFlags(this, TFL_FULLMATRIX)	//ked chceme parentit tak tento flag v ziadnom pripade

		if(rb)
		{
			dBodyActive(this, true, false)	//Musime vypnut dynamiku. engine bude robit neustale SetFrame na body. kolidovat sice poriadne nebude ale stale je to uzitocne na trasovanie do phys geometrie
			dBodyInteractionLayer(this, ET_NOCOLLISION)
		}
		
		UpdateEntity(parent)
	}

//---------------------------------------------------------------------------------
	void SetMass(float Mass)
	{
		mass = Mass
		
		if(rb)
			dBodySetMass(this, ZeroVec, mass)
	}

//---------------------------------------------------------------------------------
	void SetWeaponScale(float WeaponScale)
	{
		weaponscale = WeaponScale
	}

//---------------------------------------------------------------------------------
	void SetPickable(bool val)
	{
		pickable = val	
	}

//---------------------------------------------------------------------------------
	bool GetPickable()
	{
		if(rb && dBodyIsDynamic(this) == false)
			return false

		return pickable
	}

//---------------------------------------------------------------------------------
	void OnPickedPhysicaly(bool droped)
	{
	}

//---------------------------------------------------------------------------------
	void SetTransform(vector mat[4])
	{
		SetFlags(this, TFL_FULLMATRIX)
		Matrix[0] = mat[0]
		Matrix[1] = mat[1]
		Matrix[2] = mat[2]
		Matrix[3] = mat[3]
		SetMatrix(this, Matrix)
		//UpdateEntity(this)
	}

//---------------------------------------------------------------------------------
	void SetTransformEx(vector Angs, vector Pos)
	{
		SetFlags(this, TFL_FULLMATRIX)
		RollPitchYawMatrix2(Angs, Matrix)
		Matrix[3] = Pos
		SetMatrix(this, Matrix)
		//UpdateEntity(this)
	}
	
//---------------------------------------------------------------------------------
	int Shoot(ShotStorage shot)	//MinClass other, vector pos, vector dir, float power, int bone
	{
		float ImpactPower

		if(shot.HurtType == HT_SHOT)
		{
			ImpactPower = shot.power * ShootImpactScale
	
			if(rb)
			{
				ImpactPower *= weaponscale
				float ke = dBodyGetKineticEnergy(this)
	
				if(ke > 10)	//aby sa nic nepohybovalo extremnymi rychlostami
					ImpactPower *= 0.1
				
				if(World.ManipEntity == this)
					ImpactPower *= 0.01
				
				vector vec = shot.dir * ImpactPower
				dBodyApplyImpulseAt(this, vec, shot.to)
			}
			return 1
		}
		
		if(shot.HurtType == HT_EXPLOSION)
		{
			ImpactPower = shot.power * ExplosionImpactScale
			
			if(rb)
			{
				vector Org = GetCenterOfEntity(this)
				vector ForceDir = Org - shot.from
				float dist = VectorNormalize(ForceDir)
				float scale = dist / shot.radius
				clamp scale<0, 1>
				scale = 1 -scale
				ImpactPower += ImpactPower * scale
				float MassScale = mass / 30
				
				if(MassScale > 2)
					MassScale = 2
					
				ImpactPower *= MassScale	//lahsie telesa musia dostat mensiu silu inac su velmi velke rozdiely
				
				if(World.ManipEntity == this)
					ImpactPower *= 0.1

				vector Force = ForceDir * ImpactPower
				dBodyApplyImpulseAt(this, Force, Org + "0 0 10")
			}
			return true
		}
		
		if(shot.HurtType == HT_FLAME)
		{
			dBodyApplyForce(this, shot.dir * shot.power)
			return true
		}
		return true
	}

//---------------------------------------------------------------------------------
	bool CreateRigidBody(int GeomType, string Material, string Model)
	{
		if(rb)
			return false

		//UpdateEntity(this)
			
		if(mass == 0)
			mass = 1	//s nulovym mass to pekelne zblbne
			
		//defaultne zkusime interni kolizni model
		if(GeomType == GEOMTYPE_NONE)
		{
			if(dBodyCreate(this, ZeroVec, BC_MODEL))
			{
				//spocitej defaultni mass & inertia
/*				if(Model == "obj/vehicles/buggy_wheel.xob")
				{
					dBodySetMass(this, "1000 1000 1000", mass)
					Print("je tam")
				}
				else*/
					dBodySetMass(this, ZeroVec, mass)

				SetFlags(this, TFL_SOLID | TFL_OBBCOLLISION | TFL_TOUCHTRIGGERS)
				SetEventMask(this, EV_JOINTBREAK)
				dBodyInteractionLayer(this, ET_DEFAULT)
				rb = true
				return true
			}
			else
			{
				SetPickable(false)

				//DPrint(String("collision geometry for " + Model) + " not found")
				return false
			}
		}

		if(GeomType == GEOMTYPE_NULL)	//ziadna geometria. null body
		{
			dBodyCreate(this, ZeroVec, BC_NONE)
			dBodySetMass(this, ComputeInertiaForNullBody(mass), mass)
			dBodyInteractionLayer(this, ET_NOCOLLISION)
			rb = true
			return true
		}

		vector mins, maxs, size
		GetBoundBox(this, mins, maxs)

		size = maxs - mins

		if(size[0] == 0 || size[1] == 0 || size[2] == 0)
		{
			DPrint(String("Entity " + _name + " has zero extents. CreateRigidBody failed!"))
			ErrorNum++
			return false
		}
		
		//nepovedlo se, vytvor body s definovanyma primitivama

		if(!Material)
		{			
			DPrint(String("Physics material for body "	+ _name + " not found !"))
			ErrorNum++
			Material = "material/default"
		}
		
		vector offset = mins + maxs * (float)0.5
		dBodyCreate(this, offset, BC_NONE)

		switch(GeomType)
		{
		case GEOMTYPE_BOX:
			geom = dGeomCreateBox(size)		
			break
		
		case GEOMTYPE_SPHERE:
			geom = dGeomCreateSphere(size[0] * 0.5)
			break

		case GEOMTYPE_CAPSULE:
			int ExtAxis = 0
			int RadiusAxis
			if(size[1] > size[ExtAxis])
				ExtAxis = 1

			if(size[2] > size[ExtAxis])
				ExtAxis = 2
			
			RadiusAxis = ExtAxis + 1
			
			if(RadiusAxis > 2)
				RadiusAxis = 0
			
			float radius = size[RadiusAxis] * 0.5
			float ext = size[ExtAxis] - radius - radius * 0.5
			
			if(ext <= 0)
			{
				ext = 0
				DPrint(String("Capsule geometry in " + _name + " is not optimal. Sensible solution is switch to GEOMTYPE_SPHERE"))
				ErrorNum++
			}

			geom = dGeomCreateCapsule(radius, Vector(0, 0, ext))
			break

		default:
			geom = dGeomCreateBox(size)
		}
	

		dBodyAddGeom(this, geom, g_vIdentMatrix, Material)
		dBodySetMass(this, ZeroVec, mass)
		dBodyInteractionLayer(this, ET_DEFAULT)	//defaultne musi byt nastaveny tento typ pre pripad zeby sa neskor uz nic nenastavilo.

		SetFlags(this, TFL_SOLID | TFL_OBBCOLLISION | TFL_TOUCHTRIGGERS)
		SetEventMask(this, EV_JOINTBREAK)
		rb = true
		return true
	}

//---------------------------------------------------------------------------------
	void Show(bool stat)
	{
		if(stat)
			SetFlags(this, TFL_VISIBLE)
		else
			ClearFlags(this, TFL_VISIBLE)
	}

//---------------------------------------------------------------------------------
	void ActivateBody(bool stat)
	{
		if(!rb)
			return

		if(stat)
		{
			dBodyActive(this, true, true)
			SetEventMask(this, EV_CONTACT)
			
			if(impactsounds)
				SetEventMask(this, EV_SOUNDEND)	//koli zvukom
		}
		else
		{
			dBodyActive(this, false, false)
			ClearEventMask(this, EV_CONTACT)
			ClearEventMask(this, EV_SOUNDEND)	//koli zvukom
		}
	}

//-----------------------------------------------------------------
	void M_PhysicsOn(ExtClass other)
	{
		ActivateBody(true)
	}

//-----------------------------------------------------------------
	void M_PhysicsOff(ExtClass other)
	{
		ActivateBody(false)
	}

//-----------------------------------------------------------------
	void M_Solid(ExtClass other)
	{
		SetFlags(this, TFL_SOLID)
		
		if(rb)
		{
			if(dBodyGetInteractionLayer(this) == ET_NOCOLLISION)
				dBodyInteractionLayer(this, InterLayer)
		}
	}
//-----------------------------------------------------------------
	void M_NonSolid(ExtClass other)
	{
		ClearFlags(this, TFL_SOLID)

		if(rb)
		{
			InterLayer = dBodyGetInteractionLayer(this)
			dBodyInteractionLayer(this, ET_NOCOLLISION)
		}
	}

//-----------------------------------------------------------------
	void M_Impulse(ExtClass other)
	{
		string Parms[4]
		int NumParms = ParseString(other._value, Parms)

		if(NumParms != 3)
		{
			string Err = "wrong num of parameters " + _name + ".Impulse"
			Print(Err)
			return
		}

		MinClass ent
		if(Parms[0] == _name)
			ent = this
		else
			ent = FindClass(Parms[0])
			
		if(!ent)
			return

		float pow = atof(Parms[2])
		vector pos = GetMatrix(ent, 3)
		vector force = GetMatrix(ent, 0)
		force = force * pow
		dBodyApplyImpulseAt(this, force, pos)
	}

//-----------------------------------------------------------------
	void EnableHurting(bool enable, float maxtime, float damage)
	{
		if(enable)
		{
			if(!PhysHurtInfo)
			{
				PhysHurtInfo = new ShotStorage
				PhysHurtInfo.HurtType = HT_IMPACT
			}
			PhysHurtInfo.damage = damage
			PhysHurtInfo.timer2 = maxtime	//cas po ktory budeme schopni zranovat
			SetEventMask(this, EV_TICK)
			PhysHurtInfo.flags = ClearFlags(this, TFL_PATHBLOCK)	//toto musime zrusit po dobu zranovania
		}
		else
		{
			if(PhysHurtInfo)
			{
				SetFlags(this, PhysHurtInfo.flags)
				delete PhysHurtInfo
				PhysHurtInfo = NULL
			}
			ClearEventMask(this, EV_TICK)
		}
	}

//-----------------------------------------------------------------
	void HurtEnemy(ShotStorage PhysHurtInfo);

//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		NumContacts++
		if(PhysHurtInfo && World.ManipEntity != this)	//zranuje
		{
			if(PhysHurtInfo.timer == 0)
			{
				if(other.ClassType == ClassTypeCreature)
				{
					vector Vel = dBodyGetLastVelocity(this)		//rychlost ziskana v tomto evente je nepresna pretoze to uz je po naraze!
					float VelLengthSq = VectorLengthSq(Vel)

					if(VelLengthSq > 39.0625)	//zaujimaju nas iba silne narazy s ohladom na hmotnost. 6.25 to je pri pouziti VectorLength()
					{
						PhysHurtInfo.target = other
						PhysHurtInfo.shooter = this
						PhysHurtInfo.from = c.Position
						PhysHurtInfo.timer = 1
						PhysHurtInfo.power = 10//HurtIndex / 100
						SetEventMask(this, EV_TICK)	//budeme zranovat iba urcitych intervaloch
					}
				}
			}
		}
		
		if(impactsounds)	//zvuky
		{
			if(other == World.Player)
				return

			float TargetFrame = g_FPS * MinImpactSoundsDelay	//zvuky nesmu hrat kratko po sebe
			int TF = LastSoundFrame + TargetFrame
			
			if(g_iViewFrame <= TF)
				return

			vector ImpNorm = c.GetNormalImpulse()
			vector VelBefore, VelAfter
			float VelLngBefore = c.GetRelativeVelocityBefore(VelBefore)
	
			float ImpNormLng = VectorLength(ImpNorm)
			ImpNormLng /= (float)mass
	
			float KE = dBodyGetLastKineticEnergy(this)
			float ke = dBodyGetKineticEnergy(this)
			int KinDiff = KE - ke
			
			if(VelLngBefore < 0)
				VelLngBefore = -VelLngBefore
	
			float MinNormLng = mass * 0.002
			float MinVelLngBefore = mass * 0.00675
	
			if(ImpNormLng > MinNormLng && VelLngBefore > MinVelLngBefore && KinDiff != 0/* && VelLng > 15*/)
			{
				int Mat1 = Surf2Mat(c.Flags1)	//material tohto body
				int Mat2 = Surf2Mat(c.Flags2)	//material body s ktorym sme sa zrazili.

				hsound ImpSnd = GetImpactSound(Mat1, Mat2)

				if(ImpSnd)
				{
					int ps = PlaySound(this, ImpSnd, SF_ONCE)
					float vol = VelLngBefore
					clamp vol<0, 1>
					LastSoundFrame = g_iViewFrame
					vol = LinearVol(vol)
					SetSoundVolume(ps, vol)
	
					if(other.ClassType != ClassTypeCreature)
					{
						//teoreticky by se to melo limitovat jen od urcite hlasitosti, ale z hlediska gameplaye
						//chce proste player odlakat potvoru hozenym predmetem a ne spekulovat, jak velky ramus
						//musi udelat, aby potvora zareagovala
						float diff = currenttime - LastNotifyTime
						if(diff > 2.0)
						{
							LastNotifyTime = currenttime
							NotifyNoises(this, GetMatrix(this, 3), NOISERANGE_PHYSICS, NOISE_PHYSICS)
						}
					}
				}
			}
		}
	}
	
//---------------------------------------------------------------------------------
	void EOnTick(class other, int extra)
	{
		if(PhysHurtInfo)	//iba entity ktore maju zranovat
		{
			if(PhysHurtInfo.timer > 0)	//odpocitavame delay pocas ktoreho nebudeme znovu zranovat
			{
				if(PhysHurtInfo.timer == 1)	//z contact eventu neni mozne zranovat potvoru pretoze ona by mohla zmenit svoje rigidbody a viedlo by to k padu
				{
					HurtEnemy(PhysHurtInfo)	
				}
				
				PhysHurtInfo.timer -= TICK_PERIOD

				if(PhysHurtInfo.timer <= 0)
					PhysHurtInfo.timer = 0
			}
			
			PhysHurtInfo.timer2 -= TICK_PERIOD
			
			if(PhysHurtInfo.timer2 <= 0)
			{
				PhysHurtInfo.timer2  = 0
				EnableHurting(false, 0, 0)	//zmaze sa PhysHurtInfo!
			}
		}
	}

//---------------------------------------------------------------------------------
	void EOnInit(class other, int extra)
	{
		if(weaponscale == 0)
			SetWeaponScale(1)

		if(impactsounds == 0)	//defaultne zvuky zapiname ale dedenym entitam nechavame moznost tento default zmenit (misc_model)
			impactsounds = 3		//znamena zapnuto defaultom takze podedena entita to ma moznost menit

		if(impactsounds == 2)	//user to chce touto hodnotou vypnut
			impactsounds = 0
	}

//---------------------------------------------------------------------------------
	void RigidBodyClass()
	{
		SetEventMask(this, EV_INIT)
		SetPickable(true)	//defaultne sa da zobrat vsetko
	}

//---------------------------------------------------------------------------------
	void ~RigidBodyClass()
	{
		if(PhysHurtInfo)
		{
			delete PhysHurtInfo
			PhysHurtInfo = NULL
		}
	}

//---------------------------------------------------------------------------------
	void RemoveBodyAndGeom()
	{
		if(rb)
		{
			dBodyDestroy(this)
			rb = false
		}
			
		if(geom)
		{
			dGeomDestroy(geom)
			geom = 0
		}
	}
}


//---------------------------------------------------------------------------------	
bool JointClass::FindBodies()
{
	if(!body1 && body2)
	{
		DPrint("Missing body1 parameter for joint named " + _name)
		ErrorNum++
		return false
	}

	if(body1 && !body2)
	{
		DPrint("Missing body2 parameter for joint named " + _name)
		ErrorNum++
		return false
	}

	Body1 = FindClass(body1)
	Body2 = FindClass(body2)
	
	if(!Body1)
	{
		DPrint("Missing entity from parameter body1 in joint named " + _name)
		ErrorNum++
		return false
	}

	if(!Body2)
	{
		DPrint("Missing entity from parameter body2 in joint named " + _name)
		ErrorNum++
		return false
	}

	if(Body1 == Body2)
	{
		DPrint("the same entities in parameter body1 and body2 in joint named " + _name)
		ErrorNum++
		return false
	}

	if(Body1 != World && !IsInherited(Body1, CRigidBodyClass))
	{
		DPrint("Bad entity type in parameter body1 in joint named " + _name)
		ErrorNum++
		return false
	}

	if(Body2 != World && !IsInherited(Body2, CRigidBodyClass))
	{
		DPrint("Bad entity type in parameter body2 in joint named " + _name)
		ErrorNum++
		return false
	}

	if(Body1 != World && !Body1.rb)
	{
		DPrint("missing physics body in entity from parameter body1 in joint named " + _name)
		ErrorNum++
		return false
	}

	if(Body2 != World && !Body2.rb)
	{
		DPrint("missing physics body in entity from parameter body2 in joint named " + _name)
		ErrorNum++
		return false
	}
	return true
}

//---------------------------------------------------------------------------------	
	void JointClass::EOnLoaded(ExtClass other, int extra)
	{
#ifdef EDITOR
		if(!World.EditorActive)	//v editore vytvarame jointy iba v playmode
		{
#endif
			if(FindBodies())
			{
				if(Create())
				{
					if(Body1 != World)	//staci jednemu body ale nie worldu
						Body1.AddJointDependence(this)
					else
						Body2.AddJointDependence(this)
				}
			}
#ifdef EDITOR
		}
#endif
	}
	
//--------------------------------------------------------------------------------------
	void joint_motor::WakeUpBodies()
	{
		if(Body1 != World && dBodyIsDynamic(Body1))
			Body1.ActivateBody(true)

		if(Body2 != World && dBodyIsDynamic(Body2))
			Body2.ActivateBody(true)		
	}


//=================================================================================
class FuncClass extends RigidBodyClass
{
	string   model
	int 		flags
	int		status
	float    angle
	vector   coords
	vector   mins, maxs
	vobject	MainObject

//----------------------------------------------------------------------------
	void SetStatus(int stat)
	{
		status = stat
	}

//-----------------------------------------------------------------
	int CanActivate(int fl, ExtClass other)
	{
		if(other.ClassType != ClassTypeCreature || status != 0)
			return ACT_DISABLE

		if(flags & DisableFuncFlag)
			return ACT_BLOCKED

		if(Display && Display.CanActivate(0, this) != ACT_ENABLE)
			return ACT_DISABLE

		return ACT_ENABLE
	}

//-----------------------------------------------------------------
	void M_Disable(ExtClass other)
	{
		flags |= DisableFuncFlag
	}

//-----------------------------------------------------------------
	void M_Enable(ExtClass other)
	{
		flags &= 0xffffffff - DisableFuncFlag
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		ExtClass pl = World.Player

		if(!pl)
			return 0

		if(Owner == NULL)
			pl.AddItem(this)
		return 0
	}

//-----------------------------------------------------------------
	void FuncClass()
	{
		ClassType = ClassTypeFunc
		SetEventMask(this, EV_LOADED | EV_INIT/* | EV_ONGROUND*/)
	}
}


//=================================================================================
class ItemClass extends RigidBodyClass
{
	int		flags
	string   name
	vector	coords
	float		angle
	float		angle2
	float		angle3
	float		pickdelay
	string	OnPick
	int		Material	//pre secne zbrane
	vobject  MainObject
	ItemDef	Definition

#ifdef EDITOR
	void EditorOpened()
	{
		ActivateBody(false)
		coords = atov(GetKeyValue("coords"))
		angle = atof(GetKeyValue("angle"))
		angle2 = atof(GetKeyValue("angle2"))
		angle3 = atof(GetKeyValue("angle3"))
		SetTransformEx(Vector(angle3, angle, angle2), coords)
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

//---------------------------------------------------------------------------------------------------------
	void PlayFlySound()
	{
	}
	
//-----------------------------------------------------------------
	int Pick(MinClass owner)
	{
		ClearFlags(this, TFL_VISIBLE)
		Owner = owner
	}

//-----------------------------------------------------------------
	void Select()
	{
		SetFlags(this, TFL_VISIBLE)
	}

//-----------------------------------------------------------------
	void Unselect()
	{
		ClearFlags(this, TFL_VISIBLE)
	}

//-----------------------------------------------------------------
//virtualni, implementovano az v konkretnich classech
//vraci true pokud item akci povoli
	int Action(int act, vector target)
	{
		return false
	}

//-----------------------------------------------------------------
	void Activate(int fl, MinClass other)
	{
		Pick(other)
	}

//-----------------------------------------------------------------
	void EOnContact(MinClass other, Contact c)
	{
		if(other == World.Player)
		{
			if(CanActivate(0, other) == ACT_ENABLE)	//predmety zbierame aj dotykom
			{
				if(Owner == NULL && Definition.pickable)	//Definition.pickable znamena ze sa ma rozletiet k playerovi.
				{
					ExtClass pl = World.Player
					pl.AddItem(this)
				}
			}
		}
	}

//-----------------------------------------------------------------
	int EOnVisible(MinClass other, int extra)
	{
		ExtClass pl = World.Player

		if(!pl)
			return 0

		//don't add owned items
		if(Owner == NULL && Definition.pickable)	//Definition.pickable znamena ze sa ma rozletiet k playerovi.
			pl.AddItem(this)

		return 0
	}

//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		if(rb)
			return

		if(status == 1)	//delay pred rozbehom k playerovi
		{
			pickdelay -= ftime * 100
			
			if(pickdelay <= 0)
			{
				status = 2
				PlayFlySound()
			}
			else
				return
		}

		if(status == 2)
		{
			vector PlaPos = GetMatrix(World.Player, 3)
			vector pos = GetMatrix(this, 3)
			vector dir = PlaPos - pos
			float lng = VectorNormalize(dir)
			float step = lng * 20 * ftime
			float PlaVelFactor = VectorLength(GetVelocity(World.Player)) * 0.01
			
			if(PlaVelFactor < 1)
				PlaVelFactor = 1
				
			step *= PlaVelFactor
			vector NewPos = dir * step
			NewPos = pos + NewPos
			
			vector mat[4]
			GetMatrix4(this, mat)
			mat[3] = NewPos
			SetTransform(mat)
	//		SetOrigin(this, NewPos)

			if(lng < 5 || step > lng)
			{
				ClearEventMask(this, EV_FRAME)
				status = 3

				if(IsInherited(this, CWeaponClass))
				{
					PlaySound(NULL, g_sCommonSounds[33], SF_ONCE|SF_AMBIENT)
		
					if(Definition)
						World.InfDialog.ShowMessage("#You_picked " + Definition.name)
				}

				World.NumFlyingItems--
				Pick(World.Player)	//FIXME nejaky bug. jakykolvek kod za volanim tohto sa uz nevykona
			}
		}
	}

//-----------------------------------------------------------------
	void M_Pick(ExtClass other)
	{
		if(!Owner && status == 0)
		{
			if(CanActivate(0, World.Player))
				Activate(0, World.Player)
		}
	}

//-----------------------------------------------------------------
	void EOnLoaded(MinClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}

//-----------------------------------------------------------------
	void ItemClass()
	{
		ClassType = ClassTypeItem
		
		if(ActivateMethod == ACT_ENTITY_DEFAULT)
			ActivateMethod = ACT_DISTANCE_ONLY

		SetEventMask(this, EV_VISIBLE|EV_ANIMEND|EV_LOADED|EV_ONLOAD)
	}

//-----------------------------------------------------------------
	void ~ItemClass()
	{
		RemoveBodyAndGeom()

		if(MainObject)		//objekty itemov sa mazu v ItemsColletion (s vynimkou zbrani)
		{
			if(IsInherited(this, CWeaponClass) == true)
				ReleaseObject(MainObject, 0)

			MainObject = VNULL
		}
	}
}

