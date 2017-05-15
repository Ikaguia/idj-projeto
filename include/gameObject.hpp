#ifndef GAMEOBJECTHPP
#define GAMEOBJECTHPP

#include <common.hpp>

class GameObject{
	bitset<Component::type{t_count}> hasComponent;
	vector<Component*> components;
public:
	static vector<GameObject*> entities;

	GameObject();
	~GameObject();

	void Update(float time);
	void AddComponent(Component* component);
	void ReplaceComponent(Component* component);
	void RemoveComponent(Component::type t);

	bool IsDead()const;
};

#endif//GAMEOBJECTHPP