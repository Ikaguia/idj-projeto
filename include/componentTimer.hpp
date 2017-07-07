#ifndef COMPTIMERHPP
#define COMPTIMERHPP

#include <common.hpp>
#include <component.hpp>
#include <timer.hpp>

#define COMPTIMER(x)  ((CompTimer*)x. components[Component::type::t_timer])
#define COMPTIMERp(x) ((CompTimer*)x->components[Component::type::t_timer])

class CompTimer : public Component{
	//private members
	Timer t;
	float limit;
public:
	//public members

	CompTimer(float l);
	~CompTimer();
	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPTIMERHPP
