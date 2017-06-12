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
	Vec2 &speed = COMPMOVEp(go)->speed;

	//TODO change this to prevent infinite jump
	if(INPUT.KeyPress(KEY_UP))speed.y=-1500.0f;
	if(INPUT.IsKeyDown(KEY_LEFT) && !INPUT.IsKeyDown(KEY_RIGHT)){
		speed.x=max(-400.0f,speed.x-800*time);
		go->flipped=false;
	}
	else if(INPUT.IsKeyDown(KEY_RIGHT) && !INPUT.IsKeyDown(KEY_LEFT)){
		speed.x=min( 400.0f,speed.x+800*time);
		go->flipped=true;
	}
	else if(speed.x>0.0f)speed.x=max(0.0f,speed.x-800*time);
	else if(speed.x<0.0f)speed.x=min(0.0f,speed.x+800*time);


	static Timer t;
	t.Update(time);
	if(t.Get()>1)COMPANIMCONTp(go)->ChangeCur("idle");
	if(INPUT.KeyPress(KEY(z)) && t.Get()>1){
		COMPANIMCONTp(go)->ChangeCur("walk");
		t.Restart();
		//TODO: prevent firing arrows inside other objects
		float force=1000 + (1000-rand()%2000)/10.0f;
		float angle=       (1000-rand()%2000)/100.0f;
		if(!go->flipped){
			Vec2 pos{go->box.x-150-5,go->box.y+85};
			GAMESTATE.AddObject(GameObject::MakeBullet(pos,"player_projectile.txt",go,force,180+angle+10,10,16,true));
		}
		else{
			Vec2 pos{go->box.x+go->box.w+5,go->box.y+85};
			GAMESTATE.AddObject(GameObject::MakeBullet(pos,"player_projectile.txt",go,force,angle-10,10,16,true));
		}
	}
}
void PlayerMonsterCollision(const CompCollider* a,const CompCollider* b){
	Vec2 &speed=COMPMOVEp(a->entity)->speed;
	if(speed==Vec2{})return;

	Vec2 &totMove=COMPMOVEp(a->entity)->move;
	Vec2 move=a->collides(b,totMove,a->entity->box+move);

	if(move!=totMove)COMPHPp(a->entity)->Damage(1);
}
void EmptyCollision(const CompCollider* a,const CompCollider* b){UNUSED(a);UNUSED(b);}
GameObject* GameObject::MakePlayer(const Vec2 &pos){

	CompAnimControl* animControl = new CompAnimControl{"data/animation/player.txt"};
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
	player->AddComponent(new CompHP{100,100,true,false,0.25f});

	player->flipped=true;
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


GameObject* GameObject::MakeBullet(const Vec2 &pos,string animFile,GameObject* go,float force,float angle,int dmgLow,int dmgHigh,bool stick){
	dmgHigh=max(dmgHigh,dmgLow+1);

	CompAnim* anim = new CompAnim{animFile};
	float width=anim->sp.GetWidth();
	float height=anim->sp.GetHeight();

	GameObject *arrow = new GameObject{{pos.x,pos.y,width,height}};
	arrow->flipped=true;
	arrow->rotation=angle;
	arrow->AddComponent(anim);
	arrow->AddComponent(new CompMovement{Vec2::makeVec2(force,angle),CompMovement::moveType::t_bullet});

	CompCollider *collider = new CompCollider{CompCollider::collType::t_bullet};
	collider->useDefault[CompCollider::collType::t_bullet] = EmptyCollision;

	auto foo = [stick,go,dmgLow,dmgHigh](const CompCollider* a,const CompCollider* b){
		if(go==b->entity)return;

		Vec2 &speed=COMPMOVEp(a->entity)->speed;
		if(speed==Vec2{})return;

		Vec2 &totMove=COMPMOVEp(a->entity)->move;
		Vec2 move=a->collides(b,totMove,a->entity->box+move);

		if(move!=totMove){
			a->entity->dead=true;

			if(stick && !b->entity->hasComponent[Component::type::t_movement]){
				GameObject *arrow = new GameObject{a->entity->box + move + totMove/4.0f};
				arrow->flipped=true;
				arrow->rotation=a->entity->rotation;
				Sprite sp = COMPANIMp(a->entity)->sp;
				sp.SetFrameTime(-1.0f);
				arrow->AddComponent(new CompStaticRender{sp,Vec2{}});
				GAMESTATE.AddObject(arrow);
				arrow->AttachTo(b->entity);
			}

			if(b->entity->hasComponent[Component::type::t_hp]){
				int dmg = dmgLow+rand()%(dmgHigh-dmgLow);
				COMPHPp(b->entity)->Damage(dmg);
			}
		}
	};
	collider->useDefault[CompCollider::collType::t_any] = foo;
	arrow->AddComponent(collider);

	arrow->AddComponent(new CompGravity{500.0f});
	return arrow;
}


#define MIKE_ATTACK_DIST 5
#define MIKE_SEE_DIST 500
void MikeAIfunc(CompAI* ai,float time){
	CompAnimControl* ac = COMPANIMCONTp(ai->entity);
	if(ai->states[0]==CompAI::state::idling){
		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
		}
	}
	else if(ai->states[0]==CompAI::state::looking){
		float dist = ai->entity->box.distEdge(ai->targetGO[0]->box).x;

		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
		}
		//TODO: make line of sight component
		else if(dist<MIKE_SEE_DIST){
			ai->timers[0].Restart();
			if(dist<MIKE_ATTACK_DIST)ai->states[0]=CompAI::state::attacking,ac->ChangeCur("attack");
			else ai->states[0]=CompAI::state::walking,ac->ChangeCur("walk");
		}
	}
	else if(ai->states[0]==CompAI::state::walking){
		if(ai->targetGO[0]==nullptr || ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			float dist = ai->entity->box.distEdge(ai->targetGO[0]->box).x;
			CompMovement *movement = COMPMOVEp(ai->entity);

			//TODO: if cant see target go looking
			if(dist < MIKE_ATTACK_DIST+abs(movement->speed.x)*time){
				movement->speed.x=0;
				if(ai->entity->box.x < ai->targetGO[0]->box.x)movement->move=dist-MIKE_ATTACK_DIST;
				else      movement->move=-dist+MIKE_ATTACK_DIST;

				ai->states[0]=CompAI::state::attacking;
				ai->timers[0].Restart();
				ac->ChangeCur("attack");
			}
			else if(ai->entity->box.x < ai->targetGO[0]->box.x){
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
		if(ai->targetGO[0]==nullptr || ai->entity->box.distEdge(ai->targetGO[0]->box).x > MIKE_ATTACK_DIST){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
			ac->ChangeCur("walk");
		}
		if(ai->timers[0].Get()>5){
			if(ai->targetGO[0]->box.x > ai->entity->box.x && !ai->entity->flipped)ai->entity->flipped=true;
			if(ai->targetGO[0]->box.x < ai->entity->box.x &&  ai->entity->flipped)ai->entity->flipped=false;
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
			ac->ChangeCur("idle");
		}
	}
}
GameObject* GameObject::MakeMike(const Vec2 &pos){

	CompAnimControl* animControl = new CompAnimControl{"data/animation/mike.txt"};
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

	CompAI* ai = new CompAI{MikeAIfunc,1,1,1,1};
	ai->targetGO[0]=PLAYER;
	mike->AddComponent(ai);
	return mike;
}


void BansheeAIfunc(CompAI* ai,float time){
	if(ai->states[0]==CompAI::state::idling) {
		if(ai->timers[0].Get()>0.5f){
			ai->states[0]=CompAI::state::walking;
			ai->states[1]=!ai->states[1];
			COMPANIMCONTp(ai->entity)->ChangeCur("walk");
		}
	}
	else if(ai->states[0]==CompAI::state::walking){
		float dist = ai->targetPOS[ai->states[1]].x-ai->entity->box.x;
		CompMovement *movement = COMPMOVEp(ai->entity);
		CompAnimControl* ac = COMPANIMCONTp(ai->entity);

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
	CompAnimControl* animControl = new CompAnimControl{"data/animation/banshee.txt"};
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

#define MASK_ATTACK_DIST 500
#define MASK_SEE_DIST 1000
void MaskAIfunc(CompAI* ai,float time){
	CompAnimControl* ac = COMPANIMCONTp(ai->entity);
	if(ai->states[0]==CompAI::state::idling){
		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
		}
	}
	else if(ai->states[0]==CompAI::state::looking){
		float dist = ai->entity->box.distEdge(ai->targetGO[0]->box).x;

		if(ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::idling;
		}
		//TODO: make line of sight component
		else if(dist<MASK_SEE_DIST){
			ai->timers[0].Restart();
			if(dist<MASK_ATTACK_DIST)ai->states[0]=CompAI::state::attacking,ac->ChangeCur("attack");
			else ai->states[0]=CompAI::state::walking,ac->ChangeCur("walk");
		}
	}
	else if(ai->states[0]==CompAI::state::walking){
		if(ai->targetGO[0]==nullptr || ai->timers[0].Get()>5){
			ai->timers[0].Restart();
			ai->states[0]=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			float dist = ai->entity->box.distEdge(ai->targetGO[0]->box).x;
			CompMovement *movement = COMPMOVEp(ai->entity);

			//TODO: if cant see target go looking
			if(abs(dist) < MASK_ATTACK_DIST+abs(movement->speed.x)*time){
				movement->speed.x=0;
				if(ai->targetGO[0]->box.x>ai->entity->box.x)movement->move=dist-MASK_ATTACK_DIST;
				else movement->move=-dist+MASK_ATTACK_DIST;

				ai->states[0]=CompAI::state::attacking;
				ai->timers[0].Restart();
				ac->ChangeCur("attack");
			}
			else if(ai->targetGO[0]->box.x>ai->entity->box.x){
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
	}
}
GameObject* GameObject::MakeMask(const Vec2 &pos){
	CompAnimControl* animControl = new CompAnimControl{"data/animation/mascara.txt"};
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

	CompAI *ai = new CompAI{MaskAIfunc,1,1,0,1};
	ai->targetGO[0]=PLAYER;
	mask->AddComponent(ai);

	return mask;
}
