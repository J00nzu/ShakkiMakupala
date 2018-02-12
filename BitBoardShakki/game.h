#pragma once
#include "statebb.h"
#include "move.h"
#include "ui.h"
#include <vector>

struct GameRecording;
class Player;

class Game {
private:
	Player* _pwhite = nullptr;
	Player* _pblack = nullptr;

	std::vector<State> _state_history;
	std::vector<Move> _move_history;
public:
	Game(Player* white, Player* black);
	const std::vector<State>* getStateHistory() const;
	const std::vector<Move>* getMoveHistory() const;
	int stateOccurredTimes(const State& state) const;

	GameRecording Play(bool drawUI);
};

struct GameRecording {
	OUTCOME outcome;
	std::vector<State> stateHistory;
	std::vector<Move> moveHistory;
};