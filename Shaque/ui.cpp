#include "ui.h"
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>


using namespace std;

UI::UI() {
	_setmode(_fileno(stdout), _O_U16TEXT);
}


void UI::drawBoard(const State& state) const{

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


	for (size_t y = 0; y < 8; y++)
	{
		wcout << " " << 8 - y << " ";
		for (size_t x = 0; x < 8; x++)
		{
			if ((x+y) % 2 == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY |
					BACKGROUND_GREEN);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED |
					BACKGROUND_GREEN | BACKGROUND_BLUE);
			}

			const Piece* piece = state.getPiece((POSITION)(y * 8 + x));
			if (piece) {
				wcout << " " << piece->getUnicode() << " ";
			}
			else {
				wcout << "   ";
			}

		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		wcout << "\n";
	}

	wcout << "   " << " A " << " B " << " C " << " D " << " E " << " F " << " G " << " H " << "\n";

}

bool movesContainsEndPos(std::vector<PossibleMove> possibleMoves, POSITION pos) {
	for each (auto mov in possibleMoves)
	{
		if (mov.move.endPos == pos) {
			return true;
		}
	}
	return false;
}

void UI::drawBoard(const State& state, POSITION selectedPiece, std::vector<PossibleMove> possibleMoves) const{

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


	for (size_t y = 0; y < 8; y++)
	{
		wcout << " " << 8 - y << " ";
		for (size_t x = 0; x < 8; x++)
		{
			POSITION pos = (POSITION)(y * 8 + x);
			if (pos == selectedPiece) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY |
					BACKGROUND_BLUE);
			}
			else if(movesContainsEndPos(possibleMoves, pos)){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY |
					BACKGROUND_RED);
			}
			else if ((x + y) % 2 == 1) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY |
					BACKGROUND_GREEN);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED |
					BACKGROUND_GREEN | BACKGROUND_BLUE);
			}

			const Piece* piece = state.getPiece(pos);
			if (piece) {
				wcout << " " << piece->getUnicode() << " ";
			}
			else {
				wcout << "   ";
			}

		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		wcout << "\n";
	}

	wcout << "   " << " A " << " B " << " C " << " D " << " E " << " F " << " G " << " H " << "\n";

}
