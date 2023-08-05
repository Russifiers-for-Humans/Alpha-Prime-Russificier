
//====================================================================
class PrefabEnt
{
	string	ID
	string	ClassName
	string	PrefabKeys[64]
	string	PrefabKeyValues[64]
	int	 	NumPrefabKeys

//--------------------------------------------------------------------
	string GetPrefabKeyValue(string key)
	{
		if(!key)
			return ""

		for(int n = 0; n < NumPrefabKeys; n++)
		{
			if(key == PrefabKeys[n])
				return PrefabKeyValues[n]
		}
		return ""
	}

//--------------------------------------------------------------------
	bool AddPrefabKey(string key, string value)
	{
		if(!key)
			return false

		if(NumPrefabKeys >= sizeof(PrefabKeys))
			return false

		PrefabKeys[NumPrefabKeys] = key
		PrefabKeyValues[NumPrefabKeys++] = value
		return true
	}

//--------------------------------------------------------------------
	bool SetPrefabKeyValue(string key, string value)
	{
		if(!key)
			return false
			
		for(int n = 0; n < NumPrefabKeys; n++)
		{
			if(key == PrefabKeys[n])
			{
				PrefabKeyValues[n] = value
				return true
			}
		}
		return false
	}
}

//====================================================================
class Prefab
{
	string		ID
	PrefabEnt	PrefabEntities[32]
	int	 		NumPrefabEntities

//--------------------------------------------------------------------
	PrefabEnt GetPrefabEnt(string id)
	{
		PrefabEnt current
		for(int n = 0; n < NumPrefabEntities; n++)
		{
			current = PrefabEntities[n]

			if(id == current.ID)
				return PrefabEntities[n]
		}
		return NULL
	}

//--------------------------------------------------------------------
	bool AddPrefabEnt(PrefabEnt ent)
	{
		if(NumPrefabEntities >= sizeof(PrefabEntities))
			return false
			
		PrefabEntities[NumPrefabEntities++] = ent
		return true
	}

//--------------------------------------------------------------------	
	bool RemovePrefabEnt(PrefabEnt ent)
	{
		Prefab current
		for(int n = 0; n < NumPrefabEntities; n++)
		{
			current = PrefabEntities[n]
			if(current == ent)
			{
				delete PrefabEntities[n]
				PrefabEntities[n] = NULL
				
				for(int k = n; k < NumPrefabEntities; k++)
					PrefabEntities[k] = PrefabEntities[k + 1]
				
				PrefabEntities[NumPrefabEntities--] = NULL
				return true
			}
		}
		return false
	}

//--------------------------------------------------------------------
	void ~Prefab()
	{
		for(int n = 0; n < NumPrefabEntities; n++)
		{
			delete PrefabEntities[n]
			PrefabEntities[n] = NULL
		}
	}
}

//====================================================================
class PrefabManager
{
	Prefab	Prefabs[512]
	int	 	NumPrefabs
	string	PrefabFile

//--------------------------------------------------------------------
	Prefab GetPrefab(string id)
	{
		Prefab current
		for(int n = 0; n < NumPrefabs; n++)
		{
			current = Prefabs[n]

			if(id == current.ID)
				return Prefabs[n]
		}
		return NULL
	}

//--------------------------------------------------------------------	
	bool AddPrefab(Prefab pref)
	{
		if(NumPrefabs >= sizeof(Prefabs))
			return false
		
		if(pref == NULL)
			return false

		Prefabs[NumPrefabs++] = pref
		return true
	}

//--------------------------------------------------------------------	
	bool RemovePrefab(Prefab pref)
	{
		if(pref == NULL)
			return false

		Prefab current
		for(int n = 0; n < NumPrefabs; n++)
		{
			current = Prefabs[n]
			if(current == pref)
			{
				delete Prefabs[n]
				Prefabs[n] = NULL
				
				for(int k = n; k < NumPrefabs; k++)
					Prefabs[k] = Prefabs[k + 1]
				
				Prefabs[NumPrefabs--] = NULL
				return true
			}
		}
		return false
	}

//--------------------------------------------------------------------
	bool LoadPrefabs()
	{
		int n = 0
		string tokens[30]
		int	 NumTokens
		string uv = tostr(34)
		tparse tp = BeginParse(PrefabFile)
		Prefab		CurPrefab
		PrefabEnt	CurPrefEnt
		string		key
		string		val

		if(!tp)
		{
			string msg = "No found file " + PrefabFile
			Print(msg)
			return false
		}

		NumTokens = ParseLine(tp, n++, tokens)

		while(NumTokens)
		{
			switch(tokens[0])
			{
				case "$prefab":
					CurPrefab = new Prefab
					CurPrefab.ID = tokens[1]
					AddPrefab(CurPrefab)
		  		break

				case "$ent":
					if(CurPrefab)
					{
						CurPrefEnt = new PrefabEnt
						CurPrefEnt.ClassName = tokens[1]
						CurPrefEnt.ID = tokens[2]
						CurPrefab.AddPrefabEnt(CurPrefEnt)
					}
		  		break

				case "{":
					n = n
				break

				case "}":
					if(CurPrefEnt)
						CurPrefEnt = NULL
					else
						CurPrefab = NULL
				break

				default:
					if(CurPrefEnt && tokens[1] == "=")
					{
						key = tokens[0]
						val = RemoveQuotes(tokens[2])
						CurPrefEnt.AddPrefabKey(key, val)
					}
				break
			}

			tokens[0] = ""
			tokens[1] = ""
			tokens[2] = ""
			NumTokens = ParseLine(tp, n++, tokens)
		}

		EndParse(tp)
		return true
	}

//--------------------------------------------------------------------	
	bool SavePrefabs()
	{
/*		string fname = PrefabFile
		string nfname = substr(fname, 0, strlen(fname) - 4)
		CopyFileA(fname, nfname + ".bak", false)*/

		int f = OpenFile(PrefabFile, FILEMODE_WRITE) 
		string line
		string uv = tostr(34)	//uvodzovky
		string val
		string key
		bool Defined
		string UserEv, UserEvVal
		Prefab CurPref
		PrefabEnt CurPrefEnt
		string spacer1 = "	"
		string spacer2 = "		"
		
		if(!f)
			return false

		for(int n = 0; n < NumPrefabs; n++)
		{
			CurPref = Prefabs[n]

			FPrintln(f, "")
			line = "//--------------------------------------------------------------------"
			FPrintln(f, line)
			line = "$prefab " + CurPref.ID
			FPrintln(f, line)
			line = "{"
			FPrintln(f, line)

			for(int j = 0; j < CurPref.NumPrefabEntities; j++)
			{
				CurPrefEnt = CurPref.PrefabEntities[j]

				line = spacer1 + "$ent " + CurPrefEnt.ClassName + " " + CurPrefEnt.ID
				FPrintln(f, line)
				line = spacer1 + "{"
				FPrintln(f, line)

				for(int k = 0; k < CurPrefEnt.NumPrefabKeys; k++)
				{
					key = CurPrefEnt.PrefabKeys[k]
					val = CurPrefEnt.PrefabKeyValues[k]
					
					if(key != "prefab")
					{
						line = spacer2 + key + " = " + uv + val + uv
						FPrintln(f, line)
					}
				}

				line = spacer1 + "}"
				FPrintln(f, line)
				
				int last = CurPref.NumPrefabEntities - 1

				if(j < last)
					FPrintln(f, "") 
			}

			line = "}"
			FPrintln(f, line)
		}
		CloseFile(f)
		return true
	}

//--------------------------------------------------------------------
	void PrefabManager()
	{
		PrefabFile = GetFilePath("scripts/entities.prf")		
		LoadPrefabs()
	}
	
//--------------------------------------------------------------------
	void ~PrefabManager()
	{
		for(int n = 0; n < NumPrefabs; n++)
		{
			delete Prefabs[n]
			Prefabs[n] = NULL
		}
	}
}

PrefabManager PrefManager




//---------------------------------------------------------------------
string GetValueFromKey(ExtClass ent, string key)
{
	for(int n = 0; n < ent.DefPtr.NumKeys; n++)
	{
		if(ent.DefPtr.Keys[n] == key)
			return ent.EKeyValues[n]
	}
	
	return ""
}

//---------------------------------------------------------------------
ExtClass GetEntityPointerFromName(string entname)
{
	ExtClass ec

	if(entname == "player")
		return World.Player

	if(substr(entname, 0, strlen(entname) - 1) == "info_player_start")
		return World.Player
		
	for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
	{
		if(ec._name == entname)
			return ec
	}
	
	return NULL
}

//---------------------------------------------------------------------
void AddDefinedKeysToEntity(ExtClass ActEnt)
{
	if(!ActEnt)
		return

	ActEnt.DefPtr = DefManager.GetDefinition(ClassName(ActEnt))
	
	if(!ActEnt.DefPtr)
	{
		string ErrMsg = "Unknown entity type " + ClassName(ActEnt) + ". Missing definition in entities.def?"
		DPrint(ErrMsg)
		RequestDialog(ErrMsg, "Error", GUI_DIALOG_OK )	
	}
}

void AddKeyValueToEntity(ExtClass ActEnt, string key, string value)
{
	for(int n = 0; n < ActEnt.DefPtr.NumKeys; n++)
	{
		if(ActEnt.DefPtr.Keys[n] == key)
		{
			ActEnt.EKeyValues[n]	= value
			return
		}
	}
}



//#include "scripts/editor2.h"
string PrefabsList[2048]		//temp pole koli sortovaniu a predavaniu win GUIicku
string MaterialNames[2048]
int	NumMaterials = 0

string CopyKeyValues[16][70]
int	 CopyKeyTypes[16][70]
string CopyClassName[16]
string CopyName[16]
EntityDef CopyDefPtr[16]
int	 NumCopyValues[16]
int	 NumCopiedEntities
vector CopyOffsets[16]
string CopyStates[16]

void ParseMaterials()
{
	if(NumMaterials)
		return
		
	int tp = BeginParse("shaders/shaders.s")
	
	if(tp)
	{
		string tokens[64]
		int line = 0
		
		while(ParseLine(tp, line++, tokens))
		{
			if(tokens[0] == "shader")
			{
				if(strlen(tokens[1]) > 2)
					MaterialNames[NumMaterials++] = tokens[1]
			}
		}
		
		EndParse(tp)
	}
	
	Sort(MaterialNames, NumMaterials)
}

//----------------------------------------------------------------------------------
int FindMaterialName(string mat)
{
	for(int index = 0; index < NumMaterials; index++)
	{
		if(MaterialNames[index] == mat)
			return index
	}
	
	return 0
}

string remap[16][2]
int numremap

//----------------------------------------------------------------------------------
void AddOrReplaceRemap(string mat, string rmat)
{
	for(int n = 0; n < numremap; n++)
	{
		if(remap[n][0] == mat)
		{
			remap[n][1] = rmat
			return
		}
	}
	
	remap[numremap][0] = mat
	remap[numremap][1] = rmat
	numremap++
}

//----------------------------------------------------------------------------------
string ParseRemapping(string rem)
{
	numremap = 0
	
	while(true)
	{
		int p = strstr(rem, "$remap")
		if(p == -1)
			break
		
		rem = substr(rem, p + 6, strlen(rem) - p - 6)
		rem = trim(rem)

		p = strstr(rem, "'")
		if(p == -1)
			break

		rem = substr(rem, p + 1, strlen(rem) - 1)

		p = strstr(rem, "'")
		if(p == -1)
			break
		
		string mat = substr(rem, 0, p)

		rem = trim(substr(rem, p + 1, strlen(rem) - p - 1))

		p = strstr(rem, "'")
		if(p == -1)
			break

		rem = substr(rem, p + 1, strlen(rem) - 1)

		p = strstr(rem, "'")
		if(p == -1)
			break
		
		string rmat = substr(rem, 0, p)		
		
		AddOrReplaceRemap(mat, rmat)
	}
}


//----------------------------------------------------------------------------------
string UnparseRemap()
{
	string result = ""
	
	for(int n = 0; n < numremap; n++)
	{
		if(remap[n][0] != remap[n][1])
		{
			result += "$remap '" + remap[n][0] + "' '" + remap[n][1] + "';"
		}
	}
	return result
}

//----------------------------------------------------------------------------------
string GetRemap(string mat)
{
	for(int n = 0; n < numremap; n++)
	{
		if(remap[n][0] == mat)
			return remap[n][1]
	}
	
	return mat
}

void SetVariableTypes(ExtClass EntVar)
{
	int in
	float fl
	vector vec
	string str
	string ss, vv
	int Typ

	for(int k = 0; k < EntVar.DefPtr.NumKeys; k++)
	{
		Typ = 0
		ss = EntVar.EKeyValues[k]	//ActiveLbox.LinesTexts[k][1]
		vv = EntVar.DefPtr.Keys[k]

		if(SetValue(EntVar, vv, 0, in))	//je to integer
		{
			if(SetVar(in, ss))
				Typ = 1
			else
				Typ = 11
		}
		else
		{
			if(SetValue(EntVar, vv, 0, fl))	//je to float
			{
				if(SetVar(fl, ss))
					Typ = 2
				else
					Typ = 12
			}
			else
			{
				if(SetValue(EntVar, vv, 0, vec))	//je to vector
				{
					if(SetVar(vec, ss))
						Typ = 3
					else
						Typ = 13
				}
				else
					Typ = 4									//je to string
			}
		}
		
		if(Typ > 10)	//bola zadana blbost
		{
			Typ -= 10
//								ActiveLbox.SetLineText(k, 1, EditLineBefore)	//toto je treba nastavit ked bude spolahlivo fungovat kontrola u if(SetVar(fl, ss))
		}
		
		EntVar.EKeyTypes[k] = Typ	//zapametame si datovy typ kluca
	}	
}





//----------------------------------------------------------------------------------
string 		BackUpKeyValues[70]
int 	 		BKeyTypes[70]
EntityDef 	BDef

void BackUpKeys(ExtClass ent)
{
	int n
	BDef = ent.DefPtr

	for(n = 0; n < ent.DefPtr.NumKeys; n++)
		BackUpKeyValues[n] = ent.EKeyValues[n]

	for(n = 0; n < ent.DefPtr.NumKeys; n++)
		BKeyTypes[n] = ent.EKeyTypes[n]
}

void RestoreKeys(ExtClass ent)
{
	int n
	ent.DefPtr = BDef

	for(n = 0; n < ent.DefPtr.NumKeys; n++)
		ent.EKeyValues[n] = BackUpKeyValues[n]
		
	for(n = 0; n < ent.DefPtr.NumKeys; n++)
		ent.EKeyTypes[n] = BKeyTypes[n]
}

void ThrowEvent(local ExtClass ent, local int Event)
{
	int mask = GetEventMask(ent)

	if(mask & Event)
		Throw(ent, World, Event, 0)

	if(Event == EV_LOADED)
	{
		ent.CreateEditorVisuals()
		UpdateEntity(ent)
		ent.AfterReloadInEditor()
	}
}


string ChildEntNames[64]
int	 ChildEntNamesNum

void GrabChildsNamesFrom(local ExtClass ent)
{
	local int n
	local ExtClass Child

	for(n = 0; n < ent.ChildsNamesNum; n++)
	{
		Child = FindClass(ent.ChildsNames[n])
		
		if(Child)
		{
			ChildEntNames[ChildEntNamesNum++] = Child._name
			GrabChildsNamesFrom(Child)
		}
	}
}

void SavePositionsOfChilds(local ExtClass ent)
{
	local int n
	local ExtClass Child

	for(n = 0; n < ent.ChildsNamesNum; n++)
	{
		Child = FindClass(ent.ChildsNames[n])
		
		if(Child)
		{
/*			local ExtClass root = Child.FindParentRoot()	//musime updatovat zhora
			
			if(root)
				root.RefreshInEditor()	//namiesto UpdateEntity(this)
			else
				Child.RefreshInEditor()*/

			UpdateEntity(Child)
			AddKeyValueToEntity(Child, "coords", vectoa(GetMatrix(Child, 3)))
			SavePositionsOfChilds(Child)
		}
	}
}

void SetEntityOrientationKeys(ExtClass ent, vector Coords, vector Angles)
{
	int k
	
	if(!ent)
		return

	for(k = 0; k < ent.DefPtr.NumKeys; k++)
	{
		
		if(ent.DefPtr.Keys[k] == "coords")
		{
			ent.EKeyValues[k] = vectoa(Coords)
			ent.EKeyTypes[k] = 3
		}

		float rr
		int tt
		if(ent.DefPtr.Keys[k] == "angle3")
		{
			rr = Angles[0]
			tt = rr
			ent.EKeyValues[k] = itoa(tt)
			ent.EKeyTypes[k] = 1
		}
		
		if(ent.DefPtr.Keys[k] == "angle")
		{
			rr = Angles[1]
			tt = rr
			ent.EKeyValues[k] = itoa(tt)
			ent.EKeyTypes[k] = 1
		}
		
		if(ent.DefPtr.Keys[k] == "angle2")
		{
			rr = Angles[2]
			tt = rr
			ent.EKeyValues[k] = itoa(tt)
			ent.EKeyTypes[k] = 1
		}
	}
}

void SetEntitySizeKeys(ExtClass ent, vector Mins, vector Maxs)
{
	int k
	
	if(!ent)
		return

	for(k = 0; k < ent.DefPtr.NumKeys; k++)
	{
		if(ent.DefPtr.Keys[k] == "mins")
		{
			ent.EKeyValues[k] = vectoa(Mins)
			ent.EKeyTypes[k] = 3
		}
		
		if(ent.DefPtr.Keys[k] == "maxs")
		{
			ent.EKeyValues[k] = vectoa(Maxs)
			ent.EKeyTypes[k] = 3
		}
	}
}

bool IsNumber(string str)
{
	if(str == "0" || str == "1" || str == "2" ||	str == "3" || str == "4")
		return true
		
	if(str == "5" || str == "6" || str == "7" || str == "8" ||	str == "9")
		return true
			
	return false
}

