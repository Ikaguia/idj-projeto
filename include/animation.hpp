#ifndef ANIMATIONHPP
#define ANIMATIONHPP

#include <common.hpp>
#include <timer.hpp>
#include <sprite.hpp>
#include <gameObject.hpp>

class Animation : public GameObject{
	Timer endTimer;
	float timeLimit;
	bool oneTimeOnly;
	Sprite sp;
public:
	Animation(float x,float y,float rotation,string sprite,int fCount,float fTime,bool ends);

	void Update(float time);
	void Render();

	bool IsDead();

	void NotifyCollision(GameObject& other);
	bool Is(string type);
};

#endif//ANIMATIONHPP