#include "magic.h"
#include "convert.h"

#include <string>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace magic {
#ifdef _DEBUG
	void RunChecks(const MagicSlideGenerator& instance) {
		const MagicSlideGenerator* slider_attacks = &instance;
		const U64 bitboard = 0xF7F6F3748CA5B610ULL;
		std::wcout << L"Running magic checks..." << std::endl;
		U64 attacks = slider_attacks->QueenAttacks(bitboard, 36);
		assert(attacks == 0x4382c38509000ULL);
		attacks = slider_attacks->QueenAttacks(bitboard, 29);
		assert(attacks == 0x70d8708804ULL);
		std::wcout << L"Magic checks passed!" << std::endl;
	}
#endif // DEBUG

	MagicSlideGenerator::MagicSlideGenerator() {
		if (!CheckFiles()) {
			FindMagics();
		}
		Initialize();
#ifdef _DEBUG
		RunChecks(*this);
#endif // DEBUG
	}

	bool CheckFile(const std::string& filename) {
		std::ifstream f(filename.c_str());
		return f.good();
	}

	bool MagicSlideGenerator::CheckFiles() {
		return
			CheckFile(kRookMasks) &&
			CheckFile(kRookMagics) &&
			CheckFile(kRookOffsets) &&
			CheckFile(kRookAttackTable) &&
			CheckFile(kRookShifts) &&

			CheckFile(kBishopMasks) &&
			CheckFile(kBishopMagics) &&
			CheckFile(kBishopOffsets) &&
			CheckFile(kBishopAttackTable) &&
			CheckFile(kBishopShifts);
	}

	template <typename T>
	void ReadFromFile(const std::string& filename, std::vector<T>* v) {
		std::ifstream ifs(filename.c_str(), std::ios::in);
		if (!ifs.is_open()) {
			std::wcerr << L"Unable to open file " << str_to_wstr(filename) << std::endl;
			exit(-1);
		}
		std::string s;
		while (ifs >> s) {
			v->push_back(T(std::strtoull(s.c_str(), nullptr, 16)));
		}
		ifs.close();
	}

	void MagicSlideGenerator::Initialize() {
		std::vector<U64> rook_masks;
		ReadFromFile(kRookMasks, &rook_masks);
		assert(rook_masks.size() == kSquares);

		std::vector<U64> bishop_masks;
		ReadFromFile(kBishopMasks, &bishop_masks);
		assert(bishop_masks.size() == kSquares);

		std::vector<U64> rook_magics;
		ReadFromFile(kRookMagics, &rook_magics);
		assert(rook_magics.size() == kSquares);

		std::vector<U64> bishop_magics;
		ReadFromFile(kBishopMagics, &bishop_magics);
		assert(bishop_magics.size() == kSquares);

		std::vector<int> rook_offsets;
		ReadFromFile(kRookOffsets, &rook_offsets);
		assert(rook_offsets.size() == kSquares);

		std::vector<int> bishop_offsets;
		ReadFromFile(kBishopOffsets, &bishop_offsets);
		assert(bishop_offsets.size() == kSquares);

		std::vector<int> rook_shifts;
		ReadFromFile(kRookShifts, &rook_shifts);
		assert(rook_shifts.size() == kSquares);

		std::vector<int> bishop_shifts;
		ReadFromFile(kBishopShifts, &bishop_shifts);
		assert(bishop_shifts.size() == kSquares);

		std::wcout << L"Reading rook magic tables..." << std::endl;
		ReadFromFile(kRookAttackTable, &rook_attack_table_);
		rook_attack_table_.shrink_to_fit();

		std::wcout << L"Reading bishop magic tables..." << std::endl;
		ReadFromFile(kBishopAttackTable, &bishop_attack_table_);
		bishop_attack_table_.shrink_to_fit();

		std::wcout << L"Done!" << std::endl;

		for (int i = 0; i < kSquares; ++i) {
			rook_magics_[i] = { rook_masks.at(i), rook_magics.at(i),
				rook_shifts.at(i), rook_offsets.at(i) };
			bishop_magics_[i] = { bishop_masks.at(i), bishop_magics.at(i),
				bishop_shifts.at(i), bishop_offsets.at(i) };
		}
	}
};