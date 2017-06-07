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
	bool hover;
	bool press;
public:
	GUI_Button();
	
	void virtual Update();
	void virtual Render();
};

class GUI_CheckButton : public GUI_Button{
	bool value;
public:
	GUI_CheckButton();
	
	void virtual Update();
	void virtual Render();
};

class GUI_TextButton:public GUI_Button{
	Text label;
public:
	GUI_TextButton(string l = "Text");
	
	void virtual Update();
	void virtual Render();
};

class GUI_IconButton:public GUI_Button{
	Sprite icon;
public:
	GUI_IconButton(string i);
	
	void virtual Update();
	void virtual Render();
};

#endif //GUIHPP
