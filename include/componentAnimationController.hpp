#ifndef COMPANIMATIONCONTROLLERHPP
#define COMPANIMATIONCONTROLLERHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>
#include <componentAnimation.hpp>

#define COMPANIMCONT(x)  ((CompAnimControl*)x. components[Component::type::t_animation_control])
#define COMPANIMCONTp(x) ((CompAnimControl*)x->components[Component::type::t_animation_control])

class Collider;

class CompAnimControl : public Component{
	//private members
	map<string,unique_ptr<CompAnim>> animations;
	string cur;
	string prev;
	bool repeat=true;
public:
	//public members
	CompAnimControl(string file,CompCollider* coll);
	~CompAnimControl();

	void ChangeCur(string anim,bool rep=true);
	CompAnim& GetCur();
	const string& GetCurName()const;

	void Update(float time);
	void Own(GameObject *go);
	void Render();
	Component::type GetType()const;
};

#endif//COMPANIMATIONCONTROLLERHPP
