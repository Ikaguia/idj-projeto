#include <componentText.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
//#include <game.hpp>
//#include <inputManager.hpp>

CompText::CompText(const Text &t,Vec2 p):txt{t},pos{p}{}
CompText::CompText(string text,int size,SDL_Color c,Vec2 p):txt{text,size,c},pos{p}{}
CompText::~CompText(){}

void CompText::Update(float time){}
void CompText::Render(){
	Vec2 p=pos+entity->box.corner();
	txt.SetPos(p,true,true);
	txt.Render(CAMERA);
}
Component::type CompText::GetType()const{
	return Component::type::t_text;
}
