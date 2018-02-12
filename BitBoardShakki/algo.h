#pragma once
#include "move.h"
#include "statebb.h"
#include "game.h"

class StateEvaluator {
public:
	virtual int evaluate(const State& state) const = 0;
};

class DefaultStateEvaluator : public StateEvaluator {
public:
	int evaluate(const State& state) const;
};

class PositionalStateEvaluator : public StateEvaluator {
public:
	PositionalStateEvaluator();
	int evaluate(const State& state) const;
};

class DecisionAlgorithm {
public:
	virtual Move decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval) = 0;
};

class MinMaxAlgorithm : public DecisionAlgorithm {
public:
	Move decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval);
};

class DumbDecisions : public DecisionAlgorithm {
public:
	Move decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval);
};

class AlphaBetaAlgorithm : public DecisionAlgorithm {
public:
	Move decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval);
};