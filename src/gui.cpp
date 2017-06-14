#include <gui.hpp>

#include <game.hpp>
#include <inputManager.hpp>

#define BUTTON_MARGIN 3
#define DEFAULT_WIDTH 24
#define DEFAULT_HEIGHT 24
#define DEFAULT_FONT_SIZE 14
#define DEFAULT_TEXTBOX_WIDTH 120

#define BASE_COLOR 63,63,63,255
#define HIGHLIGHT_COLOR 127,127,127,255
#define PRESS_COLOR 47,47,47,255
#define BOX_COLOR 255,255,255,255

//GUI_Button
GUI_Button::GUI_Button(Vec2 pos):box{pos.x,pos.y,DEFAULT_WIDTH,DEFAULT_HEIGHT}{
}
void GUI_Button::Update(){
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)){
		//if(hover) cout<<"click!\n";
		press = false;
	}
}
void GUI_Button::Render(){
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	
	if(hover){
		SET_COLOR(HIGHLIGHT_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		DRAW_RECT(&rect);

		if(press){
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
void GUI_CheckButton::Update(){
	hover=box.contains(INPUT.GetMouse());
	if(hover && INPUT.MousePress(MBUTTON_LEFT))	press = true;
	if(press && INPUT.MouseRelease(MBUTTON_LEFT)){
		if(hover){
			value = !value;
			//cout<<"click!\n";
		}
		press = false;
	}
}
void GUI_CheckButton::Render(){
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	SET_COLOR(BOX_COLOR);
	rect.x+=BUTTON_MARGIN*2;
	rect.y+=BUTTON_MARGIN*2;
	rect.w-=BUTTON_MARGIN*4;
	rect.h-=BUTTON_MARGIN*4;
	DRAW_RECT(&rect);
	
	if(value){
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
void GUI_TextButton::Render(){
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	
	if(hover){
		SET_COLOR(HIGHLIGHT_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		DRAW_RECT(&rect);

		if(press){
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
void GUI_IconButton::Render(){
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	
	if(hover){
		SET_COLOR(HIGHLIGHT_COLOR);
		rect.x+=BUTTON_MARGIN;
		rect.y+=BUTTON_MARGIN;
		rect.w-=BUTTON_MARGIN*2;
		rect.h-=BUTTON_MARGIN*2;
		DRAW_RECT(&rect);

		if(press){
			SET_COLOR(PRESS_COLOR);
			rect.x++;
			rect.y++;
			rect.w-=2;
			rect.h-=2;
			FILL_RECT(&rect);
		}
	}
	box.w = icon.GetWidth()+BUTTON_MARGIN*4;
	box.h = icon.GetHeight()+BUTTON_MARGIN*4;
	icon.Render((int)(box.x+BUTTON_MARGIN*2), (int)(box.y+BUTTON_MARGIN*2));
}

//GUI_InputBox
GUI_InputBox::GUI_InputBox(Vec2 pos):GUI_Button::GUI_Button(pos), text{" ", DEFAULT_FONT_SIZE}{
	box.w = DEFAULT_TEXTBOX_WIDTH;
	text.SetHotspot(Hotspot::LEFT);
	text.SetPos({box.x+BUTTON_MARGIN*2,box.y+box.h/2});
	text.SetAlignment(Text::Align::LEFT);
}
GUI_InputBox::~GUI_InputBox(){
		INPUT.StopTextInput(&input);
}
void GUI_InputBox::Update(){
	hover=box.contains(INPUT.GetMouse());
	bool closed = false;
	if(INPUT.MousePress(MBUTTON_LEFT)){
		if(!press){
			if(hover){
				press=true;
				INPUT.StartTextInput(&input);
			}
		}
		else if(!hover){
			closed =true;
		}
			
	}
	if(!press) return;
	
	if(closed || INPUT.KeyPress(KEY_ENTER)){
		press = false;
		INPUT.StopTextInput(&input);
		while(input[0] == ' ')
			input.erase(0);
		while(input[input.size()-1] == ' ')
			input.pop_back();
		if(!input.empty())
			SetValue();
		input.clear();
	}
	else if(INPUT.KeyPress(KEY_ESC)){
		press = false;
		INPUT.StopTextInput(&input);
		input.clear();
	}
}
void GUI_InputBox::Render(){
	SDL_Rect rect=box.sdlRect();

	SET_COLOR(BASE_COLOR);
	FILL_RECT(&rect);
	if(press){
		SET_COLOR(BOX_COLOR);
	}
	else{
		SET_COLOR(HIGHLIGHT_COLOR);
	}
	rect.x+=BUTTON_MARGIN;
	rect.y+=BUTTON_MARGIN;
	rect.w-=BUTTON_MARGIN*2;
	rect.h-=BUTTON_MARGIN*2;
	DRAW_RECT(&rect);
	
	rect.x+=BUTTON_MARGIN;
	rect.w-=BUTTON_MARGIN*2;
	
	
	if(press){
		Rect textRect = text.GetBox();
		int textEnd = (textRect.x+textRect.w)-1-offset;
		int rectEnd = (rect.x+rect.w)-1;
	
		if((textRect.w > rect.w) && (textEnd < rectEnd)){
			offset -= (rectEnd - textEnd);
		}
		
		Vec2 cursor(rect.x-offset,rect.y+2);
		int c = INPUT.GetTextCursor();
		if(c>0){
			text.SetText(input.substr(0,c));
			cursor.x+=(text.GetBox().w-1);
		}
		
		if(cursor.x >= (rect.x+rect.w)){
			offset += cursor.x-(rect.x+rect.w-1);
			cursor.x = (rect.x+rect.w-1);
		}
		else if(cursor.x < rect.x){
			offset -= rect.x-cursor.x;
			cursor.x = rect.x;
		}
		
		if(INPUT.TextCursorBlink())
			DRAW_LINE(cursor.x,cursor.y,cursor.x,cursor.y+DEFAULT_FONT_SIZE);
		
		text.SetText(input);
		Rect textClip{(float)offset,0,(float)rect.w,DEFAULT_FONT_SIZE+2};
		text.Render({(float)offset,0}, &textClip);
	}
	else {
		text.SetText(GetValue());
		Rect textClip{0,0,(float)rect.w,DEFAULT_FONT_SIZE+2};
		text.Render({0,0}, &textClip);
	}
}
//GUI_StringBox
GUI_StringBox::GUI_StringBox(Vec2 pos, string& v):GUI_InputBox::GUI_InputBox(pos),value{v}{}

void GUI_StringBox::SetValue(){
	value = input; 
}
string GUI_StringBox::GetValue()const{
	return value;
}
//GUI_IntBox
GUI_IntBox::GUI_IntBox(Vec2 pos, int& v, int l, int h):GUI_InputBox::GUI_InputBox(pos),value{v},low{l},high{h}{}

void GUI_IntBox::SetValue(){
	value = stoi(input);
	value = min(high,max(value,low));
}
string GUI_IntBox::GetValue()const{
	return to_string(value);
}
