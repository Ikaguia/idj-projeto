#include <gameObject.hpp>
#include <tileMap.hpp>
#include <collision.hpp>

GameObject::GameObject(float r):rotation{r}{}

void GameObject::CheckCollisionGround(const TileMap &tileMap){
	int x1,y1,x2,y2;
	tileMap.GetIndAtPos(nextBox.x,nextBox.y,x1,y1);
	tileMap.GetIndAtPos(nextBox.x+box.w,nextBox.y+box.h,x2,y2);
	Rect tileBox(0,0,tileMap.GetWidth(),tileMap.GetHeight());
	FOR2(j,y1,y2){
		FOR2(i,x1,x2){
			if(tileMap.AtMeta(i,j)==1){
				tileBox.x=tileMap.GetWidth()*i;
				tileBox.y=tileMap.GetHeight()*j;
				if(Collision::IsColliding(box,tileBox,rotation,0.0f)){
					NotifyCollision(nullptr);
					return;
				}
			}
		}
	}
}
