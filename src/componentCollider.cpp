#include <componentCollider.hpp>
#include <componentMovement.hpp>
#include <gameObject.hpp>
#include <game.hpp>
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
	else if(useDefault.count(collType::t_any))useDefault[collType::t_any](this,other);
	else if(entity->hasComponent[Component::type::t_movement]){
		Vec2 &speed=((CompMovement*)entity->components[Component::type::t_movement])->speed;

		if(speed==Vec2{})return;

		Vec2 &totMove=((CompMovement*)entity->components[Component::type::t_movement])->move;
		Vec2 move;

 		Vec2 totX{totMove.x,0.0f};
		Vec2 moveX=collides(other,totX,entity->box+move);
		if(moveX != totX){
			cout << "collisionX " << entity->box << " with " << other->entity->box << endl;
			speed.x=0.0f;
		}
		move.x=moveX.x;

		Vec2 totY{0.0f,totMove.y};
		Vec2 moveY=collides(other,totY,entity->box+move);
		if(moveY != totY){
			cout << "collisionY " << entity->box << " with " << other->entity->box << endl;
			speed.y=0.0f;
		}
		move.y=moveY.y;


		totMove=move;
	}
}

Vec2 CompCollider::collides(const CompCollider *other,const Vec2 &move) const{
	return collides(other,move,entity->box);
}

Vec2 CompCollider::collides(const CompCollider *other,const Vec2 &move,const Rect &box) const{
	const int precision = 100;
	Vec2 moveSafe,moveSafe2,move100=move/precision,moveTry;
	FOR(i,precision+1){
		moveTry=move100*i;
		if((box+moveTry).collides(other->entity->box))return moveSafe;
		moveSafe2=moveSafe;
		moveSafe=moveTry;
	}
	return moveSafe;
}


void CompCollider::Update(float time){
	set<GameObject*> entities = GAMESTATE.GetEntitiesInRange(entity->box.x-10,entity->box.x+entity->box.w+10);
	for(GameObject* go:entities)if(go!=entity && go->hasComponent[Component::type::t_collider])collisionCheck((CompCollider*)go->components[Component::type::t_collider]);
}

void CompCollider::Render(){};

Component::type CompCollider::GetType() const{
	return Component::type::t_collider;
}
