#include <stateTitle.hpp>

#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <inputManager.hpp>
#include <stateStage.hpp>
#include <stateEditor.hpp>

#define BACKGROUND "img/mountain_bg.jpg"
#define INSTRUCTION_TEXT "IDJ-Projeto\n\nPress [Space] to continue\n[E] Level Editor\n"

StateTitle::StateTitle():State::State(),bg{Sprite(BACKGROUND)}, b{{50, 50},"PRESS ME!"}, tb{{500,500}}{
	LoadAssets();
	
	bg.StretchToFit(WINSIZE);
	
}
StateTitle::~StateTitle(){}

void StateTitle::Begin(){
	//Create gameObjects here
	GameObject* text = new GameObject{Rect{(WINSIZE.x/2),(WINSIZE.y/2),0,0}};
	text->AddComponent(new CompText{INSTRUCTION_TEXT,36,SDL_COLOR_WHITE,Hotspot::CENTER});
	AddObject(text);
}

void StateTitle::Update(float time){
	if(INPUT.QuitRequested() || INPUT.KeyPress(KEY_ESC))quitRequested=true;
	
	if(INPUT.KeyPress(KEY_SPACE))GAMEINST.Push(new StateStage{"data/level/level_0.txt"});
	if(INPUT.KeyPress(KEY(e)))GAMEINST.Push(new StateEditor{});
	UpdateArray(time);
	//b.Update();
	//tb.Update();
}
void StateTitle::Render(){
	bg.Render(0,0);
	RenderArray();
	//b.Render();
	//tb.Render();
}

void StateTitle::Pause(){}
void StateTitle::Resume(){
	CAMERA.x=CAMERA.y=0;
}


void StateTitle::LoadAssets(){}
