#include <componentAI.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompAI::CompAI(aiFunc func,int stCounts,int timCount,int posCount,int goCount):
	foo{func},states(stCounts,0),timers(timCount),targetPOS(posCount),targetGO(goCount,nullptr){}
CompAI::~CompAI(){}

void CompAI::Update(float time){
	for(auto &timer:timers)timer.Update(time);

	//TODO: use observer/observed pattern to remove this check
	//for(auto &go:targetGO)if(GAMESTATE.entities.count(make_unique(go))==0)go=nullptr;

	foo(this,time);
}

void CompAI::Render(){}

Component::type CompAI::GetType()const{
	return Component::type::t_ai;
}
