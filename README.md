# Tech_Lights

[![sampctl](https://shields.southcla.ws/badge/sampctl-Tech_Lights-2f2f2f.svg?style=for-the-badge)](https://github.com/RogueDrifter/Tech_Lights)

This is an include that allows you to manipulate lights better, simply put this creates lights that only light up when someone<br/>
 is near them within a range you can control.<br/>
<br/>
UPDATED: June 16th - 2019<br/>
<br/>
`Installation`: <br/>
You simply include it in the script you want to use the lights in and use the functions as simple as that.<br/>

`Testing`: <br/>
All can be done through the test.pwn folder which already<br/>
contains the include and a simple test use of the functions.<br/>

`Version`: `3.0`<br/>
<br/>
# Installation<br/>
<br/>
Simply install to your project:<br/>

```bash
sampctl package install RogueDrifter/Tech_Lights
```
<br/>
Include in your code and begin using the library:<br/>

```pawn
#include <Tech_Lights>
```

# Usage<br/>
<br/>
To get the coords for the lights you can do so using any map editor and the modelid for the object is 18656<br/>
https://dev.prineside.com/en/gtasa_samp_model_id/model/18656-LightBeamWhite/ is a link of that object's preview.<br/>
<br/>
So as you can see within the functions there are 2 parameters, LightID and LightType<br/>
<br/>
The first one (LightID) is put to define the light object in order to destroy it later, so say you created a light using the function CreateLights using id 0, in order to destroy it using DestroyLights you'll need to use the same ID (0) to destroy that light, simple?<br/>
<br/>
The maximum amount of lights to be created is a 100 by default which you can control by using:<br/>
<br/>

```bash
#define MAX_RTLIGHTS (NUMBER)

//Use this for types:
#define LIGHT_TYPE_WHITE 0
#define LIGHT_TYPE_RED 1
#define LIGHT_TYPE_BLUE 2
```
So yeah make sure your LightID is never below 0 or above the MAX_RTLIGHTS<br/>
<br/>
The second one (LightType) there are currently 3 types and the 4th one will be released soon, the current three ones (ranging from 0 to 2 as in 0,1,2) are white lights, red lights and blue lights in this respective order.<br/>
```bash
LightType:
0- Normal white lights
1- Colored lights red
2- Colored lights blue
```
Almost all of the functions are self explanatory so the last thing here is:<br/>
<br/>
```bash
#define RTL_MAX_PLAYER_RANGE (RANGE)
```
Which as it says defines the range the player needs to be within the position of the light in order to 'trigger' as in lighten it up.<br/>
<br/>
Quick example (included in test.pwn):<br/>
```bash
// generated by "sampctl package generate"

#include "Tech_Lights.inc"

main() {
	CreateLights (0, 0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0);
}
```

# Testing<br/>
<br/>
Use the test.pwn file and check the server for client messages/log for printed messages.<br/>
<br/>
To test, simply run the package:<br/>

```bash
sampctl package run
```

# Contains:<br/>

## Functions:

```pawn
CreateLights (LightType, LightID, Float:PosX, Float:PosY, Float:PosZ, Float:PosRX, Float:PosRY, Float:PosRZ, Float:DrawDistance = RTL_MAX_PLAYER_RANGE);

DestroyLights (LightID, LightType);

DestroyAllLights (LightType);

HideLights (LightID, LightType);

ShowLights (LightID, LightType);

BlinkLights (LightID, LightType, Toggle);

KeepLightsOn (LightID, LightType, Toggle);

KeepAllLightsOn (LightType, Toggle);

IsLightKeptOn (LightID, LightType);

IsLightValid (LightID, LightType);

IsLightBlinking (LightID, LightType);

IsLightOn (LightID, LightType);

IsPlayerTriggeringLight (playerid, LightID, LightType);

ToggleLightForPlayer (playerid, LightID, LightType, Toggle);

PointLights(LightID, LightType, Direction);

ShowAllLights (LightType);

HideAllLights (LightType);

BlinkToDestroyLights (LightID, LightType);  

GetLightObjectID(LightID, LightType);

DanceLights(LightID, LightType, bool:toggle);  

MoveLights (LightID, LightType, Float:ToX, Float:ToY, Float:ToZ, Float:ToRX, Float:ToRY, Float:ToRZ);

```
## Callbacks:<br/>

```pawn
OnPlayerTriggerLight(playerid, LightID, LightType);

OnLightGoOff(LightID, LightType);
```

# How to update:
<br/>
Simply open your project and run the code:<br/>

```bash
sampctl package ensure
```

## Example usage:<br/>
Lights_Event: https://github.com/RogueDrifter/Lights_Event<br/>
<br/>
## Non sampctl users:<br/>
Go get sampctl! <br/>
But for now:<br/>
1- Download the repository from the github link.<br/>
<br/>
2- Take the .inc file and put it in your pawno/include.<br/>
<br/>
3- Open the script you want to use the lights in and use #include <Tech_Lights><br/>
<br/>
4- Use the functions provided above as you like.<br/>
