#include <endState.hpp>
#include <inputManager.hpp>
#include <game.hpp>
#include <stageState.hpp>

EndState::EndState(StateData stateData):State::State(),
										text{
											Text(
												RESOURCESFOLDER+"font/Call me maybe.ttf",
												36,
												Text::TextStyle::BLENDED,
												"Press spacebar to start again or Escape to quit...",
												MakeColor(255,255,255)
											)
										}{
	text.SetPos(520,20,true,false);
	if(stateData.playerVictory){
		music.Open(RESOURCESFOLDER+"audio/endStateWin.ogg");
		sp.Open(RESOURCESFOLDER+"img/win.jpg");
	}
	else{
		music.Open(RESOURCESFOLDER+"audio/endStateLose.ogg");
		sp.Open(RESOURCESFOLDER+"img/lose.jpg");
	}
}
EndState::~EndState(){
	music.Stop();
}

void EndState::Update(float time){
	if(INPUTMAN.QuitRequested() || INPUTMAN.KeyPress(ESCAPE_KEY)){
		quitRequested=true;
	}
	else if(INPUTMAN.KeyPress(SPACEBAR_KEY)){
		popRequested=true;
		GAMEINST.Push(new StageState);
	}
}
void EndState::Render(){
	sp.Render(0,0);
	text.Render(0,0);
}

void EndState::Pause(){
	music.Stop();
}
void EndState::Resume(){
	music.Play(-1);
}

void EndState::LoadAssets(){}
