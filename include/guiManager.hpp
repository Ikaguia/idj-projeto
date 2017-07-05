#ifndef GUIMANAGERHPP
#define GUIMANAGERHPP

#include <common.hpp>

#include <geometry.hpp>

class GUI_Element;
class GUI_Button;
class GUI_Window;

class GUIManager{
	vector<unique_ptr<GUI_Element>> elements;
	GUI_Element* storedElement=nullptr;
	bool popRequested=false;
	
	bool mouseHover = false;
	
	vector<GUI_Window*> windows;
	GUI_Window* selectedWindow=nullptr;
	
	GUI_Button* selectedButton=nullptr;
	bool currentButtonState=false;
	bool previousButtonState=false;
public:
	GUIManager();
	~GUIManager();
	
	void Update();
	void Render();
	
	void PushElement(GUI_Element* element);
	void RequestPop(GUI_Element* element);
	
	void SelectWindow(GUI_Window* window);
	bool IsWindowSelected(GUI_Window* window)const;
	int GetSelectedWindowID()const;

	void SelectButton(GUI_Button* button);
	bool IsButtonSelected(GUI_Button* button)const;
											//These functions return true if a button with the same action:
	bool ButtonPress(uint action=0)const;	//	-was pressed in the current frame;
	bool ButtonRelease(uint action=0)const;	//	-was released in the current frame
	bool ButtonClick(uint action=0)const;	//	-was released with the cursor over it in the current frame
	bool IsButtonDown(uint action=0)const;	//	-is being pressed
};

#endif//GUIMANAGERHPP
