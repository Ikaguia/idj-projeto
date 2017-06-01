#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <stateStage.hpp>

GameObject::GameObject(){}
GameObject::GameObject(const Rect &rec,float r,bool a):box{rec},rotation{r},anchored{a}{}
GameObject::~GameObject(){
	UnAttach();
	for(GameObject* obj:attachedObjs)obj->dead=true;

	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];
	if(Camera::focus==this)Camera::Unfollow();
}

void GameObject::Update(float time){
	//reset move
	if(hasComponent[Component::type::t_movement])((CompMovement*)components[Component::type::t_movement])->move=0.0f;
	//process input control and ai first
	if(hasComponent[Component::type::t_input_control])components[Component::type::t_input_control]->Update(time);
	if(hasComponent[Component::type::t_ai])components[Component::type::t_ai]->Update(time);
	//then set move
	if(hasComponent[Component::type::t_movement]){
		CompMovement *compM = (CompMovement*) components[Component::type::t_movement];
		compM->move+=compM->speed*time;
	}
	//and then do the rest
	FOR2(i,Component::type::t__+1,Component::type::t_count)if(hasComponent[i])components[i]->Update(time);
}

void GameObject::Render(){
	FOR(i,Component::type::t_count){
		if(hasComponent[i]){
			components[i]->Render();
		}
	}
}

void GameObject::AddComponent(Component* component){
	auto t=component->GetType();
	if(hasComponent[t])cout << "Error, adding component " << t << " to a GameObject that already has it" << endl;
	hasComponent[t]=true;
	components[t]=component;
	component->Own(this);
}

void GameObject::ReplaceComponent(Component* component){
	auto t=component->GetType();
	if(!hasComponent[t]){
		cout << "Error, replacing component " << t << " on a GameObject that doesnt have it" << endl;
		hasComponent[t]=true;
	}
	else delete components[t];
	components[t]=component;
	component->Own(this);
}

void GameObject::RemoveComponent(Component::type t){
	if(!hasComponent[t])cout << "Error, removing component " << t << " on a GameObject that doesnt have it" << endl;
	else{
		delete components[t];
		components[t]=nullptr;
		hasComponent[t]=false;
	}
}

void GameObject::SetComponent(Component::type t,Component* component){
	if(!hasComponent[t])hasComponent[t]=true;
	components[t]=component;
}


void GameObject::AttachObj(GameObject* obj){
	if(find(attachedObjs.begin(), attachedObjs.end(),obj)==attachedObjs.end()){
		attachedObjs.push_back(obj);
		obj->AttachTo(this);
	}
}
void GameObject::AttachTo(GameObject* obj){
	if(attachedTo==nullptr){
		attachedTo=obj;
		obj->AttachObj(this);
	}
	else if(attachedTo!=obj){
		UnAttach();
		AttachTo(obj);
	}
}
void GameObject::UnAttachObj(GameObject* obj){
	auto it=find(attachedObjs.begin(), attachedObjs.end(),obj);
	if(it!=attachedObjs.end()){
		attachedObjs.erase(it);
		obj->UnAttach();
	}
}
void GameObject::UnAttach(){
	if(attachedTo!=nullptr){
		auto temp=attachedTo;
		attachedTo=nullptr;
		temp->UnAttachObj(this);
	}
}


bool GameObject::IsDead()const{
	return false;
}




