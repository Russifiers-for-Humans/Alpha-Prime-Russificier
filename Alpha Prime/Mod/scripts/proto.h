/*===================================================================*/
/*							Enforce script base API									*/
/*===================================================================*/

typedef int ctype
typedef int bool

//vraci primo odkaz na interni typ promenne
proto native ctype Type(string var)

//vytvori neprimo instanci daneho typu (dynamicka varianta k 'new')
proto volatile class Spawn(ctype type)

//vraci true, pokud ma class mezi predky(nebo on sam) typ 'type'
proto native bool IsInherited(class ent, ctype type);

//vraci jmeno classu
proto native string ClassName(class ent);
//umoznuje zavolat funkci jejim jmenem.
//pokud ent == NULL, vola se globalni fce, v opacnem pripade fce z classu
//vraci true, pokud fci nalezl a zavolal
//fce se vola jako thread, takze je v ni mozno pouzivat sleep/wait
proto volatile int Call(class ent, string function, void parm)

//umoznuje nacist obsah promenne podle jejiho jmena. result musi byt stejneho typu!
//ent muze byt == NULL, pokud chceme globalni promennou
//vraci true, pokud byla promenna nalezena a prectena. V opacnem pripade false
proto int GetValue(class ent, string varname,int index,void result);

//umoznuje zmenit obsah promenne podle jejiho jmena. input musi byt stejneho typu!
proto int SetValue(class ent, string varname,int index,void input);

//nastavi hodnotu promenne ze stringu. Pokud se povedlo, vraci true
proto int SetVar(void var, string value)

// strings
//prevede ascii kod na string
proto string tostr(int ascii)
//prevede integer na string
proto string itoa(int number);
//prevede string na integer
proto native int atoi(string str);
//prevede integer na string
proto string ftoa(float number);
//prevede string na float
proto native float atof(string str);
//vyjme ze stringu s len znaku pocinaje znakem from
proto string substr(string s, int from, int len);

//zmeni vsechny znaky na male. Vraci delku
proto int strlower(string s)
//vraci delku retezce
proto native int strlen(string s);

//hleda sample v s1. Vraci -1 kdyz nenajde
proto native int strstr(string s1, string sample);

//hleda sample v s1 + offset. Vraci -1 kdyz nenajde
proto native int strstroff(string s1, int offset, string sample);

//vraci string s orezanyma uvodnima a vedoucima mezerama
proto string trim(string str)

//oreze uvodni a vedouci mezery ve stringu str, vraci delku
proto int triminplace(string str)

//vygeneruje nahodne cislo v rozsahu min - max   
proto native int rand(int min, int max);
proto native float frand(float min, float max);

//znormalizuje uhel (0...360)
proto native float FixAngle(float ang)
proto native float	fabs(float a)
proto native int		abs(int a)
proto native float	sqrt(float val);
proto native float	sin(float deg);
proto native float	cos(float deg);
proto native float	asin(float s);
proto native float	acos(float c);
proto native float	tan(float deg);
proto native float	atan2(float y, float x);
proto native float	pow(float v, float power)

//vraci celociselnou cast (bez zaokrouhlovani)
proto native int floor(float f)

proto void Sort(void array[], int num)

//parser
proto int ParseStringEx(string input, string token)
/*
 Parses one token fron input string. Result is put into token string,
 and type of token is returned. Input string is left-truncated by the
 resulting token length.
 
 types:
 0 - error, no token
 1 - defined token (special characters etc.)
 2 - quoted string. Quotes are removed
 3 - alphabetic string
 4 - number
 5 - end of line
*/

proto int ParseString(string input, string tokens[])

proto native int KillThread(class owner, string name)
proto volatile void Idle()
proto string ThreadFunction(class owner, string name, int backtrace, int linenumber) //debug

//vector
string Vector2String(vector v)
{
	string s

	s = "<" + ftoa(v[0]) + ","
	s = s + ftoa(v[1]) + ","
	s = s + ftoa(v[2]) + ">"

	return s
}

//jsou i elegantnejsi zpusoby, ale tenhle je nejrychlejsi ;)
string itoal(int num, int len)
{
	const string ZeroPad[8] = {"", "0", "00", "000", "0000", "00000", "000000", "0000000"}
	
	string str = itoa(num)

	int l = len - strlen(str)
	
	if(l > 0)
		return ZeroPad[l] + str

	return str
}

//-----------------------------------------------------------------
float DiffAngle(float yaw, float a)
{
	yaw = FixAngle(yaw)
	a = FixAngle(a)

	float diff = yaw - a
	
	if(fabs(diff) > 180)
	{
		if(yaw < a)
			diff = yaw + 360 - a
		else
			diff = yaw - 360 - a
	}

	return diff
}

/*===================================================================*/
/* 3D math extension																	*/
/*===================================================================*/
proto float VectorNormalize(vector vec);

proto native float VectorLength(vector vec);
proto native float VectorLengthSq(vector vec);

proto vector   Vector2Angles(vector vec);
proto float    Vector2Yaw(vector vec);
proto native vector Yaw2Vector(float yaw)
proto vector   VVector2Angles(vector vec);
proto vector   Angles2Vector(vector ang);
proto void		RollPitchYawMatrix(vector ang, vector mat[3]);
proto void		RollPitchYawMatrix2(vector ang, vector mat[3]);
proto void		AroundVectorMatrix(vector vec, float degree, vector mat[3]);
proto vector	VectorMatrixMultiply4(vector mat[4], vector vec);
proto vector	VectorMatrixMultiply3(vector mat[3], vector vec);
proto vector	VectorMatrixInvMultiply4(vector mat[4], vector vec);
proto vector	VectorMatrixInvMultiply3(vector mat[3], vector vec);
proto void		MatrixMultiply4(vector mat0[4], vector mat1[4],vector res[4]);
proto void		MatrixMultiply3(vector mat0[3], vector mat1[3],vector res[3]);

proto native vector RotationVectorTo(vector mat0[3], vector mat1[3]);

void ScaleMatrix(float scale, vector mat[3])
{
	vector v0, v1, v2
	v0[0] = scale
	v1[1] = scale
	v2[2] = scale
	mat[0] = v0
	mat[1] = v1
	mat[2] = v2
}
	
proto vector MatrixAngles(vector mat[3])

//vytvori jednotkovy quaternion
void QuatIdentity(float q[4])
{
	q[0] = 0
	q[1] = 0
	q[2] = 0
	q[3] = 1
}

//zkopiruje quaternion
void QuatCopy(float s[4], float d[4])
{
	d[0] = s[0]
	d[1] = s[1]
	d[2] = s[2]
	d[3] = s[3]
}

proto float QuatToAxisAngle(float q[4], vector axis)

//prevede matici 3x3 na quaternion
proto void MatrixQuat(vector mat[3], float q[4])

//spocita prechod z rotace q1 do rotace q2 podle vahy frac (0...1)
proto void QuatLerp(float out[4], float q1[4], float q2[4], float frac)

//prevede quaternion na matici 3x3
proto void QuatMatrix(float q[4], vector mat[3])

//vynasobi dva quaterniony. Pozor! neni komutativni!
proto void QuatMultiply(float out[4], float q1[4], float q2[4])

//vytvori otoceny quaternion okolo osy axis o uhel angle
proto void QuatRotationAxis(float out[4], vector axis, float angle)

//vytvori otoceny quaternion o uhly pitch,yaw,roll
proto void QuatRotationRollPitchYaw(float out[4], vector angles)

proto vector CatmullRom(vector v1, vector v2, vector v3, vector v4, float weight);
   
//vraci true, kdyz boundboxy koliduji
int CheckBoundBox(vector mins1, vector maxs1,vector mins2, vector maxs2)
{
	if(mins1[0] > maxs2[0] || mins1[1] > maxs2[1] || mins1[2] > maxs2[2] || maxs1[0] < mins2[0] || maxs1[1] < mins2[1] || maxs1[2] < mins2[2])
		return false

	return true
}

const vector UpVec = "0 0 1"
const vector ZeroVec = "0 0 0"

const float M_PI = 3.14159265358979
const float M_PI2 = 6.28318530717958

const float RAD2DEG = 57.2957795130823208768
const float DEG2RAD = 0.01745329251994329577

vector Perpend(vector vec)
{
	return vec * UpVec
}

float Yaw(vector v)
{
	return v[YAW]
}

float Roll(vector v)
{
	return v[ROLL]
}

float Pitch(vector v)
{
	return v[PITCH]
}

//rychly kontruktor vectoru
proto native vector Vector(float x, float y, float z)

/*===================================================================*/
/*							Enforce engine API										*/
/*===================================================================*/

//------------------------------------------
// ENTITY API
//------------------------------------------

class Contact;
class _eclass extends _entity
{
	float TFactor[4]
	int	Color

	void EOnTouch(class other, int extra)
	{
	}
	void EOnBlock(class other, int extra)
	{
	}
	void EOnGround(class other, int extra)
	{
	}
	void EOnInit(class other, int extra)
	{
	}
	void EOnExtra(class other, int extra)
	{
	}

	void EOnNotVisible(class other, int extra)
	{
	}
	void EOnFrame(class other, int extra)
	{
	}
	int  EOnVisible(class other, int extra)
	{
	}
	void EOnPostFrame(class other, int extra)
	{
	}

	void EOnWorldProcess(class other, int extra)
	{
	}
	void EOnFlashlight(class other, int extra)
	{
	}
	void EOnLoad(class other, int extra)
	{
	}
	void EOnSave(class other, int extra)	
	{
	}
	void EOnTimer(class other, int extra)
	{
	}
	void EOnAnimEnd(class other, int extra)
	{
	}
	void EOnDummy00(class other, int extra)
	{
	}
	void EOnAnimBlend(class other, int extra)
	{
	}
	void EOnDummy01(class other, int extra)
	{
	}
	void EOnDummy02(class other, int extra)
	{
	}
	void EOnSoundEnd(class other, int extra)
	{
	}
	void EOnUser0(class other, int extra)
	{
	}
	void EOnUser1(class other, int extra)
	{
	}
	void EOnUser2(class other, int extra)
	{
	}
	void EOnUser3(class other, int extra)
	{
	}
	void EOnLoaded(class other, int extra) //24
	{
	}
	void EOnLipsync(class other, int extra)
	{
	}
	void EOnTick(class other, int extra)
	{
	}
	void EOnSimulate(class other, float dt)
	{
	}
	void EOnDummy03(class other, int extra)
	{
	}
	void EOnJointBreak(class other, int extra)
	{
	}
	void EOnPhysicsMove(class other, int extra)
	{
	}
	void EOnContact(class other, Contact extra)
	{
	}

