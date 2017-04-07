#ifndef PLAYERHPP
#define PLAYERHPP

#include <common.hpp>
#include <walkingEntity.hpp>
#include <sprite.hpp>
#include <geometry.hpp>
#include <timer.hpp>

class Player : public WalkingEntity{
	Sprite sp;
	Vec2 speed;
	int hp;
	Timer t;
	bool doubleJump;
public:
	static Player* player;

	Player(float x,float y);
	~Player();

	void Update(float time);
	void UpdatePos(float time);
	void UpdatePhysics(float time);
	void Render();
	bool IsDead();

	void Damage(int dmg=15);

	void NotifyCollision(GameObject *other);
	bool Is(string type);

	void CheckCollisionGround(const TileMap &tileMap);
};

#endif//PLAYERHPP