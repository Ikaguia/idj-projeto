#ifndef MEMORYHPP
#define MEMORYHPP

#include <common.hpp>
#include <component.hpp>

#define COMPMEMORY(x)  ((CompMemory*)x. components[Component::type::t_memory])
#define COMPMEMORYp(x) ((CompMemory*)x->components[Component::type::t_memory])

class CompMemory : public Component{
	//private members
public:
	//public members
	map<string,string> strings;
	map<string,int> integers;
	map<string,float> floats;

	CompMemory();
	~CompMemory();
	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//MEMORYHPP