	proto native vector VectorToParent(vector vec)
	proto native vector CoordToParent(vector coord)
	proto native vector VectorToLocal(vector vec)
	proto native vector CoordToLocal(vector coord)
	
//vraci pointer na nadrazenou entitu v hierarchii
	proto native class GetParent()
//vraci pointer na prvni child entitu v hierarchii. Dalsi child entity nasledujici jako sibling prvniho childu
	proto native class GetChildren()
//vraci pointer na sousedni child entitu v hierarchii
	proto native class GetSibling()

	proto native void SetObject(vobject object, string options = "")
	proto native vobject GetVObject()


//Options jsou konkretni pro jednotlive typy objektu. Zatim jediny to vyuziva XOB pro remapovani
//materialu.
//Syntaxe je nasledovna
//$remap 'puvodni material' 'novy material';

	proto native bool IsRigidBody()
}

proto native int		UpdateEntity(class ent)
//vraci nastavenou masku, co vse bylo updatovano.
//	TFL_UPDATE - byla updatovana hierarchie
//	TFL_UPDATE_ORG - byla updatovana pozice
//	TFL_UPDATE_MDL - byl updatovan objekt (muze to byt kosteny XOB, na jehoz kostech jsou pivoty a na nich childy)
//	TFL_UPDATE_ANG - byla updatovana orientace

proto native int		ClearFlags(class ent, int flag);
proto native vector	GetOrigin(class ent);
proto native vector	GetAngles(class ent);
proto native int		GetFlags(class ent);
proto native int		GetGroundFlags(class ent);
proto native class	GetNextEntity(class ent, ctype filter);

proto native void     SetWeight(class ent, float weight);
proto native void     SetEventMask(class ent, int event);
proto native int      ClearEventMask(class ent, int event);
proto native int      GetEventMask(class ent);
proto native void     SetVelocity(class ent, vector vel);
proto native vector   GetVelocity(class ent);

proto native vector   GetAnimLinearMovement(class ent);
proto native vector   GetAnimAngularMovement(class ent);

proto native vector   GetMatrix(class ent, int axis);
// precte jeden radek matice.
// index row muze byt v rozsahu 0...3

proto void GetMatrix4(class ent, vector mat[])
// precte aktualni matici entity. Precte jen tolik vektoru, jak velke je pole.
// Je tedy mozne precist jen 3x3 rotacni matici, pokud je vector mat[3]

//Nastavi flagy (TFL_) entity. Operace je typu OR, tzn. ze flagy neprepisuje, 
//ale zapina ty, ktere zapnute nebyly. Vraci puvodni nastaveni
proto native int SetFlags(class ent, int flag);

//!OBSOLETE!
proto native void SetRenderMode(class ent, int mask);

//nastavi masku, ve kterych renderview bude entita videt.
proto native int SetRenderViewMask(class ent, int mask);

proto native void    SetAngles(class ent, vector angles);
proto native void    SetOrigin(class ent, vector orig);
proto native void    SetMatrix(class ent, vector mat[4]);
	

proto native void    SetKind(class ent, int kind);
proto native void    SetMoveType(class ent, int movetype);
/* OBSOLETE PHYSICS
WALK
FLY
NOCLIP
BOUNCE
MISSILE
PUSH
*/

//prida child entitu do hierarchie. Pokud nechceme pouzit definovany pivot, dava se -1
proto native int     AddChild(class parent, class child, int pivot);
proto native void		RemoveChild(class ent, class child);
proto native void		SetSortBias(class ent, int bias)

//zjisti rozmery entity. mins/maxs jsou cilove promenne, i kdyz to tak nevypada :-)
proto void     GetBoundBox(class ent, vector mins, vector maxs);

//zjisti skutecny globalni boundbox entity.
proto void     GetCBoundBox(class ent, vector mins, vector maxs);

//zmeni velikost entity. Funguje jen pokud je selectnuty nejaky objekt.
//pri selectnuti jineho objektu se tyto rozmery prepisou a je treba je znovu nastavit.
proto void     SetBoundBox(class ent, vector mins, vector maxs);

//nastavi widget typu WTYPE_RTTEXTURE, na ktery se lze odkazat ze shaderu jako $gui0...$gui3
//lze pouzit az po selectnuti objektu. Pri selectnuti jineho objektu je treba GUI widget opet nastavit.
proto native void SetGUIWidget(class ent, int index, widget w);

//nastavi texturu, na kterou se lze odkazat ze shaderu jako $renderview + value
//a propoji ji s kamerou cislo cam_index
//lze pouzit az po selectnuti objektu. Pri selectnuti jineho objektu je treba texturu
//size je velikost textury. Pokud 0, pouzije se 512. Bude taktez
//podlehat nastaveni urovne detailu v settings!
proto native void SetRenderView(class ent, int cam_index, int size);

//umoznuje vyvolat exception ze skriptu. Parametry jako u EOnXXXX()
proto volatile void Throw(class ent, class actor, int event, int extra);

/*
 Entity flags

TFL_VISIBLE			-	bude videt
TFL_SOLID			- je nepruchozi
TFL_TRIGGER			- je pruchozi, ale vyvolava EV_TOUCH
TFL_DEF				- 	TFL_VISIBLE|TFL_SOLID
TFL_ONGROUND		- jen ke cteni, je na zemi
TFL_COLLIDEONLYWORLD - koliduje jen se svetem. Entity ignoruje (jen pro legacy fyziku)

TFL_TOUCHTRIGGERS - aktivuje triggery

TFL_AREASPLITTER	- pokud je nastaveno, zjistuje se po nahrani mapy, ci jde o oddelovac viditelnosti (dvere apod)
TFL_AREACLOSED		- pokud je nastaveno a entita je areasplitterem, rozdeli se visibilita

TFL_NOBILLBOARD	- jen pro sprity - nebude se natacet ke kamere
TFL_NOMPIVOT		- jen pro Mesh - je mozno vypnout prenaseni pohybu z animace do fyziky.

TFL_PARTICLE		- mene precizni a velmi rychly link do BSP stromu pro efekty
TFL_FULLMATRIX		- sama si nastavuje 4x3 matici. Origin/Angles se ignoruje, nesmi mit fyziku a NESMI byt child jine entity!
TFL_ROTMATRIX		- sama si nastavuje 3x3 matici. Angles se ignoruje, muze mit fyziku ale nesmi byt child jine entity

TFL_USER1			- k volnemu pouziti. Hodi se napriklad pro filtrovani entit pri VisEntities()
TFL_USER2
TFL_USER6

TFL_NOLIGHT			- nepocita se svetlo ze sveta
TFL_NOSHADOW		- nepocita se stin (stencilovy)

TFL_DYNAMICBBOX - ma smysl jen v kombinaci s TRACE_BONES. Slouzi k presne detekci kolize pri strileni.
TFL_OBBCOLLISION - nekoliduje AABB boxem, ale orientovanym boxem. Pozor na zmenu uhlu - muze zpusobit zaseknuti entit do sebe!

TFL_TRANSLUCENT - bude pruchozi pri testovani kolize s TRACE_PASSTRANSLUCENT

TFL_PATHBLOCK
*/

/*
 Entity events
 
 // funkce Exception
//   int Exception(int event,MinClass other, int extra)
//event:
//EV_TOUCH	- dotkla se entita - other

//EV_BLOCK	- PUSH pohyb byl blokovan (dvere)

//EV_ONGROUND	- dopad na entitu (shora - other na ni stoji)
// extra		= surfparms plochy, na ktere stoji

//EV_VISIBLE	- je vybran pro rendering
//	other		= WorldClass
// extra		= cislo snimku
//	result	= false ma byt renderovan, true nema byt renderovan

//EV_NOTVISIBLE	- nebyl vybran pro rendering
//	other		= WorldClass
// extra		= cislo snimku

//EV_POSTTHINK - - ||- pote co se s ni pohne
//	other		= WorldClass

//EV_BEGINFRAME	- jen u entity 'world'
//	other		= WorldClass
// extra		= unikatni cislo snimku

//EV_ENDFRAME		- jen u entity 'world'
//	other		= WorldClass
// extra		= unikatni cislo snimku

//EV_INIT      - zavolana po nahrani celeho levelu
//	other		= WorldClass

//EV_ONLOAD	- Nahrala se entita ze savemap
// extra		= pointer na file

//EV_ONSAVE	- Uklada se entita do savemap
// extra		= pointer na file

//EV_TIMER  - volano pro SetTimer kdyz prijde cas...

//Jen u MeshObject:
//EV_ANIMEND	- Konec AF_ONCE animace, extra = cislo slotu 0...11
//EV_ANIMBLEND - Animace preblendovala z predchozi. extra = cislo slotu 0...11

//EV_LOADED     - zavolana po nahrani celeho levelu a zavolani EV_INIT na vsech entitach. Jde vpodstate o "druhe kolo"
//	other		= WorldClass

//EV_TICK - fixni frekvence 15fps

//EV_FRAME     - kazdy frame, jeste pred fyzikou
//	other		= WorldClass
// extra		= unikatni cislo snimku

//EV_POSTFRAME - po aplikaci fyziky a update

//EV_SOUNDEND	- skoncilo prehravani SFL_ONCE zvuku
// extra		= pointer na sound

//EV_LIPSYNC

//EV_USER

//EV_JOINTBREAK
//EV_PHYSICSMOVE
//EV_CONTACT

//EV_ALL       - vsechny najednou

*/

//------------------------------------------
// TRIGGERS API
//------------------------------------------
/*
Trigger musi byt brush entita, neni SOLID, tudiz se skrz nej da prochazet.
(pokud to z nejakeho duvodu nezmenim zavolanim fce SetFlags)
Pokud se do nej dostane entita, je vyvolana udalost EV_TOUCH. Pokud chci
sledovat zda tato entita trigger opustila, zavola se funkce
AddTriggerInsider(this, other), ktera ulozi entitu do interniho seznamu.
Fce vrati false, pokud uz tato entita v seznamu byla. Fce TriggerInsiders
vraci pocet entit uvnitr triggeru. Pokud entita, ktera byla v seznamu
trigger opusti, je vyvolana udalost EV_EXTRA a pointer other ukazuje
na trigger
*/ 

proto native void CreateTrigger(class ent);
proto native void DestroyTrigger(class ent);
proto native int TriggerInsiders(class ent, class insiders[]);
proto native int AddTriggerInsider(class ent,class actor);

//------------------------------------------
// TIMER API
//------------------------------------------

//umoznuje vyvolat exception 'event' u entity target po uplynuti casu.
//if(once == true) timer se sam vypne. V opacnem pripade jede dokud neni zastaven StopTimer
proto native int SetTimer(class ent, class target, float tim, int event, int extra, int once);
//
proto native int SetTimerFunc(func funct, float tim, int once = true)
//to same, co SetTimer, ale nevyvolava exception, ale funkci. Pokud je pouzit pointer na class metodu, je
//entita majitelem tohoto timeru a po jejim smazani se smaze i timer. Pokud je to pointer na globalni fci,
//je majitelem timeru entita 'world'

proto native void StopTimer(int timer)
//zrusi timer vytvoreny fci SetTimer/SetTimerFunc

//------------------------------------------
// LIGHT API
//------------------------------------------

typedef int HLIGHT //handle

// Light functions
proto HLIGHT AddLight(class owner,int style,int flags,int intens,int radius,vector color);
//flags
//LFL_POINT				- bodovy zdroj svetla, siri se vsemi smery
//LFL_SPOT				- orientovany zdroj svetla, smer se ridi uhly entity 'owner'
//LFL_DYNAMIC			- dynamicky zdroj svetla. Engine je omezen na 512 dynamickych svetel najednou!
//								tyto svetla vytvaret jen pro konkretni okamzik a pote okamzite RemoveLight()!
//LFL_CHEAP				- hint pro lacinejsi render efektu vybuchu, vystrelu apod
//LFL_TRACE				- pro dynamicke svetlo - trasuji se stiny podle prostredi. Pomale, jen pro efekt!
//LFL_SHADOWSOURCE	- pro dynamicke svetlo - prednostne se pouziva jako zdroj pro stencil stiny
//LFL_LOOKUP

proto native bool RemoveLight(HLIGHT light);
proto native bool SetLightEx(HLIGHT light,int intens, int radius, vector color);
proto native bool SetLightTexture(HLIGHT light, string cubemap);

//nastavi uhel konusu pro LFL_SPOT. Udava se v degrees. Defaultne je nastaveno 90'
proto native bool SetLightCone(HLIGHT light, float cone);

//------------------------------------------
// SOUND API
//------------------------------------------

//Nahraje zvuk (*.wav, *.ogg)
proto native hsound GetSoundEx(string name, int cache, int flags);
//flags pro GetSound
//SF_3D			plne 3D zvuk
//SF_FREQCONTROL	umoznuje menit frekvenci
// toto je obzvlaste vhodne pro dialogy a samply v cutscenach obecne:
//SF_STREAM		pro velmi dlouhe,melo prehravane zvuky. Zvuk se postupne dotahuje z disku. Idealni pro OGG-Vorbis streamy
// cache - pocet cachovanych sound bufferu. Specialne pro hodne multiplexne prehravane zvuky. Podstatne snizuje zatizeni CPU
//         na zvukovem subsystemu. Pocet muze byt 0...nekonecno. Interne je limitovan podle potreby...

//vyhodi zvuk z pameti
proto native void ReleaseSound(hsound snd)

proto volatile int PlaySound(class source, hsound sound, int flags);
//flags pro PlaySound
//SF_ONCE		- Zvuk se prehraje jen jednou a vyvola EV_SOUNDEND
//SF_AMBIENT	- Zvuk neni 3D a nemeni se jeho intenzita podle vzdalenosti (nemel by se kombinovat s SF_3D!!!)
//standardni dosah zvuku - 2048 units
//SF_HALFRANGE			Omezeny dosah (1024 units)
//SF_QUARTERRANGE		Omezeny dosah (512 units)
//SF_MUSIC				Hudba. Plati na nej MusicVolume misto EffectsVolume

//volume 0...1. Logaritmic scale
proto native int		SetSoundVolume(int sound, float volume)
proto native int		SetSoundFrequency(int sound, int freq)


proto native int		GetSoundLength(int sound)
//vraci 0, pokud je soundid nevalidni

proto native int		GetSoundPosition(int sound)
//vraci -1, pokud je soundid nevalidni

proto volatile void EndSound(int snd);

//defaultne se pouziva EAX prostredi nadefinovane v mape, ale lze ho prebit touto fci.
// Mohou se michat dve ruzna prostredi v pomeru danem hodnotou fade (fade==0 -> 100% env1, fade==1 -> 100% env2).
//pokud chceme michat aktualni prostredi s nejakym jinym, lze pouzit zastupny nazev "$current".
//Tim lze dosahnout nafadovani vlastniho prostredi, kdyz fci postupne volame napr. takto:
//SetEAXEnvironment("$current", "Drugged", fade)
//pricemz hodnota fade postupne narusta od nuly do jedne
proto native bool SetEAXEnvironment(string env1, string env2, float fade)


//------------------------------------------
// WIDGET API
//------------------------------------------

//nahraje UNICODE-XML string table. Na jeji texty se lze odkazovat z widgetu pomoci textu "#id"
proto native int	LoadStringTable(string name)

/* Widgety jsou 2D graficke elementy. Muze to byt:
WTYPE_TEXT     -  Text
WTYPE_VIEW     -  Render okno pro mapu
WTYPE_IMAGE    -  Obrazek
WTYPE_RTTEXTURE
*/

proto native widget CreateWidgetEx(widget parent, int type, int left, int top, int width, int height,int flag, int color, int sort);
proto native widget CreateWidget(int type, int left, int top, int width, int height,int flag, int color, int sort);

//tyto dva flagy se navzajem vylucuji!!!
//WF_BLEND		  - textura se blenduje s podkladem na zaklade alfy
//WF_ADDITIVE    - textura se pricita k podkladu na zaklade alfy

//WF_SOURCEALPHA - bere alfu z textury * alfu z barvy. Pokud neni nastaveno, povazuje se textura za nepruhlednou
//                    a pruhlednost urcuje jen barva

//WF_NOALPHA	   Plati jen pro WTYPE_RTTEXTURE - rendertarget se nevytvari s alfa-kanalem (50% uspora velikosti!!!)
//WF_VISIBLE		Je videt - to same co ShowWidget(w, true)
//WF_NOWRAP			Newrapuje texturu kolem dokola
//WF_ZWRITE			Zapisuje do Z-Bufferu
//WF_CENTER			Centruje WTYPE_TEXT
//WF_VCENTER		Vyskove centruje WTYPE_TEXT
//WF_RALIGN			Zarovnava zprava WTYPE_TEXT
//WF_EXACTPOS		Pouziva fyzicke rozmery obrazu (g_iWidth, h_iHeight)
//WF_EXACTSIZE		Pouziva fyzicke rozmery obrazu
//WF_NOFILTER		nefiltruje texturu (zadny rozmazavani)
//WF_STRETCH		roztahuje texturu na plnou velikost
//WF_FLIPU			prevrati texturu v ose U
//WF_FLIPV			prevrati texturu v ose V

//WF_CUSTOMUV		ignoruje STRETCH/FLIPU/FLIPV a bere vlastni UV nastavene prez SetWidgetUV()


proto native void DestroyWidget(widget w);
proto native void SetWidgetColor(widget w, int argb);
proto native int LoadWidgetImage(widget w, int num, string name);
proto native void ShowWidget(widget w, bool show);

proto native int PlayVideo(widget w, int cmd)
//VP_PLAY
//VP_STOP
//VP_REWIND
//VP_POSITION
//VP_REPEAT
//VP_ISPLAYING

proto native void SetWidgetImage(widget w, int num);
proto native void SetWidgetWorld(widget w, int world, int camera);
proto native void SetWidgetPos(widget w, int left, int top, int width, int height)
proto native void SetWidgetUV(widget w, float uv[4][2])


proto native void SetWidgetText(widget w, int line, string text);

//vlozi text, na pozice #id vlozi stringy ze string-table, a podle fillmode wrapuje text na dalsi radky
//interne pracuje v unicode
proto native int SetWidgetTextEx(widget w, int line, int fillmode, string text)

proto native void SetWidgetTextColor(widget w, int color)
proto native void SetWidgetTextSpacing(widget w, int horiz, int vert)
proto native void SetWidgetTextOffset(widget w, int left, int top)

proto native int CreateFont(string name, int nwidth, int nheight)
proto native void SetFont(widget w, int font);

//------------------------------------------
// DEBUG SHAPE API
//------------------------------------------

class Shape
{
	//don't call destructor directly. Use Destroy() instead
	proto private void ~Shape()

	proto native void SetMatrix(vector mat[4])
	proto native void SetColor(int color)
	proto native void SetFlags(int flags)
	proto native void Destroy()
}

proto native Shape AddDShape(int type, int color, int flags, vector p1, vector p2)
proto native Shape AddDLines(int color, int flags, vector p[], int num)
proto native Shape AddDTris(int color, int flags, vector p[], int num)
proto native Shape AddDSphere(int color, int flags, vector origin, float radius)
proto native Shape AddDCylinder(int color, int flags, vector origin, float radius, float length)

//SHAPE_BBOX
//SHAPE_LINE
//SHAPE_DIAMOND

//Flags:
// SS_NOZBUFFER
// SS_NOZUPDATE
// SS_DOUBLESIDE
// SS_WIREFRAME
// SS_TRANSP
// SS_ONCE
// SS_NOOUTLINE
// SS_NOCULL

//pro vizualizaci geometrie levelu
proto int Brushes2Shape(int includecont, int includeflags, int excludecont, int excludeflags, Shape array[])
//vybere ty brushe, ktere maji nastavene flagy 'includecont' a 'includeflags' a nemaji nastavene 'excludecont' a 'excludeflags'
//vyrobene DShape nasype do pole array a vrati jejich pocet.

//-----------------------------------------------------------------
Shape AddDArrow(vector from, vector to, float size, int color, int flags)
{
	vector dir = to - from
	VectorNormalize(dir)
	vector dir1 = dir * size
	size = size * 0.5

	vector dir2 = Perpend(dir) * size

	vector pts[5]
	pts[0] = from
	pts[1] = to
	pts[2] = to - dir1 - dir2
	pts[3] = to - dir1 + dir2
	pts[4] = to

	return AddDLines(color, flags, pts, 5)
}
	
//-----------------------------------------------------------------
Shape AddDBridgeArrow(vector from, vector to, float size, int color, int flags)
{
	vector dir = to - from
	VectorNormalize(dir)

	vector dir1 = Vector(0, 0, -size)
	size = size * 0.5

	vector dir2 = Perpend(dir) * size

	vector pts[7]
	pts[0] = from
	pts[1] = from + "0 0 32"
	pts[2] = to + "0 0 32"
	pts[3] = to
	pts[4] = to - dir1 - dir2
	pts[5] = to - dir1 + dir2
	pts[6] = to

	return AddDLines(color, flags, pts, 7)
}

//-----------------------------------------------------------------
void Matrix2Shape(vector mat[4])
{
	vector org = mat[3]
	
	AddDShape(SHAPE_LINE, 0xffff0000, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, org, mat[0] * 16 + org)
	AddDShape(SHAPE_LINE, 0xff00ff00, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, org, mat[1] * 16 + org)
	AddDShape(SHAPE_LINE, 0xff0000ff, SS_NOZUPDATE|SS_DOUBLESIDE|SS_TRANSP, org, mat[2] * 16 + org)
}

//------------------------------------------
// WORLD API
//------------------------------------------
proto native float GetWorldTime()

//LoadMap - nahraje mapu a vraci handle. Mapa musi byt uvolnena fci FreeMap !
//Fce PlayMap zavolana ve fci update() se proste updatuje o jeden frame a
//vykresli se, pokud je prirazena k nejakemu widgetu typu VIEW
proto volatile int LoadMap(string name, bool physics);
proto volatile int ReloadMap(int map)

proto int      SaveMap(int currentmap, string name);
//kdyz currentmap == NULL, pak nahraje mapu a do nej savegame
//v opacnem pripade vycisti mapu a nahraje do ni ulozena data
proto volatile int LoadSaveMap(int currentmap, string name);
proto volatile void PlayMap(int map);
proto volatile void FreeMap(int map);

//urcuje ze ktere kamery se bude brat pozice posluchace pro zvukovy engine
proto native void SetListenerCamera(int camera)

//zmeni pozici kamery pro tuto mapu
proto native void SetCamera(vector origin, vector angle);
//origin		-		pozice kamery
//angle		-		uhly kamery

//zmeni pozici kamery. 0 je hlavni view, 1...7 jsou user view (v shaderu se pak definuji napr. jako 'diffusemap "$renderview1"' apod.)
proto native void SetCameraEx(int cam, vector mat[4]);

proto native void SetCameraVerticalFOV(int cam, float fovy)
proto native void SetCameraFarPlane(int cam, float farplane)

//zjisti aktualni pozici/uhly kamery
proto void     GetCamera(vector origin, vector angle);
//origin		-		pozice kamery
//angle		-		uhly kamery

proto vector	ProjectVector(class ent, vector vec);
proto vector	UnprojectVector(float x, float y, vector dir)

proto native void SetPostEffect(int type, float parm0, float parm1, float parm2, float parm3)
//EFF_NONE
//EFF_UNDERWATER
//EFF_MBLUR

//name je void proto, ze jde o odkaz na promennou, nikoliv jeji obsah. Vkladane
//jmeno musi byt class member owner._name, aby hashtable mela referenci na spravny string!
proto void AddEntityToHashTable(class ent, void name)
proto native void RemoveEntityFromHashTable(class ent, string name)
proto native class FindEntityInHashTable(string name)

//------------------------------------------
// TRACE & VISIBILITY API
//------------------------------------------

//nastavi aktualni mapu. Umozni tim pracovat s entitama i mimo obdobi zpracovani mapy (PlayMap, render)
//vraci predchozi aktualni mapu
proto native int SetCurrentMap(int map)

//kolize a trasovani
class TraceContact extends _serializable
{
	float		Fraction;
	int		Content;
	int		Surfparm;

	int		ShaderFlags;
/*
SHF_CLOTH
SHF_LIQUID
SHF_MIRROR
SHF_NEEDRENDERVIEW
SHF_NEEDGUI
*/
	int		Triangle;
	int		SurfaceID;
	string	Shader;
	float		Plane[4];
	vector	Point;
}

proto native bool TraceLineToEntity(class ent, vector start, vector end, TraceContact contact);

//trasuje primku start->end, vraci 0..1 pokud kolize. do end se uklada posledni pozice
// z ent se zjistuje bbox
//jeste pribydou flagy jako u P2PVisibilityEx
proto float 	TraceLine(class ent,vector start,vector end, class cent, float plane[4], int content, int surfparm);
//INPUT:
//ent			-		Entita se kterou se pohybuje po primce
//start		-		Odkud
//end			-		Kam
//OUTPUT:
//cent		-		vraci entitu, se kterou se srazila
//plane		-		vraci plane polygonu se kterym se srazila (X,Y,Z,D)
//content	-		obsah prostoru, se kterym se srazila
//surfparm	-		parametry surfacu, se kterym se srazila
//vysledek	-		hodnota 0...1, kolik procent drahy urazila

//bool FilterCallback(class target [, vector rayorigin, vector raydirection])
proto volatile float TraceLineEx(int kind,vector start,vector end, vector mins, vector maxs, class cent, float plane[4], int content, int surfparm, int flags, func filtercallback);
//navic flagy:
//TRACE_ENTS  - testuje i entity
//TRACE_WORLD - testuje world
//TRACE_DETAIL - testuje detailni brushe world geometrie
//TRACE_PATCH - testuje bezier patche world geometrie
//TRACE_WATER - testuje kolize s vodni hladinou
//TRACE_PASSTRANSLUCENT - nebude kolidovat s entitama TFL_TRANSLUCENT
//TRACE_RAGDOLLS - testuje ragdolly
//TRACE_BONES - testuje kolizni obalky kolem kosti animovanych objektu


//pomocne globalni fce pri TraceLine(Ex)
// g_iTraceSphere	obsahuje zasazenou hitsphere X objektu
// g_iTraceBone	obsahuje zasazenou kost X objektu
//g_iTraceBrush
//g_iTraceSurfaceType


//zjistuje viditelnost. Vhodnejsi nez P2PVisibility, pokud mame i zdrojovou a cilovou entitu,
//protoze jejich visibility informace test podstatne urychli
proto native int E2EVisibility(class ent1, vector from, class ent2, vector to, int flags)

//zjistuje viditelnost
proto native int P2PVisibilityEx(vector from, vector to,int flags);
//from		-		odkud
//to			-		kam
//flags:
//TRACE_VISTEST	- provadi nejprve visibility test. Zbytecne u entit pri EV_VISIBLE -> kamera (zde uz je jistota, ze podle visibility je viditelna)
//TRACE_PATCH		- testuje proti krivkam
//TRACE_MESH		- testuje proti statickym polygonovym objektum (misc_model)
//TRACE_DETAIL		- testuje proti detailnim brushum
//TRACE_ENT			- testuje proti brush entitam
//TRACE_NOTRACE	- netestuje geometrii (ma smysl ve spojeni s TRACE_VISTEST)
//OUTPUT:
//vysledek	-		true je videt/false neni videt

//najde vsechny entity v radiusu. Pomerne dost narocne, jestli to nekdo pouzije na tisice particlu, tak ho zabiju
// update: uz je to 4-5x rychlejsi, ale stejne - pouzivat s rozumem!!
proto int		SphereQuery(vector origin, float radius, class visents[], int ents, int fmask)

//jako TraceLine pro jednu pozici, ale nevyzaduje entitu
// owner je jen pro rozliseni PLAYER-PLAYERCLIP atd...
proto int		TestPosition(class owner, vector origin, vector min, vector max, int flags)
//vysledek	-	true nekoliduje, false koliduje

proto int TestEntityPos(class ent, int flags)
//vysledek  - true nekoliduje, false koliduje


//zjisti, jestli je bbox viditelny, podle view-frustum a PVS
proto native bool IsBoxVisible(vector mins, vector maxs, int flags)
//flags & 1 - testuje i PVS

//vysledek true/false je videt/neni videt

//najde vsechny viditelne entity (jen nahrubo,podle visibility. Je vhodne po vyberu entit, ktere nas zajima vyzkouset jeste preciznejsi metody viditelnosti)
proto int		VisEntities(vector origin, vector look, float angle, float radius, class ents[2], int maxents, int fmask);
//origin		-		misto ze ktereho se "koukame"
//look		-		kterym smerem se kouka
//angle		-		zorny uhel (dava se tak 90). -1 pokud nas zorny uhel nezajima
//ents		-		pole int, libovolne velikosti
//maxents	-		velikost pole (zamezeni preteceni pole)
//fmask		-		maska flagu (SetFlags()). Hledana entita musi mit nastaveny vsechny uvedene flagy
//						s vyhodou lze pouzit rezervovane flagy TFL_USER1, TFL_USER2, TFL_USER6 pro rychle
//						nalezeni konkretni kategorie entit

//-----------------------------------------------------------------
vector ClipLine(vector start, vector end, vector norm, float d)
{
	vector vec
	float d1, d2

	d1 = start * norm
	d1 = d1 - d

	d2 = end * norm 
	d2 = d2 - d

	d = d1 - d2

	if(d1 < 0)
		d1 = d1 + 0.1
	else
		d1 = d1 - 0.1
   
	d = d1 / d
	clamp d<0, 1>
   						
	vec = end - start
	vec = vec * d
	
	return vec + start
}

proto float IntersectRaySphere(vector raybase, vector raycos, vector center, float radius)
proto float IntersectRayBox(vector start, vector end, vector mins, vector maxs)

//------------------------------------------
// VISUAL OBJECT API
//------------------------------------------

//objects
//flag 0/RF_RELEASE pokud chceme objekt odstranit i z cache
//hodi se pro velike objekty, o kterych se vi, ze uz nebudou potreba - cutsceny apod
proto native vobject  GetObject(string name);
proto native void     ReleaseObject(vobject object, int flag);
proto native void     SelectObject(class ent,vobject object);

// specialitka, aby se daly delat dynamicky triggery
proto void     CreateModel(class ent, vector mins, vector maxs);
proto void     RemoveModel(class ent);

//Dynamic MeshObject
proto vobject CreateXOB(int nsurfaces, int nverts[], int numindices[], string shaders[])
proto void UpdateVertsEx(class ent, int surf, vector verts[], float uv[])
proto void UpdateIndices(vobject obj, int surf, int indices[])

//Posune animaci (jedno, jestli X, TOB, Sprite...). X+anm ignoruje parametry speed&loop
proto native int	BumpFrame(class ent, float speed, int loop);

proto native void	SetBone(class ent, int bone, vector angles, vector trans, float scale);
proto native bool	SetBoneMatrix(class ent, int bone, vector mat[4]);
proto native void	SetBoneGlobal(class ent, int bone, vector mat[4]);
proto native bool	GetBoneMatrix(class ent, int bone, vector mat[4]);

proto native void	SetFrame(class ent, int slot, float frame);

// pokud je v jakemkoliv API pouzito BoneMask == NULL, bere se, ze jsou nastavene vsechny bity
class BoneMask
{
	int Mask[3]
}

proto native void	SetAnimSlot(class ent, int slot, vobject anim, float blendin, float blendout, BoneMask mask, int fps, int flags)
//Flags
//	AF_ONCE:
// animace se prehraje jen jednou, zustane "zamrzla" na poslednim frame a vyvola se EV_ANIMEND
// AF_BLENDOUT:
// defaultne zustava animace pri prehravani a AF_ONCE po skonceni na posledni frame "zamrzla", dokud neni nahrazena jinou,
// nebo neni kanal vynulovan. Pokud se nastavi AF_BLENDOUT, postara se engine o vyhozeni animace sam a pro preblendovani pouzije hodnotu
// blendout. Pokud je odchycen EOnAnimEnd a byla zmenena animace na jinou, nebo byl kanal rucne vynulovan, tak se tato
// funkcnost neprovede.
// AF_USER:
// animace stoji na prvnim frame. Snimek se nastavuje rucne pomoci SetFrame
// AF_RESET:
// vynuti nastaveni animace na zacatek a pripadne blend i v pripade, ze ve slotu uz tato animace bezi.
// AF_FORCEFPS:
// defaultne se bere framerate z anim.def. Pokud tam neni uveden, bere se parametr 'fps'.
// Pomoci tohoto flagu je mozne prednostne pouzit parametr 'fps' i prestoze je framerate uveden v anim.def
// AF_NOANIMEND
// nevyvola se na konci EV_ANIMEND
// AF_NOANIMHOOKS
// nevyvolaji se animhooky

//jen zmeni masku, a blendne, pokud je nejaky 'blendin' nastaven
proto native void ChangeAnimSlotMask(class ent, int slot, float blendin, BoneMask mask)

//jen zmeni framerate na hodnotu 'fps'
proto native void ChangeAnimSlotFPS(class ent, int slot, int fps)

//nastavuje masku prehravanych kanalu. Nastavuji se bity 0..3 (kanalu je 12) 
proto native void	SetAnimMask(class ent, int mask)

//clearuje masku prehravanych kanalu. Vraci vyclearovane bity
proto native int	ClearAnimMask(class ent, int mask)

//vrati nastavene bity tech kanalu, ktere maji nastavenou animaci, nejsou na konci
// a pro ktere byla nastavena vstupni maska.
//Je tim mozno se dotazat na stav vice slotu najednou
proto native int IsAnimSlotPlaying(class ent, int mask)
//mask - 16bitu, pro 16 anim slotu. Maximalni hodnota je tedy 0xffff!

//sets how much this morph affect object
proto native bool SetMorphState(class ent, string morph, float value);
//morph name
//value 0...1

//vraci jmeno objektu(animace)
proto string vtoa(vobject vobj);

//------------------------------------------
// PARTICLE ENGINE API
//------------------------------------------
//vraci celkovy pocet aktivnich particles ve vsech emitorech
proto native int GetEffectorParticleCount(class ent)

//precte jmena nadefinovanych emitoru v particle effectu.
proto int		GetEmitors(class ent, string emitors[], int max)
//vraci pocet emitoru, a pole emitors naplni jmeny (do maximalni poctu max)

//nastavi parametr particle emitoru
//pokud emitor == -1, nastavi tento parametr u vsech emitoru
proto void		SetEffectorParm(class ent, int emitor, int parameter, void value)

//precte parametr particle emitorus
proto void		GetEffectorParm(class ent, int emitor, int parameter, void value)

//zmeni "predchozi pozici effectoru", jinak pri dalsim emitovani particlu dojde
//k linearni interpolaci mezi puvodni a aktualni pozici. Je nutno volat explicitne,
//protoze jinak neni mozno zjistit, jestli je skokova zmena pozice umyslna, nebo jde
//jen o neplynuly pohyb emitoru.
proto native void ResetEffectorPosition(class ent)

/*
EP_CONEANGLE 					//(vector R/W)
EP_VELOCITY						//(float R/W)
EP_VELOCITY_RND				//(float R/W)
EP_AVELOCITY					//(float R/W)
EP_SIZE							//(float R/W)
EP_STRETCH						//(float R/W)
EP_AIR_RESISTANCE				//(float R/W)
EP_AIR_RESISTANCE_RND		//(float R/W)
EP_GRAVITY_SCALE				//(float R/W)
EP_GRAVITY_SCALE_RND			//(float R/W)
EP_BIRTH_RATE					//(float R/W)
EP_BIRTH_RATE_RND				//(float R/W)
EP_LIFETIME						//(float R/W)
EP_LIFETIME_RND
EP_EFFECT_TIME					//celkovy cas efektoru (float R/W)
EP_CURRENT_TIME				//momentalni cas efektoru (float R/W)

EP_RANDOM_ROT					//otacej se nahodnym smerem (bool R/W)
EP_RANDOM_ANGLE				//zacinej s nahodnym natocenim (bool R/W)
EP_KILL_OLDEST					//po dosazeni max. particles, recykluj nejstarsi (bool R/W)
EP_POINT_SPRITES				//pouzivej HW pointsprites (bool R/W)
EP_REPEAT						//ma se efektor opakovat po uplynuti casu? (bool R/W)
EP_ACTIVE_PARTICLES			//pocet aktivnich particlu (int R)
EP_LIGHTING						//nasvetluje se (bool R/W)
EP_KILLBOX						//velikost kill shape (vector R/W)
*/
   
//------------------------------------------
// PHYSICS API
//------------------------------------------

//PhysX preliminary API

typedef int NxShape
typedef int NxJoint

class ClothDef
{
	float	Thickness
	float	Density
	float	BendingStiffness
	float	StretchingStiffness
	float	DampingCoefficient
	float	Friction
	float	Pressure
	float	CollisionResponseCoefficient
	float	AttachmentResponseCoefficient
	int	SolverIterations
	int	CollisionGroup
	int	Flags
//	NX_CLF_PRESSURE 
//	NX_CLF_TEARABLE 
//	NX_CLF_BENDING 
//	NX_CLF_COLLISION_TWOWAY 
//	NX_CLF_DAMPING 

}

proto native bool NxClothCreate(class ent, string surface, ClothDef cdef, bool attach)
proto native void NxClothDestroy(class ent)

//Actor flags
//NX_AF_DISABLE_COLLISION 
//NX_AF_LOCK_COM 
//Body flags
//NX_BF_DISABLE_GRAVITY 
//NX_BF_KINEMATIC 

proto native void NxApplyExplosion(vector center, float radius, float force)

proto native bool NxActorCreate(class ent, vector center, int geomtype, bool dynamic, float mass, int actorflags, int bodyflags)

proto native void NxActorSetFlags(class ent, int flags)
proto native void NxActorClearFlags(class ent, int flags)

proto native void NxActorSetBodyFlags(class ent, int flags)
proto native void NxActorClearBodyFlags(class ent, int flags)

//filtering of contact-reports
proto native void NxActorSetGroup(class ent, int group)
proto native int NxActorGetGroup(class ent, int group)
proto native void NxSceneSetActorGroupPairFlags(int group1, int group2, int flags)
//NX_IGNORE_PAIR  Disable contact generation for this pair.  
//NX_NOTIFY_ON_START_TOUCH  Pair callback will be called when the pair starts to be in contact.  
//NX_NOTIFY_ON_END_TOUCH

//filtering of collisions
proto native void NxActorSetCollisionGroup(class ent, int group)
proto native int NxActorGetCollisionGroup(class ent)
proto native void NxSceneSetGroupCollision(int group1, int group2, bool collision)

proto native void NxActorDestroy(class ent)
proto native NxShape NxShapeCreateBox(class ent, vector mat[], vector sizes, int material, float mass)

//vytvori mirror object pro PhysX
proto native bool dBodyCreatePhysXShadow(class ent, int group, int flags)
//PSF_KINEMATIC (static otherwise)
//PSF_COLLIDE_CLOTH
//PSF_COLLIDE_RB

proto native void dBodyDestroyPhysXShadow(class ent)

//Meqon API
proto float dShapeCastUpdate(class ent, int mode, class clipent, vector point, vector norm)
proto bool dShapeCastCreate(class ent, bool insert)
proto void dShapeCastDestroy(class ent)
proto void dShapeCastSetDir(class ent, vector dir)
proto void dShapeCastAddBox(class ent, vector mat[], vector size)
proto void dShapeCastAddSphere(class ent, vector mat[], float radius)
proto void dShapeCastAddPoint(class ent, vector mat[])
proto void dShapeCastAddCapsule(class ent, vector mat[], vector axis, float radius)

proto native void dSetInteractionLayer(int mask1, int mask2, bool enable)

