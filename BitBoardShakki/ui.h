#pragma once
#include "statebb.h"

class UI {
public:
	static UI* singleton;

	UI();
	void drawBoard(const State& state, bool clear = false) const;
};