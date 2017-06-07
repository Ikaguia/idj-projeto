#include <stateStage.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <resources.hpp>
#include <music.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <complib.hpp>

StateStage::StateStage(string lvl):State::State(), level{Level(lvl,this)}{
	LoadAssets();

	player=GameObject::MakePlayer(Vec2{130.0f,130.0f});
	AddObject(player);
}

StateStage::~StateStage(){}

void StateStage::Update(float time){
	Camera::Update(time);
	if(INPUT.QuitRequested())quitRequested=true;
	if(INPUT.KeyPress(KEY_ESC))popRequested=true;
	
	if(INPUT.KeyPress(KEY(p))) AddObject(GameObject::MakeMike(Vec2{850.0f,200.0f}));
	if(INPUT.KeyPress(KEY(o))) AddObject(GameObject::MakeBanshee(Vec2{850.0f,200.0f},Vec2{130.0f,130.0f}));
	if(INPUT.KeyPress(KEY(i))) AddObject(GameObject::MakeMask(Vec2{850.0f,200.0f}));

	
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
