#include <collider.hpp>
#include <gameObject.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

set<Collider*,collidersOrder> Collider::colliders;
set<Collider*,collidersOrderEnd> Collider::collidersEnd;

bool collidersOrder::operator() (const Collider* a,const Collider* b){
	return a->entity->box.x < b->entity->box.x;
}
bool collidersOrderEnd::operator() (const Collider* a,const Collider* b){
	return (a->entity->box.x+a->entity->box.w) < (b->entity->box.x+a->entity->box.h);
}

set<Collider*> Collider::GetCollidersInRange(float x1,float x2){
	auto func1 = [](const Collider* a,const float &x){return a->entity->box.x < x;};
	auto func2 = [](const Collider* a,const float &x){return (a->entity->box.x+a->entity->box.w) < x;};

	set<Collider*> v;
	auto cur1=lower_bound(colliders.begin(), colliders.end(),x1,func1);
	auto end1=lower_bound(colliders.begin(), colliders.end(),x2,func1);
	for(;cur1!=end1;cur1++)v.insert(*cur1);
	auto cur2=lower_bound(collidersEnd.begin(), collidersEnd.end(),x1,func2);
	auto end2=lower_bound(collidersEnd.begin(), collidersEnd.end(),x2,func2);
	for(;cur2!=end2;cur2++)v.insert(*cur2);
	return v;
}

Collider::Collider(GameObject* go):Component::Component{go}{}
Collider::~Collider(){
	entity->RemoveComponent(GetType());
	colliders.erase(this);
	collidersEnd.erase(this);
}

void Collider::collisionCheck(Collider *other){
	if(useDefault.count(other->cType))useDefault[other->cType](this,other);
	//TODO: uncomment this after making class Movement : public Component
	// else if(entity->hasComponent[Component::type::t_movement]){
	// 	Vec2 &speed=(*Movement)entity->components[Component::type::t_movement]->move;
	// 	Vec2 move=collides(other,speed);
	// 	entity->box+=move;
	// 	if(speed!=move){
	// 		Vec2 speedX{speed.x-move.x,0};
	// 		Vec2 moveX=collides(other,speedX);
	// 		entity->box+=moveX;
	// 		if(speedX!=moveX)speed.x=0;
	//
	// 		Vec2 speedY{0,speed.y-move.y};
	// 		Vec2 moveY=collides(other,speedY);
	// 		entity->box+=moveY;
	// 		if(speedY!=moveY)speed.y=0;
	// 	}
	// }
}

void Collider::collisionCheck(const TileMap &tileMap){}
Vec2 Collider::collides(const Collider *other,const Vec2 &move){return Vec2{};}
Vec2 Collider::collides(const TileMap &tileMap,const Vec2 &move){return Vec2{};}

Component::type Collider::GetType(){
	return Component::type::t_collider;
}
