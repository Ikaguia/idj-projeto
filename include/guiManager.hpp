#ifndef GUIMANAGERHPP
#define GUIMANAGERHPP

#include <common.hpp>

#include <geometry.hpp>

#define GUI InputManager::GetInstance()

class GUIManager{
	bool buttonState;
	int updateCounter;

	bool quitRequested;

	GUIManager();
	~GUIManager();
public:
	void Update();

	bool ButtonPress(int key);
	bool ButtonRelease(int key);
	bool IsButtonDown(int key);

	static GUIManager& GetInstance();
};

#endif//GUIMANAGERHPP
