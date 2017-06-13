#ifndef COMPANIMATIONCONTROLLERHPP
#define COMPANIMATIONCONTROLLERHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>
#include <componentAnimation.hpp>

#define COMPANIMCONT(x)  ((CompAnimControl*)x. components[Component::type::t_animation_control])
#define COMPANIMCONTp(x) ((CompAnimControl*)x->components[Component::type::t_animation_control])

class CompAnimControl : public Component{
	//private members
	map<string,CompAnim> animations;
	string cur;
public:
	//public members
	CompAnimControl(string file);
	~CompAnimControl();

	void ChangeCur(string anim);
	CompAnim& GetCur();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPANIMATIONCONTROLLERHPP
