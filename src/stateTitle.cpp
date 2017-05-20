#include <stateTitle.hpp>
#include <stateStage.hpp>
#include <game.hpp>
#include <inputManager.hpp>
#include <componentText.hpp>
#include <camera.hpp>

StateTitle::StateTitle():State::State(),bg{Sprite("img/title.jpg")}{
	GameObject* text = new GameObject{Rect{520,500,0,0}};
	text->AddComponent(new CompText{"Press spacebar to continue...",36,MakeColor(150,150,255)});
	AddObject(text);

	LoadAssets();
}
StateTitle::~StateTitle(){}

void StateTitle::Update(float time){
	if(INPUTMAN.QuitRequested() || INPUTMAN.KeyPress(ESCAPE_KEY))quitRequested=true;
	if(INPUTMAN.KeyPress(SPACEBAR_KEY))GAMEINST.Push(new StateStage{"img/tileset.png","map/tileMap.txt","img/stage_bg.jpg"});
	UpdateArray(time);
}
void StateTitle::Render(){
	bg.Render(0,0);
	RenderArray();
}

void StateTitle::Pause(){}
void StateTitle::Resume(){
	CAMERA.x=CAMERA.y=0;
}


void StateTitle::LoadAssets(){}
