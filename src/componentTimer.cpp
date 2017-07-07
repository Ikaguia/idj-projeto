#include <componentTimer.hpp>
//#include <compLib.hpp>
#include <gameObject.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>


CompTimer::CompTimer(float l):limit{l}{}
CompTimer::~CompTimer(){}


void CompTimer::Update(float time){
	t.Update(time);
	if(t.Get()>limit){
		GO(entity)->dead=true;
	}
}


void CompTimer::Render(){}


void CompTimer::Own(GameObject* go){
	entity=go->uid;
}


Component::type CompTimer::GetType() const{
	return Component::type::t_timer;
}


