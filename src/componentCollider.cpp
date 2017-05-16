#include <componentCollider.hpp>
#include <gameObject.hpp>
//#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

set<CompCollider*> CompCollider::colliders;

CompCollider::CompCollider(collType t):cType{t}{
	colliders.insert(this);
}
CompCollider::~CompCollider(){
	colliders.erase(this);
}

void CompCollider::collisionCheck(CompCollider *other){
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

Vec2 CompCollider::collides(const CompCollider *other,const Vec2 &move) const{
	return move;
}


void CompCollider::Update(float time){
	set<GameObject*> entities = GameObject::GetEntitiesInRange(entity->box.x,entity->box.x+entity->box.w);
	for(GameObject* go:entities)if(go->hasComponent[Component::type::t_collider])collisionCheck((CompCollider*)go->components[Component::type::t_collider]);
}

void CompCollider::Render(){};

Component::type CompCollider::GetType() const{
	return Component::type::t_collider;
}
