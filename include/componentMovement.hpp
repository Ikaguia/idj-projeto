#ifndef COMPONENTMOVEMENTHPP
#define COMPONENTMOVEMENTHPP

#include <common.hpp>
#include <component.hpp>
#include <geometry.hpp>

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

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType()const;
};

#endif//COMPONENTMOVEMENTHPP