//TODO: rename to dBodySetInteractionLayer
proto native void dBodyInteractionLayer(class ent, int mask)
proto native int dBodyGetInteractionLayer(class ent)


//aktivuje deaktivovany body
proto void dBodyActive(class ent, bool active, bool dynamic)

proto native bool dBodyIsDynamic(class ent)

proto native bool dBodyIsActive(class ent)

//vytvori RigidBody
proto bool dBodyCreate(class ent, vector center, int geom)

//center je offset z objekt-pivotu do realneho stredu. Bodies musi byt centrovane, jinak maji
//       asymetricky teziste

//Pokud uz ma entita prirazeny objekt, lze pouzit defaultni kontruktor na bodies:
//BC_NONE			- zadny
//BC_OBB				- Box
//BC_CAPSULE_X		- kapsle s delsi X osou
//BC_CAPSULE_Y		- kapsle s delsi Y osou
//BC_CAPSULE_Z		- kapsle s delsi Z osou
//BC_CONVEX			- konvexni tvar - vychazi z brushu, anebo polygonu, anebo proste vyrobi Box
//BC_SPHERE			- koule. Polomer vychazi z velikosti objektu v ose X
//BC_MODEL			- vychazi z interni geometrie nactene z XML

//zrusi body
//proto native void dBodyDestroy(class ent)
proto void dBodyDestroy(class ent)

//priradi rigid body vlastni user-typ
//proto native void dBodyType(class ent, int type)
//proto native void dBodyType(class ent, int type)
proto void dBodyType(class ent, int type)

typedef int dGeom
typedef int dJoint
typedef int dBlock

//je mozno vyloucit kolize mezi:
// dBodyCollisionBlock(entity, entity)
proto dBlock dBodyCollisionBlock(class ent1, class ent2)
proto void dBodyRemoveBlock(dBlock block)
proto native void dBodySetInertiaTensor(class body, vector m0, vector m1)
proto native void dBodySetGravity(class ent, vector g)
proto native vector dBodyGetGravity(class ent)

//vytvori geometricky tvar
proto dGeom dGeomCreateBox(vector size)
proto dGeom dGeomCreateSphere(float radius)
proto dGeom dGeomCreateCapsule(float radius, vector extent)
proto void dGeomDestroy(dGeom geom)
//zmeni velikost geometrie. funguje na:
//sphere: radius
//box: extents
//capsule: extents, radius
proto native void dGeomSetSize(dGeom geom, vector extent, float radius)

// prida geometrii k body. Libovolny pocet
proto int dBodyAddGeom(class body, dGeom geom, vector trans[4], string mat)

//nastavi hmotu body
//inertia urcuje setrvacnost angularniho pohybu v osach. Pokud vse osy stejne, chova se jako koule/krychle
proto void dBodySetMass(class ent, vector inertia, float mass)

proto native float dBodyGetMass(class ent)

//udeli impuls na bodu ve world souradnicich
proto void dBodyApplyImpulseAt(class body, vector impulse, vector pos)

//udeli impuls na origin body
proto void dBodyApplyImpulse(class body, vector impulse)

//pusobi silou na origin
proto void dBodyApplyForce(class body, vector force)

//pusobi silou na origin
proto void dBodyApplyForceAt(class body, vector pos, vector force)

//ziska aktualni uhlovou rychlost
proto vector dBodyGetAngularVelocity(class body)
//zmeni uhlovou rychlost
proto void dBodySetAngularVelocity(class body, vector angvel)

//nastavi cilovou pozici. pokud time == -1, stane se tak behem nasledujiciho snimku. Jinak za cas 'time'
proto native void dBodySetTargetFrame(class body, vector matrix[4], float timeslice, bool dynamics)
proto native float dBodyGetKineticEnergy(class body)

proto native vector dBodyGetLastVelocity(class body)
proto native float dBodyGetLastKineticEnergy(class body)

proto native vector dBodyGetVelocityAt(class body, vector globalpos)

//joint constraints
proto dJoint dJointCreateBall(class body1, class body2, vector point, bool block, float breakthreshold);
proto dJoint dJointCreateFixed(class body1, class body2, vector point, bool block, float breakthreshold);
proto dJoint dJointCreateHinge(class body1, class body2, vector point, vector axis, bool block, float breakthreshold);
proto native dJoint dJointCreateFixedOrientation(class body1, class body2, bool block, float breakthreshold);
proto dJoint dJointCreatePrismatic(class ent1, class ent2, vector point, vector axis, bool block, float breakthreshold);
proto dJoint dJointCreateSliding(class ent1, class ent2, vector point, vector axis, bool block, float breakthreshold);

//body1 je referencni, nejlepe staticke. body2 je 'animovane' relativne v souradnem systemu body1, 'mat' je lokalni matice
//urcujici pozici body2
proto dJoint dJointCreateAnim(class body1, class body2, vector mat[4], bool block, float breakthreshold);
proto native void dJointAnimSetMatrix(dJoint joint, vector mat[4])
proto native void dJointAnimSetLinearMagnitude(dJoint joint, float min, float max)

//other constraints
proto dJoint dJointCreateAngularLimit(class body1, vector axis1, class body2, vector axis2, float angle, float breakthreshold);
proto dJoint dJointCreateAngularMotor(class body1, class body2, vector axis, float angvel, float torque);
proto native dJoint dJointCreateLinearMotor(class body1, class body2, vector axis, float vel, float force);

proto dJoint dJointCreateAngularSpringDamper(class body1, vector axis1, class body2, vector axis2, float k, float d, float breakangle);
proto dJoint dJointCreateLinearLimit(class ent1, vector point1, class ent2, vector point2, float maxDist, float breakthreshold);
proto dJoint dJointCreateSpringDamper(class ent1, vector point1, class ent2, vector point2, float k, float d, float len, float breaklength);

proto native void dJointSetMotor(dJoint motor, float velocity, float force)

//relevant for
//AngularSpringDamper
//AngularLimit
//Hinge
proto native void dJointSetAxes(dJoint joint, vector axis1, vector axis2)

//relevant for
//LinearSpringDamper
//LinearLimit
//Fixed
//Ball
proto native void dJointSetPoints(dJoint joint, vector point1, vector point2)

proto void   dJointBreak(dJoint joint)

//zrusi joint
proto void dJointDestroy(int joint)

proto native bool dMaterialLoad(string xmlfile)
proto native int dMaterialClone(string target, string source, int material_index)
proto native int dMaterialGetType(string source)
proto native int dMaterialSetType(string source, int material_index)

proto int dRagdollCreate(class owner, string name, float mass, string material, ctype boneclass, class bones[])
//vytvori z entity ragdoll.
//owner - entita s X objektem (jine nelze pouzit!)
//name - jmeno ragdoll definice z obj/ragdolls.h
//mass - mass index
//material - definice materialu z obj/materials.xml
//boneclass - class, ktery ma vytvorit pro kazdou kost, pokud je budem chtit
//bones - pole vytvorenych entit pro kazdou kost. Lze s nimi registrovat kolize, pripadne ovladat dynamiku
//        prez sadu fci dBodyXXX, pripojovat jointy apod...
//        Nesmi se deletovat driv, nez se zrusi ragdoll, nedaji se z nich cist pozice, ani uhly. Od toho je
//        GetBoneMatrix pro X objekt
// !!pole musi byt [64], a indexy kosti pro X objekt pak koresponduji s pointery v poli. Nesimulovane
// kosti maji NULL pointer
//fce vraci 0, v pripade neuspechu, nebo pocet kosti



proto void dRagdollDestroy(class owner, bool keepbones = false)
//uklidi i pripadne entity vytvorene pro kosti

//-----------------------------------------------------------------
class Contact
{
	const int	Pointer //internal pointer

	int		Flags1 //surfparms lokalniho materialu
	int		Flags2 //surfparms druheho materialu
	vector	Normal
	vector	Position

	proto native float GetFrictionMagnitude() 
	proto native vector GetFrictionDirection()
	
	proto native vector GetNormalImpulse()
	proto native vector GetFrictionImpulse()

	proto native float GetRelativeVelocityBefore(vector vel)
	proto native float GetRelativeVelocityAfter(vector vel)
	proto native vector GetRelativeTangentialVelocityBefore()
	proto native vector GetRelativeTangentialVelocityAfter()
}

class Constraint
{
	proto private void ~Constraint()
	
	proto native void  setBody1(class body)
	proto native class getBody1()
	proto native void  setBody2(class body)
	proto native class getBody2()
	
	proto native void setAxis1(vector axis)
	proto native vector getAxis1()
	proto native void setAxis2(vector axis)
	proto native vector getAxis2()

	proto native void setConstraintError(float error) 
	proto native float getConstraintError() 
	proto native void setMagnitude(float magnitude) 
	proto native float getMagnitude() 
	proto native void setMaximumMagnitude(float magnitude) 
	proto native float getMaximumMagnitude() 
	proto native void setMinimumMagnitude(float magnitude) 
	proto native float getMinimumMagnitude() 
	proto native void setTarget(float target) 
	proto native float getTarget() 

	//only point&contact constraint
	proto native void setAxis(vector axis)
	proto native vector getAxis()
	proto native void setPoint(vector point)
	proto native vector getPoint()
	
	//only contact constraint
	proto native float getStaticFriction()
	proto native void setStaticFriction(float staticFriction) 
	proto native float getDynamicFriction()
	proto native void setDynamicFriction(float dynamicFriction) 
	proto native float getRestitution()
	proto native void setRestitution(float restitution) 
	proto native float getFrictionMagnitude()
	proto native vector getFrictionDirection()
	proto native void setFrictionDirection(vector frictionDirection) 
	proto native void setFrictionMagnitude(float frictionMagnitude) 
	proto native void setRelativeSurfaceVelocity(vector relativeSurfaceVelocity) 
	proto native vector getRelativeSurfaceVelocity()

	proto native void destroy()
}

proto native Constraint dConstraintCreatePoint()
proto native Constraint dConstraintCreateLinear()
proto native Constraint dConstraintCreateAngular()
proto native Constraint dConstraintCreateContact()

proto native bool dConstraintInsert(Constraint c)