string GetIncrementNameForNew(string oldname)
{
	int IncNumbers[1024]
	int IncNumbersNum = 0
				
	string tmp
	int pos = 0
	int spos = 0
	int lng,lng2, n

	ExtClass ec
	
	for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
	{
		if(ec.DefPtr && ec._name)
		{
			spos = strstr(ec._name, oldname)
			lng = strlen(oldname)
			lng2 = strlen(ec._name)
	
			if(spos != -1 && lng2 > lng)	//ak nase meno obsahuje ina entita tak ostatne znaky musia byt iba incrementy
			{
				int len = lng2 - lng
				bool Increment = true

				for(int i = 0; i < len; i++)
				{
					string st = substr(ec._name, lng + i, 1)
					if(IsNumber(st) == false)
					{
						Increment = false
						break
					}
				}

				if(Increment)
				{
					lng2 = lng2 - lng
					
					tmp = substr(ec._name, lng, lng2)
					lng = atoi(tmp)
					IncNumbers[IncNumbersNum++] = lng
				}
			}
		}
	}
/*
	if(IncNumbersNum == 0)	//v levele sme taky nazov nenasli takze v tom pripade vyhodime pripadne cisla z konca
	{
		int OldNameLng = strlen(oldname)
		string LastChar = substr(oldname, OldNameLng - 1, 1)

		while(OldNameLng >= 1 && IsNumber(LastChar) == true) 
		{
			oldname = substr(oldname, 0, OldNameLng - 1)
			OldNameLng = strlen(oldname)

			if(OldNameLng >= 1)
				LastChar = substr(oldname, OldNameLng - 1, 1)
			else
				LastChar = "NoNumberOnEnd"
		}
	}
*/
	int HNumber = -1
	
	for(n = 0; n < IncNumbersNum; n++)
	{
		if(IncNumbers[n] > HNumber)
			HNumber = IncNumbers[n]
	}

	string newname = oldname + itoa(++HNumber)

	return newname
}

void ShowDuplicityError()
{
	string ErrMsg2 = "Entita s timhle nazvem uz existuje !!!"
	RequestDialog(ErrMsg2, "Error", GUI_DIALOG_OK )	
}

bool DuplicityTestOK(string newname)
{
	ExtClass ec
	
	for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
	{
		if(ec._name == newname)
			return false
	}
	
	return true
}






#include "scripts/editor_links.h"


string Submenu[512]
int 	 EntPopUpID[512]
int	 NumIDs = 0
string IDnames[512] 

const int ST_SELECTING = 1			//selektime entity
const int ST_EDITCOORDS = 2		//menime pozicie entit

string SndShadNames[512]
int	 SndShadNamesNum
int	 g_NumEditors					//pocet instancii

class EntityEditorTempClass extends PathEditor/*MinClass*/
{
	ExtClass NewEntity(string NewClName, vector pos)
	{
	}
	
	void Hide(ExtClass other, bool HideStat)
	{
	}
}

		
class EntityEditor extends EntityEditorTempClass
{
	bool AreasVisible
	bool LightsVisible
	bool ClipsVisible

	int Status
	ExtClass Ent			//poiner pri selectovani
	string EditLine
	vector OldOrigin
	string LastCreated	//ClassName poslednej vytvorenej entity
	UIMenu PopupMenu
	UIMenu PopSubmenus[32]
	UIMenu PopMenuHide
	vector PosForNewEntity
	vector LastEntityCoords
	TraceContact SelectingContact

	vector g_ValMins, g_ValMaxs
	vector g_AngleVal
	vector g_NewEntityCoords
	vector MovingOffset[32]			//offsety ostatnych selektnutych entit k hlavnej
	vector AnglesBeforeChange		//uhly hlavnej entity pred ich zmenou
	string NameForNew
	//=================================================================================
	ExtClass MainSel
	ExtClass Selected[17]	//o 1 viac koli procesu v AddSelection
	int		SelectedNum

	void RemoveSelection(ExtClass ent)
	{
		if(!ent)
			return
	
		int n
		int index = -1
	
		for(n = 0; n < SelectedNum; n++)
		{
			if(Selected[n] == ent)
			{
				index = n
				Selected[n] = NULL
				SelectedNum--
				break
			}
		}
		
		if(index == -1)
		{
	//		debug	//toto by sa nemalo nikdy stat	
			MainSel = Selected[0]
			return
		}
		
		for(n = index; n < SelectedNum; n++)
		{
			Selected[n] = Selected[n + 1]
			Selected[n + 1] = NULL
		}
	
		MainSel = Selected[0]
	}
	
	void AddSelection(ExtClass ent)
	{
		if(!ent)
			return
	
		if(SelectedNum > 15)
			return
	
		int n
		for(n = 0; n < SelectedNum; n++)	//mohlo by to tam uz byt
		{
			if(Selected[n] == ent)
			{
				RemoveSelection(ent)
				break
			}
		}
	
		for(n = SelectedNum; n > 0; n--)
			Selected[n] = Selected[n - 1]
	
		Selected[0] = ent
		SelectedNum++
		MainSel = Selected[0]
	}
	
	void UnselectAll()
	{
		for(int n = 0; n < SelectedNum; n++)
			Selected[n] = NULL
	
		SelectedNum = 0
		MainSel = Selected[0]
	}
	
	void ReplaceSelection(ExtClass Old, ExtClass New)
	{
		if(!Old || !New)
			return
	
		int n
		for(n = 0; n < SelectedNum; n++)	//mohlo by to tam uz byt
		{
			if(Selected[n] == Old)
			{
				Selected[n] = New
				MainSel = Selected[0]
				break
			}
		}
	}
	
	ExtClass GetSelected(int index)
	{
		return Selected[index]
	}
	
	bool IsSelected(ExtClass ent)
	{
		for(int n = 0; n < SelectedNum; n++)
		{
			if(Selected[n] == ent)
				return true
		}
		return false
	}

	//----------------------------------------------------------------------------------
	ExtClass ReloadEntity(FuncClass ent, string NewClassName)
	{
		SetVariableTypes(ent)
		
		string tmp
	
		string model				; GetValue(ent, "model", 0, model)
		string _name				; GetValue(ent, "_name", 0, _name)
		
		string _states = ent._userstates
		string _events = ent._userevents
		string _event0 = ent._userevent0
		string _event1 = ent._userevent1
		string _event2 = ent._userevent2
		string _event3 = ent._userevent3
	
		BackUpKeys(ent)
		
		string classname = NewClassName//ClassName(ent)
		if(IsInherited(ent, CFuncClass) && ent.Parent)
		{
			FuncClass ent2 = ent.Parent
			ent2.Parent = NULL
			ent.Parent = NULL
		}

		ExtClass OldPtr = ent
	
		if(ent != World)
		{
			delete ent
			ent = Spawn(Type(classname))
		}

		if(Ent == OldPtr)	//mohlo by to byt pod kurzorom
			Ent = ent
	
		SetValue(ent, "model", 0, model)
		//SetValue(ent, "_name", 0, _name)
		ent.SetName(_name)
	
		ent._userstates = _states
		ent._userevents = _events
		ent._userevent0 = _event0
		ent._userevent1 = _event1
		ent._userevent2 = _event2
		ent._userevent3 = _event3
	
		RestoreKeys(ent)
		
		string key, value
		vector coords
	
		for(int m = 0; m < ent.DefPtr.NumKeys; m++)
		{
			value = ent.EKeyValues[m]
			key = ent.DefPtr.Keys[m]
			
			if(key == "coords")
				coords = atov(value)
	
			switch(ent.EKeyTypes[m])
			{
			case 1:
				int In
				SetValue(ent, key, 0, atoi(value))
				break
	
			case 2:
				float Fl
				SetValue(ent, key, 0, atof(value))
				break
	
			case 3:
				int Vc
				SetValue(ent, key, 0, atov(value))
				break
	
			case 4:
				int St
				SetValue(ent, key, 0, value)
				break
			}
		}
		return ent
	}

	void ReloadEntitiesFromList(string names[], int NumEntities)
	{
		ExtClass Child
		ExtClass Reloaded
		int n
		
		for(n = 0; n < NumEntities; n++)
		{
			Child = FindClass(names[n])
			
			if(Child)
			{
				Reloaded = ReloadEntity(Child, ClassName(Child))
				ReplaceSelection(Child, Reloaded)	//keby to bolo nahodou selektnute tak zmeni pointer v Selected[]
			}
		}
	
		for(n = 0; n < NumEntities; n++)
		{
			Child = FindClass(names[n])
	
			if(Child)
				ThrowEvent(Child, EV_INIT)
		}
	
		for(n = 0; n < NumEntities; n++)
		{
			Child = FindClass(names[n])
			
			if(Child)
				ThrowEvent(Child, EV_LOADED)
		}

/*		for(n = 0; n < NumEntities; n++)
		{
			Child = FindClass(names[n])
			
			if(Child && Child.GetParent() != NULL)
			{
				ExtClass root = Child.FindParentRoot()	//musime updatovat zhora
				
				if(root)
					root.RefreshInEditor()	//namiesto UpdateEntity(this)
				else
					Child.RefreshInEditor()
				
				Child.UpdateMainVisualizer()
			}
		}*/
	}

	void LoadEntKeysFromEntFile()
	{
		int n = 0
		string tokens[30]
		string uv = tostr(34)
		ExtClass ActEnt
		
		string EntFile = World.ThisLevelDir + World.ThisLevelName + ".ent"//substr(MapPath, 0, strlen(MapPath) - 3) + "ent"
		tparse tp = BeginParse(EntFile)
		n = 0

		if(!tp)
		{
			string msg = "Not found file " + EntFile
			Print(msg)
			return
		}

		while(ParseLine(tp, n++, tokens))
		{
			switch(tokens[0])
			{
				case "$ent":
				ActEnt = GetEntityPointerFromName(tokens[2])	//najdeme si entitu s vyparsovanym menom
	
				if(ActEnt)
					AddDefinedKeysToEntity(ActEnt)
				
		  		break
	
				default:
				string val = tokens[2]
				string key = tokens[0]
	
				if(substr(val, 0, 1) == tostr(34))
					val = substr(val, 1, strlen(val) - 2)
	
				if(ActEnt)
					AddKeyValueToEntity(ActEnt, key, val)
				break
			}
			
			for(int k = 0; k < sizeof(tokens); k++)
				tokens[k] = ""
		}
		EndParse(tp)
	}

	void SaveEntitiesKeys()
	{
		string EntFile = World.ThisLevelDir + World.ThisLevelName + ".ent"

		string fname = GetFilePath(EntFile)
		string nfname = substr(fname, 0, strlen(fname) - 4)
		CopyFileA(fname, nfname + ".bak", false)
	
		int f = OpenFile(EntFile, FILEMODE_WRITE)
		string line
		string uv = tostr(34)	//uvodzovky
		string val
		string key
		bool Defined
		string UserEv, UserEvVal
	
		if(!f)
		{
			RequestDialog("Cannot write to file " + EntFile, "Write error", GUI_DIALOG_OK )
			return
		}
	
		ExtClass ec
		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(!ec.DefPtr)	//nebola z ent suboru
				continue
				
			line = "$ent " + ClassName(ec) + " " + ec._name
			FPrintln(f, line)
	
			int k
			for(k = 0; k < ec.DefPtr.NumKeys; k++)
			{
				val = ec.EKeyValues[k]
				key = ec.DefPtr.Keys[k]
				Defined = true
	
				if(val == "")
					continue
	
				if(val == "0")
					continue
	
				if(val == "0 0 0")
					continue
	
				if(ec.DefPtr)	//je z ent suboru
				{
					for(int i = 0; i < 70; i++)
					{
						if(key == ec.DefPtr.Keys[i])
							Defined = false					//je v def subore
					}
				}
	
				if(Defined == true)
					continue
				
				if(val != "")
				{
					line = " " + key + " = " + uv + val + uv
					FPrintln(f, line)
				}
			}
	
			UserEvVal = ""
			GetValue(ec, "_userstates", 0, UserEvVal)
			
			if(UserEvVal != "")
			{
				UserEvVal = ReplaceChars(UserEvVal, ";", ",")

				line = " " + "_userstates" + " = " + uv + UserEvVal + uv
				FPrintln(f, line)
			}
	
			UserEvVal = ""
			GetValue(ec, "_userevents", 0, UserEvVal)
			
			if(UserEvVal != "")
			{
				line = " " + "_userevents" + " = " + uv + UserEvVal + uv
				FPrintln(f, line)
			}
	
			for(k = 0; k < 8; k++)
			{
				UserEv = "_userevent" + itoa(k)
				UserEvVal = ""
				
				GetValue(ec, UserEv, 0, UserEvVal)
				
				if(UserEvVal != "")
				{
					line = " " + UserEv + " = " + uv + UserEvVal + uv
					FPrintln(f, line)
				}
			}
	
			FPrintln(f, "")
		}
	
		CloseFile(f)

		Print(String("File " + EntFile + " is saved."))
	}

	ExtClass AddEntity(string classname, bool WriteToMap)
	{
		string _name = ""
		vector coords = g_NewEntityCoords
		int n
	
		ExtClass ent = Spawn(Type(classname))
		
		if(!ent)
		{
			RequestDialog("Entity type not exist. Bad name in entities.def?", "Entity spawn error", GUI_DIALOG_OK )
			return NULL
		}
		
		
	//	SetValue(ent, "model", 0, model)
		SetValue(ent, "_name", 0, _name)
		SetValue(ent, "coords", 0, coords)
	
		if(g_ValMins != ZeroVec)
		{
			SetValue(ent, "mins", 0, g_ValMins)
			SetValue(ent, "maxs", 0, g_ValMaxs)
		}
	
		if(WriteToMap)
		{
		//kontrolujeme ci ta entita je v .ent subore a ked ano tak zistime aky "_name" jej pridelime. inac return a nic sa nedeje
			if(DefManager.GetDefinition(classname))	
			{
				_name = classname + "0"
				int poc = 0
				
				bool Duplicity
				ExtClass ec
				
				while(Duplicity)
				{
					Duplicity = false

					for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
					{
						if(_name == ec._name)
						{
							poc++
							_name = classname + itoa(poc)
							Duplicity = true
						}
					}
				}				
			}
	
			if(_name == "")
			{
				string ErrMsg = "Entita " + classname + " neni nadefinovana v entities.def !"
				RequestDialog(ErrMsg, "Error", GUI_DIALOG_OK )
				return NULL
			}
	
			_name = NameForNew
			ent.SetName(_name)
			AddDefinedKeysToEntity(ent)
		}
	
		int mask = GetEventMask(ent)
		if(mask & EV_INIT)
			Throw(ent, World, EV_INIT, 0)
		
		mask = GetEventMask(ent)
		
		if(mask & EV_LOADED)	
			Throw(ent, World, EV_LOADED, 0)
	
		OnDataChange()
		return ent
	}

//-----------------------------------------------------------------
	void AddNamesToGUI(ExtClass entities[], int NumEntities)
	{
		ExtClass CurEnt
		string Names[64]
		string ClassNames[64]
		
		for(int n = 0; n < NumEntities; n++)
		{
			CurEnt = entities[n]
			Names[n] = CurEnt._name
			ClassNames[n] = ClassName(CurEnt)
		}
		EntityListAdd(Names, ClassNames, NumEntities)
	}
	
//-----------------------------------------------------------------
	void RemoveNamesFromGUI(ExtClass entities[], int NumEntities)
	{
		ExtClass CurEnt
		string Names[64]
		string ClassNames[64]
		
		for(int n = 0; n < NumEntities; n++)
		{
			CurEnt = entities[n]
			Names[n] = CurEnt._name
			ClassNames[n] = ClassName(CurEnt)
		}
		EntityListRemove(Names, ClassNames, NumEntities)
	}

	string FilterClassNames[256]
	int	 FilterClassNamesNum

//-----------------------------------------------------------------
	void UpdateEntitiesList()	//volane z GUI
	{
		string AllEntities[4096]
		string AllEntitiesClassNames[4096]
		int AllEntitiesNum = 0
		ExtClass ec
		string ClName

		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(ec.DefPtr)	//je z ent suboru
			{
				ClName = ClassName(ec)

				if(AllEntitiesNum < 4096)
				{
					AllEntities[AllEntitiesNum] = ec._name
					AllEntitiesClassNames[AllEntitiesNum++] = ClassName(ec)
				}
			}
		}
		EntityListAdd(AllEntities, AllEntitiesClassNames, AllEntitiesNum)
	}

	// ----------------------------------------------------------------------------
	void ShowCursorPernamently(bool status)
	{
		int cur;
	
		if(status == true)
		{
			cur = 0;
			while(cur <= 0)
				cur = ShowCursor(true);
		}
		else
		{
			cur = 1;
			while(cur >= 0)
				cur = ShowCursor(false);
		}
	}

//--------------------------------------------------------------------
	bool IsNotEditableKey(string key)
	{
		if(key == "prefab" || key == "_surfaces" || key == "PrefabsIncrement")	//toto neni pristupne (viditelne) v edit okne
			return true

		return false
	}

