#ifndef COMPONENTAIHPP
#define COMPONENTAIHPP

#include <common.hpp>

#include <component.hpp>
#include <timer.hpp>
#include <geometry.hpp>

class CompAI;
using aiFunc=function<void(CompAI*,float)>;

#define COMPAI(x)  ((CompAI*)x. components[Component::type::t_ai])
#define COMPAIp(x) ((CompAI*)x->components[Component::type::t_ai])

class CompAI : public Component{
	//private members
public:
	//public members
	enum state{idling,looking,walking,attacking,count};
	aiFunc foo;

	vector<int> states;
	vector<Timer> timers;
	vector<Vec2> targetPOS;
	vector<GameObject*> targetGO;

	CompAI(aiFunc func,int stCounts=0,int timCount=0,int posCount=0,int goCount=0);
	~CompAI();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPONENTAIHPP
