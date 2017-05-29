#include <inputManager.hpp>

InputManager::InputManager():mouseUpdate{{0}},updateCounter{0},quitRequested{false}{}
InputManager::~InputManager(){}

void InputManager::Update(){
	int x,y;
	SDL_GetMouseState(&x,&y);
	mouse.x = (float)x;
	mouse.y = (float)y;
	quitRequested=false;

	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type==SDL_QUIT)quitRequested=true;
		else if(event.type==SDL_MOUSEBUTTONDOWN){
			//cout << "mouse button down " << (int)event.button.button << endl;
			if(/*event.button.button>=0 && */event.button.button<6 && mouseState[event.button.button]!=true){
				mouseUpdate[event.button.button]=updateCounter;
				mouseState[event.button.button]=true;
			}
		}
		else if(event.type==SDL_MOUSEBUTTONUP){
			//cout << "mouse button up " << (int)event.button.button << endl;
			if(/*event.button.button>=0 && */event.button.button<6 && mouseState[event.button.button]!=false){
				mouseUpdate[event.button.button]=updateCounter;
				mouseState[event.button.button]=false;
			}
		}
		else if(event.type==SDL_KEYDOWN){
			//cout << "key down" << endl;
			if(!event.key.repeat){
				keyState[event.key.keysym.sym]=true;
				keyUpdate[event.key.keysym.sym]=updateCounter;
			}
		}
		else if(event.type==SDL_KEYUP){
			//cout << "key up" << endl;
			keyState[event.key.keysym.sym]=false;
			keyUpdate[event.key.keysym.sym]=updateCounter;
		}
	}
	updateCounter++;
}

bool InputManager::KeyPress(int key){
	return (keyState[key] && keyUpdate[key]==updateCounter-1);
}
bool InputManager::KeyRelease(int key){
	return ((!keyState[key]) && keyUpdate[key]==updateCounter-1);
}
bool InputManager::IsKeyDown(int key){
	return (keyState[key]);
}

bool InputManager::MousePress(int button){
	return (mouseState[button] && mouseUpdate[button]==updateCounter-1);
}
bool InputManager::MouseRelease(int button){
	return ((!mouseState[button]) && mouseUpdate[button]==updateCounter-1);
}
bool InputManager::IsMouseDown(int button){
	return (mouseState[button]);
}

Vec2 InputManager::GetMouse() {
	return mouse;
}

int InputManager::GetMouseX(){
	return mouse.x;
}
int InputManager::GetMouseY(){
	return mouse.y;
}

bool InputManager::QuitRequested(){
	return quitRequested;
}

InputManager& InputManager::GetInstance(){
	static InputManager uniqueInst;
	return uniqueInst;
}
