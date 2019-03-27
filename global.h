#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include <iostream>
#include <mutex>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

uintptr_t dwbase = 0;
uintptr_t dwdiscord = 0;

#include "xorstr.h"
#include "lazyimporter.h"
#include "memory.h"
#include "gui/imgui.h"
#include "gui/imgui_internal.h"
#include "gui/imgui_impl_dx11.h"
#include "sdk/vector3d.h"
#include "sdk/c_globalvars.h"
#include "sdk/c_entity.h"
#include "sdk/matrix.h"
#include "sdk/c_input.h"



typedef int(__stdcall* createhook_fn)(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal);
createhook_fn CreateHook = nullptr;

typedef int(__stdcall* enablehook_fn)(LPVOID pTarget,BOOL enable);
enablehook_fn EnableHook = nullptr;

typedef int (__stdcall* applyqueued_fn)(VOID);
applyqueued_fn EnableHookQue = nullptr;

typedef long(__stdcall *present_fn) (IDXGISwapChain* p_swapchain, UINT syncintreval, UINT flags);
present_fn o_present = nullptr;

typedef bool(__fastcall* createmove_fn)(void* cInputPtr, int sequence_number, float input_sample_frametime, bool active);
createmove_fn o_createmove = nullptr;

typedef uintptr_t(__fastcall* createinterface_fn)(const char *, uintptr_t);
createinterface_fn createinterface;

typedef SHORT(__stdcall* getasynckeystate_fn)(int vKey);
getasynckeystate_fn o_getasynckeystate;

typedef bool(__fastcall* worldtoscreen_fn)(c_vec& origin, c_vec& screen);
worldtoscreen_fn o_worldtoscreen;

template<typename t>
inline t vfunc(DWORD_PTR* pTable, int index)
{
	DWORD_PTR* VTableFunctionBase = *(DWORD_PTR**)pTable;
	DWORD_PTR dwAddress = VTableFunctionBase[index];
	return (t)(dwAddress);
}

//trace
#define CONTENTS_EMPTY 0x0
#define	CONTENTS_SOLID 0x1
#define	CONTENTS_WINDOW 0x2
#define CONTENTS_AUX 0x4
#define	CONTENTS_GRATE 0x8
#define CONTENTS_SLIME 0x10
#define CONTENTS_WATER 0x20
#define CONTENTS_WINDOW_NOCOLLIDE 0x40
#define CONTENTS_OPAQUE 0x80
#define CONTENTS_TESTFOGVOLUME 0x100
#define CONTENTS_PHYSICSCLIP 0x200
#define CONTENTS_SOUNDTRIGGER 0x400
#define CONTENTS_NOGRAPPLE 0x800
#define CONTENTS_OCCLUDESOUND 0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000
#define CONTENTS_MOVEABLE 0x4000
#define CONTENTS_TEST_SOLID_BODY_SHOT	0x8000
#define CONTENTS_PLAYERCLIP 0x10000
#define CONTENTS_MONSTERCLIP 0x20000
#define CONTENTS_OPERATOR_FLOOR 0x40000
#define CONTENTS_BLOCKLOS 0x80000
#define CONTENTS_NOCLIMB 0x100000
#define CONTENTS_TITANCLIP 0x200000
#define CONTENTS_BULLETCLIP 0x400000
#define CONTENTS_OPERATORCLIP 0x800000
#define CONTENTS_NOAIRDROP 0x1000000
#define CONTENTS_MONSTER 0x2000000
#define CONTENTS_DEBRIS 0x4000000
#define CONTENTS_DETAIL 0x8000000
#define CONTENTS_TRANSLUCENT 0x10000000
#define CONTENTS_HITBOX 0x40000000

