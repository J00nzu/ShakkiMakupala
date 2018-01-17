#pragma once
#include <string>
#include <cstdint>

#define UCODE_WKING		L"\u2654"
#define UCODE_WQUEEN	L"\u2655"	
#define UCODE_WROOK		L"\u2656"	 
#define UCODE_WBISHOP	L"\u2657"	
#define UCODE_WKNIGHT	L"\u2658"	
#define UCODE_WPAWN		L"\u2659"
#define UCODE_BKING		L"\u265A"	
#define UCODE_BQUEEN	L"\u265B"
#define UCODE_BROOK		L"\u265C"
#define UCODE_BBISHOP	L"\u265D"
#define UCODE_BKNIGHT	L"\u265E"
#define UCODE_BPAWN		L"\u265F"


enum PIECE : uint_fast8_t
{
	WROOK = 0,	WKNIGHT, WBISHOP, WQUEEN, WKING, WPAWN,
	BROOK,		BKNIGHT, BBISHOP, BQUEEN, BKING, BPAWN
};

Piece pieces[12] = 
{
	Piece(UCODE_WROOK,		WROOK,		WHITE), 
	Piece(UCODE_WKNIGHT,	WKNIGHT,	WHITE),
	Piece(UCODE_WBISHOP,	WBISHOP,	WHITE),
	Piece(UCODE_WQUEEN,		WQUEEN,		WHITE),
	Piece(UCODE_WKING,		WKING,		WHITE),
	Piece(UCODE_WPAWN,		WPAWN,		WHITE),

	Piece(UCODE_BROOK,		BROOK,		BLACK),
	Piece(UCODE_BKNIGHT,	BKNIGHT,	BLACK),
	Piece(UCODE_BBISHOP,	BBISHOP,	BLACK),
	Piece(UCODE_BQUEEN,		BQUEEN,		BLACK),
	Piece(UCODE_BKING,		BKING,		BLACK),
	Piece(UCODE_BPAWN,		BPAWN,		BLACK)
};

enum COLOR : bool {
	WHITE = false,
	BLACK = true
};

class Piece {

private:
	std::wstring _unicode;
	COLOR _color; // valkea = 0, musta = 1
	uint_fast8_t _code; // VT, VR, MT tms.
public:
	Piece(const std::wstring&, PIECE, COLOR);
	Piece() {};
	void setCode(PIECE);
	PIECE getCode();
	void setUnicode(const std::wstring&);
	std::wstring getUnicode() const;
	void setVari(COLOR);
	COLOR getVari() const;
};
