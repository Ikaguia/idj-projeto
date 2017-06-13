#ifndef STATEHPP
#define STATEHPP

#include <common.hpp>

#include <gameObject.hpp>
#include <sound.hpp>

class State{
protected:
	bool popRequested=false;
	bool quitRequested=false;
	uint uid=0;

	virtual void UpdateArray(float time);
	virtual void RenderArray();
public:
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

	virtual void AddObject(GameObject* obj, int layer=0, int area=0);

	void AddSound(string file,int times);

	bool PopRequested();
	bool QuitRequested();
	
	set<GameObject*> GetEntitiesInRange(const float &x1,const float &x2);

	uint GetUID();
};

#endif//STATEHPP
