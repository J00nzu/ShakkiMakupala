#include "state.h"
#include "piece.h"
#include "flagmanip.h"
#include "movement.h"
#include <math.h>

const MoveGenerator* State::safeGen = new MoveGenerator();

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

std::vector<PossibleMove> State::getAllLegalMoves(const Move& lastOpponentMove, const MoveGenerator* moveGen) const {
	std::vector<PossibleMove> allPossibleMoves;
	for (size_t i = 0; i < 64; i++)
	{
		POSITION pos = (POSITION)i;
		auto pc = getPiece(pos);
		if (!pc)
			continue;
		if (pc->getColor() == getTurnColor()) {
			pc->getPossibleMoves(allPossibleMoves, *this, lastOpponentMove, pos);
		}
	}
	movegeneration::pruneOutKingDangers(allPossibleMoves, *this);

	return allPossibleMoves;
}

std::vector<PossibleMove> State::getAllRawMoves(const Move& lastOpponentMove, const MoveGenerator* moveGen) const {
	std::vector<PossibleMove> allPossibleMoves;
	for (size_t i = 0; i < 64; i++)
	{
		POSITION pos = (POSITION)i;
		auto pc = getPiece(pos);
		if (!pc)
			continue;
		if (pc->getColor() == getTurnColor()) {
			pc->getPossibleMoves(allPossibleMoves, *this, lastOpponentMove, pos);
		}
	}
	return allPossibleMoves;
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

	state.setBlackShortCastlingAllowed();
	state.setBlackLongCastlingAllowed();

	state.setWhiteShortCastlingAllowed();
	state.setWhiteLongCastlingAllowed();

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

	state.setBlackShortCastlingAllowed();
	state.setBlackLongCastlingAllowed();

	state.setWhiteShortCastlingAllowed();
	state.setWhiteLongCastlingAllowed();

	state.setTurnWhite();

	return state;
}



int State::getRank(POSITION pos) {
	int posi = pos;
	int rank = 8 - (int)((pos-0.25) / 8.0);
	return rank;
}

char State::getLetter(POSITION pos) {
	int rem = pos % 8;
	return (char)(65 + rem);
}


void doCastling(const Move& move, const State* prevState, State& newState) {
	if (move.getColor() == WHITE) {
		if (move.getCastlingLong()) {
			newState.board[E1] = EMPTY;
			newState.board[A1] = EMPTY;
			newState.board[C1] = WKING;
			newState.board[D1] = WROOK;
		}
		else if (move.getCastlingShort()) {
			newState.board[E1] = EMPTY;
			newState.board[H1] = EMPTY;
			newState.board[G1] = WKING;
			newState.board[F1] = WROOK;
		}
		newState.unSetWhiteShortCastlingAllowed();
		newState.unSetWhiteLongCastlingAllowed();
	}else {
		if (move.getCastlingLong()) {
			newState.board[E8] = EMPTY;
			newState.board[A8] = EMPTY;
			newState.board[C8] = BKING;
			newState.board[D8] = BROOK;
		}
		else if (move.getCastlingShort()) {
			newState.board[E8] = EMPTY;
			newState.board[H8] = EMPTY;
			newState.board[G8] = BKING;
			newState.board[F8] = BROOK;
		}
		newState.unSetBlackShortCastlingAllowed();
		newState.unSetBlackLongCastlingAllowed();
	}
}

void doEnPassant(const Move& move, const State* prevState, State& newState) {
	if (move.getEnPassantRight()) {
		POSITION markPos = movement::moveRight(move.startPos);
		newState.board[markPos] = EMPTY;
	}
	else if (move.getEnPassantLeft()) {
		POSITION markPos = movement::moveLeft(move.startPos);
		newState.board[markPos] = EMPTY;
	}
}

State State::advanceTurn(const Move& move) const {
	State newState(*this);

	if (getTurnColor() == WHITE) {
		newState.setTurnBlack();
	}
	else {
		newState.setTurnWhite();
	}

	newState.board[move.startPos] = EMPTY;
	newState.board[move.endPos] = move.piece;

	if (move.getPromotion()) {
		newState.board[move.endPos] = move.promotedTo;
	}else if (move.getCastlingLong() || move.getCastlingShort()) {
		doCastling(move, this, newState);
	}else if (move.getEnPassantLeft() || move.getEnPassantRight()) {
		doEnPassant(move, this, newState);
	}

	const uint_least8_t castlingMask = STATE_H_WHITE_LONG_CASTLING_ALLOWED | STATE_H_WHITE_SHORT_CASTLING_ALLOWED | STATE_H_BLACK_LONG_CASTLING_ALLOWED | STATE_H_BLACK_SHORT_CASTLING_ALLOWED;

	if((castlingMask & flags) != 0){ //if any castling is still possible
		if (move.piece == WKING) {
			newState.unSetWhiteLongCastlingAllowed();
			newState.unSetWhiteShortCastlingAllowed();
		} else if ((move.piece == WROOK && move.startPos == A1) || move.endPos == A1) {
			newState.unSetWhiteLongCastlingAllowed();
		} else if ((move.piece == WROOK && move.startPos == H1) || move.endPos == H1) {
			newState.unSetWhiteShortCastlingAllowed();
		}

		if (move.piece == BKING) {
			newState.unSetBlackLongCastlingAllowed();
			newState.unSetBlackShortCastlingAllowed();
		} else if ((move.piece == BROOK && move.startPos == A8) || move.endPos == A8) {
			newState.unSetBlackLongCastlingAllowed();
		} else if ((move.piece == BROOK && move.startPos == H8) || move.endPos == H8) {
			newState.unSetBlackShortCastlingAllowed();
		}
	}

	return newState;
}


