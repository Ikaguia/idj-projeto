#ifndef GUIHPP
#define GUIHPP

#include <common.hpp>

#include <geometry.hpp>
#include <sprite.hpp>
#include <text.hpp>

#define GUI_NEW map<string,vector<GUI_Element*>> GUI_MAP; string GUI_CURRENT
#define GUI_SET(container) GUI_CURRENT=#container
#define GUI_ADD(element) GUI_MAP[GUI_CURRENT].push_back(new element)
#define GUI_DIV() GUI_MAP[GUI_CURRENT].push_back(nullptr)
#define GUI_CLEAR() GUI_MAP.erase(GUI_CURRENT)
#define GUI_GET(container) GUI_MAP[#container]
#define GUI_CREATE(element) gui.PushElement(new element)

#define GUI_NONE 0
#define GUI_CLOSE (uint)-1
#define GUI_CONFIRM (uint)-2
#define GUI_CONFIRM_AND_CLOSE (uint)-3

typedef enum {NORMAL_SIZE, SHORT_SIZE, LONG_SIZE} Size;
typedef enum {SNAP_NONE, SNAP_LEFT, SNAP_RIGHT} Snap;

class GUI_Element{
protected:
	Rect box;
	bool visible=true;
	
	GUI_Element(const Vec2& pos);
public:
	virtual ~GUI_Element();
	
	virtual void Update()=0;
	virtual void Render()=0;
	
	void SetPos(const Vec2& pos);
	Rect GetBox()const;
	
	void SetVisible(bool v);
	bool IsVisible()const;
};

class GUI_Button:public GUI_Element{
protected:
	bool hover=false;
	bool press=false;
public:
	const uint action;

	GUI_Button(uint a, const Vec2& pos={0,0});
	
	virtual void Update();
	virtual void Render();
	
	void Reset();
	bool IsPressed()const;
	bool IsHovered()const;
};

class GUI_CheckButton:public GUI_Button{
	bool& value;
public:
	GUI_CheckButton(bool& v, const Vec2& pos={0,0});
	
	virtual void Update();
	virtual void Render();
};

class GUI_TextButton:public GUI_Button{
	Text label;
public:
	GUI_TextButton(uint a, const string& l = "Text", const Vec2& pos={0,0});
	
	virtual void Render();
};

class GUI_IconButton:public GUI_Button{
	Sprite icon;
public:
	GUI_IconButton(uint a, string i, const Vec2& pos={0,0});
	
	virtual void Render();
};

class GUI_InputBox:public GUI_Button{
protected:
	Text text;
	string input;
	int offset=0;

	GUI_InputBox(const Vec2& pos, Size s);
	~GUI_InputBox();
	
	virtual void SetValue()=0;
	virtual string GetValue()const=0;
public:	
	void Update();
	void Render();
};

class GUI_StringBox:public GUI_InputBox{
	string& value;
	
	virtual void SetValue();
	virtual string GetValue()const;
public:
	GUI_StringBox(string& v, Size s=NORMAL_SIZE, const Vec2& pos={0,0});
};

class GUI_IntBox:public GUI_InputBox{
	int& value;
	int low;
	int high;
	
	virtual void SetValue();
	virtual string GetValue()const;
public:
	GUI_IntBox(int& v, int l=INT_MIN, int h=INT_MAX, Size s=NORMAL_SIZE, const Vec2& pos={0,0});
};

class GUI_Label:public GUI_Element{
	Text text;
	const Snap snap;
public:
	GUI_Label(const string& t, Snap s=SNAP_NONE, const Vec2& pos={0,0});
	
	virtual void Update();
	virtual void Render();
};

class GUI_Array:public GUI_Element{
protected:
	vector<GUI_Element*> array;

	GUI_Array(vector<GUI_Element*>& v, const Vec2& pos);
	~GUI_Array();
public:
	GUI_Array(vector<GUI_Element*>& v);

	virtual void Update();
	virtual void Render();
};

class GUI_HBar:public GUI_Array{
public:
	GUI_HBar(vector<GUI_Element*>& v, uint width=0, const Vec2& pos={0,0});

	virtual void Render();
};

class GUI_VBar:public GUI_Array{
public:
	GUI_VBar(vector<GUI_Element*>& v, uint height=0, const Vec2& pos={0,0});
	
	virtual void Render();
};

class GUI_Window:public GUI_Element{
	GUI_VBar array;
	Text label;
	GUI_IconButton closeButton;
public:
	uint const id;
	
	GUI_Window(vector<GUI_Element*>& v, uint i, const string& l, const Vec2& pos={-1,-1});
	
	virtual void Update();
	virtual void Render();
};
#endif //GUIHPP
