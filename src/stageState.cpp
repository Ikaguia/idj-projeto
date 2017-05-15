#include <memory>

#include <stageState.hpp>
#include <camera.hpp>
#include <resources.hpp>
#include <collision.hpp>
#include <music.hpp>
#include <game.hpp>
#include <endState.hpp>
#include <player.hpp>



StageState::StageState():State::State(),bg{Sprite(RESOURCESFOLDER+"img/ocean.jpg")},tileSet{TileSet(64,64,RESOURCESFOLDER+"img/tileset.png")},tileMap{TileMap(RESOURCESFOLDER+"map/tileMap.txt",&tileSet)}{
	LoadAssets();
	objectArray.emplace_back(unique_ptr<GameObject>(new Player(704,640)));
	Camera::Follow(Player::player);
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

	UpdateArray(time);

	CheckCollisions();

	UpdateArrayPos(time);
	UpdateArrayPhysics(time);


	CheckEnd(time);
}
void StageState::UpdateArrayPos(float time){
	for(auto &i:objectArray)i->UpdatePos(time);
}
void StageState::UpdateArrayPhysics(float time){
	for(auto &i:objectArray)i->UpdatePhysics(time);
}
void StageState::CheckCollisions(){
	//check collisions object-ground
	for(auto &it:objectArray){
		if((*it).Is("Animation"))continue;//animation does not collide
		it->CheckCollisionGround(tileMap);
	}

	//check colllisions object-object
	for(auto &it:objectArray){
		if((*it).Is("Animation"))continue;//animation does not collide
		for(auto &it2:objectArray){
			if((*it2).Is("Animation"))continue;//animation does not collide
			if(Collision::IsColliding((*it).box,(*it2).box,(*it).rotation,(*it2).rotation)){
				(*it).NotifyCollision(it2.get());
			}
		}
	}
}
void StageState::CheckEnd(float time){
	static Timer endTimer;//timer para esperar os soms terminarem de tocar
	static EndState* endP=nullptr;//ao criar adiantado o proximo estado, evita o jogo demorar para passar para ele
	if(Player::player==nullptr){
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

	static Sprite yellowDotDebug("img/point_yellow.jpg",1,1.0f);
	static Sprite greenDotDebug("img/point_green.jpg",1,1.0f);
	static Sprite blueDotDebug("img/point_blue.jpg",1,1.0f);
	static Sprite redDotDebug("img/point_red.jpg",1,1.0f);
	for(Vec2& v:debugPointsGreen)  greenDotDebug.Render(-Camera::pos.x+v.x,-Camera::pos.y+v.y);
	for(Vec2& v:debugPointsBlue)    blueDotDebug.Render(-Camera::pos.x+v.x,-Camera::pos.y+v.y);
	for(Vec2& v:debugPointsRed)      redDotDebug.Render(-Camera::pos.x+v.x,-Camera::pos.y+v.y);
	for(Vec2& v:debugPointsYellow)yellowDotDebug.Render(-Camera::pos.x+v.x,-Camera::pos.y+v.y);
	debugPointsGreen.clear();
	debugPointsBlue.clear();
	debugPointsRed.clear();
	debugPointsYellow.clear();
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
	//Resources::GetMusic(RESOURCESFOLDER+"audio/stageState.ogg");
	Resources::GetImage(RESOURCESFOLDER+"img/player.png");
	Resources::GetSound(RESOURCESFOLDER+"audio/boom.wav");
}
