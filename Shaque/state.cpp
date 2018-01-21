#include "state.h"
#include "piece.h"
#include "flagmanip.h"
#include <math.h>

const Piece* State::getPiece(POSITION pos) const{
	if (board[pos] == EMPTY) {
		return nullptr;
	}
	else {
		return pieces[board[pos]];
	}
}

void State::setPiece(POSITION pos, PIECE piece) {
	board[pos] = piece;
}


State State::initialize() {
	State state;
	for each (PIECE &p in state.board) {
		p = EMPTY;
	}

	state.board[A8] = BROOK;
	state.board[B8] = BKNIGHT;
	state.board[C8] = BBISHOP;
	state.board[D8] = BQUEEN;
	state.board[E8] = BKING;
	state.board[F8] = BBISHOP;
	state.board[G8] = BKNIGHT;
	state.board[H8] = BROOK;

	state.board[A7] = BPAWN;
	state.board[B7] = BPAWN;
	state.board[C7] = BPAWN;
	state.board[D7] = BPAWN;
	state.board[E7] = BPAWN;
	state.board[F7] = BPAWN;
	state.board[G7] = BPAWN;
	state.board[H7] = BPAWN;

	state.board[A1] = WROOK;
	state.board[B1] = WKNIGHT;
	state.board[C1] = WBISHOP;
	state.board[D1] = WQUEEN;
	state.board[E1] = WKING;
	state.board[F1] = WBISHOP;
	state.board[G1] = WKNIGHT;
	state.board[H1] = WROOK;

	state.board[A2] = WPAWN;
	state.board[B2] = WPAWN;
	state.board[C2] = WPAWN;
	state.board[D2] = WPAWN;
	state.board[E2] = WPAWN;
	state.board[F2] = WPAWN;
	state.board[G2] = WPAWN;
	state.board[H2] = WPAWN;

	state.flags = 0; // clear any flags

	state.setBlackCastlingAllowed();
	state.setWhiteCastlingAllowed();
	state.setTurnWhite();

	return state;
}
State State::initializeEmpty() 
{
	State state;
	for each (PIECE &p in state.board) {
		p = EMPTY;
	}

	state.flags = 0; // clear any flags

	state.setBlackCastlingAllowed();
	state.setWhiteCastlingAllowed();
	state.setTurnWhite();

	return state;
}



int State::getRank(POSITION pos) {
	int posi = pos;
	int rank = 8 - (int)floor(pos / 8.0);
	return rank;
}

char State::getLetter(POSITION pos) {
	int rem = pos % 8;
	return (char)(65 + rem);
}

