#pragma once
#include <string>
#include <vector>
#include "enums.h"

class Piece {

protected:
	std::wstring _unicode;
	COLOR _color; // valkea = 0, musta = 1
	PIECE _code; // VT, VR, MT tms.
public:
	Piece(const std::wstring&, PIECE, COLOR);
	Piece() {};
	void setCode(PIECE);
	PIECE getCode() const;
	void setUnicode(const std::wstring&);
	std::wstring getUnicode() const;
	void setColor(COLOR);
	COLOR getColor() const;
};

extern const Piece* pieces[12];

class Rook : public Piece {
public:
	Rook(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Rook() {};
};

class Knight : public Piece {
public:
	Knight(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Knight() {};
};

class Bishop : public Piece {
public:
	Bishop(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Bishop() {};
};

class Queen : public Piece {
public:
	Queen(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Queen() {};
};

class King : public Piece {
public:
	King(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	King() {};
};

class Pawn : public Piece {
public:
	Pawn(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Pawn() {};
};

