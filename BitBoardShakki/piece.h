#pragma once
#include "bitboardops.h"
#include <string>
#include <vector>

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
	//virtual void getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const = 0;
};

class Rook : public Piece {
public:
	Rook(const std::wstring& ws, PIECE p, COLOR c) : Piece(ws, p, c) {}
};
class Knight : public Piece {
public:
	Knight(const std::wstring& ws, PIECE p, COLOR c) : Piece(ws, p, c) {}
};
class Bishop : public Piece {
public:
	Bishop(const std::wstring& ws, PIECE p, COLOR c) : Piece(ws, p, c) {}
};
class Queen : public Piece {
public:
	Queen(const std::wstring& ws, PIECE p, COLOR c) : Piece(ws, p, c) {}
};
class King : public Piece {
public:
	King(const std::wstring& ws, PIECE p, COLOR c) : Piece(ws, p, c) {}
};
class Pawn : public Piece {
public:
	Pawn(const std::wstring& ws, PIECE p, COLOR c) : Piece(ws, p, c) {}
};



extern const Piece* pieces[12];