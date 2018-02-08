#include "movegeneration.h"
#include "state.h"

void knightMoveDir(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, DIRECTION dir1, DIRECTION dir2, DIRECTION dir3) {
	POSITION markPos = startPos;
	if (!movement::ableToMove(markPos, dir1)) {
		return;
	}
	markPos = movement::moveTo(markPos, dir1);
	if (!movement::ableToMove(markPos, dir1)) {
		return;
	}
	markPos = movement::moveTo(markPos, dir1);


	POSITION markPos2 = markPos;
	if (movement::ableToMove(markPos2, dir2)) {
		markPos2 = movement::moveTo(markPos2, dir2);
		if (state.board[markPos2] != EMPTY) {
			if (state.getPiece(markPos2)->getColor() != piece->getColor()) {
				Move move(piece, startPos, markPos2);
				move.setCapture();
				move.captured = state.board[markPos2];
				list.push_back(move);
			}
		}
		else {
			Move move(piece, startPos, markPos2);
			list.push_back(move);
		}
	}

	POSITION markPos3 = markPos;
	if (movement::ableToMove(markPos3, dir3)) {
		markPos3 = movement::moveTo(markPos3, dir3);
		if (state.board[markPos3] != EMPTY) {
			if (state.getPiece(markPos3)->getColor() != piece->getColor()) {
				Move move(piece, startPos, markPos3);
				move.setCapture();
				move.captured = state.board[markPos3];
				list.push_back(move);
			}
		}
		else {
			Move move(piece, startPos, markPos3);
			list.push_back(move);
		}
	}
}

void knightMove(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos) {
	knightMoveDir(list, state, piece, startPos, UP, RIGHT, LEFT);
	knightMoveDir(list, state, piece, startPos, DOWN, RIGHT, LEFT);
	knightMoveDir(list, state, piece, startPos, RIGHT, UP, DOWN);
	knightMoveDir(list, state, piece, startPos, LEFT, UP, DOWN);
}

void movegeneration::move(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range) {
	if (range == 0) {
		range = 10;
	}

	for (uint_least16_t di = 1; di < (1 << 8); di <<= 1) { // loop through the first 8 directions, and check if dirs includes that bit
		DIRECTION direction = (DIRECTION)di;
		POSITION markPos = startPos;
		if ((dirs & direction) != 0) {
			for (size_t i = 0; i < range; i++)
			{
				if (!movement::ableToMove(markPos, direction)) {
					break;
				}
				else {
					markPos = movement::moveTo(markPos, direction);
					if (state.board[markPos] != EMPTY) {
						break;
					}
					Move move(piece, startPos, markPos);
					list.push_back(move);
				}
			}
		}
	}
}

void movegeneration::moveOrCapture(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range) {
	if (range == 0) {
		range = 10;
	}

	if ((dirs & DIRECTION::KNIGHT) != 0) {
		knightMove(list, state, piece, startPos);
		return;
	}

	for (uint_least16_t di = 1; di < (1 << 8); di <<= 1) {
		DIRECTION direction = (DIRECTION)di;
		POSITION markPos = startPos;
		if ((dirs & direction) != 0) {
			for (size_t i = 0; i < range; i++)
			{
				if (!movement::ableToMove(markPos, direction)) {
					break;
				}
				else {
					markPos = movement::moveTo(markPos, direction);
					if (state.board[markPos] != EMPTY) {
						Move move(piece, startPos, markPos);
						if (state.getPiece(markPos)->getColor() == piece->getColor()) {
							break;
						}
						move.setCapture();
						move.captured = state.board[markPos];
						list.push_back(move);
						break;
					}
					else {
						Move move(piece, startPos, markPos);
						list.push_back(move);
					}
				}
			}
		}
	}
}

void movegeneration::capture(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos, uint_least16_t dirs, size_t range) {
	if (range == 0) {
		range = 10;
	}

	for (uint_least16_t di = 1; di < (1 << 8); di <<= 1) {
		DIRECTION direction = (DIRECTION)di;
		POSITION markPos = startPos;
		if ((dirs & direction) != 0) {
			for (size_t i = 0; i < range; i++)
			{
				if (!movement::ableToMove(markPos, direction)) {
					break;
				}
				else {
					markPos = movement::moveTo(markPos, direction);
					if (state.board[markPos] == EMPTY) {
						break;
					}
					else {
						if (state.getPiece(markPos)->getColor() == piece->getColor()) {
							break;
						}
						Move move(piece, startPos, markPos);
						move.setCapture();
						move.captured = state.board[markPos];
						list.push_back(move);
					}
				}
			}
		}
	}
}

