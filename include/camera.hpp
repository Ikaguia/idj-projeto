#ifndef CAMERAHPP
#define CAMERAHPP

#include <common.hpp>

#include <geometry.hpp>
#include <inputManager.hpp>

class GameObject;

#define CAMERA Camera::pos

class Camera{
	static GameObject* focus;
public:
	static Vec2 pos;
	static Vec2 speed;
	
	static void Follow(GameObject* newFocus);
	static void Unfollow();
	static void Update(float time);
};

#endif//CAMERAHPP
