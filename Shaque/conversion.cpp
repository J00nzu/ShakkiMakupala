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
	str += std::to_wstring(rank);
	str += ch;

	return str;
}