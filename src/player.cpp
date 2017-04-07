#include <player.hpp>
#include <inputManager.hpp>
#include <camera.hpp>
#include <game.hpp>
#include <state.hpp>
#include <bullet.hpp>
#include <animation.hpp>
#include <collision.hpp>


#define INITIAL_JUMP_SPEED 7.1//ms vertical speed


Player* Player::player=nullptr;

Player::Player(float x,float y):WalkingEntity::WalkingEntity(),sp{RESOURCESFOLDER+"img/player.png"},speed{0.0f,0.0f},hp{30}{
	box.x=x-sp.GetWidth()/2;
	box.y=y-sp.GetHeight()/2;
	box.w=sp.GetWidth();
	box.h=sp.GetHeight();
	player=this;
}
Player::~Player(){
	player=nullptr;
	Camera::Unfollow();
}

void Player::Update(float time){
	t.Update(time);

	if(!onAir)doubleJump=true;
	if(INPUTMAN.IsKeyDown(W_KEY) && !onAir){
		speed.y=-INITIAL_JUMP_SPEED;
		onAir=true;
	}
	else if(INPUTMAN.KeyPress(W_KEY) && onAir && doubleJump){
		speed.y=-INITIAL_JUMP_SPEED;
		doubleJump=false;
	}
	if((INPUTMAN.IsKeyDown(A_KEY) && INPUTMAN.IsKeyDown(D_KEY)));
	else if((!INPUTMAN.IsKeyDown(A_KEY) && !INPUTMAN.IsKeyDown(D_KEY))){
		speed.x/=1+(20*time);
		if(equals(speed.x,0.0f))speed.x=0.0f;
	}
	else if(INPUTMAN.IsKeyDown(A_KEY)){
		speed.x=std::max(-WALK_SPEED,speed.x-(WALK_SPEED*time/0.25));
	}
	else if(INPUTMAN.IsKeyDown(D_KEY)){
		speed.x=std::min( WALK_SPEED,speed.x+(WALK_SPEED*time/0.25));
	}
	if(INPUTMAN.MousePress(RIGHT_MOUSE_BUTTON))speed=Vec2(0.0f,0.0f);

	nextBox=box+(speed)*METER*time;
}
void Player::UpdatePos(float time){
	box=nextBox;
}
void Player::UpdatePhysics(float time){
	if(onAir){
		speed.y+=(GRAVITY*time);
		// if((speed.y+(GRAVITY*time))<0)speed.y+=(GRAVITY*time);
		// else{
		// 	speed.y=0.0f;
		// 	onAir=false;
		// }
	}
}
void Player::Render(){
	sp.Render(box.x-Camera::pos.x,box.y-Camera::pos.y,rotation);
}
bool Player::IsDead(){
	return hp<=0;
}

void Player::Damage(int dmg){
	hp-=dmg;
	if(IsDead()){
		GAMESTATE.AddObject(new Animation(box.x,box.y,rotation,RESOURCESFOLDER+"img/penguindeath.png",5,0.2f,true));
		GAMESTATE.AddSound(RESOURCESFOLDER+"audio/boom.wav",0);
	}
}


void Player::NotifyCollision(GameObject *other){
	if(other==nullptr){
		nextBox=box;
		speed=Vec2(0.0f,0.0f);
	}
	else if(other->Is("Bullet")){
		Bullet* b=(Bullet*)other;
		if(this != b->owner){
			if(!b->IsDead())b->NotifyCollision(this);//tell the bullet to destroy itself and call this again
			else Damage(10+rand()%10);//dmg the penguim
		}
	}
}
bool Player::Is(string type){
	return (type=="WalkingEntity") || (type=="Player");
}

void Player::CheckCollisionGround(const TileMap &tileMap){
	cout << "checking player ground collision" << endl;
	int x1,x2,y1,y2;
	bool out=false;
	tileMap.GetIndAtPos(box.x,      std::min(box.y,nextBox.y),      x1,y1);
	tileMap.GetIndAtPos(box.x+box.w,std::max(box.y,nextBox.y)+box.h,x2,y2);
	FOR2(j,y1,y2){
		if(out)break;
		FOR2(i,x1,x2){
			if(tileMap.AtMeta(i,j)==1){
				cout << "checking player collision with tile " << i << "," << j << endl;
				Rect tileBox(tileMap.GetWidth()*i,tileMap.GetHeight()*j,tileMap.GetWidth()*(i+1),tileMap.GetHeight()*(j+1));
				if(Collision::IsColliding(tileBox,tileBox,rotation,0.0f)){
					cout << "collided" << endl;
					if(onAir)onAir=false;
					speed.y=0.0f;
					nextBox.y=box.y;
					out = true;
					break;
				}
				cout << "nope" << endl;
			}
		}
	}

	out=false;
	tileMap.GetIndAtPos(std::min(box.x,nextBox.x),      std::min(box.y,nextBox.y),      x1,y1);
	tileMap.GetIndAtPos(std::max(box.x,nextBox.x)+box.w,std::max(box.y,nextBox.y)+box.h,x2,y2);
	FOR2(j,y1,y2){
		if(out)break;
		FOR2(i,x1,x2){
			if(tileMap.AtMeta(i,j)==1){
				Rect tileBox(tileMap.GetWidth()*i,tileMap.GetHeight()*j,tileMap.GetWidth()*(i+1),tileMap.GetHeight()*(j+1));
				if(Collision::IsColliding(tileBox,tileBox,rotation,0.0f)){
					nextBox.x=box.x;
					speed.x=0.0f;
					out = true;
					break;
				}
			}
		}
	}
}
