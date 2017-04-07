#include <bullet.hpp>
#include <camera.hpp>

Bullet::Bullet(float x,float y,float ang,float spd,float mD,string spr,int fCount,float fTime,GameObject* own):sp{Sprite(spr,fCount,fTime)},distLeft{mD},angle{ang},owner{own}{
	box.x=x-sp.GetWidth()/2;
	box.y=y-sp.GetHeight()/2;
	box.w=sp.GetWidth();
	box.h=sp.GetHeight();
	speed = Vec2(spd,0).rotate(angle);
}

void Bullet::Update(float time){
	box+=(speed*time);
	distLeft-=(speed*time).len();
	sp.Update(time);
}

void Bullet::Render(){
	sp.Render(box.x-Camera::pos.x,box.y-Camera::pos.y,angle);
}

bool Bullet::IsDead(){
	return distLeft<=0;
}

void Bullet::Kill(){
	distLeft=0;
}


void Bullet::NotifyCollision(GameObject *other){
	if(other==nullptr){//hit the ground
		Kill();//mark the bullet to be destroyed
	}
	else if(!other->Is("Bullet") && other!=owner){
		Kill();//mark the bullet to be destroyed
		other->NotifyCollision(this);
	}
}
bool Bullet::Is(string type){
	return (type=="Bullet");
}
