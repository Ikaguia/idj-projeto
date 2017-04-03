#ifndef STATEHPP
#define STATEHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <common.hpp>
#include <sprite.hpp>
#include <gameObject.hpp>
#include <tileSet.hpp>
#include <tileMap.hpp>


class State{
	Sprite bg;
	vector<unique_ptr<GameObject>> objectArray;

	bool quitRequested;

	TileSet tileSet;
	TileMap tileMap;
public:

	State();
	~State();

	bool QuitRequested();

	void loadAssets();
	void update();
	void render();
	void AddObject(GameObject* obj);
};

#endif//STATEHPP