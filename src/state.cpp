#include <state.hpp>

State::State():popRequested{false},quitRequested{false},uid{0}{}

set<GameObject*> State::GetEntitiesInRange(const float &x1,const float &x2){
	//TODO: change this to just iterate trought the areas
	set<GameObject*> s;
	for(auto &go:entities){
		Rect &box = go.second->box;
		if(box.x<=x2 || box.x+box.w>=x1)s.insert(go.second.get());
	}
	return s;
}
void State::AddObject(GameObject* obj, int layer, int area){
	ii key(layer,area);
	//if(!group.count(key)) group[key] = set<uint>{};//map faz isso automaticamente 

	obj->uid=uid;
	group[key].insert(uid++);
	entities[uid]=unique_ptr<GameObject>(obj);
}

bool State::PopRequested(){
	return popRequested;
}
bool State::QuitRequested(){
	return quitRequested;
}

void State::UpdateArray(float time){
	for(auto it=entities.begin();it!=entities.end();){
		it->second->Update(time);
		if(it->second->dead)it = entities.erase(it);
		else it++;
	}
}
void State::RenderArray(){
	for(const auto &i:entities){
		i.second->Render();
	}
}
