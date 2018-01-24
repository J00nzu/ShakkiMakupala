#pragma once
#include "state.h"
#include "move.h"
#include "ui.h"
#include <vector>

struct GameRecording;
class Player;

class Game {
private:
	Player* _pwhite = nullptr;
	Player* _pblack = nullptr;
	UI* _pUI = nullptr;

	std::vector<State> _state_history;
	std::vector<Move> _move_history;
public:
	Game(Player* white, Player* black);
	Game(Player* white, Player* black, UI* ui);
	const std::vector<State>* getStateHistory() const;
	const std::vector<Move>* getMoveHistory() const;
	const UI* getUI() const;

	GameRecording Play();
};

struct GameRecording {
	OUTCOME outcome;
	std::vector<State> stateHistory;
	std::vector<Move> moveHistory;
};

