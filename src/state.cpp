#include <state.hpp>

State::State(){}

set<uint> State::GetEntitiesInRange(const float &x1,const float &x2){
	COUTL(f1);
	//TODO: change this to just iterate trought the areas
	COUTL(f2);
	set<uint> s;
	COUTL(f3);
	for(uint uid:entities_){
		COUTL(f4);
		if(GO(uid)==nullptr)continue;
		COUTL(f5);
		Rect box = GO(uid)->Box();
		COUTL(f6);
		if(box.x<=x2 || box.x2()>=x1)s.insert(uid);
		COUTL(f7);
	}
	COUTL(f8);
	return s;
}

void State::End(){
	ClearObjects();
}

void State::AddObject(uint uid, int layer, int area){
	ii key(layer,area);

	group[key].insert(uid);
	entities_.insert(uid);

	lastGO = uid;
}

void State::ClearObjects(){
	for(uint uid:entities_){
		if(!isGO(uid))continue;
		GameObject::entities.erase(uid);
	}
	group.clear();
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
	COUTL(c1);
	for(uint uid:entities_){
		COUTL(c2);
		if(!isGO(uid))continue;
		COUTL(c3);
		if(GO(uid)==nullptr){
			COUTL(c4);
			GameObject::entities.erase(uid);
			COUTL(c5);
			continue;
			COUTL(c6);
		}
		COUTL(c7);
		GO(uid)->Update(time);
		COUTL(c8);
		if(GO(uid)->Remove()){
			COUTL(c9);
			// cout << "Removing go " << uid << endl;
			COUTL(c10);
			GameObject::entities.erase(uid);
			COUTL(c11);
			entities_.erase(uid);
			COUTL(c12);
		}
	}
}
void State::RenderArray(){
	for(uint uid:entities_){
		if(!isGO(uid))continue;
		if(GO(uid)==nullptr){
			GameObject::entities.erase(uid);
			continue;
		}
		GO(uid)->Render();
	}
}
