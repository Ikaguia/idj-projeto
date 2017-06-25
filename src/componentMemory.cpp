#include <componentMemory.hpp>
//#include <compLib.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>


CompMemory::CompMemory(){}
CompMemory::~CompMemory(){}


void CompMemory::Update(float time){
	for(auto& t:timers)t.second.Update(time);
}


void CompMemory::Render(){}


void CompMemory::Own(GameObject* go){
	entity=go;
}


Component::type CompMemory::GetType() const{
	return Component::type::t_memory;
}


