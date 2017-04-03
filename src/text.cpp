#include <text.hpp>
#include <resources.hpp>
#include <game.hpp>

Text::Text(string file,int fSize,TextStyle st,string txt,SDL_Color c,int x,int y):texture{nullptr},fontName{file}{
	SetColor(c);
	SetText(txt);
	SetStyle(st);
	SetFontSize(fSize);
	box.x=x;
	box.y=y;
}
Text::~Text(){
	if(texture)SDL_DestroyTexture(texture);
}

void Text::Render(int cameraX,int cameraY){
	if(texture){
		SDL_Rect dest;
		dest.x=box.x-cameraX;
		dest.y=box.y-cameraY;
		dest.w=box.w;
		dest.h=box.h;
		SDL_RenderCopy(GAMERENDER,texture,nullptr,&dest);
	}
}

void Text::SetPos(int x,int y,bool centerX,bool centerY){
	if(centerX)box.x=x-(box.w/2);
	else box.x=x;
	if(centerY)box.y=y-(box.h/2);
	else box.y=y;
}

void Text::SetText(string txt){
	text=txt;
	RemakeTexture();
}
void Text::SetColor(SDL_Color c){
	color.r=c.r;
	color.g=c.g;
	color.b=c.b;
	color.a=c.a;
	RemakeTexture();
}
void Text::SetStyle(TextStyle st){
	style=st;
	RemakeTexture();
}
void Text::SetFontSize(int fSize){
	fontSize=fSize;
	font = Resources::GetFont(fontName,fontSize);
	RemakeTexture();
}

void Text::RemakeTexture(){
	if(font.get()){
		if(texture)SDL_DestroyTexture(texture);
		SDL_Surface *surface;
		if(style==TextStyle::SOLID)surface=TTF_RenderText_Solid(font.get(),text.c_str(),color);
		if(style==TextStyle::SHADED)surface=TTF_RenderText_Shaded(font.get(),text.c_str(),color,SDL_COLOR_BLACK);
		if(style==TextStyle::BLENDED)surface=TTF_RenderText_Blended(font.get(),text.c_str(),color);
		texture = SDL_CreateTextureFromSurface(GAMERENDER,surface);
		box.w=surface->w;
		box.h=surface->h;
		SDL_FreeSurface(surface);
	}
}
