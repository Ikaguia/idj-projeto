#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>
#include <geometry.hpp>

class TileMap;

class GameObject{
public:
	Rect box;
	Rect nextBox;
	float rotation;

	GameObject(float r=0);
	virtual ~GameObject(){}

	virtual void Update(float time)=0;
	virtual void UpdatePos(float time)=0;
	virtual void UpdatePhysics(float time)=0;
	virtual void Render()=0;

	virtual bool IsDead()=0;

	virtual void CheckCollisionGround(const TileMap &tileMap);
	virtual void NotifyCollision(GameObject *other)=0;
	virtual bool Is(string type)=0;
};

#endif//GAMEOBJECTHPP