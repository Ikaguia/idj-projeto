#ifndef ENDSTATEHPP
#define ENDSTATEHPP

#include <common.hpp>
#include <state.hpp>
#include <sprite.hpp>
#include <music.hpp>
#include <text.hpp>
#include <stateData.hpp>

class EndState : public State{
	Sprite sp;
	Music music;
	Text text;
public:
	EndState(StateData stateData);
	virtual ~EndState();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//ENDSTATEHPP
