#include <componentGravity.hpp>
#include <componentMovement.hpp>
#include <gameObject.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompGravity::CompGravity(float g):gravity{g}{}
CompGravity::~CompGravity(){}


void CompGravity::Update(float time){
	if(entity->hasComponent[Component::type::t_movement]){
		((CompMovement*)entity->components[Component::type::t_movement])->speed.y+=gravity*time;
	}
}
void CompGravity::Render(){}
void CompGravity::Own(GameObject* go){
	entity=go;
}
Component::type CompGravity::GetType() const{
	return Component::type::t_gravity;
}
