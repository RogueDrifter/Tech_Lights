/* Made by RogueDrifter 2018 July 10th.
Updated: 2019 16th of June.
Info:
Lights that brighten up when players are around them close.
________________________________
Functions: 
CreateLights(LightType, LightID, Float:PosX, Float:PosY, Float:PosZ, Float:PosRX, Float:PosRY, Float:PosRZ, Float:DrawDistance = 0.0);
LightType:
0- Normal white lights
1- Colored lights red
2- Colored lights blue
LightID:
- Never goes below 0 or above MAX_RTLIGHTS

DestroyLights(LightID, LightType);
DestroyAllLights(LightType);
HideLights(LightID, LightType);
ShowLights(LightID, LightType);
BlinkLights(LightID, LightType, bool:Toggle);
KeepLightsOn(LightID, LightType, bool:Toggle);
KeepAllLightsOn(LightType, bool:Toggle);
ShowAllLights(LightType);
HideAllLights(LightType);
GetLightObjectID(LightID, LightType)

BlinkToDestroyLights(LightID, LightType, Interval);
-Usage: This will blink lights for an interval and then destroy them after the interval is over.

PointLights(LightID, LightType, Direction);
DanceLights(LightID, LightType, bool:toggle);

IsLightKeptOn(LightID, LightType);
IsLightBlinking(LightID, LightType);
IsLightOn(LightID, LightType);
IsPlayerTriggeringLight(playerid, LightID, LightType);
IsLightValid(LightID, LightType);

ToggleLightForPlayer(playerid, LightID, LightType, bool:Toggle);
-Usage: Toggle 0 to turn the player's ability to light up lights off and 1 to go back to normal. 

MoveLights(LightID, LightType, Float:ToX, Float:ToY, Float:ToZ, Float:ToRX, Float:ToRY, Float:ToRZ);
-Usage: Will change the current position/rotation of a light.

Callbacks:
__________
OnPlayerTriggerLight(playerid, LightID, LightType);
OnLightGoOff(LightID, LightType);
________________________________________________________________________________________
Definitions:(Use before including the file)
#define MAX_RTLIGHTS(NUMBER)
-Usage: Used to define the maximum amount of lights you are going to use which is 100 by default.
#define RTL_MAX_PLAYER_RANGE(RANGE)
-Usage: Used to define the range of which the player needs to be within in order for the lights to trigger.
__________________________________
To be added:
____________
Intensity parameter to the CreateLights function(controls how bright the lights would be).
Size parameter to the CreateLights function(controls the size of the lights).
Another creating function editing the position from in-game. */

#if defined RTLIGHTS_included_
	#endinput
#endif

#define RTLIGHTS_included_

#include <a_samp>

#if !defined MAX_RTLIGHTS
	#define MAX_RTLIGHTS 100
#endif

#if !defined RTL_MAX_PLAYER_RANGE
	#define RTL_MAX_PLAYER_RANGE 12.5
#endif

#if !defined RT_MAX_LIGHTS_CHECK
	#define RT_MAX_LIGHTS_CHECK 500
#endif

#define LIGHT_TYPE_WHITE 0
#define LIGHT_TYPE_RED 1
#define LIGHT_TYPE_BLUE 2

#define RTL_INVALID_LIGHTS_POS 9999.9999
#define MAX_RTTYPES 3
#define LIGHTS_DIRECTION_UP		0
#define LIGHTS_DIRECTION_DOWN	1
#define LIGHTS_DIRECTION_LEFT	2
#define LIGHTS_DIRECTION_RIGHT	3
#define RTL_LIGHTS_DANCE_SPEED 0.09

enum RT_LIGHTS_INFO
{
	RT_LightsNumbers,

	bool:RT_LightsVerify,
	bool:RT_IsLightLit,
	bool:RT_BlocKLight,
	bool:RT_IsLightBlinking,
	bool:RT_TempLightBlink,
	bool:RT_KeepLightsOn,
	bool:RT_LightsDancing,
	bool:RT_LightsDancingShift,

