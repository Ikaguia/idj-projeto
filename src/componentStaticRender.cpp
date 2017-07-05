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
	p = GO(entity)->Box().corner() + p.rotate(GO(entity)->rotation);
	sp.SetFlipH(GO(entity)->flipped);
	if(camScaling) sp.Render((p-CAMERA)*CAMERAZOOM,GO(entity)->rotation, CAMERAZOOM);
	else sp.Render(p,GO(entity)->rotation, 1);
}
Component::type CompStaticRender::GetType()const{
	return Component::type::t_static_render;
}
