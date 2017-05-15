#ifndef COLLIDERHPP
#define COLLIDERHPP

#include <common.hpp>
#include <tileMap.hpp>

class GameObject;

class Collider{
	//private members
	map<size_t,bool> useDefault;//use default collider check against colliders of type size_t
	size_t type;

	static bool collidersOrder(const Collider* a,const Collider* b);
public:
	//public members
	static set<Collider*,decltype<collidersOrder>> colliders;
	GameObject* entity;

	Collider();
	~Collider();

	void collisionCheck(Collider &other);
	void collisionCheck(const TileMap &tileMap);
	Vec2 collides(const Collider &other);
	Vec2 collides(const TileMap &tileMap);

	size_t getType();
};

#endif//COLLIDERHPP
