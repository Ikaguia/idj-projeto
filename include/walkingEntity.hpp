#ifndef WALKING_ENTITYHPP
#define WALKING_ENTITYHPP

#include <common.hpp>
#include <gameObject.hpp>
#include <tileMap.hpp>

class WalkingEntity : public GameObject{
public:
	bool onGround=false;
	bool onLeft=false;
	bool onRight=false;
	bool onTop=false;
	WalkingEntity();
	virtual ~WalkingEntity(){}
};

#endif//WALKING_ENTITYHPP
