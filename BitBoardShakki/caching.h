#pragma once
#include <unordered_map>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include "hash.h"
#include "statebb.h"



struct CacheItem {
	enum ItemType {
		EXACT_VALUE,
		LOWERBOUND,
		UPPERBOUND
	};
	StateHash hash;
	ItemType type;
	int evaluation = 0;
	int depth = 0;
	int age = 0;
	Move bestMove;
};

static const size_t CACHE_MAX_SIZE = (1024 * 1024 * 512) / sizeof(CacheItem); // 512MB
static std::shared_mutex cacheMutex;
static std::unordered_map<StateHash, CacheItem> cache;
static int currentGen = 0;


inline void pruneCache() {
	std::unique_lock<std::shared_mutex> lock(cacheMutex);
	if (cache.size() < CACHE_MAX_SIZE) {
		return;
	}
	int thold = currentGen - 1;

	for (auto it = cache.begin(); it != cache.end();) {
		if (it->second.depth <= 3) {
			it = cache.erase(it);
		}
		else {
			++it;
		}
	}

	if (cache.size() >= CACHE_MAX_SIZE) {
		cache.clear();
	}
}


inline void pruneCacheOld() {
	currentGen++;

	std::unique_lock<std::shared_mutex> lock(cacheMutex);
	int thold = currentGen - 7;
	int buckets = cache.size();

	for (auto it = cache.begin(); it != cache.end();) {
		if (it->second.age < thold) {
			it = cache.erase(it);
		}
		else {
			++it;
		}
	}
	//cache.rehash(buckets);
}

inline bool checkCache(const State& state, CacheItem& item) {
	std::shared_lock<std::shared_mutex> lock(cacheMutex);

	auto search = cache.find(state.hash);
	if (search != cache.end()) {
		const CacheItem* it = &search->second;
		if (it->hash == state.hash) {
			item = *it;
			return true;
		}
	}

	return false;
}

inline void addToCache(const State& state, int eval, int depth, CacheItem::ItemType type, Move bestMove = Move()) {
	pruneCache();

	std::unique_lock<std::shared_mutex> lock(cacheMutex);

	auto search = cache.find(state.hash);
	if (search != cache.end()) {
		const CacheItem* it = &search->second;
		if (it->hash == state.hash && depth >= it->depth) {
			CacheItem& ci = cache[state.hash];
			ci.type = type;
			ci.depth = depth;
			ci.evaluation = eval;
			ci.age = currentGen;
			ci.bestMove = bestMove;
			return;
		}
	}
	CacheItem item;
	item.hash = state.hash;
	item.type = type;
	item.evaluation = eval;
	item.depth = depth;
	item.age = currentGen;
	item.bestMove = bestMove;
	cache[state.hash] = item;
}