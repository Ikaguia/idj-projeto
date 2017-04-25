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
		speed.x=max(-WALK_SPEED,speed.x-(WALK_SPEED*time/0.25));
	}
	else if(INPUTMAN.IsKeyDown(D_KEY)){
		speed.x=min( WALK_SPEED,speed.x+(WALK_SPEED*time/0.25));
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

	//pega o intervalo de tiles pra checar
	tileMap.GetIndAtPos(min(curBox.x,nextBox.x),         min(curBox.y,nextBox.y),         x1,y1);
	tileMap.GetIndAtPos(max(curBox.x,nextBox.x)+curBox.w,max(curBox.y,nextBox.y)+curBox.h,x2,y2);
	system("clear");
	FOR(j,tileMap.GetHeight()){
		FOR(i,tileMap.GetWidth()){
			if(BETWEEN(i,x1,x2+1) && BETWEEN(j,y1,y2+1))cout << 1;
			else cout << 0;
			cout << tileMap.AtMeta(i,j) << " ";
		}
		cout << endl;
	}
	//cria o poligono do movimento do player
	ConvexPolygon move,moveLine;
	moveLine.AddPoint({});
	moveLine.AddPoint(nextBox.center()-curBox.center());
	cout << curBox.corner() << " " << nextBox.corner() << endl;	
	cout << "moveLine = " << moveLine << endl;
	cout << "playerPol = " << curBox.polygon(rotation) << endl;
	move=moveLine.MinkowskySum(curBox.polygon(rotation));

	cout << "move = " << move << endl;
	moveLine.RemovePoint(nextBox.center());

	pair<int,int> ii;
	if(tileMap.Collides(x1,x2,y1,y2,move,ii)){//se colidiu
		//acha o ponto onde o player ira ficar apos a colisao
		//busca binaria pra achar a ulima posição onde não ocorre colisão
		Vec2 inPos=curBox.corner(),endPos=nextBox.corner(),midPos;
		FOR(i,100){
			midPos=(inPos+endPos)/2.0f;
			moveLine.AddPoint(midPos);
			move=moveLine.MinkowskySum(curBox.polygon(rotation));
			moveLine.RemovePoint(midPos);
			if(tileMap.Collides(x1,x2,y1,y2,move,ii))endPos=midPos;
			else inPos=midPos;

			if(inPos==endPos){
				midPos=inPos;
				break;
			}
		}
		//Vec2 oldPos=box.corner();
		curBox.x=midPos.x;
		curBox.y=midPos.y;

		//atualiza a velocidade e a posição
		//x
		Vec2 moveX(0.00125f,0.0f);
		moveLine.AddPoint(moveX);
		move=moveLine.MinkowskySum(curBox.polygon(rotation));
		moveLine.RemovePoint(moveX);

		tileMap.GetIndAtPos(min(curBox.x,nextBox.x),      curBox.y,         x1,y1);
		tileMap.GetIndAtPos(max(curBox.x,nextBox.x)+box.w,curBox.y+curBox.h,x2,y2);
		if(tileMap.Collides(x1,x2,y1,y2,move,ii))speed.x=0.0f;
		else{//move the rest
			float lowX=0.0f,highX=speed.x,midX;
			FOR(i,100){
				midX=(lowX+highX)/2.0f;
				moveX.x=midX;
				moveLine.AddPoint(moveX);
				move=moveLine.MinkowskySum(curBox.polygon(rotation));
				moveLine.RemovePoint(moveX);
				if(tileMap.Collides(x1,x2,y1,y2,move,ii))highX=midX;
				else lowX=midX;

				if(equals(lowX,highX)){
					midX=lowX;
					break;
				}
			}
			speed.x=midX;
			curBox.x+=midX;
		}
		//y
		Vec2 moveY(0.0f,0.00125f);
		moveLine.AddPoint(moveY);
		move=moveLine.MinkowskySum(curBox.polygon(rotation));
		moveLine.RemovePoint(moveY);

		tileMap.GetIndAtPos(curBox.x,         min(curBox.y,nextBox.y),         x1,y1);
		tileMap.GetIndAtPos(curBox.x+curBox.w,max(curBox.y,nextBox.y)+curBox.h,x2,y2);
		if(tileMap.Collides(x1,x2,y1,y2,move,ii)){
			onAir=speed.y<0.0f;//if its falling
			speed.y=0.0f;
		}
		else{//move the rest
			float lowY=0.0f,highY=speed.y,midY;
			FOR(i,100){
				midY=(lowY+highY)/2.0f;
				moveY.y=midY;
				moveLine.AddPoint(moveY);
				move=moveLine.MinkowskySum(curBox.polygon(rotation));
				moveLine.RemovePoint(moveY);
				if(tileMap.Collides(x1,x2,y1,y2,move,ii))highY=midY;
				else lowY=midY;

				if(equals(lowY,highY)){
					midY=lowY;
					break;
				}
			}
			speed.y=midY;
			curBox.y+=midY;
		}
		nextBox=curBox;
	}
}
