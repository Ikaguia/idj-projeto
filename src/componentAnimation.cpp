#include <componentAnimation.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <stateStage.hpp>
//#include <inputManager.hpp>

CompAnim::CompAnim(){}
CompAnim::CompAnim(string file){
	string name,imgFile,func,animFile,type;
	int fCount,dmgLow,dmgHigh;
	float fTime,x,y,w,h,r,f;
	bool dmgSelf,stick;

	ifstream in(ANIMATION_PATH+file);
	if(!in.is_open())cout << "Erro ao abrir arquivo de animação '" << file << "'" << endl;
	else{
		while(in >> imgFile >> fCount >> fTime){
			sp.Open(imgFile,fCount,fTime);
			// colliders.resize(fCount,nullptr);
			FOR(i,fCount){
				//TODO: make colliders use this
				in >> x >> y >> w >> h >> r >> func;
				// colliders[i]=new CompCollider{CompCollider::collType::t_player};
				// colliders[i]->entity=entity;
				if(func=="damageArea"){
					in >> x >> y >> w >> h >> r >> dmgLow >> dmgHigh >> dmgSelf;
					Rect rect{x,y,w,h};
					dmgHigh=max(dmgHigh,dmgLow+1);
					frameFunc[i] = [rect,r,dmgLow,dmgHigh,dmgSelf](GameObject* self){
						Rect area = self->box;
						area.w *= rect.w;
						area.h *= rect.h;
						if(self->flipped)area.x += self->box.w * rect.x;
						else area.x += (self->box.w * (1 - rect.x)) - area.w;
						area.y += self->box.h * rect.y;

						set<GameObject*> gos = GAMESTATE.GetEntitiesInRange(rect.x,rect.x+rect.w);
						for(GameObject* go:gos){
							if(dmgSelf || go != self){
								//TODO: change collision to work with rotation
								if(go->hasComponent[Component::type::t_hp] && area.collides(go->box)){
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
						Vec2 pos = self->box.corner();
						float ang=r;
						if(self->flipped)pos.x += self->box.w * x;
						else pos.x += (self->box.w * (1 - x)),ang+=180;
						pos.y += self->box.h * y;

						GameObject* bullet = GameObject::MakeBullet(pos,animFile+".txt",self,f,ang,dmgLow,dmgHigh,stick);
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
		}
		in.close();
	}
}
CompAnim::~CompAnim(){
	// for(auto coll:colliders)delete coll;
}


int CompAnim::GetCurFrame(){
	return sp.GetCurFrame();
}
void CompAnim::SetCurFrame(int frame){
	sp.SetFrame(frame);
}
int CompAnim::GetFrameCount(){
	return sp.GetFrameCount();
}


void CompAnim::Update(float time){
	int frame1=sp.GetCurFrame();
	sp.Update(time);
	int frame2=sp.GetCurFrame();
	if(frame1 != frame2 && frameFunc.count(frame2))frameFunc[frame2](entity);
	// entity->SetComponent(Component::type::t_collider,colliders[GetCurFrame()]);
}
void CompAnim::Render(){
	Vec2 pos=entity->box.corner();
	pos.x=RENDERPOSX(pos.x);
	pos.y=RENDERPOSY(pos.y);
	sp.SetFlipH(entity->flipped);
	sp.Render(pos,entity->rotation,Camera::zoom);
}
void CompAnim::Own(GameObject* go){
	entity=go;
	//for(auto& coll:colliders)coll.Own(go);
}
Component::type CompAnim::GetType()const{
	return Component::type::t_animation;
}
