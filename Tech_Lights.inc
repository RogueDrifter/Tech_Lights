/* 
Made by RogueDrifter 2018 July 10th

Info:
Lights that brighten up when players are around them close, also gives support to streamer users.
________________________________
Functions: 

CreateLights (LightType, LightID, Float:PosX, Float:PosY, Float:PosZ, Float:PosRX, Float:PosRY, Float:PosRZ, Float:DrawDistance = 0.0);

LightType:
0- Normal white lights
1- Colored lights (TBA)
2- Blinking lights (TBA)

LightID:
- Never goes below 0 or above MAX_RTLIGHTS

DestroyLights (LightID, LightType);
DestroyAllLights(LightType);
HideLights(LightID, LightType);
ShowLights(LightID, LightType);
IsLightOn(LightID, LightType);
IsPlayerTriggeringLight(playerid, LightID, LightType);

// TBA:
MoveLights (LightID, Float:ToX, Float:ToY, Float:ToZ, Float:ToRX = -1000.0, Float:ToRY = -1000.0, Float:ToRZ = -1000.0, Float:MovingSpeed = 5.0);
________________________________________________________________________________________
Definitions: (Use before including the file)

#define MAX_RTLIGHTS (NUMBER)
-Usage: Used to define the maximum amount of lights you are going to use which is 100 by default.

#define RTL_MAX_PLAYER_RANGE (RANGE)
-Usage: Used to define the range of which the player needs to be within in order for the lights to trigger.
*/

#if defined RTLIGHTS_included_
    #endinput
#endif

#define RTLIGHTS_included_

#include <a_samp> 

#if !defined MAX_RTLIGHTS
    #define MAX_RTLIGHTS 100
#endif

#if !defined RTL_MAX_PLAYER_RANGE
    #define RTL_MAX_PLAYER_RANGE 10.0
#endif

#define RTL_INVALID_LIGHTS_POS 9999.9999
#define MAX_RTTYPES 1

enum RT_LIGHTS_INFO
{
    RT_LightsNumbers,

