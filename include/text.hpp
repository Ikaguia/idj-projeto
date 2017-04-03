#ifndef TEXTHPP
#define TEXTHPP

#include <SDL2/SDL_ttf.h>

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

	Text(string file,int fSize,TextStyle st,string txt,SDL_Color c=SDL_COLOR_WHITE,int x=0,int y=0);
	~Text();

	void Render(int cameraX=0,int cameraY=0);

	void SetPos(int x,int y,bool centerX=false,bool centerY=false);

	void SetText(string txt);
	void SetColor(SDL_Color c);
	void SetStyle(TextStyle st);
	void SetFontSize(int fSize);
};

#endif//TEXTHPP
