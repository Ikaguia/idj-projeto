#ifndef COMPONENTCOLLIDERHPP
#define COMPONENTCOLLIDERHPP

#include <functional>

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
	enum collType{t_ground,t_h_ground,t_player,t_bullet,t_count};
	collType cType;
	map<collType,colliderFunc> useDefault;//use custom collision handler against colliders of _type
	//constructor/destructor
	CompCollider(collType t);
	~CompCollider();
	//functions
	void collisionCheck(CompCollider *other);
	Vec2 collides(const CompCollider *other,const Vec2 &move) const;

	virtual void Update(float time);
	virtual void Render();
	virtual Component::type GetType() const;
};

#endif//COMPONENTCOLLIDERHPP
