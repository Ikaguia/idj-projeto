#include <componentHP.hpp>
#include <gameObject.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompHP::CompHP(int tot):total{tot},current{tot}{}
CompHP::CompHP(int tot,int cur):total{tot},current{cur}{}
CompHP::~CompHP(){}

void CompHP::Update(float time){
	if(current<=0)entity->dead=true;
}
void CompHP::Render(){
	Rect box{0,entity->box.w/5.0f + entity->box.w/10.0f,entity->box.w/5.0f,entity->box.h};
	//TODO: draw hp bar
}
Component::type CompHP::GetType()const{
	return Component::type::t_hp;
}
