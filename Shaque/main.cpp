#include <stdlib.h>

#include "piece.h"
#include "state.h"
#include "ui.h"
#include "game.h"
#include "player.h"

#include <io.h>
#include <fcntl.h>
#include <iostream>


/*TODO
* Game draw judge
* State evaluator for AI
* - Default State evaluator
* - Neural net State evaluator
* Move evaluator for AI
* - Short term gains
*/

int main() {

	//Player* p1 = new HumanPlayer();
	StateEvaluator* stateEv = new DefaultStateEvaluator();
	DecisionAlgorithm* algorithm = new MinMaxAlgorithm();

	Player* p1 = new AIPlayer(stateEv, algorithm);
	Player* p2 = new AIPlayer(stateEv, algorithm);
	UI ui;


	Game game(p1, p2, &ui);

	GameRecording record = game.Play();

	if (record.outcome == OUTCOME::WIN_WHITE) {
		std::wcout << "Checkmate. White wins!";
	}
	else if (record.outcome == OUTCOME::WIN_BLACK) {
		std::wcout << "Checkmate. Black wins!";
	}

	/*
	State gameState = State::initializeEmpty();
	POSITION pos = D4;

	gameState.board[C5] = BPAWN;

	gameState.board[pos] = WKING;
	auto piece = gameState.getPiece(pos);

	ui.drawBoard(gameState, pos, piece->getPossibleMoves(gameState, Move(), pos));

	for each (PossibleMove move in piece->getPossibleMoves(gameState, Move(), pos))
	{
		_sleep(200);
		auto str = move.toStr();

		std::wcout << str_to_wstr(str);
		std::wcout << L"\n" << std::endl;
		ui.drawBoard(gameState.advanceTurn(move.move), move.move.endPos, piece->getPossibleMoves(gameState, Move(), pos));

	}*/

	return 0;
}