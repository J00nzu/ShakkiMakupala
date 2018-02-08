#include "move.h"
#include <algorithm>
#include <functional>

#ifdef USE_HEAP_ARRAY

MoveSet::MoveSet() {
	_arr = nullptr;
	_len = 0;
	_capacity = 0;
}
MoveSet::MoveSet(const MoveSet& copy) {
	_arr = new Move[copy._capacity];
	memcpy(_arr, copy._arr, copy._len * sizeof(Move));
	_len = copy._len;
	_capacity = copy._capacity;
}
MoveSet& MoveSet::operator=(MoveSet& move) {
	delete[] _arr;
	// pilfer other’s resource
	_len = move._len;
	_arr = move._arr;
	_capacity = move._capacity;
	// reset other
	move._len = 0;
	move._arr = nullptr;
	move._capacity = 0;

	return *this;
}

void MoveSet::growSet() {
	_capacity += MOVESET_MAX_MOVES;
	auto newArr = new Move[_capacity];
	if(_arr){
		memcpy(newArr, _arr, _len * sizeof(Move));
		delete[] _arr;
	}
	_arr = newArr;
}

int MoveSet::capacity() const {
	return _capacity;
}
void MoveSet::push_back(const Move& move) {
	if ((_len+1) >= _capacity) {
		growSet();
	}
	_arr[_len++] = move;
}
MoveSet::~MoveSet() {
	delete[] _arr;
}
#else
void MoveSet::push_back(const Move& move) {
	if (_len <= MOVESET_MAX_MOVES - 1) {
		_arr[_len++] = move;
	}
}
#endif



int MoveSet::size() const{
	return _len;
}

bool MoveSet::empty() const {
	return _len == 0;
}
Move& MoveSet::get(int index) {
	return _arr[index];
}
void MoveSet::set(int index, const Move& other) {
	_arr[index] = other;
}
Move& MoveSet::operator[](int index) {
	return _arr[index];
}
void MoveSet::erase(int index) {
	auto n_to_move = (_len - index) - 1;
	if (n_to_move > 0)
		memmove(&_arr[index], &_arr[index + 1], n_to_move * sizeof(Move));

	_len--;
}


int comparator(const void *p, const void *q)
{
	int l = *((struct Move *)p);
	int r = *((struct Move *)q);
	return (r - l);
}

void MoveSet::sort() {
	std::sort(begin(), end(), std::greater<Move>());
}


MoveSet::iterator MoveSet::begin() { return &_arr[0]; }
MoveSet::const_iterator MoveSet::begin() const { return &_arr[0]; }
MoveSet::iterator MoveSet::end() { return &_arr[_len]; }
MoveSet::const_iterator MoveSet::end() const { return &_arr[_len]; }