//--------------------------------------------------------------------
	bool IsPathKey(string key)
	{
		if(key == "model" || key == "display" || key == "anim" || key == "sound")
			return true

		return false
	}

	bool Update3DCursor()
	{
		if(EdMode == EDITMODE_PATH)
			return true

		return false
	}

	void ShowMiscModels(bool show)
	{
		int nn = g_iShowFilter
		
		if(show)
			nn |= 2
		else
			nn &= 0xffffffff - 2

		g_iShowFilter = nn
	}

	void ShowLights(bool show)
	{
		ExtClass ec
		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(ClassName(ec) == "light")
			{
				if(show)
					ec.CreateEditorVisuals()
				else
					ec.DestroyEditorVisuals()
			}
		}
	}

	bool GetAreasVis()
	{
		return AreasVisible
	}

	void ShowBBoxOfEntity(MinClass En, int color)
	{
		if(!En)
			return
			
		vector mins, maxs, org
		GetBoundBox(En, mins, maxs)
		org = GetMatrix(En, 3)
/*		
		vector Dmins, Dmaxs
		if(GetValue(En, "mins", 0, Dmins) && GetValue(En, "maxs", 0, Dmaxs))
		{
			if(Status == ST_EDITCOORDS)
			{
				mins = g_ValMins
				maxs = g_ValMaxs
			}
			else
			{
				mins = Dmins
				maxs = Dmaxs
			}
		}
*/		
		if(VectorLength(maxs - mins) < 1)
		{
			mins = "-8 -8 -8"
			maxs = "8 8 8"	
		}

		TriggerClass trig = NULL
		bool IsNonBrushTrigger = false
		bool ShapeType = 0
		
		if(IsInherited(En, CTriggerClass))
		{
			trig = En
			IsNonBrushTrigger = trig.IsDynamic
			ShapeType = trig.type
		}
		
		if(ClassName(En) == "clima_bugs")
			ShapeType = 0
		
		if(IsNonBrushTrigger || ClassName(En) == "clima_bugs")
		{
			GetValue(En, "mins", 0, mins)
			GetValue(En, "maxs", 0, maxs)
		}
				
		if(ShapeType == 0)
		{
			vector mat[4]
			mat[0] = GetMatrix(En, 0)
			mat[1] = GetMatrix(En, 1)
			mat[2] = GetMatrix(En, 2)
			mat[3] = GetMatrix(En, 3)

			Shape nn = AddDShape(SHAPE_BBOX, color, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, mins + "-0.1 -0.1 -0.1", maxs + "0.1 0.1 0.1")
			nn.SetMatrix(mat)
		}

		if(ShapeType == 1)
		{
			AddDCylinder(color, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, org, maxs[0], maxs[2] * 2)
		}

		AddDShape(SHAPE_BBOX, 0xff222222, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, org + "-0.2 -0.2 -0.2", org + "0.2 0.2 0.2")
	}


	void AddSubmenuItems(string buffer[], int BufferSize, int MenuIndex, string pref)
	{
		for(int n = 0; n < BufferSize; n++)
		{
			UIMenu mm = PopSubmenus[MenuIndex]
			string sss = buffer[n]
			
			if(strstr(sss, pref + "_") != -1)
			{
				mm.AddTextItem("&" + sss, 0x8000 + NumIDs)
				IDnames[NumIDs++] = sss
			}
		}
	}

	string GetNameFromPopup(string buffer[], int BufferSize, bool submenu, int Xpos, int Ypos)
	{
		int spos = 0
		string NewLine
		int LinesNum = 0
		int n
		bool Sub = false
		int NumSubMenus = 0
		NumIDs = 0

		PopupMenu = new UIMenu()
		
		for(n = 0; n < 32; n++)
			PopSubmenus[n] = new UIMenu()

		for(n = 0; n < sizeof(Submenu); n++)
			Submenu[n] = ""

		for(n = 0; n < BufferSize; n++)
		{
			if(buffer[n] == "world" || strstr(buffer[n], "Class") >= 0)
				continue

			Sub = false
			NewLine = ""
			spos = strstr(buffer[n], "_")
			
			if(spos != -1 && submenu)
			{
				NewLine = substr(buffer[n], 0, spos)
				Sub = true
			}
			else
				NewLine = buffer[n]
			

			bool add = true
			for(int k = 0; k < n; k++)
			{
				if(NewLine == Submenu[k])
					add = false
			}

			if(NewLine == "")
				add = false

			if(add)
			{
				if(Sub)
				{
					PopupMenu.AddSubMenu(PopSubmenus[NumSubMenus], NewLine)
					AddSubmenuItems(buffer, BufferSize, NumSubMenus++, NewLine)
				}
				else
				{
					PopupMenu.AddTextItem("&" + NewLine, 0x8000 + NumIDs)
					IDnames[NumIDs++] = NewLine
				}
				
				Submenu[LinesNum++] = NewLine
			}
		}

		int c = PopupMenu.IPopup(Xpos, Ypos)
		int res = c - 0x8000
		ClearKey(KC_ESCAPE)

		delete PopupMenu
		PopupMenu = NULL
		
		for(n = 0; n < 32; n++)
		{
			delete PopSubmenus[n]
			PopSubmenus[n] = NULL
		}

		RestetGUIInput()	//toto popup zapricinuje ze sa keyup u klaves uz nevykona takze musime vyclearovat

		if(res < 0 || res > NumIDs)	//nebolo nic vybrane
			return ""

		SetFocusToRenderWindow()
		return IDnames[res]	//vracia to na co bolo kliknute
	}

	bool FlagIsSet(int flg, int bit)
	{
		if(flg & bit)
			return true
		else
			return false
	}

/*	string GetKeyHelp(string key)
	{
		for(int n = 0; n < ent.DefPtr.NumKeys; n++)
		{
			if(key == MainSel.DefPtr.Keys[n])
				return DefEntKeyHelps[MainSel.EDefIndex][n]
		}
		return ""
	}

	string GetMethodHelp(string method)
	{
		for(int n = 0; n < DefEntNumMethods[MainSel.EDefIndex]; n++)
		{
			if(method == DefEntMethods[MainSel.EDefIndex][n])
				return DefEntMethodHelps[MainSel.EDefIndex][n]
		}
		return ""
	}*/

	bool IsEvent(ExtClass ent, string key)
	{
		for(int n = 0; n < ent.DefPtr.NumEvents; n++)
		{
			if(key == ent.DefPtr.Events[n])
				return true
		}
		return false
	}

	void MemSelectionOffsets(vector OffsetArray[])
	{
		vector MainSelMat[4]
		ExtClass OtherEnt
		vector OtherPos, OtherAng
		vector OldCoords

		AnglesBeforeChange[0] = atoi(GetValueFromKey(MainSel, "angle3"))
		AnglesBeforeChange[1] = atoi(GetValueFromKey(MainSel, "angle"))
		AnglesBeforeChange[2] = atoi(GetValueFromKey(MainSel, "angle2"))
		OldCoords = atov(GetValueFromKey(MainSel, "coords"))

		RollPitchYawMatrix2(AnglesBeforeChange, MainSelMat)	//matica pred zmenou
		MainSelMat[3] = OldCoords

		for(int n = 1; n < SelectedNum; n++)	//zo starej matice potrebujeme offsety aby sme ich potom previedli do novej matice
		{
			OtherEnt = Selected[n]
			vector off =  atov(GetValueFromKey(OtherEnt, "coords")) - OldCoords
			OffsetArray[n] = VectorMatrixInvMultiply3(MainSelMat, off)		//zapameame si offsety v objectspace hlavnej entity
		}
	}

	void TransformateSelectedEntities(vector OffsetArray[])
	{
		vector MainSelMat[4]
		ExtClass OtherEnt
		vector OtherPos, OtherAng

		g_AngleVal[0] = atoi(GetValueFromKey(MainSel, "angle3"))	//ziskame hodnoty do matice po zmene klucov
		g_AngleVal[1] = atoi(GetValueFromKey(MainSel, "angle"))
		g_AngleVal[2] = atoi(GetValueFromKey(MainSel, "angle2"))
		vector Coords = atov(GetValueFromKey(MainSel, "coords"))

		RollPitchYawMatrix2(g_AngleVal, MainSelMat)	//matica po zmene
		MainSelMat[3] = Coords

		for(int n = 1; n < SelectedNum; n++)		//hlavnej entite nemusime menit nic. ta to ma spravne naplnene z listboxu
		{
			OtherEnt = Selected[n]
			OtherPos = VectorMatrixMultiply3(MainSelMat, OffsetArray[n])
			OtherPos = Coords + OtherPos
			OtherAng[0] = atoi(GetValueFromKey(OtherEnt, "angle3"))
			OtherAng[1] = atoi(GetValueFromKey(OtherEnt, "angle"))
			OtherAng[2] = atoi(GetValueFromKey(OtherEnt, "angle2"))
			
			vector AngOff = g_AngleVal - AnglesBeforeChange
			AngOff = FixAngles(AngOff)

			OtherAng = AngOff + OtherAng
			OtherAng = FixAngles(OtherAng)
			
/*			string DirAngle = GetValueFromKey(OtherEnt, "dirangle")
			
			if(DirAngle)
			{
				OtherAng
			}*/

			SetEntityOrientationKeys(OtherEnt, OtherPos, OtherAng)
		}

		for(n = 1; n < SelectedNum; n++)
			ReplaceSelection(Selected[n], ReloadEntity(Selected[n], ClassName(Selected[n])))

		for(n = 1; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_INIT)

		for(n = 1; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_LOADED)
	}

//-----------------------------------------------------------------
	string DecodePrefabNameFromKey(string CodedString)	//vracia ID prefabu z kluca "prefab"
	{
		int spos = strstr(CodedString, ";")
		string PrefabName = ""
		
		if(spos > 1)
			PrefabName = substr(CodedString, 0, spos)

		return PrefabName
	}

//-----------------------------------------------------------------
	string DecodePrefabEntNameFromKey(string CodedString)	//vracia ID definicie entity z prefabu z kluca "prefab"
	{
		int spos = strstr(CodedString, ";")
		string PrefabEntName = ""
		int lng
		string temp
		
		if(spos > 1)
		{
			lng = strlen(CodedString)
			lng -= spos + 1
			temp = substr(CodedString, spos + 1, lng)
			
			spos = strstr(temp, ";")
			
			if(spos > 1)
			{
				lng = strlen(temp)
				lng -= spos + 1
				temp = substr(temp, spos + 1, lng)
			}
			
			PrefabEntName = temp
		}
		return PrefabEntName
	}

//-----------------------------------------------------------------
	string DecodePrefabInstanceIncerementFromKey(string CodedString)	//vracia ID definicie entity z prefabu z kluca "prefab"
	{
		int spos = strstr(CodedString, ";")
		string PrefabEntName = ""
		int lng
		string temp
		
		if(spos > 1)
		{
			lng = strlen(CodedString)
			lng -= spos + 1
			temp = substr(CodedString, spos + 1, lng)
			
			spos = strstr(temp, ";")
			
			if(spos > 1)
				PrefabEntName = substr(temp, 0, spos)
		}
		return PrefabEntName
	}

//------------------------------------------------------------------------------
	string GetPrefabNameOfEntity(ExtClass ent)
	{
		string CodedPrefabID = ent.GetKeyValue("prefab")
		string RealPrefabID = DecodePrefabNameFromKey(CodedPrefabID)
		return RealPrefabID
	}

//------------------------------------------------------------------------------
	Prefab GetPrefabOfEntity(ExtClass ent)
	{
		string CodedPrefabID = ent.GetKeyValue("prefab")
		string RealPrefabID = DecodePrefabNameFromKey(CodedPrefabID)
		return PrefManager.GetPrefab(RealPrefabID)
	}
	
//------------------------------------------------------------------------------
	PrefabEnt GetPrefabSegmentOfEntity(ExtClass ent)
	{
		string CodedPrefabID = ent.GetKeyValue("prefab")
		string RealPrefabID = DecodePrefabNameFromKey(CodedPrefabID)
		string RealEntPrefabID = DecodePrefabEntNameFromKey(CodedPrefabID)
		
		Prefab pref = PrefManager.GetPrefab(RealPrefabID)
		
		if(!pref)
			return NULL
		
		PrefabEnt PrefEnt = pref.GetPrefabEnt(RealEntPrefabID)
		
		if(!PrefEnt)
			return NULL
		
		return PrefEnt
	}

//------------------------------------------------------------------------------
	bool IsPrefabed(ExtClass ent)
	{
		if(GetPrefabSegmentOfEntity(ent))
			return true
		
		return false
	}
	
//------------------------------------------------------------------------------
	bool IsKeyOfPrefabChangeable(ExtClass ent, string key)
	{
		string ClName = ClassName(ent)

		if(key == "prefab" || key == "coords" || key == "angle" || key == "angle2" || key == "angle3" || key == "flags" || key == "dirangle")
			return false
	
		if(key == "HackCameraFrom")
			return false
	
		if(ClName == "func_plat")
		{
			if(key == "length" || key == "model")
				return false
		}

		if(key == "wait" && IsInherited(ent, CDoorClass))
			return false

 		if(key == "intensity" && ClName == "dlight_dynamic")
 			return false

		if(key == "_userstates")	//trochu prasarna pretoze userstates neni kluc ale v prefaboch sa uklada ako kluc a preto tu musi byt
			return false

		if(key == "_materialremap")
			return false
		
		if(IsInherited(ent, CTriggerClass))
		{
			if(key == "actors" || key == "mins" || key == "maxs")
				return false
		}

		if(IsInherited(ent, CJointClass))
		{
			if(key == "body1" || key == "body2")
				return false
		}

		if(IsEvent(ent, key) == true)
			return false

		return true
	}

//-----------------------------------------------------------------
	int FillKeyValuesFromPrefab(ExtClass ent, string CodedPrefabID)
	{
		int ChangeCounter = 0
		string RealPrefabID = DecodePrefabNameFromKey(CodedPrefabID)
		string RealEntPrefabID = DecodePrefabEntNameFromKey(CodedPrefabID)

		Prefab pref = PrefManager.GetPrefab(RealPrefabID)

		if(!pref)
			return ChangeCounter

		PrefabEnt PrefEnt = pref.GetPrefabEnt(RealEntPrefabID)
		
		if(!PrefEnt)
			return ChangeCounter
		
		string key, val
		bool Apply

		for(int k = 0; k < PrefEnt.NumPrefabKeys; k++) //naplnime hodnoty z prefabu
		{
			key = PrefEnt.PrefabKeys[k]
			val = PrefEnt.PrefabKeyValues[k]

			if(IsKeyOfPrefabChangeable(ent, key) == false)
				continue

			string OldVal = ent.GetKeyValue(key)

			Apply = true

			if(OldVal == val)
				Apply = false
			else
			{
				if(OldVal == "" && val == "0")	//berieme ze su rovnake
					Apply = false

				if(OldVal == "0" && val == "")
					Apply = false
			}

			if(Apply)
			{
				AddKeyValueToEntity(ent, key, val)
//				Print(key)
				ChangeCounter++
			}
		}
		
		if(ChangeCounter > 0)
			OnDataChange()

		return ChangeCounter
	}
	
//-----------------------------------------------------------------
	void WritePrefabsValuesToAllEntities()
	{
		ExtClass ec
		string PrefabCodedID
		int ChangedEntities = 0
		int AllChangedKeys = 0
		int ChangedKeys

		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(ec._name && ec.DefPtr)	//je to entita z mapy a ma definiciu v entities.def
			{
				PrefabCodedID = ec.GetKeyValue("prefab")

				if(PrefabCodedID)
				{
					ChangedKeys = FillKeyValuesFromPrefab(ec, PrefabCodedID)
					
					if(ChangedKeys > 0)
					{
						ChangedEntities++
						AllChangedKeys += ChangedKeys
					}
				}
			}
		}
		
		if(ChangedEntities > 0)
		{
			string StrChangedEntities = itoa(ChangedEntities)
			RequestDialog("Prefabs applied to " + StrChangedEntities + " entities (" + itoa(AllChangedKeys) + " keys rewritten)", "Applying prefabs result", GUI_DIALOG_OK)
		}
	}

//-----------------------------------------------------------------
	void TestPrefabsForUniqueInstances()
	{
		ExtClass ec, dup
		string PrefabCodedID
		string RealPrefabID
		string PrefabInstanceID
		
		string CodedID2
		string InstanceID2
		string PrefabID2

		int NumInstancies
		int NumErrors

		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(ec._name && ec.DefPtr)	//je to entita z mapy a ma definiciu v entities.def
			{
				NumInstancies = 0
				NumErrors = 0
				PrefabCodedID = ec.prefab//ec.GetKeyValue("prefab")
				
				if(PrefabCodedID)
				{
					RealPrefabID = DecodePrefabNameFromKey(PrefabCodedID)
					PrefabInstanceID = DecodePrefabInstanceIncerementFromKey(PrefabCodedID)
					Prefab pref = PrefManager.GetPrefab(RealPrefabID)
	
					if(pref)
					{
	//					for(dup = GetNextEntity(NULL, CExtClass); dup != NULL; dup = GetNextEntity(dup, CExtClass))
						dup = GetNextEntity(NULL, CExtClass)
						while(dup)
						{
							if(dup._name && dup.DefPtr)
							{
								CodedID2 = dup.prefab//GetKeyValue("prefab")
								
								if(CodedID2)
								{
									PrefabID2 = DecodePrefabNameFromKey(CodedID2)
									InstanceID2 = DecodePrefabInstanceIncerementFromKey(CodedID2)
									
									if(InstanceID2 == PrefabInstanceID)	//co ma rovnake unikatne ID
									{
										if(PrefabID2 != RealPrefabID)		//to musi mat aj rovnaky prefab
											NumErrors++
										else
											NumInstancies++					//kontrolujeme aj ci sedi pocet entit s tymto unikatnym cislom
									}
								}
							}
							dup = GetNextEntity(dup, CExtClass)
						}

						if(NumInstancies > pref.NumPrefabEntities)
						{
							int NumErr = NumInstancies - pref.NumPrefabEntities
							RequestDialog("Too much prefabed entities with instance ID " + PrefabInstanceID + ". " + itoa(NumErr) + " errors", "Prefabs error", GUI_DIALOG_OK )
						}
					}
				}
		
				if(NumErrors)
					RequestDialog("Equal instance ID between different prefab instances detected! " + itoa(NumErrors) + " errors", "Prefabs error", GUI_DIALOG_OK )
			}
		}
	}

//------------------------------------------------------------------------------
	bool ChangePrefabFromEntity(ExtClass ent)	//klucami entity prepise prefab ktory ma nadefinovany
	{
		if(!ent)
			return false

		PrefabEnt PrefEntDef = GetPrefabSegmentOfEntity(ent)

		if(!PrefEntDef)
			return false

		int n
		string Val
		string Key

		for(n = 0; n < ent.DefPtr.NumKeys; n++)
		{
			Val = ent.EKeyValues[n]
			Key = ent.DefPtr.Keys[n]

			if(IsKeyOfPrefabChangeable(ent, Key) == false)
				continue

			PrefEntDef.SetPrefabKeyValue(Key, Val)
		}
		return true
	}

//-----------------------------------------------------------------
	int FillChoicesArrayForKey(ExtClass ent, string Key, string ChoicesArray[])
	{
		int ChoicesNum = 0
		int n
		
		if(Key == "name")
		{
			if(IsInherited(ent, CItemMisc))
			{
				n = 0
				ItemDef CurItem
	
				for(n = 10; n < World.ItemColPtr.ItemsNum; n++)
				{
					CurItem = World.ItemColPtr.Items[n]

					if(CurItem)
						ChoicesArray[ChoicesNum++] = CurItem.ID
				}
				return ChoicesNum
			}
			
			if(IsInherited(ent, CMiscPhysicsPack))
			{
				for(n = 0; n <= BrkGroupsNum; n++)
					ChoicesArray[ChoicesNum++] = BrkGroups[n]
				
				return ChoicesNum
			}
		}
		
/*		if(Key == "flags")
		{
			for(n = 0; n < DefEntFlagsNum[ent.EDefIndex]; n++)
				ChoicesArray[ChoicesNum++] = DefEntFlags[ent.EDefIndex][n]

			return ChoicesNum
		}*/

		if(Key == "model")
		{
			if(IsInherited(ent, CParticleEffector))
			{
				ChoicesArray[ChoicesNum++] = "Particle files (*.ptc)|*.ptc"
				ChoicesArray[ChoicesNum++] = "particle"	//defaultna cesta kde sa otvori filedialog
			}
			else
			{
				ChoicesArray[ChoicesNum++] = "XOB files (*.xob)|*.xob"
				ChoicesArray[ChoicesNum++] = "obj"	//defaultna cesta kde sa otvori filedialog
			}
			return ChoicesNum
		}

		if(Key == "anim" || Key == "baseanim" || Key == "beforeopenanim" || Key == "aftercloseanim" || Key == "pose")
		{
			ChoicesArray[ChoicesNum++] = "Anim files (*.anm)|*.anm"
			ChoicesArray[ChoicesNum++] = "anm"
			return ChoicesNum
		}

		if(Key == "display")
		{
			ChoicesArray[ChoicesNum++] = "Display files (*.dpl)|*.dpl"
			ChoicesArray[ChoicesNum++] = "displays"
			return ChoicesNum
		}
		return 0
	}

//-----------------------------------------------------------------
	string GetUserStatesOfSelected()
	{
		if(MainSel)
			return MainSel._userstates
	}
	
//-----------------------------------------------------------------
	void SetUserStatesOfSelected(string states)
	{
		if(MainSel) 
		{
			MainSel._userstates = states
			OnDataChange()
		}
	}

//-----------------------------------------------------------------
	void ShowPropertiesOfSelecting()
	{
		if(!MainSel)
			return
/*
		string ColTexts0[60]
		string ColTexts1[60]
		string EntMethodsHelps[60]

		int ColColors0[60]
		int ColColors1[60]
		int BackgroundColor = 0xff222222	//default pozadie
		string Label
		int LinesNum
		int n
		int Edit	//true - hodnoty budu editovatelne

		LinesNum = 0
		Edit = 1
		ExtClass CurSel, Another
		int NumPrefabed = 0

		for(int s = 0; s < SelectedNum; s++)
		{
			CurSel = Selected[s]
			
			if(IsPrefabed(CurSel))
				NumPrefabed++
		}

		if(SelectedNum > 1)
		{
			Label = itoa(SelectedNum) + " entities    (" + itoa(NumPrefabed) + " PREFABED)"

			if(NumPrefabed > 0)
				BackgroundColor = ARGB(255, 86, 0, 0)
		}
		else
		{
			Label = MainSel._name

			if(NumPrefabed > 0)
			{
				BackgroundColor = ARGB(255, 86, 0, 0)
				Prefab prf = GetPrefabOfEntity(MainSel)
				Label += "   (PREFABED to " + prf.ID + ")"
			}
			else
				Label += "   (NOT PREFABED)"
		}

		if(SelectedNum == 1)
		{
			for(n = 0; n < MainSel.DefPtr.NumKeys; n++)
			{
				if(IsNotEditableKey(MainSel.DefPtr.Keys[n]))
					continue

				ColTexts0[LinesNum] = MainSel.DefPtr.Keys[n]
				ColTexts1[LinesNum] = MainSel.EKeyValues[n]

				if(IsEvent(MainSel, ColTexts0[LinesNum]))
					ColColors0[LinesNum] = ARGB(255, 255, 100, 100)
				else
					ColColors0[LinesNum] = ARGB(255, 255, 255, 0)
					
				ColColors1[LinesNum] = ARGB(255, 255, 255, 255)
				LinesNum++
			}
		}
		else
		{
			string KeysToEdit[256]
			int KeysToEditNum = 0
			string CurKey, CurValue, AnotherKey
			int NumDup

			for(s = 0; s < SelectedNum; s++)
			{
				CurSel = Selected[s]

				for(int c = 0; c < CurSel.DefPtr.NumKeys; c++)
				{
					CurKey = CurSel.DefPtr.Keys[c]
					NumDup = 0
					
					for(int r = 0; r < SelectedNum; r++)
					{
						Another = Selected[r]
						
						if(CurSel != Another)
						{
							for(int a = 0; a < Another.DefPtr.NumKeys; a++)
							{
								AnotherKey = Another.DefPtr.Keys[a]

								if(AnotherKey == CurKey)
								{
									NumDup++
									break	
								}
							}								
						}
					}
					
					int CorrectNum = SelectedNum - 1
					bool AddKey = true

					if(NumDup == CorrectNum)
					{
						for(int k = 0; k < KeysToEditNum; k++)
						{
							if(KeysToEdit[k] == CurKey)
							{
								AddKey = false
								break	
							}
						}
						
						if(AddKey)
							KeysToEdit[KeysToEditNum++] = CurKey
					}
				}
			}

			for(k = 0; k < KeysToEditNum; k++)
			{
				CurKey = KeysToEdit[k]
				CurValue = MainSel.GetKeyValue(CurKey)

				if(IsNotEditableKey(CurKey)
					continue

				ColTexts0[LinesNum] = CurKey
				ColTexts1[LinesNum] = MainSel.GetKeyValue(CurKey)

				if(IsEvent(MainSel, CurKey)
					ColColors0[LinesNum] = ARGB(255, 255, 100, 100)
				else
					ColColors0[LinesNum] = ARGB(255, 255, 255, 0)
					
				ColColors1[LinesNum] = ARGB(255, 255, 255, 255)
				LinesNum++
			}
		}

		string KeyChoices[1024]
		int KeyChoicesNum = 0
		int VarType, ControlType
		string Key, Val
*/
		ShowSelectionProperties(ClassName(MainSel), MainSel.DefPtr.Keys, MainSel.EKeyValues, MainSel.DefPtr.NumKeys)
	}

//------------------------------------------------------------------------------
	ExtClass NewEntity(string NewClName, vector pos)
	{
		if(!DefManager.GetDefinition(NewClName))		//entita neni v entities.def
		{
			string ErrMsg = "Entita " + NewClName + " neni nadefinovana v entities.def"
			RequestDialog(ErrMsg, "Error", GUI_DIALOG_OK )
			return NULL
		}

		g_NewEntityCoords = pos
		
		ExtClass SpawnedEnt = AddEntity(NewClName, true)
		
		if(!SpawnedEnt)
			return NULL

		AddSelection(SpawnedEnt)
		SetMoveType(MainSel, NONE)
		UpdateEntity(MainSel)
		LastCreated = ClassName(MainSel)

		vector mins, maxs, tm
		GetBoundBox(MainSel, mins, maxs)
		if(GetValue(MainSel, "mins", 0, tm))
		{
			g_ValMins = mins
			g_ValMaxs = maxs
		}

		float height = maxs[2] - mins[2]
		vector xxx = GetTracPos(MainSel, g_NewEntityCoords + Vector(0, 0, height), "0 0 -500")

		SetEntityOrientationKeys(MainSel, xxx, ZeroVec)
		ReplaceSelection(MainSel, ReloadEntity(MainSel, ClassName(MainSel)))
		ThrowEvent(MainSel, EV_INIT)
		ThrowEvent(MainSel, EV_LOADED)

		UpdateEntityLinks(true)
		g_ValMins = ZeroVec
		g_ValMaxs = ZeroVec

		Status = ST_SELECTING
		return MainSel
	}

	bool CanEditKey(ExtClass ent, string key)	//zistujeme ci sa nejedna o entitu ktora je z prefabu
	{
		if(IsPrefabed(ent) == true)	//ak je to prefabovana entita tak zmena klucov s vynimkou coords, angles a eventov znamena zmenit to v prefabe a vsetkych entitach ktore pouzivaju ten prefab
		{
			if(IsKeyOfPrefabChangeable(ent, key) == false)	//je to kluc ktory nema nema vplyv na prefab takze sa prefab menit nebude
				return true

			if(PrfIsWritable() == false)
			{
				RequestDialog("Prefabs file is read only!", "Error", GUI_DIALOG_OK)
				return false
			}
	
			if(RequestDialog("This entity is prefabed. Changing this value will change prefab and all entities that uses it. Really accept?", "Warning for prefab changing", GUI_DIALOG_YES_NO) == GUI_REQUEST_YES)
				return true		//user potvrdil ze mu nevadi zmena prefabu

			return false
		}
		else
			return true
	}

//--------------------------------------------
	MinClass GetTracEnt(MinClass ent, vector from, vector dir)
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
		
		float fl = TraceLineEx(DEFAULT, start, end, mins, maxs, TargEnt, Plane, NULL, NULL, TRACE_WORLD, NULL)

		if(fl == 1)
			return NULL
		else
			return TargEnt
	}

	int GetCameraPosFactor()
	{
		float Angle
		vector Pos = GetMatrix(MainSel, 3)
		vector offset = World.CameraPos - Pos

		vector ang = Vector2Angles(offset)
		Angle = ang[1]

		if(Angle >= 315 && Angle < 360)
			return 1
			
		if(Angle >= 0 && Angle < 45)
			return 1

		if(Angle >= 45 && Angle < 135)
			return 2

		if(Angle >= 135 && Angle < 225)
			return 3

		if(Angle >= 225 && Angle < 315)
			return 4
	} 

//-----------------------------------------------------------------
	bool PrfIsWritable()
	{
		return IsFileWritable("scripts/entities.prf")
	}

//-----------------------------------------------------
	bool Filter2(ExtClass other)
	{
		return true
	}
	
//-----------------------------------------------------
	void SetEditMode(int mode)	//entity <-> area edit mode
	{
		EdMode = mode

		if(EdMode == EDITMODE_ENTITY)
		{
			r_bright = false
			ShowClips(false)
			Status = ST_SELECTING
		}

		if(EdMode == EDITMODE_PATH)
		{
			r_bright = true
			ShowClips(ClipsVisible)
			ShowGrid = 1
		}

		if(EdMode == EDITMODE_NONE)
		{
			r_bright = false
			ShowClips(false)
			ShowGrid = 0
		}

		PrintToStatusBar(0, "")
		PrintToStatusBar(1, "")
		PrintToStatusBar(2, "")
		PrintToStatusBar(3, "")
		SetAreasVis()
		Ent = NULL
		UnselectAll()
	}
//-----------------------------------------------------

	bool PickEntity()
	{
		if(EntIsWritable() == false)
		{
			RequestDialog("File is Read Only!", "Error", GUI_DIALOG_OK)
			return false
		}

		if(IsBrushEntity(MainSel))
			return false

		if(MainSel == World)
			return false

		if(MainSel.ParentName && SelectedNum > 1)
		{
			Print("Cannot change orientation of group when main selection is child of other entity!")
			return false
		}

		vector mins, maxs
		GetBoundBox(MainSel, mins, maxs)

		vector tm1, tm2
		if(GetValue(MainSel, "mins", 0, tm1) && GetValue(MainSel, "maxs", 0, tm2))
		{
			if(IsInherited(MainSel, CDoorClass))	//dvere maju v pravych uhloch tieto hodnoty uz transformovane podla otocenia. Debug box by sa potom zobrazil blbo
			{
				g_ValMins = mins
				g_ValMaxs = maxs
			}
			else
			{
				g_ValMins = tm1
				g_ValMaxs = tm2
			}
		}

		g_AngleVal[0] = atoi(GetValueFromKey(MainSel, "angle3"))
		g_AngleVal[1] = atoi(GetValueFromKey(MainSel, "angle"))
		g_AngleVal[2] = atoi(GetValueFromKey(MainSel, "angle2"))

		if(MainSel.ParentName)
			g_NewEntityCoords = GetOrigin(MainSel)
		else
		{
			string TmpVal
			TmpVal = GetValueFromKey(MainSel, "coords")
			g_NewEntityCoords = atov(TmpVal)
		}

		OldOrigin = g_NewEntityCoords
		LastEntityCoords = g_NewEntityCoords

		MemSelectionOffsets(MovingOffset)	//zapametame si offsety na hlavnu entitu
		ShowCursorPernamently(false)
		return true
	}

	string ClearIncerementFromName(string Name)
	{
		string ext = substr(Name, strlen(Name) - 1, 1)

		while(IsNumber(ext))
		{
			if(strlen(Name) > 0)
				Name	= substr(Name, 0 , strlen(Name) - 1)
				
			ext = substr(Name, strlen(Name) - 1, 1)
		}
		
		return Name
	}

	void Hide(ExtClass other, bool HideStat)
	{
		if(HideStat)	//hide
		{
			if(!other)
				return
			
			other.HideInEditor(true)
			other.DestroyEditorVisuals()

			if(IsSelected(other))
			{
				RemoveSelection(other)
				
				if(MainSel)
				{
					ShowPropertiesOfSelecting()
					OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
				}
				else
					OnUpdateSelection("", "", "", false)
			}
		}
		else				//unhide
		{
			if(other == NULL)
			{
				ExtClass ec
				for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
				{
					if(ec.DefPtr && ec != World)	//je z ent suboru. ostatne musia byt osetrene tentitami ktore ich vytvorili
					{
						if(ec.Hidden == true)
						{
							ec.HideInEditor(false)
							ec.CreateEditorVisuals()	//ked je este vyfiltrovana tak sa aj tak nezobrazi
						}
					}
				}
			}
			else
			{
				other.HideInEditor(false)
				other.CreateEditorVisuals()
			}
		}
		
		UpdateEntityLinks(true)
	}

