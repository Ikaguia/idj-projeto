#include <guiManager.hpp>

GUIManager::GUIManager():updateCounter{0}{}
GUIManager::~GUIManager(){}

void GUIManager::Update(){
}

bool GUIManager::ButtonPress(int key){
	return (buttonState && updateCounter==0);
}
bool GUIManager::ButtonRelease(int key){
	return ((!buttonState) && updateCounter==0);
}
bool GUIManager::IsButtonDown(int key){
	return buttonState;
}

GUIManager& GUIManager::GetInstance(){
	static GUIManager uniqueInst;
	return uniqueInst;
}