void PlayerControlFunc(GameObject* go, float time){
	Vec2 &speed = ((CompMovement*)go->components[Component::type::t_movement])->speed;

	//TODO change this to prevent infinite jump
	if(INPUTMAN.KeyPress(KEY_UP))speed.y=-1500.0f;
	if(INPUTMAN.IsKeyDown(KEY_LEFT) && !INPUTMAN.IsKeyDown(KEY_RIGHT)){
		speed.x=max(-400.0f,speed.x-800*time);
		go->flipped=true;
	}
	else if(INPUTMAN.IsKeyDown(KEY_RIGHT) && !INPUTMAN.IsKeyDown(KEY_LEFT)){
		speed.x=min( 400.0f,speed.x+800*time);
		go->flipped=false;
	}
	else if(speed.x>0.0f)speed.x=max(0.0f,speed.x-800*time);
	else if(speed.x<0.0f)speed.x=min(0.0f,speed.x+800*time);


	if(INPUTMAN.KeyPress(KEY(z))){
		//TODO: prevent firing arrows inside other objects
		float force=1000 + (1000-rand()%2000)/10.0f;
		float angle=       (1000-rand()%2000)/100.0f;
		if(go->flipped){
			Vec2 pos{go->box.x-150-20,go->box.y};
			GAMESTATE.AddObject(GameObject::MakeBullet(pos,"img/arrow.png",force,180+angle,true));
		}
		else{
			Vec2 pos{go->box.x+go->box.w+20,go->box.y};
			GAMESTATE.AddObject(GameObject::MakeBullet(pos,"img/arrow.png",force,angle,true));
		}
	}
}
void PlayerMonsterCollision(const CompCollider* a,const CompCollider* b){
	Vec2 &speed=((CompMovement*)a->entity->components[Component::type::t_movement])->speed;
	if(speed==Vec2{})return;

	Vec2 &totMove=((CompMovement*)a->entity->components[Component::type::t_movement])->move;
	Vec2 move=a->collides(b,totMove,a->entity->box+move);

	if(move!=totMove)((CompHP*)a->entity->components[Component::type::t_hp])->Damage(1);
}
void EmptyCollision(const CompCollider* a,const CompCollider* b){}
GameObject* GameObject::MakePlayer(const Vec2 &pos){

	CompAnimControl* animControl = new CompAnimControl{"animation/player.txt"};
	float width=animControl->GetCur().sp.GetWidth();
	float height=animControl->GetCur().sp.GetHeight();

	GameObject* player = new GameObject{Rect{pos.x,pos.y,width,height}};

	player->AddComponent(animControl);
	player->AddComponent(new CompInputControl{PlayerControlFunc});
	player->AddComponent(new CompMovement{});

	CompCollider *coll = new CompCollider{CompCollider::collType::t_player};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=PlayerMonsterCollision;
	player->AddComponent(coll);

	player->AddComponent(new CompGravity{2500.0f});
	player->AddComponent(new CompHP{100,100,true,false,1.0f});
	return player;
}


GameObject* GameObject::MakeTarget(const Vec2 &pos){
	CompStaticRender* img = new CompStaticRender{Sprite{"img/target.png"},Vec2{}};
	GameObject* target = new GameObject{Rect{pos.x,pos.y,(float)img->sp.GetWidth(),(float)img->sp.GetHeight()}};
	target->AddComponent(img);
	target->AddComponent(new CompCollider{CompCollider::collType::t_solid});
	target->AddComponent(new CompHP{100,100,true,false});
	return target;
}


void BulletAnyCollision1(const CompCollider* a,const CompCollider* b){
	Vec2 &speed=((CompMovement*)a->entity->components[Component::type::t_movement])->speed;
	if(speed==Vec2{})return;

	Vec2 &totMove=((CompMovement*)a->entity->components[Component::type::t_movement])->move;
	Vec2 move=a->collides(b,totMove,a->entity->box+move);

	if(move!=totMove){
		a->entity->dead=true;

		if(!b->entity->hasComponent[Component::type::t_movement]){
			GameObject *arrow = new GameObject{a->entity->box + move + totMove/4.0f};
			arrow->rotation=a->entity->rotation;
			arrow->AddComponent(new CompStaticRender{Sprite{"img/arrow.png"},Vec2{}});
			GAMESTATE.AddObject(arrow);
			arrow->AttachTo(b->entity);
		}

		if(b->entity->hasComponent[Component::type::t_hp]){
			((CompHP*)b->entity->components[Component::type::t_hp])->Damage(7+rand()%6);
		}
	}
}
void BulletAnyCollision2(const CompCollider* a,const CompCollider* b){
	Vec2 &speed=((CompMovement*)a->entity->components[Component::type::t_movement])->speed;

	if(speed==Vec2{})return;

	Vec2 &totMove=((CompMovement*)a->entity->components[Component::type::t_movement])->move;
	Vec2 move=a->collides(b,totMove,a->entity->box+move);

	if(move!=totMove){
		a->entity->dead=true;
		if(b->entity->hasComponent[Component::type::t_hp]){
			((CompHP*)b->entity->components[Component::type::t_hp])->Damage(7+rand()%6);
		}
	}
}
GameObject* GameObject::MakeBullet(const Vec2 &pos,string image,float force,float angle,bool stick){
	CompStaticRender* img = new CompStaticRender{Sprite{image},Vec2{}};
	float width=img->sp.GetWidth();
	float height=img->sp.GetHeight();

	GameObject *arrow = new GameObject{{pos.x,pos.y,width,height}};
	arrow->rotation=angle;
	arrow->AddComponent(img);
	arrow->AddComponent(new CompMovement{Vec2::makeVec2(force,angle),CompMovement::moveType::t_bullet});

	CompCollider *collider = new CompCollider{CompCollider::collType::t_bullet};
	collider->useDefault[CompCollider::collType::t_bullet] = EmptyCollision;
	if(stick)collider->useDefault[CompCollider::collType::t_any] = BulletAnyCollision1;
	else     collider->useDefault[CompCollider::collType::t_any] = BulletAnyCollision2;
	arrow->AddComponent(collider);

	arrow->AddComponent(new CompGravity{500.0f});
	return arrow;
}


