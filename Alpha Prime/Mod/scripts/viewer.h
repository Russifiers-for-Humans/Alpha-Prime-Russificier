
#ifdef _XBOX
void ShowCursor(bool show)
{
}
#endif


string gTOBView
int gOBJType
string tobname
string gDisplayProgram
func_display TestDisplay
ctype gCreatureType

class HelpModelClass extends MinClass
{
	vobject HelpObject

	void HelpModelClass()
	{
		HelpObject = GetObject("obj/creatures/arnold/arnold.xob")
		SetObject(HelpObject, "")
		SetFlags(this, TFL_VISIBLE)
		SetOrigin(this, "0 32 29")
		SetAngles(this, "0 270 0")
	}
	
	void ~HelpModelClass()
	{
		if(HelpObject)
		{
			ReleaseObject(HelpObject, 0)	
			HelpObject = VNULL
		}
	}
}

class viewer extends MinClass
{
	widget ParticleStat
vector coords,angles

string path, file
string opath, ofile
vobject anm
widget wtext
float fps

int PAnim

string animname
bool body


AICreatureClass CEntity
vobject CEntityAnims[256]
vobject NullAnims[256]

MinClass StaticObjectOwner
vobject PObject
HelpModelClass HelpModel


	MiscTestDialogue 	m_Object
	bool			m_bRenderReady


    //-----------------------------------------------------------------
    MinClass LoadTestObject(string objectname, string morphname, string soundname)
    {
/*      if (CEntity)
        {
            delete CEntity
            CEntity = NULL
        }
*/
	m_bRenderReady = false

        if (m_Object)
        {
            delete m_Object
            m_Object = NULL
        }

        m_Object = new MiscTestDialogue(objectname, morphname, soundname)

        if (m_Object.m_bReady)
        {
 	    m_bRenderReady = true	  
	    CEntity = m_Object

            angles = "0 0 0"
            SetAngles(this, angles)
        } else
        {
            delete m_Object
            m_Object = NULL
        }

        return m_Object
    }

    //-----------------------------------------------------------------
    void PlayTestObject()
    {
        if (m_Object)
        {
            m_Object.StartAnim()
        }
    }



