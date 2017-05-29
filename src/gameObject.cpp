#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>

GameObject::GameObject(){}
GameObject::GameObject(const Rect &rec,float r,bool a):box{rec},rotation{r},anchored{a},dead{false}{}
GameObject::~GameObject(){
	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];
}

void GameObject::Update(float time){
	if(hasComponent[Component::type::t_input_control])components[Component::type::t_input_control]->Update(time);

	if(hasComponent[Component::type::t_movement]){
		CompMovement *compM = (CompMovement*) components[Component::type::t_movement];
		compM->move=compM->speed*time;
	}

	FOR2(i,Component::type::t_collider,Component::type::t_count)if(hasComponent[i])components[i]->Update(time);
}

void GameObject::Render(){
	FOR(i,Component::type::t_count){
		if(hasComponent[i]){
			components[i]->Render();
		}
	}
}

void GameObject::AddComponent(Component* component){
	auto t=component->GetType();
	if(hasComponent[t])cout << "Error, adding component " << t << " to a GameObject that already has it" << endl;
	hasComponent[t]=true;
	components[t]=component;
	component->entity=this;
}

void GameObject::ReplaceComponent(Component* component){
	auto t=component->GetType();
	if(!hasComponent[t]){
		cout << "Error, replacing component " << t << " on a GameObject that doesnt have it" << endl;
		hasComponent[t]=true;
	}
	else delete components[t];
	components[t]=component;
	component->entity=this;
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
