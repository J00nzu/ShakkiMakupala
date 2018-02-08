#include "algo.h"

int DefaultStateEvaluator::evaluate(const State& state) const {
	int score = 0;
	
	score += bitB_popCount(state.pieceBB[Wpawn]) * 10;
	score += bitB_popCount(state.pieceBB[Wknight]) * 30;
	score += bitB_popCount(state.pieceBB[Wrook]) * 40;
	score += bitB_popCount(state.pieceBB[Wbishop]) * 30;
	score += bitB_popCount(state.pieceBB[Wqueen]) * 90;
	score += bitB_popCount(state.pieceBB[Wking]) * 900;

	score -= bitB_popCount(state.pieceBB[Bpawn]) * 10;
	score -= bitB_popCount(state.pieceBB[Bknight]) * 30;
	score -= bitB_popCount(state.pieceBB[Brook]) * 40;
	score -= bitB_popCount(state.pieceBB[Bbishop]) * 30;
	score -= bitB_popCount(state.pieceBB[Bqueen]) * 90;
	score -= bitB_popCount(state.pieceBB[Bking]) * 900;

	return score;
}