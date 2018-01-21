#include "piece.h"
#include "state.h"


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

const Piece* pieces[12] =
{
	new Rook(UCODE_WROOK,		WROOK,		WHITE),
	new Knight(UCODE_WKNIGHT,	WKNIGHT,	WHITE),
	new Bishop(UCODE_WBISHOP,	WBISHOP,	WHITE),
	new Queen(UCODE_WQUEEN,		WQUEEN,		WHITE),
	new King(UCODE_WKING,		WKING,		WHITE),
	new Pawn(UCODE_WPAWN,		WPAWN,		WHITE),

	new Rook(UCODE_BROOK,		BROOK,		BLACK),
	new Knight(UCODE_BKNIGHT,	BKNIGHT,	BLACK),
	new Bishop(UCODE_BBISHOP,	BBISHOP,	BLACK),
	new Queen(UCODE_BQUEEN,		BQUEEN,		BLACK),
	new King(UCODE_BKING,		BKING,		BLACK),
	new Pawn(UCODE_BPAWN,		BPAWN,		BLACK)
};

Piece::Piece(const std::wstring& unicode, PIECE code, COLOR color) {
	_unicode = unicode;
	_code = code;
	_color = color;
}
void Piece::setCode(PIECE code) {
	_code = code;
}

PIECE Piece::getCode() const {
	return _code;
}

void Piece::setUnicode(const std::wstring& unicode) {
	_unicode = unicode;
}

std::wstring Piece::getUnicode() const {
	return _unicode;
}

void Piece::setColor(COLOR color) {
	_color = color;
}

COLOR Piece::getColor() const {
	return _color;
}
