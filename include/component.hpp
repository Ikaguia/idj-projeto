#ifndef COMPONENTHPP
#define COMPONENTHPP

#include <common.hpp>

class GameObject;

class Component{
public:
	GameObject* entity;
	enum type{t_collider,t_sprite,t_static_render,t_animated_render,t_movement,t_count};

	Component(GameObject* go);
	virtual ~Component(){};

	virtual void Update(float time)=0;
	virtual type GetType()const=0;
};

#endif//COMPONENTHPP
