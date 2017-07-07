#include <componentAnimationController.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <game.hpp>
//#include <inputManager.hpp>

CompAnimControl::CompAnimControl(string file,CompCollider* coll){
	string name,animFile;

	ifstream in(ANIMATION_PATH + file + ".txt");
	if(!in.is_open())cerr << "Erro ao abrir arquivo de grupo de animações '" << file << "'" << endl;
	else{
		in >> name >> animFile;
		animations[name] = make_unique<CompAnim>(animFile,coll);
		cur=prev=name;
		while(in >> name >> animFile)animations[name] = make_unique<CompAnim>(animFile,coll);
		in.close();
	}
}
CompAnimControl::~CompAnimControl(){}


void CompAnimControl::ChangeCur(string anim,bool rep){
	if(animations.count(anim)){
		if(cur != anim){
			if(repeat)prev = cur;
			repeat = rep;
			cur=anim;
			GetCur().SetCurFrame(0);
			GetCur().sp.looped=false;
			GetCur().Own(GO(entity));
		}
	}
	else cerr << "Erro: Controle de animação nao tem animação '" << anim << "' entity uid = " << entity << endl;
}
CompAnim& CompAnimControl::GetCur(){
	return *animations[cur];
}
const string& CompAnimControl::GetCurName()const{
	return cur;
}


void CompAnimControl::Update(float time){
	if(animations.count(cur)){
		GetCur().Update(time);
		if(!repeat && GetCur().Looped()){
			if(dying)cur = "dead";
			else{
				GetCur().sp.looped=false;
				ChangeCur(prev);
				GetCur().Update(time);
			}
		}
	}
}
void CompAnimControl::Render(){
	if(animations.count(cur))GetCur().Render();
}
void CompAnimControl::Own(GameObject *go){
	entity=go->uid;
	for(auto &anim:animations){
		anim.second->Own(go);
	}
	GetCur().Own(go);
}
bool CompAnimControl::Die(float time){
	UNUSED(time);

	if(!animations.count("die"))return true;
	if(dying){if(cur != "die")return true;}
	else{
		ChangeCur("die",false);
		dying = true;
	}
	return false;
}
Component::type CompAnimControl::GetType()const{
	return Component::type::t_animation_control;
}
