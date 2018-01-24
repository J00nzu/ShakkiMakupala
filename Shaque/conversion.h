#pragma once
#include <string>
#include <codecvt>
#include "enums.h"

std::wstring str_to_wstr(const std::string& input);
bool strToPos(const std::wstring& string, POSITION &pos);
std::wstring posToStr(POSITION pos);