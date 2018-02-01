#include "conversion.h"

std::wstring str_to_wstr(const std::string& input)
{
	try
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(input);
	}
	catch (std::range_error& e)
	{
		e.what();
		size_t length = input.length();
		std::wstring result;
		result.reserve(length);
		for (size_t i = 0; i < length; i++)
		{
			result.push_back(input[i] & 0xFF);
		}
		return result;
	}
}

bool strToPos(const std::wstring& string, POSITION &pos) {
	const wchar_t *cstr = string.c_str();
	if (string.length() < 2)
		return false;

	int x, y;
	if ((int)cstr[0] < 97) {
		x = (int)cstr[0] - 65;
		y = (int)cstr[1] - 49;
	}
	else {
		x = (int)cstr[0] - 97;
		y = (int)cstr[1] - 49;
	}

	if (
		x < 0 || x > 7 ||
		y < 0 || y > 7
		)
		return false;

	int p = (7 - y) * 8 + x;

	pos = POSITION(p);
	return true;
}

std::wstring posToStr(POSITION pos) {
	int posi = pos;
	int rank = 8 - (int)floor(pos / 8.0);

	int rem = pos % 8;
	wchar_t ch = (wchar_t)(65 + rem);

	std::wstring str;
	str += ch;
	str += std::to_wstring(rank);

	return str;
}


std::wstring posToStr(POSITION pos, bool lowercase) {
	int posi = pos;
	int rank = 8 - (int)floor(pos / 8.0);

	int rem = pos % 8;
	wchar_t ch = (wchar_t)(65 + rem);
	if (lowercase) {
		ch = (wchar_t)(97 + rem);
	}

	std::wstring str;
	str += ch;
	str += std::to_wstring(rank);

	return str;
}


bool strToMove(const std::wstring& string, Move &move, COLOR col) {
	if ((string.compare(L"O-O-O") == 0) || (string.compare(L"0-0-0") == 0)) {
		move.setCastlingLong();
		if (col == WHITE) {
			move.startPos = E1;
			move.endPos = C1;
		}
		else {
			move.startPos = E8;
			move.endPos = C8;
		}
		return true;
	}
	else if ((string.compare(L"O-O") == 0) || (string.compare(L"0-0") == 0)) {
		move.setCastlingShort();
		if (col == WHITE) {
			move.startPos = E1;
			move.endPos = G1;
		}
		else {
			move.startPos = E8;
			move.endPos = G8;
		}
		return true;
	}
	else if (string.length() == 6) {
		POSITION startPos;
		if (!strToPos(string.substr(1), startPos)) {
			return false;
		}
		POSITION endPos;
		if (!strToPos(string.substr(4), endPos)) {
			return false;
		}
		move.startPos = startPos;
		move.endPos = endPos;
		return true;
	}
	else if (string.length() == 7) {
		move.setPromotion();
		POSITION startPos;
		if (!strToPos(string.substr(1), startPos)) {
			return false;
		}
		POSITION endPos;
		if (!strToPos(string.substr(3), endPos)) {
			return false;
		}
		switch (string.c_str()[6]) {
		case L'Q':
			move.promotedTo = col == WHITE ? WQUEEN : BQUEEN;
			break;
		case L'N':
			move.promotedTo = col == WHITE ? WKNIGHT : BKNIGHT;
			break;
		case L'B':
			move.promotedTo = col == WHITE ? WBISHOP : BBISHOP;
			break;
		case L'R':
			move.promotedTo = col == WHITE ? WROOK : BROOK;
			break;
		default:
			return false;
		}
		return true;
	}

	return false;
}

wchar_t pieceToChar(PIECE piece) {
	switch (piece)
	{
	case WROOK:
		return L'R';
		break;
	case WKNIGHT:
		return L'N';
		break;
	case WBISHOP:
		return L'B';
		break;
	case WQUEEN:
		return L'Q';
		break;
	case WKING:
		return L'K';
		break;
	case WPAWN:
		return L'P';
		break;
	case BROOK:
		return L'R';
		break;
	case BKNIGHT:
		return L'N';
		break;
	case BBISHOP:
		return L'B';
		break;
	case BQUEEN:
		return L'Q';
		break;
	case BKING:
		return L'K';
		break;
	case BPAWN:
		return L'P';
		break;
	default:
		return L' ';
		break;
	}
}

std::wstring moveToStr(const Move& move) {
	if (move.getCastlingShort()) {
		return L"0-0";
	}
	else if (move.getCastlingLong()) {
		return L"0-0-0";
	}

	std::wstring res;
	res += pieceToChar(move.piece);
	res += posToStr(move.startPos, true);
	res += L"-";
	res += posToStr(move.endPos, true);
	if (move.getPromotion()) {
		res += pieceToChar(move.promotedTo);
	}

	return res;
}