#include <componentStaticRender.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
//#include <game.hpp>
//#include <inputManager.hpp>

CompStaticRender::CompStaticRender(const Sprite &s,const Vec2 &p):sp{s},pos{p}{}
CompStaticRender::~CompStaticRender(){}

void CompStaticRender::Update(float time){}
void CompStaticRender::Render(){
	Vec2 p=pos;
	p=entity->box.corner()+p.rotate(entity->rotation);
	sp.Render(p.x-CAMERA.x,p.y-CAMERA.y,entity->rotation);
}
Component::type CompStaticRender::GetType()const{
	return Component::type::t_static_render;
}
