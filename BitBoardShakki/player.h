#pragma once
#include "game.h"
#include "algo.h"

class Player {
public:
	virtual Move askMove(const Game& game, const State& state, COLOR col) = 0;
	virtual void opponentMoveMade(const Game& game, const State& newState, const Move& move, COLOR enemyCol) {}
};

class HumanPlayer : public Player {
public:
	Move askMove(const Game& game, const State& state, COLOR col);
};

class AIPlayer : public Player {
private:
	static StateEvaluator* _defaultEval;
	static DecisionAlgorithm* _defaultAlgo;

	StateEvaluator* _eval;
	DecisionAlgorithm* _algo;
public:
	AIPlayer();
	AIPlayer(StateEvaluator* eval, DecisionAlgorithm* algo);
	Move askMove(const Game& game, const State& state, COLOR col);
};