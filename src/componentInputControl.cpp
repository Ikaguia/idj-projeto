#include <componentInputControl.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompInputControl::CompInputControl(function<void(GameObject*,float)> f):func{f}{}
CompInputControl::~CompInputControl(){}

void CompInputControl::Update(float time){
	func(entity,time);
}
void CompInputControl::Render(){}
void CompInputControl::Own(GameObject* go){
	entity=go;
}
Component::type CompInputControl::GetType()const{
	return Component::type::t_input_control;
}
