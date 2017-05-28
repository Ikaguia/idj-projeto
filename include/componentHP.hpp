#ifndef COMPONENTHPHPP
#define COMPONENTHPHPP

#include <common.hpp>
#include <component.hpp>

class CompHP : public Component{
	//private members
public:
	//public members
	int total,current;

	CompHP(int tot=100);
	CompHP(int tot,int cur);
	~CompHP();

	void Update(float time);
	void Render();
	Component::type GetType()const;
};

#endif//COMPONENTHPHPP