	Float:RT_LightsPosX,
	Float:RT_LightsPosY,
	Float:RT_LightsPosZ,
	Float:RT_LightsPosRX,
	Float:RT_LightsPosRY,
	Float:RT_LightsPosRZ,
	Float:RT_LightsDrawDistance
}

static 
	RT_LightsInfo[MAX_RTLIGHTS][MAX_RTTYPES][RT_LIGHTS_INFO],
	bool:RT_LightsToggle[MAX_PLAYERS][MAX_RTLIGHTS][MAX_RTTYPES],
	RT_LightsTimer
	;

forward rt_LightsDetector();
forward RT_BlinkToDestroy(LightID, LightType);

#if defined OnPlayerTriggerLight
	forward OnPlayerTriggerLight(playerid, LightID, LightType);
#endif

#if defined OnLightGoOff
	forward OnLightGoOff(LightID, LightType);
#endif

#if defined RTL_OnPlayerConnect
	forward RTL_OnPlayerConnect(playerid);
#endif

#if defined FILTERSCRIPT

	#if defined RTL_OnFilterScriptExit
		forward RTL_OnFilterScriptExit();
	#endif

	#if defined RTL_OnFilterScriptInit
		forward RTL_OnFilterScriptInit();
	#endif

#else

	#if defined RTL_OnGameModeExit
		forward RTL_OnGameModeExit();
	#endif

	#if defined RTL_OnGameModeInit
		forward RTL_OnGameModeInit();
	#endif

#endif

#if defined FILTERSCRIPT

	public OnFilterScriptInit()
	{
		RT_LightsTimer = SetTimer("rt_LightsDetector", RT_MAX_LIGHTS_CHECK, true);

		#if defined RTL_OnFilterScriptInit
			return RTL_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}

	public OnFilterScriptExit()
	{
		KillTimer(RT_LightsTimer);

		#if defined RTL_OnFilterScriptExit
			return RTL_OnFilterScriptExit();
		#else
			return 1;
		#endif
	}

#else

	public OnGameModeInit()
	{
		RT_LightsTimer = SetTimer("rt_LightsDetector", RT_MAX_LIGHTS_CHECK, true);

		#if defined RTL_OnGameModeInit
			return RTL_OnGameModeInit();
		#else
			return 1;
		#endif
	}

	public OnGameModeExit()
	{
		KillTimer(RT_LightsTimer);

		#if defined RTL_OnGameModeExit
			return RTL_OnGameModeExit();
		#else
			return 1;
		#endif
	}

#endif

