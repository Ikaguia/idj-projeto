#ifndef COMPONENTMOVEMENTHPP
#define COMPONENTMOVEMENTHPP

#include <common.hpp>
#include <component.hpp>
#include <geometry.hpp>

class CompMovement : public Component{
	//private members
public:
	//public members
	Vec2 move;

	CompMovement();
	~CompMovement();

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType()const;
};

#endif//COMPONENTMOVEMENTHPP
