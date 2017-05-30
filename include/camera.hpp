#ifndef CAMERAHPP
#define CAMERAHPP

#include <common.hpp>

#include <geometry.hpp>
#include <inputManager.hpp>

class GameObject;

#define CAMERA Camera::pos
#define CAMERAZOOM Camera::zoom
#define RENDERPOS Camera::RenderPos
#define RENDERPOSX Camera::RenderPosX
#define RENDERPOSY Camera::RenderPosY


class Camera{
public:
	static GameObject* focus;
	static Vec2 pos;
	static Vec2 speed;
	static float zoom;
	
	static void Follow(GameObject* newFocus);
	static void Unfollow();
	static void Update(float time);
	
	static void CenterTo(const Vec2& v);
	
	static Vec2 RenderPos(const Vec2& v);
	static int RenderPosX(const float& x);
	static int RenderPosY(const float& y);
};

#endif//CAMERAHPP
