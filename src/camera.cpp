#include <camera.hpp>

#include <common.hpp>

#include <game.hpp>
#include <gameObject.hpp>
#include <inputManager.hpp>

#define CAMERA_SPEED 500
#define MAX_ZOOM 1.0f
#define MIN_ZOOM 0.2f

GameObject* Camera::focus=nullptr;
Vec2 Camera::pos;
Vec2 Camera::speed;
float Camera::zoom = 1.0f;

void Camera::Follow(GameObject* newFocus){
	focus=newFocus;
}
void Camera::Unfollow(){
	focus=nullptr;
}
void Camera::Update(float time){
	Vec2 center = pos + (WINSIZE/2/zoom);
	if(INPUTMAN.IsKeyDown(KEY(z))) {
		zoom += 0.5*time;
		if(zoom > MAX_ZOOM)
			zoom = MAX_ZOOM;
		//cout<<"zoom: "<<zoom<<endl;
	}
	if(INPUTMAN.IsKeyDown(KEY(x))) {
		zoom -= 0.5*time;
		if(zoom < MIN_ZOOM)
			zoom = MIN_ZOOM;
		//cout<<"zoom: "<<zoom<<endl;
	}
	CenterTo(center);
	
	if(!focus){
		speed=Vec2(0,0);
		if(INPUTMAN.IsKeyDown(LEFT_ARROW_KEY)) speed.x-=CAMERA_SPEED;
		if(INPUTMAN.IsKeyDown(RIGHT_ARROW_KEY))speed.x+=CAMERA_SPEED;
		if(INPUTMAN.IsKeyDown(UP_ARROW_KEY))   speed.y-=CAMERA_SPEED;
		if(INPUTMAN.IsKeyDown(DOWN_ARROW_KEY)) speed.y+=CAMERA_SPEED;
		speed/=zoom;
		speed*=time;
		pos+=speed;
		//if(speed != Vec2(0,0)) cout<<"camera x= "<<pos.x<<"\t y= "<<pos.y<<endl;
	}
	else CenterTo(focus->box.center());
}

void Camera::CenterTo(const Vec2& v) {
	pos = v - (WINSIZE/2/zoom);
}

Vec2 Camera::RenderPos(const Vec2& v) {
	return (v-CAMERA)*CAMERAZOOM;
}

int Camera::RenderPosX(const float& x) {
	return (x-CAMERA.x)*CAMERAZOOM;
}

int Camera::RenderPosY(const float& y) {
	return (y-CAMERA.y)*CAMERAZOOM;
}
