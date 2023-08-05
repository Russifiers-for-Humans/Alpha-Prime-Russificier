BoneMask BoneMasks[32]

int CurrentGroup

const int MAX_ANIMSLOTS = 512
const int MAX_ANIMSETS = 192

//-----------------------------------------------------------------
void Bone(int bone)
{
	BoneMask mask = BoneMasks[CurrentGroup]
	
	int m = mask.Mask[bone >> 5]
	mask.Mask[bone >> 5] = 1 << (bone & 31) | m
}

void SetAllBones()
{
	BoneMask mask = BoneMasks[CurrentGroup]
	
	mask.Mask[0] = 0xffffffff
	mask.Mask[1] = 0xffffffff
	mask.Mask[2] = 0xffffffff
}

//-----------------------------------------------------------------
void BeginGroup(int group)
{
	CurrentGroup = group
	BoneMasks[group] = new BoneMask
}

class AnimSet extends _serializable
{
	int 		Counter
	string	Name

	string	AnimNames[MAX_ANIMSLOTS]
	vobject	Anims[MAX_ANIMSLOTS]
	int		FPS[MAX_ANIMSLOTS]
	vector	Motion[MAX_ANIMSLOTS]
	int		Flags[MAX_ANIMSLOTS]

//-----------------------------------------------------------------
	void AnimSet(string name)
	{
		Name = name
	}
	
//-----------------------------------------------------------------
	void Load()
	{
		if(!Counter)
		{
			for(int n = 0; n < MAX_ANIMSLOTS; n++)
			{
				string name = AnimNames[n]
				if(name)
					Anims[n] = GetObject(name)
			}
		}
		Counter++
	}

//-----------------------------------------------------------------
	void Unload()
	{
		int cnt = Counter - 1
		if(cnt == 0)
		{
			for(int n = 0; n < MAX_ANIMSLOTS; n++)
			{
				if(Anims[n])
					ReleaseObject(Anims[n], 0)
			}
		}
		Counter = cnt
	}
}


AnimSet	CurrentAnimSet
int CurrentAnimFlags, CurrentAnimID
