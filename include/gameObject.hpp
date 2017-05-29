#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>

class GameObject{
public:
	bitset<Component::type::t_count> hasComponent;
	array<Component*,Component::type::t_count> components;

	Rect box;
	float rotation;
	bool anchored;
	bool dead;

	GameObject();
	GameObject(const Rect &rec,float r=0, bool a=false);
	~GameObject();

	void Update(float time);
	void Render();
	void AddComponent(Component* component);
	void ReplaceComponent(Component* component);
	void RemoveComponent(Component::type t);


	bool IsDead()const;
};

#endif//GAMEOBJECTHPP
