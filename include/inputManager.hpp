#ifndef INPUTMANAGERHPP
#define INPUTMANAGERHPP

#include <common.hpp>

#include <geometry.hpp>

#define KEY_LEFT		SDLK_LEFT
#define KEY_RIGHT		SDLK_RIGHT
#define KEY_UP			SDLK_UP
#define KEY_DOWN		SDLK_DOWN
#define KEY_ESC			SDLK_ESCAPE
#define KEY_SPACE		SDLK_SPACE

#define KEY_F(x)		SDLK_F ## x
#define KEY(x)			SDLK_ ## x

#define MBUTTON_LEFT	SDL_BUTTON_LEFT
#define MBUTTON_RIGHT 	SDL_BUTTON_RIGHT
// #define MBUTTON_MIDDLE 	SDL_BUTTON_MIDDLE

#define INPUTMAN InputManager::GetInstance()
#define MOUSE INPUTMAN.GetMouse()

class InputManager{
	bitset<6> mouseState;//vetor de 6 booleanos //bitset ocupa o menor numero de bytes possivel(char para < 8 bits)
	array<int,6> mouseUpdate;
	unordered_map<int,bool> keyState;
	unordered_map<int,int>  keyUpdate;
	int updateCounter;
	Vec2 mouse;

	bool quitRequested;

	InputManager();
	~InputManager();
public:
	void Update();

	bool KeyPress(int key);
	bool KeyRelease(int key);
	bool IsKeyDown(int key);

	bool MousePress(int button);
	bool MouseRelease(int button);
	bool IsMouseDown(int button);

	Vec2 GetMouse();
	int GetMouseX();
	int GetMouseY();

	bool QuitRequested();

	static InputManager& GetInstance();
};

#endif//INPUTMANAGERHPP
