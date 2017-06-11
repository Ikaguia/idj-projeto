#ifndef COMPANIMATIONHPP
#define COMPANIMATIONHPP

#include <common.hpp>

#include <component.hpp>
#include <geometry.hpp>
#include <sprite.hpp>
#include <componentCollider.hpp>

class CompAnim : public Component{
public:
	vector<CompCollider*> colliders;
	map<int,function<void(GameObject*)>> frameFunc;
	Sprite sp;

	//public members
	CompAnim();
	CompAnim(string file);
	~CompAnim();

	int GetCurFrame();
	void SetCurFrame(int frame);
	int GetFrameCount();

	void Update(float time);
	void Render();
	void Own(GameObject* go);
	Component::type GetType()const;
};

#endif//COMPANIMATIONHPP
