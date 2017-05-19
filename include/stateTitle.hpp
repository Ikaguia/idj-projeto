#ifndef TITLESTATEHPP
#define TITLESTATEHPP

#include <common.hpp>
#include <state.hpp>
#include <sprite.hpp>
#include <text.hpp>

class StateTitle: public State{
	Sprite bg;
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
