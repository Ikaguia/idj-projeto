#include <camera.hpp>
#include <gameObject.hpp>
#include <inputManager.hpp>

GameObject* Camera::focus=nullptr;
Vec2 Camera::pos;
Vec2 Camera::speed;

#define WINDOWSIZE Vec2(1024,600)

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
	else pos=focus->box.center()-(WINDOWSIZE/2);
}
