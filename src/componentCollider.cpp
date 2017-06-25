#include <componentCollider.hpp>
#include <componentMovement.hpp>
#include <gameObject.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

set<CompCollider*> CompCollider::colliders;

CompCollider::CompCollider(const Rect &box,collType t):pos{box.x,box.y},size{box.w,box.h},cType{t}{
	colliders.insert(this);
}

CompCollider::CompCollider(collType t):size{1.0f,1.0f},cType{t}{
	colliders.insert(this);
}

void CompCollider::collisionCheck(CompCollider *other){
	if(entity->dead || other->entity->dead)return;
	if(useDefault.count(other->cType))useDefault[other->cType](this,other);
	else if(useDefault.count(collType::t_any))useDefault[collType::t_any](this,other);
	else if(entity->hasComponent[Component::type::t_movement]){
		Vec2 &speed=COMPMOVEp(entity)->speed;

		if(speed==Vec2{})return;

		Vec2 &totMove=COMPMOVEp(entity)->move;
		Vec2 move;

		move.x = collides(other,{totMove.x,0.0f},entity->Box()+move).x;
		if(move.x != totMove.x){
			// cout << "collision X " << entity->Box() << " with " << other->entity->Box() << endl;
			speed.x=0.0f;
		}

		move.y = collides(other,{0.0f,totMove.y},entity->Box()+move).y;
		if(move.y != totMove.y){
			// cout << "collision Y " << entity->Box() << " with " << other->entity->Box() << endl;
			speed.y=0.0f;
		}

		totMove=move;
	}
}

Vec2 CompCollider::collides(const CompCollider *other,const Vec2 &move) const{
	return collides(other,move,entity->Box());
}

Vec2 CompCollider::collides(const CompCollider *other,const Vec2 &move,const Rect &box) const{
	const int precision = 100;
	Rect OtherBox = other->entity->Box();
	Vec2 moveSafe,move100=move/precision,moveTry;
	FOR(i,precision+1){
		moveTry=move100*i;
		if((box+moveTry).collides(OtherBox)){
			return moveSafe;
		}
		moveSafe=moveTry;
	}
	return move;
}

void CompCollider::Activate(){
	active=true;
}

void CompCollider::Deactivate(){
	active=false;
}

void CompCollider::Update(float time){
	UNUSED(time);
	if(active){
		auto ent = GAMESTATE.GetEntitiesInRange(entity->pos.x-10,entity->pos.x+entity->size.x+10);
		for(GameObject* go:ent)if(go!=entity && go->hasComponent[Component::type::t_collider]){
			if(COMPCOLLIDERp(go)->active)collisionCheck(COMPCOLLIDERp(go));
		}
	}
}
void CompCollider::Render(){
	#ifdef RENDERCOLLISION
		if     (cType==CompCollider::collType::t_player) SET_COLOR4(255,0,0,100);
		else if(cType==CompCollider::collType::t_monster)SET_COLOR4(0,255,0,100);
		else if(cType==CompCollider::collType::t_bullet) SET_COLOR4(0,0,255,100);
		else if(cType==CompCollider::collType::t_ground) SET_COLOR4(255,255,0,100);
		else SET_COLOR4(255,255,255,100);
		SDL_Rect r = (entity->Box().renderBox().sdlRect());
		FILL_RECT(&r);
	#endif//RENDERCOLLISION
};
void CompCollider::Own(GameObject* go){
	entity=go;
	entity->curPos = pos;
	entity->curSize = size;
}
Component::type CompCollider::GetType() const{
	return Component::type::t_collider;
}
