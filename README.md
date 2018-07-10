# Tech_Lights

[![sampctl](https://shields.southcla.ws/badge/sampctl-Tech_Lights-2f2f2f.svg?style=for-the-badge)](https://github.com/RogueDrifter/Tech_Lights)

> This is an include that allows you to manipulate lights better, simply put this creates lights that only light up when someone
> is near them within a range you can control, more functions will be added such as the intensity and much more.
>
> UPDATED: July 10th - 2018
>
> `## Installation`: 
> You simply include it in the script you want to use the lights in and use the functions as simple as that.
>
> `## Testing`: 
> All can be done through the test.pwn folder which already
> contains the include and a simple test use of the functions.
>
> `## Version`: `1.0`

## Installation

Simply install to your project:

```bash
sampctl package install RogueDrifter/Tech_Lights
```

Include in your code and begin using the library:

```pawn
#include <Tech_Lights>
```

## Usage

> To get the coords for the lights you can do so using any map editor and the modelid for the object is 18656
> https://dev.prineside.com/en/gtasa_samp_model_id/model/18656-LightBeamWhite/ is a link of that object's preview.
>
> So as you can see within the functions there are 2 parameters, LightID and LightType
>
> The first one (LightID) is put to define the light object in order to destroy it later, so say you created a light using the function CreateLights using id 0, in order to destroy it using DestroyLights you'll need to use the same ID (0) to destroy that light, simple?

> The maximum amount of lights to be created is a 100 by default which you can control by using:
>

```bash
#define MAX_RTLIGHTS (NUMBER)
```
> So yeah make sure your LightID is never below 0 or above the MAX_RTLIGHTS
>
> The second one (LightType) is something you shouldn't worry about for now, just use 0 because that's the only one available on version 1.0 later on i will add more light types (colored ones, blinking ones and so on) but for now it is always 0.

Almost all of the functions are self explanatory so the last thing here is:

```bash
#define RTL_MAX_PLAYER_RANGE (RANGE)
```
> Which as it says defines the range the player needs to be within the position of the light in order to 'trigger' as in lighten it up.

Quick example (included in test.pwn):
```bash
// generated by "sampctl package generate"

#include "RT-Lights.inc"

main() {
	CreateLights (0, 0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0);
}
```

## Testing

> Use the test.pwn file and check the server for client messages/log for printed messages.

To test, simply run the package:

```bash
sampctl package run
```

## Contains:

> CreateLights (LightType, LightID, Float:PosX, Float:PosY, Float:PosZ, Float:PosRX, Float:PosRY, Float:PosRZ, Float:DrawDistance = 0.0);
> DestroyLights (LightID, LightType);
>
> DestroyAllLights(LightType);
>
> HideLights(LightID, LightType);
>
> ShowLights(LightID, LightType);
>
> IsLightOn(LightID, LightType);
>
> IsPlayerTriggeringLight(playerid, LightID, LightType);


## How to update:

>Simply open your project and run the code:

```bash
sampctl package ensure
```

## Non sampctl users:
> Go get sampctl! 
>
> But for now:
>
> 1- Download the repository from the github link.

> 2- Take the .inc file and put it in your pawno/include.

> 3- Open the script you want to use the lights in and use #include <Tech_Lights>

> 4- Use the functions provided above as you like.
