#pragma once
#include "statebb.h"

class MoveGenerator
{
public:
	static MoveGenerator& getInstance()
	{
		static MoveGenerator instance; 
							  
		return instance;
	}
private:
	MoveGenerator();
public:
	MoveGenerator(MoveGenerator const&) = delete;
	void operator=(MoveGenerator const&) = delete;

	MoveSet GenPseudoLegalMoves(const State& state, U64 dangerSquares = 0)const;
	MoveSet GenLegalMoves(const State& state)const;
	U64 DangerSquares(const State& state, COLOR eCol) const;

};