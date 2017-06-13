#ifndef TEXTHPP
#define TEXTHPP

#include <common.hpp>

#include <geometry.hpp>

class Text{
public:
	enum Style{SOLID,SHADED,BLENDED};
	enum Align{LEFT, RIGHT, CENTERED};
private:
	class TextLine;
	
	vector<TextLine> lineArray;
	shared_ptr<TTF_Font> font;
	string fontName;
	string text;
	Style style;
	int fontSize;
	SDL_Color color;
	Align alignment;
	Hotspot hotspot;
	Rect box;

	void RemakeTexture();
public:
	Text(string txt,int fSize,SDL_Color c=SDL_COLOR_WHITE,Style st=Style::BLENDED,string file="font/arial.ttf",int x=0,int y=0);
	~Text();

	void Render(Vec2 camera = {0,0}, Rect* clipRect=nullptr);

	void SetPos(int x,int y);
	void SetPos(Vec2 v);

	void SetText(string txt);
	void SetLine(int line, string txt);
	
	void SetColor(SDL_Color c);
	void SetStyle(Style st);
	void SetFontSize(int fSize);
	void SetAlignment(Align al);
	void SetHotspot(Hotspot h=Hotspot::CENTER);
	
	Rect GetBox() const;
};

class Text::TextLine {
public:
	SDL_Texture* texture;
	string text;
	Rect box;
	
	TextLine();
};

#endif//TEXTHPP