public OnPlayerConnect(playerid)
{
	for(new i; i < MAX_RTLIGHTS; i++)
	{	
		for(new x; x < MAX_RTTYPES; x++)
		{	
			RT_LightsToggle[playerid][i][x] = false;
		}
	}

	#if defined RTL_OnPlayerConnect
		return RTL_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public rt_LightsDetector()
{
	for(new i; i < MAX_RTLIGHTS; i++)
	{
		for(new x; x < MAX_RTTYPES; x++)
		{
			if(!RT_LightsInfo[i][x][RT_LightsVerify]) continue;

			if(RT_LightsInfo[i][x][RT_LightsDancing])
			{
				if(!IsObjectMoving(RT_LightsInfo[i][x][RT_LightsNumbers]))
				{
					if(RT_LightsInfo[i][x][RT_LightsDancingShift]) 
					{
						MoveDynamicObject(RT_LightsInfo[i][x][RT_LightsNumbers], RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ]+0.05, RTL_LIGHTS_DANCE_SPEED, RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], 91.00000);
						RT_LightsInfo[i][x][RT_LightsDancingShift] = false;
					}
					else if(!RT_LightsInfo[i][x][RT_LightsDancingShift])
					{
						MoveDynamicObject(RT_LightsInfo[i][x][RT_LightsNumbers], RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ]-0.05, RTL_LIGHTS_DANCE_SPEED, RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], -91.00000);
						RT_LightsInfo[i][x][RT_LightsDancingShift] = true;
					}
				}
			}

			if(RT_LightsInfo[i][x][RT_KeepLightsOn])
			{
				if(RT_LightsInfo[i][x][RT_IsLightBlinking])
				{   
					switch(RT_LightsInfo[i][x][RT_TempLightBlink])
					{
						case 0:
						{
							RT_LightsInfo[i][x][RT_TempLightBlink] = true;
							HideLights(i, x);
						}

						case 1:
						{
							RT_LightsInfo[i][x][RT_TempLightBlink] = false;
							ShowLights(i, x);
						}
					}
				}

				if(!RT_LightsInfo[i][x][RT_IsLightLit] && !RT_LightsInfo[i][x][RT_BlocKLight])
				{
					switch(x)
					{
						case 0:
						{	
							RT_LightsInfo[i][x][RT_LightsNumbers] = CreateDynamicObject(18656, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ], RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], RT_LightsInfo[i][x][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][x][RT_LightsDrawDistance]);
						}

						case 1:
						{
							RT_LightsInfo[i][x][RT_LightsNumbers] = CreateDynamicObject(18657, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ], RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], RT_LightsInfo[i][x][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][x][RT_LightsDrawDistance]);
						}

						case 2:
						{
							RT_LightsInfo[i][x][RT_LightsNumbers] = CreateDynamicObject(18658, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ], RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], RT_LightsInfo[i][x][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][x][RT_LightsDrawDistance]);
						}
					}

					RT_LightsInfo[i][x][RT_IsLightLit] = true;
				}
			}

			else
			{
				if(RTL_IsAnybodyNearLights(i, x, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ]))
				{
					if(RT_LightsInfo[i][x][RT_IsLightBlinking])
					{   
						switch(RT_LightsInfo[i][x][RT_TempLightBlink])
						{
							case 0:
							{
								RT_LightsInfo[i][x][RT_TempLightBlink] = true;
								HideLights(i, x);
							}

							case 1:
							{
								RT_LightsInfo[i][x][RT_TempLightBlink] = false;
								ShowLights(i, x);
							}
						}
					}

					if(!RT_LightsInfo[i][x][RT_IsLightLit] && !RT_LightsInfo[i][x][RT_BlocKLight])
					{
						switch(x)
						{
							case 0:
							{
								RT_LightsInfo[i][x][RT_LightsNumbers] = CreateDynamicObject(18656, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ], RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], RT_LightsInfo[i][x][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][x][RT_LightsDrawDistance]);
							}

							case 1:
							{
								RT_LightsInfo[i][x][RT_LightsNumbers] = CreateDynamicObject(18657, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ], RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], RT_LightsInfo[i][x][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][x][RT_LightsDrawDistance]);
							}

							case 2:
							{
								RT_LightsInfo[i][x][RT_LightsNumbers] = CreateDynamicObject(18658, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ], RT_LightsInfo[i][x][RT_LightsPosRX], RT_LightsInfo[i][x][RT_LightsPosRY], RT_LightsInfo[i][x][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][x][RT_LightsDrawDistance]);
							}
						}
						RT_LightsInfo[i][x][RT_IsLightLit] = true;
					}
				}
			}
			
			if(!RTL_IsAnybodyNearLights(i, x, RT_LightsInfo[i][x][RT_LightsPosX], RT_LightsInfo[i][x][RT_LightsPosY], RT_LightsInfo[i][x][RT_LightsPosZ]))
			{
				if(RT_LightsInfo[i][x][RT_IsLightLit] && !RT_LightsInfo[i][x][RT_BlocKLight] && !RT_LightsInfo[i][x][RT_KeepLightsOn])
				{

					DestroyDynamicObject(RT_LightsInfo[i][x][RT_LightsNumbers]);
					#if defined OnLightGoOff
						OnLightGoOff(i, x);
					#endif

					RT_LightsInfo[i][x][RT_LightsNumbers] = INVALID_OBJECT_ID;
					RT_LightsInfo[i][x][RT_IsLightLit] = false;
				}
			}
		}
	}

	return 1;
}