//--------------------------------------------------------------------
	bool InsertEntitiesFromPrefab(string prefab, vector pos, vector ForcedAngles)
	{
		Prefab pref = PrefManager.GetPrefab(prefab)
		PrefabEnt CurPrefEnt
		string key, val
		
		string ErrMsg
		vector offset
		vector Origin
		vector OriginMain
		int n
		ExtClass CreatedEntities[16]
		int CreatedEntitiesNum = 0

		if(!pref)
		{
			ErrMsg = "Prefab " + prefab + " not found"
			MessageBoxA(0, ErrMsg, "Error", 0)
			return false
		}

		for(n = 0; n < pref.NumPrefabEntities; n++)
		{
			CurPrefEnt = pref.PrefabEntities[n]

			if(!DefManager.GetDefinition(CurPrefEnt.ClassName))	//entita neni v entities.def
			{
				ErrMsg = "Entita " + CurPrefEnt.ClassName + " neni nadefinovana v entities.def"
				MessageBoxA(0, ErrMsg, "Error", 0)
				return false
			}
		}

		World.PrefabsIncrement = atoi(World.GetKeyValue("PrefabsIncrement"))
		AddKeyValueToEntity(World, "PrefabsIncrement", itoa(++World.PrefabsIncrement))

		UnselectAll()
		g_NewEntityCoords = pos

		for(n = 0; n < pref.NumPrefabEntities; n++)
		{
			CurPrefEnt = pref.PrefabEntities[n]
			string NewName = CurPrefEnt.ID
			
			NewName = ClearIncerementFromName(NewName)
			
			NameForNew = GetIncrementNameForNew(NewName)

			ExtClass SpawnedEnt = AddEntity(CurPrefEnt.ClassName, true)

			if(!SpawnedEnt)
				continue

			AddSelection(SpawnedEnt)

			AddDefinedKeysToEntity(MainSel)	//naplni kluce, eventy, metody, flagy proste vsetko ale bez hodnot

			for(int k = 0; k < CurPrefEnt.NumPrefabKeys; k++) //naplnime hodnoty z prefabu
			{
				key = CurPrefEnt.PrefabKeys[k]
				val = CurPrefEnt.PrefabKeyValues[k]

				if(key == "_userstates")
					MainSel._userstates = val
				else
					AddKeyValueToEntity(MainSel, key, val)
			}

			UpdateEntity(MainSel)
			LastCreated = ClassName(MainSel)

			if(n == 0)
			{
				vector bmins, bmaxs
				GetBoundBox(MainSel, bmins, bmaxs)
				float height = bmaxs[2] - bmins[2]
				Origin = GetTracPos(MainSel, g_NewEntityCoords + Vector(0, 0, height + 10), "0 0 -500")
				OriginMain = Origin
			}
			else
			{
				offset = atov(CurPrefEnt.GetPrefabKeyValue("coords"))
				Origin = OriginMain + offset
			}

			string CodedPrefabID = pref.ID + ";" + itoal(World.PrefabsIncrement, 6)	//do kluca "prefab si zakodujeme ID prefabu a ID definicie entity v prefabe"
			CodedPrefabID = CodedPrefabID + ";" + CurPrefEnt.ID
//			string CodedPrefabID = pref.ID + ";" + itoal(World.PrefabsIncrement, 6) + ";" + CurPrefEnt.ID
			AddKeyValueToEntity(MainSel, "prefab", CodedPrefabID)

 			float ang3 = atof(CurPrefEnt.GetPrefabKeyValue("angle3")) 
 			float ang = atof(CurPrefEnt.GetPrefabKeyValue("angle"))
 			float ang2 = atof(CurPrefEnt.GetPrefabKeyValue("angle2"))

 			vector Angles = Vector(ang3, ang, ang2)
			SetEntityOrientationKeys(MainSel, Origin, Angles)
			
			CreatedEntities[CreatedEntitiesNum++] = MainSel
		}


		string EventLine
		string Temp1, Temp2
		string CurEntName
		ExtClass CurEnt
		ExtClass Created
		PrefabEnt PrfEnt
		int Spos
		int lng
		int IDlng
		string SplitChar

		for(n = 0; n < CreatedEntitiesNum; n++)
		{
			CurEnt = CreatedEntities[n]
			CurEntName = CurEnt._name
			
			for(k = 0; k < CurEnt.DefPtr.NumKeys; k++)
			{
				key = CurEnt.DefPtr.Keys[k]
				val = CurEnt.EKeyValues[k]
				
				if(val && key && IsNotEditableKey(key) == false && IsPathKey(key) == false/* && IsEvent(CurEnt, key)*/)
				{
					EventLine = val

					for(int s = 0; s < pref.NumPrefabEntities; s++)
					{
						Created = CreatedEntities[s]
						CurPrefEnt = pref.PrefabEntities[s]
						IDlng = strlen(CurPrefEnt.ID)
						Spos = strstroff(EventLine, 0, CurPrefEnt.ID)
						
						if(EventLine == CurPrefEnt.ID)	//ak je v EventLine napisana iba jedna a nic viac tak to vybavime ihned
						{
							EventLine = Created._name
							Spos = -1
							s = pref.NumPrefabEntities	//zbytocne testovat dalsie entity
						}
						
						while(Spos != -1)
						{
//							if(CurPrefEnt.ID == "MostB" && key == "actors" && EventLine == "MostA0,MostB0")
//								debug
								
							if(Spos >= strlen(EventLine))
								break
								
							Temp1 = substr(EventLine, 0, Spos)
							lng = strlen(EventLine) - Spos
							int max = Spos + IDlng + 1

							if(strlen(EventLine) > max)
								SplitChar = substr(EventLine, Spos + IDlng, 1)
							else
								SplitChar = ""

							if(SplitChar == "." || SplitChar == "," || SplitChar == " " || SplitChar == ")" || SplitChar == "_")
							{
								bool miss = false
								int BestLng = IDlng

								for(int i = 0; i < pref.NumPrefabEntities; i++)
								{
									PrfEnt = pref.PrefabEntities[i]

									if(PrfEnt.ID != CurPrefEnt.ID && strlen(PrfEnt.ID) > BestLng)
									{
										bool ok = false
										int okpos = 0
										while(okpos != -1)
										{
											okpos = strstroff(EventLine, okpos, PrfEnt.ID)
											
											if(okpos == Spos)
												ok = true

											if(okpos != -1)
											{
												okpos++
												
												int lpos = strlen(EventLine) - 1
												if(okpos >= lpos)
													okpos = -1
											}
										}

										if(ok)
										{
											BestLng = strlen(PrfEnt.ID)
											miss = true
										}
									}
								}
								
								if(miss == false)	//replacujeme nazov z prefabu za nazov novej entity ale iba v pripade ze neexistuje iny, dlhsi nazov ktory obsahuje tento v sebe + nieco naviac
								{
									Temp2 = substr(EventLine, Spos + IDlng, lng - IDlng)
									EventLine = Temp1 + Created._name + Temp2
									Spos = strstroff(Temp2, 0, CurPrefEnt.ID)
	
									if(Spos != -1)
										Spos += strlen(Temp1 + Created._name)
								}
								else	//inac to preskocime
								{
									Spos += BestLng
									int sp = strstroff(EventLine, Spos, CurPrefEnt.ID)

									if(sp != -1)
										Spos += sp
									else
										Spos = -1
								}
							}
							else	//IDcko sme v stringu nasli ale za nim neni split znak takze to bude ine IDcko ktore sa sklada z tohto naseho + nieco navyse alebo je na konci lajny
							{
								if(SplitChar)
								{
									Temp2 = substr(EventLine, Spos + IDlng, lng - IDlng)
									Spos = strstroff(Temp2, 0, CurPrefEnt.ID)

									if(Spos != -1)
										Spos += strlen(Temp1 + CurPrefEnt.ID)
								}
								else
								{
									int AfterIDpos = Spos + IDlng
									int LastPos = strlen(EventLine) - 1

									if(AfterIDpos >= LastPos)	//sme na konci lajny takze mozeme prepisat nazov
										EventLine = Temp1 + Created._name// + Temp2

									Spos = -1
								}
							}
						}
					}

					if(EventLine != val)
						AddKeyValueToEntity(CurEnt, key, EventLine)
				}
			}
		}
		
		UnselectAll()
		
		for(n = 0; n < CreatedEntitiesNum; n++)
		{
			CurEnt = CreatedEntities[n]
			AddSelection(ReloadEntity(CurEnt, ClassName(CurEnt)))	
		}

		for(n = 0; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_INIT)

		for(n = 0; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_LOADED)

		ExtClass MainEnt = Selected[SelectedNum - 1]
		RemoveSelection(MainEnt)
		AddSelection(MainEnt)		//bude selektnuta ako hlavna

		if(ForcedAngles != ZeroVec)	//pri replacovani entity prefabom potrebujeme vnutit uhly
		{
			MemSelectionOffsets(MovingOffset)	//zapametame si offsety na hlavnu entitu (este nepretocenu)
			SetEntityOrientationKeys(MainSel, pos, ForcedAngles)
			TransformateSelectedEntities(MovingOffset)		//nastavime pozicie a uhly ostatnych selektnutych entit a reloadneme ich
			ReplaceSelection(MainSel, ReloadEntity(MainSel, ClassName(MainSel)))
			ThrowEvent(MainSel, EV_INIT)
			ThrowEvent(MainSel, EV_LOADED)
		}

		UpdateEntityLinks(true)
		AddNamesToGUI(Selected, SelectedNum)
		ShowPropertiesOfSelecting()
		OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
		OnDataChange()

		g_ValMins = ZeroVec
		g_ValMaxs = ZeroVec
		Status = ST_SELECTING
		return true
	}

	//--------------------------------------------------------------------
	bool MakePrefab(string ID)
	{
		int n
		string PrefEntName
		string SelectedNames[32]
		int    SelectedNamesNum = 0
		ExtClass CurEnt
		
		for(n = 0; n < SelectedNum; n++)	//skontrolujeme nazvy entit ci su vhodne pre prefab
		{
			CurEnt = Selected[n]
			PrefEntName = ClearIncerementFromName(CurEnt._name)

			if(PrefEntName != CurEnt._name)
			{
				RequestDialog("Entity name " + CurEnt._name + " is incremental. This is not allowed for prefab!", "Bad name of entity", GUI_DIALOG_OK)
				return false
			}

			if(PrefEntName == "")
			{
				RequestDialog("Entity name " + CurEnt._name + " is not allowed!", "Bad name of entity", GUI_DIALOG_OK)	
				return false
			}

			for(int k = 0; k < SelectedNamesNum; k++)
			{
				if(PrefEntName == SelectedNames[k])
				{
					RequestDialog("Duplicity names detected after removing increments from names. Cannot create prefab!", "Make prefab error", GUI_DIALOG_OK)	
					return false
				}
			}
			
			SelectedNames[SelectedNamesNum++] = PrefEntName
		}

		Prefab pref = new Prefab
		pref.ID = ID

//		int DefIndex
		string key, value
		PrefabEnt PEnt
		vector MainOrigin = GetMatrix(Selected[0], 3)
		vector offset

		World.PrefabsIncrement = atoi(World.GetKeyValue("PrefabsIncrement"))
		AddKeyValueToEntity(World, "PrefabsIncrement", itoa(++World.PrefabsIncrement))

		for(n = 0; n < SelectedNum; n++)
		{
			CurEnt = Selected[n]
			PrefEntName = ClearIncerementFromName(CurEnt._name)
			string CodedPrefabID = ID + ";" + itoal(World.PrefabsIncrement, 6) + ";" + PrefEntName	//do kluca "prefab si zakodujeme ID prefabu a ID definicie entity v prefabe"
			AddKeyValueToEntity(CurEnt, "prefab", CodedPrefabID)
			PEnt = new PrefabEnt
			PEnt.ID = PrefEntName
			PEnt.ClassName = ClassName(CurEnt)
			PEnt.AddPrefabKey("_userstates", CurEnt._userstates)

			for(int j = 0; j < CurEnt.DefPtr.NumKeys; j++)
			{
				key = CurEnt.DefPtr.Keys[j]

				if(key == "coords")
				{
					if(n == 0)
						value = "0 0 0"
					else
					{
						offset = GetMatrix(Selected[n], 3) - MainOrigin
						value = vectoa(offset)
					}
				}
				else
					value = CurEnt.EKeyValues[j]	

				if(key != "_surfaces")				//toto do prefabu nedavame
					PEnt.AddPrefabKey(key, value)
			}
			pref.AddPrefabEnt(PEnt)
		}

		for(n = 0; n < SelectedNum; n++)
		{
			CurEnt = Selected[n]
			ReplaceSelection(CurEnt, ReloadEntity(CurEnt, ClassName(CurEnt)))
		}

		for(n = 0; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_INIT)

		for(n = 0; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_LOADED)

		PrefManager.AddPrefab(pref)
		OnDataChange()
		return true
	}

//-------------------------------------------------------------------- 
	int UnprefabEntities(string PrefabID)
	{
		if(!PrefabID)
			return 0

		int count = 0
		ExtClass ec
		string DecodedPrefabID
		
		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(ec.DefPtr && ec != World/* && ec != World.Player*/)	//je z ent suboru
			{
				DecodedPrefabID = DecodePrefabNameFromKey(ec.GetKeyValue("prefab"))

				if(DecodedPrefabID == PrefabID)
				{
					ec.NullKeyValue("prefab")
					count++
				}
			}
		}
		return count
	}

//-------------------------------------------------------------------- 
	void ProcessMakePrefab()
	{
		if(Selected[0])
		{
			string PrefabID = ""
again:
			if(TextEntryDialog(PrefabID, "New prefab name") == GUI_REQUEST_YES)
			{
				if(PrefabID == "")	//odklepol ok ale nezadal nic
				{
					string ErrMsg2 = "Zadny nazev pro prefab !!!"
					RequestDialog(ErrMsg2, "Error", GUI_DIALOG_OK)
					goto again
				}
				else
				{
					if(PrefManager.GetPrefab(PrefabID))
					{
						RequestDialog("Prefab with this name now exist!", "Error", GUI_DIALOG_OK)
						goto again
					}

					if(MakePrefab(PrefabID) == true)
					{
						PrefManager.SavePrefabs()
						UpdateEntityLinks(true)
					}
				}
			}
		}
	}

//-------------------------------------------------------------------- 
	void ProcessDeletePrefab()
	{
		int n
		Prefab CurPrefab
		for(n = 0; n < PrefManager.NumPrefabs; n++)
		{
			CurPrefab = PrefManager.Prefabs[n]
			PrefabsList[n] = CurPrefab.ID
		}
		
		for(n = n; n < sizeof(PrefabsList); n++)
			PrefabsList[n] = ""
				
		Sort(PrefabsList, PrefManager.NumPrefabs)
		int sel = SingleChoiceDialog("select prefab to delete", PrefabsList, PrefManager.NumPrefabs, 0)

		if(sel >= 0)
		{
			if(RequestDialog("Really delete prefab " + PrefabsList[sel] + " ?", "Deleting prefab", GUI_DIALOG_YES_NO) == GUI_REQUEST_YES)
			{
				Prefab PrefToDel = PrefManager.GetPrefab(PrefabsList[sel])
				string ResultMsg
				string ResultTitle

				if(PrefManager.RemovePrefab(PrefToDel) == true) 
				{
					PrefManager.SavePrefabs()
					int NumUnprefabed = UnprefabEntities(PrefabsList[sel])
					UpdateEntityLinks(true)
					ResultMsg = "Prefab " + PrefabsList[sel] + " deleted. " + itoa(NumUnprefabed) + " entities successfully unprefabed!"
					ResultTitle = "Unprefab result"
				}
				else
				{
					ResultMsg = "Cannot delete prefab " + PrefabsList[sel] + ". Unknown error"
					ResultTitle = "Error"
				}

				RequestDialog(ResultMsg, ResultTitle, GUI_DIALOG_OK)
			}
		}
	}

//--------------------------------------------------------------------
	void ProcessUnprefab()
	{
		if(SelectedNum == 0)
			return

		if(RequestDialog("Really unprefab " + itoa(SelectedNum) + " entities?", "Unprefab", GUI_DIALOG_YES_NO) == GUI_REQUEST_NO)
			return

		ExtClass CurEnt
		int count = 0
		string CodedKey
		string DecodedPrefabID

		for(int n = 0; n < SelectedNum; n++)
		{
			CurEnt = Selected[n]
			CodedKey = CurEnt.GetKeyValue("prefab")

			if(!CodedKey)
				continue

			CurEnt.NullKeyValue("prefab")
			ReplaceSelection(CurEnt, ReloadEntity(CurEnt, ClassName(CurEnt)))
			count++
		}

		for(n = 0; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_INIT)

		for(n = 0; n < SelectedNum; n++)
			ThrowEvent(Selected[n], EV_LOADED)

		string ResultMsg

		if(count == 0)
			ResultMsg = "Cannot unprefab not prefabed entities :-)"
		else
			ResultMsg = "From " + itoa(SelectedNum) + " selected entities was unprefabed " + itoa(count)

		 RequestDialog(ResultMsg, "Unprefab result", GUI_DIALOG_OK)

		UpdateEntityLinks(true)
	}

//--------------------------------------------------------------------
	void ProcessReplaceEntityWithPrefab()
	{
		if(SelectedNum == 0)
			return

		if(SelectedNum > 1)
		{
			RequestDialog("Pouze jednu entitu je mozno nahradit prefabem!", "Relacing entity with prefab", GUI_DIALOG_OK)
			return
		}

		int n
		Prefab CurPrefab
		for(n = 0; n < PrefManager.NumPrefabs; n++)
		{
			CurPrefab = PrefManager.Prefabs[n]
			PrefabsList[n] = CurPrefab.ID
		}
		
		for(n = n; n < sizeof(PrefabsList); n++)
			PrefabsList[n] = ""
				
		Sort(PrefabsList, PrefManager.NumPrefabs)
		int sel = SingleChoiceDialog("select prefab to replace", PrefabsList, PrefManager.NumPrefabs, 0)

		if(sel >= 0)
		{
			if(PrefabsList[sel])
			{
				ExtClass EntToDelete = MainSel
				UnselectAll()
				vector PosForPrefab = atov(EntToDelete.GetKeyValue("coords"))
				vector AnglesForPrefab
				AnglesForPrefab[0] = atof(EntToDelete.GetKeyValue("angle3"))
				AnglesForPrefab[1] = atof(EntToDelete.GetKeyValue("angle"))
				AnglesForPrefab[2] = atof(EntToDelete.GetKeyValue("angle2"))
				delete EntToDelete
				EntToDelete = NULL
				InsertEntitiesFromPrefab(PrefabsList[sel], PosForPrefab, AnglesForPrefab)
			}
		}
	}

