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

	void Damage(int dmg);

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPONENTHPHPP
