#pragma once
#include "move.h"
#include "state.h"
#include "stateevaluator.h"

class DecisionAlgorithm {
public:
	virtual Move decideAMove(const State& state, const Move& lastOpponentMove, const StateEvaluator* eval) = 0;
};


class MinMaxAlgorithm : public DecisionAlgorithm {
public:
	Move decideAMove(const State& state, const Move& lastOpponentMove, const StateEvaluator* eval);
};