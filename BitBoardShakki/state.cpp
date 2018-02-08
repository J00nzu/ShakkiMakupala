
#include "statebb.h";
#include <cassert>

const Piece* State::getPiece(BITPOS pos) const{
	for (int i = 0; i < 12; i++) {
		if ((pieceBB[i] & pos) != 0) {
			return pieces[i];
		}
	}
	return nullptr;
}

int State::getRank(BITPOS pos) {
	return bitP_getRank(pos);
}


void DoCastling(State& newState, const Move& move) {
	COLOR col = move.getColor();
	PIECE king = (col == WHITE ? Wking : Bking);
	PIECE rook = (col == WHITE ? Wrook : Brook);
	bool shortCastle = (move.getMoveType() == Move::KING_CASTLE);
	if (shortCastle) {
		U64 kingFromTo = (col == WHITE ? (bE1 | bG1): (bE8 | bG8));
		U64 rookFromTo = (col == WHITE ? (bF1 | bH1): (bF8 | bH8));
		U64 fromToBoth = kingFromTo | rookFromTo;

		newState.pieceBB[king] ^= kingFromTo;   // update king bitboard
		newState.pieceBB[rook] ^= rookFromTo;   // update rook bitboard
		newState.colorBB[col] ^= fromToBoth;   // update white or black color bitboard
		newState.occupiedBB ^= fromToBoth;   // update occupied ...
		newState.emptyBB ^= fromToBoth;   // ... and empty bitboard
	}
	else {
		U64 kingFromTo = (col == WHITE ? (bE1 | bC1) : (bE8 | bC8));
		U64 rookFromTo = (col == WHITE ? (bA1 | bD1) : (bA8 | bD8));
		U64 fromToBoth = kingFromTo | rookFromTo;

		newState.pieceBB[king] ^= kingFromTo;   // update king bitboard
		newState.pieceBB[rook] ^= rookFromTo;   // update rook bitboard
		newState.colorBB[col] ^= fromToBoth;   // update white or black color bitboard
		newState.occupiedBB ^= fromToBoth;   // update occupied ...
		newState.emptyBB ^= fromToBoth;   // ... and empty bitboard
	}
}


void WhiteCheckMoveKingOrRook(State& newState, const Move& move) {
	U64 shortCastlingMask = 0x0000000000000009ULL;
	U64 longCastlingMask =	0x0000000000000088ULL;

	U64 fromBB = move.getFromBB();
	U64 toBB = move.getToBB();
	U64 fromToBB = fromBB ^ toBB;

	if (fromToBB | shortCastlingMask) {
		newState.unSetWhiteShortCastlingAllowed();
	}if (fromToBB | longCastlingMask) {
		newState.unSetWhiteLongCastlingAllowed();
	}
}

void BlackCheckMoveKingOrRook(State& newState, const Move& move) {
	U64 shortCastlingMask = 0x0900000000000000ULL;
	U64 longCastlingMask =	0x8800000000000000ULL;

	U64 fromBB = move.getFromBB();
	U64 toBB = move.getToBB();
	U64 fromToBB = fromBB ^ toBB;

	if (fromToBB | shortCastlingMask) {
		newState.unSetBlackShortCastlingAllowed();
	}if (fromToBB | longCastlingMask) {
		newState.unSetBlackLongCastlingAllowed();
	}
}

void DoEnPassant(State& newState, const Move& move) {
	COLOR col = move.getColor();
	PIECE piece = (col == WHITE ? Wpawn : Bpawn);
	PIECE cPiece = (col == WHITE ? Bpawn : Wpawn);

	U64 fromBB = move.getFromBB();
	U64 toBB = move.getToBB();
	U64 fromToBB = fromBB | toBB;
	U64 capBB = (col == WHITE ? bitB_soutOne(toBB) : bitB_nortOne(toBB));

	newState.pieceBB[piece] ^= fromToBB;	// update piece bitboard
	newState.colorBB[col] ^= fromToBB;		// update white or black color bitboard
	newState.pieceBB[cPiece] ^= capBB;      // reset the captured piece
	newState.colorBB[!col] ^= capBB;		// update color bitboard by captured piece
	newState.occupiedBB ^= capBB;			// update occupied, only cap becomes empty
	newState.emptyBB ^= capBB;				// update empty bitboard
	newState.occupiedBB ^= fromToBB;		// update occupied, only from becomes empty
	newState.emptyBB ^= fromToBB;			// update empty bitboard
}

