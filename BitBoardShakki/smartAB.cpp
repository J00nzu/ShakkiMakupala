#include "algo.h"
#include "caching.h"
#include "convert.h"
#include "movegen.h"
#include <iostream>


struct SmartAlphaBetaReturn {
	int eval = 0;
	int depthReached = 0;
	int evaluatedPositions = 0;
	int cacheHits = 0;
	Move move;
	const State* state;
	const StateEvaluator* evaluator;
	const Game* game;

	SmartAlphaBetaReturn() {}
	SmartAlphaBetaReturn(const State* st, const Game* g, const Move& m, const StateEvaluator* e) {
		move = m;
		state = st;
		evaluator = e;
		game = g;

		eval = INT_MIN;
	}
};

static std::mutex queueMutex;
static std::queue<SmartAlphaBetaReturn*> workQueue;

static MoveSet getMoves(const State& state, int depth = 1) {
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();
	MoveSet moves;
	if (depth < 4) {
		moves = moveGen->GenLegalMoves(state);
	}
	else {
		moves = moveGen->GenPseudoLegalMoves(state);
	}
	moves.sort();
	return moves;
}

int evaluateForMovingSide(const State& state, const StateEvaluator* eval) {
	if (state.getTurnColor() == WHITE) {
		return eval->evaluate(state);
	}
	else {
		return -eval->evaluate(state);
	}
}