    bool:RT_LightsVerify,
    bool:RT_IsLightLit,
    bool:RT_BlocKLight,

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
    RT_LightsTimer
    ;

forward rt_LightsDetector();

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

stock CreateLights (LightType, LightID, Float:PosX, Float:PosY, Float:PosZ, Float:PosRX, Float:PosRY, Float:PosRZ, Float:DrawDistance = 0.0)
{
    if (0 > LightID >= MAX_RTLIGHTS || LightType != 0) return 0;
    if (RT_LightsInfo[LightID][LightType][RT_LightsVerify] != false) return 0;

    switch (LightType)
    {
        case 0:
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

stock DestroyAllLights (LightType)
{
    if (LightType != 0) return 0;

    for (new i; i < MAX_RTLIGHTS; i++)
    {
        if (!RT_LightsInfo[i][LightType][RT_LightsVerify]) continue;

        #if !defined CreateDynamicObject 
            DestroyObject (RT_LightsInfo[i][LightType][RT_LightsNumbers]);
        #else 
            DestroyDynamicObject (RT_LightsInfo[i][LightType][RT_LightsNumbers]);
        #endif

        RT_LightsInfo[i][LightType][RT_LightsPosX] = RTL_INVALID_LIGHTS_POS;
        RT_LightsInfo[i][LightType][RT_LightsPosY] = RTL_INVALID_LIGHTS_POS;
        RT_LightsInfo[i][LightType][RT_LightsPosZ] = RTL_INVALID_LIGHTS_POS;
        RT_LightsInfo[i][LightType][RT_LightsPosRX] = RTL_INVALID_LIGHTS_POS;
        RT_LightsInfo[i][LightType][RT_LightsPosRY] = RTL_INVALID_LIGHTS_POS;
        RT_LightsInfo[i][LightType][RT_LightsPosRZ] = RTL_INVALID_LIGHTS_POS;
        RT_LightsInfo[i][LightType][RT_LightsDrawDistance] = RTL_INVALID_LIGHTS_POS;

        RT_LightsInfo[i][LightType][RT_LightsNumbers] = INVALID_OBJECT_ID;
        RT_LightsInfo[i][LightType][RT_LightsVerify] = false;
    }
    return 1;
}

stock DestroyLights (LightID, LightType)
{
    if (0 > LightID >= MAX_RTLIGHTS || LightType != 0) return 0;
    if (!RT_LightsInfo[LightID][LightType][RT_LightsVerify]) return 0;

    #if !defined CreateDynamicObject 
        DestroyObject (RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
    #else 
        DestroyDynamicObject (RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
    #endif

    RT_LightsInfo[LightID][LightType][RT_LightsPosX] = RTL_INVALID_LIGHTS_POS;
    RT_LightsInfo[LightID][LightType][RT_LightsPosY] = RTL_INVALID_LIGHTS_POS;
    RT_LightsInfo[LightID][LightType][RT_LightsPosZ] = RTL_INVALID_LIGHTS_POS;
    RT_LightsInfo[LightID][LightType][RT_LightsPosRX] = RTL_INVALID_LIGHTS_POS;
    RT_LightsInfo[LightID][LightType][RT_LightsPosRY] = RTL_INVALID_LIGHTS_POS;
    RT_LightsInfo[LightID][LightType][RT_LightsPosRZ] = RTL_INVALID_LIGHTS_POS;
    RT_LightsInfo[LightID][LightType][RT_LightsDrawDistance] = RTL_INVALID_LIGHTS_POS;

    RT_LightsInfo[LightID][LightType][RT_LightsNumbers] = INVALID_OBJECT_ID;
    RT_LightsInfo[LightID][LightType][RT_LightsVerify] = false;
    
    return 1;
}

#if defined FILTERSCRIPT

    public OnFilterScriptInit ()
    {
        RT_LightsTimer = SetTimer ("rt_LightsDetector", 1000, true);

        #if defined RTL_OnFilterScriptInit
            return RTL_OnFilterScriptInit ();
        #else
            return 1;
        #endif
    }

    public OnFilterScriptExit ()
    {
        KillTimer (RT_LightsTimer);

        #if defined RTL_OnFilterScriptExit
            return RTL_OnFilterScriptExit ();
        #else
            return 1;
        #endif
    }

#else

    public OnGameModeInit ()
    {
        RT_LightsTimer = SetTimer ("rt_LightsDetector", 1000, true);

        #if defined RTL_OnGameModeInit
            return RTL_OnGameModeInit ();
        #else
            return 1;
        #endif
    }

    public OnGameModeExit ()
    {
        KillTimer (RT_LightsTimer);

        #if defined RTL_OnGameModeExit
            return RTL_OnGameModeExit ();
        #else
            return 1;
        #endif
    }

#endif

public rt_LightsDetector ()
{

    for (new i; i < MAX_RTLIGHTS; i++)
    {
        if (!RT_LightsInfo[i][0][RT_LightsVerify]) continue;//To optimize till the latter types are added

        //Type 0:
        if (RTL_IsAnybodyNearLights(RT_LightsInfo[i][0][RT_LightsPosX], RT_LightsInfo[i][0][RT_LightsPosY], RT_LightsInfo[i][0][RT_LightsPosZ]))
        {
            if (!RT_LightsInfo[i][0][RT_IsLightLit] && !RT_LightsInfo[i][0][RT_BlocKLight])
            {

                #if !defined CreateDynamicObject 
                    RT_LightsInfo[i][0][RT_LightsNumbers] = CreateObject (18656, RT_LightsInfo[i][0][RT_LightsPosX], RT_LightsInfo[i][0][RT_LightsPosY], RT_LightsInfo[i][0][RT_LightsPosZ], RT_LightsInfo[i][0][RT_LightsPosRX], RT_LightsInfo[i][0][RT_LightsPosRY], RT_LightsInfo[i][0][RT_LightsPosRZ], RT_LightsInfo[i][0][RT_LightsDrawDistance]);
                #else 
                    RT_LightsInfo[i][0][RT_LightsNumbers] = CreateDynamicObject (18656, RT_LightsInfo[i][0][RT_LightsPosX], RT_LightsInfo[i][0][RT_LightsPosY], RT_LightsInfo[i][0][RT_LightsPosZ], RT_LightsInfo[i][0][RT_LightsPosRX], RT_LightsInfo[i][0][RT_LightsPosRY], RT_LightsInfo[i][0][RT_LightsPosRZ], -1, -1, -1, RT_LightsInfo[i][0][RT_LightsDrawDistance], RT_LightsInfo[i][0][RT_LightsDrawDistance]);
                #endif

                RT_LightsInfo[i][0][RT_IsLightLit] = true;
            }
        }

        if (!RTL_IsAnybodyNearLights(RT_LightsInfo[i][0][RT_LightsPosX], RT_LightsInfo[i][0][RT_LightsPosY], RT_LightsInfo[i][0][RT_LightsPosZ]))
        {
            if (RT_LightsInfo[i][0][RT_IsLightLit] && !RT_LightsInfo[i][0][RT_BlocKLight])
            {

                #if !defined CreateDynamicObject 
                    DestroyObject (RT_LightsInfo[i][0][RT_LightsNumbers]);
                #else 
                    DestroyDynamicObject (RT_LightsInfo[i][0][RT_LightsNumbers]);
                #endif

                RT_LightsInfo[i][0][RT_LightsNumbers] = INVALID_OBJECT_ID;
                RT_LightsInfo[i][0][RT_IsLightLit] = false;
            }
        }
        //End of type 0.
    }

    return 1;
}

stock RTL_IsAnybodyNearLights (Float:LightPosX, Float:LightPosY, Float:LightPosZ)
{
    #if defined foreach 

        foreach (new i: Player)
        {

            if (IsPlayerInRangeOfPoint (i, RTL_MAX_PLAYER_RANGE, LightPosX, LightPosY, LightPosZ)) 
                return 1;
        }

    #else

        for (new i, j = GetPlayerPoolSize(); i <= j; i++)
        {
            if (!IsPlayerConnected (i)) continue;

            if (IsPlayerInRangeOfPoint (i, RTL_MAX_PLAYER_RANGE, LightPosX, LightPosY, LightPosZ)) 
                return 1;
        }

    #endif
    return 0;
}

stock IsLightOn(LightID, LightType) 
{
    if (0 > LightID >= MAX_RTLIGHTS || LightType != 0) return 0;
    if (RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
    return RT_LightsInfo[LightID][LightType][RT_IsLightLit];
} 

stock HideLights(LightID, LightType)
{
    if (0 > LightID >= MAX_RTLIGHTS || LightType != 0) return 0;
    if (RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
    #if !defined CreateDynamicObject 
        DestroyObject (RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
    #else 
        DestroyDynamicObject (RT_LightsInfo[LightID][LightType][RT_LightsNumbers]);
    #endif

    RT_LightsInfo[LightID][LightType][RT_LightsNumbers] = INVALID_OBJECT_ID;
    RT_LightsInfo[LightID][LightType][RT_IsLightLit] = false;
    return RT_LightsInfo[LightID][LightType][RT_BlocKLight] = true;
}

stock ShowLights(LightID, LightType)
{
    if (0 > LightID >= MAX_RTLIGHTS || LightType != 0) return 0;
    if (RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
    return RT_LightsInfo[LightID][LightType][RT_BlocKLight] = false;
}

stock IsPlayerTriggeringLight(playerid, LightID, LightType)
{
    if (0 > LightID >= MAX_RTLIGHTS || LightType != 0 || playerid > MAX_PLAYERS || playerid < 0) return 0;
    if (RT_LightsInfo[LightID][LightType][RT_LightsVerify] == false) return 0;
    return (IsPlayerInRangeOfPoint (playerid, RTL_MAX_PLAYER_RANGE, RT_LightsInfo[LightID][LightType][RT_LightsPosX], RT_LightsInfo[LightID][LightType][RT_LightsPosY], RT_LightsInfo[LightID][LightType][RT_LightsPosZ]) && RT_LightsInfo[LightID][LightType][RT_IsLightLit]);
}

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
