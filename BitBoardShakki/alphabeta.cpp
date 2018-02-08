#include "algo.h"
#include "movegen.h"
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>


#define NUM_DEPTH 4

struct AlphaBetaReturn {
	int eval = 0;
	Move move;
	const State* state;
	const StateEvaluator* evaluator;

	AlphaBetaReturn() {};
	AlphaBetaReturn(const State* st, const Move& m, const StateEvaluator* e) {
		move = m;
		state = st;
		evaluator = e;

		if (state->getTurnColor() == WHITE) {
			eval = INT_MIN;
		}else{
			eval = INT_MAX;
		}

	}
};


MoveSet& getMoves(const State& state, const Move& lastOpponenMove) {
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();

	MoveSet moves = moveGen->GenPseudoLegalMoves(state, lastOpponenMove);
	moves.sort();
	return moves;
}

std::mutex queueMutex;
std::queue<AlphaBetaReturn*> workQueue;


/*
int alphaBetaMax(int alpha, int beta, int depthleft) {
	if (depthleft == 0) return evaluate();
	for (all moves) {
		score = alphaBetaMin(alpha, beta, depthleft - 1);
		if (score >= beta)
			return beta;   // fail hard beta-cutoff
		if (score > alpha)
			alpha = score; // alpha acts like max in MiniMax
	}
	return alpha;
}

int alphaBetaMin(int alpha, int beta, int depthleft) {
	if (depthleft == 0) return -evaluate();
	for (all moves) {
		score = alphaBetaMax(alpha, beta, depthleft - 1);
		if (score <= alpha)
			return alpha; // fail hard alpha-cutoff
		if (score < beta)
			beta = score; // beta acts like min in MiniMax
	}
	return beta;
}
*/

int alphaBetaMin(int alpha, int beta, int depthleft, const State& state, const Move& lastOpponentMove, const StateEvaluator* eval, const MoveGenerator* moveGen);

int alphaBetaMax(int alpha, int beta, int depthleft, const State& state, const Move& lastOpponentMove, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	if (state.pieceBB[Wking] == 0) return INT_MIN;
	if (depthleft == 0) return eval->evaluate(state);
	auto mvs = getMoves(state, lastOpponentMove);
	for each(auto move in mvs) {
		State newState = state.advanceTurn(move);
		int score = alphaBetaMin(alpha, beta, depthleft - 1, newState, move, eval, moveGen);
		if (score >= beta)
			return beta;   // fail hard beta-cutoff
		if (score > alpha)
			alpha = score; // alpha acts like max in MiniMax
	}
	return alpha;
}

int alphaBetaMin(int alpha, int beta, int depthleft, const State& state, const Move& lastOpponentMove, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	if (state.pieceBB[Bking] == 0) return INT_MAX;
	if (depthleft == 0) return eval->evaluate(state);

	auto mvs = getMoves(state, lastOpponentMove);
	mvs.sort();
	for each(auto move in mvs) {
		State newState = state.advanceTurn(move);
		int score = alphaBetaMax(alpha, beta, depthleft - 1, newState, move, eval, moveGen);
		if (score <= alpha)
			return alpha; // fail hard alpha-cutoff
		if (score < beta)
			beta = score; // beta acts like min in MiniMax
	}
	return beta;
}




void workerThread() {
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();

	while (true) {
		queueMutex.lock();
		if (workQueue.empty()) {
			queueMutex.unlock();
			return;
		}
		else {
			AlphaBetaReturn* next = workQueue.front();
			workQueue.pop();
			queueMutex.unlock();

			State newState = next->state->advanceTurn(next->move);
			bool late = ((next->state->pieceBB[Wqueen] | next->state->pieceBB[Bqueen]) == 0);
			int depth = NUM_DEPTH + ((int)late);

			int score;
			if (next->state->getTurnColor() == WHITE) {
				score = alphaBetaMin(INT_MIN, INT_MAX, depth, newState, next->move, next->evaluator, moveGen);
				next->eval = score;
			}
			else {
				score = alphaBetaMax(INT_MIN, INT_MAX, depth, newState, next->move, next->evaluator, moveGen);
				next->eval = score;
			}

		}
	}
}

void LaunchAndWaitWorkers() {
	unsigned nthreads = std::thread::hardware_concurrency();
	if (nthreads == 0) nthreads = 1;

	auto threads = new std::thread[nthreads];

	for (int i = 0; i < nthreads; i++) {
		threads[i] = std::thread(workerThread);
	}

	for (int i = 0; i < nthreads; i++) {
		threads[i].join();
	}

	delete[] threads;
}


Move AlphaBetaAlgorithm::decideAMove(const State& state, const Move& lastOpponentMove, const StateEvaluator* eval) {
	srand(0);
	std::vector<AlphaBetaReturn> returnMoves;
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();
	auto mvs = moveGen->GenLegalMoves(state, lastOpponentMove);
	if (mvs.empty()) {
		return Move();
	}

	State initial = State(state);

	mvs.sort();
	for each (auto move in mvs)
	{
		AlphaBetaReturn abr(&initial, move, eval);
		returnMoves.push_back(abr);
	}

	for (int i = 0; i < returnMoves.size(); i++) {
		workQueue.push(&returnMoves[i]);
	}

	LaunchAndWaitWorkers();

	AlphaBetaReturn* bestMove = &returnMoves[0];

	for (int i = 1; i < returnMoves.size();i++) {
		AlphaBetaReturn& ret = returnMoves[i];
		int score = ret.eval;
		if (state.getTurnColor() == WHITE) {
			if (score > bestMove->eval) {
				bestMove = &ret;
			}
		}
		else {
			if (score < bestMove->eval) {
				bestMove = &ret;
			}
		}
	}

	return bestMove->move;
}