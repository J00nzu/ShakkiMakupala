#pragma once
#include "flagmanip.h"
#include "bitboardops.h"

/*
#define MOVE_H_COLOR_WHITE			0b00000001U
#define MOVE_H_CAPTURE				0b00000010U
#define MOVE_H_CASTLING_SHORT		0b00000100U
#define MOVE_H_CASTLING_LONG		0b00001000U
#define MOVE_H_EN_PASSANT_LEFT		0b00010000U
#define MOVE_H_EN_PASSANT_RIGHT		0b00100000U
#define MOVE_H_PROMOTION			0b01000000U
#define MOVE_H_NO_POSSIBLE_MOVES	0b10000000U
*/

#define MOVE_FROM				0b00000000000000000000000000111111U
#define MOVE_TO					0b00000000000000000000111111000000U
#define MOVE_PIECE				0b00000000000000001111000000000000U
#define MOVE_CPIECE				0b00000000000011110000000000000000U
#define MOVE_FLAGS				0b00001111111100000000000000000000U
#define MOVE_FLAGS_QUIET		0b00000000U
#define MOVE_FLAGS_DOUBLE		0b00000001U
#define MOVE_FLAGS_KCASTLE		0b00000010U
#define MOVE_FLAGS_QCASTLE		0b00000011U
#define MOVE_FLAGS_CAPTURE		0b00000100U
#define MOVE_FLAGS_EP_CAPTURE	0b00000101U
#define MOVE_FLAGS_PROMO_N		0b00001000U
#define MOVE_FLAGS_PROMO_B		0b00001001U
#define MOVE_FLAGS_PROMO_R		0b00001010U
#define MOVE_FLAGS_PROMO_Q		0b00001011U
#define MOVE_FLAGS_PROMO_N_CAP	0b00001100U
#define MOVE_FLAGS_PROMO_B_CAP	0b00001101U
#define MOVE_FLAGS_PROMO_R_CAP	0b00001110U
#define MOVE_FLAGS_PROMO_Q_CAP	0b00001111U
#define MOVE_NO_POSSIBLE_MOVES	0xFFFFFFFFU


struct Move{
private:
	uint_fast32_t m_Move = 0;
public:
	enum MoveType {
		QUIET,
		PAWN_DOUBLE_PUSH,
		KING_CASTLE,
		QUEEN_CASTLE,
		CAPTURE,
		ENPASSANT_CAP,
		PROMOTION_KNIGHT,
		PROMOTION_BISHOP,
		PROMOTION_ROOK,
		PROMOTION_QUEEN,
		CAPTURE_PROMOTION_KNIGHT,
		CAPTURE_PROMOTION_BISHOP,
		CAPTURE_PROMOTION_ROOK,
		CAPTURE_PROMOTION_QUEEN
	};

	Move(BITPOS from, BITPOS to, PIECE piece, MoveType type);
	Move(BITPOS from, BITPOS to, PIECE piece, MoveType type, PIECE cPiece);
	Move(int fromIdx, int toIdx, PIECE piece, int flags);
	Move(int fromIdx, int toIdx, PIECE piece, int flags, PIECE cPiece);
	Move(uint_fast32_t move);
	Move();

	operator uint_fast32_t() const;

	inline bool Move::operator <(const Move& other) {
		return m_Move < other.m_Move;
	}
	inline bool Move::operator >(const Move& other) {
		return m_Move > other.m_Move;
	}
	inline bool Move::operator ==(const Move& other) {
		return m_Move == other.m_Move;
	}


	BITPOS getFromBB() const;
	BITPOS getToBB() const;
	PIECE getPiece() const;
	PIECE getCPiece() const;
	COLOR getColor() const;
	COLOR getCColor() const;
	PIECE getPromotedPiece() const;
	unsigned int getFlags() const;

	bool isPromotion() const;
	bool isQuiet() const;
	bool isCapture() const;
	bool isDoublePush() const;
	bool isCastle() const;
	bool isEnPassant() const;
	bool isNoPossibleMoves() const;
	std::wstring to_str() const;


	MoveType getMoveType() const;

#ifdef _DEBUG
private:
	BITPOS _dbg_from;
	BITPOS _dbg_to;
	PIECE _dbg_piece;
	PIECE _dbg_cPiece;
	MoveType _dbg_mType;
#endif
};

#define MOVESET_MAX_MOVES 218
//#define USE_HEAP_ARRAY

#ifdef USE_HEAP_ARRAY

struct MoveSet {
private:
	Move* _arr;
	int _len;
	int _capacity;

	void growSet();
public:

	MoveSet();
	MoveSet(const MoveSet& copy);
	MoveSet& operator=(MoveSet& move);

	void push_back(const Move& move);
	int size() const;
	int capacity() const;
	bool empty() const;
	Move& get(int index);
	void set(int index, const Move& other);
	Move& operator[](int index);
	void erase(int index);
	void sort();

	typedef Move* iterator;
	typedef const Move* const_iterator;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	~MoveSet();
};
#else
struct MoveSet {
private:
	Move _arr[MOVESET_MAX_MOVES];
	int _len = 0;
public:

	void push_back(const Move& move);
	int size() const;
	bool empty() const;
	Move get(int index) const;
	void set(int index, const Move& other);
	Move& operator[](int index);
	void erase(int index);
	void sort();

	typedef Move* iterator;
	typedef const Move* const_iterator;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

};
#endif // USE_HEAP_ARRAY