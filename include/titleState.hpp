#ifndef TITLESTATEHPP
#define TITLESTATEHPP

#include <common.hpp>
#include <state.hpp>
#include <sprite.hpp>
#include <text.hpp>

class TitleState: public State{
	Sprite bg;
	Text text;
public:
	TitleState();
	~TitleState();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//TITLESTATEHPP
