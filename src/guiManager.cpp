#include <guiManager.hpp>

#include <gui.hpp>
#include <inputManager.hpp>

GUIManager::GUIManager(){}
GUIManager::~GUIManager(){
	elements.clear();
}

void GUIManager::Update(){
	if(popRequested){
		selectedWindow = nullptr;
		selectedButton = nullptr;
		elements.pop_back();
	}
	if(storedElement){
		selectedWindow = nullptr;
		if(selectedButton){
			selectedButton->Reset();
			selectedButton = nullptr;
			selectedWindow = nullptr; 
		}
		elements.emplace_back(unique_ptr<GUI_Element>(storedElement));
		storedElement = nullptr;
	}

	if(elements.empty()) return;
	
	popRequested = false;
	buttonUpdate = false;
	if(selectedButton){
		GUI_Button* selectedButtonCopy = selectedButton;
		selectedButton = nullptr;
		selectedButtonCopy->Update();
		selectedButton = selectedButtonCopy;
		if(selectedButton->IsPressed()) return;
	}
	if(selectedButton != elements.back().get())
		elements.back()->Update();	
}
void GUIManager::Render(){
	for(auto& it:elements)
		if(it->IsVisible())
			it->Render();
}

void GUIManager::PushElement(GUI_Element* element){
	if(storedElement) delete storedElement;
	storedElement = element;
}
void GUIManager::RequestPop(GUI_Element* element){
	if(element == elements.back().get())
		popRequested = true;
}

void GUIManager::SelectWindow(GUI_Window* window){
	selectedWindow = window;
}
bool GUIManager::IsWindowSelected(GUI_Window* window)const{
	return window==selectedWindow;
}
uint GUIManager::GetSelectedWindowID()const{
	if(selectedWindow) return selectedWindow->id;
	else if(elements.size() == 1) return 0;
	else return (uint)-1;
}

void GUIManager::SelectButton(GUI_Button* button){
	selectedButton = button;
}
bool GUIManager::IsButtonSelected(GUI_Button* button)const{
	return button && button==selectedButton;
}
void GUIManager::FlagButtonUpdate(GUI_Button* button){
	if(button!=nullptr && button==selectedButton)
		buttonUpdate = true;
}
bool GUIManager::ButtonPress(uint action)const{
	if(!selectedButton) return false;
	if(selectedButton->action != action) return false;
	return (buttonUpdate && selectedButton->IsPressed());
}
bool GUIManager::ButtonRelease(uint action)const{
	if(!selectedButton) return false;
	if(selectedButton->action != action) return false;
	return (buttonUpdate && !selectedButton->IsPressed());
}
bool GUIManager::ButtonClick(uint action)const{
	if(!selectedButton) return false;
	if(selectedButton->action != action) return false;
	return (buttonUpdate && !selectedButton->IsPressed() && selectedButton->IsHovered());
}
bool GUIManager::IsButtonDown(uint action)const{
	if(!selectedButton) return false;
	if(selectedButton->action != action) return false;
	return selectedButton->IsPressed();
}
