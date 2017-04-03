#include <memory>

#include <stageState.hpp>
#include <alien.hpp>
#include <pinguims.hpp>
#include <camera.hpp>
#include <resources.hpp>
#include <collision.hpp>
#include <music.hpp>
#include <game.hpp>
#include <endState.hpp>



StageState::StageState():State::State(),bg{Sprite(RESOURCESFOLDER+"img/ocean.jpg")},tileSet{TileSet(64,64,RESOURCESFOLDER+"img/tileset.png")},tileMap{TileMap(RESOURCESFOLDER+"map/tileMap.txt",&tileSet)}{
	LoadAssets();
	FOR(i,5)objectArray.emplace_back(unique_ptr<GameObject>(new Alien(rand()%1024,rand()%600,8)));
	objectArray.emplace_back(unique_ptr<GameObject>(new Pinguims(704,640)));
	Camera::Follow(Pinguims::player);
	music.Open(RESOURCESFOLDER+"audio/stageState.ogg");
}
StageState::~StageState(){
	music.Stop();
	FOR(i,sounds.size())sounds[i]->Stop();
}

void StageState::Update(float time){
	Camera::Update(time);
	if(INPUTMAN.QuitRequested())quitRequested=true;
	if(INPUTMAN.KeyPress(ESCAPE_KEY))popRequested=true;
	// if(INPUTMAN.KeyPress(ESCAPE_KEY))quitRequested=true;

	UpdateArray(time);

	FOR(i,objectArray.size()){
		if(objectArray[i]->Is("Animation"))continue;//animation does not collide
		FOR2(j,i+1,objectArray.size()){
			if(objectArray[j]->Is("Animation"))continue;//animation does not collide
			if(Collision::IsColliding(objectArray[i]->box,objectArray[j]->box,objectArray[i]->rotation,objectArray[j]->rotation)){
				objectArray[i]->NotifyCollision(*objectArray[j]);
			}
		}
	}

	static Timer endTimer;//timer para esperar os soms terminarem de tocar
	static EndState* endP=nullptr;//ao criar adiantado o proximo estado, evita o jogo demorar para passar para ele
	if(Alien::count==0){
		endTimer.Update(time);
		if(endP==nullptr)endP=new EndState(StateData(true));
		if(endTimer.Get()>=2.0f){
			endTimer.Restart();
			popRequested=true;
			GAMEINST.Push(endP);
			endP=nullptr;
		}
	}
	if(Pinguims::player==nullptr){
		endTimer.Update(time);
		if(endP==nullptr)endP=new EndState(StateData(false));
		if(endTimer.Get()>=2.0f){
			endTimer.Restart();
			popRequested=true;
			GAMEINST.Push(endP);
			endP=nullptr;
		}
	}
}
void StageState::Render(){
	bg.Render(0,0);
	tileMap.RenderLayer(0,Camera::pos.x,Camera::pos.y);

	RenderArray();

	tileMap.RenderLayer(1,Camera::pos.x*2,Camera::pos.y*2);
}

void StageState::Pause(){
	music.Stop();
}
void StageState::Resume(){
	music.Play(-1);
}


void StageState::LoadAssets(){
	//Resources::GetImage(RESOURCESFOLDER+"img/ocean.jpg");
	//Resources::GetImage(RESOURCESFOLDER+"img/tileset.png");
	Resources::GetImage(RESOURCESFOLDER+"img/alien.png");
	Resources::GetImage(RESOURCESFOLDER+"img/aliendeath.png");
	Resources::GetImage(RESOURCESFOLDER+"img/minion.png");
	Resources::GetImage(RESOURCESFOLDER+"img/minionbullet2.png");
	Resources::GetImage(RESOURCESFOLDER+"img/miniondeath.png");
	Resources::GetImage(RESOURCESFOLDER+"img/penguin.png");
	Resources::GetImage(RESOURCESFOLDER+"img/cubngun.png");
	Resources::GetImage(RESOURCESFOLDER+"img/penguinbullet.png");
	Resources::GetImage(RESOURCESFOLDER+"img/penguindeath.png");
	//Resources::GetMusic(RESOURCESFOLDER+"audio/stageState.ogg");
	Resources::GetSound(RESOURCESFOLDER+"audio/boom.wav");
}
