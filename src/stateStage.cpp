#include <stateStage.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <resources.hpp>
#include <music.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <complib.hpp>

StateStage::StateStage(string lvl):State::State(), level{Level(lvl,&entities)}{
	LoadAssets();

	player=GameObject::MakePlayer(Vec2{130.0f,130.0f});
	AddObject(player);

	AddObject(GameObject::MakeMike(Vec2{800.0f,200.0f}));
	AddObject(GameObject::MakeTarget(Vec2{1000.0f,750.0f}));
}

StateStage::~StateStage(){}

void StateStage::Update(float time){
	Camera::Update(time);
	if(INPUTMAN.QuitRequested())quitRequested=true;
	if(INPUTMAN.KeyPress(KEY_ESC))popRequested=true;
	UpdateArray(time);
}
void StateStage::Render(){
	level.background.Render(0, 0);
	level.tileMap.Render();
	RenderArray();
}

void StateStage::Pause(){
	Camera::Unfollow();
}
void StateStage::Resume(){
	Camera::Follow(player);
}

void StateStage::LoadAssets(){}
