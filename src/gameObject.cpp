#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <stateStage.hpp>

GameObject::GameObject():uid{GAMESTATE.GetUID()}{}
GameObject::GameObject(const Vec2 &pos_,float r,Hotspot hs,bool a):
						uid{GAMESTATE.GetUID()},
						pos{pos_},
						rotation{r},
						hotspot{hs},
						anchored{a}{}
GameObject::GameObject(const Rect &rect,float r,Hotspot hs,bool a):
						uid{GAMESTATE.GetUID()},
						pos{rect.x,rect.y},
						size{rect.w,rect.h},
						rotation{r},
						hotspot{hs},
						anchored{a}{}
GameObject::~GameObject(){
	UnAttach();
	for(GameObject* obj:attachedObjs)obj->dead=true;

	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];

	if(Camera::GetFocus()==uid)Camera::Unfollow();

	if(!GAMESTATE.ending)GAMESTATE.entities.erase(uid);
}

void GameObject::Update(float time){
	//reset move
	if(hasComponent[Component::type::t_movement]){
		COMPMOVEp(this)->move=0.0f;
	}
	//process input control and ai first
	if(hasComponent[Component::type::t_input_control]){
		components[Component::type::t_input_control]->Update(time);
	}
	if(hasComponent[Component::type::t_ai]){
		components[Component::type::t_ai]->Update(time);
	}
	//then set move
	if(hasComponent[Component::type::t_movement]){
		CompMovement *compM = (CompMovement*) components[Component::type::t_movement];
		compM->move+=compM->speed*time;
	}
	//and then do the rest
	FOR2(i,Component::type::t__+1,Component::type::t_count){
		if(hasComponent[i]){
			components[i]->Update(time);
		}
	}
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
	component->Own(this);
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


bool GameObject::IsDead() const{
	return false;
}

Rect GameObject::Box() const{
	Rect r{pos,size};
	r.x += curPos.x * r.w;
	r.y += curPos.y * r.h;
	r.w *= curSize.x;
	r.h *= curSize.y;
	return Rect{r.hotspot(hotspot),Vec2{r.w,r.h}};
}
Rect GameObject::FullBox() const{
	Rect r{pos,size};
	return Rect{r.hotspot(hotspot),size};
}




template<int attackDist,int seeDist> void HostileAIfunc(CompAI* ai,float time){
	CompAnimControl* ac = COMPANIMCONTp(ai->entity);
	GameObject* target=nullptr;
	uint target_uid = COMPMEMORYp(ai->entity)->ints["target"];
	if(GAMESTATE.entities.count(target_uid))target=GAMESTATE.entities[target_uid].get();
	int &state = COMPMEMORYp(ai->entity)->ints["state"];
	Timer &cd = COMPMEMORYp(ai->entity)->timers["cooldown"];
	if(state==CompAI::state::idling){
		if(cd.Get() > 3 && target!=nullptr){
			cd.Restart();
			state=CompAI::state::looking;
		}
	}
	else if(target==nullptr){
		state=CompAI::state::idling;
		ac->ChangeCur("idle");
	}
	else if(state==CompAI::state::looking){
		if(cd.Get() > 5) {
			cd.Restart();
			state=CompAI::state::idling;
			return;
		}
		//TODO: make line of sight component
		float dist = ai->entity->Box().distEdge(target->Box()).x;
		if(dist < seeDist){
			cd.Restart();
			if(dist <= attackDist)state=CompAI::state::attacking,ac->ChangeCur("attack");
			else                  state=CompAI::state::walking,ac->ChangeCur("walk");
		}
	}
	else if(state == CompAI::state::walking){
		if(cd.Get() > 5){
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			float dist = ai->entity->Box().distEdge(target->Box()).x;
			CompMovement *movement = COMPMOVEp(ai->entity);

			//TODO: make line of sight component
			if(dist > seeDist){
				cd.Restart();
				state=CompAI::state::looking;
				ac->ChangeCur("idle");
			}
			if(dist < attackDist+abs(movement->speed.x)*time){
				movement->speed.x=0;
				if(ai->entity->Box().x < target->Box().x)movement->move= dist-attackDist;
				else                                     movement->move=-dist+attackDist;

				state=CompAI::state::attacking;
				cd.Restart();
				ac->ChangeCur("attack");
			}
			else if(ai->entity->Box().x < target->Box().x){
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
		if(ai->entity->Box().distEdge(target->Box()).x > attackDist){
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			if(target->Box().x > ai->entity->Box().x && !ai->entity->flipped)ai->entity->flipped=true;
			if(target->Box().x < ai->entity->Box().x &&  ai->entity->flipped)ai->entity->flipped=false;
		}
		if(cd.Get() > 5){
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
			COMPANIMCONTp(ai->entity)->ChangeCur("walk");
		}
	}
	else if(state==CompAI::state::walking){
		float pos = COMPMEMORYp(ai->entity)->floats["pos" + to_string(next)];
		float dist = pos - ai->entity->Box().x;
		CompMovement *movement = COMPMOVEp(ai->entity);

		if(abs(dist)<abs(movement->speed.x)*time){
			movement->speed.x=0;
			movement->move=dist;

			state=CompAI::state::idling;
			cd.Restart();
			next=(next+1)%posCount;
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

template<int attackDistMelee, int attackDist, int seeDist> void HostileBossAIfunc(CompAI* ai,float time){
	CompAnimControl* ac = COMPANIMCONTp(ai->entity);
	GameObject* target=nullptr;
	uint target_uid = COMPMEMORYp(ai->entity)->ints["target"];
	if(GAMESTATE.entities.count(target_uid))target=GAMESTATE.entities[target_uid].get();
	int &state = COMPMEMORYp(ai->entity)->ints["state"];
	Timer &cd = COMPMEMORYp(ai->entity)->timers["cooldown"];
	if(state==CompAI::state::idling){
		if(cd.Get() > 3 && target!=nullptr){
			cd.Restart();
			state=CompAI::state::looking;
		}
	}
	else if(target==nullptr){
		state=CompAI::state::idling;
		ac->ChangeCur("idle");
	}
	else if(state==CompAI::state::looking){
		if(cd.Get() > 5){
			cd.Restart();
			state=CompAI::state::idling;
			return;
		}
		//TODO: make line of sight component
		float dist = ai->entity->box.distEdge(target->box).x;
		if(dist < seeDist){
			cd.Restart();
			// Stun do Poroc: if(dist < 2*attackDistMelee) state=CompAI::state::attacking,ac->ChangeCur("stunning");
			if(dist <= attackDist)state=CompAI::state::attacking,ac->ChangeCur("attack");
			else                  state=CompAI::state::walking,ac->ChangeCur("walk");
		}
	}
	else if(state == CompAI::state::walking){
		if(cd.Get() > 5){
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			float dist = ai->entity->box.distEdge(target->box).x;
			CompMovement *movement = COMPMOVEp(ai->entity);

			//TODO: make line of sight component
			if(dist > seeDist){
				cd.Restart();
				state=CompAI::state::looking;
				ac->ChangeCur("idle");
			}
			if(dist < attackDist+abs(movement->speed.x)*time){
				movement->speed.x=0;
				if(ai->entity->box.x < target->box.x)movement->move= dist-attackDist;
				else                                 movement->move=-dist+attackDist;

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
		if(ai->entity->box.distEdge(target->box).x > attackDist){
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
		else{
			if(target->box.x > ai->entity->box.x && !ai->entity->flipped)ai->entity->flipped=true;
			if(target->box.x < ai->entity->box.x &&  ai->entity->flipped)ai->entity->flipped=false;
		}
		/* Stun do Porco
		if(cd.Get() > 3) { Sem dar cd.Restart()
            ac->ChangeCur("Attack_Stun");
            float dist = ai->entity->box.distEdge(target->box).x;
            if(dist > attackDist && dist > attackDistMelee) {
                cd.Restart();
                state=CompAI::state::walking;
                ac->ChangeCur("walk");
            } else if(cd > 4 && (attackDistMelee <= dist or attackDist <= dist)) {
                cd.Restart();
                state=CompAI::state::attacking;
                ac->ChangeCur("attack");
            }
		}
		*/
		if(cd.Get() > 5.5){
			cd.Restart();
			state=CompAI::state::idling;
			ac->ChangeCur("idle");
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
			Vec2 pos{go->Box().x,go->Box().y+85};
			GameObject* bullet = GameObject::MakeBullet(pos,"player_projectile",go,force,180+angle+10,10,16,true);
			bullet->pos.x += bullet->size.x - 25;
			GAMESTATE.AddObject(bullet);
		}
		else{
			Vec2 pos{go->Box().x,go->Box().y+85};
			GameObject* bullet = GameObject::MakeBullet(pos,"player_projectile",go,force,angle-10,10,16,true);
			bullet->pos.x += go->size.x - bullet->size.x;
			GAMESTATE.AddObject(bullet);
		}
	}
}

void PlayerMonsterCollision(const CompCollider* a,const CompCollider* b){
	Vec2 &speed=COMPMOVEp(a->entity)->speed;
	if(speed==Vec2{})return;

	Vec2 &totMove=COMPMOVEp(a->entity)->move;
	Vec2 move=a->collides(b,totMove);

	if(move!=totMove)COMPHPp(a->entity)->Damage(1);
}

void EmptyCollision(const CompCollider* a,const CompCollider* b){UNUSED(a);UNUSED(b);}

GameObject* GameObject::MakePlayer(const Vec2 &pos){

	GameObject* player = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider *coll = new CompCollider{CompCollider::collType::t_player};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=PlayerMonsterCollision;

	CompAnimControl* animControl = new CompAnimControl{"player",coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	player->AddComponent(animControl);


	player->AddComponent(new CompInputControl{PlayerControlFunc});
	player->AddComponent(new CompMovement{});

	player->AddComponent(new CompGravity{2500.0f});
	player->AddComponent(new CompHP{100,100,true,false,0.25f});

	player->flipped = true;
	player->team = Team::player;
	player->size = size;

	delete coll;

	return player;
}

GameObject* GameObject::MakeTarget(const Vec2 &pos){
	GameObject* target = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompStaticRender* img = new CompStaticRender{Sprite{"img/target.png"},Vec2{}};
	Vec2 size{(float)img->sp.GetWidth(),(float)img->sp.GetHeight()};
	target->AddComponent(img);

	target->AddComponent(new CompCollider{CompCollider::collType::t_solid});
	target->AddComponent(new CompHP{100,100,true,false});

	target->team=Team::other;
	target->size = size;
	return target;
}


GameObject* GameObject::MakeBullet(const Vec2 &pos,string animFile,GameObject* go,float force,float angle,int dmgLow,int dmgHigh,bool stick){
	dmgHigh=max(dmgHigh,dmgLow+1);

	GameObject* bullet = new GameObject{pos,0.0f};

	CompCollider *collider = new CompCollider{CompCollider::collType::t_bullet};
	collider->useDefault[CompCollider::collType::t_bullet] = EmptyCollision;

	auto foo = [stick,go,dmgLow,dmgHigh](const CompCollider* a,const CompCollider* b){
		Vec2 &totMove=COMPMOVEp(a->entity)->move;
		if(totMove==Vec2{})return;

		Vec2 move=a->collides(b,totMove);

		if(move!=totMove && a->entity->team != b->entity->team){
			a->entity->dead=true;

			if(stick && !b->entity->hasComponent[Component::type::t_movement]){
				GameObject *bullet = new GameObject{a->entity->Box() + move + totMove/4.0f};
				bullet->flipped=true;
				bullet->rotation=a->entity->rotation;
				Sprite sp = COMPANIMp(a->entity)->sp;
				sp.SetFrameTime(-1.0f);
				bullet->AddComponent(new CompStaticRender{sp,Vec2{}});
				GAMESTATE.AddObject(bullet);
				bullet->AttachTo(b->entity);
			}

			if(b->entity->hasComponent[Component::type::t_hp]){
				int dmg = dmgLow+rand()%(dmgHigh-dmgLow);
				COMPHPp(b->entity)->Damage(dmg);
			}
		}
	};

	collider->useDefault[CompCollider::collType::t_any] = foo;

	CompAnim* anim = new CompAnim{animFile,collider};
	Vec2 size{(float)anim->sp.GetWidth(),(float)anim->sp.GetHeight()};
	bullet->AddComponent(anim);

	bullet->AddComponent(new CompMovement{Vec2::makeVec2(force,angle),CompMovement::moveType::t_bullet});

	bullet->AddComponent(new CompGravity{500.0f});

	bullet->team = go->team;
	bullet->flipped = true;
	bullet->rotation = angle;
	bullet->size = size;

	delete collider;

	return bullet;
}


GameObject* GameObject::MakeMike(const Vec2 &pos){
	GameObject* mike = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider *coll = new CompCollider{CompCollider::collType::t_monster};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"mike",coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	mike->AddComponent(animControl);

	mike->AddComponent(new CompMovement{});
	mike->AddComponent(new CompGravity{2500.0f});
	mike->AddComponent(new CompHP{100,100,true,false});
	mike->AddComponent(new CompAI{HostileAIfunc<5,500>});

	CompMemory *memory = new CompMemory{};
	memory->ints["state"]=CompAI::state::idling;
	memory->ints["target"]=PLAYER_UID;
	mike->AddComponent(memory);

	mike->team = Team::enemy;
	mike->size = size;

	delete coll;

	return mike;
}

GameObject* GameObject::MakeBanshee(const Vec2 &pos,const Vec2 &pos2){
	GameObject* banshee = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider *coll = new CompCollider{CompCollider::collType::t_monster};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"banshee",coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	banshee->AddComponent(animControl);

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

	banshee->team = Team::enemy;
	banshee->size = size;

	delete coll;

	return banshee;
}

GameObject* GameObject::MakeMask(const Vec2 &pos){
	GameObject* mask = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider *coll = new CompCollider{CompCollider::collType::t_monster};
	coll->useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll->useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"mascara",coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	mask->AddComponent(animControl);

	mask->AddComponent(new CompMovement{});
	// mask->AddComponent(new CompGravity{2500.0f});
	mask->AddComponent(new CompHP{50,50,true,false});
	mask->AddComponent(new CompAI{HostileAIfunc<500,1000>});


	CompMemory *memory = new CompMemory{};
	memory->ints["target"]=PLAYER_UID;
	mask->AddComponent(memory);

	mask->team = Team::enemy;
	mask->size = size;

	delete coll;

	return mask;
}