    //-----------------------------------------------------------------
    void LoadOtherStaticObject(string file)
    {
        vobject sobj = GetObject(file)

        if(sobj)
        {
            if(!StaticObjectOwner)
            {
                StaticObjectOwner = new MinClass()
                SetOrigin(StaticObjectOwner, coords + "0 0 1")
                SetFlags(StaticObjectOwner, TFL_VISIBLE|TFL_DYNAMICBBOX)
            }

            SelectObject(StaticObjectOwner, sobj)
        }
    }

//-----------------------------------------------------------------
    MinClass GetObjectPointer()
    {
        return CEntity
    }

//-----------------------------------------------------------------
    void SetOrientation(vector pos, vector angles)
    {
        if(CEntity)
        {
            SetAngles(CEntity, angles)
            coords = pos
        }
    }

//-----------------------------------------------------------------
    void MorphObject(class ent, string morph, float value)
    {
        SetMorphState(ent, morph, value);
    }

//-----------------------------------------------------------------
    void LoadObject(string file)
    {
	m_bRenderReady = false

        if (m_Object)
        {
            delete m_Object
            m_Object = NULL
            CEntity = NULL
        }

        vobject nobj = GetObject(file)

        if(nobj)
        {
            if(strstr(file, ".xob") != -1)
                gOBJType = 0
            else
            {
                if(strstr(file, ".ptc") != -1)
                    gOBJType = 2
                else
                    gOBJType = 1
            }

            if(body)
            {
                dBodyDestroy(this)
                body = false
            }

#ifndef EDITOR
            SetWidgetText(wtext, 0, file)
#endif
            SelectObject(this, nobj)

            SetFlags(this, TFL_DYNAMICBBOX)
            vector mins, maxs
            GetBoundBox(this, mins, maxs)
#ifndef EDITOR
            SetWidgetText(wtext, 4, "Box size: " + Vector2String(mins) + " - " + Vector2String(maxs))
#endif

            if(gOBJType == 0)
            {
                if(dBodyCreate(this, ZeroVec, BC_MODEL))
                {
	                dBodyActive(this, true, false)
	                body = true
//                    dBodyCreate(this, ZeroVec, BC_OBB)
                }

                    //spocitej defaultni mass & inertia

            }

		m_bRenderReady = true
		CEntity = this


            if(anm)
                SetAnimSlot(CEntity, 0, anm, 4.0, 0, NULL, fps, AF_FORCEFPS)
        }
    }

//-----------------------------------------------------------------
    void DrawGrid(int show, int grid, vector end)
    {
        if(show == 0)
            return

        int ncolor, fcolor

        if(show == 1)
        {
            ncolor = 0x4f000000
            fcolor = 0xff000000
        }
        else
        {
            ncolor = 0x4fffffff
            fcolor = 0xffffffff
        }

        const float tsize = 512, tsize2 = 256

        int lnum = tsize / grid
        int color

        float sx = end[0] - tsize2

        int mid = lnum / 2

        vector v1, v2

        v1[2] = end[2] + 0.5
        v2[2] = v1[2]

        v1[1] = end[1] - tsize2
        v2[1] = end[1] + tsize2

        for(int x = 0; x < lnum; x++)
        {
            v1[0] = sx
            v2[0] = sx

            color = ncolor

            if(x == mid)
                color = fcolor

            AddDShape(SHAPE_LINE, color, SS_NOZBUFFER|SS_ONCE|SS_NOZUPDATE|SS_TRANSP|SS_NOCULL, v1, v2)
            sx = sx + grid
        }

        float sy = end[1] - tsize2

        v1[0] = end[0] - tsize2
        v2[0] = end[0] + tsize2

        for(int y = 0; y < lnum; y++)
        {
            v1[1] = sy
            v2[1] = sy

            color = ncolor

            if(y == mid)
                color = fcolor

            AddDShape(SHAPE_LINE, color, SS_NOZBUFFER|SS_ONCE|SS_NOZUPDATE|SS_TRANSP|SS_NOCULL, v1, v2)

            sy = sy + grid
        }

    }

//-----------------------------------------------------------------
    void EOnFrame(MinClass other, int extra)
    {
    		if(ParticleStat)
    		{
    			string particles = "Particles: " + itoa(GetEffectorParticleCount(this))
    			SetWidgetText(ParticleStat, 0, particles)
    		}

        //LipsSync viewer
        if (m_Object)
        {
            SetOrigin(this, ZeroVec)
            SetCamera(coords, angles)

            BumpFrame (m_Object.m_AnimObject, ftime, 0)
            m_Object.SetBonesSetup()
            return
        }

//	if (!m_bRenderReady)
//		return


        if(TestDisplay) //iba pre display viewer
        {
            if(KeyState(KC_NUMPAD1))
            {
                ClearKey(KC_NUMPAD1)
                TestDisplay.RunActionFromIndex(0)
            }

            if(KeyState(KC_NUMPAD2))
            {
                ClearKey(KC_NUMPAD2)
                TestDisplay.RunActionFromIndex(1)
            }

            if(KeyState(KC_NUMPAD3))
            {
                ClearKey(KC_NUMPAD3)
                TestDisplay.RunActionFromIndex(2)
            }

            if(KeyState(KC_NUMPAD4))
            {
                ClearKey(KC_NUMPAD4)
                TestDisplay.RunActionFromIndex(3)
            }

            if(KeyState(KC_NUMPAD5))
            {
                ClearKey(KC_NUMPAD5)
                TestDisplay.RunActionFromIndex(4)
            }

            if(KeyState(KC_NUMPAD6))
            {
                ClearKey(KC_NUMPAD6)
                TestDisplay.RunActionFromIndex(5)
            }

            if(KeyState(KC_NUMPAD7))
            {
                ClearKey(KC_NUMPAD7)
                TestDisplay.RunActionFromIndex(6)
            }

            if(KeyState(KC_NUMPAD8))
            {
                ClearKey(KC_NUMPAD8)
                TestDisplay.RunActionFromIndex(7)
            }

            if(KeyState(KC_NUMPAD9))
            {
                ClearKey(KC_NUMPAD9)
                TestDisplay.RunActionFromIndex(8)
            }

            TestDisplay.UpdateTime()
            TestDisplay.UpdateElements()
            return
        }

        vector v
        int n, z

    if(gOBJType == 4)
        {
            if(CEntity == NULL)
            {
                CEntity = Spawn(gCreatureType)
                if(CEntity)
                {
                    CEntity.flags = FL_PUPPET
                    CEntity.coords = ZeroVec
                    SetOrigin(CEntity, CEntity.coords)
                    SetAngles(CEntity, "0 90 0")

                    Throw(CEntity, this, EV_INIT, 0)
                    Throw(CEntity, this, EV_LOADED, 0)
                    ClearEventMask(CEntity, EV_FRAME)
                }

                if(CEntity.ASet)
                    CEntityAnims = CEntity.ASet.Anims
                else
                    CEntityAnims = NullAnims

                CompileAnims("anm/anim.def")
                for(z = 0; z < sizeof(CEntityAnims); z++)
                {
                    PAnim++
                    if(PAnim >= sizeof(CEntityAnims))
                        PAnim = 0

                    if(CEntityAnims[PAnim])
                    {
                        anm = CEntityAnims[PAnim]

                        SetAnimSlot(CEntity, 0, anm, 4.0, 0, NULL, fps, AF_FORCEFPS)
                        animname = ""
                        break
                    }
                }
            }

            SetVelocity(CEntity, ZeroVec)
            SetOrigin(CEntity, "0 0 48")
            SetWeight(CEntity, 0)
            //SetFlags(CEntity, TFL_NOMPIVOT)

            if(KeyState(KC_N))
            {
                ClearKey(KC_N)

                CEntity.CAmbientSoundEnd()
                CompileAnims("anm/anim.def")

                vobject prev = CEntityAnims[PAnim]
                for(z = 0; z < sizeof(CEntityAnims); z++)
                {
                    PAnim--
                    if(PAnim < 0)
                        PAnim = sizeof(CEntityAnims) - 1

                    if(CEntityAnims[PAnim] && CEntityAnims[PAnim] != prev)
                    {
                        anm = CEntityAnims[PAnim]

                        SetAnimSlot(CEntity, 0, anm, 4.0, 0, NULL, fps, AF_FORCEFPS)
                        break
                    }
                }
            }

            if(KeyState(KC_M))
            {
                ClearKey(KC_M)

                CEntity.CAmbientSoundEnd()
                CompileAnims("anm/anim.def")
                prev = CEntityAnims[PAnim]
                for(z = 0; z < sizeof(CEntityAnims); z++)
                {
                    PAnim++
                    if(PAnim >= sizeof(CEntityAnims))
                        PAnim = 0

                    if(CEntityAnims[PAnim] && CEntityAnims[PAnim] != prev)
                    {
                        anm = CEntityAnims[PAnim]

                        SetAnimSlot(CEntity, 0, anm, 4.0, 0, NULL, fps, AF_FORCEFPS)
                        break
                    }
                }
            }
        }


        if(CEntity == NULL)
            CEntity = this


        if(gOBJType == 2)
        {
            int frm = BumpFrame(CEntity, ftime, 0)
        }
        else
        {
            if(KeyState(KC_R))
            {
                ClearKey(KC_R)

                //FIXME prasacky sposob s temp animackou. nudzovo
                CompileAnims("anm/anim.def")
                vobject nanim2 = GetObject(animname)
                vobject tempanim = GetObject("anm/player/hands/Pistol_idle.anm")

                if(nanim2)
                {
                    if(anm)
                        ReleaseObject(anm, true)

                    SetAnimSlot(CEntity, 0, tempanim, 4.0, 0, NULL, fps, AF_FORCEFPS)

                    if(tempanim)
                        ReleaseObject(tempanim, true)

                    anm = nanim2
                    SetAnimSlot(CEntity, 0, anm, 4.0, 0, NULL, fps, AF_FORCEFPS)
                }
            }

            if(KeyState(KC_SPACE))
            {
                ClearKey(KC_SPACE)

                ShowCursor(true)
                if(OpenFileName(path, file, "Anims", "*.anm"))
                {
                    animname = path + "/" + file
                    vobject nanim = GetObject(animname)

                    if(nanim)
                    {
#ifndef EDITOR
                        SetWidgetText(wtext, 1, animname)
                        SetWidgetText(wtext, 2, ftoa(fps) + " fps")
#endif
                        if(anm)
                            ReleaseObject(anm, true)

                        anm = nanim
                        SetAnimSlot(CEntity, 0, anm, 4.0, 0, NULL, fps, AF_FORCEFPS)
                    }
                }
                ShowCursor(false)
            }


            if(KeyState(KC_RETURN))
            {
                ClearKey(KC_RETURN)

                ShowCursor(true)
                if(OpenFileName(opath, ofile, "Objects", "*.x;*.tob;*.xob"))
                {
                    LoadObject(opath + "/" + ofile)
                }
                ShowCursor(false)
            }

            if(KeyState(KC_UP))
            {
                ClearKey(KC_UP)
                fps = fps + 5
                clamp fps<0, 100>
                if(anm)
                    ChangeAnimSlotFPS(CEntity, 0, fps)
            }

            if(KeyState(KC_DOWN))
            {
                ClearKey(KC_DOWN)
                fps = fps - 5
                clamp fps<0, 100>
                if(anm)
                    ChangeAnimSlotFPS(CEntity, 0, fps)
            }

        frm = BumpFrame(CEntity, ftime, 0)
        }

        v = ZeroVec

#ifndef EDITOR
        SetWidgetText(wtext, 3, "frame: " + itoa(frm))

        SetWidgetText(wtext, 2, ftoa(fps) + " fps")
        if(anm)
            SetWidgetText(wtext, 1, vtoa(anm))
        else
            SetWidgetText(wtext, 1, "no anim")

        if(LMB)
        {
            v[0] = 0
            v[1] = (float)MouseDeltaX * 0.5
            v[2] = 0
            SetAngles(CEntity, GetAngles(CEntity) + v)
        }

        if(RMB)
        {
            v[0] = (float)MouseDeltaY * 0.5
            v[1] = 0
            v[2] = 0
            SetAngles(CEntity, GetAngles(CEntity) + v)
        }

        float m, k, scale = 100, scale2 = 100

        if(gOBJType == 2)
            scale = 200

        m = (float)MouseWheel * 32
        k = m

        if(KeyState(KC_W))
            m = ftime * -scale

        if(KeyState(KC_S))
            m = ftime * scale

        if(KeyState(KC_A))
            k = ftime * -scale2

        if(KeyState(KC_D))
            k = ftime * scale2

        coords[0] = coords[0] - m
        coords[2] = coords[2] - k

        if(coords[0] > -16)
            coords[0] = -16

        if(coords[0] < -1000)
            coords[0] = -1000
#endif
/*      World.CameraPos = coords
        World.CameraAngs = angles
        World.CameraNorm = Angles2Vector(World.CameraAngs)*/
        SetCamera(coords, angles)

        int gshow
        if(KeyState(KC_G))
        {
            ClearKey(KC_G)

            if(++gshow > 2)
                gshow = 0
        }
        DrawGrid(gshow, 8, "0 0 0")
    }

#ifdef EDITOR
    void OnKeyDownFromGUI(int KeyCode)
    {
        float step = 2
        float vert = 0
        float horz = 0

        switch(KeyCode)
        {
        case wx_W:
            horz += step
            break
        case wx_S:
            horz -= step
            break
        case wx_A:
            vert += step
            break
        case wx_D:
            vert -= step
            break
        }

        coords[0] = coords[0] - vert
        coords[2] = coords[2] - horz

        if(coords[0] > -16)
            coords[0] = -16

        if(coords[0] < -1000)
            coords[0] = -1000
    }
#endif



