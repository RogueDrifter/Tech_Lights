// generated by "sampctl package generate"

#include "Tech_Lights.inc"

main() {
	CreateLights(0, 0, -2084.66650, -212.29964, 47.75250,   -92.00000, 0.00000, 1.00000, 300.0);
	CreateLights(1, 1, -2083.24731, -146.62630, 47.75250,   -97.00000, 0.00000, 1.00000, 300.0);
	CreateLights(2, 2, -2083.54443, -163.21716, 47.75250,   -92.00000, 0.00000, 1.00000, 300.0);
	CreateLights(0, 3, -2083.43384, -176.61205, 47.75250,   -92.00000, 0.00000, 1.00000, 300.0);
	CreateLights(1, 4, -2084.10913, -193.73671, 47.75250,   -92.00000, 0.00000, 1.00000, 300.0);
}

public OnPlayerTriggerLight(playerid, LightID, LightType)
{
	SendClientMessageToAll(-1, "A light has been turned on");
	return 1;
}

public OnLightGoOff(LightID, LightType)
{
	SendClientMessageToAll(-1, "A light has been turned off");
	return 1;
}
