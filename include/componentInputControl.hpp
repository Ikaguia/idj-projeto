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

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType()const;
};

#endif//COMPONENTINPUTCONTROLHPP
