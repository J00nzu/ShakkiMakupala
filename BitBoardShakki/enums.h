#pragma once
#include "defines.h"
#include <cstdint>


enum DIR8 : uint8_t {
	noEa,
	east,
	soEa,
	sout,
	soWe,
	west,
	noWe,
	nort
};

enum PIECE : uint8_t {
	Wpawn,
	Wrook,
	Wknight,
	Wbishop,
	Wqueen,
	Wking,
	Bpawn,
	Brook,
	Bknight,
	Bbishop,
	Bqueen,
	Bking
};

enum COLOR : bool {
	WHITE = false,
	BLACK = true
};

enum OUTCOME : int_least8_t {
	WIN_WHITE = 1,
	WIN_BLACK = -1,
	DRAW = 0
};

enum FILEIDX : uint_least8_t {
	FILE1 = 0,
	FILE2,
	FILE3,
	FILE4,
	FILE5,
	FILE6,
	FILE7,
	FILE8,
	NOFILE
};

enum BITPOS : U64 {
	bH8 = 1ULL << 63,
	bG8 = 1ULL << 62,
	bF8 = 1ULL << 61,
	bE8 = 1ULL << 60,
	bD8 = 1ULL << 59,
	bC8 = 1ULL << 58,
	bB8 = 1ULL << 57,
	bA8 = 1ULL << 56,
	bH7 = 1ULL << 55,
	bG7 = 1ULL << 54,
	bF7 = 1ULL << 53,
	bE7 = 1ULL << 52,
	bD7 = 1ULL << 51,
	bC7 = 1ULL << 50,
	bB7 = 1ULL << 49,
	bA7 = 1ULL << 48,
	bH6 = 1ULL << 47,
	bG6 = 1ULL << 46,
	bF6 = 1ULL << 45,
	bE6 = 1ULL << 44,
	bD6 = 1ULL << 43,
	bC6 = 1ULL << 42,
	bB6 = 1ULL << 41,
	bA6 = 1ULL << 40,
	bH5 = 1ULL << 39,
	bG5 = 1ULL << 38,
	bF5 = 1ULL << 37,
	bE5 = 1ULL << 36,
	bD5 = 1ULL << 35,
	bC5 = 1ULL << 34,
	bB5 = 1ULL << 33,
	bA5 = 1ULL << 32,
	bH4 = 1ULL << 31,
	bG4 = 1ULL << 30,
	bF4 = 1ULL << 29,
	bE4 = 1ULL << 28,
	bD4 = 1ULL << 27,
	bC4 = 1ULL << 26,
	bB4 = 1ULL << 25,
	bA4 = 1ULL << 24,
	bH3 = 1ULL << 23,
	bG3 = 1ULL << 22,
	bF3 = 1ULL << 21,
	bE3 = 1ULL << 20,
	bD3 = 1ULL << 19,
	bC3 = 1ULL << 18,
	bB3 = 1ULL << 17,
	bA3 = 1ULL << 16,
	bH2 = 1ULL << 15,
	bG2 = 1ULL << 14,
	bF2 = 1ULL << 13,
	bE2 = 1ULL << 12,
	bD2 = 1ULL << 11,
	bC2 = 1ULL << 10,
	bB2 = 1ULL << 9,
	bA2 = 1ULL << 8,
	bH1 = 1ULL << 7,
	bG1 = 1ULL << 6,
	bF1 = 1ULL << 5,
	bE1 = 1ULL << 4,
	bD1 = 1ULL << 3,
	bC1 = 1ULL << 2,
	bB1 = 1ULL << 1,
	bA1 = 1ULL << 0,
};