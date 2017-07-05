#ifndef COMPANIMATIONHPP
#define COMPANIMATIONHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>
#include <sprite.hpp>
#include <componentCollider.hpp>

#define COMPANIM(x)  ((CompAnim*)x. components[Component::type::t_animation])
#define COMPANIMp(x) ((CompAnim*)x->components[Component::type::t_animation])

class CompAnim : public Component{
public:
	vector<CompCollider*> colliders;
	map<int,vector<function<void(GameObject*)>>> frameFunc;
	Sprite sp;
	bool called=true;

	//public members
	CompAnim();
	CompAnim(string file,CompCollider* coll);
	~CompAnim();

	int GetFrameCount()const;
	int GetCurFrame()const;
	void SetCurFrame(int frame,bool force=false);

	bool Looped()const;

	void Update(float time);
	void Own(GameObject *go);
	void Render();
	Component::type GetType()const;
};

#endif//COMPANIMATIONHPP
