#include "bitboardops.h"
#include "convert.h"
#include "statebb.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "ui.h"
#include "enums.h"
#include "movegen.h"
#include "magic.h"
#include <ctime>
#include <windows.h>


using namespace std;

int main2() {
	/*
	State st;
	st.clear();
	st.setTurnWhite();
	//st.setWhiteLongCastlingAllowed();
	//st.setWhiteShortCastlingAllowed();
	//st.setBlackLongCastlingAllowed();
	//st.setBlackShortCastlingAllowed();
	st.pieceBB[Wking] = bE1;
	st.pieceBB[Wrook] = bE2;
	st.pieceBB[Brook] = bE7;
	st.pieceBB[Bbishop] = bH4;


	st.rebuildBBs();

	UI::singleton->drawBoard(st);

	MoveSet set = MoveGenerator::getInstance().GenLegalMoves(st, Move(bB2, bB4, Wpawn, Move::PAWN_DOUBLE_PUSH));
	set.sort();

	for each (Move mv in set)
	{
		wcout << mv.to_str() << endl;
	}

	st = st.advanceTurn(set[0]);

	UI::singleton->drawBoard(st);

	wcout << bitBtoStr(st.emptyBB) << endl << endl;
	wcout << bitBtoStr(st.occupiedBB) << endl << endl;
	wcout << bitBtoStr(st.colorBB[WHITE]) << endl << endl;
	wcout << bitBtoStr(st.colorBB[BLACK]) << endl << endl;




	system("pause");
	return 0;
	*/

	State st = State::initialize();
	MoveSet set;
	
	//Init slide generator
	magic::MagicSlideGenerator::getInstance();

	Move mv1(bA2, bA7, Wpawn, Move::CAPTURE, Bpawn);
	Move mv2(bA2, bA7, Wpawn, Move::QUIET);
	Move mv3(bA2, bA7, Wpawn, Move::PROMOTION_KNIGHT);
	Move mv4(bA2, bA7, Wpawn, Move::PROMOTION_QUEEN);
	Move mv5(bA2, bA7, Wpawn, Move::CAPTURE_PROMOTION_QUEEN, Bpawn);

	clock_t begin = clock();

	for(int i=0; i < 10; i++){
		set.push_back(mv1);
		set.push_back(mv2);
		set.push_back(mv3);
		set.push_back(mv4);
		set.push_back(mv5);
	}

	set = MoveGenerator::getInstance().GenPseudoLegalMoves(st, Move());

	for each (Move mv in set)
	{
		wcout << mv.to_str() << endl;
	}

	wcout << endl;

	auto nu = MoveSet(set);
	State newState = State(st);
	Move lastMove;
	for (long long i = 0; i < 5000000; i++) {
		//nu = MoveSet(set);
		if (nu.empty() || (i % 50) == 0) {
			newState = State::initialize();
		}
		nu = MoveGenerator::getInstance().GenLegalMoves(newState,lastMove);
		nu.sort();
		
		for each (Move mv in nu)
		{
			lastMove = mv;
			newState = newState.advanceTurn(mv);
			
			break;
		}
		//UI::singleton->drawBoard(newState);
		//Sleep(100);
	}

	for each (Move mv in nu)
	{
		wcout << mv.to_str() << endl;
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	wcout << L"Elapsed time was " << elapsed_secs << endl;
	system("pause");

	return 0;

	wcout << bitBtoStr(st.pieceBB[Wpawn]) << endl;
	wcout << bitBtoStr(st.pieceBB[Bpawn]) << endl;

	UI::singleton->drawBoard(st);


	Move mv(bA2, bA7, Wpawn, Move::CAPTURE, Bpawn);
	wcout << bitP_getRank(bA2) << L", " << bitP_getRank(bA7) << endl;

	st = st.advanceTurn(mv);
	
	wcout << bitBtoStr(st.pieceBB[Wpawn]) << endl;
	wcout << bitBtoStr(st.pieceBB[Bpawn]) << endl;

	UI::singleton->drawBoard(st);


	system("pause");
	return 0;
}