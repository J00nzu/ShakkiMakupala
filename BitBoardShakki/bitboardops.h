#pragma once
#include <bitset>
#include <stdlib.h>
#include "enums.h"
#include "flagmanip.h"
#include <intrin.h>


static const int shift[8] = { 9, 1,-7,-8,-9,-1, 7, 8 };

static const U64 avoidWrap[8] =
{
	U64(0xfefefefefefefe00),
	U64(0xfefefefefefefefe),
	U64(0x00fefefefefefefe),
	U64(0x00ffffffffffffff),
	U64(0x007f7f7f7f7f7f7f),
	U64(0x7f7f7f7f7f7f7f7f),
	U64(0x7f7f7f7f7f7f7f00),
	U64(0xffffffffffffff00)
};

static const U64 ranks[8] = {
	U64(0x00000000000000FF),
	U64(0x000000000000FF00),
	U64(0x0000000000FF0000),
	U64(0x00000000FF000000),
	U64(0x000000FF00000000),
	U64(0x0000FF0000000000),
	U64(0x00FF000000000000),
	U64(0xFF00000000000000)
};

static const U64 files[8] = {
	U64(0x0101010101010101),
	U64(0x0202020202020202),
	U64(0x0404040404040404),
	U64(0x0808080808080808),
	U64(0x1010101010101010),
	U64(0x2020202020202020),
	U64(0x4040404040404040),
	U64(0x8080808080808080)
};

inline U64 bitB_move(U64 b, DIR8 d) {
	//return b << shift[d] & avoidWrap[d];
	return _rotl64(b, shift[d]) & avoidWrap[d];
}


inline U64 bitB_nortOne(U64 b) {
	return _rotl64(b, shift[nort]) & avoidWrap[nort];
}
inline U64 bitB_soutOne(U64 b) {
	return _rotl64(b, shift[sout]) & avoidWrap[sout];
}
inline U64 bitB_eastOne(U64 b) {
	return _rotl64(b, shift[east]) & avoidWrap[east];
}
inline U64 bitB_westOne(U64 b) {
	return _rotl64(b, shift[west]) & avoidWrap[west];
}
inline U64 bitB_noWeOne(U64 b) {
	return _rotl64(b, shift[noWe]) & avoidWrap[noWe];
}
inline U64 bitB_noEaOne(U64 b) {
	return _rotl64(b, shift[noEa]) & avoidWrap[noEa];
}
inline U64 bitB_soWeOne(U64 b) {
	return _rotl64(b, shift[soWe]) & avoidWrap[soWe];
}
inline U64 bitB_soEaOne(U64 b) {
	return _rotl64(b, shift[soEa]) & avoidWrap[soEa];
}


inline int bitP_getRank(BITPOS p) {
	for (int i = 0; i < 8; i++) {
		if (CheckFlag(ranks[i], p)) return i+1;
	}
	return 0;
}

inline int bitB_popCount(U64 i)
{
#ifdef _WIN64
	return (int)_mm_popcnt_u64(i);
#else
	i = i - ((i >> 1) & 0x5555555555555555UL);
	i = (i & 0x3333333333333333UL) + ((i >> 2) & 0x3333333333333333UL);
	return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FUL) * 0x101010101010101UL) >> 56);
#endif
}

inline int bitB_bitScanForward(U64 i)
{
	unsigned long index = 0;
	_BitScanForward64(&index, i);
	return (int)index;
}

inline int bitB_bitScanReverse(U64 i)
{
	unsigned long index = 0;
	_BitScanReverse64(&index, i);
	return (int)index;
}