#include <componentStaticRender.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
//#include <game.hpp>
//#include <inputManager.hpp>

CompStaticRender::CompStaticRender(const Sprite &s,const Vec2 &p, const bool cs):sp{s},pos{p}, camScaling{cs}{}
CompStaticRender::~CompStaticRender(){}

void CompStaticRender::Update(float time){}
void CompStaticRender::Render(){
	Vec2 p=pos;
	p=entity->box.corner()+p.rotate(entity->rotation);
	if(camScaling) sp.Render((p-CAMERA)*CAMERAZOOM,entity->rotation, CAMERAZOOM);
	else sp.Render(p,entity->rotation, 1);
}
Component::type CompStaticRender::GetType()const{
	return Component::type::t_static_render;
}
