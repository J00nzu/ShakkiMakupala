#pragma once
#include "enums.h"
#include "move.h"
#include <string>



bool strToMove(const std::wstring& string, Move &move, COLOR col);
std::wstring moveToStr(const Move& move);
std::wstring str_to_wstr(const std::string& input);
std::wstring bitBtoStr(U64);
std::wstring bitPosToStr(BITPOS);