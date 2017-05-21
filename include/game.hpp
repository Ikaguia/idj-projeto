#ifndef GAMEHPP
#define GAMEHPP

#include <common.hpp>

#include <geometry.hpp>
#include <state.hpp>

#define GAMEINST Game::GetInstance()
#define GAMESTATE GAMEINST.GetCurrentState()
#define GAMERENDER GAMEINST.GetRenderer()
#define WINSIZE GAMEINST.winSize

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
	const Vec2 winSize;

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
