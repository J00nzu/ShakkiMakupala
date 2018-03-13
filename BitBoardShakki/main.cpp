#include "algo.h"
#include "game.h"
#include "player.h"
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "hash.h"


int main() {
	//Player* p1 = new HumanPlayer();
	StateEvaluator* stateEv = new DefaultStateEvaluator();
	StateEvaluator* posStateEv = new PositionalStateEvaluator();
	StateEvaluator* mapEv = new MapStateEvaluator();

	DecisionAlgorithm* minmax = new MinMaxAlgorithm();
	DecisionAlgorithm* alphabeta = new AlphaBetaAlgorithm();
	DecisionAlgorithm* dumbai = new DumbDecisions();
	DecisionAlgorithm* smart = new SmartAlphaBeta(MoveClock(10));

	//Player* p1 = new HumanPlayer();
	Player* p1 = new AIPlayer(mapEv, alphabeta);
	Player* p2 = new AIPlayer(posStateEv, alphabeta);



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