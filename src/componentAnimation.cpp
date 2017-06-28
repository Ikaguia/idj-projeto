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
			colliders[i]=new CompCollider{coll->colls[0].cType,Rect{x,y,w,h}};
			colliders[i]->colls[0].useDefault = coll->colls[0].useDefault;
			colliders[i]->colls[0].active = coll->colls[0].active;

			colliders[i]->entity = entity;

			FOR(funcI,funcCount){
				in >> func;
				if(txtFuncsF.count(func))frameFunc[i] = txtFuncsF[func](in);
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
void CompAnim::SetCurFrame(int frame){
	if(frame != GetCurFrame()){
		sp.SetFrame(frame);
		ENTITY(entity)->SetComponent(Component::type::t_collider,colliders[GetCurFrame()]);
		if(frameFunc.count(frame))frameFunc[frame](ENTITY(entity).get());
	}
}

bool CompAnim::Looped()const{
	return sp.Looped();
}

void CompAnim::Update(float time){
	int frame1=GetCurFrame();
	if(!called){
		frameFunc[frame1](ENTITY(entity).get());
		called=true;
	}
	sp.Update(time);
	int frame2=GetCurFrame();
	if(frame1 != frame2 && frameFunc.count(frame2))called=false;
	ENTITY(entity)->SetComponent(Component::type::t_collider,colliders[frame2]);
}
void CompAnim::Render(){
	Vec2 pos=ENTITY(entity)->FullBox().corner().renderPos();

	sp.SetFlipH(ENTITY(entity)->flipped);
	sp.Render(pos,ENTITY(entity)->rotation,Camera::zoom);
}
void CompAnim::Own(GameObject *go){
	entity=go->uid;
	for(auto& coll:colliders)coll->Own(go);
	colliders[GetCurFrame()]->Own(go);
	go->SetComponent(Component::type::t_collider,colliders[GetCurFrame()]);
}
Component::type CompAnim::GetType()const{
	return Component::type::t_animation;
}
