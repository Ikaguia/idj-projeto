#include <stateTitle.hpp>

#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <inputManager.hpp>
#include <stateStage.hpp>
#include <stateEditor.hpp>

#define BACKGROUND "img/tela-inicio2.png"
#define INSTRUCTION_TEXT "IDJ-Projeto\n\nPress [Space] to continue\n[E] Level Editor\n"

StateTitle::StateTitle():State::State(),bg{Sprite(BACKGROUND)},bt1{"img/botao-editor.png",2},bt2{"img/botao-inicio.png",2}{
	LoadAssets();
	
	bg.StretchToFit(WINSIZE);
}
StateTitle::~StateTitle(){}

void StateTitle::LoadAssets(){}
void StateTitle::LoadGUI(){}

void StateTitle::Begin(){
	//Create gameObjects here
	GameObject* text = new GameObject{Rect{(WINSIZE.x/2),(WINSIZE.y/2),0,0}};
	text->AddComponent(new CompText{INSTRUCTION_TEXT,36,SDL_COLOR_WHITE,Hotspot::CENTER});
	AddObject(text->uid);
}

void StateTitle::Update(float time){
	if(INPUT.QuitRequested() || INPUT.KeyPress(KEY_ESC)) quitRequested=true;

	if(INPUT.KeyPress(KEY_SPACE)){
		bt2.SetFrame(1);
		GAMEINST.Push(new StateStage{"level_0"});
	}
	if(INPUT.KeyPress(KEY(e))){
		bt1.SetFrame(1);
		GAMEINST.Push(new StateEditor{});
	}
	UpdateArray(time);
}
void StateTitle::Render(){
	bg.Render(0,0);
	bt1.Render(500,300);
	bt2.Render(100,300);
	// RenderArray();
}

void StateTitle::Pause(){}
void StateTitle::Resume(){
	CAMERA.x=CAMERA.y=0;
}
