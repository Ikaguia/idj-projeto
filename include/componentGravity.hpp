#ifndef COMPGRAVITYHPP
#define COMPGRAVITYHPP

#include <common.hpp>
#include <component.hpp>

class CompGravity : public Component{
	//private members
	float gravity;
public:
	//public members
	CompGravity(float g=GRAVITY);
	~CompGravity();

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType() const;
};

#endif//COMPGRAVITYHPP
