#ifndef BULLETHPP
#define BULLETHPP

#include <common.hpp>
#include <gameObject.hpp>
#include <geometry.hpp>
#include <sprite.hpp>

class Bullet : public GameObject{
	Sprite sp;
	Vec2 speed;
	float distLeft;
	float angle;
public:
	GameObject* owner;

	Bullet(float x,float y,float ang,float spd,float mD,string spr,int fCount,float fTime,GameObject* owner=nullptr);

	void Update(float time);
	void Render();
	bool IsDead();
	void Kill();

	virtual void NotifyCollision(GameObject& other);
	virtual bool Is(string type);
};

#endif//BULLETHPP