#pragma once
#include "global.h"


namespace memory {
	inline uintptr_t occurence(const char* module, const char* pattern) {

		#define in_range(x, a, b) (x >= a && x <= b)
		#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
		#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

		MODULEINFO mod;
		iat(K32GetModuleInformation).get()(iat(GetCurrentProcess).get()(), iat(GetModuleHandleA).get()(module), &mod, sizeof(MODULEINFO));
		uintptr_t start = (uintptr_t)mod.lpBaseOfDll;
		uintptr_t end = (uintptr_t)mod.lpBaseOfDll + (uintptr_t)mod.SizeOfImage;
		uintptr_t match = 0;
		const char* current = pattern;

		for (uintptr_t pCur = start; pCur < end; pCur++) {

			if (!*current)
				return match;

			if (*(PBYTE)current == ('\?') || *(BYTE*)pCur == get_byte(current)) {
				if (!match)
					match = pCur;

				if (!current[2])
					return match;

				if (*(PWORD)current == ('\?\?') || *(PBYTE)current != ('\?'))
					current += 3;
				else
					current += 2;
			} else {
				current = pattern;
				match = 0;
			}
		}
		return 0;
	}

	inline uintptr_t dereference(uintptr_t address, unsigned int offset)
	{
		if (address == 0)
			return 0;

		if (sizeof(uintptr_t) == 8)
			return address + (int)((*(int*)(address + offset) + offset) + sizeof(int));

		return (uintptr_t)*(unsigned long*)(address + offset);

	}

}