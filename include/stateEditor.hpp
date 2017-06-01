#ifndef STATEEDITORHPP
#define STATEEDITORHPP

#include <common.hpp>

#include <level.hpp>
#include <state.hpp>
#include <text.hpp>

class StateEditor: public State{
	Level level;
	
	int tileIndex;
	bool showGrid;
	bool showHelp;
	bool showCollision;
	Text helpText;
	Text statusText;

	vector<pair<ii,ii>> grouped;
public:
	StateEditor();
	~StateEditor();

	virtual void Update(float time);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void LoadAssets();
	
	void RenderBackground();
	void RenderGrid(int x, int y, int w, int h);
	void RenderBorder();
	void RenderCursor();
	void RenderCollision();
	
	Vec2 GetCurrentTile();

	void RecomputeCollisionRectangles();
};

#endif//STATEEDITORHPP
