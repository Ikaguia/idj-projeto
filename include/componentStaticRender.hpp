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

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPSTATICRENDERHPP
