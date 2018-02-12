#pragma once
#include "statebb.h"

class UI {
public:
	static UI* singleton;

	UI();
	void drawBoard(const State& state, bool clear = false) const;
	Move askForMove(const MoveSet& possibleMoves) const;
};