#include "stateevaluator.h"


int DefaultStateEvaluator::evaluate(const State& state) const {
	int score = 0;
	for (int i = 0; i < 64; i++) {
		switch (state.board[i])
		{
		case EMPTY:
			break;
		case WPAWN:
			score += 10+State::getRank((POSITION)i);
			break;
		case BPAWN:
			score += -(10 + (9 - State::getRank((POSITION)i)));
			break;
		case WKNIGHT:
			score += 30;
			break;
		case WBISHOP:
			score += 30;
			break;
		case WROOK:
			score += 40;
			break;
		case BKNIGHT:
			score += -30;
			break;
		case BBISHOP:
			score += -30;
			break;
		case BROOK:
			score += -40;
			break;
		case WKING:
			score += 900;
			break;
		case WQUEEN:
			score += 90;
			break;
		case BKING:
			score += -900;
			break;
		case BQUEEN:
			score += -90;
			break;
		default:
			break;
		}
	}

	return score;
}