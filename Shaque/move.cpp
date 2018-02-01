#include "move.h"
#include "flagmanip.h"
#include "piece.h"
#include "state.h"
#include "conversion.h"


Move::Move(const Piece* pc, POSITION start, POSITION end){
	piece = pc->getCode();
	if (pc->getColor() == WHITE) {
		setColorWhite();
	}
	startPos = start;
	endPos = end;
}

bool Move::operator == (const Move& other) {
	if (
		(getCastlingLong() && other.getCastlingLong()) ||
		(getCastlingShort() && other.getCastlingShort())) {
		return true;
	}
	if (other.startPos == startPos && other.endPos == endPos && other.promotedTo == promotedTo) {
		return true;
	}
	return false;
}


std::wstring PossibleMove::toStr() {
	return moveToStr(move);
}

PossibleMove::operator Move() const {
	return move;
}
