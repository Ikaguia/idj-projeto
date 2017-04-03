#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>
#include <geometry.hpp>

class GameObject{
public:
	Rect box;
	float rotation;

	GameObject(float r=0);
	virtual ~GameObject(){}

	virtual void Update(float time)=0;
	virtual void Render()=0;
	virtual bool IsDead()=0;

	virtual void NotifyCollision(GameObject& other)=0;
	virtual bool Is(string type)=0;
};

#endif//GAMEOBJECTHPP