#include <componentAnimation.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <stateStage.hpp>
#include <txtFuncs.hpp>
//#include <inputManager.hpp>

CompAnim::CompAnim(){}
CompAnim::CompAnim(string file,CompCollider* tempColl){
	string name,imgFile,func,animFile,type;
	int fCount,collCount,funcCount;
	float fTime,x,y,w,h,r;

	ifstream in(ANIMATION_PATH + file + ".txt");
	if(!in.is_open())cerr << "Erro ao abrir arquivo de animação '" << file << "'" << endl;
	else{
		in >> imgFile >> fCount >> fTime;
		sp.Open(imgFile,fCount,fTime);
		colliders.resize(fCount,nullptr);
		FOR(i,fCount){
			in >> collCount;
			if(collCount){
				colliders[i]=new CompCollider{};
				colliders[i]->entity = entity;
				FOR(j,collCount){
					//TODO: use rotation
					//TODO: different collider types for each coll
					in >> x >> y >> w >> h >> r;
					colliders[i]->colls.emplace_back(entity,tempColl->colls[0].cType,Rect{x,y,w,h});
					colliders[i]->colls[j].useDefault = tempColl->colls[0].useDefault;
					colliders[i]->colls[j].active = tempColl->colls[0].active;
				}
			}
			else colliders[i]=nullptr;

			in >> funcCount;
			FOR(funcI,funcCount){
				in >> func;
				if(txtFuncsF.count(func))frameFunc[i].push_back(txtFuncsF[func](in));
			}
		}
		in.close();
	}
	if(frameFunc.count(0))called=false;
}
CompAnim::~CompAnim(){
	FOR(i,colliders.size()){
		if(i==GetCurFrame())continue;
		delete colliders[i];
	}
}


int CompAnim::GetFrameCount()const{
	return sp.GetFrameCount();
}
int CompAnim::GetCurFrame()const{
	return sp.GetCurFrame();
}
void CompAnim::SetCurFrame(int frame,bool force){
	if(frame != GetCurFrame()){
		sp.SetFrame(frame);
		for(auto &foo:frameFunc[frame])foo(GO(entity));
		called=true;
		force=true;
	}
	if(force){
		if(colliders[frame] != nullptr){
			GO(entity)->SetComponent(Component::type::t_collider,colliders[frame]);
		}
		else if(GO(entity)->hasComponent[Component::type::t_collider]){
			GO(entity)->hasComponent[Component::type::t_collider]=false;
			GO(entity)->components[Component::type::t_collider]=nullptr;
		}
	}
}

bool CompAnim::Looped()const{
	return sp.Looped();
}

void CompAnim::Update(float time){
	int frame1=GetCurFrame();
	if(!called){
		for(auto &foo:frameFunc[frame1])foo(GO(entity));
		called=true;
	}
	sp.Update(time);
	int frame2=GetCurFrame();
	if(frame1 != frame2){
		called=false;
		SetCurFrame(frame2,true);
	}
}
void CompAnim::Render(){
	Vec2 pos=GO(entity)->FullBox().corner().renderPos();

	sp.SetFlipH(GO(entity)->flipped);
	sp.Render(pos,GO(entity)->rotation,Camera::zoom);
}
void CompAnim::Own(GameObject *go){
	entity=go->uid;
	for(CompCollider *coll:colliders){
		if(coll != nullptr){
			coll->Own(go);
		}
	}
	SetCurFrame(GetCurFrame(),true);
}
Component::type CompAnim::GetType()const{
	return Component::type::t_animation;
}
