#include <player.hpp>
#include <inputManager.hpp>
#include <camera.hpp>
#include <game.hpp>
#include <state.hpp>
#include <bullet.hpp>
#include <animation.hpp>
#include <collision.hpp>


#define SMALL 0.00125f


Player* Player::player=nullptr;

Player::Player(float x,float y):WalkingEntity::WalkingEntity(),sp{RESOURCESFOLDER+"img/monstro1-sprites.png",8,0.125f},speed{0.0f,0.0f},hp{30}{
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
	sp.Update(time);

	cout << "onGround " << onGround << endl;
	cout << "onTop " << onTop << endl;
	// cout << "onLeft " << onLeft << endl;
	// cout << "onRight " << onRight << endl;

	if(onGround)doubleJump=true;

	if(INPUTMAN.IsKeyDown(W_KEY) && onGround && !onTop){
		speed.y=-PLAYER_JUMP;
		onGround=false;
		cout << "pulou " << speed.y << endl;
	}
	else if(INPUTMAN.KeyPress(W_KEY) && !onGround && !onTop && doubleJump){
		speed.y=-PLAYER_JUMP;
		doubleJump=false;
		cout << "pulou2 " << speed.y << endl;
	}
	if((INPUTMAN.IsKeyDown(A_KEY) && INPUTMAN.IsKeyDown(D_KEY)));
	else if((!INPUTMAN.IsKeyDown(A_KEY) && !INPUTMAN.IsKeyDown(D_KEY))){
		speed.x/=1+(20*time);
		if(equals(speed.x,0.0f))speed.x=0.0f;
	}
	else if(INPUTMAN.IsKeyDown(A_KEY) && !onLeft){
		if(equals(speed.x,0.0f))speed.x=-WALK_SPEED/4;
		else speed.x=max(-WALK_SPEED,speed.x-(WALK_SPEED*time/0.25));
	}
	else if(INPUTMAN.IsKeyDown(D_KEY) && !onRight){
		if(equals(speed.x,0.0f))speed.x=WALK_SPEED/4;
		else speed.x=min( WALK_SPEED,speed.x+(WALK_SPEED*time/0.25));
	}

	if(INPUTMAN.MousePress(RIGHT_MOUSE_BUTTON))speed=Vec2(0.0f,0.0f);

	cout << "speed = " << speed << endl;
	nextBox=box+(speed)*METER*time;
}
void Player::UpdatePos(float time){
	box=nextBox;
}
void Player::UpdatePhysics(float time){
	if(!onGround){
		if((onLeft && INPUTMAN.IsKeyDown(A_KEY)) || (onRight && INPUTMAN.IsKeyDown(D_KEY)))speed.y+=(GRAVITY*time)/2.0f;
		else speed.y+=(GRAVITY*time);
		// if((speed.y+(GRAVITY*time))<0)speed.y+=(GRAVITY*time);
	}
	else speed.y=0.0f;
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
	int x1,x2,y1,y2;
	Rect curBox=box;
	ii ret;

	//pega o intervalo de tiles pra checar
	tileMap.GetIndAtPos(min(curBox.x,nextBox.x),         min(curBox.y,nextBox.y),         x1,y1);
	tileMap.GetIndAtPos(max(curBox.x,nextBox.x)+curBox.w,max(curBox.y,nextBox.y)+curBox.h,x2,y2);
	x1--;y1--;x2+=2;y2+=2;//checa um quadrado a mais em cada direção, para evitar erros de precisão

	//se a posição atual do player ja colide
	if(tileMap.Collides(x1,x2,y1,y2,curBox.polygon(rotation),ret)){
		cout << "buggg" << endl;
	}


	//cria o poligono do movimento do player
	Vec2 move=nextBox.corner()-curBox.corner();
	ConvexPolygon movePol=(move.polygon()).MinkowskySum(curBox.polygon(rotation));
	//cout << "movePol = " << move.polygon() << " + " << curBox.polygon(rotation) << " = " << movePol << endl;

	//se o movimento completo colide
	if(tileMap.Collides(x1,x2,y1,y2,movePol,ret)){
		Vec2 pos1,pos2,pos3=move,safe;
		//busca binaria para achar a ultima posição onde nao colide
		FOR(i,100){
			pos2=(pos1+pos3)/2.0f;
			pos2.floor();
			movePol=(pos2.polygon()).MinkowskySum(curBox.polygon(rotation));
			if(tileMap.Collides(x1,x2,y1,y2,movePol,ret))pos3=pos2;
			else{
				pos1=pos2;
				safe=pos2;
			}
		 if(pos1==pos3)break;
		}
		curBox+=safe;

		Vec2 moveX{nextBox.x-curBox.x,0.0f};
		movePol=(moveX.polygon()).MinkowskySum(curBox.polygon(rotation));
		if(tileMap.Collides(x1,x2,y1,y2,movePol,ret)){
			pos1=pos2=safe=Vec2{};
			pos3=moveX;
			FOR(i,100){
				pos2=(pos1+pos3)/2.0f;
				movePol=(pos2.polygon()).MinkowskySum(curBox.polygon(rotation));
				if(tileMap.Collides(x1,x2,y1,y2,movePol,ret))pos3=pos2;
				else{
					pos1=pos2;
					safe=pos2;
				}
			}
			curBox+=safe;
			speed.x=0.0f;
		}
		else curBox.x=nextBox.x;

		Vec2 moveY{0.0f,nextBox.y-curBox.y};
		movePol=(moveY.polygon()).MinkowskySum(curBox.polygon(rotation));
		if(tileMap.Collides(x1,x2,y1,y2,movePol,ret)){
			cout << "aaaa " << pos2.y << endl;
			pos1=pos2=safe=Vec2{};
			pos3=moveY;
			FOR(i,100){
				pos2=(pos1+pos3)/2.0f;
				pos2.floor();
				movePol=(pos2.polygon()).MinkowskySum(curBox.polygon(rotation));
				if(tileMap.Collides(x1,x2,y1,y2,movePol,ret))pos3=pos2;
				else{
					pos1=pos2;
					safe=pos2;
				}
			}
			curBox+=safe;
			speed.y=0.0f;
		}
		else curBox.y=nextBox.y;

		curBox.floor();
		nextBox=curBox;
		speed.x=0.0f;
	}
	if(box.x < nextBox.x)onRight=false;
	else{
		Vec2 move{SMALL,0.0f};
		movePol=(move.polygon()).MinkowskySum(curBox.polygon(rotation));
		onRight=tileMap.Collides(x1,x2,y1,y2,movePol,ret);
	}
	if(box.x > nextBox.x)onLeft=false;
	else{
		Vec2 move{-SMALL,0.0f};
		movePol=(move.polygon()).MinkowskySum(curBox.polygon(rotation));
		onLeft=tileMap.Collides(x1,x2,y1,y2,movePol,ret);
	}
	if(box.y < nextBox.y)onGround=false;
	else{
		Vec2 move{0.0f,SMALL};
		movePol=(move.polygon()).MinkowskySum(curBox.polygon(rotation));
		onGround=tileMap.Collides(x1,x2,y1,y2,movePol,ret);
	}
	if(box.y > nextBox.y)onTop=false;
	else{
		Vec2 move{0.0f,-SMALL};
		movePol=(move.polygon()).MinkowskySum(curBox.polygon(rotation));
		onTop=tileMap.Collides(x1,x2,y1,y2,movePol,ret);
	}
}
