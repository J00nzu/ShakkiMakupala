#pragma once
#include <vector>
#include "enums.h"
#include "piece.h"
#include "move.h"
#include "movement.h"

class MoveGenerator {
	//TODO
};

namespace movegeneration{

void move(std::vector<PossibleMove>& list,const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range);

void moveOrCapture(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range);

void capture(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range);

void checkCastling(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos);

void checkEnPassant(std::vector<PossibleMove>& list, const State& state, const Move& lastOpponentMove, const Piece* piece, POSITION startPos);

void checkPromotionForPawnMoves(std::vector<PossibleMove>& list, const Piece* piece, POSITION startPos);

// Remove any moves that put the king in danger
void pruneOutKingDangers(std::vector<PossibleMove>& list, const State& state);
};