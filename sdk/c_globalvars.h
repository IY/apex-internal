#pragma once
#include "../global.h"

class c_globalvars
{
public:
	float realtime; //0x0000
	int framecount; //0x0004
	float absoluteframetime; //0x0008
	float curtime; //0x000C
	float frametime; //0x0010
	int tickcount; //0x0014
	float Unk1; //0x0018
	float interval_per_tick; //0x001C
	float interpolation_amount; //0x0020
	int simTicksThisFrame; //0x0024
	int network_protocol; //0x0028
}; c_globalvars* g_pglobals = nullptr;