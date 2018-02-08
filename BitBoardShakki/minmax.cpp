#include "algo.h"
#include "movegen.h"

struct MinMaxReturn {
	int eval = 0;
	Move move;

	MinMaxReturn() {
		eval = INT_MIN;
	}
	MinMaxReturn(const Move& m) {
		move = m;
		eval = INT_MIN;
	}
};

#define NUM_DEPTH 3

//int maxi(int depth, asema a) {
// Tarkasta onko matti tai patti, jos on niin poistu asap, matti -100000, patti 0
//	if (depth == 0) return evaluate();
//	int max = -oo;
//	for (all moves ) {
//		score = mini(depth - 1, seuraaja);
//		if (score > max)
//			max = score;
//	}
//	return max;
//}

//int mini(int depth, asema a) {
// Tarkasta onko matti tai patti, jos on niin poistu asap, matti -100000, patti 0
//	if (depth == 0) return -evaluate();
//	int min = +oo;
//	for (all moves) {
//		score = maxi(depth - 1);
//		if (score < min)
//			min = score;
//	}
//	return min;
//}

int mini(int depth, const State& state, const Move& lastOpponentMove, const StateEvaluator* eval, const MoveGenerator* moveGen);

int maxi(int depth, const State& state, const Move& lastOpponentMove, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	if (depth == 0) return eval->evaluate(state);
	int max = INT_MIN;
	auto mvs = moveGen->GenPseudoLegalMoves(state, lastOpponentMove);
	for each(auto move in mvs) {
		State newState = state.advanceTurn(move);
		int score = mini(depth - 1, newState, move, eval, moveGen);
		if (score > max)
			max = score;
	}
	return max;
}

int mini(int depth, const State& state, const Move& lastOpponentMove, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	if (depth == 0) return eval->evaluate(state);
	int min = INT_MAX;
	auto mvs = moveGen->GenPseudoLegalMoves(state, lastOpponentMove);
	for each(auto move in mvs) {
		State newState = state.advanceTurn(move);
		int score = maxi(depth - 1, newState, move, eval, moveGen);
		if (score < min)
			min = score;
	}
	return min;
}



Move MinMaxAlgorithm::decideAMove(const State& state, const Move& lastOpponentMove, const StateEvaluator* eval) {
	srand(0);
	MinMaxReturn bestMove;
	const MoveGenerator* moveGen = &MoveGenerator::getInstance();
	if (state.getTurnColor() == BLACK) {
		bestMove.eval = INT_MAX;
	}
	auto mvs = moveGen->GenLegalMoves(state, lastOpponentMove);
	mvs.sort();
	for each (auto move in mvs)
	{
		State newState = state.advanceTurn(move);
		int score;
		if (state.getTurnColor() == WHITE) {
			score = mini(NUM_DEPTH, newState, move, eval, moveGen);
			if (score > bestMove.eval) {
				bestMove.move = move;
				bestMove.eval = score;
			}
		}
		else {
			score = maxi(NUM_DEPTH, newState, move, eval, moveGen);
			if (score < bestMove.eval) {
				bestMove.move = move;
				bestMove.eval = score;
			}
		}


		if (score == bestMove.eval && (rand() % 2) == 0) {
			bestMove.move = move;
		}
	}

	return bestMove.move;
}
