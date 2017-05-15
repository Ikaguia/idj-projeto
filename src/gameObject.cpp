#include <gameObject.hpp>

vector<GameObject*> GameObject::entities;

GameObject::GameObject(){
	entities.push_back(this);
}
GameObject::~GameObject(){
	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];
}

void GameObject::Update(float time){
	FOR(i,Component::type::t_count)if(hasComponent[i])components[hasComponent[i]]->Update(time);
}

void GameObject::Render(){
	//TODO: use this after making static and animated render components
	// if(hasComponent[Component::type::t_static_render])  (*StaticRender)  Component[Component::type::t_static_render]  ->Render();
	// if(hasComponent[Component::type::t_animated_render])(*AnimatedRender)Component[Component::type::t_animated_render]->Render();
}

void GameObject::AddComponent(Component* component){
	auto t=component->GetType();
	if(hasComponent[t])cout << "Error, adding component " << t << " to a GameObject that already has it" << endl;
	hasComponent[t]=true;
	components[t]=component;
}

void GameObject::ReplaceComponent(Component* component){
	auto t=component->GetType();
	if(!hasComponent[t]){
		cout << "Error, replacing component " << t << " on a GameObject that doesnt have it" << endl;
		hasComponent[t]=true;
	}
	else delete components[t];
	components[t]=component;
}

void GameObject::RemoveComponent(Component::type t){
	if(!hasComponent[t])cout << "Error, removing component " << t << " on a GameObject that doesnt have it" << endl;
	else{
		delete components[t];
		components[t]=nullptr;
		hasComponent[t]=false;
	}
}

bool GameObject::IsDead()const{
	return false;
}