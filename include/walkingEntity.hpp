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
};

#endif//WALKING_ENTITYHPP
