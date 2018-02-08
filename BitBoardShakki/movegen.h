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

	MoveSet GenPseudoLegalMoves(const State& state, const Move& lastOpponentMove, U64 dangerSquares = 0)const;
	MoveSet GenLegalMoves(const State& state, const Move& lastOpponentMove)const;

};