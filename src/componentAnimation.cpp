#include <componentAnimation.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <stateStage.hpp>
//#include <inputManager.hpp>

CompAnim::CompAnim(){}
CompAnim::CompAnim(string file,CompCollider* coll){
	string name,imgFile,func,animFile,type;
	int fCount,dmgLow,dmgHigh;
	float fTime,x,y,w,h,r,f;
	bool dmgSelf,stick;

	ifstream in(ANIMATION_PATH + file + ".txt");
	if(!in.is_open())cout << "Erro ao abrir arquivo de animação '" << file << "'" << endl;
	else{
		in >> imgFile >> fCount >> fTime;
		sp.Open(imgFile,fCount,fTime);
		colliders.resize(fCount,nullptr);
		FOR(i,fCount){
			//TODO: use rotation
			in >> x >> y >> w >> h >> r >> func;
			colliders[i]=new CompCollider{Rect{x,y,w,h},coll->cType};
			colliders[i]->useDefault = coll->useDefault;
			colliders[i]->active = coll->active;


			colliders[i]->entity=entity;
			if(func=="damageArea"){
				in >> x >> y >> w >> h >> r >> dmgLow >> dmgHigh >> dmgSelf;
				Rect rect{x,y,w,h};
				dmgHigh=max(dmgHigh,dmgLow+1);
				frameFunc[i] = [rect,r,dmgLow,dmgHigh,dmgSelf](GameObject* self){
					Rect area = self->Box();
					area.w *= rect.w;
					area.h *= rect.h;
					if(self->flipped)area.x += self->size.x * rect.x;
					else area.x += (self->size.x * (1 - rect.x)) - area.w;
					area.y += self->size.y * rect.y;

					set<GameObject*> gos = GAMESTATE.GetEntitiesInRange(rect.x,rect.x+rect.w);
					for(GameObject* go:gos){
						if(dmgSelf || go->team != self->team){
							//TODO: change collision to work with rotation
							if(go->hasComponent[Component::type::t_hp] && area.collides(go->Box())){
								COMPHPp(go)->Damage(dmgLow+(rand()%(dmgHigh-dmgLow)));
							}
						}
					}
				};
			}
			if(func=="fireProjectile"){
				in >> x >> y >> f >> r >> dmgLow >> dmgHigh >> animFile >> stick;
				dmgHigh=max(dmgHigh,dmgLow+1);
				frameFunc[i] = [x,y,f,r,dmgLow,dmgHigh,animFile,stick](GameObject* self){
					Vec2 pos = self->Box().relativePos({x,y},self->flipped);
					float ang=r;
					if(!self->flipped)ang+=180;

					GameObject* bullet = GameObject::MakeBullet(pos,animFile,self,f,ang,dmgLow,dmgHigh,stick);

					GAMESTATE.AddObject(bullet);
				};
			}
			if(func=="changeVar"){
				in >> type >> name;
				if(type=="string"){
					string val;
					in >> val;
					frameFunc[i] = [name,val](GameObject* self){
						if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
						COMPMEMORYp(self)->strings[name]=val;
					};
				}
				if(type=="int"){
					int val;
					in >> val;
					frameFunc[i] = [name,val](GameObject* self){
						if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
						COMPMEMORYp(self)->ints[name]=val;
					};
				}
				if(type=="float"){
					float val;
					in >> val;
					frameFunc[i] = [name,val](GameObject* self){
						if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
						COMPMEMORYp(self)->floats[name]=val;
					};
				}
				if(type=="timer"){
					frameFunc[i] = [name](GameObject* self){
						if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
						COMPMEMORYp(self)->timers[name].Restart();
					};
				}
			}
		}
		in.close();
	}
}
CompAnim::~CompAnim(){
	FOR(i,colliders.size()){
		if(i==GetCurFrame())continue;
		delete colliders[i];
	}
}


int CompAnim::GetCurFrame(){
	return sp.GetCurFrame();
}
void CompAnim::SetCurFrame(int frame){
	sp.SetFrame(frame);
	entity->SetComponent(Component::type::t_collider,colliders[GetCurFrame()]);
}
int CompAnim::GetFrameCount(){
	return sp.GetFrameCount();
}


void CompAnim::Update(float time){
	int frame1=GetCurFrame();
	sp.Update(time);
	int frame2=GetCurFrame();
	if(frame1 != frame2 && frameFunc.count(frame2))frameFunc[frame2](entity);
	entity->SetComponent(Component::type::t_collider,colliders[frame2]);
}
void CompAnim::Render(){
	Vec2 pos=entity->FullBox().corner().renderPos()	;

	sp.SetFlipH(entity->flipped);
	sp.Render(pos,entity->rotation,Camera::zoom);
}
void CompAnim::Own(GameObject* go){
	entity=go;
	for(auto& coll:colliders)coll->Own(go);
	colliders[GetCurFrame()]->Own(go);
	entity->SetComponent(Component::type::t_collider,colliders[GetCurFrame()]);
}
Component::type CompAnim::GetType()const{
	return Component::type::t_animation;
}
