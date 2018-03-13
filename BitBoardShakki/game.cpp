#include "game.h"
#include "player.h"

Game::Game(Player* white, Player* black) {
	_pwhite = white;
	_pblack = black;
}

const std::vector<State>* Game::getStateHistory() const {
	return &_state_history;
}

const std::vector<Move>* Game::getMoveHistory() const {
	return &_move_history;
}

GameRecording Game::Play(bool drawUI) {
	State currentState = State::initialize();
	Move lastMove;
	Move move;
	while (true) {
		_state_history.push_back(currentState);

		if (drawUI) {
			UI::singleton->drawBoard(currentState, lastMove);
		}

		if (currentState.getTurnColor() == WHITE) {
			move = _pwhite->askMove(*this, currentState, WHITE);
		}
		else {
			move = _pblack->askMove(*this, currentState, BLACK);
		}

		if (move.isNoPossibleMoves()) { // if a player has no moves left :x
			GameRecording record;
			record.moveHistory = _move_history;
			record.stateHistory = _state_history;
			if (currentState.getTurnColor() == WHITE) {
				record.outcome = WIN_BLACK;
			}
			else {
				record.outcome = WIN_WHITE;
			}
			return record;
		}

		currentState = currentState.advanceTurn(move);
		_move_history.push_back(move);

		if (currentState.getTurnColor() == WHITE) {
			_pwhite->opponentMoveMade(*this, currentState, move, BLACK);
		}
		else {
			_pblack->opponentMoveMade(*this, currentState, move, WHITE);
		}
		lastMove = move;
	}
}

int Game::stateOccurredTimes(const State& state) const {
	if (_state_history.empty())
		return 0;
	int times = 0;
	for each (auto st in _state_history)
	{
		if (state == st) {
			times++;
		}
	}
	return times;
}
