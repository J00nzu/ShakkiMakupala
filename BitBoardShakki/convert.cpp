#include "convert.h"
#include "bitboardops.h"
#include <sstream>
#include <codecvt>

using namespace std;

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

std::wstring bitBtoStr(U64 board) {
	wstringstream stream;

	for (int y = 7; y >= 0; y--) {
		for (int x = 0; x < 8; x++) {
			U64 mask = 1ULL << (y*8+x);
			stream << L' ';
			if ((board & mask) != 0) {
				stream << L'1';
			}
			else {
				stream << L'0';
			}
			stream << L' ';
		}
		stream << L'\n';
	}

	return stream.str();
}


std::wstring bitPosToStr(BITPOS pos) {
	int idx = bitB_bitScanForward(pos);
	int rank = bitP_getRank(pos);

	int rem = idx % 8;
	wchar_t file = L'A';
	switch (rem)
	{
	case 0:
		file = L'A';
		break;
	case 1:
		file = L'B';
		break;
	case 2:
		file = L'C';
		break;
	case 3:
		file = L'D';
		break;
	case 4:
		file = L'E';
		break;
	case 5:
		file = L'F';
		break;
	case 6:
		file = L'G';
		break;
	case 7:
		file = L'H';
		break;
	default:
		break;
	}

	std::wstringstream strm;
	strm << file << rank;
	return strm.str();
}

bool strToPos(std::wstring str, BITPOS &pos) {
	const wchar_t *cstr = str.c_str();
	if (str.length() < 2)
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

	U64 p = 1ULL << (y * 8 + x);

	pos = BITPOS(p);
	return true;
}


bool strToMove(const std::wstring& string, Move &move, COLOR col) {
	if ((string.compare(L"O-O-O") == 0) || (string.compare(L"0-0-0") == 0)) {
		if (col == WHITE) {
			move = Move(bE1, bC1, Wking, Move::QUEEN_CASTLE);
		}
		else {
			move = Move(bE8, bC8, Bking, Move::QUEEN_CASTLE);
		}
		return true;
	}
	else if ((string.compare(L"O-O") == 0) || (string.compare(L"0-0") == 0)) {
		if (col == WHITE) {
			move = Move(bE1, bG1, Wking, Move::KING_CASTLE);
		}
		else {
			move = Move(bE8, bG8, Bking, Move::KING_CASTLE);
		}
		return true;
	}
	else if (string.length() == 5) {
		BITPOS startPos;
		if (!strToPos(string.substr(0),startPos)) {
			return false;
		}
		BITPOS endPos;
		if (!strToPos(string.substr(3), endPos)) {
			return false;
		}
		move = Move(startPos, endPos, Wpawn, Move::QUIET);
		return true;
	}
	else if (string.length() == 6 && (string.c_str()[2]==L'-' || string.c_str()[2] == L'x')) {
		BITPOS startPos;
		if (!strToPos(string.substr(0), startPos)) {
			return false;
		}
		BITPOS endPos;
		if (!strToPos(string.substr(3), endPos)) {
			return false;
		}
		PIECE promop;
		switch (string.c_str()[5]) {
		case L'Q':
			if (col == WHITE) {
				move = Move(startPos, endPos, Wpawn, Move::PROMOTION_QUEEN);
			}
			else {
				move = Move(startPos, endPos, Bpawn, Move::PROMOTION_QUEEN);
			}
			break;
		case L'N':
			if (col == WHITE) {
				move = Move(startPos, endPos, Wpawn, Move::PROMOTION_KNIGHT);
			}
			else {
				move = Move(startPos, endPos, Bpawn, Move::PROMOTION_KNIGHT);
			}
			break;
		case L'B':
			if (col == WHITE) {
				move = Move(startPos, endPos, Wpawn, Move::PROMOTION_BISHOP);
			}
			else {
				move = Move(startPos, endPos, Bpawn, Move::PROMOTION_BISHOP);
			}
			break;
		case L'R':
			if (col == WHITE) {
				move = Move(startPos, endPos, Wpawn, Move::PROMOTION_ROOK);
			}
			else {
				move = Move(startPos, endPos, Bpawn, Move::PROMOTION_ROOK);
			}
			break;
		default:
			return false;
		}
		return true;
	}
	else if (string.length() == 6) {
		BITPOS startPos;
		if (!strToPos(string.substr(1), startPos)) {
			return false;
		}
		BITPOS endPos;
		if (!strToPos(string.substr(4), endPos)) {
			return false;
		}

		move = Move(startPos, endPos, Wpawn, Move::QUIET);
		return true;
	}

	return false;
}

wchar_t pieceToChar(PIECE piece) {
	switch (piece)
	{
	case Wrook:
		return L'R';
		break;
	case Wknight:
		return L'N';
		break;
	case Wbishop:
		return L'B';
		break;
	case Wqueen:
		return L'Q';
		break;
	case Wking:
		return L'K';
		break;
	case Wpawn:
		return L'P';
		break;
	case Brook:
		return L'R';
		break;
	case Bknight:
		return L'N';
		break;
	case Bbishop:
		return L'B';
		break;
	case Bqueen:
		return L'Q';
		break;
	case Bking:
		return L'K';
		break;
	case Bpawn:
		return L'P';
		break;
	default:
		return L' ';
		break;
	}
}

std::wstring moveToStr(const Move& move) {
	auto type = move.getMoveType();
	if (type == Move::KING_CASTLE) {
		return L"0-0";
	}
	else if (type == Move::QUEEN_CASTLE) {
		return L"0-0-0";
	}

	std::wstring res;
	res += pieceToChar(move.getPiece());
	res += bitPosToStr(move.getFromBB());
	res += L"-";
	res += bitPosToStr(move.getToBB());
	if (move.isPromotion()) {
		res += pieceToChar(move.getPromotedPiece());
	}

	return res;
}