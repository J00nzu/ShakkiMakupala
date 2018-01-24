#pragma once
#include "piece.h"
#include "enums.h"
#include "flagmanip.h"

#define STATE_H_TURN_WHITE						0b00000001
#define STATE_H_WHITE_LONG_CASTLING_ALLOWED		0b00000010
#define STATE_H_WHITE_SHORT_CASTLING_ALLOWED	0b00000100
#define STATE_H_BLACK_LONG_CASTLING_ALLOWED		0b00001000
#define STATE_H_BLACK_SHORT_CASTLING_ALLOWED	0b00010000
#define STATE_H_WHITE_CHECKED					0b00100000
#define STATE_H_BLACK_CHECKED					0b01000000

class State {

public:
	PIECE board[8 * 8];
	uint_least8_t flags;

	State() {};
	const Piece* getPiece(POSITION pos) const;
	void setPiece(POSITION pos, PIECE piece);
	State advanceTurn(const Move& move) const;
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
	inline COLOR State::getTurnColor() const{
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
	inline void State::setWhiteChecked() {
		SetFlag(flags, STATE_H_WHITE_CHECKED);
	}
	inline void State::setBlackChecked() {
		SetFlag(flags, STATE_H_BLACK_CHECKED);
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
	inline void State::unSetWhiteChecked() {
		UnsetFlag(flags, STATE_H_WHITE_CHECKED);
	}
	inline void State::unSetBlackChecked() {
		UnsetFlag(flags, STATE_H_BLACK_CHECKED);
	}

	inline bool State::getWhiteShortCastlingAllowed() const{
		return (flags & STATE_H_WHITE_SHORT_CASTLING_ALLOWED) != 0;
	}
	inline bool State::getBlackShortCastlingAllowed() const {
		return (flags & STATE_H_BLACK_SHORT_CASTLING_ALLOWED) != 0;
	}
	inline bool State::getWhiteLongCastlingAllowed() const {
		return (flags & STATE_H_WHITE_LONG_CASTLING_ALLOWED) != 0;
	}
	inline bool State::getBlackLongCastlingAllowed() const {
		return (flags & STATE_H_BLACK_LONG_CASTLING_ALLOWED) != 0;
	}
	inline bool State::getWhiteChecked() const {
		return (flags & STATE_H_WHITE_CHECKED) != 0;
	}
	inline bool State::getBlackChecked() const {
		return (flags & STATE_H_BLACK_CHECKED) != 0;
	}


};


