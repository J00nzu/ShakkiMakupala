#include "piece.h"
#include "state.h"
#include "ui.h"

#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "coversion.h"

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	State gameState = State::initializeEmpty();
	UI ui(&gameState);
	POSITION pos = D4;

	gameState.board[C5] = BPAWN;

	gameState.board[pos] = WBISHOP;
	auto piece = gameState.getPiece(pos);

	ui.drawBoard(pos, piece->getPossibleMoves(gameState, Move(), pos));
	for each (PossibleMove move in piece->getPossibleMoves(gameState, Move(), pos))
	{
		auto str = move.toStr();
		
		std::wcout << str_to_wstr(str);
		std::wcout << L", ";
	}

	return 0;
}