public RT_BlinkToDestroy(LightID, LightType)
{
	if(!RT_LightsInfo[LightID][LightType][RT_IsLightBlinking]) return 0;
	return DestroyLights(LightID, LightType);
}

static stock PointLightsInternal(LightID, LightType, Direction)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(Direction < 0 || Direction > 3) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;

	switch(Direction)
	{
		case LIGHTS_DIRECTION_UP:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = 91.0000;
			MoveDynamicObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers], RT_LightsInfo[LightID][LightType][RT_LightsPosX], RT_LightsInfo[LightID][LightType][RT_LightsPosY], RT_LightsInfo[LightID][LightType][RT_LightsPosZ]+0.05, RTL_LIGHTS_DANCE_SPEED, RT_LightsInfo[LightID][LightType][RT_LightsPosRX], RT_LightsInfo[LightID][LightType][RT_LightsPosRY], RT_LightsInfo[LightID][LightType][RT_LightsPosRZ]);
		}
		case LIGHTS_DIRECTION_DOWN:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = -91.0000;
			MoveDynamicObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers], RT_LightsInfo[LightID][LightType][RT_LightsPosX], RT_LightsInfo[LightID][LightType][RT_LightsPosY], RT_LightsInfo[LightID][LightType][RT_LightsPosZ]-0.05, RTL_LIGHTS_DANCE_SPEED, RT_LightsInfo[LightID][LightType][RT_LightsPosRX], RT_LightsInfo[LightID][LightType][RT_LightsPosRY], RT_LightsInfo[LightID][LightType][RT_LightsPosRZ]);
		}
		case LIGHTS_DIRECTION_LEFT:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = 91.0000;
			MoveDynamicObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers], RT_LightsInfo[LightID][LightType][RT_LightsPosX], RT_LightsInfo[LightID][LightType][RT_LightsPosY], RT_LightsInfo[LightID][LightType][RT_LightsPosZ]+0.05, RTL_LIGHTS_DANCE_SPEED, RT_LightsInfo[LightID][LightType][RT_LightsPosRX], RT_LightsInfo[LightID][LightType][RT_LightsPosRY], RT_LightsInfo[LightID][LightType][RT_LightsPosRZ]);
		}
		case LIGHTS_DIRECTION_RIGHT:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = -91.0000;
			MoveDynamicObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers], RT_LightsInfo[LightID][LightType][RT_LightsPosX], RT_LightsInfo[LightID][LightType][RT_LightsPosY], RT_LightsInfo[LightID][LightType][RT_LightsPosZ]-0.05, RTL_LIGHTS_DANCE_SPEED, RT_LightsInfo[LightID][LightType][RT_LightsPosRX], RT_LightsInfo[LightID][LightType][RT_LightsPosRY], RT_LightsInfo[LightID][LightType][RT_LightsPosRZ]);
		}
	}
	return 1;
}

