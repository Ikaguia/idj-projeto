#include <collider.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

static set<Collider*,decltype(Collider::collidersOrder)> Collider::colliders(Collider::collidersOrder);

static bool collidersOrder(const Collider* a,const Collider* b){
	return a->pol.BoundingRect().x < b->pol.BoundingRect().x;
}
static bool collidersEnd(const Collider* a,const Collider* b){
	return a->pol.BoundingRect().x < b->pol.BoundingRect().x;
}


Collider::Collider(){}
Collider::~Collider(){}



