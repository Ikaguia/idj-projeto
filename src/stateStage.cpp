#include <stateStage.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <resources.hpp>
#include <music.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <complib.hpp>
StateStage::StateStage(string lvl):levelName{lvl},
	// floresta_bg1{"img/jogotela-campo-1-away.png"},
	floresta_bg2{"img/floresta-away.png"},
	floresta_bg3{"img/jogotela-ponte-away.png"}
	{
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
	
	Vec2 pos = GO(PLAYER_UID)->pos;
	if(INPUT.KeyPress(KEY(u))) AddObject(GameObject::MakePorco(pos+Vec2{2000.0f,-100.0f}));
	if(INPUT.KeyPress(KEY(p))) AddObject(GameObject::MakeMike(pos+Vec2{850.0f,-100.0f}));
	if(INPUT.KeyPress(KEY(o))) AddObject(GameObject::MakeBanshee(pos+Vec2{850.0f,-100.0f},pos+Vec2{230.0f,-200.0f}));
	if(INPUT.KeyPress(KEY(i))) AddObject(GameObject::MakeMask(pos+Vec2{850.0f,-100.0f}));
	if(INPUT.KeyPress(KEY(y))) GO(player)->dead=true;

	if(INPUT.KeyPress(KEY(n))) SETTINGS.showHP = !SETTINGS.showHP;
	if(INPUT.KeyPress(KEY(m))) SETTINGS.showCollision = !SETTINGS.showCollision;

	UpdateArray(time);
}
void StateStage::Render(){
	level.background.Render(0, 0);
	// floresta_bg1.Render(RENDERPOS(Vec2(0,0)));
	// floresta_bg2.Render(RENDERPOS(Vec2(4433,0)));
	// floresta_bg3.Render(RENDERPOS(Vec2(8465,0)));
	floresta_bg2.Render(RENDERPOS(Vec2(0,0)));
	floresta_bg2.Render(RENDERPOS(Vec2(4032*1,0)));
	floresta_bg2.Render(RENDERPOS(Vec2(4032*2,0)));
	floresta_bg2.Render(RENDERPOS(Vec2(4032*3,0)));
	floresta_bg2.Render(RENDERPOS(Vec2(4032*4,0)));
	floresta_bg2.Render(RENDERPOS(Vec2(4032*5,0)));
	floresta_bg3.Render(RENDERPOS(Vec2(4032*6,0)));
	level.tileMap.Render();
	RenderArray();
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
