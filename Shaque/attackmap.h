#pragma once
#include "enums.h"
#include <nmmintrin.h>

struct AttackMap{
	uint_least64_t map;
	void setAttacked(POSITION pos);
	void isAttacked(POSITION pos);
};

static int NumberOfSetBits(uint64_t i)
{
#ifdef _WIN64
	return _mm_popcnt_u64(i);
#else
	i = i - ((i >> 1) & 0x5555555555555555UL);
	i = (i & 0x3333333333333333UL) + ((i >> 2) & 0x3333333333333333UL);
	return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FUL) * 0x101010101010101UL) >> 56);
#endif
}