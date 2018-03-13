#include "algo.h"
#include "movegen.h"
#include "bitboardops.h"

int evaluateAttackMap(const State& state, U64 aMap, COLOR color) {
	double aMapRes = 0;

	if (color == WHITE) {
		aMapRes += bitB_popCount(aMap) / 8.0;
		aMapRes += bitB_popCount(aMap & state.occupiedBB) / 2.0;
	}
	else {
		aMapRes -= bitB_popCount(aMap) / 8.0;
		aMapRes -= bitB_popCount(aMap & state.occupiedBB) / 2.0;
	}

	aMapRes /= 2;
	return (int)aMapRes;
}

int MapStateEvaluator::evaluate(const State& state) const {
	int eval = PositionalStateEvaluator::evaluate(state);

	U64 wAMap = MoveGenerator::getInstance().DangerSquares(state, WHITE);
	U64 bAMap = MoveGenerator::getInstance().DangerSquares(state, BLACK);

	eval += evaluateAttackMap(state, wAMap, WHITE);
	eval += evaluateAttackMap(state, bAMap, BLACK);

	return eval;
}