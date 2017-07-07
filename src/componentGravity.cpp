#include <componentGravity.hpp>
#include <componentMovement.hpp>
#include <gameObject.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompGravity::CompGravity(float g):gravity{g}{}
CompGravity::~CompGravity(){}


void CompGravity::Update(float time){
	if(GO(entity)->HasComponent(Component::type::t_movement)){
		COMPMOVEp(GO(entity))->speed.y+=gravity*time;
	}
}
void CompGravity::Render(){}
Component::type CompGravity::GetType() const{
	return Component::type::t_gravity;
}
