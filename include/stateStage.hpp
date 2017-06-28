#ifndef STATESTAGEHPP
#define STATESTAGEHPP

#include <common.hpp>

#include <level.hpp>
#include <music.hpp>
#include <state.hpp>
#include <game.hpp>


#define STATESTAGE ((StateStage*)&GAMESTATE)
#define PLAYER_UID (STATESTAGE->player)
#define PLAYER (GAMESTATE.entities[PLAYER_UID].get())

class StateStage: public State{
	string levelName;
	Level level;
public:
	uint player;

	StateStage(string lvl);
	~StateStage();

	virtual void Update(float time);
	virtual void Render();

	virtual void Begin();
	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
	virtual void LoadGUI();
};

#endif//STATESTAGEHPP