static RTL_IsAnybodyNearLights(LightID, LightType, Float:LightPosX, Float:LightPosY, Float:LightPosZ)
{
	#if defined foreach 

		foreach(new i: Player)
		{
			if(IsPlayerInRangeOfPoint(i, RTL_MAX_PLAYER_RANGE, LightPosX, LightPosY, LightPosZ) 
			&& GetPlayerState(i) != PLAYER_STATE_SPECTATING && !RT_LightsToggle[i][LightID][LightType])
			{
				#if defined OnPlayerTriggerLight
					if(!RT_LightsInfo[LightID][LightType][RT_IsLightLit]) OnPlayerTriggerLight(i, LightID, LightType);
				#endif
				return 1;
			}
		}

	#else

		for(new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(!IsPlayerConnected(i)) continue;

			if(IsPlayerInRangeOfPoint(i, RTL_MAX_PLAYER_RANGE, LightPosX, LightPosY, LightPosZ)
			&& GetPlayerState(i) != PLAYER_STATE_SPECTATING && !RT_LightsToggle[i][LightID][LightType]) 
			{
				#if defined OnPlayerTriggerLight
					if(!RT_LightsInfo[LightID][LightType][RT_IsLightLit]) OnPlayerTriggerLight(i, LightID, LightType);
				#endif
				return 1;
			}
		}

	#endif
	return 0;
}

stock CreateLights(LightType, LightID, Float:PosX, Float:PosY, Float:PosZ, Float:PosRX, Float:PosRY, Float:PosRZ, Float:DrawDistance = RTL_MAX_PLAYER_RANGE)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] != false) return 0;

	switch(LightType)
	{
		case 0..2:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosX] = PosX;
			RT_LightsInfo[LightID][LightType][RT_LightsPosY] = PosY;
			RT_LightsInfo[LightID][LightType][RT_LightsPosZ] = PosZ;
			RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = PosRX;
			RT_LightsInfo[LightID][LightType][RT_LightsPosRY] = PosRY;
			RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = PosRZ;
			RT_LightsInfo[LightID][LightType][RT_LightsDrawDistance] = DrawDistance;

			RT_LightsInfo[LightID][LightType][RT_LightsVerify] = true;
		}
	}
	return 1;
}

stock DestroyAllLights(LightType)
{
	if( 0 > LightType || LightType >= MAX_RTTYPES) return 0;

	for(new x; x < MAX_RTLIGHTS; x++)
	{
		if(!RT_LightsInfo[x][LightType][RT_LightsVerify]) continue;

		#if defined foreach 
			foreach(new i: Player)
			{
				RT_LightsToggle[i][x][LightType] = false;
			}
		#else
			for(new i, j = GetPlayerPoolSize(); i <= j; i++)
			{
				RT_LightsToggle[i][x][LightType] = false;
			}
		#endif
		DestroyDynamicObject(RT_LightsInfo[x][LightType][RT_LightsNumbers]);
		RT_LightsInfo[x][LightType][RT_KeepLightsOn] = false;
		RT_LightsInfo[x][LightType][RT_IsLightBlinking] = false;
		RT_LightsInfo[x][LightType][RT_TempLightBlink]  = false;
		RT_LightsInfo[x][LightType][RT_LightsPosX] = 
		RT_LightsInfo[x][LightType][RT_LightsPosY] =
		RT_LightsInfo[x][LightType][RT_LightsPosZ] = 
		RT_LightsInfo[x][LightType][RT_LightsPosRX] = 
		RT_LightsInfo[x][LightType][RT_LightsPosRY] = 
		RT_LightsInfo[x][LightType][RT_LightsPosRZ] = 
		RT_LightsInfo[x][LightType][RT_LightsDrawDistance] = RTL_INVALID_LIGHTS_POS;

		RT_LightsInfo[x][LightType][RT_LightsNumbers] = INVALID_OBJECT_ID;
		RT_LightsInfo[x][LightType][RT_LightsVerify] = false;
	}
	return 1;
}

