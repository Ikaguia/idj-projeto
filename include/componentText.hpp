#ifndef COMPONENTTEXTHPP
#define COMPONENTTEXTHPP

#include <common.hpp>
#include <component.hpp>
#include <geometry.hpp>
#include <text.hpp>

#define COMPTEXT(x)  ((CompText*)x. components[Component::type::t_text])
#define COMPTEXTp(x) ((CompText*)x->components[Component::type::t_text])

class CompText : public Component{
	Text txt;
	Vec2 pos;
public:
	//public members
	CompText(const Text &t, Hotspot h=Hotspot::TOP_LEFT, Vec2 p=Vec2{});
	CompText(string text,int size,SDL_Color c=SDL_COLOR_WHITE, Hotspot h=Hotspot::TOP_LEFT, Vec2 p=Vec2{});
	~CompText();

	void Update(float time);
	void Render();
	Component::type GetType()const;
};

#endif//COMPONENTTEXTHPP
