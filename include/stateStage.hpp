#ifndef STATESTAGEHPP
#define STATESTAGEHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <common.hpp>
#include <state.hpp>
#include <sprite.hpp>
#include <tileSet.hpp>
#include <tileMap.hpp>
#include <music.hpp>


class StateStage: public State{
	Sprite bg;

	TileSet tileSet;
	TileMap tileMap;

	GameObject* player;
public:
	StateStage(string fileTSet,string fileTMap,string fileBG);
	~StateStage();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//STATESTAGEHPP