proto void VerletStep(vector npos[], vector opos[], vector force[], float imass[], float tstep, int num)
proto float StickConstraints(int sticks[][2], vector npos[], float length[], float lengthsq[], float imass[], int num)

//----------------------------------------------
// PATHENGINE API
//----------------------------------------------

class CPosition extends _serializable
{
	int x, y, cell
	int group

	bool IsValid()
	{
		if(cell == -1)
			return false
		
		return true
	}
	
	bool IsEqual(CPosition pos)
	{
		if(cell != pos.cell)
			return false
		
		return true
	}
	
	CPosition Invalidate()
	{
		cell = -1
		return this
	}
	
	void CPosition()
	{
		cell = -1
	}
}

class CPath
{
	proto private void ~CPath()
	proto private void CPath()
	
	proto native void Destroy()
	proto native int Size()
	proto native vector Position(int index)
	proto native float Length()
	proto native class Connection(int index)
}

class CAgent
{
	proto private void ~CAgent()
	proto private void CAgent()
	
	proto native void Destroy()
	
	//pokud cost >=0, stane se agent soft-obstacle, a muze byt prekonan.
	proto native void SetTraverseCost(float cost)
	proto native float GetTraverseCost()

	//muze vratit false, pokud pro jeho rozmer neni predpocitany zadny navigacni mesh
	proto native bool CanPathfind()
	
	proto native class GetOwner()
	proto native void SetOwner(class owner)
}

class PathFinder
{
	proto private void ~PathFinder()
	proto private void PathFinder()

	//agent is NULL or agent to update
	proto native CAgent AgentFromOBB(CAgent agent, vector matrix[], vector mins, vector maxs, float traversecost = -1)
	proto native CAgent AgentFromID(CAgent agent, vector pos, int id, float traversecost = -1)
	
	//posune agenta na novou pozici. Muze vratit false, pokud by se Agent dostal mimo groupu.
	proto native bool MoveAgent(CAgent agent, vector pos)

	//posune agenta na novou pozici. Muze vratit false, pokud by se Agent dostal mimo groupu.
	proto native bool UpdateAgent(CAgent agent, vector matrix[], vector mins, vector maxs)
	
	//
	proto native bool FindClosestPosition(int shape, CPosition pos, CPosition cp, float maxdist)
	proto native bool PositionForPoint(vector pos, CPosition cp)

	proto native vector PointForPosition(CPosition cp)
	
	//vyhleda trasu z bodu 'from' do bodu 'to'
	//vraci pointer na CPath, nebo NULL, pokud trasu nenasel
	//flags:
	// 1 - nepouzivej offmesh connections!
	// 2 - nelimituj prostor pro hledani trasy
	proto native CPath FindShortestPath(int shape, CPosition from, CPosition to, int flags)

	//otestuje primku mezi body na kolize. 
	//vraci true, pokud ke kolizi nedoslo
	proto native bool TestLineCollision(int shape, CPosition from, CPosition to)

	proto native int GetPortalNames(string names[], int size)
	proto native void SwitchPortal(string names, int state)

	proto native int AddConnection(class ent, CPosition p0, CPosition p1)

	//Finds waypoints in radius and fill pointers into ents array up to arraysize.
	//returns number of waypoints
	proto native int FindWaypoints(CPosition from, float radius, class ents[], int arraysize)
	
	//returns node id of linked waypoint, or 0 if position is wrong
	proto native int LinkWaypoint(class ent, CPosition cpos)
	
	//removing waypoint. Returns true if there exists ent on node id
	proto native bool UnlinkWaypoint(class ent, int node)

	proto native void LoadEntities()
	
	//shapes - bitmask of shapes we want to preprocess for
	proto native void Complete(int shapes)

	//prida do seznamu novy tvar. Pro tyto tvary bude proveden preprocessing, takze by jich melo byt
	//co nejmene na jednu mapu!!!
	proto native bool SetShape(int id, int pnts[], int nump)
	
	//returns true, if shape is is obstructed on this position
	proto native bool TestPointCollision(int id, CPosition pos)

	//vraci posledni chybu pri hledani trasy (FindShortestPath)
	proto native int GetLastError()
}

bool IsConnected(CPosition p0, CPosition p1)
{
	if(p0 == NULL || p1 == NULL || p0.group != p1.group)
		return false

	return true
}

//----------------------------------------------

//nahraje novou navigacni mapu do worldu. Pokud uz nejaka byla, tak ji nahradi.
//Mazat se nemusi - odstrani se spolu s worldem
proto native PathFinder NAVLoad(string name)


//----------------------------------------------
// FILES API
//----------------------------------------------
typedef int tparse

proto native tparse	BeginParse(string filename);
proto int		ParseLine(tparse tp, int num, string tokens[])
proto native void		EndParse(tparse file);

proto int OpenFileName(string path, string file, string filtername, string filter)

//ziska kompletni systemovou trasu k souboru (pokud to neni ze ZIPu)
proto string GetFilePath(string name)

//ziska lokalni jmeno souboru. Tj. odrizne trasu k filesystemu
proto string GetRelativeFilePath(string name)

//otevre soubor
proto int		OpenFile(string name, int mode);
//name		-	jmeno souboru
//mode		-	FILEMODE_WRITE,FILEMODE_READ
//vysledek	-	handle souboru

proto int 		ReadFile(int file, void array, int length)

//zavre soubor
proto void		CloseFile(int file);
//file		-	handle souboru

//vypise do souboru hodnotu promenne
proto void		FPrint(int file,void var);
//vypise do souboru hodnotu promenne a odradkuje
proto void		FPrintln(int file,void var);
//nacte radek ze souboru.
proto int		FGets(int file,string var);
//vysledek	-	pocet nactenych znaku, nebo -1 kdyz uz neni co cist

//spusti editor nadefinovany v registrech.
proto native void Edit(string name, int line);
//name		-	soubor urceny k editaci
//line		-	na jakou radku ma skocit


//----------------------------------------------
// MISCELANEOUS API
//----------------------------------------------

typedef int hshader
typedef int Decal

//find&load shader
proto hshader GetShader(string name)
proto void ReleaseShader(hshader shader)

proto int LockTexture(string texture, int ptr[], int x, int y, int pitch)
proto void UnlockTexture(int tex)   

proto native Decal CreateDecal(class ent, vector origin, vector project, float nearclip, float angle, float size, int shader, float lifetime, int flags)

proto native void DecalParms(Decal decal, float alpha, float fadetime)

//kdyz lifetime=0, tak vraci fce pointer na decal, ktery muze byt pote odstranen prez RemoveDecal
//lifetime je v sekudach
proto native void RemoveDecal(Decal decal)

class OcclusionQuery
{
	proto private void ~OcclusionQuery()

	//vraci vysledek Query
	proto native int GetResult()

//  -1 vysledek jeste neni k dispozici. Zkus to o frame pozdeji
//  >0 bod je videt
// ==0 bod je neviditelny

	proto native void SetPosition(vector pos)
	proto native void Destroy()
	
}

//zjisti perpixel visibilitu z GPU. Pozice je ve world souradnicich.
//Size je velikost testovaneho bodu. Testuje se rovnostrannym trojuhelnikem o hrane delky size.
//vraci class, ktery muze byt nasledne dotazovan
proto native OcclusionQuery CreateOcclusionQuery(vector pos, float size)

//performance counter
proto native int TickCount(int prev)

proto void CalcTrailVertsPositions(vector BezierPoints[], int NumBezierPoints, int SegNum, float TrailLength, float radius, vector ResultVerts[])


//zjisti stav klavesy (viz. soubor keys.h)
proto int      KeyState(int key);
//key			-		kod klavesy

//"shodi" klavesu.Zamezi se tim "autorepeatu". Klavesa muze byt Set az pote, co je uvolnena a opet stisknuta
proto void     ClearKey(int key);
//key			-		kod klavesy

proto int CompileAnims(string name)

proto native void Repaint()
proto native void ResetRender(int width, int height)

//Vypise obsah promenne do konzole.
proto void     Print(void var);

//Vypise obsah stacku do konzole
proto void DumpStack();

//slouzi k vypisovani kritickych chyb, ktere maji byt
//vice zviditelnene (specialni log, popup okno, listbox apod)
proto void     DPrint(string var);

proto int      CreateBezierEx(vector array[], int numpoints);
proto int		UpdateBezier(int bezier, vector array[], int numpoints);
proto int      RemoveBezier(int bezier);

//vrati pozici na krivce, rozsah vzdalenosti 0 - 1
proto vector   GetPositionBezier(int bezier, float dist);
//vrati nejblizsi pozici na krivce
proto float    GetNearestPositionBezier(int bezier, vector pos);
proto float    GetLengthBezier(int bezier);

#ifdef _XBOX
string GetDateTime()
{
	return "2004/01/01 12:00:00"
}

#else

proto void "kernel32.dll" GetLocalTime(int ptr[])

string GetDateTime()
{
	int datetime[4]
	GetLocalTime(datetime)

	string dtime = itoa(datetime[0] & 0xffff) + "/"  //year
	dtime += itoal(datetime[0] >> 16 & 0xffff, 2) + "/"  //month
	dtime += itoal(datetime[1] >> 16 & 0xffff, 2) + " " //day
	dtime += itoal(datetime[2] & 0xffff, 2) + ":" 		 //hour
	dtime += itoal(datetime[2] >> 16 & 0xffff, 2) + ":"  //minute
	dtime += itoal(datetime[3] & 0xffff, 2) //second
	return dtime
}
#endif

proto int QueryBrushes(vector origin, vector min, vector max, int brush[], int fmask, int ctmask)
proto int GetBrushFaces(int brush, float face[][4])

proto void ReleaseEffect(int effect)
proto int PlayEffect(int effect)
proto int GetEffect(string name)

proto int ClipPoly(vector in[], int num, vector out1[], int num1, vector out2[], int num2, float plane[4])
proto int PolyToTriangles(vector in[], int num, vector out1[])

string String(string s)
{
	return s
}

