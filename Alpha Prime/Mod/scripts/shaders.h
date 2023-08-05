const float g_RefractGlassMat0[4] = {0.02, 0.0, 0.0, 0.0}
const float g_RefractGlassMat1[4] = {0.0, 0.02, 0.0, 0.0}

float g_fTime,g_fTime01,g_fTime001,g_fTime10,g_fTime100
float g_fTimeR,g_fTime01R,g_fTime001R,g_fTime10R,g_fTime100R
float g_fRotate,g_fRRotate
float g_fScaleSin,g_fScaleSin100
float g_fHSRotate, g_fHMRotate, g_fHHRotate
float g_ShaderShift
float	g_lightningU, g_lightningV
int	g_iAngle
float g_fNoiseU, g_fNoiseV


int g_iColorElevatorBig01_L1
int g_iColorElevatorBig01_L2


float g1 = 0.1
float g2 = 0.05

//=================================================================
float tab(float table[], float t)
{
	int s = sizeof(table)
	int i = floor(t)

	t = t - i
	i = i % s

	return 1 - t * table[i] + (t * table[i + 1 % s])
}

//=================================================================
float wave[2] = {155, 255}
float DronFlare[2] = {0, 20}
float LiftL1[12] = {255, 20, 20, 20, 20, 20, 50, 20, 20, 20, 20, 20}
float LiftL2[12] = { 20, 20, 20,255, 20, 20, 20, 20, 20, 50, 20, 20}


//=================================================================
void UpdateShaders()
{
	
	float t
	int jbc
	
	g_fTime100 = g_fTime * 100
	g_fTime10 = g_fTime * 10
	g_fTime01 = g_fTime * 0.1
	g_fTime001 = g_fTime * 0.01
	g_fTimeR = 0 - g_fTime
	g_fTime100R = 0 - g_fTime100
	g_fTime10R = 0 - g_fTime10
	g_fTime01R = 0 - g_fTime01
	g_fTime001R = 0 - g_fTime001

// -- Elevator big 01 ------	
	jbc = tab(LiftL1, g_fTime10)
	g_iColorElevatorBig01_L1 = ARGBF(255, jbc, jbc, jbc)
	jbc = tab(LiftL2, g_fTime10)
	g_iColorElevatorBig01_L2 = ARGBF(255, jbc, jbc, jbc)


// lightning animtex 8 x 4 pole
/*
	lightningspeed = g_fTime100 * 25
	g_lightningU = lightningspeed * 	0.125
	g_lightningU = floor( g_lightningU * 0.01) * 0.25
	g_lightningV = floor( lightningspeed * 0.01) * 0.125
*/
	g_lightningU = floor( g_fTime100 * 0.03125 ) * 0.25
	g_lightningV = floor( g_fTime100 * 0.25 ) * 0.125

 
// -- OTHERS ------	
	vector vec = Vector(-sin(g_fTime * 4), cos(g_fTime * 4), 1)
	
	VectorNormalize(vec)
	vec = vec * (float)0.7
	g_iAngle	= VectortoRGBA(vec, 0.5)
	
	g_ShaderShift = g_ShaderShift + ftime
	
	t = g_fTime * 2.0
	g_fScaleSin = sin(t) * 0.5 + 1

	g_fScaleSin100 = sin(t) * 100
	
	t = ftime * 0.5
	g_fRotate = g_fRotate + t

   t = ftime * 0.1047
   g_fHSRotate = g_fHSRotate - t
   t = ftime * 0.001745
   g_fHMRotate = g_fHMRotate - t
   t = ftime * 0.0002908
   g_fHHRotate = g_fHHRotate - t
	
	g_fRRotate = 0 - g_fRotate * 2
//	if(g_fRotate > M_PI2)
//		g_fRotate = g_fRotate - M_PI2

	g_fTime += ftime	
	
	g_fNoiseU = frand(-1, 1)
	g_fNoiseV = frand(-1, 1)

}