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
