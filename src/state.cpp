#include <memory>

#include <state.hpp>
#include <game.hpp>
#include <inputManager.hpp>
#include <camera.hpp>
#include <alien.hpp>
#include <resources.hpp>
#include <pinguims.hpp>
#include <collision.hpp>


State::State():quitRequested{false},bg{Sprite("../resources/img/ocean.jpg")},tileSet{TileSet(64,64,"../resources/img/tileset.png")},tileMap{TileMap("../resources/map/tileMap.txt",&tileSet)}{
	loadAssets();
	objectArray.emplace_back(unique_ptr<GameObject>(new Alien(512,300,8)));
	objectArray.emplace_back(unique_ptr<GameObject>(new Pinguims(704,640)));
	Camera::Follow(Pinguims::player);
}

State::~State(){
	objectArray.clear();
	Resources::clearImages();
}

bool State::QuitRequested(){
	return quitRequested;
}

void State::loadAssets(){
	Resources::getImage("../resources/img/alien.png");
	Resources::getImage("../resources/img/aliendeath.png");
	Resources::getImage("../resources/img/minion.png");
	Resources::getImage("../resources/img/minionbullet2.png");
	Resources::getImage("../resources/img/miniondeath.png");
	Resources::getImage("../resources/img/penguin.png");
	Resources::getImage("../resources/img/cubngun.png");
	Resources::getImage("../resources/img/penguinbullet.png");
	Resources::getImage("../resources/img/penguindeath.png");
}
void State::render(){
	bg.render(0,0);
	tileMap.renderLayer(0,Camera::pos.x,Camera::pos.y);
	for(const auto &i:objectArray)i->Render();
	tileMap.renderLayer(1,Camera::pos.x*2,Camera::pos.y*2);
}
void State::update(){
	Camera::Update(GAMEINST.GetDeltaTime());
	if(INPUTMAN.QuitRequested() || INPUTMAN.KeyPress(ESCAPE_KEY))quitRequested=true;

	int i=0;
	while(i<objectArray.size()){
		objectArray[i]->Update(GAMEINST.GetDeltaTime());
		if(objectArray[i]->IsDead())objectArray.erase(objectArray.begin() + i);
		else i++;
	}
	FOR(i,objectArray.size()){
		if(objectArray[i]->Is("Animation"))continue;//animation does not collide
		FOR2(j,i+1,objectArray.size()){
			if(objectArray[j]->Is("Animation"))continue;//animation does not collide
			if(Collision::IsColliding(objectArray[i]->box,objectArray[j]->box,objectArray[i]->rotation,objectArray[j]->rotation)){
				objectArray[i]->NotifyCollision(*objectArray[j]);
			}
		}
	}
}

void State::AddObject(GameObject* obj){
	objectArray.emplace_back(obj);
}
