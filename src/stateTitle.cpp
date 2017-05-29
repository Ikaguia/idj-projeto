#include <stateTitle.hpp>

#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <inputManager.hpp>
#include <stateStage.hpp>
#include <stateEditor.hpp>

#define INSTRUCTION_TEXT "IDJ-Projeto\n\nPress [Space] to continue\n[E] Level Editor\n"

StateTitle::StateTitle():State::State(),bg{Sprite("img/title.jpg")}{
	GameObject* text = new GameObject{Rect{(WINSIZE.x/2),(WINSIZE.y/2),0,0}};
	text->AddComponent(new CompText{INSTRUCTION_TEXT,36,SDL_COLOR_WHITE,Hotspot::CENTER});
	AddObject(text);

	LoadAssets();
}
StateTitle::~StateTitle(){}

void StateTitle::Update(float time){
	if(INPUTMAN.QuitRequested() || INPUTMAN.KeyPress(ESCAPE_KEY))quitRequested=true;
	
	if(INPUTMAN.KeyPress(SPACEBAR_KEY))GAMEINST.Push(new StateStage{"level/level_0.txt"});
	if(INPUTMAN.KeyPress(KEY(e)))GAMEINST.Push(new StateEditor{});
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
