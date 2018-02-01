#pragma once
#include "state.h"

class StateEvaluator {
public:
	virtual int evaluate(const State& state) const = 0;
};

class DefaultStateEvaluator : public StateEvaluator {
public:
	int evaluate(const State& state) const;
};