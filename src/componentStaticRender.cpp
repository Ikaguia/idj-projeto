#include <componentStaticRender.hpp>
#include <gameObject.hpp>
#include <camera.hpp>
#include <game.hpp>
//#include <inputManager.hpp>

CompStaticRender::CompStaticRender(const Sprite &s,const Vec2 &p, const bool cs):sp{s},pos{p}, camScaling{cs}{}
CompStaticRender::~CompStaticRender(){}


void CompStaticRender::Update(float time){
	sp.Update(time);
}
void CompStaticRender::Render(){
	Vec2 p = pos;
	p = ENTITY(entity)->Box().corner() + p.rotate(ENTITY(entity)->rotation);
	sp.SetFlipH(ENTITY(entity)->flipped);
	if(camScaling) sp.Render((p-CAMERA)*CAMERAZOOM,ENTITY(entity)->rotation, CAMERAZOOM);
	else sp.Render(p,ENTITY(entity)->rotation, 1);
}
Component::type CompStaticRender::GetType()const{
	return Component::type::t_static_render;
}