//-----------------------------------------------------------------
float CorrectAngle(float yaw, float a, float speed)
{
	yaw = FixAngle(yaw)
	a = FixAngle(a)
	
	if(fabs(a - yaw) > 180)
	{
		if(yaw < a)
			yaw = yaw + 360
		else
			yaw = yaw - 360
	}

	if(yaw < a)
	{
		yaw = ftime * speed + yaw
		if(yaw > a)
			yaw = a
	}
	else
	{
		yaw = ftime * -speed + yaw
		if(yaw < a)
			yaw = a
	}


	return FixAngle(yaw)
}

//-----------------------------------------------------------------
// Fce na podporu ingame skriptu
// preparsuje skript a nahradi parametry.
proto string PrepareScript(string script, string parms, string parmnames[], int parmlengths[], int numparms)
//script - zdrojovy kod
//parms - parametry volani, oddelene carkama
//parmnames - pole se jmeny parametru
//parmlengths - pole s delkami jmen parametru
//numparms - pocet parametru

//=================================================================================
int VectortoRGBA( vector vec, float h)
{
float x,y,z
int r,g,b,a,rgba

	x = vec[0]
	y = vec[1]
	z = vec[2]

	x = x * 127.0 + 128.0
	y = y * 127.0 + 128.0
	z = z * 127.0 + 128.0	
	h = h * 255.0

	a = (int)h << 24
	r = (int)x << 16
	g = (int)y << 8
	b = z
	
	return r | g | b | a
}


//-----------------------------------------------------------------
int ARGB(int a, int r, int g, int b)
{
	a = a << 24
	r = r << 16
	g = g << 8
	return a | r | g | b
}

//-----------------------------------------------------------------
int ARGBF(float fa, float fr, float fg, float fb)
{
	int a = fa << 24
	int r = fr << 16
	int g = fg << 8
	return a | r | g | fb
}

//-----------------------------------------------------------------
int AWHITE(int a)
{
	return a << 24 | 0xffffff
}

//-----------------------------------------------------------------
int LerpARGB(int c1, int c2)
{
	int cb1, cb2
	const int cmask = 0x00ff00ff
	
	cb1 = c1 >> 8 & cmask
	cb2 = c2 >> 8 & cmask
	cb1 = cb1 + cb2 >> 1
	
	c1 = c1 & cmask
	c2 = c2 & cmask
	c1 = c1 + c2 >> 1
	
	return cb1 << 8 | c1
}

void SetMatrixEx(class ent, vector angs, vector pos)
{
	vector mat[4]
	
	RollPitchYawMatrix2(angs, mat)
	mat[3] = pos
	SetMatrix(ent, mat)
}

proto native void Error2(string title, string err)

void Error(string err)
{
	Error2("", err)
}

//for editor and console commands only!!
proto native void UpdateCubeMap(class owner, int size, string name)
proto bool "kernel32.dll" DeleteFileA(string s)
proto bool "kernel32.dll" MoveFileA(string source, string target)

#ifdef DEVELOPER

proto int "kernel32.dll" GetTickCount()
proto int "user32.dll" ShowCursor(bool show)
proto int "user32.dll" CreatePopupMenu()
proto int "user32.dll" AppendMenuA(int menu, int flags, int item, string text)
proto int "user32.dll" InsertMenuItemA(int menu, int pos, bool bypos, int info[])
proto int "user32.dll" TrackPopupMenu(int menu, int flags, int x, int y, int res, int wnd, int rect)
proto int "user32.dll" DestroyMenu(int menu)
proto int "user32.dll" GetActiveWindow()
proto int "user32.dll" SetMenuDefaultItem(int menu, int item, bool bypos)
proto int "user32.dll" SetMenuItemInfoA(int menu, int item, bool bypos, int info[])
proto int "user32.dll" ClientToScreen( int hwnd, int point[2])
proto int "user32.dll" SetCursorPos(int x, int y)
proto void "kernel32.dll" CopyFileA(string existing, string newfile, bool nooverwrite)
proto int "kernel32.dll" GetFileAttributesA(string file)
proto int "user32.dll" MessageBoxA(int wnd, string text, string caption, int style)
proto int "kernel32.dll" GetComputerNameA(string s, int l[])
proto int "user32.dll" GetAsyncKeyState(int key)
proto int "kernel32.dll" CreateDirectoryA(string path, int SecurityAttributes)


//just for cutscene export
proto native bool OpenVideoStream()
proto native void CloseVideoStream()
proto native void AppendVideoFrame()
proto native void MakeScreenshot(string name)
//

proto native void ResetCursor()
proto native void ResetInputDevice()

void SetMousePos(int x, int y)
{
	int point[2]
	
	point[0] = x
	point[1] = y
	
	ClientToScreen(GetActiveWindow(), point)
	SetCursorPos(point[0], point[1])
}

proto int		ListBox(string values[], int numvals, int def, string title)

class number
{
	int text
}

class Str 
{
	string text
	
	int s(string s)
	{
		text = s
		
		number n = this
		return n.text
	}
}

//-----------------------------------------------------------------
bool Bool(int num)
{
	if(num == 0)
		return false
	
	return true
}

//-----------------------------------------------------------------
class UIMenu extends _eclass
{
	int Handle
	
	int info[12]
	int ItemID[512]
	int NumItems

	Str str2int

//-----------------------------------------------------------------
	void SetCheckItem(int id, bool check)
	{
		for(int n = 0; n < NumItems; n++)
		{
			if(ItemID[n] == id)
			{
				info[1] = 0x00000001 //fMask
				info[3] = check << 3//fState

				SetMenuItemInfoA(Handle, n, true, info)
				return
			}
		}
	}
	
//-----------------------------------------------------------------
	int AddTextItem(string text, int id)
	{
		ItemID[NumItems] = id
		info[1] = 0x00000142 //fMask
		info[2] = 0
		info[4] = id //wID
		info[9] = str2int.s(text) //dwTypeData
		info[10] = strlen(text) //cch

		InsertMenuItemA(Handle, NumItems, true, info)

		NumItems++

		return NumItems - 1
	}
	
//-----------------------------------------------------------------
	int AddSeparator()
	{
		info[1] = 0x00000100 //fMask
		info[2] = 0x00000800 //fType

		InsertMenuItemA(Handle, NumItems, true, info)
		
		NumItems++
		
		return NumItems - 1
	}
	
//-----------------------------------------------------------------
	int AddSubMenu(UIMenu child, string text)
	{
		info[1] = 0x00000046 //fMask
		info[2] = 0x00000000 //fType
		info[5] = child.Handle
		info[9] = str2int.s(text) //dwTypeData
		info[10] = strlen(text) //cch

		InsertMenuItemA(Handle, NumItems, true, info)

		NumItems++

		return NumItems - 1
	}
	
//-----------------------------------------------------------------
	int AddCheckItem(string text, int id)
	{
		ItemID[NumItems] = id
		info[1] = 0x0000014a //fMask
		info[2] = 0
		info[4] = id //wID
		info[9] = str2int.s(text) //dwTypeData
		info[10] = strlen(text) //cch

		InsertMenuItemA(Handle, NumItems, true, info)

		NumItems++

		return NumItems - 1
	}
	
//-----------------------------------------------------------------
	void TPopup(int X, int Y)
	{
		ShowCursor(true)
		Sleep(10)

		TrackPopupMenu(Handle, 0, X, Y, 0, GetActiveWindow(), 0)
	}
	
//-----------------------------------------------------------------
	void Popup(int X, int Y)
	{
		thread TPopup(X, Y)
	}
	
//-----------------------------------------------------------------
	int IPopup(int X, int Y)
	{
		ShowCursor(true)
		return TrackPopupMenu(Handle, 0x0100, X, Y, 0, GetActiveWindow(), 0)		
	}
	
//-----------------------------------------------------------------
	void UIMenu()
	{
		info[0] = 48

		Handle = CreatePopupMenu()
		str2int = new Str
	}

	void ~UIMenu()
	{
		delete str2int
		DestroyMenu(Handle)
	}
}

#endif





/*
//globalni promenne, nastavene enginem

float ftime       - --||-- ve vterinach
int MouseDeltaX   - relativni X souradnice od posledniho frame
int MouseDeltaY   - relativni Y souradnice od posledniho frame
int LMB           - Stav leveho tlacitka mysi (true/false)
int MMB           - Stav stredniho tlacitka mysi (true/false)
int RMB           - Stav praveho tlacitka mysi (true/false)
int MouseWheel		- delta kolecka +/- hodnota v krocich
int time          - cas od posledniho frame v setinach vteriny

g_lod
g_bump
g_env

g_iFullscreen
g_iWidth
g_iHeight
g_iDepth
g_iZBuffDepth
g_iTexDepth
g_iMipBias
g_iTriFilter
g_iTriBuffer
g_iDither
g_iVSync

g_iCastShadow
g_iDetailMap
g_iGeomDetail
g_iShadow
g_iBrightness
g_iDisablePhysics

g_iStats				(bool) zapina HUD se statistikama
g_iWire				(bool) zapina wireframe zobrazeni
g_iDepthComplex	(bool) zapina zobrazeni prekresleni

g_fov					horizontalni uhel zobrazeni. Pocita se polovina, takze 30 odpovida FOV 60'
g_fovy				vertikalni uhel. Defaultne se bere 4/3 aspekt. Toto je jen pro upravu orezovych planes.
g_fVisDist			(units) max. distance orezovy plane.
g_vMainMatrix2		(float[4][4]) transformacni matice pro cube-envmapping
g_iViewFrame		(int) unikatni cislo frejmu
g_iIsOutdoor		(bool) jsme v otevrenem prostranstvi (je na dohled obloha)
g_vSunDirection
g_vSunColor
g_fSunIntensity

g_fDecalAlpha
g_iDecalFrame

JoyAxis
JoyRAxis
JoySliders
JoyButtons
JoyPOV

g_fDopplerFactor
g_iMusicVolume
g_iEffectsVolume
g_iSoundQuality
g_iSpeakers
g_bEAX
g_iShowPhysics

g_iShowAABB			(int) 0-3 zobrazi bound-boxy entit. 1 jen X, 2 i TOBy, 3 vsechno
g_iShowFilter		(int) maskuje renderovane fejsy. Defaultne 7. 1 - ,2 - , 4 -
g_iStatNumTriangles
g_iStatNumVertexes
g_iStatNumParticles
g_iStatFPS

_depth				(float) depth bias pro decals. Defaultne 0.05
*/

