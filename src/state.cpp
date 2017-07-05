#include <state.hpp>

State::State(){}

set<uint> State::GetEntitiesInRange(const float &x1,const float &x2){
	//TODO: change this to just iterate trought the areas
	set<uint> s;
	for(uint uid:entities_){
		if(GO(uid)==nullptr)continue;
		Rect box = GO(uid)->Box();
		if(box.x<=x2 || box.x2()>=x1)s.insert(uid);
	}
	return s;
}

void State::End(){
	ending=true;
	for(uint uid:entities_){
		if(!GameObject::entities.count(uid))continue;
		GameObject::entities.erase(uid);
	}
}

void State::AddObject(uint uid, int layer, int area){
	ii key(layer,area);

	group[key].insert(uid);
	entities_.insert(uid);

	lastGO = uid;
}

GameObject* State::GetLastObject(){
	return GO(lastGO);
}


bool State::PopRequested(){
	return popRequested;
}
bool State::QuitRequested(){
	return quitRequested;
}

void State::UpdateArray(float time){
	for(uint uid:entities_){
		if(!GameObject::entities.count(uid))continue;
		if(GO(uid)==nullptr){
			GameObject::entities.erase(uid);
			continue;
		}
		GO(uid)->Update(time);
		if(GO(uid)->Remove()){
			// cout << "Removing go " << uid << endl;
			GameObject::entities.erase(uid);
			entities_.erase(uid);
		}
	}
}
void State::RenderArray(){
	for(uint uid:entities_){
		if(!GameObject::entities.count(uid))continue;
		if(GO(uid)==nullptr){
			GameObject::entities.erase(uid);
			continue;
		}
		GO(uid)->Render();
	}
}
