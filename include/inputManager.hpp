#ifndef INPUTMANAGERHPP
#define INPUTMANAGERHPP

#include <common.hpp>

#include <geometry.hpp>

#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define ESCAPE_KEY SDLK_ESCAPE
#define SPACEBAR_KEY SDLK_SPACE
#define KEY(x) SDLK_ ## x
#define W_KEY SDLK_w
#define A_KEY SDLK_a
#define S_KEY SDLK_s
#define D_KEY SDLK_d
#define KEY_W SDLK_w
#define KEY_A SDLK_a
#define KEY_S SDLK_s
#define KEY_D SDLK_d
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT

#define INPUTMAN InputManager::GetInstance()
#define MOUSE InputManager::GetMouse()

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
