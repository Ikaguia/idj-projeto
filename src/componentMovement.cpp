#include <componentMovement.hpp>
#include <gameObject.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompMovement::CompMovement(const Vec2& s,moveType t):mType{t},speed{s}{}
CompMovement::~CompMovement(){}


void CompMovement::Update(float time){
	entity->box+=move;
	if(mType==moveType::t_bullet)entity->rotation=speed.angle();
}
void CompMovement::Render(){}
void CompMovement::Own(GameObject* go){
	entity=go;
}
Component::type CompMovement::GetType()const{
	return Component::type::t_movement;
}
