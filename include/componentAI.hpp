#ifndef COMPONENTAIHPP
#define COMPONENTAIHPP

#include <common.hpp>
#include <component.hpp>

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

	CompAI(aiFunc func);
	~CompAI();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPONENTAIHPP
