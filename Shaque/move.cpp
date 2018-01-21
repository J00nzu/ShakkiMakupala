#include "move.h"
#include "flagmanip.h"
#include "piece.h"
#include "state.h"


Move::Move(const Piece* pc, POSITION start, POSITION end){
	piece = pc->getCode();
	if (pc->getColor() == WHITE) {
		setColorWhite();
	}
	startPos = start;
	endPos = end;
}

std::string PossibleMove::toStr() {
	std::string out;
	out += State::getLetter(move.startPos);
	out += std::to_string(State::getRank(move.startPos));
	out += " to ";
	out += State::getLetter(move.endPos);
	out += std::to_string(State::getRank(move.endPos));
	return out;
}
