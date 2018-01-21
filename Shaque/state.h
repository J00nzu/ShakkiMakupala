#pragma once
#include "piece.h"
#include "enums.h"


class State {

public:
	PIECE board[8 * 8];

	State() {};
	const Piece* getPiece(POSITION pos) const;
	void setPiece(POSITION pos, PIECE piece);
	static State initialize();
	static State initializeEmpty();
	static int getRank(POSITION);
	static char getLetter(POSITION pos);

};