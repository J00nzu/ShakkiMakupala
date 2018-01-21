#pragma once
#include <cstdint>

inline void SetFlag(uint_least8_t& flags, const uint_least8_t flag) { flags |= flag; }
inline void UnsetFlag(uint_least8_t& flags, const uint_least8_t flag) { flags &= ~flag; }