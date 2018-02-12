#include "player.h"

StateEvaluator* AIPlayer::_defaultEval = new DefaultStateEvaluator();
DecisionAlgorithm* AIPlayer::_defaultAlgo = new MinMaxAlgorithm();


AIPlayer::AIPlayer() {
	_eval = _defaultEval;
	_algo = _defaultAlgo;
}
AIPlayer::AIPlayer(StateEvaluator* eval, DecisionAlgorithm* algo) {
	_eval = eval;
	_algo = algo;
}
Move AIPlayer::askMove(const Game& game, const State& state, COLOR col) {
	Move lastOpponentMove = game.getMoveHistory()->empty() ? Move() : game.getMoveHistory()->back();
	Move chosenMove = _algo->decideAMove(state, game, lastOpponentMove, _eval);
	return chosenMove;
}