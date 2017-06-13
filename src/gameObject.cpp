#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <stateStage.hpp>

GameObject::GameObject():uid{GAMESTATE.GetUID()}{}
GameObject::GameObject(const Rect &rec,float r,bool a):uid{GAMESTATE.GetUID()},box{rec},rotation{r},anchored{a}{
}
GameObject::~GameObject(){
	UnAttach();
	for(GameObject* obj:attachedObjs)obj->dead=true;

	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];

	if(Camera::GetFocus()==uid)Camera::Unfollow();
	
	if(!GAMESTATE.ending)GAMESTATE.entities.erase(uid);
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




template<int attackDist,int seeDist> void HostileAIfunc(CompAI* ai,float time){
	CompAnimControl* ac = COMPANIMCONTp(ai->entity);
	GameObject* target = GAMESTATE.entities[COMPMEMORYp(ai->entity)->ints["target"]].get();
	int &state = COMPMEMORYp(ai->entity)->ints["state"];
	Timer &cd = COMPMEMORYp(ai->entity)->timers["cooldown"];
	DEBUG(state);
	DEBUG(cd.Get());
	if(state==CompAI::state::idling){
		if(cd.Get()>5 && target!=nullptr){
			cd.Restart();
			state=CompAI::state::looking;
		}
	}
	else if(state==CompAI::state::looking){
		if(cd.Get()>5 || target==nullptr){
			cd.Restart();
			state=CompAI::state::idling;
			return;
		}
		//TODO: make line of sight component
		float dist = ai->entity->box.distEdge(target->box).x;
		if(dist<seeDist){
			cd.Restart();
			if(dist<attackDist)state=CompAI::state::attacking,ac->ChangeCur("attack");
			else state=CompAI::state::walking,ac->ChangeCur("walk");
		}
	}
	else if(state==CompAI::state::walking){
		if(target==nullptr || cd.Get()>5){
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			float dist = ai->entity->box.distEdge(target->box).x;
			CompMovement *movement = COMPMOVEp(ai->entity);

			//TODO: if cant see target go looking
			if(dist < attackDist+abs(movement->speed.x)*time){
				movement->speed.x=0;
				if(ai->entity->box.x < target->box.x)movement->move=dist-attackDist;
				else      movement->move=-dist+attackDist;

				state=CompAI::state::attacking;
				cd.Restart();
				ac->ChangeCur("attack");
			}
			else if(ai->entity->box.x < target->box.x){
				ai->entity->flipped=true;
				movement->speed.x= 100.0f;
			}
			else{
				ai->entity->flipped=false;
				movement->speed.x=-100.0f;
			}
		}
	}
	else if(state==CompAI::state::attacking){
		if(target==nullptr || ai->entity->box.distEdge(target->box).x > attackDist){
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		if(cd.Get()>5){
			if(target->box.x > ai->entity->box.x && !ai->entity->flipped)ai->entity->flipped=true;
			if(target->box.x < ai->entity->box.x &&  ai->entity->flipped)ai->entity->flipped=false;
			cd.Restart();
			state=CompAI::state::idling;
			ac->ChangeCur("idle");
		}
	}
}
template<int posCount> void PassiveAIfunc(CompAI* ai,float time){
	CompAnimControl *ac = COMPANIMCONTp(ai->entity);
	int &state = COMPMEMORYp(ai->entity)->ints["state"];
	int &next = COMPMEMORYp(ai->entity)->ints["nextPos"];
	Timer &cd = COMPMEMORYp(ai->entity)->timers["cooldown"];
	if(state==CompAI::state::idling){
		if(cd.Get()>0.5f){
			state=CompAI::state::walking;
			next=(next+1)%posCount;
			COMPANIMCONTp(ai->entity)->ChangeCur("walk");
		}
	}
	else if(state==CompAI::state::walking){
		float dist = COMPMEMORYp(ai->entity)->floats["pos"+next] - ai->entity->box.x;
		CompMovement *movement = COMPMOVEp(ai->entity);

		if(abs(dist)<abs(movement->speed.x)*time){
			movement->speed.x=0;
			movement->move=dist;

			state=CompAI::state::idling;
			cd.Restart();
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
			Vec2 pos{go->box.x,go->box.y+85};
			GameObject* bullet = GameObject::MakeBullet(pos,"player_projectile",go,force,180+angle+10,10,16,true);
			bullet->box.x+=bullet->box.w - 25;
			GAMESTATE.AddObject(bullet);
		}
		else{
			Vec2 pos{go->box.x,go->box.y+85};
			GameObject* bullet = GameObject::MakeBullet(pos,"player_projectile",go,force,angle-10,10,16,true);
			bullet->box.x+=go->box.w - bullet->box.w;
			GAMESTATE.AddObject(bullet);
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

	CompAnimControl* animControl = new CompAnimControl{"player"};
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


GameObject* GameObject::MakeMike(const Vec2 &pos){

	CompAnimControl* animControl = new CompAnimControl{"mike"};
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
	mike->AddComponent(new CompAI{HostileAIfunc<5,500>});

	CompMemory *memory = new CompMemory{};
	memory->ints["state"]=CompAI::state::idling;
	memory->ints["target"]=PLAYER_UID;
	mike->AddComponent(memory);

	return mike;
}


GameObject* GameObject::MakeBanshee(const Vec2 &pos,const Vec2 &pos2){
	CompAnimControl* animControl = new CompAnimControl{"banshee"};
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

	banshee->AddComponent(new CompAI{PassiveAIfunc<2>});

	CompMemory *memory = new CompMemory{};
	memory->floats["pos0"]=pos.x;
	memory->floats["pos1"]=pos2.x;
	memory->ints["state"]=CompAI::state::idling;
	memory->ints["nextPos"]=0;
	banshee->AddComponent(memory);

	return banshee;
}

GameObject* GameObject::MakeMask(const Vec2 &pos){
	CompAnimControl* animControl = new CompAnimControl{"mascara"};
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
	mask->AddComponent(new CompAI{HostileAIfunc<500,1000>});


	CompMemory *memory = new CompMemory{};
	memory->ints["target"]=PLAYER_UID;
	mask->AddComponent(memory);

	return mask;
}
