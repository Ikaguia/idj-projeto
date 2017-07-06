#include <componentHP.hpp>
#include <componentMemory.hpp>
#include <componentAI.hpp>
#include <gameObject.hpp>
#include <game.hpp>
#include <camera.hpp>
//#include <inputManager.hpp>

CompHP::CompHP(int tot,bool showHP,bool showDMG,float dmgCD):total{tot},current{tot},showHP{showHP},showDMG{showDMG},cooldown{dmgCD}{}
CompHP::CompHP(int tot,int cur,bool showHP,bool showDMG,float dmgCD):total{tot},current{cur},showHP{showHP},showDMG{showDMG},cooldown{dmgCD}{}
CompHP::~CompHP(){}


void CompHP::Damage(int dmg){
	if(dmgCoolDown.Get()>cooldown){
		dmgCoolDown.Restart();
		current-=dmg;
		if(showDMG){
			//TODO: renderiza dano
		}
		if(GO(entity)->hasComponent[Component::type::t_memory])COMPMEMORYp(GO(entity))->ints["hit"]++;
	}
}


void CompHP::Update(float time){
	if(current<=0)GO(entity)->dead=true;
	dmgCoolDown.Update(time);
}
void CompHP::Render(){
	if(SETTINGS.showHP && showHP && current>0){
		Rect box{0,-GO(entity)->Box().w/5.0f - GO(entity)->Box().w/10.0f,GO(entity)->Box().w,GO(entity)->Box().w/5.0f};
		box+=GO(entity)->Box().corner();

		box = box.renderBox();

		SDL_SetRenderDrawColor(GAMERENDER, 0, 0, 0, 255);
		SDL_Rect rect=box.sdlRect();
		SDL_RenderDrawRect(GAMERENDER,&rect);

		SDL_SetRenderDrawColor(GAMERENDER, 255, 0, 0, 255);
		rect.w*=current;
		rect.w/=total;
		rect.x++;
		rect.y++;
		rect.w-=2;
		rect.h-=2;
		SDL_RenderFillRect(GAMERENDER,&rect);
	}
}
Component::type CompHP::GetType()const{
	return Component::type::t_hp;
}
