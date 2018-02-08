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