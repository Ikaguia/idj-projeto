#include <titleState.hpp>
#include <game.hpp>
#include <inputManager.hpp>
#include <componentText.hpp>

TitleState::TitleState():State::State(),bg{Sprite("img/title.jpg")}{
	GameObject::entities = entities = new set<GameObject*>;


	GameObject* text = new GameObject{Rect{520,500,0,0}};
	text->AddComponent(new CompText{"Press spacebar to continue...",36,MakeColor(150,150,255)});
	GameObject::entities->insert(text);

}
TitleState::~TitleState(){}

void TitleState::Update(float time){
	if(INPUTMAN.QuitRequested() || INPUTMAN.KeyPress(ESCAPE_KEY))quitRequested=true;
	//TODO: remake stagestate
	//if(INPUTMAN.KeyPress(SPACEBAR_KEY))GAMEINST.Push(new StageState);
}
void TitleState::Render(){
	bg.Render(0,0);
	for(GameObject* go:(*GameObject::entities))go->Render();
}

void TitleState::Pause(){
	GameObject::entities=nullptr;
}
void TitleState::Resume(){
	GameObject::entities=entities;
}

void TitleState::LoadAssets(){}