stock DestroyLights(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(!RT_LightsInfo[LightID][LightType][RT_LightsVerify]) return 0;

	#if defined foreach 
		foreach(new i: Player)
		{
			RT_LightsToggle[i][LightID][LightType] = false;
		}
	#else
		for(new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
			RT_LightsToggle[i][LightID][LightType] = false;
		}
	#endif

	DestroyDynamicObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
	RT_LightsInfo[LightID][LightType][RT_KeepLightsOn] = false;
	RT_LightsInfo[LightID][LightType][RT_IsLightBlinking] = false;
	RT_LightsInfo[LightID][LightType][RT_TempLightBlink]  = false;
	RT_LightsInfo[LightID][LightType][RT_LightsPosX] = 
	RT_LightsInfo[LightID][LightType][RT_LightsPosY] = 
	RT_LightsInfo[LightID][LightType][RT_LightsPosZ] = 
	RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = 
	RT_LightsInfo[LightID][LightType][RT_LightsPosRY] = 
	RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = 
	RT_LightsInfo[LightID][LightType][RT_LightsDrawDistance] = RTL_INVALID_LIGHTS_POS;

	RT_LightsInfo[LightID][LightType][RT_LightsNumbers] = INVALID_OBJECT_ID;
	RT_LightsInfo[LightID][LightType][RT_LightsVerify] = false;
	return 1;
}

stock IsLightOn(LightID, LightType) 
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	return RT_LightsInfo[LightID][LightType][RT_IsLightLit];
} 

stock IsLightBlinking(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	return RT_LightsInfo[LightID][LightType][RT_IsLightBlinking];
}

stock IsLightKeptOn(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	return RT_LightsInfo[LightID][LightType][RT_KeepLightsOn];
}

stock IsLightValid(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	return RT_LightsInfo[LightID][LightType][RT_LightsVerify];
}

stock KeepLightsOn(LightID, LightType, bool:Toggle)
{
	if(Toggle != true && Toggle != false) return 0;
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	return RT_LightsInfo[LightID][LightType][RT_KeepLightsOn] = Toggle;
}

stock BlinkToDestroyLights(LightID, LightType, Interval)
{
	if(Interval < 0) return 0;
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_IsLightBlinking]) return 0;

	RT_LightsInfo[LightID][LightType][RT_IsLightBlinking] = true;
	SetTimerEx("RT_BlinkToDestroy", Interval, false, "ii", LightID, LightType);
	return 1;
}

stock KeepAllLightsOn(LightType, bool:Toggle)
{
	if(Toggle != true && Toggle != false) return 0;
	if(0 > LightType || LightType >= MAX_RTTYPES) return 0;

	for(new i; i < MAX_RTLIGHTS; i++)
	{
		if(RT_LightsInfo[i][LightType][RT_LightsVerify] == false) continue;
		RT_LightsInfo[i][LightType][RT_KeepLightsOn] = Toggle;
	}
	return 1;
}

stock BlinkLights(LightID, LightType, bool:Toggle)
{
	if(Toggle != true && Toggle != false) return 0;
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	switch(Toggle)
	{
		case false:
		{
			RT_LightsInfo[LightID][LightType][RT_IsLightBlinking] = false;
			RT_LightsInfo[LightID][LightType][RT_TempLightBlink]  = false;
			RT_LightsInfo[LightID][LightType][RT_BlocKLight]	  = false;
		}

		case true:
		{
			RT_LightsInfo[LightID][LightType][RT_IsLightBlinking] = true;
		}
	} 
	return 1;
}

stock HideLights(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	DestroyDynamicObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
	RT_LightsInfo[LightID][LightType][RT_LightsNumbers] = INVALID_OBJECT_ID;
	RT_LightsInfo[LightID][LightType][RT_IsLightLit] = false;
	return RT_LightsInfo[LightID][LightType][RT_BlocKLight] = true;
}

stock ShowLights(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	return RT_LightsInfo[LightID][LightType][RT_BlocKLight] = false;
}

stock ShowAllLights(LightType)
{
	if(0 > LightType || LightType >= MAX_RTTYPES) return 0;

	for(new i; i < MAX_RTLIGHTS; i++)
	{
		if(RT_LightsInfo[i][LightType][RT_LightsVerify] == false) continue;
		RT_LightsInfo[i][LightType][RT_BlocKLight] = false;
	}
	return 1;
}

