#pragma once
#include "enums.h"


#define M_UP -8
#define M_DOWN 8
#define M_LEFT -1
#define M_RIGHT 1


namespace movement {

	inline bool ableToMoveLeft(POSITION pos) {
		return (pos % 8 != 0);
	}

	inline bool ableToMoveRight(POSITION pos) {
		return (((pos % 8) != 7) || pos == 0);
	}

	inline bool ableToMoveUp(POSITION pos) {
		return pos > 7;
	}

	inline bool ableToMoveDown(POSITION pos) {
		return pos < 55;
	}

	inline bool ableToMoveTopLeft(POSITION pos) {
		return ableToMoveUp(pos) && ableToMoveLeft(pos);
	}
	inline bool ableToMoveTopRight(POSITION pos) {
		return ableToMoveUp(pos) && ableToMoveRight(pos);
	}
	inline bool ableToMoveDownLeft(POSITION pos) {
		return ableToMoveDown(pos) && ableToMoveLeft(pos);
	}
	inline bool ableToMoveDownRight(POSITION pos) {
		return ableToMoveDown(pos) && ableToMoveRight(pos);
	}

	bool ableToMove(POSITION pos, DIRECTION dir);

	inline POSITION moveLeft(POSITION pos) {
		return (POSITION)(pos + M_LEFT);
	}
	inline POSITION moveRight(POSITION pos) {
		return (POSITION)(pos + M_RIGHT);
	}
	inline POSITION moveUp(POSITION pos) {
		return (POSITION)(pos + M_UP);
	}
	inline POSITION moveDown(POSITION pos) {
		return (POSITION)(pos + M_DOWN);
	}

	inline POSITION moveTopLeft(POSITION pos) {
		return (POSITION)(pos + M_LEFT + M_UP);
	}

	inline POSITION moveTopRight(POSITION pos) {
		return (POSITION)(pos + M_RIGHT + M_UP);
	}
	inline POSITION moveDownLeft(POSITION pos) {
		return (POSITION)(pos + M_LEFT + M_DOWN);
	}
	inline POSITION moveDownRight(POSITION pos) {
		return (POSITION)(pos + M_RIGHT + M_DOWN);
	}

	POSITION moveTo(POSITION pos, DIRECTION dir);

};