   void viewer()
   {
    	m_Object = NULL
 	CEntity = NULL
	m_bRenderReady = false

    	if(gDisplayProgram)
    	{
            if(!World.DispColPtr)
                World.DispColPtr = new DisplayCollection

            TestDisplay = new func_display(this, gDisplayProgram, true)
            TestDisplay.M_Play(this)
            SetEventMask(this, EV_FRAME)
            return
        }

#ifndef EDITOR
        wtext = CreateWidget(WTYPE_TEXT, 0, 200, 600, 600, WF_SOURCEALPHA | WF_BLEND|WF_VISIBLE, 0xe0ffffff, 255)
#endif
        LoadObject(gTOBView)
    SetFlags(this, TFL_VISIBLE|TFL_DYNAMICBBOX)
    SetEventMask(this, EV_FRAME)

    fps = 30

        SetOrigin(this, ZeroVec)

        vector mins, maxs, size
        GetBoundBox(this, mins, maxs)

        size = mins + maxs
        size = size * 0.5
        float lng = VectorLength(maxs - mins)
        lng = -lng
    coords[0] = lng
    coords[1] = size[0]
    coords[2] = size[2]

        if(coords[0] < -500)
            coords[0] = -500

        if(coords[0] > 500)
            coords[0] = 500

        if(coords[1] < -500)
            coords[1] = -500

        if(coords[1] > 500)
            coords[1] = 500

        if(coords[2] < -500)
            coords[2] = -500

        if(coords[2] > 500)
            coords[2] = 500

    angles = "0 0 0"

        SetAngles(this, "0 90 0")
        path = "anm"
        opath = "obj"

#ifndef EDITOR
        SetWidgetText(wtext, 0, tobname)
#endif

        World.ViewerClass = this
        Color = 0xffffffff
   }

//=================================================================================
    void PViewerRefresh(string ParticleFile)
    {
        SelectObject(this, VNULL)

        if(PObject)
            ReleaseObject(PObject, true)

        PObject = GetObject(ParticleFile/*"particle_temp.ptc"*/)
        SelectObject(this, PObject)
        
        if(!HelpModel)
        		HelpModel = new HelpModelClass()
        		
     	if(!ParticleStat)
     	   ParticleStat = CreateWidget(WTYPE_TEXT, 400, 0, 100, 10, WF_VISIBLE | WF_SOURCEALPHA|WF_BLEND, 0xffffffff, 5) 
    }