#define TRACE_CONTENTS_OPERATOR_FLOOR (CONTENTS_OPERATOR_FLOOR)
#define TRACE_MASK_SOLID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_PHYSICSCLIP|CONTENTS_MOVEABLE|CONTENTS_MONSTER)
#define TRACE_MASK_PLAYERSOLID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_MONSTER)
#define TRACE_MASK_TITANSOLID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MOVEABLE|CONTENTS_TITANCLIP|CONTENTS_MONSTER)
#define TRACE_MASK_NPCSOLID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_PHYSICSCLIP|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_MONSTER)
#define TRACE_MASK_NPCFLUID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_MONSTER)
#define TRACE_MASK_SOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_PHYSICSCLIP|CONTENTS_MOVEABLE)
#define TRACE_MASK_PLAYERSOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP)
#define TRACE_MASK_NPCSOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP)
#define TRACE_MASK_WATER (CONTENTS_SLIME|CONTENTS_WATER)
#define TRACE_MASK_OPAQUE (CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_MOVEABLE)
#define TRACE_MASK_OPAQUE_AND_NPCS (CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_MOVEABLE|CONTENTS_MONSTER)
#define TRACE_MASK_BLOCKLOS (CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define TRACE_MASK_BLOCKLOS_AND_NPCS (CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS|CONTENTS_MONSTER)
#define TRACE_MASK_VISIBLE (CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE|CONTENTS_MOVEABLE)
#define TRACE_MASK_VISIBLE_AND_NPCS (CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE|CONTENTS_MOVEABLE|CONTENTS_MONSTER)
#define TRACE_MASK_SHOT (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_SLIME|CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_BULLETCLIP|CONTENTS_MONSTER|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define TRACE_MASK_SHOT_BRUSHONLY (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_SLIME|CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_BULLETCLIP|CONTENTS_DEBRIS)
#define TRACE_MASK_GRENADE (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MOVEABLE|CONTENTS_BULLETCLIP|CONTENTS_MONSTER|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define TRACE_MASK_SHOT_HULL (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MOVEABLE|CONTENTS_BULLETCLIP|CONTENTS_MONSTER|CONTENTS_DEBRIS)
#define TRACE_MASK_NPCWORLDSTATIC (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_GRATE|CONTENTS_MONSTERCLIP)

struct surface_t
{
	const char*		name;
	char			surface_props;
	unsigned short	flags; // BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

struct plane_t
{
	c_vec	normal;
	float			dist;
	char			type;			// for fast side tests
	char			signbits;		// signx + (signy<<1) + (signz<<1)
	char			pad[2];
};

struct trace_t
{
	c_vec startpos;
	c_vec endpos;
	plane_t plane;

	int unk01;
	float fraction;

	int contents;

	bool allsolid;
	bool startsolid;

	int unk02;
	surface_t surface;
	float fractionleftsolid;
	int hitgroup; // not sure
	short physicsbone; // not sure
	unsigned short world_surface_index; // not sure
	void* entity;
	int hitbox; // not sure
		// has to have more here. sizeof trace_t == 0x180
};

enum Hitbox
{
	NECK_B = 0, // basically head
	NECK_A,
	SPINE_C,
	SPINE_B,
	SPINE_A,
	HIP,
	LEFT_SHOULDER,
	LEFT_ELBOW,
	LEFT_WRIST,
	RIGHT_SHOULDER,
	RIGHT_ELBOW,
	RIGHT_WRIST,
	LEFT_THIGH,
	LEFT_KNEE,
	LEFT_ANKLE,
	LEFT_BALL,
	RIGHT_THIGH,
	RIGHT_KNEE,
	RIGHT_ANKLE,
	RIGHT_BALL,
	LEFT_THIGH2,
	RIGHT_THIGH2,
	RIGHT_SHOULDER2,
	LEFT_SHOULDER2
};

// used for ray tracing
enum Hitgroup
{
	GENERIC = 0,
	HEAD = 1,
	CHEST = 2,
	STOMACH = 3,
	LEFTARM = 4,
	RIGHTARM = 5,
	LEFTLEG = 6,
	RIGHTLEG = 7,
	GEAR = 10
};