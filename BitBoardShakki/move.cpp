#include "move.h"
#include "piece.h"
#include "statebb.h"
#include "convert.h"
#include <sstream>

Move::Move() {
	m_Move = MOVE_NO_POSSIBLE_MOVES;
}

unsigned int getFlagsFrom(Move::MoveType move) {
	switch (move)
	{
	case Move::QUIET:
		return MOVE_FLAGS_QUIET;
		break;
	case Move::PAWN_DOUBLE_PUSH:
		return MOVE_FLAGS_DOUBLE;
		break;
	case Move::KING_CASTLE:
		return MOVE_FLAGS_KCASTLE;
		break;
	case Move::QUEEN_CASTLE:
		return MOVE_FLAGS_QCASTLE;
		break;
	case Move::CAPTURE:
		return MOVE_FLAGS_CAPTURE;
		break;
	case Move::ENPASSANT_CAP:
		return MOVE_FLAGS_EP_CAPTURE;
		break;
	case Move::PROMOTION_KNIGHT:
		return MOVE_FLAGS_PROMO_N;
		break;
	case Move::PROMOTION_BISHOP:
		return MOVE_FLAGS_PROMO_B;
		break;
	case Move::PROMOTION_ROOK:
		return MOVE_FLAGS_PROMO_R;
		break;
	case Move::PROMOTION_QUEEN:
		return MOVE_FLAGS_PROMO_Q;
		break;
	case Move::CAPTURE_PROMOTION_KNIGHT:
		return MOVE_FLAGS_PROMO_N_CAP;
		break;
	case Move::CAPTURE_PROMOTION_BISHOP:
		return MOVE_FLAGS_PROMO_B_CAP;
		break;
	case Move::CAPTURE_PROMOTION_ROOK:
		return MOVE_FLAGS_PROMO_R_CAP;
		break;
	case Move::CAPTURE_PROMOTION_QUEEN:
		return MOVE_FLAGS_PROMO_Q_CAP;
		break;
	default:
		return MOVE_FLAGS_QUIET;
		break;
	}
}


Move::Move(BITPOS from, BITPOS to, PIECE piece, MoveType type) {
	int fromindex = bitB_bitScanForward(from);
	int toindex = bitB_bitScanForward(to);
	auto flags = getFlagsFrom(type);

	m_Move = 0 | (fromindex) | (toindex << 6) | (piece << 12) | (flags << 20);

#ifdef _DEBUG
	 _dbg_from = from;
	 _dbg_to = to;
	 _dbg_piece = piece;
	 _dbg_cPiece = piece;
	 _dbg_mType = type;
#endif
}

Move::Move(BITPOS from, BITPOS to, PIECE piece, MoveType type, PIECE cPiece) {
	int fromindex = bitB_bitScanForward(from);
	int toindex = bitB_bitScanForward(to);
	auto flags = getFlagsFrom(type);

	m_Move = 0 | (fromindex) | (toindex << 6) | (piece << 12) | (cPiece << 16) | (flags << 20);
	
#ifdef _DEBUG
	_dbg_from = from;
	_dbg_to = to;
	_dbg_piece = piece;
	_dbg_cPiece = cPiece;
	_dbg_mType = type;
#endif
}

Move::Move(int fromIdx, int toIdx, PIECE piece, int flags) {
	m_Move = 0 | (fromIdx) | (toIdx << 6) | (piece << 12) | (flags << 20);
#ifdef _DEBUG
	_dbg_from = (BITPOS)(1ULL << fromIdx);
	_dbg_to = (BITPOS)(1ULL << toIdx);
	_dbg_piece = piece;
	_dbg_cPiece = piece;
	_dbg_mType = getMoveType();
#endif
}
Move::Move(int fromIdx, int toIdx, PIECE piece, int flags, PIECE cPiece) {
	m_Move = 0 | (fromIdx) | (toIdx << 6) | (piece << 12) | (cPiece << 16) | (flags << 20);
#ifdef _DEBUG
	_dbg_from = (BITPOS)(1ULL << fromIdx);
	_dbg_to = (BITPOS)(1ULL << toIdx);
	_dbg_piece = piece;
	_dbg_cPiece = cPiece;
	_dbg_mType = getMoveType();
#endif
}

Move::Move(uint_fast32_t move) {
	m_Move = move;
}

Move::operator uint_fast32_t() const {
	return m_Move;
}

BITPOS Move::getFromBB()  const {
	const unsigned int mask = 0x3f;
	auto i = m_Move & mask;
	return (BITPOS)(1ULL << i);
}
BITPOS Move::getToBB() const {
	const unsigned int mask = 0x3f;
	auto i =(m_Move >> 6) & mask;
	return (BITPOS)(1ULL << i);
}
PIECE Move::getPiece() const {
	const unsigned int mask = 0xf;
	auto i = (m_Move >> 12) & mask;
	return (PIECE)i;
}
PIECE Move::getCPiece() const {
	const unsigned int mask = 0xf;
	auto i = (m_Move >> 16) & mask;
	return (PIECE)i;
}
COLOR Move::getColor() const {
	const unsigned int mask = 0xf;
	auto i = (m_Move >> 12) & mask;
	if (i < 6) {
		return WHITE;
	}
	else {
		return BLACK;
	}
}
COLOR Move::getCColor() const {
	const unsigned int mask = 0xf;
	auto i = (m_Move >> 16) & mask;
	if (i < 6) {
		return WHITE;
	}
	else {
		return BLACK;
	}
}

