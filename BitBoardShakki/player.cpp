#include "player.h"
#include "movegen.h"
#include <iostream>

using namespace std;

Move HumanPlayer::askMove(const Game& game, const State& state, COLOR col) {
	auto allPossibleMoves = MoveGenerator::getInstance().GenLegalMoves(state,
		game.getMoveHistory()->empty() ? Move() : game.getMoveHistory()->back());
	if (allPossibleMoves.empty()) {
		Move noMoves;
		return noMoves;
	}
	allPossibleMoves.sort();
	
	auto ui = UI::singleton;
	if (ui) {
		return ui->askForMove(allPossibleMoves);
	}
	else {
		wcerr << L"No UI in the game. Please create a game with UI if you have human players";
		return allPossibleMoves[0];
	}
}