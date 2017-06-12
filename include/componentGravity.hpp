#ifndef COMPGRAVITYHPP
#define COMPGRAVITYHPP

#include <common.hpp>
#include <component.hpp>

#define COMPGRAVITY(x)  ((CompGravity*)x. components[Component::type::t_gravity])
#define COMPGRAVITYp(x) ((CompGravity*)x->components[Component::type::t_gravity])

class CompGravity : public Component{
	//private members
	float gravity;
public:
	//public members
	CompGravity(float g=GRAVITY);
	~CompGravity();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType() const;
};

#endif//COMPGRAVITYHPP
