#pragma once
#include "../global.h"

class CUserCmd
{
public:
	int        command_number;
	int        tick_count;
	float      command_time;
	c_vec      viewangles;
	char pad[0xC];
	float      forwardmove; //clamp to [-1;1]
	float      sidemove; //clamp to [-1;1]
	float      upmove; //clamp to [-1;1]
	int        buttons;
	BYTE      impulse;
};