int quiesce(int alpha, int beta, const State& state, const StateEvaluator* eval) {
	int stand_pat = evaluateForMovingSide(state, eval);
	//test
	return stand_pat;

	if (stand_pat >= beta)
		return beta;
	if (alpha < stand_pat)
		alpha = stand_pat;

	auto mvs = getMoves(state, 10);
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

int reduceExtend(int depth, const Move& move, const State& state) {
	if (depth < 3) {
		return 0;
	}
	if (state.inCheck()) {
		return 2;
	}
	if (move.isCapture() || move.isPromotion()) {
		return 1;
	}
	return -1;
}

int alphaBeta(int alpha, int beta, int depthleft, int depth, int maxdepth, int* depthReached, int* positionsEvaluated, int* cacheHits, const State& state, const StateEvaluator* eval, const ThreadProcessingClock* clock) {
	if (depth > *depthReached) *depthReached = depth;
	(*positionsEvaluated)++;
	if (state.pieceBB[(state.getTurnColor() == WHITE ? Wking : Bking)] == 0) {
		return INT_MIN + 1 + depth;
	}
	
	CacheItem cacheit;
	if (checkCache(state, cacheit) && depthleft <= cacheit.depth) {
		(*cacheHits)++;
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
		int newdepth = depthleft + reduceExtend(depth + 1, move, newState) - 1;
		int score = -alphaBeta(-beta, -alpha, newdepth, depth + 1, maxdepth, depthReached, positionsEvaluated, cacheHits, newState, eval, clock);
	
		return score;
	}
	else {

		if (clock->isDone()) return evaluateForMovingSide(state, eval);
		if (depthleft <= 0 || depth >= maxdepth) {
			int score = quiesce(alpha, beta, state, eval);
			addToCache(state, score, 0, CacheItem::EXACT_VALUE);
			return score;
		}

		int best = INT_MIN + 1;
		Move bestMove;
		int oDepth = depthleft;

		auto mvs = getMoves(state, depth);
		if (mvs.empty()) {
			if (state.inCheck()) {
				return INT_MIN + 1 + depth; // checkmate
			}
			else {
				return 0; // stalemate - return a draw
			}
		}
		for each(auto move in mvs) {
			State newState = state.advanceTurn(move);

			int newdepth = depthleft + reduceExtend(depth + 1, move, newState) - 1;
			int score = -alphaBeta(-beta, -alpha, newdepth, depth + 1, maxdepth, depthReached, positionsEvaluated, cacheHits, newState, eval, clock);

			if (score > best) {
				bestMove = move;
				best = score;
			}
			if (best > alpha)
				alpha = best;
			if (best > beta) {
				beta = best;
				depthleft -= 4; // reduce search
			}
			if (depthleft < 0) {
				break;
			}
		}

		if (oDepth > 0) {
			if (best <= alpha) // a lowerbound value
				//StoreTTEntry(board.getHashKey(), best, LOWERBOUND, depth);
				addToCache(state, best, oDepth, CacheItem::LOWERBOUND, bestMove);
			else if (best >= beta) // an upperbound value
				//StoreTTEntry(board.getHashKey(), best, UPPERBOUND, depth);
				addToCache(state, best, oDepth, CacheItem::UPPERBOUND, bestMove);
			else // a true minimax value
				//StoreTTEntry(board.getHashKey(), best, EXACT, depth);
				addToCache(state, best, oDepth, CacheItem::EXACT_VALUE, bestMove);
		}


		return best;
	}
}

static bool compareABRetPtr(const SmartAlphaBetaReturn* i, const SmartAlphaBetaReturn* j) {
	return (i->eval > j->eval);
}

void alphaBetaRoot(SmartAlphaBetaReturn* next, ThreadProcessingClock* clock, int depth) {
int alpha = INT_MIN+1;
int beta = INT_MAX;
int extra_depth = 1;
	
	State newState = next->state->advanceTurn(next->move);
		
	if (next->game->stateOccurredTimes(newState) >= 2) {
		clock->join();
		return;
	}

	int depthReached = next->depthReached;
	int posEvaluated = next->evaluatedPositions;
	int cacheHits = next->cacheHits;
	int maxdepth = depth * 2;

	int score = -alphaBeta(-beta, -alpha, depth - 1, 1, maxdepth, &depthReached, &posEvaluated, &cacheHits, newState, next->evaluator, clock);
	if (clock->isDone() && depthReached < next->depthReached) return;

	next->eval = score;
	next->depthReached = depthReached;
	next->evaluatedPositions = posEvaluated;
	next->cacheHits = cacheHits;
	/*
	if (score > alpha)
		alpha = score;*/

		
	if(depth > 1){
		if (score <= alpha) // a lowerbound value
			//StoreTTEntry(board.getHashKey(), best, LOWERBOUND, depth);
			addToCache(newState, score, depth-1, CacheItem::LOWERBOUND);
		else if (score >= beta) // an upperbound value
			//StoreTTEntry(board.getHashKey(), best, UPPERBOUND, depth);
			addToCache(newState, score, depth-1, CacheItem::UPPERBOUND);
		else // a true minimax value
				//StoreTTEntry(board.getHashKey(), best, EXACT, depth);
			addToCache(newState, score, depth-1, CacheItem::EXACT_VALUE);
	}
}

void workerThread(MoveClock* clock) {
	ThreadProcessingClock tClock = clock->ReserveTimeForThread();

	SmartAlphaBetaReturn* next;
	queueMutex.lock();
	if (workQueue.empty()) {
		queueMutex.unlock();
		return;
	}
	else {
		next = workQueue.front();
		workQueue.pop();
		queueMutex.unlock();
	}

	tClock.start();
	int depth = 2;
	while (!tClock.isDone()) {
		alphaBetaRoot(next, &tClock, depth);
		depth+=2;
	}
}

//#define SINGLE_THREADED

void LaunchAndWaitWorkers(MoveClock* clock, int num_moves) {
	unsigned nthreads = num_moves;

	auto threads = new std::thread[nthreads];

	for (size_t i = 0; i < nthreads; i++) {
		threads[i] = std::thread(workerThread, clock);
	}

	for (size_t i = 0; i < nthreads; i++) {
		threads[i].join();
	}

	delete[] threads;
}

static bool compareABRet(const SmartAlphaBetaReturn& i, const SmartAlphaBetaReturn& j) {
	return (i.eval > j.eval);
}

static void printABRet(const SmartAlphaBetaReturn& i) {
	std::wcout << moveToStr(i.move);
	std::wcout << "  depth: ";
	std::wcout << i.depthReached;
	std::wcout << "  positions: ";
	std::wcout << i.evaluatedPositions;
	std::wcout << "  cachehits: ";
	std::wcout << i.cacheHits;
	std::wcout << "  eval: ";
	std::wcout << i.eval;
	std::wcout << std::endl;
}

static void printStats(const std::vector<SmartAlphaBetaReturn>& moves) {
	long long evaluatedPositions = 0;
	long long cacheHits = 0;
	long long evalSum = 0;
	for each(auto m in moves) {
		evaluatedPositions += m.evaluatedPositions;
		cacheHits += m.cacheHits;
		evalSum += m.eval;
	}
	long long eval = evalSum / moves.size();

	std::wcout << "Sum of positions: " << evaluatedPositions << " sum of cache hits: " << cacheHits << std::endl;
	std::wcout << "Evaluation average: " << evalSum << std::endl;
}

SmartAlphaBeta::SmartAlphaBeta(MoveClock clock) {
	_clock = clock;
}

Move SmartAlphaBeta::decideAMove(const State& state, const Game& game, const Move& lastOpponentMove, const StateEvaluator* eval) {
	currentGen++;
	pruneCacheOld();

	std::vector<SmartAlphaBetaReturn> returnMoves;
	static const MoveGenerator* moveGen = &MoveGenerator::getInstance();
	auto mvs = moveGen->GenLegalMoves(state);
	if (mvs.empty()) {
		return Move();
	}

	State initial = State(state);

	mvs.sort();
	for each (auto move in mvs)
	{
		SmartAlphaBetaReturn abr(&initial, &game, move, eval);
		returnMoves.push_back(abr);
	}

	for (int i = 0; i < returnMoves.size(); i++) {
		workQueue.push(&returnMoves[i]);
	}

	LaunchAndWaitWorkers(&_clock, returnMoves.size());

	std::sort(returnMoves.begin(), returnMoves.end(), compareABRet);

	//for (int i = 0; i < 6; i++) {
	for(int i=0; i< returnMoves.size(); i++){
		if (i >= returnMoves.size()) break;
		printABRet(returnMoves[i]);
	}
	printStats(returnMoves);

	SmartAlphaBetaReturn* bestMove = &returnMoves[0];

	return bestMove->move;
}