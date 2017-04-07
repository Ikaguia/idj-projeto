#ifndef WALKING_ENTITYHPP
#define WALKING_ENTITYHPP

#include <common.hpp>
#include <gameObject.hpp>
#include <tileMap.hpp>

class WalkingEntity : public GameObject{
public:
	bool onAir;
	WalkingEntity();
	virtual ~WalkingEntity(){}

	virtual void Update(float time)=0;
	virtual void UpdatePos(float time)=0;
	virtual void UpdatePhysics(float time)=0;
	virtual void Render()=0;

	virtual bool IsDead()=0;

	virtual void NotifyCollision(GameObject *other)=0;
	virtual bool Is(string type)=0;

	virtual void CheckCollisionGround(const TileMap &tileMap)=0;
};

#endif//WALKING_ENTITYHPP
