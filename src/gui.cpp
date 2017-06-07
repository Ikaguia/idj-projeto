#include <gui.hpp>

#include <game.hpp>
#include <inputManager.hpp>

#define BUTTON_MARGIN 3
#define DEFAULT_WIDTH 24
#define DEFAULT_HEIGHT 24
#define DEFAULT_FONT_SIZE 12

GUI_Button::GUI_Button():box{0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT}{
}

void GUI_Button::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		if(hover) cout<<"click!\n";
		press = false;
	}
}

void GUI_Button::Render() {
	SDL_Rect rect=box.sdlRect();

	SDL_SetRenderDrawColor(GAMERENDER, 63, 63, 63, 255);
	SDL_RenderFillRect(GAMERENDER,&rect);
	
	if(hover) {
		SDL_SetRenderDrawColor(GAMERENDER, 127, 127, 127, 255);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		SDL_RenderDrawRect(GAMERENDER, &rect);

		if(press) {
			SDL_SetRenderDrawColor(GAMERENDER, 47, 47, 47, 255);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			SDL_RenderFillRect(GAMERENDER,&rect);
		}
	}
}

GUI_CheckButton::GUI_CheckButton():GUI_Button::GUI_Button(){
}

void GUI_CheckButton::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		if(hover) {
			value = !value;
			cout<<"click!\n";
		}
		press = false;
	}
}

void GUI_CheckButton::Render() {
	SDL_Rect rect=box.sdlRect();

	SDL_SetRenderDrawColor(GAMERENDER, 63, 63, 63, 255);
	SDL_RenderFillRect(GAMERENDER,&rect);
	SDL_SetRenderDrawColor(GAMERENDER, 255, 255, 255, 255);
	rect.x+=BUTTON_MARGIN*2;
	rect.y+=BUTTON_MARGIN*2;
	rect.w-=BUTTON_MARGIN*4;
	rect.h-=BUTTON_MARGIN*4;
	SDL_RenderDrawRect(GAMERENDER, &rect);
	
	if(value) {
		if(press && hover) SDL_SetRenderDrawColor(GAMERENDER, 127, 127, 127, 255);
		else SDL_SetRenderDrawColor(GAMERENDER, 255, 255, 255, 255);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		SDL_RenderFillRect(GAMERENDER, &rect);
	}
}

GUI_TextButton::GUI_TextButton(string l):GUI_Button::GUI_Button(), label{l, DEFAULT_FONT_SIZE}{
	label.SetHotspot();
}

void GUI_TextButton::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		if(hover) cout<<"click!\n";
		press = false;
	}
	box.w = label.GetBox().w+BUTTON_MARGIN*4;
}

void GUI_TextButton::Render() {
	SDL_Rect rect=box.sdlRect();

	SDL_SetRenderDrawColor(GAMERENDER, 63, 63, 63, 255);
	SDL_RenderFillRect(GAMERENDER,&rect);
	
	if(hover) {
		SDL_SetRenderDrawColor(GAMERENDER, 127, 127, 127, 255);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		SDL_RenderDrawRect(GAMERENDER, &rect);

		if(press) {
			SDL_SetRenderDrawColor(GAMERENDER, 47, 47, 47, 255);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			SDL_RenderFillRect(GAMERENDER,&rect);
		}
	}
	label.SetPos(box.center());
	label.Render();
}

GUI_IconButton::GUI_IconButton(string i):GUI_Button::GUI_Button(), icon{i}{
}

void GUI_IconButton::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		if(hover) cout<<"click!\n";
		press = false;
	}
	box.w = icon.GetWidth()+BUTTON_MARGIN*4;
	box.h = icon.GetHeight()+BUTTON_MARGIN*4;
}

void GUI_IconButton::Render() {
	SDL_Rect rect=box.sdlRect();

	SDL_SetRenderDrawColor(GAMERENDER, 63, 63, 63, 255);
	SDL_RenderFillRect(GAMERENDER,&rect);
	
	if(hover) {
		SDL_SetRenderDrawColor(GAMERENDER, 127, 127, 127, 255);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		SDL_RenderDrawRect(GAMERENDER, &rect);

		if(press) {
			SDL_SetRenderDrawColor(GAMERENDER, 47, 47, 47, 255);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			SDL_RenderFillRect(GAMERENDER,&rect);
		}
	}
	icon.Render((int)(box.x+BUTTON_MARGIN*2), (int)(box.y+BUTTON_MARGIN*2));
}
