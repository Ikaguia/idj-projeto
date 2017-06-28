#ifndef COMPONENTMOVEMENTHPP
#define COMPONENTMOVEMENTHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>

#define COMPMOVE(x)  ((CompMovement*)x. components[Component::type::t_movement])
#define COMPMOVEp(x) ((CompMovement*)x->components[Component::type::t_movement])

class CompMovement : public Component{
	//private members
public:
	//public members
	enum moveType{t_horizontal,t_bullet,t_count};

	moveType mType;

	Vec2 speed;
	Vec2 move;

	CompMovement(const Vec2& s=Vec2{},moveType t=t_horizontal);
	~CompMovement();

	void Update(float time);
	void Render();
	Component::type GetType()const;
};

#endif//COMPONENTMOVEMENTHPP
