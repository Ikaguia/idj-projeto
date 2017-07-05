#include <componentInputControl.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompInputControl::CompInputControl(function<void(GameObject*,float)> f):func{f}{}
CompInputControl::~CompInputControl(){}

void CompInputControl::Update(float time){
	func(GO(entity),time);
}
void CompInputControl::Render(){}
Component::type CompInputControl::GetType()const{
	return Component::type::t_input_control;
}
