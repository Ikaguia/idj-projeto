#ifndef COMPONENTHPP
#define COMPONENTHPP

#include <common.hpp>

class GameObject;

class Component{
public:
	GameObject* entity;
	enum type{
		t_input_control,
		t_ai,

		t__,

		t_animation,
		t_animation_control,
		t_collider,
		t_text,
		//t_click,
		t_static_render,
		//t_animated_render,
		//t_particle_emmiter,
		t_hp,
		t_movement,
		t_gravity,
		t_count,
		t_memory,
	};

	Component(){};
	virtual ~Component(){};

	virtual void Update(float time)=0;
	virtual void Render()=0;
	virtual void Own(GameObject* go)=0;
	virtual type GetType()const=0;
};

#endif//COMPONENTHPP
