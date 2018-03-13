#include "hash.h"
#include <random>
#include <limits>
#include "statebb.h"

namespace zobhash {

	uint64_t ZobristHashing::pieces_squares[12][64];
	uint64_t ZobristHashing::black_turn;
	uint64_t ZobristHashing::castling_rights[4];
	uint64_t ZobristHashing::ep_file[8];

	uint64_t Rand64() {
		static std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
		static std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
		static std::uniform_int_distribution<unsigned long long> distr;

		return (uint64_t)distr(eng);
	}

	ZobristHashing::ZobristHashing() {
		for (int p = 0; p < 12; p++) {
			for (int s = 0; s < 64; s++) {
				pieces_squares[p][s] = Rand64();
			}
		}

		black_turn = Rand64();

		for (int i = 0; i < 4;i++) {
			castling_rights[i] = Rand64();
		}

		for (int i = 0; i < 8;i++) {
			ep_file[i] = Rand64();
		}
	}

};

void StateHash::reconstruct(const State& state) {
	static zobhash::ZobristHashing& zobber = zobhash::ZobristHashing::getInstance();
	hash = 0;
	for (int i = 0; i < 64; i++) {
		U64 pos = 1ULL << (i);
		auto piec = state.getPiece((BITPOS)pos);
		if (piec) {
			PIECE p = piec->getCode();
			hash ^= zobber.getPieceSquare(p, i);
		}
	}
	if (state.getWhiteShortCastlingAllowed()) hash ^= zobber.getCastlingRights(zobhash::W_K_CASTLE);
	if (state.getWhiteLongCastlingAllowed()) hash ^= zobber.getCastlingRights(zobhash::W_Q_CASTLE);
	if (state.getBlackShortCastlingAllowed()) hash ^= zobber.getCastlingRights(zobhash::B_K_CASTLE);
	if (state.getBlackLongCastlingAllowed()) hash ^= zobber.getCastlingRights(zobhash::B_Q_CASTLE);
	if (state.getTurnColor() == BLACK) hash ^= zobber.getBlackTurn();
	if (state.ep_file != NOFILE) hash ^= zobber.getEpFile(state.ep_file);
}