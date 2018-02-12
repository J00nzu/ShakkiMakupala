#include "movegen.h"
#include "bitboardops.h"
#include "magic.h"
#include "convert.h"
#include <iostream>

static U64 arrKnightMoves[64];
static U64 arrKingMoves[64];
static U64 arrPawnAttacks[2][64];

static U64 notAFile = ~(files[0]);
static U64 notABFile = ~(files[0] | files[1]);
static U64 notHFile = ~(files[7]);
static U64 notGHFile = ~(files[7] | files[6]);

U64 noNoEa(U64 b) { return (b << 17) & notAFile; }
U64 noEaEa(U64 b) { return (b << 10) & notABFile; }
U64 soEaEa(U64 b) { return (b >> 6) & notABFile; }
U64 soSoEa(U64 b) { return (b >> 15) & notAFile; }
U64 noNoWe(U64 b) { return (b << 15) & notHFile; }
U64 noWeWe(U64 b) { return (b << 6) & notGHFile; }
U64 soWeWe(U64 b) { return (b >> 10) & notGHFile; }
U64 soSoWe(U64 b) { return (b >> 17) & notHFile; }

void genStaticKnightMoves() {
	for (int i = 0; i < 64; i++) {
		BITPOS sPos = (BITPOS)(1ULL << i);

		U64 endPoses = 0;
		endPoses |= noNoEa(sPos);
		endPoses |= noEaEa(sPos);
		endPoses |= soEaEa(sPos);
		endPoses |= soSoEa(sPos);
		endPoses |= noNoWe(sPos);
		endPoses |= noWeWe(sPos);
		endPoses |= soWeWe(sPos);
		endPoses |= soSoWe(sPos);

		arrKnightMoves[i] = endPoses;
	}
}

void genStaticKingMoves() {
	for (int i = 0; i < 64; i++) {
		BITPOS sPos = (BITPOS)(1ULL << i);

		U64 endPoses = 0;
		for (int dir = 0; dir < 8; dir++) {
			endPoses |= bitB_move(sPos, (DIR8)dir);
		}
		arrKingMoves[i] = endPoses;
	}
}

void genStaticPawnAttacks() {
	//WHITE
	for (int i = 0; i < 64; i++) {
		BITPOS sPos = (BITPOS)(1ULL << i);
		U64 atcks = 0 | bitB_move(sPos, noWe) | bitB_move(sPos, noEa);
		arrPawnAttacks[WHITE][i] = atcks;
	}
	//BLACK
	for (int i = 0; i < 64; i++) {
		BITPOS sPos = (BITPOS)(1ULL << i);
		U64 atcks = 0 | bitB_move(sPos, soWe) | bitB_move(sPos, soEa);
		arrPawnAttacks[BLACK][i] = atcks;
	}
}

MoveGenerator::MoveGenerator() {
	genStaticKnightMoves();
	genStaticKingMoves();
	genStaticPawnAttacks();
}


//##################### PAWN STUFF ############################
#pragma region pawnStuff

U64 wSinglePushTargets(U64 wpawns, U64 empty) {
	return bitB_nortOne(wpawns) & empty;
}

U64 wDblPushTargets(U64 wpawns, U64 empty) {
	const U64 rank4 = U64(0x00000000FF000000);
	U64 singlePushs = wSinglePushTargets(wpawns, empty);
	return bitB_nortOne(singlePushs) & empty & rank4;
}

U64 bSinglePushTargets(U64 bpawns, U64 empty) {
	return bitB_soutOne(bpawns) & empty;
}

U64 bDblPushTargets(U64 bpawns, U64 empty) {
	const U64 rank5 = U64(0x000000FF00000000);
	U64 singlePushs = bSinglePushTargets(bpawns, empty);
	return bitB_soutOne(singlePushs) & empty & rank5;
}

void FilterAndAddPawnPromoPush(MoveSet& list, PIECE piece, COLOR col, BITPOS startPos, BITPOS endPos) {
	U64 rankp = 0;
	if (col == WHITE) {
		rankp = U64(0xFF00000000000000);
	}
	else {
		rankp = U64(0x00000000000000FF);
	}

	if (endPos & rankp) {
		Move qp(startPos, endPos, piece, Move::PROMOTION_QUEEN);
		Move np(startPos, endPos, piece, Move::PROMOTION_KNIGHT);
		Move rp(startPos, endPos, piece, Move::PROMOTION_ROOK);
		Move bp(startPos, endPos, piece, Move::PROMOTION_BISHOP);
		list.push_back(qp);
		list.push_back(np);
		list.push_back(rp);
		list.push_back(bp);
	}
	else {
		Move move(startPos, endPos, piece, Move::QUIET);
		list.push_back(move);
	}
}

