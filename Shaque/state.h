#pragma once
#include "piece.h"
#include "enums.h"
#include "flagmanip.h"

#define STATE_H_TURN_WHITE				0b00000001
#define STATE_H_WHITE_CASTLING_ALLOWED	0b00000010
#define STATE_H_BLACK_CASTLING_ALLOWED	0b00000100
#define STATE_H_WHITE_CHECKED			0b00001000
#define STATE_H_BLACK_CHECKED			0b00010000

class State {

public:
	PIECE board[8 * 8];
	uint_least8_t flags;

	State() {};
	const Piece* getPiece(POSITION pos) const;
	void setPiece(POSITION pos, PIECE piece);
	static State initialize();
	static State initializeEmpty();
	static int getRank(POSITION);
	static char getLetter(POSITION pos);

	//flag manipulation
	/*
	void setTurnWhite();
	void setTurnBlack();
	COLOR getTurnColor();

	void setWhiteCastlingAllowed();
	void setBlackCastlingAllowed();
	void setWhiteChecked();
	void setBlackChecked();

	void unSetWhiteCastlingAllowed();
	void unSetBlackCastlingAllowed();
	void unSetWhiteChecked();
	void unSetBlackChecked();

	bool getWhiteCastlingAllowed();
	bool getBlackCastlingAllowed();
	bool getWhiteChecked();
	bool getBlackChecked();
	*/

	inline void State::setTurnWhite() {
		SetFlag(flags, STATE_H_TURN_WHITE);
	}
	inline void State::setTurnBlack() {
		UnsetFlag(flags, STATE_H_TURN_WHITE);
	}
	inline COLOR State::getTurnColor() {
		if ((flags & STATE_H_TURN_WHITE) != 0) {
			return WHITE;
		}
		else {
			return BLACK;
		}
	}

	inline void State::setWhiteCastlingAllowed() {
		SetFlag(flags, STATE_H_WHITE_CASTLING_ALLOWED);
	}
	inline void State::setBlackCastlingAllowed() {
		SetFlag(flags, STATE_H_BLACK_CASTLING_ALLOWED);
	}
	inline void State::setWhiteChecked() {
		SetFlag(flags, STATE_H_WHITE_CHECKED);
	}
	inline void State::setBlackChecked() {
		SetFlag(flags, STATE_H_BLACK_CHECKED);
	}

	inline void State::unSetWhiteCastlingAllowed() {
		UnsetFlag(flags, STATE_H_WHITE_CASTLING_ALLOWED);
	}
	inline void State::unSetBlackCastlingAllowed() {
		UnsetFlag(flags, STATE_H_BLACK_CASTLING_ALLOWED);
	}
	inline void State::unSetWhiteChecked() {
		UnsetFlag(flags, STATE_H_WHITE_CHECKED);
	}
	inline void State::unSetBlackChecked() {
		UnsetFlag(flags, STATE_H_BLACK_CHECKED);
	}

	inline bool State::getWhiteCastlingAllowed() {
		return (flags & STATE_H_WHITE_CASTLING_ALLOWED) != 0;
	}
	inline bool State::getBlackCastlingAllowed() {
		return (flags & STATE_H_BLACK_CASTLING_ALLOWED) != 0;
	}
	inline bool State::getWhiteChecked() {
		return (flags & STATE_H_WHITE_CHECKED) != 0;
	}
	inline bool State::getBlackChecked() {
		return (flags & STATE_H_BLACK_CHECKED) != 0;
	}


};


