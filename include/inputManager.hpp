#ifndef INPUTMANAGERHPP
#define INPUTMANAGERHPP

#include <common.hpp>

#include <geometry.hpp>
#include <timer.hpp>

#define KEY_LEFT		SDLK_LEFT
#define KEY_RIGHT		SDLK_RIGHT
#define KEY_UP			SDLK_UP
#define KEY_DOWN		SDLK_DOWN
#define KEY_ESC			SDLK_ESCAPE
#define KEY_SPACE		SDLK_SPACE
#define KEY_ENTER		SDLK_RETURN

#define KEY_F(x)		SDLK_F ## x
#define KEY(x)			SDLK_ ## x

#define MBUTTON_LEFT	SDL_BUTTON_LEFT
#define MBUTTON_RIGHT 	SDL_BUTTON_RIGHT
#define MBUTTON_MIDDLE 	SDL_BUTTON_MIDDLE

#define INPUT InputManager::GetInstance()
#define MOUSE INPUT.GetMouse()

class InputManager{
	bitset<6> mouseState;//vetor de 6 booleanos //bitset ocupa o menor numero de bytes possivel(char para < 8 bits)
	array<int,6> mouseUpdate;
	unordered_map<int,bool> keyState;
	unordered_map<int,int>  keyUpdate;
	int updateCounter=0;
	Vec2 mouse;
	bool mouseMotion;
	
	string* text=nullptr;
	uint cursor;
	Timer cursorBlinker;
	//uint selection_len; //unused

	bool quitRequested=false;

	InputManager();
	~InputManager();
public:
	void Update(float time);

	bool KeyPress(int key);
	bool KeyRelease(int key);
	bool IsKeyDown(int key);

	bool MousePress(int button);
	bool MouseRelease(int button);
	bool IsMouseDown(int button);
	bool IsMouseMoving();

	Vec2 GetMouse();
	int GetMouseX();
	int GetMouseY();
	
	void StartTextInput(string* t);
	void StopTextInput(string* t);
	uint GetTextCursor();
	bool TextCursorBlink();
 

	bool QuitRequested();

	static InputManager& GetInstance();
};

#endif//INPUTMANAGERHPP