void FilterAndAddPawnPromoCapture(MoveSet& list, PIECE piece, COLOR col, BITPOS startPos, BITPOS endPos, PIECE capturedPiece) {
	U64 rankp = 0;
	if (col == WHITE) {
		rankp = U64(0xFF00000000000000);
	}
	else {
		rankp = U64(0x00000000000000FF);
	}

	if (endPos & rankp) {
		Move qp(startPos, endPos, piece, Move::CAPTURE_PROMOTION_QUEEN, capturedPiece);
		Move np(startPos, endPos, piece, Move::CAPTURE_PROMOTION_KNIGHT, capturedPiece);
		Move rp(startPos, endPos, piece, Move::CAPTURE_PROMOTION_ROOK, capturedPiece);
		Move bp(startPos, endPos, piece, Move::CAPTURE_PROMOTION_BISHOP, capturedPiece);
		list.push_back(qp);
		list.push_back(np);
		list.push_back(rp);
		list.push_back(bp);
	}
	else {
		Move move(startPos, endPos, piece, Move::CAPTURE, capturedPiece);
		list.push_back(move);
	}
}


void GenPawnPushes(MoveSet& list, const State& state, PIECE piece, COLOR col) {
	U64 empty = state.emptyBB;

	U64 pawns = state.pieceBB[piece];

	U64 singleEndPoses = (col == WHITE ? (wSinglePushTargets(pawns, empty)) : (bSinglePushTargets(pawns, empty)));
	U64 dblEndPoses = (col == WHITE ? (wDblPushTargets(pawns, empty)) : (bDblPushTargets(pawns, empty)));

	while (singleEndPoses) {
		U64 endPos = 1ULL << bitB_bitScanForward(singleEndPoses);
		U64 startPos = (col == WHITE ? bitB_soutOne(endPos): bitB_nortOne(endPos));

		FilterAndAddPawnPromoPush(list, piece, col, (BITPOS)startPos, (BITPOS)endPos);

		singleEndPoses ^= endPos;
	}

	while (dblEndPoses) {
		U64 endPos = 1ULL << bitB_bitScanForward(dblEndPoses);
		U64 startPos = (col == WHITE ? endPos >> 16 : endPos << 16);

		Move m((BITPOS)startPos, (BITPOS)endPos, piece, Move::PAWN_DOUBLE_PUSH);
		list.push_back(m);

		dblEndPoses ^= endPos;
	}
}

void GenPawnAttacks(MoveSet& list, const State& state, PIECE piece, COLOR col) {
	U64 enemyPieces = state.colorBB[!col];

	U64 pawns = state.pieceBB[piece];

	while (pawns) {
		int stPosIdx = bitB_bitScanForward(pawns);
		U64 startPos = 1ULL << stPosIdx;
		U64 endPoses = arrPawnAttacks[col][stPosIdx];

		while (endPoses) {
			U64 endPos = 1ULL << bitB_bitScanForward(endPoses);

			if (col == WHITE) {
				for (int i = 6; i < 12; i++) {
					if (CheckFlag(state.pieceBB[i], endPos)) {
						PIECE capP = (PIECE)i;
						FilterAndAddPawnPromoCapture(list, piece, col, (BITPOS)startPos, (BITPOS)endPos, capP);
						break;
					}
				}
			}
			else {
				for (int i = 0; i < 6; i++) {
					if (CheckFlag(state.pieceBB[i], endPos)) {
						PIECE capP = (PIECE)i;
						FilterAndAddPawnPromoCapture(list, piece, col, (BITPOS)startPos, (BITPOS)endPos, capP);
						break;
					}
				}
			}

			endPoses ^= endPos;
		}

		pawns ^= startPos;
	}
}

void GenPawnEP(MoveSet& list, const State& state, PIECE piece, COLOR col, const Move& lastOpponentMove) {
	if (lastOpponentMove.isDoublePush()) {
		BITPOS opponentPos = lastOpponentMove.getToBB();
		U64 availableEP = 0 | bitB_eastOne(opponentPos) | bitB_westOne(opponentPos);
		U64 startPoses = state.pieceBB[piece] & availableEP;
		while (startPoses) {
			int stPosIdx = bitB_bitScanForward(startPoses);
			U64 startPos = 1ULL << stPosIdx;
			if (col == WHITE) {
				U64 endPos = bitB_nortOne(opponentPos);
				Move mv((BITPOS)startPos, (BITPOS)endPos, piece, Move::ENPASSANT_CAP, Bpawn);
				list.push_back(mv);
			}
			else {
				U64 endPos = bitB_soutOne(opponentPos);
				Move mv((BITPOS)startPos, (BITPOS)endPos, piece, Move::ENPASSANT_CAP, Wpawn);
				list.push_back(mv);
			}
			startPoses ^= startPos;
		}
	}
}

