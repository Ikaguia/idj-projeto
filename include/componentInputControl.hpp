#ifndef COMPONENTINPUTCONTROLHPP
#define COMPONENTINPUTCONTROLHPP

#include <common.hpp>

#include <component.hpp>

class GameObject;

class CompInputControl : public Component{
	function<void(GameObject*,float)> func;
public:
	CompInputControl(function<void(GameObject*,float)> f);
	~CompInputControl();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPONENTINPUTCONTROLHPP
