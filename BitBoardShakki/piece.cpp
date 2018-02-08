#include "piece.h"

const Piece* pieces[12] =
{
	new Pawn(UCODE_WPAWN,		Wpawn,		WHITE),
	new Rook(UCODE_WROOK,		Wrook,		WHITE),
	new Knight(UCODE_WKNIGHT,	Wknight,	WHITE),
	new Bishop(UCODE_WBISHOP,	Wbishop,	WHITE),
	new Queen(UCODE_WQUEEN,		Wqueen,		WHITE),
	new King(UCODE_WKING,		Wking,		WHITE),
	
	new Pawn(UCODE_BPAWN,		Bpawn,		BLACK),
	new Rook(UCODE_BROOK,		Brook,		BLACK),
	new Knight(UCODE_BKNIGHT,	Bknight,	BLACK),
	new Bishop(UCODE_BBISHOP,	Bbishop,	BLACK),
	new Queen(UCODE_BQUEEN,		Bqueen,		BLACK),
	new King(UCODE_BKING,		Bking,		BLACK)
	
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