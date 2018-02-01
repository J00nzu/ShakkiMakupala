#pragma once
#include <string>
#include <codecvt>
#include "enums.h"
#include "move.h"

std::wstring str_to_wstr(const std::string& input);
bool strToPos(const std::wstring& string, POSITION &pos);
std::wstring posToStr(POSITION pos);
std::wstring posToStr(POSITION pos, bool lowercase);


bool strToMove(const std::wstring& string, Move &move, COLOR col);
std::wstring moveToStr(const Move& move);