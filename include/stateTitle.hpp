#ifndef TITLESTATEHPP
#define TITLESTATEHPP

#include <common.hpp>

#include <sprite.hpp>
#include <state.hpp>
#include <text.hpp>

class StateTitle: public State{
	Sprite bg;
	Sprite bt1;
	Sprite bt2;
public:

	StateTitle();
	~StateTitle();

	virtual void Update(float time);
	virtual void Render();

	virtual void Begin();
	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
	virtual void LoadGUI();
};

#endif//TITLESTATEHPP
