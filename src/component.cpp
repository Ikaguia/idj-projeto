#include <component.hpp>
#include <gameObject.hpp>

void Component::Own(GameObject *go){
	entity = go->uid;
}
