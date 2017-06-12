#include <gui.hpp>

#include <game.hpp>
#include <inputManager.hpp>

#define BUTTON_MARGIN 3
#define DEFAULT_WIDTH 24
#define DEFAULT_HEIGHT 24
#define DEFAULT_FONT_SIZE 14
#define DEFAULT_TEXTBOX_WIDTH 240

#define BASE_COLOR 63,63,63,255
#define HIGHLIGHT_COLOR 127,127,127,255
#define PRESS_COLOR 47,47,47,255
#define BOX_COLOR 255,255,255,255

//GUI_Button
GUI_Button::GUI_Button(Vec2 pos):box{pos.x,pos.y,DEFAULT_WIDTH,DEFAULT_HEIGHT}{
}
void GUI_Button::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		//if(hover) cout<<"click!\n";
		press = false;
	}
}
void GUI_Button::Render() {
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	
	if(hover) {
		SET_COLOR(HIGHLIGHT_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		DRAW_RECT(&rect);

		if(press) {
			SET_COLOR(PRESS_COLOR);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			FILL_RECT(&rect);
		}
	}
}

//GUI_CheckButton
GUI_CheckButton::GUI_CheckButton(Vec2 pos):GUI_Button::GUI_Button(pos){
}
void GUI_CheckButton::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		if(hover) {
			value = !value;
			//cout<<"click!\n";
		}
		press = false;
	}
}
void GUI_CheckButton::Render() {
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	SET_COLOR(BOX_COLOR);
	rect.x+=BUTTON_MARGIN*2;
	rect.y+=BUTTON_MARGIN*2;
	rect.w-=BUTTON_MARGIN*4;
	rect.h-=BUTTON_MARGIN*4;
	DRAW_RECT(&rect);
	
	if(value) {
		if(press && hover) SET_COLOR(HIGHLIGHT_COLOR);
		else SET_COLOR(BOX_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		FILL_RECT(&rect);
	}
}

//GUI_TextButton
GUI_TextButton::GUI_TextButton(Vec2 pos,string l):GUI_Button::GUI_Button(pos), label{l, DEFAULT_FONT_SIZE}{
	label.SetHotspot();
	box.w = label.GetBox().w+BUTTON_MARGIN*4;
	label.SetPos(box.center());
}
void GUI_TextButton::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		//if(hover) cout<<"click!\n";
		press = false;
	}
}
void GUI_TextButton::Render() {
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	
	if(hover) {
		SET_COLOR(HIGHLIGHT_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		DRAW_RECT(&rect);

		if(press) {
			SET_COLOR(PRESS_COLOR);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			FILL_RECT(&rect);
		}
	}
	label.Render();
}

//GUI_IconButton
GUI_IconButton::GUI_IconButton(Vec2 pos, string i):GUI_Button::GUI_Button(pos), icon{i}{
}
void GUI_IconButton::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)) {
		//if(hover) cout<<"click!\n";
		press = false;
	}
	box.w = icon.GetWidth()+BUTTON_MARGIN*4;
	box.h = icon.GetHeight()+BUTTON_MARGIN*4;
}
void GUI_IconButton::Render() {
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	
	if(hover) {
		SET_COLOR(HIGHLIGHT_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		DRAW_RECT(&rect);

		if(press) {
			SET_COLOR(PRESS_COLOR);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			FILL_RECT(&rect);
		}
	}
	icon.Render((int)(box.x+BUTTON_MARGIN*2), (int)(box.y+BUTTON_MARGIN*2));
}

//GUI_TextBox
GUI_TextBox::GUI_TextBox(Vec2 pos):GUI_Button::GUI_Button(pos), text{"abcdefghijklmnopqrstuvwxyAAAAAAAAAAa cçeoiv eçoheçoeçfiz", DEFAULT_FONT_SIZE}{
	box.w = DEFAULT_TEXTBOX_WIDTH;
	text.SetHotspot(Hotspot::LEFT);
	text.SetPos({box.x+BUTTON_MARGIN*2,box.y+box.h/2});
	text.SetAlignment(Text::Align::LEFT);
}
void GUI_TextBox::Update() {
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MousePress(MBUTTON_LEFT)) {
		if(hover) cout<<"click!\n";
		else press = false;
	}
}
void GUI_TextBox::Render() {
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	if(press) {
		SET_COLOR(BOX_COLOR);
	}
	else {
		SET_COLOR(HIGHLIGHT_COLOR);
	}
	rect.x+=BUTTON_MARGIN;
	rect.y+=BUTTON_MARGIN;
	rect.w-=BUTTON_MARGIN*2;
	rect.h-=BUTTON_MARGIN*2;
	DRAW_RECT(&rect);
	
	
	Rect textClip{0,0,box.w-(BUTTON_MARGIN*4),DEFAULT_FONT_SIZE+2};
	text.Render({0,0}, &textClip);
}
