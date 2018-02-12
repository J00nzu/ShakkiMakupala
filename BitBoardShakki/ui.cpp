#include "ui.h"
#include "convert.h"
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

UI* UI::singleton = new UI();

UI::UI() {
	_setmode(_fileno(stdout), _O_U16TEXT);
}

void UI::drawBoard(const State& state, bool clear) const {
	if(clear)system("cls");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


	for (int y = 7; y >= 0; y--)
	{
		wcout << L" " << (y+1) << L" ";
		for (size_t x = 0; x < 8; x++)
		{
			U64 pos = 1ULL << (y * 8 + x);
			if ((x + y) % 2 == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY |
					BACKGROUND_GREEN);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED |
					BACKGROUND_GREEN | BACKGROUND_BLUE);
			}

			const Piece* piece = state.getPiece((BITPOS)(pos));
			if (piece) {
				wcout << " " << piece->getUnicode() << " ";
			}
			else {
				wcout << "   ";
			}

		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		wcout << endl;
	}

	wcout << "   " << " A " << " B " << " C " << " D " << " E " << " F " << " G " << " H " << endl;
}

Move UI::askForMove(const MoveSet& possibleMoves) const {
	while (true) {
		Move selected;
		wcout << L"Input a valid move in Dd1-e2 format (promotion is Pa7-a8Q): ";
		std::wstring in;
		wcin >> in;
		if (!strToMove(in, selected, possibleMoves.get(0).getColor())) {
			wcout << L"Invalid format!" << endl;
			continue;
		}
		for each (auto mov in possibleMoves)
		{
			if (selected.isCastle()) {
				if (selected.getMoveType() == mov.getMoveType()) {
					return mov;
				}
			}else if (selected.isPromotion()) {
				if (selected.getCPiece() == mov.getCPiece() && selected.getFromBB() == mov.getFromBB()
					&& selected.getToBB() == mov.getToBB()) {
					return mov;
				}
			}else if (selected.getFromBB() == mov.getFromBB()
				&& selected.getToBB() == mov.getToBB()) {
				return mov;
			}
		}
		wcout << L"Given move is not legal! grr : " << moveToStr(selected) << endl;

	}
}
