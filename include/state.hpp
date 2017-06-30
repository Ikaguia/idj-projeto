#ifndef STATEHPP
#define STATEHPP

#include <common.hpp>

#include <gameObject.hpp>
#include <guiManager.hpp>
#include <sound.hpp>


#define ENTITY(x) (GAMESTATE.entities[x])

class State{
protected:

	bool popRequested=false;
	bool quitRequested=false;
	uint uid=0;

	virtual void UpdateArray(float time);
	virtual void RenderArray();
public:
	GUIManager gui;
	
	bool ending=false;

	map<uint, unique_ptr<GameObject>> entities;
	map<ii,set<uint>> group;

	State();
	virtual ~State(){}

	virtual void Update(float time)=0;
	virtual void Render()=0;

	virtual void Begin()=0;
	virtual void End();
	virtual void Pause()=0;
	virtual void Resume()=0;

	virtual void LoadAssets()=0;
	virtual void LoadGUI()=0;

	virtual void AddObject(GameObject* obj, int layer=0, int area=0);
	GameObject* GetLastObject();

	void AddSound(string file,int times);

	virtual bool PopRequested();
	virtual bool QuitRequested();
	
	set<uint> GetEntitiesInRange(const float &x1,const float &x2);

	uint GetUID();
};

#endif//STATEHPP
