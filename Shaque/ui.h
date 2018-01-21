#pragma once
#include "state.h"
#include <vector>

class UI {
	State* _state;

public:
	UI(State* state);
	State* getState();
	void setState(State* state);
	void drawBoard();
	void drawBoard(POSITION selectedPiece, std::vector<PossibleMove> possibleMoves);
};