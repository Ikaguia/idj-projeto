#ifndef STATEHPP
#define STATEHPP

#include <common.hpp>
#include <gameObject.hpp>
#include <sound.hpp>

class State{
protected:
	bool popRequested;
	bool quitRequested;
	vector<unique_ptr<GameObject>> objectArray;

	vector<unique_ptr<Sound>> sounds;

	virtual void UpdateArray(float time);
	virtual void RenderArray();
public:
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
