#include "algo.h"
#include "movegen.h"
#include <time.h>

Move DumbDecisions::decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval) {
	srand((unsigned int)time(NULL));
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();

	MoveSet moves = moveGen->GenLegalMoves(state);
	moves.sort();
	Move move;
	if (moves.size() > 6) {
		move = moves[rand() % 6];
	}
	else if(!moves.empty()){
		move = moves[0];
	}
	return move;

}
