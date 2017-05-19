#include <componentMovement.hpp>
#include <gameObject.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompMovement::CompMovement(){}
CompMovement::~CompMovement(){}

void CompMovement::Update(float time){
	entity->box+=move;
}

void CompMovement::Render(){}

Component::type CompMovement::GetType()const{
	return Component::type::t_movement;
}