PIECE Move::getPromotedPiece()  const {
	MoveType type = getMoveType();
	COLOR col = getColor();
	switch (type)
	{
	case Move::PROMOTION_KNIGHT:
		return col == WHITE ? Wknight : Bknight;
		break;
	case Move::PROMOTION_BISHOP:
		return col == WHITE ? Wbishop : Bbishop;
		break;
	case Move::PROMOTION_ROOK:
		return col == WHITE ? Wrook : Brook;
		break;
	case Move::PROMOTION_QUEEN:
		return col == WHITE ? Wqueen : Bqueen;
		break;
	case Move::CAPTURE_PROMOTION_KNIGHT:
		return col == WHITE ? Wknight : Bknight;
		break;
	case Move::CAPTURE_PROMOTION_BISHOP:
		return col == WHITE ? Wbishop : Bbishop;
		break;
	case Move::CAPTURE_PROMOTION_ROOK:
		return col == WHITE ? Wrook : Brook;
		break;
	case Move::CAPTURE_PROMOTION_QUEEN:
		return col == WHITE ? Wqueen : Bqueen;
		break;
	default:
		return col == WHITE ? Wpawn : Bpawn;
		break;
	}
}

unsigned int Move::getFlags()  const {
	const unsigned int mask = 0xff;
	auto i = (m_Move >> 20) & mask;
	return i;
}
bool Move::isCapture() const {
	auto flags = getFlags();
	return CheckFlag(flags, MOVE_FLAGS_CAPTURE);
}
bool Move::isPromotion() const {
	auto flags = getFlags();
	return CheckFlag(flags, MOVE_FLAGS_PROMO_N);
}
bool Move::isQuiet() const {
	auto flags = getFlags();
	return (flags == MOVE_FLAGS_QUIET);
}
bool Move::isDoublePush() const {
	auto flags = getFlags();
	return (flags == MOVE_FLAGS_DOUBLE);
}
bool Move::isCastle() const {
	auto flags = getFlags();
	return (flags == MOVE_FLAGS_KCASTLE || flags == MOVE_FLAGS_QCASTLE);
}
bool Move::isEnPassant() const {
	auto flags = getFlags();
	return (flags == MOVE_FLAGS_EP_CAPTURE);
}
bool Move::isNoPossibleMoves() const {
	return m_Move == MOVE_NO_POSSIBLE_MOVES;
}

Move::MoveType Move::getMoveType() const {
	auto flags = getFlags();

	switch (flags)
	{
	case MOVE_FLAGS_QUIET:
		return Move::QUIET;
	case MOVE_FLAGS_CAPTURE:
		return Move::CAPTURE;
	case MOVE_FLAGS_DOUBLE:
		return Move::PAWN_DOUBLE_PUSH;
	case MOVE_FLAGS_KCASTLE:
		return Move::KING_CASTLE;
	case MOVE_FLAGS_QCASTLE:
		return Move::QUEEN_CASTLE;
	case MOVE_FLAGS_EP_CAPTURE:
		return Move::ENPASSANT_CAP;
	case MOVE_FLAGS_PROMO_N:
		return Move::PROMOTION_KNIGHT;
	case MOVE_FLAGS_PROMO_B:
		return Move::PROMOTION_BISHOP;
	case MOVE_FLAGS_PROMO_R:
		return Move::PROMOTION_ROOK;
	case MOVE_FLAGS_PROMO_Q:
		return Move::PROMOTION_QUEEN;
	case MOVE_FLAGS_PROMO_N_CAP:
		return Move::CAPTURE_PROMOTION_KNIGHT;
	case MOVE_FLAGS_PROMO_B_CAP:
		return Move::CAPTURE_PROMOTION_BISHOP;
	case MOVE_FLAGS_PROMO_R_CAP:
		return Move::CAPTURE_PROMOTION_ROOK;
	case MOVE_FLAGS_PROMO_Q_CAP:
		return Move::CAPTURE_PROMOTION_QUEEN;
	default:
		return Move::QUIET;
	}
}

std::wstring Move::to_str() const {
	if (isCastle()) {
		auto mtype = getMoveType();
		if (mtype == Move::KING_CASTLE) {
			return L"0-0";
		}
		else {
			return L"0-0-0";
		}
	}

	std::wstringstream stream;
	auto p = pieces[getPiece()];
	stream << p->getUnicode();
	stream << bitPosToStr(getFromBB());
	stream << (isCapture() ? L"x" : L"-");
	stream << bitPosToStr(getToBB());
	if (isPromotion()) {
		auto promoP = pieces[getPromotedPiece()];
		stream << promoP->getUnicode();
	}
	return stream.str();
}

