#include <componentGravity.hpp>
#include <componentMovement.hpp>
#include <gameObject.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompGravity::CompGravity(float g):gravity{g}{}
CompGravity::~CompGravity(){}


void CompGravity::Update(float time){
	if(ENTITY(entity)->hasComponent[Component::type::t_movement]){
		COMPMOVEp(ENTITY(entity).get())->speed.y+=gravity*time;
	}
}
void CompGravity::Render(){}
Component::type CompGravity::GetType() const{
	return Component::type::t_gravity;
}