bool checkAttackedSquares(const State& state, COLOR col, const POSITION* posList, size_t len) {
	State bluffState(state);
	for (size_t i = 0; i < len; i++) {
		POSITION pos = posList[i];
		bluffState.board[pos] = col == WHITE ? BPAWN : WPAWN;
	}

	for (int i = 0; i < 64;i++) {
		auto p = state.getPiece((POSITION)i);
		if (p && p->getColor() == col) {
			std::vector<PossibleMove> posMoves;
			p->getPossibleMoves(posMoves, bluffState, Move(pieces[WPAWN], A5, B5), (POSITION)i);
			for each (auto pos in posMoves)
			{
				for (size_t i = 0; i < len; i++) {
					POSITION positon = posList[i];
					if (pos.move.endPos == positon) {
						return false;
					}
				}
			}
		}
	}

	return true;
}

void movegeneration::checkCastling(std::vector<PossibleMove>& list, const State& state, const Piece* piece, POSITION startPos) {
	if (piece->getColor() == WHITE && startPos == E1) {
		if (state.getWhiteShortCastlingAllowed()) {
			if (state.board[E1] == WKING &&
				state.board[F1] == EMPTY &&
				state.board[G1] == EMPTY &&
				state.board[H1] == WROOK)
			{
				static const POSITION poses[2] = {F1, G1};
				if (checkAttackedSquares(state, (COLOR)(!piece->getColor()), poses, sizeof(poses))) {
					Move move(piece, startPos, G1);
					move.setCastlingShort();
					list.push_back(move);
				}
			}
		}if (state.getWhiteLongCastlingAllowed()) {
			if (state.board[E1] == WKING &&
				state.board[D1] == EMPTY &&
				state.board[C1] == EMPTY &&
				state.board[B1] == EMPTY &&
				state.board[A1] == WROOK) 
			{
				static const POSITION poses[2] = { D1, C1 };
				if (checkAttackedSquares(state, (COLOR)(!piece->getColor()), poses, sizeof(poses))) {
					Move move(piece, startPos, C1);
					move.setCastlingLong();
					list.push_back(move);
				}
			}
		}
	}
	else if (piece->getColor() == BLACK && startPos == E8) {
		if (state.getBlackShortCastlingAllowed()) {
			if (state.board[E8] == BKING &&
				state.board[F8] == EMPTY &&
				state.board[G8] == EMPTY &&
				state.board[H8] == BROOK)
			{
				static const POSITION poses[2] = { F8, G8 };
				if (checkAttackedSquares(state, (COLOR)(!piece->getColor()), poses, sizeof(poses))) {
					Move move(piece, startPos, G8);
					move.setCastlingShort();
					list.push_back(move);
				}
			}
		}if (state.getBlackLongCastlingAllowed()) {
			if (state.board[E8] == BKING &&
				state.board[D8] == EMPTY &&
				state.board[C8] == EMPTY &&
				state.board[B8] == EMPTY &&
				state.board[A8] == BROOK)
			{
				static const POSITION poses[2] = { D8, C8 };
				if (checkAttackedSquares(state, (COLOR)(!piece->getColor()), poses, sizeof(poses))) {
					Move move(piece, startPos, C8);
					move.setCastlingLong();
					list.push_back(move);
				}
			}
		}
	}
}

