#pragma once
#include <cstdint>

/*
inline void SetFlag(uint_fast32_t& flags, const uint_fast32_t flag) { flags |= flag; }
inline void UnsetFlag(uint_fast32_t& flags, const uint_fast32_t flag) { flags &= ~flag; }
inline void SwitchFlag(uint_fast32_t& flags, const uint_fast32_t flag) { flags ^= flag; }
inline bool CheckFlag(const uint_fast32_t& flags, const uint_fast32_t flag) { return ((flags & flag) != 0); }
*/

template <class T,class B>
inline void SetFlag(T& flags, const B flag) { flags |= flag; }
template <class T, class B>
inline void UnsetFlag(T& flags, const B flag) { flags &= ~flag; }
template <class T, class B>
inline void SwitchFlag(T& flags, const B flag) { flags ^= flag; }
template <class T, class B>
inline bool CheckFlag(const T& flags, const B flag) { return ((flags & flag) != 0); }