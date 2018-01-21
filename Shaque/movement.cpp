#include "movement.h"

namespace movement {

	bool ableToMove(POSITION pos, DIRECTION dir) {
		switch (dir)
		{
		case UP:
			return ableToMoveUp(pos);
			break;
		case DOWN:
			return ableToMoveDown(pos);
			break;
		case LEFT:
			return ableToMoveLeft(pos);
			break;
		case RIGHT:
			return ableToMoveRight(pos);
			break;
		case UPLEFT:
			return ableToMoveTopLeft(pos);
			break;
		case UPRIGHT:
			return ableToMoveTopRight(pos);
			break;
		case DOWNLEFT:
			return ableToMoveDownLeft(pos);
			break;
		case DOWNRIGHT:
			return ableToMoveDownRight(pos);
			break;
		default:
			return false;
			break;
		}
	}

	POSITION moveTo(POSITION pos, DIRECTION dir) {
		switch (dir)
		{
		case UP:
			return moveUp(pos);
			break;
		case DOWN:
			return moveDown(pos);
			break;
		case LEFT:
			return moveLeft(pos);
			break;
		case RIGHT:
			return moveRight(pos);
			break;
		case UPLEFT:
			return moveTopLeft(pos);
			break;
		case UPRIGHT:
			return moveTopRight(pos);
			break;
		case DOWNLEFT:
			return moveDownLeft(pos);
			break;
		case DOWNRIGHT:
			return moveDownRight(pos);
			break;
		default:
			return pos;
			break;
		}
	}
}