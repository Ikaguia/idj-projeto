#ifndef STATEHPP
#define STATEHPP

#include <common.hpp>

#include <gameObject.hpp>
#include <guiManager.hpp>
#include <sound.hpp>

class State{
public:
	class Layer;
protected:

	bool popRequested=false;
	bool quitRequested=false;
	uint lastGO=-1;

	virtual void UpdateArray(float time);
	virtual void RenderArray();
public:
	GUIManager gui;
	
	vector<Layer> layerList;
	map<string,set<uint>> objectLayer;
	map<ii,set<uint>> group;
	set<uint> entities_;

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

	virtual void AddObject(uint uid, const string& layer="MAIN", int area=0);
	virtual void ClearObjects();
	GameObject* GetLastObject();

	void AddSound(string file,int times);

	virtual bool PopRequested();
	virtual bool QuitRequested();
	
	set<uint> GetEntitiesInRange(const float &x1,const float &x2);
};

class State::Layer{
public:
	string name;
	const char type;
	float parallax;
	uint tileMapLayer=0;
	bool visible=true;
	
	Layer(const string& n, char t, const float& p);
};

#endif//STATEHPP
