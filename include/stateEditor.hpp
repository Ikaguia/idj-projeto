#ifndef STATEEDITORHPP
#define STATEEDITORHPP

#include <common.hpp>

#include <music.hpp>
#include <sprite.hpp>
#include <state.hpp>
#include <tileSet.hpp>
#include <tileMap.hpp>


class StateEditor: public State{
	Sprite bg;

	TileSet tileSet;
	TileMap tileMap;

	GameObject* player;
public:
	StateStage(string fileTSet,string fileTMap,string fileBG);
	~StateStage();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
};

#endif//STATEEDITORHPP
