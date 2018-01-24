#pragma once
#include "game.h"

class Player {
public:
	virtual Move askMove(const Game& game, const State& state, COLOR col) = 0;
};

class HumanPlayer : public Player {
	Move askMove(const Game& game, const State& state, COLOR col);
};