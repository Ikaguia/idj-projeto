#ifndef GAMEHPP
#define GAMEHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdlib>
#include <ctime>

#include <common.hpp>

#ifndef STATEHPP
class State;
#endif//STATEHPP

#define GAMEINST Game::getInstance()
#define GAMESTATE GAMEINST.getState()
#define GAMERENDER GAMEINST.getRenderer()

class Game{
	static Game* instance;
	SDL_Window* window;
	SDL_Renderer* renderer;
	State* state;

	int frameStart;
	float dt;

	void calculateDeltaTime();
public:
	Game(string title,int width,int height);
	~Game();

	void run();
	SDL_Renderer* getRenderer();
	State& getState();
	static Game& getInstance();

	float GetDeltaTime();
};

#endif//GAMEHPP