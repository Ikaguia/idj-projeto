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

	GameObject *target = new GameObject{Rect{1000.0f,750.0f,200.0f,250.0f}};
	target->AddComponent(new CompStaticRender{Sprite{"img/target.png"},Vec2{}});
	target->AddComponent(new CompCollider{CompCollider::collType::t_player});
	target->AddComponent(new CompHP{100,100,true,false});
	AddObject(target);
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
