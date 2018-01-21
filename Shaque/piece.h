#pragma once
#include <string>
#include <vector>

#include "enums.h"
#include "move.h"

/*
#define UCODE_WKING		L"K"
#define UCODE_WQUEEN	L"Q"	
#define UCODE_WROOK		L"R"	 
#define UCODE_WBISHOP	L"B"	
#define UCODE_WKNIGHT	L"K"	
#define UCODE_WPAWN		L"P"
#define UCODE_BKING		L"K"	
#define UCODE_BQUEEN	L"Q"
#define UCODE_BROOK		L"R"
#define UCODE_BBISHOP	L"B"
#define UCODE_BKNIGHT	L"K"
#define UCODE_BPAWN		L"P"
*/
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

class State;

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
	virtual std::vector<PossibleMove> getPossibleMoves(const State& gameState, const Move& lastOpponentMove, POSITION pos) const = 0;
};

extern const Piece* pieces[12];

class Rook : public Piece {
public:
	Rook(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Rook() {};
	std::vector<PossibleMove> getPossibleMoves(const State&, const Move&, POSITION) const;
};

class Knight : public Piece {
public:
	Knight(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Knight() {};
	std::vector<PossibleMove> getPossibleMoves(const State&, const Move&, POSITION) const;
};

class Bishop : public Piece {
public:
	Bishop(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Bishop() {};
	std::vector<PossibleMove> getPossibleMoves(const State&, const Move&, POSITION) const;
};

class Queen : public Piece {
public:
	Queen(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Queen() {};
	std::vector<PossibleMove> getPossibleMoves(const State&, const Move&, POSITION) const;
};

class King : public Piece {
public:
	King(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	King() {};
	std::vector<PossibleMove> getPossibleMoves(const State&, const Move&, POSITION) const;
};

class Pawn : public Piece {
public:
	Pawn(const std::wstring& unicode, PIECE code, COLOR color) : Piece(unicode, code, color) {};
	Pawn() {};
	std::vector<PossibleMove> getPossibleMoves(const State&, const Move&, POSITION) const;
};

