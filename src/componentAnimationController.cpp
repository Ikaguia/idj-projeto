#include <componentAnimationController.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
//#include <game.hpp>
//#include <inputManager.hpp>

CompAnimControl::CompAnimControl(string file){
	string name,animFile;

	ifstream in(file);
	if(!in.is_open())cout << "Erro ao abrir arquivo de grupo de animações '" << file << "'" << endl;
	else{
		in >> name >> animFile;
		animations[name]=CompAnim{animFile};
		cur=name;
		while(in >> name >> animFile){
			animations[name]=CompAnim{animFile};
		}
		in.close();
	}
}
CompAnimControl::~CompAnimControl(){}


void CompAnimControl::ChangeCur(string anim){
	if(animations.count(anim))cur=anim;
	else cout << "Erro: Controle de animação nao tem animação " << anim << endl;
}
CompAnim& CompAnimControl::GetCur(){
	return animations[cur];
}



void CompAnimControl::Update(float time){
	GetCur().Update(time);
}
void CompAnimControl::Render(){
	GetCur().Render();
}
void CompAnimControl::Own(GameObject* go){
	entity=go;
	for(auto &anim:animations)anim.second.Own(go);
}
Component::type CompAnimControl::GetType()const{
	return Component::type::t_animation_control;
}
