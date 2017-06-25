#include <componentAnimationController.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
//#include <game.hpp>
//#include <inputManager.hpp>

CompAnimControl::CompAnimControl(string file,CompCollider* coll){
	string name,animFile;

	ifstream in(ANIMATION_PATH + file + ".txt");
	if(!in.is_open())cout << "Erro ao abrir arquivo de grupo de animações '" << file << "'" << endl;
	else{
		in >> name >> animFile;
		animations[name] = make_unique<CompAnim>(animFile,coll);
		cur=name;
		while(in >> name >> animFile){
			animations[name] = make_unique<CompAnim>(animFile,coll);
		}
		in.close();
	}
}
CompAnimControl::~CompAnimControl(){}


void CompAnimControl::ChangeCur(string anim){
	if(animations.count(anim)){
		cur=anim;
		GetCur().Own(entity);
	}
	else cout << "Erro: Controle de animação nao tem animação " << anim << endl;
}
CompAnim& CompAnimControl::GetCur(){
	return *animations[cur];
}



void CompAnimControl::Update(float time){
	GetCur().Update(time);
}
void CompAnimControl::Render(){
	GetCur().Render();
}
void CompAnimControl::Own(GameObject* go){
	entity=go;
	for(auto &anim:animations)anim.second->Own(go);
	GetCur().Own(go);
}
Component::type CompAnimControl::GetType()const{
	return Component::type::t_animation_control;
}
