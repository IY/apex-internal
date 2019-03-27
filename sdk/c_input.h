#pragma once
#include "../global.h"
class c_input {
public:
	void third_person(bool enable) {
		*(bool*)((uintptr_t)this + 0x8D) = enable;
	}
}; c_input* g_pinput = nullptr;