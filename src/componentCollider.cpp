#include <componentCollider.hpp>
#include <componentMovement.hpp>
#include <gameObject.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

CompCollider::CompCollider(){}
CompCollider::CompCollider(collType t,const Rect &r){
	colls.emplace_back(entity,t,r);
}
CompCollider::CompCollider(collType t,const Vec2 &p,const Vec2 &sz){
	colls.emplace_back(entity,t,p,sz);
}

void CompCollider::CollisionCheck(CompCollider *other){
	if(GO(entity)->dead || GO(other->entity)->dead)return;
	for(Coll &collA:colls)for(Coll &collB:other->colls)collA.CollisionCheck(collB);
}

void CompCollider::Update(float time){
	UNUSED(time);
	for(Coll &coll:colls){
		if(coll.active){
			int x1 = coll.Box().x-10;
			int x2 = coll.Box().x2()+10;
			set<uint> ent = GAMESTATE.GetEntitiesInRange(x1,x2);
			for(uint go:ent){
				if(go != entity && GO(go)->HasComponent(Component::type::t_collider)){
					CollisionCheck(COMPCOLLIDERp(GO(go)));
				}
			}
		}
	}
}
void CompCollider::Render(){
	if(SETTINGS.showCollision)
		for(Coll coll:colls){
			if     (coll.cType==CompCollider::collType::t_player) SET_COLOR4(255,0,0,100);
			else if(coll.cType==CompCollider::collType::t_monster)SET_COLOR4(0,255,0,100);
			else if(coll.cType==CompCollider::collType::t_bullet) SET_COLOR4(0,0,255,100);
			else if(coll.cType==CompCollider::collType::t_ground) SET_COLOR4(255,255,0,100);
			else SET_COLOR4(255,255,255,100);
			SDL_Rect r = (coll.Box().renderBox().sdlRect());
			FILL_RECT(&r);
		}
};
void CompCollider::Own(GameObject *go){
	entity=go->uid;
	if(go != nullptr){
		if(colls.size()){
			Rect r{};
			for(Coll coll:colls)r = r.sum(Rect{coll.pos,coll.size});
			go->curPos = r.corner();
			go->curSize = r.size();
		}
		else{
			go->curPos = Vec2{};
			go->curSize = Vec2{0.0f,0.0f};
		}
	}
}
bool CompCollider::Die(float time){
	UNUSED(time);

	if(GO(entity)->HasComponent(Component::type::t_animation))return true;
	if(GO(entity)->HasComponent(Component::type::t_animation_control))return true;
	return false;
}
Component::type CompCollider::GetType() const{
	return Component::type::t_collider;
}





CompCollider::Coll::Coll(const uint &e,collType t,const Rect &r):
	entity{e},pos{r.corner()},size{r.size()},cType{t}{}
CompCollider::Coll::Coll(const uint &e,collType t,const Vec2 &p,const Vec2 &sz):
	entity{e},pos{p},size{sz},cType{t}{}

Rect CompCollider::Coll::Box() const{
	Rect r = GO(entity)->Box(pos,size);
	return r;
}

void CompCollider::Coll::CollisionCheck(const CompCollider::Coll &other){
	if(useDefault.count(other.cType))useDefault[other.cType](*this,other);
	else if(useDefault.count(collType::t_any))useDefault[collType::t_any](*this,other);
	else if(GO(entity)->HasComponent(Component::type::t_movement)){
		CompMovement *compMove = COMPMOVEp(GO(entity));

		Vec2 &speed=compMove->speed;
		Vec2 &totMove=compMove->move;
		Vec2 move;

		if(totMove==Vec2{})return;


		move.x = Collides(other,{totMove.x,0.0f},move).x;
		if(move.x != totMove.x){
			// cout << "collision X " << GO(entity)->Box() << " with " << GO(other.entity)->Box() << endl;
			speed.x=0.0f;
		}

		move.y = Collides(other,{0.0f,totMove.y},move).y;
		if(move.y != totMove.y){
			// cout << "collision Y " << GO(entity)->Box() << " with " << GO(other.entity)->Box() << endl;
			speed.y=0.0f;
		}

		totMove=move;
	}
}
Vec2 CompCollider::Coll::Collides(const Coll &other,const Vec2 &move,const Vec2 &moved) const{
	const int precision = 100;
	Rect box1 = Box()+moved;
	Rect box2 = other.Box();
	Vec2 moveSafe,move100=move/precision,moveTry;
	FOR(i,precision+1){
		moveTry = move100*i;
		if((box1+moveTry).collides(box2))return moveSafe;
		moveSafe = moveTry;
	}
	return move;
}
