#include <state.hpp>

State::State():popRequested{false},quitRequested{false}{
}

void State::AddObject(GameObject* obj){
	objectArray.emplace_back(obj);
}

bool State::PopRequested(){
	return popRequested;
}
bool State::QuitRequested(){
	return quitRequested;
}

void State::AddSound(string file,int times){
	Sound *s = new Sound(file);
	sounds.push_back(unique_ptr<Sound>(s));
	s->Play(times);
}

void State::UpdateArray(float time){
	for(auto it=objectArray.begin();it!=objectArray.end();){
		(*it)->Update(time);
		if((*it)->IsDead())it = objectArray.erase(it);
		else it++;
	}
}
void State::RenderArray(){
	for(const auto &i:objectArray)i->Render();
}
