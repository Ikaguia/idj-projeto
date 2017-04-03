#include <titleState.hpp>
#include <game.hpp>
#include <stageState.hpp>
#include <inputManager.hpp>
#include <text.hpp>

TitleState::TitleState():State::State(),
						bg{Sprite(RESOURCESFOLDER+"img/title.jpg")},
						text{
							Text(
								RESOURCESFOLDER+"font/Call me maybe.ttf",
								36,
								Text::TextStyle::BLENDED,
								"Press spacebar to continue...",
								MakeColor(80,140,190)
							)
						}{
	text.SetPos(520,500,true,true);
}
TitleState::~TitleState(){}

void TitleState::Update(float time){
	if(INPUTMAN.QuitRequested() || INPUTMAN.KeyPress(ESCAPE_KEY))quitRequested=true;
	if(INPUTMAN.KeyPress(SPACEBAR_KEY))GAMEINST.Push(new StageState);
}
void TitleState::Render(){
	bg.Render(0,0);
	text.Render(0,0);
}

void TitleState::Pause(){}
void TitleState::Resume(){}

void TitleState::LoadAssets(){}
