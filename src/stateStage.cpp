#include <stateStage.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <resources.hpp>
#include <music.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <complib.hpp>
StateStage::StateStage(string lvl):levelName{lvl}{
	LoadAssets();
}

StateStage::~StateStage(){}

void StateStage::Begin(){
	level.Load(levelName);
	player = GameObject::MakePlayer(Vec2{150.0f,430.0f});
	AddObject(player);
	Camera::Follow(player);
	level.LoadObjects();
}

void StateStage::Update(float time){
	Camera::Update(time);
	if(INPUT.QuitRequested())quitRequested=true;
	if(INPUT.KeyPress(KEY_ESC))popRequested=true;
	
	if(INPUT.KeyPress(KEY(u))) AddObject(GameObject::MakePorco(Vec2{850.0f,300.0f}));
	if(INPUT.KeyPress(KEY(p))) AddObject(GameObject::MakeMike(Vec2{850.0f,300.0f}));
	if(INPUT.KeyPress(KEY(o))) AddObject(GameObject::MakeBanshee(Vec2{850.0f,400.0f},Vec2{230.0f,130.0f}));
	if(INPUT.KeyPress(KEY(i))) AddObject(GameObject::MakeMask(Vec2{850.0f,300.0f}));
	if(INPUT.KeyPress(KEY(y))) GO(player)->dead=true;

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
	Camera::Follow(PLAYER_UID);
}

void StateStage::LoadAssets(){}
void StateStage::LoadGUI(){}
