#ifndef COMPONENTCOLLIDERHPP
#define COMPONENTCOLLIDERHPP

#include <common.hpp>

#include <component.hpp>
#include <tileMap.hpp>

#define colliderFunc std::function<void(const Coll &a,const Coll &b)>

#define COMPCOLLIDER(x)  ((CompCollider*)x. components[Component::type::t_collider])
#define COMPCOLLIDERp(x) ((CompCollider*)x->components[Component::type::t_collider])

class CompCollider : public Component{
public:
	//members
	enum collType{t_any,t_ground,t_h_ground,t_player,t_monster,t_bullet,t_solid,t_count};
	struct Coll{
		const uint &entity;
		Vec2 pos;
		Vec2 size;
		const collType cType=t_any;
		bool active=true;
		map<collType,colliderFunc> useDefault;//use custom collision handler against colliders of _type

		Coll(const uint &e,collType t,const Rect &r=Rect{0.0f,0.0f,1.0f,1.0f});
		Coll(const uint &e,collType t,const Vec2 &p,const Vec2 &sz=Vec2{1.0f,1.0f});

		Rect Box() const;

		void CollisionCheck(const Coll &other);
		Vec2 Collides(const Coll &other,const Vec2& move,const Vec2& moved=Vec2{}) const;
	};
	vector<Coll> colls;

	//constructor/destructor
	CompCollider();
	CompCollider(collType t,const Rect &r=Rect{0.0f,0.0f,1.0f,1.0f});
	CompCollider(collType t,const Vec2 &p,const Vec2 &sz=Vec2{1.0f,1.0f});
	//functions
	void CollisionCheck(CompCollider *other);

	void Update(float time);
	void Render();
	void Own(GameObject *go);
	bool Die(float time);//return true to remove this component
	Component::type GetType() const;
};

#endif//COMPONENTCOLLIDERHPP
