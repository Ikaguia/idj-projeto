#include <state.hpp>

State::State(){}

set<GameObject*> State::GetEntitiesInRange(const float &x1,const float &x2){
	//TODO: change this to just iterate trought the areas
	set<GameObject*> s;
	for(const auto &go:entities){
		if(go.second.get()==nullptr)continue;
		Rect box = go.second->Box();
		if(box.x<=x2 || box.x+box.w>=x1)s.insert(go.second.get());
	}
	return s;
}

void State::End(){
	ending=true;
	entities.clear();
}

void State::AddObject(GameObject* obj, int layer, int area){
	ii key(layer,area);

	group[key].insert(obj->uid);
	entities[obj->uid]=unique_ptr<GameObject>(obj);
}

bool State::PopRequested(){
	return popRequested;
}
bool State::QuitRequested(){
	return quitRequested;
}

uint State::GetUID(){
	return ++uid;
}

void State::UpdateArray(float time){
	for(auto it=entities.begin();it!=entities.end();){
		if(it->second.get()==nullptr){
			it++;
			continue;
		}
		it->second->Update(time);
		if(it->second->dead)it = entities.erase(it);
		else it++;
	}
}
void State::RenderArray(){
	for(const auto &i:entities){
		if(i.second.get()==nullptr)continue;
		i.second->Render();
	}
}
