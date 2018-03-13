#include "moveclock.h";

void timeCount(ThreadProcessingClock* clock) {
	while (clock->_timeLeft > 0) {
		using namespace std::chrono_literals;
		auto start = std::chrono::high_resolution_clock::now();

		std::this_thread::sleep_for(10ms);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = end - start;

		float timePassed = elapsed.count();
		clock->_timeLeft -= timePassed;
	};
}

ThreadProcessingClock::ThreadProcessingClock() {};

ThreadProcessingClock::ThreadProcessingClock(float time) {
	_timeLeft = time;
	_running = false;
}

ThreadProcessingClock::~ThreadProcessingClock() {
	join();
	delete _pThread;
}

ThreadProcessingClock::ThreadProcessingClock(ThreadProcessingClock const& clone) {
	if (clone._pThread) {
		throw("Cannot copy a running ThreadProcessingClock!");
	}
	else {
		_timeLeft = clone._timeLeft;
		_running = false;
	}
}

void ThreadProcessingClock::join() {
	if (!_pThread) return;
	if (_pThread->joinable()) {
		_pThread->join();
	}
}


void ThreadProcessingClock::operator=(ThreadProcessingClock& move) {
	_pThread = move._pThread;
	move._pThread = nullptr;
	_timeLeft = move._timeLeft;
	move._timeLeft = 0;
	_running = move._running;
	move._running = false;
}


bool ThreadProcessingClock::isDone() const{
	return _timeLeft <= 0;
}

void ThreadProcessingClock::start() {
	if (!_running) {
		_pThread = new std::thread(timeCount, this);
		_running = true;
	}
}

float ThreadProcessingClock::timeLeft() const{
	return _timeLeft;
}


MoveClock::MoveClock(float totalTime) {
	_totalTime = totalTime;
}

ThreadProcessingClock MoveClock::ReserveTimeForThread() const{
	float threadTime = _totalTime;
	ThreadProcessingClock clock(threadTime);
	return clock;
}
