#ifndef STATEEDITORHPP
#define STATEEDITORHPP

#include <common.hpp>

#include <level.hpp>
#include <state.hpp>
#include <text.hpp>

class StateEditor: public State{
	Level level;
	
	int tileIndex=0;
	bool showGrid=true;
	bool showHelp=true;
	bool showCollision=false;
	Text helpText;
	Text statusText;

	Vec2 clickPos;
	Vec2 camPos;

	vector<pair<ii,ii>> grouped;
public:
	StateEditor();
	~StateEditor();

	virtual void Update(float time);
	virtual void Render();

	virtual void Begin();
	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
	
	void RenderBackground();
	void RenderGrid(int w, int h);
	void RenderBorder();
	void RenderCursor();
	void RenderCollision();
	
	Vec2 GetCurrentTile();

	void RecomputeCollisionRectangles();
};

#endif//STATEEDITORHPP