void GenPawnMoves(MoveSet& list, const State& state, const Move& lastOpponentMove) {
	COLOR col = state.getTurnColor();
	PIECE p = col == WHITE ? Wpawn : Bpawn;

	GenPawnPushes(list, state, p, col);
	GenPawnAttacks(list, state, p, col);
	GenPawnEP(list, state, p, col, lastOpponentMove);
}





#pragma endregion
//################## PAWN STUFF END ###########################

inline void AddMoves(MoveSet& list, const State& state, PIECE piece, COLOR col, U64 startPos, U64 endMovePoses) {
	while (endMovePoses) {
		int ePosIdx = bitB_bitScanForward(endMovePoses);
		U64 endPos = 1ULL << ePosIdx;

		Move mv((BITPOS)startPos, (BITPOS)endPos, piece, Move::QUIET);
		list.push_back(mv);

		endMovePoses ^= endPos;
	}
}

inline void AddCaps(MoveSet& list, const State& state, PIECE piece, COLOR col, U64 startPos, U64 endCapPoses) {
	while (endCapPoses) {
		int ePosIdx = bitB_bitScanForward(endCapPoses);
		U64 endPos = 1ULL << ePosIdx;

		const int min = col == WHITE ? 6 : 0;
		const int max = col == WHITE ? 12 : 6;

		for (int i = min; i < max; i++) {
			if (CheckFlag(state.pieceBB[i], endPos)) {
				PIECE capP = (PIECE)i;
				Move mv((BITPOS)startPos, (BITPOS)endPos, piece, Move::CAPTURE, capP);
				list.push_back(mv);
				break;
			}
		}
		endCapPoses ^= endPos;
	}
}

void GenKnightMoves(MoveSet& list, const State& state) {
	COLOR col = state.getTurnColor();
	PIECE p = col == WHITE ? Wknight : Bknight;

	U64 startPoses = state.pieceBB[p];
	while (startPoses) {
		int sPosIdx = bitB_bitScanForward(startPoses);
		U64 startPos = 1ULL << sPosIdx;
		U64 knightMoves = arrKnightMoves[sPosIdx];
		
		U64 endMovePoses = knightMoves & state.emptyBB;
		AddMoves(list, state, p, col, startPos, endMovePoses);

		U64 endCapPoses = knightMoves & state.colorBB[!col];
		AddCaps(list, state, p, col, startPos, endCapPoses);

		startPoses ^= startPos;
	}
}

void GenCastling(MoveSet& list, const State& state, COLOR col, PIECE p, U64 dangerSquares) {
	if (col == WHITE) {
		if (state.getWhiteShortCastlingAllowed()) {
			static const U64 emptyMask = U64(0x000000000000060ULL);
			if (!(emptyMask & dangerSquares) && !(emptyMask & state.occupiedBB)) {
				Move mv(bE1, bG1, p, Move::KING_CASTLE);
				list.push_back(mv);
			}
		}
		if (state.getWhiteLongCastlingAllowed()) {
			static const U64 emptyMask = U64(0x000000000000000EULL);
			static const U64 dangerMask = U64(0x000000000000000CULL);
			if (!(dangerMask & dangerSquares) && !(emptyMask & state.occupiedBB)) {
				Move mv(bE1, bC1, p, Move::QUEEN_CASTLE);
				list.push_back(mv);
			}
		}
	}
	else {
		if (state.getBlackShortCastlingAllowed()) {
			static const U64 emptyMask = U64(0x6000000000000000ULL);
			if (!(emptyMask & dangerSquares) && !(emptyMask & state.occupiedBB)) {
				Move mv(bE8, bG8, p, Move::KING_CASTLE);
				list.push_back(mv);
			}
		}
		if (state.getBlackLongCastlingAllowed()) {
			static const U64 emptyMask = U64(0x0E00000000000000ULL);
			static const U64 dangerMask = U64(0x0C00000000000000ULL);
			if (!(dangerMask & dangerSquares) && !(emptyMask & state.occupiedBB)) {
				Move mv(bE8, bC8, p, Move::QUEEN_CASTLE);
				list.push_back(mv);
			}
		}
	}
}


