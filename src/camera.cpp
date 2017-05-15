#include <camera.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <inputManager.hpp>

GameObject* Camera::focus=nullptr;
Vec2 Camera::pos;
Vec2 Camera::speed;

void Camera::Follow(GameObject* newFocus){
	focus=newFocus;
}
void Camera::Unfollow(){
	focus=nullptr;
}
void Camera::Update(float time){
	if(!focus){
		speed=Vec2(0,0);
		if(INPUTMAN.IsKeyDown(LEFT_ARROW_KEY)) speed.x-=200;
		if(INPUTMAN.IsKeyDown(RIGHT_ARROW_KEY))speed.x+=200;
		if(INPUTMAN.IsKeyDown(UP_ARROW_KEY))   speed.y-=200;
		if(INPUTMAN.IsKeyDown(DOWN_ARROW_KEY)) speed.y+=200;
		speed*=time;
		pos+=speed;
	}
	else pos=focus->box.center()-(WINSIZE/2);
}
