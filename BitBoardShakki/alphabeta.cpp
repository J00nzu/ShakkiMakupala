#include "algo.h"
#include "movegen.h"
#include <queue>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include "convert.h"
#include <iostream>
#include <sstream>
#include "caching.h"

#ifdef _DEBUG
#define NUM_DEPTH 5
#else
#define NUM_DEPTH 5
#endif // _DEBUG


struct AlphaBetaReturn { 
	int eval = 0;
	Move move;
	const State* state;
	const StateEvaluator* evaluator;
	const Game* game;

	AlphaBetaReturn() {};
	AlphaBetaReturn(const State* st, const Game* g, const Move& m, const StateEvaluator* e) {
		move = m;
		state = st;
		evaluator = e;
		game = g;

		if (state->getTurnColor() == WHITE) {
			eval = INT_MIN;
		}else{
			eval = INT_MAX;
		}

	}
};

static std::mutex queueMutex;
static std::queue<AlphaBetaReturn*> workQueue;

static MoveSet getMoves(const State& state) {
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();

	MoveSet moves = moveGen->GenPseudoLegalMoves(state);
	moves.sort();
	return moves;
}


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

int alphaBetaMin(int alpha, int beta, int depthleft, const State& state, const StateEvaluator* eval, const MoveGenerator* moveGen);

int alphaBetaMax(int alpha, int beta, int depthleft, const State& state, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	//if (state.pieceBB[Wking] == 0) return INT_MIN;
	if (depthleft == 0) return eval->evaluate(state);
	
	/*
	CacheItem cacheit;
	if (checkCache(state, cacheit) && depthleft <= cacheit.depth) {
		return cacheit.evaluation;
	}*/
	
	auto mvs = getMoves(state);
	for each(auto move in mvs) {
		State newState = state.advanceTurn(move);
		int score = alphaBetaMin(alpha, beta, depthleft - 1, newState, eval, moveGen);
		if (score >= beta)
			return beta;   // fail hard beta-cutoff
		if (score > alpha)
			alpha = score; // alpha acts like max in MiniMax
	}
	
	//addToCache(state, alpha, depthleft);

	return alpha;
}

int alphaBetaMin(int alpha, int beta, int depthleft, const State& state, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	//if (state.pieceBB[Bking] == 0) return INT_MAX;
	if (depthleft == 0) return eval->evaluate(state);

	/*
	CacheItem cacheit;
	if (checkCache(state, cacheit) && depthleft <= cacheit.depth) {
		return cacheit.evaluation;
	}*/
	
	auto mvs = getMoves(state);
	for each(auto move in mvs) {
		State newState = state.advanceTurn(move);
		int score = alphaBetaMax(alpha, beta, depthleft - 1, newState, eval, moveGen);
		if (score <= alpha)
			return alpha; // fail hard alpha-cutoff
		if (score < beta)
			beta = score; // beta acts like min in MiniMax
	}

	//addToCache(state, beta, depthleft);

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

			if (next->game->stateOccurredTimes(newState) >= 2) {
				continue;
			}

			bool late = (next->state->countPieces() < 7);
			int depth = NUM_DEPTH + ((int)late)*2;

			int score;
			if (next->state->getTurnColor() == WHITE) {
				score = alphaBetaMin(INT_MIN, INT_MAX, depth, newState, next->evaluator, moveGen);
				next->eval = score;
			}
			else {
				score = alphaBetaMax(INT_MIN, INT_MAX, depth, newState, next->evaluator, moveGen);
				next->eval = score;
			}

			//addToCache(newState, score, depth);
		}
	}
}
//#define SINGLE_THREADED

void LaunchAndWaitWorkers() {
	unsigned nthreads = std::thread::hardware_concurrency();
	if (nthreads == 0) nthreads = 1;

#ifdef SINGLE_THREADED
	nthreads = 1;
#endif

	auto threads = new std::thread[nthreads];

	for (size_t i = 0; i < nthreads; i++) {
		threads[i] = std::thread(workerThread);
	}

	for (size_t i = 0; i < nthreads; i++) {
		threads[i].join();
	}

	delete[] threads;
}


static bool compareABRet(const AlphaBetaReturn& i, const AlphaBetaReturn& j) {
	if(i.state->getTurnColor()==WHITE)
		return (i.eval > j.eval); 
	else
		return (i.eval < j.eval);
}

static void printABRet(const AlphaBetaReturn& i) {
	std::wcout << moveToStr(i.move);
	std::wcout << "      ";
	std::wcout << i.eval;
	std::wcout << std::endl;
}


Move AlphaBetaAlgorithm::decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval) {
	currentGen++;
	pruneCacheOld();

	std::vector<AlphaBetaReturn> returnMoves;
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();
	auto mvs = moveGen->GenLegalMoves(state);
	if (mvs.empty()) {
		return Move();
	}

	State initial = State(state);

	mvs.sort();
	for each (auto move in mvs)
	{
		AlphaBetaReturn abr(&initial, &game, move, eval);
		returnMoves.push_back(abr);
	}

	for (int i = 0; i < returnMoves.size(); i++) {
		workQueue.push(&returnMoves[i]);
	}

	LaunchAndWaitWorkers();

	std::sort(returnMoves.begin(), returnMoves.end(), compareABRet);

	for (int i = 0; i < 6; i++) {
		if (i >= returnMoves.size()) break;
		printABRet(returnMoves[i]);
	}

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