void movegeneration::checkEnPassant(std::vector<PossibleMove>& list, const State& state, const Move& lastOpponentMove, const Piece* piece, POSITION startPos) {
	if (piece->getColor() == WHITE && lastOpponentMove.piece == BPAWN && State::getRank(startPos) == 5) {
		if (State::getRank(lastOpponentMove.startPos) == 7 && State::getRank(lastOpponentMove.endPos) == 5) {
			if (movement::ableToMoveRight(startPos)) {
				POSITION markPos = movement::moveRight(startPos);
				if (markPos == lastOpponentMove.endPos) {
					markPos = movement::moveUp(markPos);
					Move move(piece, startPos, markPos);
					move.setEnPassantRight();
					move.setCapture();
					move.captured = BPAWN;
					list.push_back(move);
				}
			}
			if (movement::ableToMoveLeft(startPos)) {
				POSITION markPos = movement::moveLeft(startPos);
				if (markPos == lastOpponentMove.endPos) {
					markPos = movement::moveUp(markPos);
					Move move(piece, startPos, markPos);
					move.setEnPassantLeft();
					move.setCapture();
					move.captured = BPAWN;
					list.push_back(move);
				}
			}
		}
	}else if (piece->getColor() == BLACK && lastOpponentMove.piece == WPAWN && State::getRank(startPos) == 4) {
		if (State::getRank(lastOpponentMove.startPos) == 2 && State::getRank(lastOpponentMove.endPos) == 4) {
			if (movement::ableToMoveRight(startPos)) {
				POSITION markPos = movement::moveRight(startPos);
				if (markPos == lastOpponentMove.endPos) {
					markPos = movement::moveDown(markPos);
					Move move(piece, startPos, markPos);
					move.setEnPassantRight();
					move.setCapture();
					move.captured = WPAWN;
					list.push_back(move);
				}
			}
			if (movement::ableToMoveLeft(startPos)) {
				POSITION markPos = movement::moveLeft(startPos);
				if (markPos == lastOpponentMove.endPos) {
					markPos = movement::moveDown(markPos);
					Move move(piece, startPos, markPos);
					move.setEnPassantLeft();
					move.setCapture();
					move.captured = WPAWN;
					list.push_back(move);
				}
			}
		}
	}
}

void movegeneration::checkPromotionForPawnMoves(std::vector<PossibleMove>& list, const Piece* piece, POSITION startPos) {
	//std::vector<PossibleMove> toAdd;

	std::vector< PossibleMove >::iterator it = list.begin();
	auto end = list.end();
	while (it != end) {
		PossibleMove p = *it;
		bool skipIt = false;

		if (piece->getCode() == WPAWN && State::getRank(p.move.endPos) == 8) {
			it._Ptr->move.setPromotion();
			it._Ptr->move.promotedTo = WQUEEN;

			/*
			p.move.setPromotion();
			p.move.promotedTo = WQUEEN;
			Move m2(p.move);
			m2.promotedTo = WKNIGHT;
			Move m3(p.move);
			m2.promotedTo = WROOK;
			Move m4(p.move);
			m2.promotedTo = WBISHOP;
			toAdd.push_back(p);
			toAdd.push_back(m2);
			toAdd.push_back(m3);
			toAdd.push_back(m4);
			it = list.erase(it);
			skipIt = true;*/
		}
		else if (piece->getCode() == BPAWN && State::getRank(p.move.endPos) == 1) {

			it._Ptr->move.setPromotion();
			it._Ptr->move.promotedTo = BQUEEN;
			/*
			p.move.setPromotion();
			p.move.promotedTo = BQUEEN;
			Move m2(p.move);
			m2.promotedTo = BKNIGHT;
			Move m3(p.move);
			m2.promotedTo = BROOK;
			Move m4(p.move);
			m2.promotedTo = BBISHOP;
			toAdd.push_back(p);
			toAdd.push_back(m2);
			toAdd.push_back(m3);
			toAdd.push_back(m4);

			it = list.erase(it);
			skipIt = true;*/
		}

		/* (!skipIt)*/ ++it;
	}
	/*
	for each (auto m in toAdd)
	{
		list.push_back(m);
	}*/
	
}


void movegeneration::pruneOutKingDangers(std::vector<PossibleMove>& list, const State& state) {
	PIECE kingPiece = state.getTurnColor() == WHITE ? WKING : BKING;

	std::vector< PossibleMove >::iterator it = list.begin();

	while (it != list.end()) {
		PossibleMove posMove = *it;

		auto newState = state.advanceTurn(posMove.move);
		std::vector<PossibleMove> allEnemyPossibleMoves;
		POSITION kingPos;
		for (size_t i = 0; i < 64; i++)
		{
			POSITION pos = (POSITION)i;
			auto pc = newState.getPiece(pos);
			if (!pc)
				continue;
			if (pc->getCode() == kingPiece) {
				kingPos = pos;
			}
			if (pc->getColor() == newState.getTurnColor()) {
				pc->getPossibleMoves(allEnemyPossibleMoves, newState, posMove.move, pos);
			}
		}

		bool skipIt = false;
		for each (auto enemyPosMove in allEnemyPossibleMoves)
		{
			if (enemyPosMove.move.endPos == kingPos) {
				it = list.erase(it);
				skipIt = true;
				break;
			}
		}
		if(!skipIt) ++it;
	}
}




