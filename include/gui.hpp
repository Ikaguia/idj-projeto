#ifndef GUIHPP
#define GUIHPP

#include <common.hpp>

#include <geometry.hpp>
#include <sprite.hpp>
#include <text.hpp>

#define GUI_NONE 0
#define GUI_SAVE 1
#define GUI_SHOW_GRID 2
#define GUI_SHOW_COLLISION 3
//#define GUI_

class GUI_Button{
protected:
	Rect box;
	bool hover=false;
	bool press=false;
public:
	GUI_Button(Vec2 pos = {0,0});
	
	void virtual Update();
	void virtual Render();
};

class GUI_CheckButton : public GUI_Button{
	bool value;
public:
	GUI_CheckButton(Vec2 pos);
	
	void virtual Update();
	void virtual Render();
};

class GUI_TextButton:public GUI_Button{
	Text label;
public:
	GUI_TextButton(Vec2 pos, string l = "Text");
	
	void virtual Render();
};

class GUI_IconButton:public GUI_Button{
	Sprite icon;
public:
	GUI_IconButton(Vec2 pos, string i);
	
	void virtual Render();
};

class GUI_InputBox:public GUI_Button{
protected:
	Text text;
	string input;
	int offset=0;

	GUI_InputBox(Vec2 pos);
	~GUI_InputBox();
	
	void virtual SetValue()=0;
	string virtual GetValue()const=0;
public:	
	void Update();
	void Render();
};

class GUI_StringBox:public GUI_InputBox{
	string& value;
	
	void virtual SetValue();
	string virtual GetValue()const;
public:
	GUI_StringBox(Vec2 pos, string& v);
};

class GUI_IntBox:public GUI_InputBox{
	int& value;
	int low;
	int high;
	
	void virtual SetValue();
	string virtual GetValue()const;
public:
	GUI_IntBox(Vec2 pos, int& v, int l=INT_MIN, int h=INT_MAX);
};
#endif //GUIHPP
