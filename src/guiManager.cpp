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
		}
		elements.emplace_back(unique_ptr<GUI_Element>(storedElement));
		storedElement = nullptr;
	}

	if(elements.empty()) return;
	
	popRequested = false;
	
	previousButtonState = currentButtonState;
	if(selectedButton){
		GUI_Button* selectedButtonCopy = selectedButton;
		selectedButton = nullptr;
		selectedButtonCopy->Update();
		selectedButton = selectedButtonCopy;
		currentButtonState = selectedButton->IsPressed();
	}
	
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
int GUIManager::GetSelectedWindowID()const{
	if(selectedWindow) return selectedWindow->id;
	else if(elements.size() == 1) return 0;
	else return -1;
}

void GUIManager::SelectButton(GUI_Button* button){
	selectedButton = button;
}
bool GUIManager::IsButtonSelected(GUI_Button* button)const{
	return button && button==selectedButton;
}

bool GUIManager::ButtonPress(uint action)const{
	if(!selectedButton) return false;
	if(action && selectedButton->action != action) return false;
	return (!previousButtonState && currentButtonState);
}
bool GUIManager::ButtonRelease(uint action)const{
	if(!selectedButton) return false;
	if(action && selectedButton->action != action) return false;
	return (previousButtonState && !currentButtonState);
}
bool GUIManager::ButtonClick(uint action)const{
	if(!selectedButton) return false;
	if(action && selectedButton->action != action) return false;
	return (previousButtonState && !currentButtonState && selectedButton->IsHovered());
}
bool GUIManager::IsButtonDown(uint action)const{
	if(!selectedButton) return false;
	if(action && selectedButton->action != action) return false;
	return currentButtonState;
}
