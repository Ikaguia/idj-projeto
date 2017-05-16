#ifndef COMPONENTHPP
#define COMPONENTHPP

#include <common.hpp>

class GameObject;

class Component{
public:
	GameObject* entity;
	enum type{
		t_collider,
		t_text,
		//t_click,
		//t_static_render,
		//t_animated_render,
		//t_movement,
		//t_gravity,
		//t_input_control,
		//t_ai,
		//t_timer,
		//t_life,
		t_count,
	};

	Component(){};
	virtual ~Component(){};

	virtual void Update(float time)=0;
	virtual void Render()=0;
	virtual type GetType()const=0;
};

#endif//COMPONENTHPP
