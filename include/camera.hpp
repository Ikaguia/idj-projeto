#ifndef CAMERAHPP
#define CAMERAHPP

#include <common.hpp>

#include <geometry.hpp>
#include <inputManager.hpp>

class GameObject;

#define CAMERA Camera::pos
#define CAMERAZOOM Camera::zoom
#define CAMERALOCK Camera::lock
#define RENDERPOS Camera::RenderPos
#define RENDERPOSX Camera::RenderPosX
#define RENDERPOSY Camera::RenderPosY


class Camera{
	static bool following;
public:
	static uint focus;
	static Vec2 pos;
	static Vec2 speed;
	static float zoom;
	static bool lock;
	
	static void Follow(uint newFocus);
	static void Unfollow();
	static void Update(float time);
	
	static void CenterTo(const Vec2& v);
	
	static Vec2 RenderPos(const Vec2& v);
	static int RenderPosX(const float& x);
	static int RenderPosY(const float& y);
};

#endif//CAMERAHPP