State State::advanceTurn(const Move& move) const{
	State newState(*this);
	newState.switchTurn();

	Move::MoveType type = move.getMoveType();

	if (getBlackLongCastlingAllowed() || getBlackShortCastlingAllowed()) {
		BlackCheckMoveKingOrRook(newState, move);
	}
	if (getWhiteLongCastlingAllowed() || getWhiteShortCastlingAllowed()) {
		WhiteCheckMoveKingOrRook(newState, move);
	}

	if (type == Move::KING_CASTLE || type == Move::QUEEN_CASTLE) {
		DoCastling(newState, move);
	}
	else if (type == Move::ENPASSANT_CAP) {
		DoEnPassant(newState, move);
	}
	else if (move.isCapture() && move.isPromotion()) {
		U64 fromBB = move.getFromBB();
		U64 toBB = move.getToBB();
		U64 fromToBB = fromBB ^ toBB; // |+
		PIECE piece = move.getPiece();
		PIECE promoPiece = move.getPromotedPiece();
		COLOR color = move.getColor();
		PIECE cPiece = move.getCPiece();
		COLOR cColor = move.getCColor();
		newState.pieceBB[piece] ^= fromBB;   // update piece bitboard
		newState.pieceBB[promoPiece] ^= toBB;   // update piece bitboard
		newState.colorBB[color] ^= fromToBB;   // update white or black color bitboard
		newState.pieceBB[cPiece] ^= toBB;       // reset the captured piece
		newState.colorBB[cColor] ^= toBB;       // update color bitboard by captured piece
		newState.occupiedBB ^= fromBB;     // update occupied, only from becomes empty
		newState.emptyBB ^= fromBB;     // update empty bitboard
	}else if (move.isPromotion()) {
		U64 fromBB = move.getFromBB();
		U64 toBB = move.getToBB();
		U64 fromToBB = fromBB ^ toBB; // |+
		PIECE piece = move.getPiece();
		PIECE promoPiece = move.getPromotedPiece();
		COLOR color = move.getColor();
		newState.pieceBB[piece] ^= fromBB;   // update piece bitboard
		newState.pieceBB[promoPiece] ^= toBB;   // update piece bitboard
		newState.colorBB[color] ^= fromToBB;   // update white or black color bitboard
		newState.occupiedBB ^= fromToBB;   // update occupied ...
		newState.emptyBB ^= fromToBB;   // ... and empty bitboard
	}else if (type == Move::CAPTURE) {
		U64 fromBB = move.getFromBB();
		U64 toBB = move.getToBB();
		U64 fromToBB = fromBB ^ toBB; // |+
		PIECE piece = move.getPiece();
		COLOR color = move.getColor();
		PIECE cPiece = move.getCPiece();
		COLOR cColor = move.getCColor();
		newState.pieceBB[piece] ^= fromToBB;   // update piece bitboard
		newState.colorBB[color] ^= fromToBB;   // update white or black color bitboard
		newState.pieceBB[cPiece] ^= toBB;       // reset the captured piece
		newState.colorBB[cColor] ^= toBB;       // update color bitboard by captured piece
		newState.occupiedBB ^= fromBB;     // update occupied, only from becomes empty
		newState.emptyBB ^= fromBB;     // update empty bitboard
	}
	else {
		U64 fromBB = move.getFromBB();
		U64 toBB = move.getToBB();
		U64 fromToBB = fromBB ^ toBB; // |+
		PIECE piece = move.getPiece();
		COLOR color = move.getColor();
		newState.pieceBB[piece] ^= fromToBB;   // update piece bitboard
		newState.colorBB[color] ^= fromToBB;   // update white or black color bitboard
		newState.occupiedBB ^= fromToBB;   // update occupied ...
		newState.emptyBB ^= fromToBB;   // ... and empty bitboard
	}

#ifdef _DEBUG //check if all the collection boards are updating correctly
	U64 lempty = newState.emptyBB;
	U64 loccupied = newState.occupiedBB;
	U64 lwhite = newState.colorBB[WHITE];
	U64 lblack = newState.colorBB[BLACK];
	newState.rebuildBBs();
	assert(lempty == newState.emptyBB);
	assert(loccupied == newState.occupiedBB);
	assert(lwhite == newState.colorBB[WHITE]);
	assert(lblack == newState.colorBB[BLACK]);
#endif 
	return newState;
}

void State::clear() {
	flags = 0;
	for (int i = 0; i < 12; i++) {
		pieceBB[i] = 0;
	}
}

State State::initialize() {
	State state;
	state.pieceBB[Wpawn] =		bA2	| bB2 | bC2 | bD2 | bE2 | bF2 | bG2 | bH2;
	state.pieceBB[Wrook] =		bA1	| bH1;
	state.pieceBB[Wknight] =	bB1 | bG1;
	state.pieceBB[Wbishop] =	bC1 | bF1;
	state.pieceBB[Wqueen] =		bD1;
	state.pieceBB[Wking] =		bE1;

	state.pieceBB[Bpawn] =		bA7 | bB7 | bC7 | bD7 | bE7 | bF7 | bG7 | bH7;
	state.pieceBB[Brook] =		bA8 | bH8;
	state.pieceBB[Bknight] =	bB8 | bG8;
	state.pieceBB[Bbishop] =	bC8 | bF8;
	state.pieceBB[Bqueen] =		bD8;
	state.pieceBB[Bking] =		bE8;

	state.rebuildBBs();

	state.flags = 0; // clear any flags

	state.setBlackShortCastlingAllowed();
	state.setBlackLongCastlingAllowed();

	state.setWhiteShortCastlingAllowed();
	state.setWhiteLongCastlingAllowed();

	state.setTurnWhite();

	return state;
}


void State::rebuildBBs() {
	colorBB[WHITE] = 0;
	for (int i = 0; i < 6; i++) {
		colorBB[WHITE] |= pieceBB[i];
	}

	colorBB[BLACK] = 0;
	for (int i = 6; i < 12; i++) {
		colorBB[BLACK] |= pieceBB[i];
	}

	occupiedBB = 0;
	for (int i = 0; i < 12; i++) {
		occupiedBB |= pieceBB[i];
	}

	emptyBB = ~occupiedBB;
}
