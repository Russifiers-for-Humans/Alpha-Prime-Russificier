//=================================================================================
// TODO: Move it elsewhere!
//=================================================================================
const int ST_RELOADING = 1 //prebijeni zbrane. Vykriky kryjte me apod.
const int ST_CHANGINGWP = 2 //zmena strelecke pozice, protoze stavajici stoji za hovno.
const int ST_TAKECOVER = 3 //dostal kour a klidi se na krytou pozici.
const int ST_ALERT = 4 //byl v klidu a spatril nepritele.
const int ST_ALERTBYATTACK = 5 //byl v klidu a uvidel jak do jeho kamose strili nepritel
const int ST_ALERTBYSOUND = 6 //byl v klidu a zaslechl podezrely zvuk, takze se jde podivat, co se deje. Obvykle
  //vzapeti nasleduje faze 4. protoze treba zaslechne kroky a otoci se, cimz spatri nepritele.
const int ST_ALERTBYDEAD = 7 //byl v klidu a nasel mrtvolu. Jde se podivat o co jde.
const int ST_ALERTBYSHOT = 8 // byl v klidu a dostal zasah.
const int ST_ALERTBYFRIENDISSHOT = 9 // byl v klidu a videl kamose, jak dostal zasah.
const int ST_SQUADATTACKED = 10 // skupina byla prepadena
const int ST_TEAMMATEKILLED = 11 // nekdo ze skupiny byl zabit
const int ST_SQUADLASTMAN = 12 // ve skupine zbyva posledni clovek
const int ST_GRENADE = 13 //kurva, granat!
const int ST_SIZEOF = 14

const int SPEAKERS_COUNT = 4

class SpeechSet extends _serializable
{
	string Name
	
	hsound Samples[ST_SIZEOF][16]
	string SampleNames[ST_SIZEOF][16]
	int Counts[ST_SIZEOF]
	int Position[ST_SIZEOF]
	int LoadCount

//-----------------------------------------------------------------
	hsound GetSound(int type)
	{
		int pos = Position[type]
		hsound hs = Samples[type][pos]

		pos++
		if(pos > Counts[type])
			pos = 0

		Position[type] = pos

		return hs
	}

//-----------------------------------------------------------------
	bool HasSounds(int type)
	{
		if(Counts[type])
			return true
		
		return false
	}
	
//-----------------------------------------------------------------
	void Load()
	{
		LoadCount++
		if(LoadCount > 1)
			return

		for(int t = 0; t < ST_SIZEOF; t++)
		{
			for(int n = 0; n < Counts[t]; n++)
			{
				Samples[t][n] = GetSoundEx(SampleNames[t][n], 1, SF_3D)
			}
		}
	}

//-----------------------------------------------------------------
	void Unload()
	{
		LoadCount--
		if(LoadCount > 0)
			return

		for(int t = 0; t < ST_SIZEOF; t++)
		{
			for(int n = 0; n < Counts[t]; n++)
			{
				Samples[t][n] = HNULL
			}
		}
	}

//-----------------------------------------------------------------
	void SpeechSet(string name)
	{
		Name = name
	}
}

class SoundSet
{
	string Name
	int Count
}

//Humus, ale setri pamet. Pouzijeme jen tolik, kolik potrebujem
//TODO: pouvazovat nad dynamickym polem :-/
class SoundSet16 extends SoundSet
{
	string	Sounds[16]
	int		Numbers[16]
}

class SoundSet32 extends SoundSet
{
	string	Sounds[32]
	int		Numbers[32]
}

class SoundSet64 extends SoundSet
{
	string	Sounds[64]
	int		Numbers[64]
}

SoundSet64 SoundSets[96]
int NumSoundSets

//---------------------------------------------------------------------------------
bool LoadSoundSet(string name, hsound sounds[], bool global = false)
{
	for(int n = 0; n < NumSoundSets; n++)
	{
		int sf = 0
		
		if(global)
			sf = SF_GLOBAL

		SoundSet64 ss = SoundSets[n]
		if(ss.Name == name)
		{
			for(int s = 0; s < ss.Count; s++)
			{
				sounds[ss.Numbers[s]] = GetSoundEx(ss.Sounds[s], 0, sf)
			}
			return true
		}
	}
	
	DPrint("Can't found SoundSet '" + name + "'")
	return false
}

//---------------------------------------------------------------------------------
SoundSet64 AddSoundSet(string name, int count, string names[], int numbers[])
{
	SoundSet64 ss
	
	if(count > 32)
		ss = new SoundSet64
	else
	{
		if(count > 16)
			ss = new SoundSet32
		else
			ss = new SoundSet16
	}
	
	ss.Name = name
	ss.Count = count
	
//	Print(String("AddSoundSet " + name))
	
	for(int n = 0; n < count; n++)
	{
	//	Print(String("Sound " + names[n] + " " + itoa(numbers[n])))
		ss.Sounds[n] = names[n]
		ss.Numbers[n] = numbers[n]
	}
	
	SoundSets[NumSoundSets++] = ss
	
	return ss
}

//---------------------------------------------------------------------------------
void CleanupSoundSets()
{
	for(int n = 0; n < NumSoundSets; n++)
	{
		delete SoundSets[n]
		SoundSets[n] = NULL
	}
	NumSoundSets = 0
}

//---------------------------------------------------------------------------------
bool LoadSoundSets()
{
	CleanupSoundSets()

	int f = OpenFile("scripts/definitions/soundsets.h", FILEMODE_READ)

	if(f == NULL)
	{
		DPrint("Can't load 'scripts/definitions/soundsets.h'")
		return false
	}

	string line
	string name

	string sounds[128]
	int numbers[128]
	int count
	int linenum = 0
	while(FGets(f, line) >= 0)
	{
		linenum++
		string token
		
		int type = ParseStringEx(line, token)
		
		if(type == 1)
		{
			if(token == "{")
			{
				//assert(name != "")
				continue
			}

			if(token == "}")
			{
				//assert(name != "")
				AddSoundSet(name, count, sounds, numbers)
				count = 0
				name = ""
				continue
			}
			DPrint("SoundSet: Syntax error on line " + itoa(linenum))
			return false
		}
		
		if(type != 3)
		{
			continue
		}

		if(token == "soundset")
		{
			if(ParseStringEx(line, name) != 2)
			{
				DPrint("SoundSet: Syntax error on line " + itoa(linenum))
				return false
			}
			continue
		}

		if(token == "sound")
		{
			if(ParseStringEx(line, token) != 4)
			{
				DPrint("SoundSet: Syntax error on line " + itoa(linenum))
				return false
			}
			numbers[count] = atoi(token)

			if(ParseStringEx(line, token) != 1)
			{
				DPrint("SoundSet: Syntax error on line " + itoa(linenum))
				return false
			}

			if(ParseStringEx(line, token) != 2)
			{
				DPrint("SoundSet: Syntax error on line " + itoa(linenum))
				return false
			}
			sounds[count] = token
			count++
			continue
		}
	}	
	CloseFile(f)
	return true
}

//---------------------------------------------------------------------------------
string RemoveQuotes(string str)
{
	if(strlen(str) >= 2 && substr(str, 0, 1) == tostr(34))
		return substr(str, 1, strlen(str) - 2)
		
	return str
}

string SoundMaterials[256]
int NumSoundMaterials
string SndMatParms[256][4]
hsound MaterialSounds[256][3]
int MatRemap[256]					
int LogicalMaterials[256]
int PhysMatRemap[256]
int MatHardness[32]

//---------------------------------------------------------------------------------
int GetLogicalMaterial(int mat)	//Surf2Mat vracia index na predefinovane materialy zo sounds/materials.h .Toto ho prevedie na jeden z 10 defaultnych materialov
{
	return LogicalMaterials[MatRemap[mat]]
}

//---------------------------------------------------------------------------------
hsound GetImpactSound(int Mat1, int Mat2)
{
	return MaterialSounds[MatRemap[Mat1]][MatHardness[MatRemap[Mat2]]]
}

