#include <componentAnimation.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <complib.hpp>
//#include <game.hpp>
//#include <inputManager.hpp>

CompAnim::CompAnim(){}
CompAnim::CompAnim(string file){
	string name,imgFile;
	int fCount;
	float fTime,x,y,w,h,r;

	ifstream in(ANIMATION_PATH+file);
	if(!in.is_open())cout << "Erro ao abrir arquivo de animação '" << file << "'" << endl;
	else{
		while(in >> imgFile >> fCount >> fTime){
			sp.Open(imgFile,fCount,fTime);
			// colliders.resize(fCount,nullptr);
			FOR(i,fCount){
				//TODO: make colliders use this
				in >> x >> y >> w >> h >> r;
				// colliders[i]=new CompCollider{CompCollider::collType::t_player};
				// colliders[i]->entity=entity;
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
	sp.Update(time);
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
