#include <stateStage.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <resources.hpp>
#include <music.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <componentInputControl.hpp>
#include <componentStaticRender.hpp>
#include <componentMovement.hpp>
#include <componentCollider.hpp>
#include <componentGravity.hpp>
#include <componentHP.hpp>

StateStage::StateStage(string lvl):State::State(), level{Level(lvl,&entities)}{
	LoadAssets();

	player = new GameObject{Rect{130.0f,130.0f,150.0f,250.0f}};
	player->AddComponent(new CompInputControl{[](GameObject* go, float time){
		Vec2 &speed = ((CompMovement*)go->components[Component::type::t_movement])->speed;

		//TODO change this to prevent infinite jump
		if     (INPUTMAN.KeyPress(KEY_UP))speed.y=-1500.0f;
		if     (INPUTMAN.IsKeyDown(KEY_LEFT) && !INPUTMAN.IsKeyDown(KEY_RIGHT))speed.x=max(-400.0f,speed.x-800*time);
		else if(INPUTMAN.IsKeyDown(KEY_RIGHT) && !INPUTMAN.IsKeyDown(KEY_LEFT))speed.x=min( 400.0f,speed.x+800*time);
		else if(speed.x>0.0f)speed.x=max(0.0f,speed.x-800*time);
		else if(speed.x<0.0f)speed.x=min(0.0f,speed.x+800*time);
	

		if(INPUTMAN.KeyPress(KEY(z))){
			cout << "firing arrow" << endl;
			GameObject *arrow = new GameObject{{go->box.x+go->box.w+20,go->box.y,75,10}};
			arrow->AddComponent(new CompStaticRender{Sprite{"img/arrow.png"},Vec2{}});
			float strenght=1000 + (1000-rand()%2000)/10.0f;
			float angle=          (1000-rand()%2000)/100.0f;
			arrow->AddComponent(new CompMovement{Vec2::makeVec2(strenght,angle),CompMovement::moveType::t_bullet});

			CompCollider *collider = new CompCollider{CompCollider::collType::t_bullet};
			collider->useDefault[CompCollider::collType::t_bullet] =
				[](const CompCollider *a,const CompCollider *b){cout << "bullet to bullet collision" << endl;};
			collider->useDefault[CompCollider::collType::t_any] =
				[](const CompCollider *a,const CompCollider *b){
					Vec2 &speed=((CompMovement*)a->entity->components[Component::type::t_movement])->speed;

					if(speed==Vec2{})return;

					Vec2 &totMove=((CompMovement*)a->entity->components[Component::type::t_movement])->move;
					Vec2 move=a->collides(b,totMove,a->entity->box+move);

					if(move!=totMove){
						cout << "bullet " << a->entity->box << " with " << b->entity->box << endl;
						a->entity->dead=true;

						GameObject *arrow = new GameObject{a->entity->box + move + totMove/4.0f};
						arrow->rotation=a->entity->rotation;
						arrow->AddComponent(new CompStaticRender{Sprite{"img/arrow.png"},Vec2{}});
						GAMESTATE.AddObject(arrow);
						arrow->AttachTo(b->entity);

						if(b->entity->hasComponent[Component::type::t_hp]){
							((CompHP*)b->entity->components[Component::type::t_hp])->Damage(7+rand()%6);
						}
					}
				};
			arrow->AddComponent(collider);

			arrow->AddComponent(new CompGravity{500.0f});
			GAMESTATE.AddObject(arrow);
		}
	}});
	player->AddComponent(new CompStaticRender{Sprite{"img/player_static.jpg"},Vec2{}});
	player->AddComponent(new CompMovement{});
	player->AddComponent(new CompCollider{CompCollider::collType::t_player});
	player->AddComponent(new CompGravity{2500.0f});
	player->AddComponent(new CompHP{100,100,true,false});
	AddObject(player);

	GameObject *target = new GameObject{Rect{1000.0f,750.0f,200.0f,250.0f}};
	target->AddComponent(new CompStaticRender{Sprite{"img/target.png"},Vec2{}});
	target->AddComponent(new CompCollider{CompCollider::collType::t_player});
	target->AddComponent(new CompHP{100,100,true,false});
	AddObject(target);
}

StateStage::~StateStage(){}

void StateStage::Update(float time){
	Camera::Update(time);
	if(INPUTMAN.QuitRequested())quitRequested=true;
	if(INPUTMAN.KeyPress(KEY_ESC))popRequested=true;
	UpdateArray(time);
}
void StateStage::Render(){
	level.background.Render(0, 0);
	level.tileMap.Render();
	RenderArray();
}

void StateStage::Pause(){
	Camera::Unfollow();
}
void StateStage::Resume(){
	Camera::Follow(player);
}

void StateStage::LoadAssets(){}
