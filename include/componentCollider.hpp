#ifndef COMPONENTCOLLIDERHPP
#define COMPONENTCOLLIDERHPP

#include <common.hpp>

#include <component.hpp>
#include <tileMap.hpp>

class CompCollider;

using colliderFunc=std::function<void(const CompCollider *a,const CompCollider *b)>;

#define COMPCOLLIDER(x)  ((CompCollider*)x. components[Component::type::t_collider])
#define COMPCOLLIDERp(x) ((CompCollider*)x->components[Component::type::t_collider])

class CompCollider : public Component{
	Vec2 pos;
	Vec2 size;
public:
	//static members
	static set<CompCollider*> colliders;
	//members
	enum collType{t_any,t_ground,t_h_ground,t_player,t_monster,t_bullet,t_solid,t_count};
	const collType cType;
	map<collType,colliderFunc> useDefault;//use custom collision handler against colliders of _type
	bool active=true;

	//constructor/destructor
	CompCollider(collType t);
	CompCollider(const Rect &box,collType t);
	//functions
	void collisionCheck(CompCollider *other);
	Vec2 collides(const CompCollider *other,const Vec2 &move) const;
	Vec2 collides(const CompCollider *other,const Vec2 &move,const Rect &box) const;
	void Activate();
	void Deactivate();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType() const;
};

#endif//COMPONENTCOLLIDERHPP
