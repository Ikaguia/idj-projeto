#ifndef TIMERHPP
#define TIMERHPP

#include <common.hpp>

class Timer{
	float time=0;
public:
	Timer();

	void Update(float dTime);
	void Restart();
	float Get();
};

#endif//TIMERHPP
