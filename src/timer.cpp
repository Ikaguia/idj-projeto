#include <timer.hpp>

Timer::Timer():time{0}{}

void Timer::Update(float dTime){
	time+=dTime;
}
void Timer::Restart(){
	time=0;
}
float Timer::Get(){
	return time;
}
