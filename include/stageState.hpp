#ifndef STAGESTATEHPP
#define STAGESTATEHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <common.hpp>
#include <state.hpp>
#include <sprite.hpp>
#include <tileSet.hpp>
#include <tileMap.hpp>
#include <music.hpp>


class StageState: public State{
	Sprite bg;

	TileSet tileSet;
	TileMap tileMap;

	Music music;
public:

	StageState();
	~StageState();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//STAGESTATEHPP