//--------------------------------------------------------------------
	bool SelectPrefab(Prefab pref, string UniqueInstance)
	{
		string CurID
		string CurEntID
		PrefabEnt PEntMain
		ExtClass ec
		ExtClass MainEntity = NULL

		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			if(!ec._name || !ec.DefPtr )
				continue
			
			if(ec.prefab)
			{
				CurID = DecodePrefabNameFromKey(ec.prefab)
			
				if(CurID == pref.ID && UniqueInstance == DecodePrefabInstanceIncerementFromKey(ec.prefab))
				{
					AddSelection(ec)
					CurEntID = DecodePrefabEntNameFromKey(ec.prefab)
					PEntMain = pref.PrefabEntities[0]

					if(CurEntID == PEntMain.ID)
						MainEntity = ec
				}
			}
		}

		RemoveSelection(MainEntity)
		AddSelection(MainEntity)		//hlavna entita musi byt selektnuta ako poslena
		
		if(SelectedNum > 0)
			return true
		
		return false
	}

	void ProcessAddEntity()
	{
		if(EntIsWritable() == false)
		{
			RequestDialog("File is Read Only!", "Error", GUI_DIALOG_OK)
			Status = ST_SELECTING
			return	
		}

		PosForNewEntity = end

		if(EdKeyStates[wx_SHIFT] == 1)
		{
			int n
			Prefab CurPrefab
			for(n = 0; n < PrefManager.NumPrefabs; n++)
			{
				CurPrefab = PrefManager.Prefabs[n]
				PrefabsList[n] = CurPrefab.ID
			}
			
			for(n = n; n < sizeof(PrefabsList); n++)
				PrefabsList[n] = ""

			string NewPrefabName = GetNameFromPopup(PrefabsList, PrefManager.NumPrefabs, true, EdScreenMouseX, EdScreenMouseY)

			if(NewPrefabName)
			{
				InsertEntitiesFromPrefab(NewPrefabName, PosForNewEntity, ZeroVec)
				return
			}
			else
				return
		}
		else
		{
			string AllClassNames[256]
			int AllClassNamesNum = DefManager.GetClassNames(AllClassNames)
			string NewClName = GetNameFromPopup(AllClassNames, AllClassNamesNum, true, EdScreenMouseX, EdScreenMouseY)
			
			if(NewClName)
			{
				if(NewClName == "creature_player")
				{
					if(FindClass("player"))
					{
						RequestDialog("Only one instance of creature_player allowed!", "Error", GUI_DIALOG_OK)
						return
					}
				}
	again:
				if(!TextEntryDialog(NameForNew, "Name of new entity"))	//user si to rozmyslel
				{
					Status = ST_SELECTING
					return
				}
				
				if(NameForNew == "")	//odklepol ok ale nezadal nic
				{
					RequestDialog("Zadny nazev pro entitu !!!", "Error", GUI_DIALOG_OK)
					goto again
				}
				else
				{
					while(DuplicityTestOK(NameForNew) == false)
					{
						ShowDuplicityError()
						goto again
					}
					ExtClass EntNew = NewEntity(NewClName, PosForNewEntity)
					
					if(EntNew)
					{
						UnselectAll()
						AddSelection(EntNew)
					}
				}
			}
			else
				return
		}

		if(MainSel)
		{
			AddNamesToGUI(Selected, SelectedNum)
			ShowPropertiesOfSelecting()
			OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
		}

		Status = ST_SELECTING
	}

	void VizualizeSelected()
	{
		if(MainSel == NULL)
			return

		if(MainSel.Hidden == true)
			return

//		MainSel.ShowVisualsSharpColor()

		int color = 0x3fffffff
		
		if(Status == ST_EDITCOORDS)
		{
			vector DBoxPos = GetMatrix(MainSel, 3)
						
			if(GetTracEnt(MainSel, DBoxPos, ZeroVec))
				color = 0x3f555555
			else
				color = 0x3fffffff
		}

		string TmpVal = "0 " + GetValueFromKey(MainSel, "angle") + " 0"
		vector TmpAng = atov(TmpVal)
		
		vector Or = GetMatrix(MainSel, 3)
		vector dvec = GetMatrix(MainSel, 0)//Angles2Vector(TmpAng)
		vector doff = "0 0 0"//"0 0 2"
		dvec = dvec * 100.0
		dvec = dvec + doff
		AddDShape(SHAPE_LINE, 0xffffaaaa, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, Or + doff, Or + dvec)

		vector Start = GetMatrix(MainSel, 3)
		vector End = Start + "0 0 -1024"
		float fplane[4]
		TraceLineEx(PLAYER, Start, End, ZeroVec, ZeroVec, NULL, fplane, NULL, NULL, TRACE_WORLD, NULL)

		AddDShape(SHAPE_LINE, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, Start, End)
		AddDShape(SHAPE_BBOX, 0xffffffff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP|SS_ONCE, End + "-1 -1 -1", End + "1 1 1")
		DrawGrid(ShowGrid, End)
		
		ShowBBoxOfEntity(MainSel, color)
		
		for(int n = 1; n < SelectedNum; n++)
			ShowBBoxOfEntity(Selected[n], 0x3f8080ff)
	}

	void OnMaterialRemapFromGUI()		//na klavesu M
	{
		if(!World.EditorActive)
			return

		if(!MainSel)
			return

		string mremap

		if(GetValue(MainSel, "_materialremap", 0, mremap))
		{			
			TraceContact cont = new TraceContact
			vector to = CursorDir * 1024 + end

			if(TraceLineToEntity(MainSel, World.CameraPos, to, cont))
			{
				ParseRemapping(mremap)

				string mat = GetRemap(cont.Shader)

				int index = FindMaterialName(mat)
				int sel = SingleChoiceDialog("Material list", MaterialNames, NumMaterials, index)

				if(sel >= 0)
				{
					AddOrReplaceRemap(cont.Shader, MaterialNames[sel])
					
					string newremap = UnparseRemap()

					if(mremap != newremap)
					{
						AddKeyValueToEntity(MainSel, "_materialremap", newremap)
						ReplaceSelection(MainSel, ReloadEntity(MainSel, ClassName(MainSel)))
						ThrowEvent(MainSel, EV_INIT)
						ThrowEvent(MainSel, EV_LOADED)
						ChangeKeyInGUI("_materialremap", newremap, "string")
						OnDataChange()
					}
				}
			}
			delete cont
		}
	}

//--------------- pre zvuky -----------------------------
 	void ParseSoundShaders()
 	{
 		int n = 0
 		int lng
 		string tokens[32]
		
		string SndShadFile = "sound/shaders.h"
		tparse tp = BeginParse(SndShadFile)
		
		if(!tp)
		{
			string msg = "No found file " + SndShadFile
			Print(msg)
			return
		}

 		SndShadNamesNum = 0
		while(ParseLine(tp, n++, tokens))
		{
			if(tokens[0] == "sound")
			{
				lng = strlen(tokens[1])
				
				if(lng > 2)
					SndShadNames[SndShadNamesNum++] = substr(tokens[1], 1, lng - 2)
			}
		}
		EndParse(tp)
	}

//===============================================================================================================
//=== NEW EDITOR ================================================================================================
//===============================================================================================================

	void ProcessSelecting()
	{
		UpdateWayEnts()

		vector CursorDir
		vector pos = UnprojectVector(EdMouseX, EdMouseY, CursorDir)
		end = pos
		end = CursorDir * (float)8192 + pos
		float plane[4]
		ExtClass TracEnt = NULL
		string MaterialName = ""

		float f = TraceLineEx(PLAYER, pos, end, ZeroVec, ZeroVec, TracEnt, plane, NULL, NULL, TRACE_WORLD, NULL)				

		if(!IsInherited(TracEnt, CExtClass))
			TracEnt = NULL

		if(TracEnt == World)
			MaterialName = "Material:   " + g_strTraceShader

/*			if(EdMode == EDITMODE_PATH)	//area editor
		{
			end = UserSnapPoint(end)
			DrawGrid(ShowGrid, end)
			end = end + CursorOffset
			SetOrigin(mc, end)
		}*/

		vector targ = end + CursorDir	//jeden unit za kontaktny bod
		Ent = PointEntity(targ)
		
		string EntName = ""
		string EntClName = ""
		string GetValTmp

		if(Ent)
		{
			MaterialName = ""
			vector to = CursorDir * 1024 + end

			if(TraceLineToEntity(Ent, World.CameraPos, to, SelectingContact))
				MaterialName = "Material:   " + SelectingContact.Shader

			Ent = Ent.GetEventualOwner()
			
			if(!Ent.DefPtr || Ent._name == "world")	//nepochadza z .ent suboru
				Ent = NULL
				
			if(Ent)
			{
				EntName = "Name:   " + Ent._name
				EntClName = "ClassName:   " + ClassName(Ent)
			}
		}

		PrintToStatusBar(0, EntName)
		PrintToStatusBar(1, EntClName)
		PrintToStatusBar(2, MaterialName)
//		if(Ent)
//			ShowBBoxOfEntity(Ent, 0x0f00ffff)

		if(EdLMBdown)	//drzime stlacenu lavu mys
		{
			if(MainSel)
			{
				if(abs(EdMouseDeltaX) > 0 || abs(EdMouseDeltaY) > 0)	//pohybom mysi prepina na edit coords
				{
					ClearMousePress(0)

					if(PickEntity())
					{
						Status = ST_EDITCOORDS
						ClipCursorInsideWindow(true)
						return
					}	
				}
			}
		}
	
		if(EdRMBdown)	//drzime stlacenu pravu mys
		{
			if(MainSel)
			{
				if(abs(EdMouseDeltaX) > 0 || abs(EdMouseDeltaY) > 0)	//pohybom mysi prepina na edit coords
				{
					ClearMousePress(0)
	
					if(PickEntity())
					{
						Status = ST_EDITCOORDS
						ClipCursorInsideWindow(true)
						return
					}	
				}
			}	
		}		
	}

//-----------------------------------------------------------------
	void OnJumpCameraToEntityFromGUI()
	{
		if(!World.EditorActive)
			return

		if(!MainSel)
			return
				
		World.CameraPos = GetMatrix(MainSel, 3) + "0 0 50"	//premiestnime sa nad entitu
	}

//-----------------------------------------------------------------
	void OnHideFromGUI(bool hide)
	{
		if(!World.EditorActive)
			return

		ExtClass ToHide = Ent
		
		if(hide)
		{
			if(!ToHide)
				ToHide = MainSel

			Hide(ToHide, true)

			if(Ent == ToHide)
				Ent = NULL
		}
		else
		{
/*			if(MainSel)
				ToHide = MainSel
			else
				ToHide = NULL
	*/			
			Hide(NULL, false)
		}
	}

//-----------------------------------------------------------------
	void OnClearFilterFromGUI()
	{
		if(!World.EditorActive)
			return

		FilterClassNamesNum = 0
	}

//-----------------------------------------------------------------
	void OnAddItemToFilterFromGUI(string ClName)
	{
		if(!World.EditorActive)
			return

		if(FilterClassNamesNum >= sizeof(FilterClassNames))
			return

		FilterClassNames[FilterClassNamesNum++] = ClName
	}

//-----------------------------------------------------------------
	void OnApplyEntitiesFilterFromGUI()
	{
		if(!World.EditorActive)
			return

		ExtClass ec
		bool NeedBeHidden
		string ClName
		
		for(ec = GetNextEntity(NULL, CExtClass); ec != NULL; ec = GetNextEntity(ec, CExtClass))
		{
			ClName = ClassName(ec)
			
			if(ec.DefPtr)
			{
				NeedBeHidden = false
	
				for(int n = 0; n < FilterClassNamesNum; n++)
				{
					if(ClName == FilterClassNames[n])
					{
						NeedBeHidden = true
						break
					}
				}
	
				if(ec.CategoryHidden == false && NeedBeHidden == true)	//co neni skryte a ma byt to skryjeme
					ec.HideByCategory(true)
				
				if(ec.CategoryHidden == true && NeedBeHidden == false)	//co je skryte a nema byt to odkryjeme
					ec.HideByCategory(false)
				
				if(ClName != "light" && ClName != "world")
				{
					if(NeedBeHidden)
						ec.DestroyEditorVisuals()
					else
						ec.CreateEditorVisuals()	//vo vnutri je osetrene keby bola este hidnuta
				}
			}
		}

		UpdateEntityLinks(true)
	}

//-----------------------------------------------------------------
	void OnApplyOtherFilterFromGUI(bool Areas, bool Lights, bool Clips)
	{
		if(!World.EditorActive)
			return

		AreasVisible = Areas
		LightsVisible = Lights
		ClipsVisible = Clips

		if(EdMode == EDITMODE_ENTITY)	//path editor ich ma zapnute vzdy
			ShowAreas(Areas)

		ShowLights(Lights)
		ShowClips(Clips)
	}

//-----------------------------------------------------------------
	void OnUnselectFromGUI()
	{
		if(!World.EditorActive)
			return

		UnselectAll()
		OnUpdateSelection("", "", "", false)
	}

//-----------------------------------------------------------------
	void OnSaveEntitiesFromGUI()
	{
		if(!World.EditorActive)
			return

		SaveEntitiesKeys()
	}

//-----------------------------------------------------------------
	void OnCreatePrefabFromGUI()
	{
		if(!World.EditorActive)
			return

		if(PrfIsWritable() == false)
			RequestDialog("Prefab file is read only!", "Error", GUI_DIALOG_OK)
		else
			ProcessMakePrefab()	//proces konci po skonceni funkcie
	}

//-----------------------------------------------------------------
	void OnDeletePrefabFromGUI()
	{
		if(!World.EditorActive)
			return

		if(PrfIsWritable() == false)
			RequestDialog("Prefab file is read only!", "Error", GUI_DIALOG_OK)
		else
			ProcessDeletePrefab()		//proces konci po skonceni funkcie		
	}

//-----------------------------------------------------------------
	void OnForcePostitionFromGUI()
	{
		if(!World.EditorActive)
			return

		if(!MainSel)
			return
	
		if(!EntIsWritable())
		{
			RequestDialog("File is Read Only!", "Error", GUI_DIALOG_OK)
			return
		}
	
		if(MainSel != World && ClassName(MainSel) != "misc_model" && !IsBrushEntity(MainSel))
		{
			AddKeyValueToEntity(MainSel, "coords", vectoa(end))
			ReplaceSelection(MainSel, ReloadEntity(MainSel, ClassName(MainSel)))
			ThrowEvent(MainSel, EV_INIT)
			ThrowEvent(MainSel, EV_LOADED)

			ShowPropertiesOfSelecting()
			OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
			UpdateEntityLinks(true)
			OnDataChange()
		}
	}

//-----------------------------------------------------------------
	void OnUnprefabSelectionFromGUI()
	{
		if(!World.EditorActive)
			return

		ProcessUnprefab()
	}

//-----------------------------------------------------------------
	void OnReplaceEntityWithPrefabFromGUI()
	{
		if(!World.EditorActive)
			return

		ProcessReplaceEntityWithPrefab()
	}

//-----------------------------------------------------------------
	void OnCopySelectionFromGUI()
	{
		if(!World.EditorActive)
			return

		ExtClass EntCopy

		if(SelectedNum == 0)
			return
		
		NumCopiedEntities = 0

		for(int s = 0; s < SelectedNum; s++)
		{
			EntCopy = Selected[s]
			CopyClassName[s] = ClassName(EntCopy)
			CopyName[s] = EntCopy._name
			CopyDefPtr[s] = EntCopy.DefPtr
			NumCopyValues[s] = EntCopy.DefPtr.NumKeys
			CopyStates[s] = EntCopy._userstates
			NumCopiedEntities++

			int n			
			for(n = 0; n < EntCopy.DefPtr.NumKeys; n++)
				CopyKeyValues[s][n] = EntCopy.EKeyValues[n]
				
			for(n = 0; n < EntCopy.DefPtr.NumKeys; n++)
				CopyKeyTypes[s][n] = EntCopy.EKeyTypes[n]
		}
		MemSelectionOffsets(CopyOffsets)
	}

//-----------------------------------------------------------------
	void OnPasteSelectionFromGUI()
	{
		if(!World.EditorActive)
			return

		if(EntIsWritable() == false)
		{
			RequestDialog("File is Read Only!", "Error", GUI_DIALOG_OK)
			return
		}

		if(Ent)
			return

		if(NumCopiedEntities == 0)
			return

		UnselectAll()
		string UsedPrefabs[16]
		int PrefabIndex[16]
		int PrefabUniqueIncrements[16]
		int UsedPrefabsNum = 0
		string CodedPrefabID
		string RealPrefabID
		string RealPrefabEntID
		
		for(int p = 0; p < NumCopiedEntities; p++)
		{
			if(CopyClassName[p] == "")
				continue

			string Cname = CopyName[p]

			if(strlen(Cname) > 0)
			{
				if(IsNumber(substr(Cname, strlen(Cname) - 1, 1)))	//ak je koncovka cislo
				{
					Cname = ClearIncerementFromName(Cname)		
					NameForNew = GetIncrementNameForNew(Cname)				
				}
				else
					NameForNew = ""
			}
			else
				NameForNew = ""

			if(NameForNew == "")
			{
	again:
				if(!TextEntryDialog(NameForNew, "Name of new entity"))	//user si to rozmyslel
				{
					OnUpdateSelection("", "", "", false)
					return
				}
				else
				{
					while(DuplicityTestOK(NameForNew) == false)
					{
						ShowDuplicityError()
						goto again
					}
				}
				
				if(NameForNew == "")	//odklepol ok ale nezadal nic
				{
					RequestDialog("Zadny nazev pro entitu !!!", "Error", GUI_DIALOG_OK)
					goto again
				}
			}

			ExtClass EntNew = NewEntity(CopyClassName[p], end)  
	
			if(!EntNew)
				continue

			EntNew.DefPtr = CopyDefPtr[p]
	
			int n			
			for(n = 0; n < NumCopyValues[p]; n++)
				EntNew.EKeyValues[n] = CopyKeyValues[p][n]
				
			for(n = 0; n < NumCopyValues[p]; n++)
				EntNew.EKeyTypes[n] = CopyKeyTypes[p][n]

			EntNew._userstates = CopyStates[p]
			
			if(IsPrefabed(EntNew))
			{
				CodedPrefabID = EntNew.GetKeyValue("prefab")
				RealPrefabID = DecodePrefabNameFromKey(CodedPrefabID)
//				RealPrefabEntID = DecodePrefabEntNameFromKey(CodedPrefabID)
//				return PrefManager.GetPrefab(RealPrefabID)
				
				bool exist = false				
		
				for(int k = 0; k < UsedPrefabsNum; k++)
				{
					if(RealPrefabID == UsedPrefabs[k])	
					{
						PrefabIndex[p]	= k
						exist = true
						break
					}
				}
				
				if(exist == false)
				{
					UsedPrefabs[UsedPrefabsNum] = RealPrefabID
					PrefabIndex[p] = UsedPrefabsNum++
				}
			}
			else
				PrefabIndex[p]	= -1
		}

		if(UsedPrefabsNum > 0)	//ked su entity prefabovane tak im musime zvysit instance ID a ked maju spolocny prefab tak instance ID bude rovnake ale iba v pripade ze v samotnom prefabe sa entita vola inac
		{
			for(p = 0; p < UsedPrefabsNum; p++)
			{
				World.PrefabsIncrement = atoi(World.GetKeyValue("PrefabsIncrement"))
				AddKeyValueToEntity(World, "PrefabsIncrement", itoa(++World.PrefabsIncrement))
				PrefabUniqueIncrements[p] = World.PrefabsIncrement 
			}

			ExtClass CurEnt
			string NewCodedPrefabID
			string RealPrefabIDs[32]
			string RealPrefabEntIDs[32]
			int NumAddedIncrements = 0
	
			for(p = 0; p < SelectedNum; p++)
			{
				CurEnt = Selected[SelectedNum - 1 - p]

				if(PrefabIndex[p] != -1)
				{
					CodedPrefabID = CurEnt.GetKeyValue("prefab")//UsedPrefabs[PrefabIndex[p]]
					RealPrefabID = DecodePrefabNameFromKey(CodedPrefabID)
					RealPrefabEntID = DecodePrefabEntNameFromKey(CodedPrefabID)

					RealPrefabIDs[NumAddedIncrements] = RealPrefabID 
					RealPrefabEntIDs[NumAddedIncrements] = RealPrefabEntID
	
					bool NeedExtraIncrement = false
					for(k = 0; k < NumAddedIncrements; k++)
					{
						string CurAdedPrefabID = RealPrefabIDs[k]
						string CurAdedPrefabEntID = RealPrefabEntIDs[k]
		
						if(CurAdedPrefabID == RealPrefabID && CurAdedPrefabEntID == RealPrefabEntID)	//kopirovane entity s rovnakym prefabom ale aj rovnakym nazvom v prefabe nemozu mat rovnake instanceID takze pre nich dame uplne nove cislo
						{
							NeedExtraIncrement = true
							break
						}
					}
	
					NumAddedIncrements++
	
					int NewIncrement
					if(NeedExtraIncrement)
					{
						World.PrefabsIncrement = atoi(World.GetKeyValue("PrefabsIncrement"))
						AddKeyValueToEntity(World, "PrefabsIncrement", itoa(++World.PrefabsIncrement))
						NewIncrement = World.PrefabsIncrement
					}
					else
						NewIncrement = PrefabUniqueIncrements[PrefabIndex[p]]

					CodedPrefabID = RealPrefabID + ";" + itoal(NewIncrement, 6) + ";" + RealPrefabEntID	//do kluca "prefab si zakodujeme ID prefabu a ID definicie entity v prefabe"
					AddKeyValueToEntity(CurEnt, "prefab", CodedPrefabID)
				}
			}
		}

		ExtClass Ptrs[16]
		int		PtrsNum = SelectedNum

		for(p = 0; p < SelectedNum; p++)
			Ptrs[p] = Selected[p]

		UnselectAll()

		for(p = 0; p < PtrsNum; p++)
			AddSelection(Ptrs[p])

		g_NewEntityCoords = end
		vector xxx = GetTracPos(MainSel, g_NewEntityCoords + "0 0 50", "0 0 -500")
		AddKeyValueToEntity(MainSel, "coords", vectoa(xxx))
		TransformateSelectedEntities(CopyOffsets)		//nastavime pozicie a uhly ostatnych selektnutych entit a reloadneme ich
		ReplaceSelection(MainSel, ReloadEntity(MainSel, ClassName(MainSel)))
		ThrowEvent(MainSel, EV_INIT)
		ThrowEvent(MainSel, EV_LOADED)

		AddNamesToGUI(Selected, SelectedNum)
		ShowPropertiesOfSelecting()
//		ChangeKeyInGUI("coords", vectoa(xxx), "vector")
		OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
		UpdateEntityLinks(true)
		OnDataChange()
	}

