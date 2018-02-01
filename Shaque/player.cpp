#include "player.h"
#include "move.h"
#include "game.h"
#include "state.h"
#include <iostream>
#include <string>
#include "conversion.h"

using namespace std;



POSITION askPiece(const Game& game, const State& state, COLOR col) {
	while (true) {
		wcout << (col==WHITE?L"White, ":L"Black, ") << L"select a piece to move: ";
		std::wstring in;
		wcin >> in;
		POSITION pos;
		if (!strToPos(in, pos)) {
			wcout << L"Wrong input position format, please input in format 'B4'" << endl;
			continue;
		}
		auto pc = state.getPiece(pos);
		if (pc == nullptr) {
			wcout << L"No piece in position: " << in << endl;
			continue;
		}
		if (col != pc->getColor()) {
			wcout << L"Piece in position is not your color: " << in << endl;
			continue;
		}
		return pos;
	}
}

Move selectMove(const std::vector<PossibleMove>& moves, POSITION poshison) {
	while (true) {
		wcout << L"Select the position to move to: ";
		std::wstring in;
		wcin >> in;
		POSITION pos;
		if (!strToPos(in, pos)) {
			wcout << L"Wrong input position format, please input in format 'B4'" << endl;
			continue;
		}
		for each(PossibleMove p in moves) {
			if (p.move.endPos == pos) {
				if (p.move.getPromotion()) {
					// ask for promotion piece
				}
				return p.move;
			}
		}
		wcout << L"Selected position was not in possible moves, please reinput" << endl;
	}
}


/*
Move HumanPlayer::askMove(const Game& game, const State& state, COLOR col) {
	POSITION pos;
	std::vector<PossibleMove> moves;
	while (moves.empty()) {
		pos = askPiece(game, state, col);
		auto pc = state.getPiece(pos);


		moves = pc->getPossibleMoves(state,
			game.getMoveHistory()->empty() ? Move() : game.getMoveHistory()->back(), pos);
		if (moves.empty()) {
			wcout << L"No moves available for the selected piece" << endl;
		}
	}
	auto ui = game.getUI();
	if (ui) {
		ui->drawBoard(state, pos, moves);
	}
	return selectMove(moves, pos);
}
*/

Move HumanPlayer::askMove(const Game& game, const State& state, COLOR col) {
	std::vector<PossibleMove> allPossibleMoves = state.getAllLegalMoves(
		game.getMoveHistory()->empty() ? Move() : game.getMoveHistory()->back());
	if (allPossibleMoves.empty()) {
		Move noMoves;
		noMoves.setNoPossibleMoves();
		return noMoves;
	}
	auto ui = game.getUI();
	if (ui) {
		return ui->askForMove(allPossibleMoves);
	}
	else {
		wcerr << L"No UI in the game. Please create a game with UI if you have human players";
		return allPossibleMoves[0].move;
	}
}