#ifndef STATEHPP
#define STATEHPP

#include <common.hpp>
#include <gameObject.hpp>
#include <sound.hpp>

class State{
protected:
	bool popRequested;
	bool quitRequested;

	virtual void UpdateArray(float time);
	virtual void RenderArray();
public:
	set<unique_ptr<GameObject>> entities;
	set<GameObject*> GetEntitiesInRange(const float &x1,const float &x2);


	State();
	virtual ~State(){}

	virtual void Update(float time)=0;
	virtual void Render()=0;

	virtual void Pause()=0;
	virtual void Resume()=0;

	virtual void LoadAssets()=0;

	virtual void AddObject(GameObject* obj);

	void AddSound(string file,int times);

	bool PopRequested();
	bool QuitRequested();
};

#endif//STATEHPP
