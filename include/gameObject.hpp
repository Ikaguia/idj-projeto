#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>

#define GO(uid) ((GameObject*)(GameObject::entities[(uint)uid].get()))
#define isGO(uid) (GameObject::entities.count((uint)uid))

class GameObject{
public:
	static uint goCount;
	static map<uint, unique_ptr<GameObject>> entities;

	const uint uid;
	
	string type=""; 

	bitset<Component::type::t_count> hasComponent;
	array<Component*,Component::type::t_count> components;

	function<void(GameObject*)> onDeath;

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
	bool remove=false;
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
	bool Remove()const;

	Rect Box()const;
	Rect Box(const Vec2& p,const Vec2 &sz)const;
	Rect FullBox()const;
	
	static uint Create(const string&, const Vec2& pos, const Vec2& aux=Vec2{});
	static uint MakePlayer(const Vec2 &pos);
	static uint MakeTarget(const Vec2 &pos);
	static uint MakeMike(const Vec2 &pos);
	static uint MakeBanshee(const Vec2 &pos,const Vec2 &pos2);
	static uint MakeMask(const Vec2 &pos);
	static uint MakePorco(const Vec2 &pos);
	//static uint Make...(const Vec2 &pos);
};



#endif//GAMEOBJECTHPP
