#include "algo.h"
#include "game.h"
#include "player.h"

#include <io.h>
#include <fcntl.h>
#include <iostream>

/*TODO castling human*/

int main() {

	//Player* p1 = new HumanPlayer();
	StateEvaluator* stateEv = new DefaultStateEvaluator();
	StateEvaluator* posStateEv = new PositionalStateEvaluator();

	DecisionAlgorithm* minmax = new MinMaxAlgorithm();
	DecisionAlgorithm* alphabeta = new AlphaBetaAlgorithm();
	DecisionAlgorithm* dumbai = new DumbDecisions();


	Player* p1 = new AIPlayer(posStateEv, alphabeta);
	Player* p2 = new AIPlayer(stateEv, alphabeta);


	Game game(p1, p2);

	GameRecording record = game.Play(true);

	if (record.outcome == OUTCOME::WIN_WHITE) {
		std::wcout << "Checkmate. White wins!";
	}
	else if (record.outcome == OUTCOME::WIN_BLACK) {
		std::wcout << "Checkmate. Black wins!";
	}

	system("pause");

	return 0;
}