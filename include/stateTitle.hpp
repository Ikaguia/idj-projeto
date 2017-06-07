#ifndef TITLESTATEHPP
#define TITLESTATEHPP

#include <common.hpp>

#include <gui.hpp>
#include <sprite.hpp>
#include <state.hpp>
#include <text.hpp>

class StateTitle: public State{
	Sprite bg;
	GUI_CheckButton b;
public:

	StateTitle();
	~StateTitle();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//TITLESTATEHPP
