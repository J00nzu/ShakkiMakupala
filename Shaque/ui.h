#pragma once
#include "state.h"

class UI {
	State* _state;

public:
	UI(State* state);
	State* getState();
	void setState(State* state);
	void drawBoard();
};
