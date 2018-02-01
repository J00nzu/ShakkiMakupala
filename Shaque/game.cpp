#include "game.h"
#include "player.h"


Game::Game(Player* white, Player* black) {
	_pwhite = white;
	_pblack = black;
}

Game::Game(Player* white, Player* black, UI* ui) {
	_pwhite = white;
	_pblack = black;
	_pUI = ui;
}

const std::vector<State>* Game::getStateHistory() const {
	return &_state_history;
}

const std::vector<Move>* Game::getMoveHistory() const {
	return &_move_history;
}

const UI* Game::getUI() const {
	return _pUI;
}

GameRecording Game::Play() {
	State currentState = State::initialize();
	Move move;
	while (true) {
		_state_history.push_back(currentState);

		if (_pUI) {
			_pUI->drawBoard(currentState);
		}

		if (currentState.getTurnColor() == WHITE) {
			move = _pwhite->askMove(*this, currentState, WHITE);
		}
		else {
			move = _pblack->askMove(*this, currentState, BLACK);
		}

		if (move.getNoPossibleMoves()) { // if a player has no moves left :x
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
	}
}