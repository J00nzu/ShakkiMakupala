#include "ui.h"
#include <iostream>
#include <Windows.h>


using namespace std;

UI::UI(State* state) {
	_state = state;
}

State* UI::getState() {
	return _state;
}

void UI::setState(State* state) {
	_state = state;
}

void UI::drawBoard() {

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

			const Piece* piece = _state->getPiece((POSITION)(y * 8 + x));
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

