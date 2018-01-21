#pragma once
#include "enums.h"
#include <string>
#include "flagmanip.h"

#define MOVE_H_COLOR_WHITE			0b00000001
#define MOVE_H_CAPTURE				0b00000010
#define MOVE_H_CASTLING_SHORT		0b00000100
#define MOVE_H_CASTLING_LONG		0b00001000
#define MOVE_H_EN_PASSANT_LEFT		0b00010000
#define MOVE_H_EN_PASSANT_RIGHT		0b00100000
#define MOVE_H_PROMOTION			0b01000000
#define MOVE_H_CHECK				0b10000000

class Piece;

struct Move {
	PIECE piece;
	POSITION startPos;
	POSITION endPos;
	uint_least8_t flags = 0;
	PIECE promotedTo = EMPTY;
	PIECE captured = EMPTY;

	Move(){}
	Move(const Piece* pc, POSITION start, POSITION end);

	inline void setColorWhite() {
		SetFlag(flags, MOVE_H_COLOR_WHITE);
	}
	inline void setCapture() {
		SetFlag(flags, MOVE_H_CAPTURE);
	}
	inline void setCastlingShort() {
		SetFlag(flags, MOVE_H_CASTLING_SHORT);
	}
	inline void setCastlingLong() {
		SetFlag(flags, MOVE_H_CASTLING_LONG);
	}
	inline void setEnPassantLeft() {
		SetFlag(flags, MOVE_H_EN_PASSANT_LEFT);
	}
	inline void setEnPassantRight() {
		SetFlag(flags, MOVE_H_EN_PASSANT_RIGHT);
	}
	inline void setPromotion() {
		SetFlag(flags, MOVE_H_PROMOTION);
	}
	inline void setCheck() {
		SetFlag(flags, MOVE_H_CHECK);
	}


	inline void unSetColorWhite() {
		UnsetFlag(flags, MOVE_H_COLOR_WHITE);
	}
	inline void unSetCapture() {
		UnsetFlag(flags, MOVE_H_CAPTURE);
	}
	inline void unSetCastlingShort() {
		UnsetFlag(flags, MOVE_H_CASTLING_SHORT);
	}
	inline void unSetCastlingLong() {
		UnsetFlag(flags, MOVE_H_CASTLING_LONG);
	}
	inline void unSetEnPassantLeft() {
		UnsetFlag(flags, MOVE_H_EN_PASSANT_LEFT);
	}
	inline void unSetEnPassantRight() {
		UnsetFlag(flags, MOVE_H_EN_PASSANT_RIGHT);
	}
	inline void unSetPromotion() {
		UnsetFlag(flags, MOVE_H_PROMOTION);
	}
	inline void unSetCheck() {
		UnsetFlag(flags, MOVE_H_CHECK);
	}




	inline COLOR getColor() const {
		if ((flags & MOVE_H_COLOR_WHITE) != 0) {
			return WHITE;
		}
		else {
			return BLACK;
		}
	}
	inline bool getCapture() const {
		return (flags & MOVE_H_CAPTURE) != 0;
	}
	inline bool getCastlingShort() const {
		return (flags & MOVE_H_CASTLING_SHORT) != 0;
	}
	inline bool getCastlingLong() const {
		return (flags & MOVE_H_CASTLING_LONG) != 0;
	}
	inline bool getEnPassantLeft() const {
		return (flags & MOVE_H_EN_PASSANT_LEFT) != 0;
	}
	inline bool getEnPassantRight() const {
		return (flags & MOVE_H_EN_PASSANT_RIGHT) != 0;
	}
	inline bool getPromotion() const {
		return (flags & MOVE_H_PROMOTION) != 0;
	}
	inline bool getCheck() const {
		return (flags & MOVE_H_CHECK) != 0;
	}
};

struct PossibleMove {
	Move move;
	int evaluation = 0;

	PossibleMove(){}
	PossibleMove(const Move& m) {
		move = m;
	}

	std::string toStr();
};


