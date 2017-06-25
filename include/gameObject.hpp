#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>

class GameObject{
public:
	const uint uid;

	bitset<Component::type::t_count> hasComponent;
	array<Component*,Component::type::t_count> components;

	vector<GameObject*> attachedObjs;
	GameObject* attachedTo=nullptr;

	enum Team{neutral,player,enemy,other};
	Team team=Team::neutral;

	Vec2 pos;
	Vec2 curPos;
	Vec2 size;
	Vec2 curSize=Vec2{1.0f,1.0f};
	float rotation;
	Hotspot hotspot;

	bool anchored=false;
	bool dead=false;
	bool flipped=false;

	GameObject();
	GameObject(const Vec2 &pos_,float r=0.0f,Hotspot h=Hotspot::TOP_LEFT, bool a=false);
	GameObject(const Rect &rect,float r=0.0f,Hotspot h=Hotspot::TOP_LEFT, bool a=false);
	~GameObject();

	void Update(float time);
	void Render();
	void AddComponent(Component* component);
	void ReplaceComponent(Component* component);
	void RemoveComponent(Component::type t);
	void SetComponent(Component::type t,Component* component);//override current component, doesnt dealloc

	void AttachObj(GameObject* obj);
	void AttachTo(GameObject* obj);
	void UnAttachObj(GameObject* obj);
	void UnAttach();

	bool IsDead()const;
	Rect Box()const;
	Rect FullBox()const;


	static GameObject* MakePlayer(const Vec2 &pos);
	static GameObject* MakeTarget(const Vec2 &pos);
	static GameObject* MakeMike(const Vec2 &pos);
	static GameObject* MakeBanshee(const Vec2 &pos,const Vec2 &pos2);
	static GameObject* MakeMask(const Vec2 &pos);
	static GameObject* MakePorco(const Vec2 &pos);
	//static GameObject* Make...(const Vec2 &pos);
};



#endif//GAMEOBJECTHPP
