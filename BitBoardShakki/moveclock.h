#pragma once
#include <time.h>
#include <thread>

class ThreadProcessingClock {
private:
	float _timeLeft = 0;
	bool _running = false;
	std::thread* _pThread = nullptr;
public:
	ThreadProcessingClock();
	ThreadProcessingClock(float time);
	~ThreadProcessingClock();
	bool isDone() const;
	void start();
	float timeLeft() const;

	ThreadProcessingClock(ThreadProcessingClock const&);
	void operator=(ThreadProcessingClock&);
	void join();

	friend void timeCount(ThreadProcessingClock* clock);
};

class MoveClock {
	float _totalTime;
public:
	MoveClock() : _totalTime(1) {}
	MoveClock(float totalTime);
	ThreadProcessingClock ReserveTimeForThread() const;
};