void MikeAIfunc(CompAI* ai,float time){
	if(ai->states[0]==CompAI::state::idling){
		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
		}
	}
	else if(ai->states[0]==CompAI::state::looking){
		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
		}
		//TODO: make line of sight component
		else if(ai->entity->box.corner().dist(PLAYER->box.corner())<1000){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::walking;
			ai->targetPOS[0]=PLAYER->box.corner();
			((CompAnimControl*)ai->entity->components[Component::type::t_animation_control])->ChangeCur("walk");
		}
	}
	else if(ai->states[0]==CompAI::state::walking){
		float dist = ai->targetPOS[0].x-ai->entity->box.x;
		CompMovement *movement = ((CompMovement*)ai->entity->components[Component::type::t_movement]);
		CompAnimControl* ac = ((CompAnimControl*)ai->entity->components[Component::type::t_animation_control]);

		if(abs(dist)<abs(movement->speed.x)*time){
			movement->speed.x=0;
			movement->move=dist;

			ai->states[0]=CompAI::state::idling;
			ai->timers[0].Restart();
			ac->ChangeCur("idle");
		}
		else if(dist>0){
			ai->entity->flipped=true;
			movement->speed.x= 100.0f;
		}
		else{
			ai->entity->flipped=false;
			movement->speed.x=-100.0f;
		}
	}
	else if(ai->states[0]==CompAI::state::attacking){
		if(ai->targetGO[0]==nullptr){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
		}
		else if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
		}
	}
}
GameObject* GameObject::MakeMike(const Vec2 &pos){

	CompAnimControl* animControl = new CompAnimControl{"animation/mike.txt"};
	float width=animControl->GetCur().sp.GetWidth();
	float height=animControl->GetCur().sp.GetHeight();

	GameObject* mike = new GameObject{Rect{pos.x,pos.y,width,height}};

	mike->AddComponent(animControl);

	CompCollider *coll = new CompCollider{CompCollider::collType::t_monster};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=EmptyCollision;
	mike->AddComponent(coll);

	mike->AddComponent(new CompMovement{});
	mike->AddComponent(new CompGravity{2500.0f});
	mike->AddComponent(new CompHP{100,100,true,false});
	mike->AddComponent(new CompAI{MikeAIfunc,1,1,1,1});
	return mike;
}


