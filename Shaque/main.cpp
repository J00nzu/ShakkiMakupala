#include "piece.h"
#include "state.h"
#include "ui.h"

#include <io.h>
#include <fcntl.h>
#include <iostream>

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	State gameState = State::initialize();
	UI ui(&gameState);

	ui.drawBoard();

	return 0;
}