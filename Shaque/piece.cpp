#include "piece.h"
#include "state.h"
#include "movegeneration.h"
#include "movement.h"


const Piece* pieces[12] =
{
	new Rook(UCODE_WROOK,		WROOK,		WHITE),
	new Knight(UCODE_WKNIGHT,	WKNIGHT,	WHITE),
	new Bishop(UCODE_WBISHOP,	WBISHOP,	WHITE),
	new Queen(UCODE_WQUEEN,		WQUEEN,		WHITE),
	new King(UCODE_WKING,		WKING,		WHITE),
	new Pawn(UCODE_WPAWN,		WPAWN,		WHITE),

	new Rook(UCODE_BROOK,		BROOK,		BLACK),
	new Knight(UCODE_BKNIGHT,	BKNIGHT,	BLACK),
	new Bishop(UCODE_BBISHOP,	BBISHOP,	BLACK),
	new Queen(UCODE_BQUEEN,		BQUEEN,		BLACK),
	new King(UCODE_BKING,		BKING,		BLACK),
	new Pawn(UCODE_BPAWN,		BPAWN,		BLACK)
};

Piece::Piece(const std::wstring& unicode, PIECE code, COLOR color) {
	_unicode = unicode;
	_code = code;
	_color = color;
}
void Piece::setCode(PIECE code) {
	_code = code;
}

PIECE Piece::getCode() const {
	return _code;
}

void Piece::setUnicode(const std::wstring& unicode) {
	_unicode = unicode;
}

std::wstring Piece::getUnicode() const {
	return _unicode;
}

void Piece::setColor(COLOR color) {
	_color = color;
}

COLOR Piece::getColor() const {
	return _color;
}




void Rook::getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const {

	movegeneration::moveOrCapture(moves, gameState, this, pos, DIRECTION::STRAIGHT, 0);
}

void Knight::getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const {
	
	movegeneration::moveOrCapture(moves, gameState, this, pos, DIRECTION::KNIGHT, 0);
}

void Bishop::getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const {
	
	movegeneration::moveOrCapture(moves, gameState, this, pos, DIRECTION::DIAGONAL, 0);
}

void Queen::getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const {
	
	movegeneration::moveOrCapture(moves, gameState, this, pos, DIRECTION::ANY, 0);
}

void King::getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const {
	
	movegeneration::moveOrCapture(moves, gameState, this, pos, DIRECTION::ANY, 1);
	movegeneration::checkCastling(moves, gameState, this, pos);
}

void Pawn::getPossibleMoves(std::vector<PossibleMove>& moves, const State& gameState, const Move& lastOpponentMove, POSITION pos) const {
	
	int moveRange = 1;

	COLOR color = this->_color;
	
	if (color == WHITE) {
		if (gameState.getRank(pos) == 2) {
			moveRange = 2;
		}
		movegeneration::move(moves, gameState, this, pos, DIRECTION::UP, moveRange);
		uint_least16_t dirs = (uint_least16_t)DIRECTION::UPRIGHT | (uint_least16_t)DIRECTION::UPLEFT;
		movegeneration::capture(moves, gameState, this, pos, dirs, 1);

	}
	else {
		if (gameState.getRank(pos) == 7) {
			moveRange = 2;
		}
		movegeneration::move(moves, gameState, this, pos, DIRECTION::DOWN, moveRange);
		uint_least16_t dirs = (uint_least16_t)DIRECTION::DOWNRIGHT | (uint_least16_t)DIRECTION::DOWNLEFT;
		movegeneration::capture(moves, gameState, this, pos, dirs, 1);
	}

	if (moveRange == 1) {
		movegeneration::checkEnPassant(moves, gameState, lastOpponentMove, this, pos);
		movegeneration::checkPromotionForPawnMoves(moves, this, pos);
	}
}
