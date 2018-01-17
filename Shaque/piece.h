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
	VT, VR, VL, VD, VK, VS,
	MT, MR, ML, MD, MK, MS
};

enum COLOR : bool {
	WHITE = false,
	BLACK = true
};

class Piece {

private:
	std::wstring _unicode;
	COLOR _vari; // valkea = 0, musta = 1
	uint_fast8_t _koodi; // VT, VR, MT tms.
public:
	Piece(const std::wstring&, int, bool);
	Piece() {};
	void setKoodi(uint_fast8_t);
	uint_fast8_t getKoodi();
	void setUnicode(const std::wstring&);
	std::wstring getUnicode() const;
	void setVari(COLOR);
	COLOR getVari() const;
};