//---------------------------------------------------------------------------------
void LoadSoundMaterials()
{
	int n
	string tokens[32]
	int	 NumTokens
	string uv = tostr(34)
	string DefinitionFile = "sound/materials.h"
	tparse tp = BeginParse(DefinitionFile)
	NumSoundMaterials = 0
	int CloneIndex = 32		//od 32 zacinaju custom (sound) materialy
	int RetypeIndex = MATERIAL_LAST		//od MATERIAL_LAST do 31 budu custom fyzikalne materialy (material/player a pod)
	string PhysMat, SndMat


	if(!tp)
	{
		string msg = "No found file " + DefinitionFile
		Print(msg)
		return
	}

	for(n = 0; n < sizeof(MatHardness); n++)
	{
		MatHardness[n] = 2
	}

	MatHardness[MATERIAL_METAL] = 0
	MatHardness[MATERIAL_IRON] = 0
	MatHardness[MATERIAL_GLASS] = 0
	MatHardness[MATERIAL_BETON] = 0
	MatHardness[MATERIAL_STONE] = 0
	MatHardness[MATERIAL_GRAVEL] = 0
	
	MatHardness[MATERIAL_PLASTIC] = 1
	MatHardness[MATERIAL_RUBBER] = 1
	MatHardness[MATERIAL_FLESH] = 1
	
	for(n = 0; n < sizeof(MatRemap); n++)
	{
		MatRemap[n] = 0
	}

	n = 0
	NumTokens = ParseLine(tp, n++, tokens)

	while(NumTokens)
	{
		switch(tokens[0])
		{
		case "material":
			SoundMaterials[NumSoundMaterials] = RemoveQuotes(tokens[1])
			SndMatParms[NumSoundMaterials][0] = ""
	  		break

		case "{":
			break

		case "physicsmaterial":
			SndMatParms[NumSoundMaterials][0] = RemoveQuotes(tokens[2])
			break

		case "sound_hard":
			SndMatParms[NumSoundMaterials][1] = RemoveQuotes(tokens[2])
			break

		case "sound_solid":
			SndMatParms[NumSoundMaterials][2] = RemoveQuotes(tokens[2])
			break

		case "sound_soft":
			SndMatParms[NumSoundMaterials][3] = RemoveQuotes(tokens[2])
			break

		case "}":
			PhysMat = SndMatParms[NumSoundMaterials][0]
			SndMat = SoundMaterials[NumSoundMaterials]

			if(PhysMat == "")		//nema definovany fyzikalny material takze to je defaultny material
			{
				PhysMat = SndMat
				SndMatParms[NumSoundMaterials][0] = PhysMat
			}

			int Type = 0

			if(SndMat != PhysMat)	//vytvarame kopiu defaultneho materialu
			{
				if(dMaterialClone("material/" + SndMat, "material/" + PhysMat, CloneIndex))	//spravime jeho kopiu podla nazvu
				{
					int PhysInd = dMaterialGetType("material/" + PhysMat)
					
					Type = PhysMatRemap[PhysInd]
					MatRemap[CloneIndex++] = NumSoundMaterials
				}
			}
			else	//pokial sa nazov zhoduje s existujucim materialom, ide o definiciu pre defaultny material takze iba nastavime index
			{
				Type = dMaterialGetType("material/" + PhysMat)
				PhysMatRemap[NumSoundMaterials] = Type

				if(Type)
				{
					if(MatRemap[Type])	//ak niektory custom fyzikalny material pouziva defaultny material, pretypujeme ho
					{
						MatRemap[RetypeIndex] = NumSoundMaterials
						dMaterialSetType("material/" + PhysMat, RetypeIndex)
						RetypeIndex++
					}
					else
						MatRemap[Type] = NumSoundMaterials	//defaultny material
				}
			}

			LogicalMaterials[NumSoundMaterials] = Type
			NumSoundMaterials++
			break
		}

		NumTokens = ParseLine(tp, n++, tokens)
	}

	EndParse(tp)
end:;

   for(n = 0; n < NumSoundMaterials; n++)	//nahrajeme zvuky
   {
   	for(int s = 1; s <= 3; s++)
   	{
   		if(SndMatParms[n][s])
   			MaterialSounds[n][s - 1] = GetSoundEx(SndMatParms[n][s], 1, SF_3D)
   	}
   }
/*   Print(NumSoundMaterials)
   for(n = 0; n < NumSoundMaterials; n++)
   {
	   int Lg = GetLogicalMaterial(n)
	   string sss = SoundMaterials[n] + " " + itoa(Lg);
	   Print(sss)
	}*/
}