void BansheeAIfunc(CompAI* ai,float time){
	if(ai->states[0]==CompAI::state::idling) {
		if(ai->timers[0].Get()>0.5f){
			ai->states[0]=CompAI::state::walking;
			ai->states[1]=!ai->states[1];
			((CompAnimControl*)ai->entity->components[Component::type::t_animation_control])->ChangeCur("walk");
		}
	}
	else if(ai->states[0]==CompAI::state::walking){
		float dist = ai->targetPOS[ai->states[1]].x-ai->entity->box.x;
		CompMovement *movement = ((CompMovement*)ai->entity->components[Component::type::t_movement]);
		CompAnimControl* ac = ((CompAnimControl*)ai->entity->components[Component::type::t_animation_control]);

		if(abs(dist)<abs(movement->speed.x)*time){
			movement->speed.x=0;
			movement->move=dist;

			ai->states[0]=CompAI::state::idling;
			ai->timers[0].Restart();
			ac->ChangeCur("idle");
		}
		else if(dist>0){
			ai->entity->flipped=true;
			movement->speed.x= 100.0f;
		}
		else{
			ai->entity->flipped=false;
			movement->speed.x=-100.0f;
		}
	}
}
GameObject* GameObject::MakeBanshee(const Vec2 &pos,const Vec2 &pos2){
	CompAnimControl* animControl = new CompAnimControl{"animation/banshee.txt"};
	float width=animControl->GetCur().sp.GetWidth();
	float height=animControl->GetCur().sp.GetHeight();

	GameObject* banshee = new GameObject{Rect{pos.x,pos.y,width,height-10}};
	banshee->AddComponent(animControl);

	CompCollider *coll = new CompCollider{CompCollider::collType::t_monster};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=EmptyCollision;
	banshee->AddComponent(coll);

	banshee->AddComponent(new CompMovement{});
	banshee->AddComponent(new CompGravity{2500.0f});
	// banshee->AddComponent(new CompHP{100,100,true,false});

	CompAI* ai = new CompAI{BansheeAIfunc,2,1,2};
	ai->targetPOS[0]=pos;
	ai->targetPOS[1]=pos2;
	banshee->AddComponent(ai);

	return banshee;
}

#define MASK_FIRE_DIST 200
#define MASK_SEE_DIST 500
void MaskAIfunc(CompAI* ai,float time){
	CompAnimControl* ac = ((CompAnimControl*)ai->entity->components[Component::type::t_animation_control]);
	cout << "mask ai begin" << endl;
	if(ai->states[0]==CompAI::state::idling){
		cout << "mask ai idling" << endl;
		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
		}
	}
	else if(ai->states[0]==CompAI::state::looking){
		cout << "mask ai looking" << endl;
		float dist = ai->entity->box.corner().dist(PLAYER->box.corner());

		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
		}
		//TODO: make line of sight component
		else if(dist<MASK_SEE_DIST){
			ai->timers[0].Restart();
			ai->targetGO[0]=PLAYER;
			if(dist<MASK_FIRE_DIST)ai->states[0]=CompAI::state::attacking;
			else                   ai->states[0]=CompAI::state::walking;
		}
	}
	else if(ai->states[0]==CompAI::state::walking){
		cout << "mask ai walking" << endl;
		if(ai->targetGO[0]==nullptr || ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
		}
		else{
			float dist = ai->targetGO[0]->box.x-ai->entity->box.x;
			CompMovement *movement = ((CompMovement*)ai->entity->components[Component::type::t_movement]);

			//TODO: if cant see target go looking
			if(abs(dist) < MASK_FIRE_DIST+abs(movement->speed.x)*time){
				movement->speed.x=0;
				if(dist>0)movement->move=dist-MASK_FIRE_DIST;
				else      movement->move=dist+MASK_FIRE_DIST;

				ai->states[0]=CompAI::state::attacking;
				ai->timers[0].Restart();
				ac->ChangeCur("attack");
			}
			else if(dist>0){
				ai->entity->flipped=true;
				movement->speed.x= 100.0f;
			}
			else{
				ai->entity->flipped=false;
				movement->speed.x=-100.0f;
			}
		}
	}
	else if(ai->states[0]==CompAI::state::attacking){
		cout << "mask ai attacking" << endl;
		if(ai->targetGO[0]==nullptr){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
			ac->ChangeCur("walk");
		}
		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
			ac->ChangeCur("idle");
		}
		//fire projectile
	}
	cout << "mask ai end" << endl;
}
GameObject* GameObject::MakeMask(const Vec2 &pos){
	CompAnimControl* animControl = new CompAnimControl{"animation/mascara.txt"};
	float width=animControl->GetCur().sp.GetWidth();
	float height=animControl->GetCur().sp.GetHeight();

	GameObject* mask = new GameObject{Rect{pos.x,pos.y,width,height-10}};
	mask->AddComponent(animControl);

	CompCollider *coll = new CompCollider{CompCollider::collType::t_monster};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=EmptyCollision;
	mask->AddComponent(coll);

	mask->AddComponent(new CompMovement{});
	// mask->AddComponent(new CompGravity{2500.0f});
	mask->AddComponent(new CompHP{50,50,true,false});
	mask->AddComponent(new CompAI{MaskAIfunc,1,1,0,1});

	return mask;
}
