#ifndef STATEEDITORHPP
#define STATEEDITORHPP

#include <common.hpp>

#include <gui.hpp>
#include <level.hpp>
#include <state.hpp>
#include <text.hpp>

class StateEditor: public State{
	Level level;
	string fileName="level_0";
	int levelWidth=0;
	int levelHeight=0;
	
	uint window=0;
	bool closeFlag=false;
	
	int tileIndex=0;
	int gridWidth=64;
	int gridHeight=64;
	
	Vec2 clickPos;
	Vec2 camPos;
	bool showGrid=true;
	bool showHelp=true;
	bool showCollision=false;
	Text helpText;
	Text statusText;

	vector<pair<ii,ii>> grouped;
	
	void Exit();
	
	void RecomputeCollisionRectangles();
	Vec2 GetCurrentTile();
	
	void SaveLevel();
	void ResizeLevel();
	
	void CreateWindow(uint type);
	
	void RenderBackground();
	void RenderGrid(int w, int h);
	void RenderBorder();
	void RenderCursor();
	void RenderCollision();
public:
	StateEditor();
	~StateEditor();

	virtual void Update(float time);
	virtual void Render();

	virtual void Begin();
	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
	virtual void LoadGUI();
	
	bool PopRequested();
	bool QuitRequested();
};

#endif//STATEEDITORHPP
