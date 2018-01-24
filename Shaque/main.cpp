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
* King move - King does not end up in check
* Check & checkmate
* Game draw judge
* Ask player for promotion piece
*/

int main() {

	Player* p1 = new HumanPlayer();
	Player* p2 = new HumanPlayer();
	UI ui;


	Game game(p1, p2, &ui);

	game.Play();

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