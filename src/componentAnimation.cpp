#include <componentAnimation.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <stateStage.hpp>
#include <txtFuncs.hpp>
//#include <inputManager.hpp>

CompAnim::CompAnim(){}
CompAnim::CompAnim(string file,CompCollider* coll){
	string name,imgFile,func,animFile,type;
	int fCount,funcCount;
	float fTime,x,y,w,h,r;

	ifstream in(ANIMATION_PATH + file + ".txt");
	if(!in.is_open())cout << "Erro ao abrir arquivo de animação '" << file << "'" << endl;
	else{
		in >> imgFile >> fCount >> fTime;
		sp.Open(imgFile,fCount,fTime);
		colliders.resize(fCount,nullptr);
		FOR(i,fCount){
			//TODO: use rotation
			in >> x >> y >> w >> h >> r >> funcCount;
			colliders[i]=new CompCollider{Rect{x,y,w,h},coll->cType};
			colliders[i]->useDefault = coll->useDefault;
			colliders[i]->active = coll->active;

			colliders[i]->entity = entity;

			FOR(funcI,funcCount){
				in >> func;
				if(txtFuncsF.count(func))frameFunc[i] = txtFuncsF[func](in);
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
