#ifndef COLLIDERHPP
#define COLLIDERHPP

#include <functional>

#include <common.hpp>
#include <component.hpp>

using colliderFunc=std::function<void(Collider* a,Collider* b)>;

class Collider : public Component{
	map<_type,colliderFunc> useDefault;//use default collision check against colliders of type size_t

	static bool collidersOrder(const Collider* a,const Collider* b);
	static bool collidersOrderEnd(const Collider* a,const Collider* b);
public:
	static set<Collider*,decltype(collidersOrder)> colliders;
	static set<Collider*,decltype(collidersOrderEnd)> collidersEnd;

	enum _type{t_player,t_bullet,t_count};
	_type type;
	ConvexPolygon pol;

	Collider();
	~Collider();

	void collisionCheck(Collider &other);
	void collisionCheck(const TileMap &tileMap);
	Vec2 collides(const Collider &other);
	Vec2 collides(const TileMap &tileMap);

	Component::type GetType();
};

#endif//COLLIDERHPP
