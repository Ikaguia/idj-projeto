#include <componentHP.hpp>
#include <gameObject.hpp>
#include <game.hpp>
#include <camera.hpp>
//#include <inputManager.hpp>

CompHP::CompHP(int tot,bool showHP,bool showDMG):total{tot},current{tot},showHP{showHP},showDMG{showDMG}{}
CompHP::CompHP(int tot,int cur,bool showHP,bool showDMG):total{tot},current{cur},showHP{showHP},showDMG{showDMG}{}
CompHP::~CompHP(){}


void CompHP::Damage(int dmg){
	current-=dmg;
	if(showDMG){
		//TODO: renderiza dano
	}
}


void CompHP::Update(float time){
	if(current<=0)entity->dead=true;
}
void CompHP::Render(){
	if(showHP && current>0){
		Rect box{0,-entity->box.w/5.0f - entity->box.w/10.0f,entity->box.w,entity->box.w/5.0f};
		box+=entity->box.corner();
		box.x=RENDERPOSX(box.x);
		box.y=RENDERPOSY(box.y);
		box.w*=CAMERAZOOM;
		box.h*=CAMERAZOOM;

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
void CompHP::Own(GameObject* go){
	entity=go;
}
Component::type CompHP::GetType()const{
	return Component::type::t_hp;
}
