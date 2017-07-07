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
	player = GameObject::MakePlayer(Vec2{550.0f,430.0f});
	AddObject(player);
	Camera::Follow(player);
	level.LoadObjects();
}

void StateStage::Update(float time){
	Camera::Update(time);
	if(INPUT.QuitRequested())quitRequested=true;
	if(INPUT.KeyPress(KEY_ESC))popRequested=true;
	
	if(INPUT.KeyPress(KEY(u))) AddObject(GameObject::MakePorco(Vec2{2000.0f,800.0f}));
	if(INPUT.KeyPress(KEY(p))) AddObject(GameObject::MakeMike(Vec2{850.0f,300.0f}));
	if(INPUT.KeyPress(KEY(o))) AddObject(GameObject::MakeBanshee(Vec2{850.0f,400.0f},Vec2{230.0f,130.0f}));
	if(INPUT.KeyPress(KEY(i))) AddObject(GameObject::MakeMask(Vec2{850.0f,400.0f}));
	if(INPUT.KeyPress(KEY(y))) GO(player)->dead=true;

	if(INPUT.KeyPress(KEY(n))) SETTINGS.showHP = !SETTINGS.showHP;
	if(INPUT.KeyPress(KEY(m))) SETTINGS.showCollision = !SETTINGS.showCollision;

	UpdateArray(time);
}
void StateStage::Render(){
	SET_COLOR4(127,127,127,255);
	CLEAR_SCREEN();
	for(auto& layer:layerList){
		if(!layer.visible) continue;

		if(layer.type == '*'){
			set<uint>& objects= objectLayer[layer.name];
			
			for(uint uid:objects){
				if(!isGO(uid))continue;
				if(GO(uid)==nullptr){
					GameObject::entities.erase(uid);
					continue;
				}
				GO(uid)->Render();
			}
		}
		else if(layer.type == '#'){
			level.tileMap.RenderLayer(layer.tileMapLayer);
		}
	}
}

void StateStage::Pause(){
	Camera::Unfollow();
}
void StateStage::Resume(){
	Camera::Follow(PLAYER_UID);
}

void StateStage::LoadAssets(){
	Resources::GetImage("img/porco/porco-chifrada.png");
	Resources::GetImage("img/porco/porco-correndo.png");
	Resources::GetImage("img/porco/porco-idle.png");
	Resources::GetImage("img/porco/porco-morte.png");
	Resources::GetImage("img/porco/porco-stun.png");
	Resources::GetImage("img/porco/porco-virando.png");
	Resources::GetImage("img/porco/porco-chifrada-virado.png");
	Resources::GetImage("img/porco/porco-correndo-virado.png");
	Resources::GetImage("img/porco/porco-idle-virado.png");
	Resources::GetImage("img/porco/porco-morte-virado.png");
	Resources::GetImage("img/porco/porco-stun-virado.png");
	Resources::GetImage("img/porco/porco-virando-virado.png");
	Resources::GetImage("img/porco/rage/porco-chifrada.png");
	Resources::GetImage("img/porco/rage/porco-correndo.png");
	Resources::GetImage("img/porco/rage/porco-idle.png");
	Resources::GetImage("img/porco/rage/porco-morte.png");
	Resources::GetImage("img/porco/rage/porco-stun.png");
	Resources::GetImage("img/porco/rage/porco-virando.png");
	Resources::GetImage("img/porco/rage/porco-chifrada-virado.png");
	Resources::GetImage("img/porco/rage/porco-correndo-virado.png");
	Resources::GetImage("img/porco/rage/porco-idle-virado.png");
	Resources::GetImage("img/porco/rage/porco-morte-virado.png");
	Resources::GetImage("img/porco/rage/porco-stun-virado.png");
	Resources::GetImage("img/porco/rage/porco-virando-virado.png");
}
void StateStage::LoadGUI(){}