    void ~viewer()
    {
		if(HelpModel)
		{
			delete HelpModel
			HelpModel = NULL	
		}

		if (m_Object)
		   delete m_Object
		
		if(PObject)
		{
			ReleaseObject(PObject, true)
			PObject = VNULL
		}
		
		if(body)
		{
		   dBodyDestroy(this)
		   body = false
		}
		
		if(wtext)
		   DestroyWidget(wtext)
  
		 if(ParticleStat)
		 {
		 	 DestroyWidget(ParticleStat)
		 	 ParticleStat = NULL
		 }
    }
}

string wrlpath

bool TOBViewer(string objname)
{
    gTOBView = objname

   globalmap = LoadMap("worlds/viewer.wrl", true)

   if( globalmap == NULL)
      return false

   return true
}

bool CreatureViewer(string parms)
{
    gCreatureType = Type(parms)

   globalmap = LoadMap("worlds/viewer.wrl", true)
   if( globalmap == NULL)
      return false

   return true
}

bool DPLViewer(string parms)
{
    gDisplayProgram = parms
   globalmap = LoadMap("worlds/viewer.wrl", true)

   if( globalmap == NULL)
      return false

   return true
}

bool WRLViewer(string name)
{
    wrlpath = "worlds/"

    int p = strstr(name, ".map")
    if(p > 0)
    name = substr(name, 0, p) + ".wrl"

    if(strstr(name, ".wrl") == -1)
    name = name + ".wrl"

    if(strstr(name, wrlpath) == -1)
      name = wrlpath + name

    string tname = name

    int ln = strlen(wrlpath)

    if(strstr(tname, ".wrl") != -1)
        tname = substr(tname, 0, strlen(tname) - 4)

    if(strstr(tname, wrlpath) != -1)
        tname = substr(tname, ln, strlen(tname) - ln)

    if(strstr(tname, wrlpath) != -1)
        tname = substr(tname, ln, strlen(tname) - ln)

    BeforeRunFromCommandLine(DIFFICULTY_NORMAL)
    globalmap = LoadMapEx(tname)

   if( globalmap == NULL)
        return false

   return true
}
