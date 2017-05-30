#include <state.hpp>

State::State():popRequested{false},quitRequested{false}{}

set<GameObject*> State::GetEntitiesInRange(const float &x1,const float &x2){
	set<GameObject*> s;
	for(auto &go:entities){
		Rect &box = go->box;
		if(box.x<=x2 || box.x+box.w>=x1)s.insert(go.get());
	}
	return s;
}
void State::AddObject(GameObject* obj){
	entities.insert(unique_ptr<GameObject>(obj));
}

bool State::PopRequested(){
	return popRequested;
}
bool State::QuitRequested(){
	return quitRequested;
}

void State::UpdateArray(float time){
	for(auto it=entities.begin();it!=entities.end();){
		(*it)->Update(time);
		if((*it)->dead)it = entities.erase(it);
		else it++;
	}
}
void State::RenderArray(){
	for(const auto &i:entities){
		i->Render();
	}
}