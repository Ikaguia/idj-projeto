#include <memory>

#include <stateStage.hpp>
#include <camera.hpp>
#include <resources.hpp>
#include <music.hpp>
#include <game.hpp>
#include <componentInputControl.hpp>
#include <componentStaticRender.hpp>
#include <componentMovement.hpp>
#include <componentCollider.hpp>

StateStage::StateStage(string fileTSet,string fileTMap,string fileBG):State::State(),
						bg{Sprite(fileBG)},
						tileSet{TileSet(64,64,fileTSet)},
						tileMap{TileMap(fileTMap,&tileSet,&entities)}{
	LoadAssets();

	player = new GameObject{Rect{100.0f,100.0f,150.0f,250.0f}};
	player->AddComponent(new CompInputControl{[](GameObject* go, float time){
		Vec2 &move = ((CompMovement*)go->components[Component::type::t_movement])->move;
		if     (INPUTMAN.IsKeyDown(KEY_W))move.y=-250*time;
		else if(INPUTMAN.IsKeyDown(KEY_S))move.y=+250*time;
		else move.y=0;
		if     (INPUTMAN.IsKeyDown(KEY_A))move.x=-250*time;
		else if(INPUTMAN.IsKeyDown(KEY_D))move.x=+250*time;
		else move.x=0;
	}});
	player->AddComponent(new CompStaticRender{Sprite{"img/player_static.jpg"},Vec2{}});
	player->AddComponent(new CompMovement{});
	player->AddComponent(new CompCollider{CompCollider::collType::t_player});
	AddObject(player);
}

StateStage::~StateStage(){}

void StateStage::Update(float time){
	Camera::Update(time);
	if(INPUTMAN.QuitRequested())quitRequested=true;
	if(INPUTMAN.KeyPress(ESCAPE_KEY))popRequested=true;
	UpdateArray(time);
}
void StateStage::Render(){
	bg.Render(0,0);
	tileMap.Render(CAMERA.x,CAMERA.y);
	RenderArray();
}

void StateStage::Pause(){
	Camera::Unfollow();
}
void StateStage::Resume(){
	Camera::Follow(player);
}

void StateStage::LoadAssets(){}