stock HideAllLights(LightType)
{
	if(0 > LightType || LightType >= MAX_RTTYPES) return 0;

	for(new i; i < MAX_RTLIGHTS; i++)
	{
		if(RT_LightsInfo[i][LightType][RT_LightsVerify] == false) continue;
		RT_LightsInfo[i][LightType][RT_BlocKLight] = true;
	}
	return 1;
}

stock IsPlayerTriggeringLight(playerid, LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES || playerid > MAX_PLAYERS || playerid < 0) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false || RT_LightsToggle[playerid][LightID][LightType]) return 0;
	return(IsPlayerInRangeOfPoint(playerid, RTL_MAX_PLAYER_RANGE, RT_LightsInfo[LightID][LightType][RT_LightsPosX], RT_LightsInfo[LightID][LightType][RT_LightsPosY], RT_LightsInfo[LightID][LightType][RT_LightsPosZ]) && RT_LightsInfo[LightID][LightType][RT_IsLightLit]);
}

stock ToggleLightForPlayer(playerid, LightID, LightType, bool:Toggle)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES || playerid > MAX_PLAYERS || playerid < 0) return 0;
	if(Toggle != true && Toggle != false) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;

	return RT_LightsToggle[playerid][LightID][LightType] = !Toggle;
}

stock MoveLights(LightID, LightType, Float:ToX, Float:ToY, Float:ToZ, Float:ToRX, Float:ToRY, Float:ToRZ)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;

	RT_LightsInfo[LightID][LightType][RT_LightsPosX] = ToX;
	RT_LightsInfo[LightID][LightType][RT_LightsPosY] = ToY;
	RT_LightsInfo[LightID][LightType][RT_LightsPosZ] = ToZ;
	RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = ToRX;
	RT_LightsInfo[LightID][LightType][RT_LightsPosRY] = ToRY;
	RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = ToRZ;
	return 1;
}

stock PointLights(LightID, LightType, Direction)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(Direction < 0 || Direction > 3) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;

	switch(Direction)
	{
		case LIGHTS_DIRECTION_UP:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = 91.0000;
		}
		case LIGHTS_DIRECTION_DOWN:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = -91.0000;
		}
		case LIGHTS_DIRECTION_LEFT:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = 91.0000;
		}
		case LIGHTS_DIRECTION_RIGHT:
		{
			RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = -91.0000;
		}
	}
	return 1;
}

stock DanceLights(LightID, LightType, bool:toggle)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return 0;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
	if(toggle != true && toggle != false) return 0;

	switch(toggle)
	{
		case true:
		{
			if(RT_LightsInfo[LightID][LightType][RT_LightsDancing]) return 0;
			RT_LightsInfo[LightID][LightType][RT_LightsDancing] = true;
			PointLightsInternal(LightID, LightType, LIGHTS_DIRECTION_RIGHT);
			RT_LightsInfo[LightID][LightType][RT_LightsDancingShift] = true;
		}

		case false:
		{
			if(!RT_LightsInfo[LightID][LightType][RT_LightsDancing]) return 0;
			StopObject(RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
			RT_LightsInfo[LightID][LightType][RT_LightsDancing] = false;
		}
	}
	return 1;
}

stock GetLightObjectID(LightID, LightType)
{
	if(0 > LightID || LightID >= MAX_RTLIGHTS || 0 > LightType || LightType >= MAX_RTTYPES) return INVALID_OBJECT_ID;
	if(RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return INVALID_OBJECT_ID;

	return RT_LightsInfo[LightID][LightType][RT_LightsNumbers];
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect RTL_OnPlayerConnect

#if defined FILTERSCRIPT

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit RTL_OnFilterScriptInit

#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif

#define OnFilterScriptExit RTL_OnFilterScriptExit

#else

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit RTL_OnGameModeInit

#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif

#define OnGameModeExit RTL_OnGameModeExit

#endif