void GenKingMoves(MoveSet& list, const State& state, U64 dangerSquares) {
	COLOR col = state.getTurnColor();
	PIECE p = col == WHITE ? Wking : Bking;

	U64 startPoses = state.pieceBB[p];
	while (startPoses) {
		int sPosIdx = bitB_bitScanForward(startPoses);
		U64 startPos = 1ULL << sPosIdx;
		U64 kingMoves = arrKingMoves[sPosIdx];

		U64 endMovePoses = kingMoves & state.emptyBB & (~dangerSquares);
		AddMoves(list, state, p, col, startPos, endMovePoses);

		U64 endCapPoses = kingMoves & state.colorBB[!col];
		AddCaps(list, state, p, col, startPos, endCapPoses);

		startPoses ^= startPos;
	}
	//TODO danger squares
	GenCastling(list, state, col, p, dangerSquares);
}


void GenRookMoves(MoveSet& list, const State& state) {
	COLOR col = state.getTurnColor();
	PIECE p = col == WHITE ? Wrook : Brook;
	magic::MagicSlideGenerator *slideGen = &magic::MagicSlideGenerator::getInstance();

	U64 startPoses = state.pieceBB[p];
	U64 occupied = state.occupiedBB;
	U64 empty = state.emptyBB;
	U64 enemyPieces = state.colorBB[!col];

	while (startPoses) {
		int sPosIdx = bitB_bitScanForward(startPoses);
		U64 startPos = 1ULL << sPosIdx;
		U64 endPoses = slideGen->RookAttacks(occupied, sPosIdx) & (empty | enemyPieces);

		U64 endMovePoses = endPoses & empty;
		AddMoves(list, state, p, col, startPos, endMovePoses);

		U64 endCapPoses = endPoses & enemyPieces;
		AddCaps(list, state, p, col, startPos, endCapPoses);

		startPoses ^= startPos;
	}
}


void GenBishopMoves(MoveSet& list, const State& state) {
	COLOR col = state.getTurnColor();
	PIECE p = col == WHITE ? Wbishop : Bbishop;
	magic::MagicSlideGenerator *slideGen = &magic::MagicSlideGenerator::getInstance();

	U64 startPoses = state.pieceBB[p];
	U64 occupied = state.occupiedBB;
	U64 empty = state.emptyBB;
	U64 enemyPieces = state.colorBB[!col];

	while (startPoses) {
		int sPosIdx = bitB_bitScanForward(startPoses);
		U64 startPos = 1ULL << sPosIdx;
		U64 endPoses = slideGen->BishopAttacks(occupied, sPosIdx) & (empty | enemyPieces);

		U64 endMovePoses = endPoses & empty;
		AddMoves(list, state, p, col, startPos, endMovePoses);

		U64 endCapPoses = endPoses & enemyPieces;
		AddCaps(list, state, p, col, startPos, endCapPoses);

		startPoses ^= startPos;
	}
}

void GenQueenMoves(MoveSet& list, const State& state) {
	COLOR col = state.getTurnColor();
	PIECE p = col == WHITE ? Wqueen : Bqueen;
	magic::MagicSlideGenerator *slideGen = &magic::MagicSlideGenerator::getInstance();

	U64 startPoses = state.pieceBB[p];
	U64 occupied = state.occupiedBB;
	U64 empty = state.emptyBB;
	U64 enemyPieces = state.colorBB[!col];

	while (startPoses) {
		int sPosIdx = bitB_bitScanForward(startPoses);
		U64 startPos = 1ULL << sPosIdx;
		U64 endPoses = slideGen->QueenAttacks(occupied, sPosIdx) & (empty | enemyPieces);

		U64 endMovePoses = endPoses & empty;
		AddMoves(list, state, p, col, startPos, endMovePoses);

		U64 endCapPoses = endPoses & enemyPieces;
		AddCaps(list, state, p, col, startPos, endCapPoses);

		startPoses ^= startPos;
	}
}


MoveSet MoveGenerator::GenPseudoLegalMoves(const State& state, const Move& lastOpponentMove , U64 dangerSquares) const{
	MoveSet list;

	GenPawnMoves(list, state, lastOpponentMove);
	GenKnightMoves(list, state);
	GenRookMoves(list, state);
	GenBishopMoves(list, state);
	GenQueenMoves(list, state);
	GenKingMoves(list, state, dangerSquares);

	return list;
}

#pragma region Danger

