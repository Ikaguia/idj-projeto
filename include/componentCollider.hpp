#ifndef COMPONENTCOLLIDERHPP
#define COMPONENTCOLLIDERHPP

#include <common.hpp>

#include <component.hpp>
#include <tileMap.hpp>

class CompCollider;

using colliderFunc=std::function<void(const CompCollider *a,const CompCollider *b)>;

class CompCollider : public Component{
public:
	//static members
	static set<CompCollider*> colliders;
	//members
	enum collType{t_any,t_ground,t_h_ground,t_player,t_monster,t_bullet,t_solid,t_count};
	collType cType;
	map<collType,colliderFunc> useDefault;//use custom collision handler against colliders of _type
	//constructor/destructor
	CompCollider(collType t);
	~CompCollider();
	//functions
	void collisionCheck(CompCollider *other);
	Vec2 collides(const CompCollider *other,const Vec2 &move) const;
	Vec2 collides(const CompCollider *other,const Vec2 &move,const Rect &box) const;

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType() const;
};

#endif//COMPONENTCOLLIDERHPP
