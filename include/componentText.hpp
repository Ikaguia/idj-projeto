#ifndef COMPONENTTEXTHPP
#define COMPONENTTEXTHPP

#include <common.hpp>
#include <component.hpp>
#include <geometry.hpp>
#include <text.hpp>

class CompText : public Component{
	Text txt;
	Vec2 pos;
public:
	//public members
	CompText(const Text &t,Vec2 p=Vec2{});
	CompText(string text,int size,SDL_Color c=SDL_COLOR_WHITE,Vec2 p=Vec2{});
	~CompText();

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType()const;
};

#endif//COMPONENTTEXTHPP
