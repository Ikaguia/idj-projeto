#include <component.hpp>
#include <gameObject.hpp>

void Component::Own(GameObject *go){
	entity = go->uid;
}
bool Component::Die(float time){
	UNUSED(time);
	return true;
}
