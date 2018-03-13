#pragma once
#include "enums.h"
#include "move.h"
#include <cassert>

namespace zobhash {
	static const int W_K_CASTLE = 0;
	static const int W_Q_CASTLE = 1;
	static const int B_K_CASTLE = 2;
	static const int B_Q_CASTLE = 3;


	class ZobristHashing {
	private:
		static uint64_t pieces_squares[12][64];
		static uint64_t black_turn;
		static uint64_t castling_rights[4];
		static uint64_t ep_file[8];

		ZobristHashing();
	public:
		static ZobristHashing& getInstance()
		{
			static ZobristHashing instance;

			return instance;
		}
		ZobristHashing(ZobristHashing const&) = delete;
		void operator=(ZobristHashing const&) = delete;

		inline uint64_t getPieceSquare(PIECE p, int squareIdx) const{
			return pieces_squares[p][squareIdx];
		}
		inline uint64_t getBlackTurn() const{
			return black_turn;
		}
		inline uint64_t getCastlingRights(int idx) const{
			return castling_rights[idx];
		}
		inline uint64_t getEpFile(int fileIdx) const{
			return ep_file[fileIdx];
		}
	};
};

struct State;
struct Move;

struct StateHash {
	uint64_t hash = 0;

	inline bool operator ==(const StateHash& other) const{
		return hash == other.hash;
	}

#ifdef _MSC_VER
	__forceinline void update(const State& state, const Move& move) {
#else
	inline void update(const State& state, const Move& move) {
#endif
		static const zobhash::ZobristHashing* zobber = &zobhash::ZobristHashing::getInstance();
		
		if (move.isPromotion()) {
			if (move.isCapture()) {
				int fromIdx = move.getFromIdx();
				int toIdx = move.getToIdx();
				PIECE p = move.getPiece();
				PIECE cP = move.getCPiece();
				PIECE pP = move.getPromotedPiece();
				hash ^= zobber->getPieceSquare(cP, toIdx);
				hash ^= zobber->getPieceSquare(p, fromIdx);
				hash ^= zobber->getPieceSquare(pP, toIdx);
			}
			else {
				int fromIdx = move.getFromIdx();
				int toIdx = move.getToIdx();
				PIECE p = move.getPiece();
				PIECE pP = move.getPromotedPiece();
				hash ^= zobber->getPieceSquare(p, fromIdx);
				hash ^= zobber->getPieceSquare(pP, toIdx);
			}
		}else if (move.isCapture()) {
			if (move.isEnPassant()) {
				int epIdx = move.getEpSquareIdx();
				int fromIdx = move.getFromIdx();
				int toIdx = move.getToIdx();
				PIECE p = move.getPiece();
				PIECE cP = move.getCPiece();
				hash ^= zobber->getPieceSquare(cP, epIdx);
				hash ^= zobber->getPieceSquare(p, fromIdx);
				hash ^= zobber->getPieceSquare(p, toIdx);
			}
			else {
				int fromIdx = move.getFromIdx();
				int toIdx = move.getToIdx();
				PIECE p = move.getPiece();
				PIECE cP = move.getCPiece();
				hash ^= zobber->getPieceSquare(cP, toIdx);
				hash ^= zobber->getPieceSquare(p, fromIdx);
				hash ^= zobber->getPieceSquare(p, toIdx);
			}
		}
		else if (move.isCastle()) {
			Move::MoveType type = move.getMoveType();
			if (type == Move::KING_CASTLE) {
				if (move.getColor() == WHITE) {
					static int KfromIdx = bitB_bitScanForward(bE1);
					static int KtoIdx = bitB_bitScanForward(bG1);
					static int RfromIdx = bitB_bitScanForward(bH1);
					static int RtoIdx = bitB_bitScanForward(bF1);
					const PIECE K = Wking;
					const PIECE R = Wrook;
					hash ^= zobber->getPieceSquare(K, KfromIdx);
					hash ^= zobber->getPieceSquare(K, KtoIdx);
					hash ^= zobber->getPieceSquare(R, RfromIdx);
					hash ^= zobber->getPieceSquare(R, RtoIdx);
				}
				else {
					static int KfromIdx = bitB_bitScanForward(bE8);
					static int KtoIdx = bitB_bitScanForward(bG8);
					static int RfromIdx = bitB_bitScanForward(bH8);
					static int RtoIdx = bitB_bitScanForward(bF8);
					const PIECE K = Bking;
					const PIECE R = Brook;
					hash ^= zobber->getPieceSquare(K, KfromIdx);
					hash ^= zobber->getPieceSquare(K, KtoIdx);
					hash ^= zobber->getPieceSquare(R, RfromIdx);
					hash ^= zobber->getPieceSquare(R, RtoIdx);
				}
			}
			else {
				if (move.getColor() == WHITE) {
					static int KfromIdx = bitB_bitScanForward(bE1);
					static int KtoIdx = bitB_bitScanForward(bC1);
					static int RfromIdx = bitB_bitScanForward(bA1);
					static int RtoIdx = bitB_bitScanForward(bD1);
					const PIECE K = Wking;
					const PIECE R = Wrook;
					hash ^= zobber->getPieceSquare(K, KfromIdx);
					hash ^= zobber->getPieceSquare(K, KtoIdx);
					hash ^= zobber->getPieceSquare(R, RfromIdx);
					hash ^= zobber->getPieceSquare(R, RtoIdx);
				}
				else {
					static int KfromIdx = bitB_bitScanForward(bE8);
					static int KtoIdx = bitB_bitScanForward(bC8);
					static int RfromIdx = bitB_bitScanForward(bA8);
					static int RtoIdx = bitB_bitScanForward(bD8);
					const PIECE K = Bking;
					const PIECE R = Brook;
					hash ^= zobber->getPieceSquare(K, KfromIdx);
					hash ^= zobber->getPieceSquare(K, KtoIdx);
					hash ^= zobber->getPieceSquare(R, RfromIdx);
					hash ^= zobber->getPieceSquare(R, RtoIdx);
				}
			}
		}
		else {
			int fromIdx = move.getFromIdx();
			int toIdx = move.getToIdx();
			PIECE p = move.getPiece();
			hash ^= zobber->getPieceSquare(p, fromIdx);
			hash ^= zobber->getPieceSquare(p, toIdx);
		}

		hash ^= zobber->getBlackTurn();
		
#ifdef _DEBUG
		uint64_t lhash = hash;
		reconstruct(state);
		assert(lhash == hash);
#endif // _DEBUG
	}

	inline void unsetCastlingRights(int cIdx) {
		static const zobhash::ZobristHashing* zobber = &zobhash::ZobristHashing::getInstance();
		hash ^= zobber->getCastlingRights(cIdx);
	}

	inline void switchEpFile(FILEIDX file) {
		static const zobhash::ZobristHashing* zobber = &zobhash::ZobristHashing::getInstance();
		hash ^= zobber->getEpFile(file);
	}

	void reconstruct(const State& state);
};

namespace std {

	template <>
	struct hash<StateHash>
	{
		inline std::size_t operator()(const StateHash& k) const
		{
			return k.hash;
		}
	};

}