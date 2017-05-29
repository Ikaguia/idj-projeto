#ifndef STATESTAGEHPP
#define STATESTAGEHPP

#include <common.hpp>

#include <level.hpp>
#include <music.hpp>
#include <state.hpp>


class StateStage: public State{
	Level level;

	GameObject* player;
public:
	StateStage(string lvl);
	~StateStage();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//STATESTAGEHPP
