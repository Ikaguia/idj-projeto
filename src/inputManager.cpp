#include <inputManager.hpp>

InputManager::InputManager(){}
InputManager::~InputManager(){}

void InputManager::Update(float time){
	int x,y;
	SDL_GetMouseState(&x,&y);
	mouseMotion = (mouse.x!=x || mouse.y!=y);
	mouse.x = (float)x;
	mouse.y = (float)y;
	quitRequested=false;
	
	cursorBlinker.Update(time);

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
		
		if(text != nullptr){
			if(event.type==SDL_TEXTINPUT){
				string input(event.text.text);
				text->insert(cursor,input);
				cursor += input.size();
			}
			else if(event.type==SDL_KEYDOWN){
				cursorBlinker.Restart();
				if(event.key.keysym.sym == SDLK_BACKSPACE && text->size() && cursor){
					text->erase(--cursor,1);
					if(cursor>text->size())
						cursor=text->size();
				}
				else if(event.key.keysym.sym == SDLK_LEFT && cursor > 0){
					cursor--;
				}
				else if(event.key.keysym.sym == SDLK_RIGHT && cursor < text->size()){
					cursor++;
				}
			}
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
bool InputManager::IsMouseMoving(){
	return mouseMotion;
}

Vec2 InputManager::GetMouse(){
	return mouse;
}

int InputManager::GetMouseX(){
	return mouse.x;
}
int InputManager::GetMouseY(){
	return mouse.y;
}

void InputManager::StartTextInput(string* t){
	if(t == nullptr) return;
	SDL_StartTextInput();
	text = t;
	cursor = text->size();
	cursorBlinker.Restart();
}
void InputManager::StopTextInput(string* t){
	if(text != t) return;
	text = nullptr;
	SDL_StopTextInput();
}
uint InputManager::GetTextCursor(){
	return cursor;
}
bool InputManager::TextCursorBlink(){
	return !((int)(cursorBlinker.Get()/0.5)%2);
}

bool InputManager::QuitRequested(){
	return quitRequested;
}

InputManager& InputManager::GetInstance(){
	static InputManager uniqueInst;
	return uniqueInst;
}
