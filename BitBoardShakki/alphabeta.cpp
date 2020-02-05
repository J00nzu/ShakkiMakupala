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
#define NUM_DEPTH 4
#else
#define NUM_DEPTH 6
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

static int evaluateForMovingSide(const State& state, const StateEvaluator* eval) {
	if (state.getTurnColor() == WHITE) {
		return eval->evaluate(state);
	}
	else {
		return -eval->evaluate(state);
	}
}

static int quiesce(int alpha, int beta, const State& state, const StateEvaluator* eval) {
	int stand_pat = evaluateForMovingSide(state, eval);

	if (stand_pat >= beta)
		return beta;
	if (alpha < stand_pat)
		alpha = stand_pat;

	auto mvs = getMoves(state);
	for each(auto move in mvs) {
		if (!move.isCapture()) {
			continue;
		}
		State newState = state.advanceTurn(move);
		int score = -quiesce(-beta, -alpha, newState, eval);

		if (score >= beta)
			return beta;
		if (score > alpha)
			alpha = score;
	}
	return alpha;
}


static int alphaBeta(int alpha, int beta, int depthleft, const State& state, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	//if (state.pieceBB[Wking] == 0) return INT_MIN;
	if (depthleft == 0) return quiesce(alpha, beta, state, eval);
	if (state.pieceBB[(state.getTurnColor() == WHITE ? Wking : Bking)] == 0) {
		return -999999 - depthleft;
	}
	
	CacheItem cacheit;
	// if (checkCache(state, cacheit) && depthleft <= cacheit.depth) {
	if (false) {
		/*
		if (cacheit.type == CacheItem::EXACT_VALUE) // stored value is exact
			return cacheit.evaluation;
		if (cacheit.type == CacheItem::LOWERBOUND && cacheit.evaluation > alpha)
			alpha = cacheit.evaluation; // update lowerbound alpha if needed
		else if (cacheit.type == CacheItem::UPPERBOUND && cacheit.evaluation < beta)
			beta = cacheit.evaluation; // update upperbound beta if needed
		if (alpha >= beta)
			return cacheit.evaluation; // if lowerbound surpasses upperbound

		Move move = cacheit.bestMove;
		State newState = state.advanceTurn(move);
		int score = -alphaBeta(-beta, -alpha, depthleft - 1, newState, eval, moveGen);

		return score;*/ return 0;
	}
	else {
		int best = INT_MIN + 1;
		Move bestMove;

		auto mvs = getMoves(state);
		for each(auto move in mvs) {
			State newState = state.advanceTurn(move);
			int score = -alphaBeta(-beta, -alpha, depthleft - 1, newState, eval, moveGen);
			if (score > best) {
				bestMove = move;
				best = score;
			}
			if (best > alpha)
				alpha = best;
			if (best >= beta) {
				break;
			}
		}
		/*
		if (best <= alpha) // a lowerbound value
			//StoreTTEntry(board.getHashKey(), best, LOWERBOUND, depth);
			addToCache(state, best, depthleft, CacheItem::LOWERBOUND, bestMove);
		else if (best >= beta) // an upperbound value
			//StoreTTEntry(board.getHashKey(), best, UPPERBOUND, depth);
			addToCache(state, best, depthleft, CacheItem::UPPERBOUND, bestMove);
		else // a true minimax value
			 //StoreTTEntry(board.getHashKey(), best, EXACT, depth);
			addToCache(state, best, depthleft, CacheItem::EXACT_VALUE, bestMove);
			*/
		return best;
	}
}


/*
int alphaBetaMin(int alpha, int beta, int depthleft, const State& state, const StateEvaluator* eval, const MoveGenerator* moveGen) {
	//if (state.pieceBB[Bking] == 0) return INT_MAX;
	if (depthleft == 0) return eval->evaluate(state);


	
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
}*/




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
			bool queens = ((next->state->pieceBB[Wqueen] | next->state->pieceBB[Bqueen]) != 0);
			int depth = NUM_DEPTH + ((int)late)*2 - ((int)queens);

			int score;
			score = -alphaBeta(-INT_MAX, INT_MAX, depth-1, newState, next->evaluator, moveGen);
			next->eval = score;

			addToCache(newState, score, depth - 1, CacheItem::EXACT_VALUE);
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
	return (i.eval > j.eval);
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

	for (int i = 0; i < 10; i++) {
		if (i >= returnMoves.size()) break;
		printABRet(returnMoves[i]);
	}

	AlphaBetaReturn* bestMove = &returnMoves[0];


	return bestMove->move;
}