//-----------------------------------------------------------------
	void OnDeleteSelectionFromGUI()
	{
		if(!World.EditorActive)
			return

		if(EdMode != EDITMODE_ENTITY)	//osetrenie nutnej prasarny. v GUI mame iba jedno delete a tak vola toto ale aj PathEditoru OnDeleteSelectedAreaFromGUI()
			return

		if(!MainSel)
			return

		ExtClass CurSel
		for(int n = 0; n < SelectedNum; n++)
		{
			CurSel = Selected[n]

			if(CurSel._name == "player")
			{
				RequestDialog("Cannot delete creature_player entity!", "Delete", GUI_DIALOG_OK)
				return
			}
		}

		if(SelectedNum > 1)	//na mazanie viacerych entit sa radsej este spytame
		{
			if(RequestDialog("Really delete " + itoa(SelectedNum) + " entities?", "Delete", GUI_DIALOG_YES_NO) == GUI_REQUEST_NO)
				return
		}

		ExtClass CurEnt
		string DelNames[64]
		string DelClassNames[64]
		int DelNum = SelectedNum
		for(n = 0; n < SelectedNum; n++)
		{
			CurEnt = Selected[n]
			DelNames[n] = CurEnt._name
			DelClassNames[n] = ClassName(CurEnt)
		}

		ExtClass EntDel
		while(MainSel)
		{
			EntDel = MainSel
			RemoveSelection(MainSel)

			if(IsBrushEntity(EntDel) == false)
			{
				delete EntDel
				EntDel = NULL
			}
		}

		EntityListRemove(DelNames, DelClassNames, DelNum)
		Ent = NULL
		UpdateEntityLinks(true)
		OnUpdateSelection("", "", "", false)
	}

//-----------------------------------------------------------------
	void OnRenameSelectionFromGUI()
	{
		if(!World.EditorActive)
			return

		ExtClass ToRename
		
		if(!MainSel && !Ent)
			return
		
		if(Ent)
			ToRename = Ent
		else
			ToRename = MainSel
			
		string oldname = ToRename._name
		string newname = oldname
		
again:
		if(!TextEntryDialog(newname, "Name of new entity"))
			return

		while(DuplicityTestOK(newname) == false)
		{
			ShowDuplicityError()
			goto again
		}

		ExtClass ToRenameArray[1]
		ToRenameArray[0] = ToRename
		RemoveNamesFromGUI(ToRenameArray, 1)

		ToRename.SetName(newname)
		ExtClass EntRename = ReloadEntity(ToRename, ClassName(ToRename))
		ReplaceSelection(ToRename, EntRename)

		ThrowEvent(EntRename, EV_INIT)
		ThrowEvent(EntRename, EV_LOADED)

		UpdateEntityLinks(true)
		ToRenameArray[0] = EntRename
		AddNamesToGUI(ToRenameArray, 1)
		
		if(MainSel)
			OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
	}

//-----------------------------------------------------------------
	void RenameClassNameFromGUI()
	{
		if(!World.EditorActive)
			return

		ExtClass ToRename
		
		if(!MainSel)
			return
		
		if(!IsInherited(MainSel, CCreatureClass))
		{
			RequestDialog("Class change works only on creatures!", "Error", GUI_DIALOG_OK )	
			return
		}
		
		ToRename = MainSel
			
		string oldname = ClassName(ToRename)
		string newname = oldname
	
again:
		if(!TextEntryDialog(newname, "Enter type (class) to overwrite current"))
			return

		if(!DefManager.GetDefinition(newname))
		{
			string ErrMsg = "Unknown class type " + newname
			RequestDialog(ErrMsg, "Error", GUI_DIALOG_OK )	
			goto again
		}

		ExtClass EntRename = ReloadEntity(ToRename, newname)
		ReplaceSelection(ToRename, EntRename)
		ThrowEvent(EntRename, EV_INIT)
		ThrowEvent(EntRename, EV_LOADED)
		UpdateEntityLinks(true)
		
		if(MainSel)
			OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
	}
	
//-----------------------------------------------------------------
	void EditCoordsUpdate(bool SnapToGrid)
	{
		vector gr
		gr[0] = grid
		gr[1] = grid
		gr[2] = grid
		vector FinalPos = g_NewEntityCoords
				
		if(ShowGrid != 0 && SnapToGrid)
			FinalPos = SnapVector(FinalPos, gr)

		MainSel.OnPositionChangingInEditor(FinalPos, g_AngleVal)	//update hlavnej entity
		ChangeKeyInGUI("coords", vectoa(FinalPos), "vector")

		vector MainSelMat2[4]
		ExtClass OtherEnt2
		vector OtherPos2, OtherAng2
		RollPitchYawMatrix2(g_AngleVal, MainSelMat2)
		MainSelMat2[3] = FinalPos

		for(int n = 1; n < SelectedNum; n++)
		{
			OtherEnt2 = Selected[n]
			OtherPos2 = VectorMatrixMultiply3(MainSelMat2, MovingOffset[n])
			OtherPos2 = FinalPos + OtherPos2
			OtherAng2[0] = atoi(GetValueFromKey(OtherEnt2, "angle3"))
			OtherAng2[1] = atoi(GetValueFromKey(OtherEnt2, "angle"))
			OtherAng2[2] = atoi(GetValueFromKey(OtherEnt2, "angle2"))

			vector AngOff = g_AngleVal - AnglesBeforeChange
			AngOff = FixAngles(AngOff)

			OtherAng2 = AngOff + OtherAng2
			OtherAng2 = FixAngles(OtherAng2)

			if(!OtherEnt2.ParentName)
				OtherEnt2.OnPositionChangingInEditor(OtherPos2, OtherAng2)	//update ostatnych selektnutych entit
		}
		UpdateWayEnts()
	}

//-----------------------------------------------------------------
	void DropEntity(bool SnapToGrid)	//koniec posuvania
	{
		if(!MainSel)
			return

		int n
		string EntClName = ClassName(MainSel)
		vector grd
		grd[0] = grid
		grd[1] = grid
		grd[2] = grid
		vector FinePos = g_NewEntityCoords

		if(MainSel.ParentName)
		{
			//FinePos = GetMatrix(MainSel, 3)
			FinePos = atov(MainSel.GetKeyValue("coords"))	//pocas movingu sa ten kluc meni
		}
		else
		{
			if(ShowGrid != 0 && SnapToGrid)
				FinePos = SnapVector(FinePos, grd)
		}
		
		ChildEntNamesNum = 0						//vytiahneme vsetky nazvy parentov zo selektnutych entit
		for(n = 0; n < SelectedNum; n++)
		{
			GrabChildsNamesFrom(Selected[n])
			SavePositionsOfChilds(Selected[n])				//hned ulozime aktualne pozicie vsetkych childov do klucov
		}

		SetEntitySizeKeys(MainSel, g_ValMins, g_ValMaxs)
		SetEntityOrientationKeys(MainSel, FinePos, g_AngleVal)
		TransformateSelectedEntities(MovingOffset)		//nastavime pozicie a uhly ostatnych selektnutych entit a reloadneme ich
		ReplaceSelection(MainSel, ReloadEntity(MainSel, ClassName(MainSel)))
		ThrowEvent(MainSel, EV_INIT)
		ThrowEvent(MainSel, EV_LOADED)

		ReloadEntitiesFromList(ChildEntNames, ChildEntNamesNum)	//reloadneme childy selektnutych entit v poradi konstruktor/init/loaded

		UpdateEntityLinks(true) 
		OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
		OnDataChange()
		ChangeKeyInGUI("coords", vectoa(FinePos), "vector")
		ChangeKeyInGUI("angle", ftoa(g_AngleVal[1]), "float")
//		ChangeKeyInGUI("angle2", ftoa(g_AngleVal[2]), "float")
//		ChangeKeyInGUI("angle3", ftoa(g_AngleVal[0]), "float")

		g_ValMins = ZeroVec
		g_ValMaxs = ZeroVec
		g_AngleVal = ZeroVec
		
		Status = ST_SELECTING
		ClipCursorInsideWindow(false)
		ShowCursorPernamently(true)
		ClearKey(KC_LEFT)
		ClearKey(KC_RIGHT)
		ClearKey(KC_UP)
		ClearKey(KC_DOWN)
		ClearKey(KC_ADD)
		ClearKey(KC_SUBTRACT)
	}
	
//-----------------------------------------------------------------
	void OnKeyChangeFromGUI(string key, string val)
	{
		if(!World.EditorActive)
			return

		if(!MainSel)
			return

		if(MainSel == World)	//world sa nesmie reloadovat ani transformovat!
		{
			AddKeyValueToEntity(MainSel, key, val)
			OnDataChange()
			return	
		}

		int n
		ExtClass CurSel

		if(PickEntity())
		{
			Status = ST_EDITCOORDS
			
			if(key == "angle" || key == "angle2" || key == "angle3")
			{
				float ang = atof(MainSel.GetKeyValue("angle"))
				float ang2 = atof(MainSel.GetKeyValue("angle2"))
				float ang3 = atof(MainSel.GetKeyValue("angle3"))

				if(key == "angle")
					g_AngleVal = Vector(ang3, atof(val), ang2)
					
				if(key == "angle2")
					g_AngleVal = Vector(ang3, ang, atof(val))
					
				if(key == "angle3")
					g_AngleVal = Vector(atof(val), ang, ang2)
			}
			else
			{
				for(int s = 0; s < SelectedNum; s++)
				{
					CurSel = Selected[s]
					AddKeyValueToEntity(CurSel, key, val)
				}	
			}

			if(key == "coords")
			{
				if(MainSel.ParentName)
					g_NewEntityCoords = GetOrigin(MainSel)
				else
					g_NewEntityCoords = atov(val)

				AddKeyValueToEntity(MainSel, key, val)
			}

			EditCoordsUpdate(false)
			DropEntity(false)

			int NumPrefabed = 0
	
			for(s = 0; s < SelectedNum; s++)
			{
				CurSel = Selected[s]
	
				if(IsPrefabed(CurSel) == true)
				{
					NumPrefabed++
					ChangePrefabFromEntity(CurSel)
				}
			}
	
			if(NumPrefabed > 0)
			{
				PrefManager.SavePrefabs()
				WritePrefabsValuesToAllEntities()		//prepiseme vsetky hodnoty prefabovanych entit hodnotami z prefabov
			}
	
			UpdateEntityLinks(true) 
			OnDataChange()
		}
	}

//-----------------------------------------------------------------
	void OnLMBupWhenSelecting()
	{
		int Shift = EdKeyStates[wx_SHIFT]

		if(Ent)			//mame nieco pod kurzorom
		{
			if(MainSel)	//mame uz nieco selektnute
			{
				if(Ent == MainSel)
				{
					RemoveSelection(Ent)
					
					if(MainSel)
					{
						ShowPropertiesOfSelecting()
						OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
					}
					else
						OnUpdateSelection("", "", "", false)

					return
				}
				else
				{
					if(Shift == false)
						UnselectAll()

					AddSelection(Ent)
					ShowPropertiesOfSelecting()
					OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
					return
				}
			}
			else	//nemame este nic selektnute
			{
				if(Shift == true)
				{
					AddSelection(Ent)
					ShowPropertiesOfSelecting()
					OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
				}
				else
				{
					if(IsPrefabed(Ent))
						SelectPrefab(GetPrefabOfEntity(Ent), DecodePrefabInstanceIncerementFromKey(Ent.GetKeyValue("prefab")))
					else
						AddSelection(Ent)
					
					ShowPropertiesOfSelecting()
					OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
				}

				return
			}
		}
		else
		{
			UnselectAll()
			OnUpdateSelection("", "", "", false)
			return
		}	
	}

//-----------------------------------------------------------------
/*	int ShowUserPopupMenu()
	{
		AddUserPopupItem("item 0", 0, true)
		AddUserPopupItem("item 1", 1, true)
		AddUserPopupItem("item 2", 2, true)
		int result = PopupUserMenu()
		
//		if(result == 1)
//			kliknuto na "item 1"
		
		return result
	}*/

//-----------------------------------------------------------------
	void OnRMBupWhenSelecting()
	{
		if(Ent)
		{
			if(Ent == MainSel)
				ShowEntityPopupMenu()
/*			else
			{
				RemoveUserPoupItems()
				
				AddEditorPopupItem("copy", 1000, true)		//label id active
				AddEditorPopupItem("paste", 1001, false)
				AddEditorPopupItem("delete", 1002, true)
				
				int result = Ent->ShowUserPopupMenu()
				
				
//				if(result == 2)
				
			}*/
				
/*			if(MainSel)
			{
				if(Ent == MainSel)
				{
//					Status = ST_EDITKEYS	//budeme editovat kluce entity
					return
				}
				else
				{
					if(IsSelected(Ent) == false)
						UnselectAll()

					AddSelection(Ent)
//					Status = ST_EDITKEYS	//budeme editovat kluce entity
					return
				}
			}
			else
			{
				UnselectAll()
				AddSelection(Ent)
//				Status = ST_EDITKEYS	//budeme editovat kluce entity
				return
			}*/
		}
		else
		{
			ProcessAddEntity()
			EdKeyStates[wx_SHIFT] = 0	//hack. popup sposobi ze ked je klavesa stlacena tak sa uz nevyvola keyup event. potom vyriestit systemovo
//			Status = ST_NEWENTITY
			return
		}
	}

//-----------------------------------------------------------------
	void OnSelectFromGUI(string EntName)
	{
		if(!World.EditorActive)
			return
		
		ExtClass ent = FindClass(EntName)
		
		if(!ent)
			return

		UnselectAll()
		AddSelection(ent)
		ShowPropertiesOfSelecting()
		OnUpdateSelection(MainSel._name, ClassName(MainSel), GetPrefabNameOfEntity(MainSel), true)
	}

//-----------------------------------------------------------------
	void OnKeyDownWhenEditCoords()
	{
		
	}

//-----------------------------------------------------------------
	void OnLMBUpWhenEditCoords()
	{
		DropEntity(true)
	}

//-----------------------------------------------------------------
	void OnRMBUpWhenEditCoords()
	{
		DropEntity(true)
	}

//-----------------------------------------------------------------
	bool IsEntitySizable(ExtClass ent)
	{
		bool sizable = false
		vector mins, maxs
		GetBoundBox(ent, mins, maxs)

		vector tm1, tm2
		if(GetValue(ent, "mins", 0, tm1) && GetValue(ent, "maxs", 0, tm2))
			sizable = true
		
		return sizable
	}

