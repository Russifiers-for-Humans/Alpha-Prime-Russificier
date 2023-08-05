/*
ST_RELOADING //prebijeni zbrane. Vykriky kryjte me apod.
ST_CHANGINGWP //zmena strelecke pozice, protoze stavajici stoji za hovno.
ST_TAKECOVER //dostal kour a klidi se na krytou pozici.
ST_ALERT //byl v klidu a spatril nepritele.
ST_ALERTBYATTACK //byl v klidu a uvidel jak do jeho kamose strili nepritel
ST_ALERTBYSOUND //byl v klidu a zaslechl podezrely zvuk, takze se jde podivat, co se deje. Obvykle
  //vzapeti nasleduje faze 4. protoze treba zaslechne kroky a otoci se, cimz spatri nepritele.
ST_ALERTBYDEAD //byl v klidu a nasel mrtvolu. Jde se podivat o co jde.
ST_ALERTBYSHOT // byl v klidu a dostal zasah.
ST_ALERTBYFRIENDISSHOT // byl v klidu a videl kamose, jak dostal zasah.
ST_SQUADATTACKED //jednotka byla napadena
ST_TEAMMATEKILLED //nekdo z jednotky byl zabit
ST_SQUADLASTMAN //zbyva posledni clen jednotky
ST_GRENADE //vykrik pri spatreni/zaslechnuti granatu
*/
//-----------------------------------------------------------------
void WorldClass::InitSpeechSet()
{
	BeginSpeechSet("Trooper")
		SpeechType(ST_RELOADING)
			Speech("LTrooper_coverme")
			//Speech("LTrooper_reloading")
			//Speech("LTrooper_giveme")
		SpeechType(ST_CHANGINGWP)
			Speech("LTrooper_ST_CHANGINGWP")
			//Speech("LTrooper_common")
			//Speech("LTrooper_move")
		SpeechType(ST_TAKECOVER)
			Speech("LTrooper_ST_TAKECOVER")
			Speech("omen")
			Speech("LTrooper_listen")
		SpeechType(ST_ALERT)
			//Speech("LTrooper_ST_ALERT")
			Speech("LTrooper_righthere")
			Speech("LTrooper_whoisthat")
		SpeechType(ST_ALERTBYATTACK)
			Speech("LTrooper_ST_ALERTBYATTACK")
			Speech("LTrooper_watchback")
			Speech("LTrooper_turnaround")
		SpeechType(ST_ALERTBYSOUND)
			Speech("LTrooper_righthere")
			Speech("LTrooper_whoisthat")
		SpeechType(ST_ALERTBYDEAD)
				Speech("LTrooper_letsgo")
		SpeechType(ST_ALERTBYSHOT)
			Speech("LTrooper_ST_ALERTBYSHOT")
			Speech("LTrooper_shit")
			Speech("LTrooper_common2")
		SpeechType(ST_ALERTBYFRIENDISSHOT)
			Speech("LTrooper_ST_ALERTBYFRIENDISSHOT")
			Speech("LTrooper_niceshot")
		/*SpeechType(ST_RANDOM)
		Speech("LTrooper_mumraj")
		*/
		SpeechType(ST_SQUADATTACKED)
			Speech("LTrooper_ST_SQUADATTACKED")
			Speech("LTrooper_attacked")
		SpeechType(ST_TEAMMATEKILLED)
			Speech("LTrooper_ST_TEAMMATEKILLED")
		SpeechType(ST_SQUADLASTMAN)
			Speech("LTrooper_ST_SQUADLASTMAN")
		SpeechType(ST_GRENADE)
			Speech("LTrooper_ST_GRENADE")
}
