
class light_effect extends LightClass
{
int ltim
float flashtime
float fDuration
bool bDeleteOnEnd
//-----------------------------------------------------------------
   void Switch(int onoff)
   {    	
   	if(onoff == true)
      {
         SetLightEx(LightHandle,intensity, radius, _color)
      }
   	else
      {
         SetLightEx(LightHandle,0,0, ZeroVec)
      }
   }

//-----------------------------------------------------------------
	int EOnFrame(MinClass other, int extra)
	{
		if(fDuration)
		{
			float fd = currenttime - flashtime
			fd = fDuration - fd
			clamp fd<0, fDuration>
			
			if(fd == 0)
			{
				fDuration = 0
				if(bDeleteOnEnd)
					delete this
				
				Switch(false)
				return
			}

			fd /= fDuration
         SetLightEx(LightHandle,0, radius, _color * fd)
		}
		
	}

//-----------------------------------------------------------------
	void Flash(float duration, bool deleteonend)
	{
		flashtime = currenttime
		
		SetEventMask(this, EV_FRAME)
 		Switch(true)
 		bDeleteOnEnd = deleteonend
 		fDuration = duration
	}

//-----------------------------------------------------------------
   void light_effect(vector coo, int intens, int rad, vector col, int flags)
   {
      ltim = 0

      SetOrigin(this,coo)
      intensity = intens
      radius = rad
      _color = col

      LightHandle = AddLight(this, style, flags | LFL_POINT | LFL_DYNAMIC, intensity, radius, _color)
      Switch(false)
   }
//-----------------------------------------------------------------
   void ~light_effect()
   {
   	if(LightHandle)
      	RemoveLight(LightHandle)
   }
}

//-----------------------------------------------------------------
ExtClass FindClassByLink(string link);

//=================================================================================
class dlight_dynamic extends LightClass
{
	MinClass target
	int flags, lfl
	string texture
	
//-----------------------------------------------------------------
	void EOnFrame(ExtClass other, int extra)
	{
		if(target)
		{
      	vector vec = GetOrigin(target)
      	vec = vec - GetOrigin(this)
     		SetAngles(this, Vector2Angles(vec))
		}		
	}
	
//-----------------------------------------------------------------
	void M_SwitchOn(ExtClass other)
	{
		if(LightHandle)
		{
			RemoveLight(LightHandle)
			LightHandle = 0
		}

		Init(0, lfl)	
		
		if(texture)
		{
			SetLightTexture(LightHandle, texture)
		}

		if(target)
  			SetEventMask(this, EV_FRAME)
	}
	
//-----------------------------------------------------------------
	void M_SwitchOff(ExtClass other)
	{
		if(LightHandle)
		{
			RemoveLight(LightHandle)
			LightHandle = 0
		}
		
		ClearEventMask(this, EV_FRAME)
	}

//-----------------------------------------------------------------
	void M_SetColor(ExtClass other)
	{
		string str = other._value
		string tok
		
		if(ParseStringEx(str, tok) != 4)
			return
		
		_color[0] = atof(tok)

		if(ParseStringEx(str, tok) != 1)
			return

		if(ParseStringEx(str, tok) != 4)
			return
		
		_color[1] = atof(tok)

		if(ParseStringEx(str, tok) != 1)
			return
		
		if(ParseStringEx(str, tok) != 4)
			return
		
		_color[2] = atof(tok)

		if(LightHandle)
			SetLightEx(LightHandle,intensity, radius, _color)
	}

//-----------------------------------------------------------------
	void EOnLoaded(ExtClass other, int extra)
	{
		CallMethods(this, OnLoad)
	}
	
//-----------------------------------------------------------------
	void EOnInit(ExtClass other, int extra)
	{
    	target = FindClassByLink(link)

		if(intensity == 0)
			intensity = 32

		_color = _color * (float)255
		
		lfl = LFL_DYNAMIC
		
		if(target)
      	lfl |= LFL_SPOT
      else
      	lfl |= LFL_POINT

		if(flags & 2)
			lfl |= LFL_SHADOWSOURCE
			
		if(flags & 1)
			M_SwitchOn(this)
		
		SetOrigin(this, coords)
	}
	
//-----------------------------------------------------------------
   void dlight_dynamic()
   {
   	SetEventMask(this, EV_INIT|EV_LOADED)   	
   }

//-----------------------------------------------------------------
	void ~dlight_dynamic()
	{
		if(LightHandle)
		{
			RemoveLight(LightHandle)
			LightHandle = 0
		}
	}	
}

void Matrix(vector pos, vector mat[4])
{
	mat[0] = "1 0 0"
	mat[1] = "0 1 0"
	mat[2] = "0 0 1"
	mat[3] = pos
}

//=================================================================================
class light extends LightClass
{
	float angle
	int	flags
	int	specular
	int	_sun
	
//-----------------------------------------------------------------
	void Update()
	{
		if(LightHandle)
			RemoveLight(LightHandle)

		SetOrigin(this, coords)

		int fl = LFL_POINT
		
		if(specular != 0)
		{
			fl |= LFL_SPECULAR
			//AddDShape(SHAPE_BBOX, 0x7f7f7f7f, 0, coords - "2 2 2", coords + "2 2 2")
			radius = specular
		}

		LightHandle = AddLight(this, style, fl, intensity, radius, _color)
	}
	
//-----------------------------------------------------------------
#ifdef EDITOR
Shape shape

	void DestroyCustomVisuals()
	{
		if(shape)
		{
			shape.Destroy()
			shape = NULL
		}		
	}
	
	void CreateCustomVisuals()
	{
		if(!shape)
			shape = AddDSphere(VectortoRGBA(_color, 0.15) , SS_NOZUPDATE|SS_TRANSP|SS_NOOUTLINE, coords, radius * 0.5)

		vector mat[4]
		Matrix(coords, mat)
		shape.SetMatrix(mat)
	}
	
	void UpdateCustomVisuals()
	{
		if(shape)
		{
			vector mat[4]
			Matrix(GetMatrix(this, 3), mat)
			shape.SetMatrix(mat)
		}
		Update()
	}	
#endif

//-----------------------------------------------------------------
   void ~light()
   {
		if(LightHandle)
			RemoveLight(LightHandle)
   }
   
//-----------------------------------------------------------------
	void EOnInit(MinClass other, int extra)
	{
		float f = _color * _color
		
		if( f == 0)
			_color = "1.0 1.0 1.0"

		if(intensity == 0)
			intensity = 304

		if(radius == 0)
			radius = intensity

		Update()
	}

//-----------------------------------------------------------------
   void light()
   {
		SetEventMask(this, EV_INIT)
   }
}

//=================================================================================
class light_flashlamp extends LightClass
{
//-----------------------------------------------------------------
   void Switch(int onoff)
   {
   	if(onoff == true)
         SetLightEx(LightHandle,intensity, radius, _color)
   	else
         SetLightEx(LightHandle, 0, 0, ZeroVec)
   }

//-----------------------------------------------------------------
   void light_flashlamp()
   {
      _color = "255 240 200"

      intensity = 35
      radius = 250

      Init(0, LFL_SPOT | LFL_DYNAMIC | LFL_SHADOWSOURCE)
		Switch(false)
      ClearEventMask(this, EV_ALL)
   }
}
