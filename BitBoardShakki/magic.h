#pragma once
#include "enums.h"
#include <vector>


/*
*	CODE IS MOSTLY FROM https://github.com/goutham/magic-bits
*	Huge props to the original author Goutham Bhat
*/

namespace magic {
	static const int kSquares = 64;

	static const char kMagicFolder[] = "magic";
	static const char kRookMagics[] = "magic/rook_magics.magic";
	static const char kRookMasks[] = "magic/rook_masks.magic";
	static const char kRookShifts[] = "magic/rook_shifts.magic";
	static const char kRookOffsets[] = "magic/rook_offsets.magic";
	static const char kRookAttackTable[] = "magic/rook_attack_table.magic";

	static const char kBishopMagics[] = "magic/bishop_magics.magic";
	static const char kBishopMasks[] = "magic/bishop_masks.magic";
	static const char kBishopShifts[] = "magic/bishop_shifts.magic";
	static const char kBishopOffsets[] = "magic/bishop_offsets.magic";
	static const char kBishopAttackTable[] = "magic/bishop_attack_table.magic";

	struct Magic {
		U64 mask;
		U64 magic;
		int shift;
		int offset;
	};

	class MagicSlideGenerator {
	public:
		static MagicSlideGenerator& getInstance()
		{
			static MagicSlideGenerator instance;

			return instance;
		}
		inline U64 RookAttacks(const U64 occupied, const int index) const {
			const Magic& m = rook_magics_[index];
			return rook_attack_table_[AttackTableIndex(occupied, m)];
		}

		inline U64 BishopAttacks(const U64 occupied, const int index) const {
			const Magic& m = bishop_magics_[index];
			return bishop_attack_table_[AttackTableIndex(occupied, m)];
		}

		inline U64 QueenAttacks(const U64 occupied, const int index) const {
			return RookAttacks(occupied, index) | BishopAttacks(occupied, index);
		}

		MagicSlideGenerator(MagicSlideGenerator const&) = delete;
		void operator=(MagicSlideGenerator const&) = delete;

	private:
		MagicSlideGenerator();

		bool CheckFiles();
		void Initialize();
		void FindMagics();

		U64 AttackTableIndex(const U64 bitboard, const Magic& m) const {
			U64 occupancy = bitboard & m.mask;
			return ((occupancy * m.magic) >> (kSquares - m.shift)) + m.offset;
		}

		Magic rook_magics_[kSquares];
		Magic bishop_magics_[kSquares];
		std::vector<U64> rook_attack_table_;
		std::vector<U64> bishop_attack_table_;
	};
}
