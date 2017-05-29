#ifndef COMPSTATICRENDERHPP
#define COMPSTATICRENDERHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>
#include <sprite.hpp>

class CompStaticRender : public Component{
	//private members
public:
	//public members
	Sprite sp;
	Vec2 pos;
	bool camScaling;

	CompStaticRender(const Sprite &sp,const Vec2 &p, const bool cs = true);
	~CompStaticRender();

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType()const;
};

#endif//COMPSTATICRENDERHPP