U64 GenDangerSquaresPawn(const State& state, COLOR col) {
	U64 danger = 0;
	PIECE p = col == WHITE ? Wpawn : Bpawn;

	U64 sPoses = state.pieceBB[p];
	if (col == WHITE) {
		danger |= bitB_noEaOne(sPoses) | bitB_noWeOne(sPoses);
	}else {
		danger |= bitB_soEaOne(sPoses) | bitB_soWeOne(sPoses);
	}
	return danger;
}
U64 GenDangerSquaresKnight(const State& state, COLOR col) {
	U64 danger = 0;
	PIECE p = col == WHITE ? Wknight : Bknight;
	U64 sPoses = state.pieceBB[p];
	while (sPoses) {
		int sposIdx = bitB_bitScanForward(sPoses);
		U64 sPos = 1ULL << sposIdx;

		danger |= arrKnightMoves[sposIdx];

		sPoses ^= sPos;
	}
	return danger;
}

U64 GenDangerSquaresKing(const State& state, COLOR col) {
	U64 danger = 0;
	PIECE p = col == WHITE ? Wking : Bking;
	U64 sPoses = state.pieceBB[p];
	while (sPoses) {
		int sposIdx = bitB_bitScanForward(sPoses);
		U64 sPos = 1ULL << sposIdx;

		danger |= arrKingMoves[sposIdx];

		sPoses ^= sPos;
	}
	return danger;
}

U64 GenDangerSquaresRook(const State& state, COLOR col, U64 occupied) {
	U64 danger = 0;
	PIECE p = col == WHITE ? Wrook : Brook;
	magic::MagicSlideGenerator *slideGen = &magic::MagicSlideGenerator::getInstance();
	U64 sPoses = state.pieceBB[p];
	while (sPoses) {
		int sposIdx = bitB_bitScanForward(sPoses);
		U64 sPos = 1ULL << sposIdx;

		danger |= slideGen->RookAttacks(occupied, sposIdx);

		sPoses ^= sPos;
	}
	return danger;
}

U64 GenDangerSquaresBishop(const State& state, COLOR col, U64 occupied) {
	U64 danger = 0;
	PIECE p = col == WHITE ? Wbishop : Bbishop;
	magic::MagicSlideGenerator *slideGen = &magic::MagicSlideGenerator::getInstance();
	U64 sPoses = state.pieceBB[p];
	while (sPoses) {
		int sposIdx = bitB_bitScanForward(sPoses);
		U64 sPos = 1ULL << sposIdx;

		danger |= slideGen->BishopAttacks(occupied, sposIdx);

		sPoses ^= sPos;
	}
	return danger;
}

U64 GenDangerSquaresQueen(const State& state, COLOR col, U64 occupied) {
	U64 danger = 0;
	PIECE p = col == WHITE ? Wqueen : Bqueen;
	magic::MagicSlideGenerator *slideGen = &magic::MagicSlideGenerator::getInstance();
	U64 sPoses = state.pieceBB[p];
	while (sPoses) {
		int sposIdx = bitB_bitScanForward(sPoses);
		U64 sPos = 1ULL << sposIdx;

		danger |= slideGen->QueenAttacks(occupied, sposIdx);

		sPoses ^= sPos;
	}
	return danger;
}

U64 GenDangerSquares(const State& state, COLOR eCol) {
	U64 danger = 0;
	U64 occupied = state.occupiedBB ^ state.pieceBB[(eCol == WHITE ? Bking : Wking)];
	danger |= GenDangerSquaresPawn(state, eCol);
	danger |= GenDangerSquaresKing(state, eCol);
	danger |= GenDangerSquaresKnight(state, eCol);
	danger |= GenDangerSquaresBishop(state, eCol, occupied);
	danger |= GenDangerSquaresRook(state, eCol, occupied);
	danger |= GenDangerSquaresQueen(state, eCol, occupied);

	return danger;
}

#pragma endregion

void pruneOutIllegalMoves(MoveSet& list, const State& state) {

	int it = 0;

	while (it < list.size()) {
		Move move = list[it];
		bool skipIt = false;
		auto newState = state.advanceTurn(move);

		U64 dangerSquares = GenDangerSquares(newState, (COLOR)(!state.getTurnColor()));
		U64 kingPos = newState.pieceBB[(state.getTurnColor() == WHITE ? Wking : Bking)];

		if (CheckFlag(dangerSquares, kingPos)) {
			list.erase(it);
			skipIt = true;
		}

		if (!skipIt) ++it;
	}

	
}

MoveSet MoveGenerator::GenLegalMoves(const State& state, const Move& lastOpponentMove) const{
	U64 dangerSquares = GenDangerSquares(state, (COLOR)(!state.getTurnColor()));
	MoveSet list = GenPseudoLegalMoves(state, lastOpponentMove, dangerSquares);
	pruneOutIllegalMoves(list, state);

	return list;
}