//-----------------------------------------------------------------
	void OnMouseMoveWhenEditCoords()
	{
		if(!World.EditorActive)
			return
/*		ExtClass cc
		for(cc = GetNextEntity(NULL, CExtClass); cc != NULL; cc = GetNextEntity(cc, CExtClass))
			UpdateEntity(cc)*/
		
		if(!MainSel)
			return
		
		if(GUIKeyState(wx_CONTROL, false))
		{
			if(IsEntitySizable(MainSel))
			{
				int CameraFactor = GetCameraPosFactor()
	
				if(CameraFactor == 1 || CameraFactor == 3)
				{
					g_ValMaxs[0] = g_ValMaxs[0] + EdMouseDeltaY
					g_ValMins[0] = g_ValMins[0] - EdMouseDeltaY
					g_ValMaxs[1] = g_ValMaxs[1] + EdMouseDeltaX
					g_ValMins[1] = g_ValMins[1] - EdMouseDeltaX
				}
	
				if(CameraFactor == 2 || CameraFactor == 4)
				{
					g_ValMaxs[0] = g_ValMaxs[0] + EdMouseDeltaX
					g_ValMins[0] = g_ValMins[0] - EdMouseDeltaX
					g_ValMaxs[1] = g_ValMaxs[1] + EdMouseDeltaY
					g_ValMins[1] = g_ValMins[1] - EdMouseDeltaY
				}

				if(g_ValMaxs[0] < 1)
					g_ValMaxs[0] = 1
					
				if(g_ValMins[0] > -1)
					g_ValMins[0] = -1
					
				if(g_ValMaxs[1] < 1)
					g_ValMaxs[1] = 1
					
				if(g_ValMins[1] > -1)
					g_ValMins[1] = -1
					
				if(g_ValMaxs[2] < 1)
					g_ValMaxs[2] = 1
					
				if(g_ValMins[2] > -1)
					g_ValMins[2] = -1
	
				SetValue(MainSel, "mins", 0, g_ValMins)
				SetValue(MainSel, "maxs", 0, g_ValMaxs)
				AddKeyValueToEntity(MainSel, "mins", vectoa(g_ValMins))
				AddKeyValueToEntity(MainSel, "maxs", vectoa(g_ValMaxs))
				ChangeKeyInGUI("mins", vectoa(g_ValMins), "vector")
				ChangeKeyInGUI("maxs", vectoa(g_ValMaxs), "vector")
				OnDataChange()
			}
		}
		else
		{
			vector Up = "0 0 1"
			vector Left = g_CamMat[0]	//"1 0 0"
			vector Forward = Left * Up

			if(RMB)
			{
				vector off3 = Up * (float)EdMouseDeltaY
				g_NewEntityCoords = g_NewEntityCoords - off3
			}
			else
			{
				vector off1 = Forward * (float)EdMouseDeltaY
				vector off2 = Left * (float)EdMouseDeltaX
				g_NewEntityCoords = g_NewEntityCoords - off1
				g_NewEntityCoords = g_NewEntityCoords - off2
			}
			AddKeyValueToEntity(MainSel, "coords", vectoa(GetMatrix(MainSel, 3)))	//pocas posuvania zapisujeme poziciu z matice pretoze entita ktorou pohybujeme moze byt naparentena na inu
		}
		EditCoordsUpdate(true)
	}

//-----------------------------------------------------------------
	void OnMouseLeaveFromGUI()
	{
		if(!World.EditorActive)
			return

		if(EdMode == EDITMODE_ENTITY && Status == ST_SELECTING)
			Ent = NULL
	}

//-----------------------------------------------------------------
	void OnMouseDownFromGUI(int button)
	{
		if(!World.EditorActive)
			return

		if(button == 0)
			EdLMBdown = true

		if(button == 2)
			EdRMBdown = true
		
		if(CameraMode == false)
		{
			if(EdMode == EDITMODE_PATH)
				MainUpdateFromGUI()
		}
	}

//-----------------------------------------------------------------
	void OnMouseUpFromGUI(int button)
	{
		if(!World.EditorActive)
			return

		if(button == 0)
		{
			EdLMBdown = false

			if(CameraMode == false)
			{
				if(EdMode == EDITMODE_PATH)
					MainUpdateFromGUI()
					
				if(EdMode == EDITMODE_ENTITY)
				{
					if(Status == ST_SELECTING)
						OnLMBupWhenSelecting()
						
					if(Status == ST_EDITCOORDS)
						OnLMBUpWhenEditCoords()
				}
			}
		}

		if(button == 2)
		{
			EdRMBdown = false

			if(CameraMode == false)
			{
				if(EdMode == EDITMODE_PATH)
					MainUpdateFromGUI()
				
				if(EdMode == EDITMODE_ENTITY)
				{
					if(Status == ST_SELECTING)
						OnRMBupWhenSelecting()
		
					if(Status == ST_EDITCOORDS)
						OnRMBUpWhenEditCoords()
				}
			}
		}
	}

//-----------------------------------------------------------------
	void OnMouseMoveFromGUI(int PosX, int PosY, int DeltaX, int DeltaY)	//vola GUI editor ked za zmeni pozicia mysi
	{
		if(!World.EditorActive)
			return

		EdMouseX = PosX
		EdMouseY = PosY
		EdMouseDeltaX = DeltaX
		EdMouseDeltaY = DeltaY
		
		EdScreenMouseX = EdMouseX
		EdScreenMouseY = EdMouseY
		ToScreenPos(EdScreenMouseX, EdScreenMouseY)

		if(CameraMode)	//iba hybeme kamerou
		{
			float CamStepX = (float)EdMouseDeltaX * 0.3
			float CamStepY = (float)EdMouseDeltaY * 0.3
			
			if(!g_InvMouse)
				CamStepY = -CamStepY
			
			World.CameraAngs[0] = World.CameraAngs[0] - CamStepY
			World.CameraAngs[1] = World.CameraAngs[1] - CamStepX

			if(World.CameraAngs[0] < -89)
				World.CameraAngs[0] = -89

			if(World.CameraAngs[0] > 89)
				World.CameraAngs[0] = 89

//			SetCamera(World.CameraPos, World.CameraAngs)
//			CalcMatrixFromCamera()
			ClipCursorInsideWindow(true)
			ClampCursorInsideWindow()
		}
		else
		{
			if(EdMode == EDITMODE_PATH)
				MainUpdateFromGUI()

			if(EdMode == EDITMODE_ENTITY)
			{
				if(Status == ST_SELECTING)
					ProcessSelecting()
		
				if(Status == ST_EDITCOORDS)
					OnMouseMoveWhenEditCoords()
			}
		}
	}

//-----------------------------------------------------------------
	void OnMouseWheelFromGUI(int delta)
	{
		if(!World.EditorActive)
			return

		if(MainSel && Status == ST_SELECTING)
		{
			if(GUIKeyState(wx_CONTROL, false))	//sizujeme triggery a po
			{
				if(IsEntitySizable(MainSel))
				{
					if(PickEntity())
					{
						g_ValMins = atov(MainSel.GetKeyValue("mins"))
						g_ValMaxs = atov(MainSel.GetKeyValue("maxs"))
						g_NewEntityCoords = atov(MainSel.GetKeyValue("coords"))
	
						if(delta > 0)
						{
							g_ValMaxs[2] = g_ValMaxs[2] + (float)grid
							g_ValMins[2] = g_ValMins[2] - (float)grid
							g_NewEntityCoords[2] = g_NewEntityCoords[2] + (float)grid
						}
						else
						{
							g_ValMaxs[2] = g_ValMaxs[2] - (float)grid
							g_ValMins[2] = g_ValMins[2] + (float)grid
							g_NewEntityCoords[2] = g_NewEntityCoords[2] - (float)grid
						}
						
						if(g_ValMaxs[0] < 1)
							g_ValMaxs[0] = 1
							
						if(g_ValMins[0] > -1)
							g_ValMins[0] = -1
							
						if(g_ValMaxs[1] < 1)
							g_ValMaxs[1] = 1
							
						if(g_ValMins[1] > -1)
							g_ValMins[1] = -1
							
						if(g_ValMaxs[2] < 1)
							g_ValMaxs[2] = 1
							
						if(g_ValMins[2] > -1)
							g_ValMins[2] = -1
			
						SetValue(MainSel, "mins", 0, g_ValMins)
						SetValue(MainSel, "maxs", 0, g_ValMaxs)
						AddKeyValueToEntity(MainSel, "mins", vectoa(g_ValMins))
						AddKeyValueToEntity(MainSel, "maxs", vectoa(g_ValMaxs))
						AddKeyValueToEntity(MainSel, "coords", vectoa(g_NewEntityCoords))
						OnDataChange()
						ChangeKeyInGUI("mins", vectoa(g_ValMins), "vector")
						ChangeKeyInGUI("maxs", vectoa(g_ValMaxs), "vector")
						EditCoordsUpdate(true)
						DropEntity(true)
					}
				}
			}
			else
			{
				if(PickEntity())
				{
					Status = ST_EDITCOORDS
					int ang_
					float ang__
		
					if(GetValue(MainSel, "angle", 0, ang_) || GetValue(MainSel, "angle", 0, ang__))
					{
						if(delta > 0)
							g_AngleVal[1] = g_AngleVal[1] + 5
						else
							g_AngleVal[1] = g_AngleVal[1] - 5
					}
			
					g_AngleVal[0] = FixAngle(g_AngleVal[0])
					g_AngleVal[1] = FixAngle(g_AngleVal[1])
					g_AngleVal[2] = FixAngle(g_AngleVal[2])
					
					EditCoordsUpdate(false)
					DropEntity(false)
				}
			}
		}
	}

//-----------------------------------------------------------------
	int GetCameraMode()	//pre editor
	{
		return CameraMode
	}

//-----------------------------------------------------------------
	int IsMovementKeyPressed()
	{
		if(EdKeyStates[wx_W] == 1 || EdKeyStates[wx_S] == 1 || EdKeyStates[wx_A] == 1 || EdKeyStates[wx_D] == 1)
			return 1
		
		return 0
	}

	int LastTime

//-----------------------------------------------------------------
	void CheckForExactMoveKeys(int wxKeyCode)
	{
		int MoveFromKey = 0

		if(wxKeyCode == wx_LEFT)
			MoveFromKey = 1

		if(wxKeyCode == wx_DOWN)
			MoveFromKey = 2

		if(wxKeyCode == wx_RIGHT)
			MoveFromKey = 3
						
		if(wxKeyCode == wx_UP)
			MoveFromKey = 4

		int CameraFactor = GetCameraPosFactor()

		if(MoveFromKey)
		{
			int AddAng = MoveFromKey + CameraFactor

			if(AddAng > 4)
				AddAng = 0 + (AddAng - 4)
			
			MoveFromKey = AddAng
		}
			
		if(wxKeyCode == wx_ADD)
			MoveFromKey = 5

		if(wxKeyCode == wx_SUBTRACT)
			MoveFromKey = 6
			
		if(MoveFromKey)
		{
			if(PickEntity())
			{
				float Step = grid
	
				if(MoveFromKey == 1)	//left				//vo worldovych suradniciach ale smer prisposobeny pohladu kamery
					g_NewEntityCoords[0] = g_NewEntityCoords[0] - Step
	
				if(MoveFromKey == 2)	//down
					g_NewEntityCoords[1] = g_NewEntityCoords[1] - Step
	
				if(MoveFromKey == 3)	//right
					g_NewEntityCoords[0] = g_NewEntityCoords[0] + Step
													
				if(MoveFromKey == 4)	//up
					g_NewEntityCoords[1] = g_NewEntityCoords[1] + Step
					
				if(MoveFromKey == 5)	//upZ
					g_NewEntityCoords[2] = g_NewEntityCoords[2] + Step
					
				if(MoveFromKey == 6)	//downZ
					g_NewEntityCoords[2] = g_NewEntityCoords[2] - Step

				EditCoordsUpdate(true)
				DropEntity(true)
			}
		}
	}

//-----------------------------------------------------------------
	void OnKeyDownFromGUI(int wxKeyCode)
	{
		if(!World.EditorActive)
			return

		if(EdKeyStates[wxKeyCode] == -1)	//je vyclearovana
			return

		int MovementBefore = IsMovementKeyPressed()
		EdKeyStates[wxKeyCode] = 1
		int MovementAfter = IsMovementKeyPressed()
		
		if(MovementBefore == 0 && MovementAfter == 1)
			LastTime = GetWindowsCurrentTime()
		
		if(wxKeyCode == wx_SPACE)
		{
			CameraMode = 1 - CameraMode
//			SetGUICursorType(CameraMode)
			ClipCursorInsideWindow(CameraMode)
			ShowCursorPernamently(1 - CameraMode)
		}

		if(CameraMode == false)
		{
			if(EdMode == EDITMODE_PATH)
				MainUpdateFromGUI()
		}

		if(MainSel)
			CheckForExactMoveKeys(wxKeyCode)
	}

//-----------------------------------------------------------------
	void OnKeyUpFromGUI(int wxKeyCode)
	{
		if(!World.EditorActive)
			return

		EdKeyStates[wxKeyCode] = 0
		
		if(CameraMode == false)
		{
			if(EdMode == EDITMODE_PATH)
				MainUpdateFromGUI()
		}
	}

//-----------------------------------------------------------------
	void BeforeRenderFromGUI()
	{
		if(!World.EditorActive)
			return

		SetCamera(World.CameraPos, World.CameraAngs)
		CalcMatrixFromCamera()

		if(EdMode == EDITMODE_ENTITY)
		{
			VizualizeSelected()
	
			if(!CameraMode)
			{
				if(Status == ST_SELECTING)
				{
					if(Ent)
						ShowBBoxOfEntity(Ent, 0x0f00ffff)
				}
			}
		}
		
		if(EdMode == EDITMODE_PATH)
		{
			UpdateArea()
			
			if(!CameraMode)
				DrawGrid(ShowGrid, end)	
		}
	}

//-----------------------------------------------------------------
	void OnActivateWindow(bool active)	//aktivuje/deactivuje sa okno modulu s editorom
	{
/*		if(active)
		{
			SetCamera(World.CameraPos, World.CameraAngs)
			CalcMatrixFromCamera()	
		}*/
	}

//-----------------------------------------------------------------
	int OnIdleFromGUI()	//vraciame true ked chceme aby to bolo volane frekventovane
	{
		if(!World.EditorActive)
			return 0

		int CurTime = GetWindowsCurrentTime()
		int Diff = CurTime - LastTime
		float EdFtime = (float)Diff * 0.1
		float MovementSpeed = CameraMovementSpeed	//default 2
		
		if(EdKeyStates[wx_SHIFT])
			MovementSpeed *= 5
		
		LastTime = CurTime

		clamp EdFtime<0.01, 2.6>	//v dobe od stlacenia klavesy pre pohyb po prvy event OnIdle() moze prejst dlha doba takze LastTime je potom prilis velky

		if(CameraMode || EdMode == EDITMODE_PATH)
		{
			if(IsMovementKeyPressed())
			{
				OnCameraMove()
				vector CamStep = ZeroVec
				float step = EdFtime * MovementSpeed//1 //* EdFtime//0.2f * ftime;

				vector mat[4]
				vector angs = World.CameraAngs
				float flip = angs[0]
				angs[0] = -flip
				RollPitchYawMatrix2(angs, mat)
	
				vector LeftVec = mat[1]
				LeftVec = -LeftVec
				vector ForwardVec = mat[0]
	
				if(EdKeyStates[wx_D] == 1)	//vpravo
					CamStep += LeftVec
	
				if(EdKeyStates[wx_A] == 1)	//vlavo
					CamStep -= LeftVec
	
				if(EdKeyStates[wx_W] == 1)	//dopredu
					CamStep += ForwardVec
	
				if(EdKeyStates[wx_S] == 1)	//dozadu
					CamStep -= ForwardVec
	
				VectorNormalize(CamStep)
				CamStep = CamStep * step
				World.CameraPos += CamStep
	
	//			if(EdMouseDelatX == 0 && EdMouseDelatX == 0)
	//			{
	//				SetCamera(World.CameraPos, World.CameraAngs)
	//				CalcMatrixFromCamera()
	//				VizualizeSelected()
	//			}
				return true
			}
		}

		if(!CameraMode)
		{
			if(EdMode == EDITMODE_PATH)
			{
				if(mode == 1)
				{
					if(NumPoints && IsAppActive())
					{
						ClipCursorInsideWindow(true)	//nepusti kurzor z plochy okna
						int MinX = 0
						int MinY = 0
						int MaxX = g_iWidth - 3
						int MaxY = g_iHeight - 3
						float Tstep = EdFtime// * 2
	
						if(EdMouseX <= MinX)
							World.CameraAngs[1] = World.CameraAngs[1] + Tstep
	
						if(EdMouseX >= MaxX)
							World.CameraAngs[1] = World.CameraAngs[1] - Tstep
	
						if(EdMouseY <= MinY)
							World.CameraAngs[0] = World.CameraAngs[0] + Tstep
	
						if(EdMouseY >= MaxY)
							World.CameraAngs[0] = World.CameraAngs[0] - Tstep
							
						if(World.CameraAngs[0] < -60)
							World.CameraAngs[0] = -60

						if(World.CameraAngs[0] > 60)
							World.CameraAngs[0] = 60
	
						return true
					}
					else
						ClipCursorInsideWindow(false)
				}
				else
					ClipCursorInsideWindow(false)
			}
		}

		return false
	}




//===============================================================================================================
//=== NEW EDITOR ================================================================================================
//===============================================================================================================



//-----------------------------------------------------------------
	void EOnFrame(MinClass other, int extra)
	{
		CalcMatrixFromCamera()
	}

//-----------------------------------------------------------------
	void OnLoadedMap()	//bola nahrana nova mapa. nevola sa pri reloadingu mapy ked sa prepiname do playmodu
	{
		DisableEnemies(false)
		WritePrefabsValuesToAllEntities()
		TestPrefabsForUniqueInstances()
		ParseMaterials()
		OnUpdateSelection("", "", "", false)
	}

//-----------------------------------------------------------------
	void EntityEditor()
	{
		g_NumEditors++
		LoadEntKeysFromEntFile()

		if(!SelectingContact)
			SelectingContact = new TraceContact

//toto sa vola neskor z GUI
//		WritePrefabsValuesToAllEntities()		//prepiseme vsetky hodnoty prefabovanych entit hodnotami z prefabov
//		TestPrefabsForUniqueInstances()			//kontrola chyb prefabov
	}

	void ~EntityEditor()
	{
		g_NumEditors--

		if(SelectingContact)
		{
			delete SelectingContact
			SelectingContact = NULL	
		}

/*		if(g_NumEditors == 0)
		{
		}*/
	}
}














