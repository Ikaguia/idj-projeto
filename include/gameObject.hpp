#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>
#include <component.hpp>
#include <geometry.hpp>

class GameObject{
public:
	bitset<Component::type::t_count> hasComponent;
	vector<Component*> components;

	static vector<GameObject*> entities;
	Rect box;
	float rotation;

	GameObject();
	GameObject(const Rect &r);
	~GameObject();

	void Update(float time);
	void Render();
	void AddComponent(Component* component);
	void ReplaceComponent(Component* component);
	void RemoveComponent(Component::type t);

	bool IsDead()const;
};

#endif//GAMEOBJECTHPP