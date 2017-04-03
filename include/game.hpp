#ifndef GAMEHPP
#define GAMEHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdlib>
#include <ctime>

#include <common.hpp>
#include <state.hpp>

#define GAMEINST Game::GetInstance()
#define GAMESTATE GAMEINST.GetCurrentState()
#define GAMERENDER GAMEINST.GetRenderer()

class Game{
	static Game* instance;

	State* storedState;
	SDL_Window* window;
	SDL_Renderer* renderer;
	stack<unique_ptr<State>> stateStack;

	int frameStart;
	float dt;

	void CalculateDeltaTime();
public:
	Game(string title,int width,int height);
	~Game();

	static Game& GetInstance();
	SDL_Renderer* GetRenderer();
	State& GetCurrentState();

	void Push(State* state);

	void Run();

	float GetDeltaTime();
};

#endif//GAMEHPP