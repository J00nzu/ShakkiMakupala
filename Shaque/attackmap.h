#pragma once
#include "enums.h"

struct AttackMap{
	uint_least64_t map;
	void setAttacked(POSITION pos);
	void isAttacked(POSITION pos);
};