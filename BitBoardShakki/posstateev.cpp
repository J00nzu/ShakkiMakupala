#include "algo.h"
#include "enums.h"

static int pawnRaw[] = {
	0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	50,  50, 10, 30, 30, 10,  50,  50,
	0,  0,  0, 20, 20,  0,  0,  0,
	50, -50,-10,  0,  0,-10, -50,  50,
	50, 10, 10,-20,-20, 10, 10,  50,
	0,  0,  0,  0,  0,  0,  0,  0
};

static int knightRaw[] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-30,  0, 10, 10, 10, 10,  0,-30,
	-30,  50, 10, 20, 20, 10,  50,-30,
	-30,  0, 10, 20, 20, 10,  0,-30,
	-30,  50, 10, 10, 10, 10,  50,-30,
	-40,-20,  0,  50,  50,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50
};

static int bishopRaw[] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  50, 10, 10,  50,  0,-10,
	-10,  50,  50, 10, 10,  50,  50,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  50,  0,  0,  0,  0,  50,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};

static int queenRaw[] = {
	-20,-10,-10, -50, -50,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  50,  50,  50,  50,  0,-10,
	-50,  0,  50,  50,  50,  50,  0, -50,
	0,  0,  50,  50,  50,  50,  0, -50,
	-10,  50,  50,  50,  50,  50,  0,-10,
	-10,  0,  50,  0,  0,  0,  0,-10,
	-20,-10,-10, -50, -50,-10,-10,-20
};

static int rookRaw[] = {
	0,  0,  0,  0,  0,  0,  0,  0,
	50, 10, 10, 10, 10, 10, 10,  50,
	-50,  0,  0,  0,  0,  0,  0, -50,
	-50,  0,  0,  0,  0,  0,  0, -50,
	-50,  0,  0,  0,  0,  0,  0, -50,
	-50,  0,  0,  0,  0,  0,  0, -50,
	-50,  0,  0,  0,  0,  0,  0, -50,
	0,  0,  50,  0,  0,  50,  0,  0
};

static int kingMidRaw[] = {
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	20, 20,  0,  0,  0,  0, 20, 20,
	20, 30, 10,  0,  0, 10, 30, 20
};

static int kingLateRaw[] = {
	-50,-40,-30,-20,-20,-30,-40,-50,
	-30,-20,-10,  0,  0,-10,-20,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-50,-30,-30,-30,-30,-30,-30,-50
};

static int eval_pawns[2][64];
static int eval_knights[2][64];
static int eval_bishops[2][64];
static int eval_rooks[2][64];
static int eval_queens[2][64];
static int eval_kings_mid[2][64];
static int eval_kings_late[2][64];


void fillArrays(const int* raw, int dest[2][64]) {
	for (int i = 0; i < 64;i++) {
		dest[BLACK][i] = raw[i];
		dest[WHITE][i] = raw[63-i];
	}	
}


PositionalStateEvaluator::PositionalStateEvaluator() {
	fillArrays(pawnRaw, eval_pawns);
	fillArrays(knightRaw, eval_knights);
	fillArrays(rookRaw, eval_rooks);
	fillArrays(bishopRaw, eval_bishops);
	fillArrays(queenRaw, eval_queens);
	fillArrays(kingMidRaw, eval_kings_mid);
	fillArrays(kingLateRaw, eval_kings_late);

}

#define VAL_PAWN 100
#define VAL_KNIGHT 350
#define VAL_ROOK 400
#define VAL_BISHOP 500
#define VAL_QUEEN 1500
#define VAL_KING 9000


int evalPawns(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wpawn];
	U64 blacks = state.pieceBB[Bpawn];
	const int val = VAL_PAWN;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_pawns[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_pawns[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}

int evalKnights(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wknight];
	U64 blacks = state.pieceBB[Bknight];
	const int val = VAL_KNIGHT;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_knights[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_knights[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}

int evalRooks(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wrook];
	U64 blacks = state.pieceBB[Brook];
	const int val = VAL_ROOK;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_rooks[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_rooks[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}

int evalBishops(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wbishop];
	U64 blacks = state.pieceBB[Bbishop];
	const int val = VAL_BISHOP;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_bishops[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_bishops[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}

int evalQueens(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wqueen];
	U64 blacks = state.pieceBB[Bqueen];
	const int val = VAL_QUEEN;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_queens[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_queens[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}

int evalKingMid(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wking];
	U64 blacks = state.pieceBB[Bking];
	const int val = VAL_KING;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_kings_mid[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_kings_mid[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}

int evalKingLate(const State& state) {
	int eval = 0;
	U64 whites = state.pieceBB[Wking];
	U64 blacks = state.pieceBB[Bking];
	const int val = VAL_KING;

	while (whites) {
		int posIdx = bitB_bitScanForward(whites);
		U64 white = 1ULL << posIdx;
		eval += val + eval_kings_late[WHITE][posIdx];
		whites ^= white;
	}

	while (blacks) {
		int posIdx = bitB_bitScanForward(blacks);
		U64 black = 1ULL << posIdx;
		eval -= val + eval_kings_late[BLACK][posIdx];
		blacks ^= black;
	}
	return eval;
}


int PositionalStateEvaluator::evaluate(const State& state) const {
	int eval = 0;
	bool late = ((state.pieceBB[Wqueen] | state.pieceBB[Bqueen]) == 0);

	eval += evalPawns(state);
	eval += evalKnights(state);
	eval += evalRooks(state);
	eval += evalBishops(state);
	eval += evalQueens(state);
	if (late) {
		eval += evalKingLate(state);
	}
	else {
		eval += evalKingMid(state);
	}

	return eval;
}
