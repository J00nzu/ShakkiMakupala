#pragma once
#include "enums.h"
#include "bitboardops.h"
#include "piece.h"
#include "move.h"
#include "flagmanip.h"
#include <functional>
#include "hash.h"


#define STATE_H_TURN_WHITE						0b00000001U
#define STATE_H_WHITE_LONG_CASTLING_ALLOWED		0b00000010U
#define STATE_H_WHITE_SHORT_CASTLING_ALLOWED	0b00000100U
#define STATE_H_BLACK_LONG_CASTLING_ALLOWED		0b00001000U
#define STATE_H_BLACK_SHORT_CASTLING_ALLOWED	0b00010000U

struct State {
	U64 pieceBB[12];
	U64 colorBB[2];
	U64 occupiedBB;
	U64 emptyBB;
	uint_fast32_t flags;
	StateHash hash;

	State advanceTurn(const Move& move) const;

	void clear();
	void rebuildBBs();
	const Piece* getPiece(BITPOS pos) const;


	static int getRank(BITPOS);
	static State initialize();




	inline void State::setTurnWhite() {
		SetFlag(flags, STATE_H_TURN_WHITE);
	}
	inline void State::setTurnBlack() {
		UnsetFlag(flags, STATE_H_TURN_WHITE);
	}
	inline void State::switchTurn() {
		SwitchFlag(flags, STATE_H_TURN_WHITE);
	}
	inline COLOR State::getTurnColor() const {
		if ((flags & STATE_H_TURN_WHITE) != 0) {
			return WHITE;
		}
		else {
			return BLACK;
		}
	}

	inline void State::setWhiteShortCastlingAllowed() {
		SetFlag(flags, STATE_H_WHITE_SHORT_CASTLING_ALLOWED);
	}
	inline void State::setBlackShortCastlingAllowed() {
		SetFlag(flags, STATE_H_BLACK_SHORT_CASTLING_ALLOWED);
	}
	inline void State::setWhiteLongCastlingAllowed() {
		SetFlag(flags, STATE_H_WHITE_LONG_CASTLING_ALLOWED);
	}
	inline void State::setBlackLongCastlingAllowed() {
		SetFlag(flags, STATE_H_BLACK_LONG_CASTLING_ALLOWED);
	}

	inline void State::unSetWhiteShortCastlingAllowed() {
		UnsetFlag(flags, STATE_H_WHITE_SHORT_CASTLING_ALLOWED);
	}
	inline void State::unSetBlackShortCastlingAllowed() {
		UnsetFlag(flags, STATE_H_BLACK_SHORT_CASTLING_ALLOWED);
	}
	inline void State::unSetWhiteLongCastlingAllowed() {
		UnsetFlag(flags, STATE_H_WHITE_LONG_CASTLING_ALLOWED);
	}
	inline void State::unSetBlackLongCastlingAllowed() {
		UnsetFlag(flags, STATE_H_BLACK_LONG_CASTLING_ALLOWED);
	}

	inline bool State::getWhiteShortCastlingAllowed() const {
		return CheckFlag(flags, STATE_H_WHITE_SHORT_CASTLING_ALLOWED);
	}
	inline bool State::getBlackShortCastlingAllowed() const {
		return CheckFlag(flags, STATE_H_BLACK_SHORT_CASTLING_ALLOWED);
	}
	inline bool State::getWhiteLongCastlingAllowed() const {
		return CheckFlag(flags, STATE_H_WHITE_LONG_CASTLING_ALLOWED);
	}
	inline bool State::getBlackLongCastlingAllowed() const {
		return CheckFlag(flags, STATE_H_BLACK_LONG_CASTLING_ALLOWED);
	}

	inline bool operator==(const State& other) const{
		for (int i = 0; i < 12; i++) {
			if (pieceBB[i] != other.pieceBB[i]) {
				return false;
			}
		}
		if (flags != other.flags) {
			return false;
		}
		return true;
	}
};
