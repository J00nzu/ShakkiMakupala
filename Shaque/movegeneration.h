#pragma once
#include <vector>
#include "enums.h"
#include "piece.h"
#include "move.h"
#include "movement.h"

namespace movegeneration{

void move(std::vector<PossibleMove>& list,const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range);

void moveOrCapture(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range);

void capture(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range);

};