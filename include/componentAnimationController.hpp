#ifndef COMPANIMATIONCONTROLLERHPP
#define COMPANIMATIONCONTROLLERHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>
#include <componentAnimation.hpp>

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
