#include <componentAI.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompAI::CompAI(aiFunc func):foo{func}{}
CompAI::~CompAI(){}

void CompAI::Update(float time){
	foo(this,time);
}

void CompAI::Render(){}

Component::type CompAI::GetType()const{
	return Component::type::t_ai;
}
