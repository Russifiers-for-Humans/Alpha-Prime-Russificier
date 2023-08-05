class music_player extends MinClass
{
	hsound ambient[6]
	int	 SoundNum
	int 	 NowPlaing
	int	 NextToPlay
	float  FadeInSpeeds[6]
	float  FadeOutSpeeds[6]
	float  SwitchWait
	float  volume
	float  fspeed
	float  MasterVolumes[6]
	string SndPaths[6]
	float  SndVolumes[6]

	void SetMasterVolumes()
	{
		MasterVolumes[BS_AMBIENT] = SndVolumes[BS_AMBIENT] * 0.01
		MasterVolumes[BS_ATMOSPHERE] = SndVolumes[BS_ATMOSPHERE] * 0.01
		MasterVolumes[BS_FIGHT] = SndVolumes[BS_FIGHT] * 0.01
		MasterVolumes[BS_FIGHT2] = SndVolumes[BS_FIGHT2] * 0.01
		MasterVolumes[BS_DIALOG] = SndVolumes[BS_DIALOG] * 0.01
		MasterVolumes[BS_DIALOG2] = SndVolumes[BS_DIALOG2] * 0.01
	}

	void PlayNextSample()
	{
		if(ambient[NextToPlay])
		{
			if(NowPlaing != -1)
				EndSound(SoundNum)
				
			NowPlaing = NextToPlay
			SetMasterVolumes()
			SoundNum = PlaySound(this, ambient[NowPlaing], SF_AMBIENT|SF_MUSIC)
			SetSoundVolume(SoundNum, 0)
		}
	}

	void SetFadeTempo()
	{
		if(NextToPlay == -2)
		{
			if(NowPlaing >= 0)
				fspeed = FadeOutSpeeds[NowPlaing]
			else
				fspeed = 0.5
			return
		}

		if(NowPlaing == -1)
			fspeed = FadeOutSpeeds[NextToPlay]
		else
			fspeed = FadeOutSpeeds[NowPlaing]

		if(NextToPlay == BS_FIGHT || NextToPlay == BS_FIGHT2/* || NextToPlay == BS_ATMOSPHERE*/)
			fspeed = 1.5

		if(World.CutScene && NowPlaing != -1)	//pokial nabehne cutscena tak to co hralo predtym musi dohrat rychle
			fspeed *= 3
	}

	void PlayBackMusic(ExtClass other, int sample)
	{
		if(other == World.Player && sample == BS_FIGHT)	//play z playera nepouzijeme
			return

		if(sample == -1)
			sample = BS_AMBIENT

		if(sample >= 0 && !ambient[sample])
		{
//			string Err = "No loaded sound file " + SndPaths[sample]
//			MessageBoxA(0, Err, "Error", 0)
			return
		}

		switch(status)
		{
			case 0:
				if(sample == NowPlaing)
					return
				else
				{
					NextToPlay = sample
					status = 1
					SetFadeTempo()
					return
				}
			break

			case 1:
				if(sample == NextToPlay)
					return
				else
				{
					NextToPlay = sample
					return
				}
			break

			case 2:
				if(sample == NowPlaing)
					return
				else
				{
					NextToPlay = sample
					status = 1
					SetFadeTempo()
					return
				}
			break
		}
	}

	void EOnTick(MinClass other, int extra)
	{
		if(status == 0)
			return

		if(status == 1)	//fade out
		{
			volume -= TICK_PERIOD * fspeed
			if(volume <= 0.2)
			{
				if(NextToPlay == BS_NONE)
				{
					EndSound(SoundNum)
					SoundNum = 0
					NowPlaing = -1
					status = 0
					return
				}

				volume = 0.2
				PlayNextSample()

				if(NowPlaing < 0)
				{
					status = 0
					return
				}

				status = 2
				if(NextToPlay == BS_FIGHT || NextToPlay == BS_FIGHT2  || NextToPlay == BS_ATMOSPHERE)	//vynimka
					fspeed = 0.8
				else
				{
					fspeed = FadeInSpeeds[NextToPlay]

					if(World.CutScene)
						fspeed *= 3
				}
			}

			SetSoundVolume(SoundNum, LinearVol(volume * MasterVolumes[NowPlaing]))	//SetSoundVolume(SoundNum, LinearVol(MasterVolumes[NowPlaing]))
			return
		}

		if(status == 2)	//fade in
		{
			volume += TICK_PERIOD * fspeed
			if(volume >= 1)
			{
				volume = 1
				status = 0
			}

			SetSoundVolume(SoundNum, LinearVol(volume * MasterVolumes[NowPlaing]))
			return
		}
	}

	void InterruptPlaying()
	{
		if(SoundNum)
		{
			EndSound(SoundNum)
			SoundNum = 0
		}
	
		NowPlaing = -1
		status = 0
	}

	void LoadSamples()
	{
		if(SndPaths[BS_AMBIENT])
			ambient[BS_AMBIENT] = GetSoundEx(SndPaths[BS_AMBIENT], 1, SF_MUSIC|SF_STREAM)
			
		if(SndPaths[BS_ATMOSPHERE])
			ambient[BS_ATMOSPHERE] = GetSoundEx(SndPaths[BS_ATMOSPHERE], 1, SF_MUSIC|SF_STREAM)
			
		if(SndPaths[BS_FIGHT])
			ambient[BS_FIGHT] = GetSoundEx(SndPaths[BS_FIGHT], 1, SF_MUSIC|SF_STREAM)
			
		if(SndPaths[BS_FIGHT2])
			ambient[BS_FIGHT2] = GetSoundEx(SndPaths[BS_FIGHT2], 1, SF_MUSIC|SF_STREAM)
			
		if(SndPaths[BS_DIALOG])
			ambient[BS_DIALOG] = GetSoundEx(SndPaths[BS_DIALOG], 1, SF_MUSIC|SF_STREAM)
			
		if(SndPaths[BS_DIALOG2])
			ambient[BS_DIALOG2] = GetSoundEx(SndPaths[BS_DIALOG2], 1, SF_MUSIC|SF_STREAM)
	}

	void EOnLoad(MinClass other, int extra)
	{
		LoadSamples()
		status = 0
		volume = 0.2
		if(NowPlaing >= 0)
		{
			NextToPlay = NowPlaing
			NowPlaing = -1
			PlayBackMusic(this, NextToPlay)
		}
		else
			NowPlaing = -1
	}

	void music_player()
	{
		if(SoundPaths[BS_AMBIENT] == "" || SoundPaths[BS_ATMOSPHERE] == "" || SoundPaths[BS_FIGHT] == "")
		{
//			Print("No loaded backround music")
			ClearEventMask(this, EV_ALL)
			return
		}

		SetEventMask(this, EV_TICK|EV_ONLOAD)
		NowPlaing = -1
		volume = 0.2
		status = 0
		NextToPlay = BS_AMBIENT

		SndPaths[BS_AMBIENT] = SoundPaths[BS_AMBIENT]
		SndPaths[BS_ATMOSPHERE] = SoundPaths[BS_ATMOSPHERE]
		SndPaths[BS_FIGHT] = SoundPaths[BS_FIGHT]
		SndPaths[BS_FIGHT2] = SoundPaths[BS_FIGHT2]
		SndPaths[BS_DIALOG] = SoundPaths[BS_DIALOG]
		SndPaths[BS_DIALOG2] = SoundPaths[BS_DIALOG2]

		SndVolumes[BS_AMBIENT] = SoundVolumes[BS_AMBIENT]
		SndVolumes[BS_ATMOSPHERE] = SoundVolumes[BS_ATMOSPHERE]
		SndVolumes[BS_FIGHT] = SoundVolumes[BS_FIGHT]
		SndVolumes[BS_FIGHT2] = SoundVolumes[BS_FIGHT2]
		SndVolumes[BS_DIALOG] = SoundVolumes[BS_DIALOG]
		SndVolumes[BS_DIALOG2] = SoundVolumes[BS_DIALOG2]

		FadeInSpeeds[BS_AMBIENT] = 0.5
		FadeInSpeeds[BS_ATMOSPHERE] = 0.5
		FadeInSpeeds[BS_FIGHT] = 1
		FadeInSpeeds[BS_FIGHT2] = 0.5
		FadeInSpeeds[BS_DIALOG] = 0.5
		FadeInSpeeds[BS_DIALOG2] = 1

		FadeOutSpeeds[BS_AMBIENT] = 0.2
		FadeOutSpeeds[BS_ATMOSPHERE] = 0.2
		FadeOutSpeeds[BS_FIGHT] = 0.2
		FadeOutSpeeds[BS_FIGHT2] = 0.2
		FadeOutSpeeds[BS_DIALOG] = 0.2
		FadeOutSpeeds[BS_DIALOG2] = 0.2

		SetMasterVolumes()
		LoadSamples()
//		PlayBackMusic(this, NextToPlay)
//		PlayNextSample()
	}
	
	void ~music_player()
	{
		if(SoundNum)
			EndSound(SoundNum)
	}
}
