#include <animation.hpp>
#include <camera.hpp>

Animation::Animation(float x,float y,float r,string sprite,int fCount,float fTime,bool ends):GameObject(r),timeLimit{fCount*fTime},oneTimeOnly{ends},sp{Sprite(sprite,fCount,fTime)}{
	//cout << "criado animacao " << sprite << " " << this << endl;
	box.x=x-sp.GetWidth()/2;
	box.y=y-sp.GetHeight()/2;
	box.w=sp.GetWidth();
	box.h=sp.GetHeight();
}

void Animation::Update(float time){
	endTimer.Update(time);
	sp.Update(time);
}
void Animation::Render(){
	sp.Render(box.center().x-Camera::pos.x,box.center().y-Camera::pos.y);
}

bool Animation::IsDead(){
	if(oneTimeOnly)return endTimer.Get()>=timeLimit;
	return false;
}

void Animation::NotifyCollision(GameObject& other){}
bool Animation::Is(string type){
	return (type=="Animation");
}
