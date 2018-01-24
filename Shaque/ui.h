#pragma once
#include "state.h"
#include <vector>

class UI {
public:
	UI();
	void drawBoard(const State& state) const;
	void drawBoard(const State& state, POSITION selectedPiece, std::vector<PossibleMove> possibleMoves) const;
};