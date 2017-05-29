#include <text.hpp>

#include <camera.hpp>
#include <game.hpp>
#include <resources.hpp>

Text::Text(string txt,int fSize,SDL_Color c,Style st,string file,int x,int y):fontName{file},alignment{Text::Align::LEFT},hotspot{Hotspot::TOP_LEFT}{
	SetColor(c);
	SetText(txt);
	SetStyle(st);
	SetFontSize(fSize);
	box.x=x;
	box.y=y;
}

Text::~Text(){
	for(auto& i : lineArray)
		if(i.texture)SDL_DestroyTexture(i.texture);
}

void Text::Render(Vec2 camera){
	int x = box.x-(camera.x*CAMERAZOOM);
	int y = box.y-(camera.y*CAMERAZOOM);
	
		 if(hotspot == Hotspot::TOP) 			 x-=(box.w/2);
	else if(hotspot == Hotspot::LEFT) 							y-=(box.h/2);
	else if(hotspot == Hotspot::TOP_RIGHT) 		 x-=(box.w);
	else if(hotspot == Hotspot::BOTTOM_LEFT)					y-=(box.h);
	else if(hotspot == Hotspot::CENTER)			{x-=(box.w/2);	y-=(box.h/2);}
	else if(hotspot == Hotspot::RIGHT)			{x-=(box.w);	y-=(box.h/2);}
	else if(hotspot == Hotspot::BOTTOM) 		{x-=(box.w/2);	y-=(box.h);}
	else if(hotspot == Hotspot::BOTTOM_RIGHT)	{x-=(box.w);	y-=(box.h);}
	
	for(auto& i : lineArray) {
		SDL_Rect dest;
		dest.x=x+i.box.x;
		dest.y=y+i.box.y;
		dest.w=i.box.w;
		dest.h=i.box.h;
		SDL_RenderCopy(GAMERENDER,i.texture,nullptr,&dest);
	}
}

void Text::SetPos(int x,int y){
	box.x=x;
	box.y=y;
}

void Text::SetPos(Vec2 v) {
	SetPos(v.x, v.y);
}

void Text::SetText(string txt){
	stringstream text(txt);
	lineArray.clear();
	for(TextLine line;getline(text, line.text);) {
		if(line.text=="")
			line.text = " ";
		lineArray.push_back(line);
	}
	
	RemakeTexture();
}

void Text::SetLine(int line, string txt) {
	if(line>=0 && line<(int)lineArray.size()) {
		lineArray[line].text = txt;	
		RemakeTexture();
	}
}

void Text::SetColor(SDL_Color c){
	color.r=c.r;
	color.g=c.g;
	color.b=c.b;
	color.a=c.a;
	RemakeTexture();
}

void Text::SetStyle(Style st){
	style=st;
	RemakeTexture();
}

void Text::SetFontSize(int fSize){
	fontSize=fSize;
	font = Resources::GetFont(fontName,fontSize);
	RemakeTexture();
}

void Text::SetAlignment(Align al) {
	alignment = al;
	RemakeTexture();
}

void Text::SetHotspot(Hotspot h) {
	hotspot = h;
}

Rect Text::GetBox()const {
	return box;
}

void Text::RemakeTexture(){
	if(font.get()){
		SDL_Surface *surface;
		box.h = 0;
		for(auto& i : lineArray) {
			if(i.texture)SDL_DestroyTexture(i.texture);
			if(style==Style::SOLID)surface=TTF_RenderText_Solid(font.get(),i.text.c_str(),color);
			else if(style==Style::SHADED)surface=TTF_RenderText_Shaded(font.get(),i.text.c_str(),color,SDL_COLOR_BLACK);
			else if(style==Style::BLENDED)surface=TTF_RenderText_Blended(font.get(),i.text.c_str(),color);
			i.texture = SDL_CreateTextureFromSurface(GAMERENDER,surface);
			
			i.box.w=surface->w;
			i.box.h=surface->h;
			if(i.box.w>box.w) box.w=i.box.w;
			i.box.y=box.h;
			box.h+=i.box.h;
			//cout<<"\""<<i.text<<"\" "<<i.box.x<<","<<i.box.y<<" "<<i.box.w<<","<<i.box.h<<endl;
		}
		SDL_FreeSurface(surface);
		
		if(alignment == Align::CENTERED) {
			for(auto& i : lineArray)
				i.box.x=(box.w-i.box.w)/2;
		}
		else if(alignment == Align::RIGHT) {
			for(auto& i : lineArray)
				i.box.x=(box.w-i.box.w);
		}
	}
}

Text::TextLine::TextLine():texture{nullptr}{
}
