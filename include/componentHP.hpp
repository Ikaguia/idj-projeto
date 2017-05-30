#ifndef COMPONENTHPHPP
#define COMPONENTHPHPP

#include <common.hpp>
#include <component.hpp>

class CompHP : public Component{
	//private members
public:
	//public members
	int total,current;
	bool showHP,showDMG;

	CompHP(int tot=100,bool showHP=false,bool showDMG=false);
	CompHP(int tot,int cur,bool showHP=false,bool showDMG=false);
	~CompHP();

	void Update(float time);
	void Render();
	void Damage(int dmg);
	Component::type GetType()const;
};

#endif//COMPONENTHPHPP
