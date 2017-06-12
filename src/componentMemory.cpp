#include <componentMemory.hpp>
//#include <compLib.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>


CompMemory::CompMemory(){}
CompMemory::~CompMemory(){}


void CompMemory::Update(float time){
	UNUSED(time);
}


void CompMemory::Render(){}


void CompMemory::Own(GameObject* go){
	UNUSED(go);
}


Component::type CompMemory::GetType() const{
	return Component::type::t_memory;
}


