#ifndef COLLIDERHPP
#define COLLIDERHPP

#include <functional>

#include <common.hpp>
#include <component.hpp>
#include <tileMap.hpp>

class Collider;

using colliderFunc=std::function<void(Collider* a,Collider* b)>;
struct collidersOrder{
	bool operator() (const Collider* a,const Collider* b);
};
struct collidersOrderEnd{
	bool operator() (const Collider* a,const Collider* b);
};

class Collider : public Component{
public:
	//static members
	static set<Collider*,collidersOrder> colliders;
	static set<Collider*,collidersOrderEnd> collidersEnd;
	//static functions
	static set<Collider*> GetCollidersInRange(float x1,float x2);
	//members
	enum collType{t_ground,t_h_ground,t_player,t_bullet,t_count};
	collType cType;
	map<collType,colliderFunc> useDefault;//use custom collision handler against colliders of _type
	//constructor/destructor
	Collider(GameObject* go);
	~Collider();
	//functions
	void collisionCheck(Collider *other);
	void collisionCheck(const TileMap &tileMap);
	Vec2 collides(const Collider *other,const Vec2 &move);
	Vec2 collides(const TileMap &tileMap,const Vec2 &move);
	Component::type GetType();
};

#endif//COLLIDERHPP
