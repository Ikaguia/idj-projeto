#ifndef TEXTHPP
#define TEXTHPP

#include <common.hpp>

#include <geometry.hpp>

class Text{
public:
	enum TextStyle{SOLID,SHADED,BLENDED};
private:
	shared_ptr<TTF_Font> font;
	SDL_Texture* texture;

	string fontName;
	string text;
	TextStyle style;
	int fontSize;
	SDL_Color color;
	Rect box;

	void RemakeTexture();
public:
	Text(string txt,int fSize,SDL_Color c=SDL_COLOR_WHITE,TextStyle st=TextStyle::BLENDED,string file="font/Call me maybe.ttf",int x=0,int y=0);
	~Text();

	void Render(Vec2 camera = {0,0});

	void SetPos(int x,int y,bool centerX=false,bool centerY=false);
	void SetPos(Vec2 v,bool centerX=false,bool centerY=false);

	void SetText(string txt);
	void SetColor(SDL_Color c);
	void SetStyle(TextStyle st);
	void SetFontSize(int fSize);
};

#endif//TEXTHPP
