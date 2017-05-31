#ifndef COMPONENTHPHPP
#define COMPONENTHPHPP

#include <common.hpp>
#include <component.hpp>
#include <timer.hpp>

class CompHP : public Component{
	//private members
public:
	//public members
	int total,current;
	bool showHP,showDMG;
	float cooldown;
	Timer dmgCoolDown;


	CompHP(int tot=100,bool showHP=false,bool showDMG=false,float dmgCD=0.0f);
	CompHP(int tot,int cur,bool showHP=false,bool showDMG=false,float dmgCD=0.0f);
	~CompHP();

	void Damage(int dmg);

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPONENTHPHPP
