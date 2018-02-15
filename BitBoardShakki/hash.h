#pragma once
#include "enums.h"
#include "move.h"
#include "statebb.h"
namespace zobhash {
	class ZobristHashing {
	private:
		ZobristHashing();
	public:
		static ZobristHashing& getInstance()
		{
			static ZobristHashing instance;

			return instance;
		}
		ZobristHashing(ZobristHashing const&) = delete;
		void operator=(ZobristHashing const&) = delete;
	};
};

struct StateHash {
	size_t hash = 0;

	inline bool operator ==(const StateHash& other) const{
		return hash == other.hash;
	}
	inline void update(const Move& move) {
		
	}
	inline void reconstruct(const State& state) {
		hash = 0;
	}
};