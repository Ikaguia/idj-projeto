#ifndef TIMERHPP
#define TIMERHPP

#include <common.hpp>

class Timer{
	float time;
public:
	Timer();

	void Update(float dTime);
	void Restart();
	float Get();
};

#endif//TIMERHPP