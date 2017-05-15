#ifndef COMPONENTHPP
#define COMPONENTHPP

#include <common.hpp>

class Component{
public:
	GameObject* entity;
	enum type{t_collider,t_sprite,t_count};

	Component(GameObject* go);
	~Component()=0;

	void Update(float time)=0;
	type GetType()const=0;
};

#endif//COMPONENTHPP
