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
	ClearObjects();
}

void State::AddObject(uint uid, const string& layer, int area){
	//ii key(layer,area);
	//group[key].insert(uid);
	objectLayer[layer].insert(uid);
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
	for(uint uid:entities_){
		if(!isGO(uid))continue;
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
		if(!isGO(uid))continue;
		if(GO(uid)==nullptr){
			GameObject::entities.erase(uid);
			continue;
		}
		GO(uid)->Render();
	}
}

State::Layer::Layer(const string& n, char t, const float& p):name{n},type{t},parallax{p}{}
