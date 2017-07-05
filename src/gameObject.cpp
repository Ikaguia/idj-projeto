#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <stateStage.hpp>
#include <txtFuncs.hpp>

uint GameObject::goCount=0;
map<uint, unique_ptr<GameObject>> GameObject::entities;

GameObject::GameObject():uid{goCount++}{
	entities[uid]=unique_ptr<GameObject>(this);
}
GameObject::GameObject(const Vec2 &pos_,float r,Hotspot hs,bool a):
						uid{goCount++},
						pos{pos_},
						rotation{r},
						hotspot{hs},
						anchored{a}{
	entities[uid]=unique_ptr<GameObject>(this);
}
GameObject::GameObject(const Rect &rect,float r,Hotspot hs,bool a):
						uid{goCount++},
						pos{rect.x,rect.y},
						size{rect.w,rect.h},
						rotation{r},
						hotspot{hs},
						anchored{a}{
	entities[uid]=unique_ptr<GameObject>(this);
}
GameObject::~GameObject(){
	UnAttach();
	for(GameObject* obj:attachedObjs)obj->dead=true;

	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];

	if(Camera::GetFocus()==uid)Camera::Unfollow();

	entities.erase(uid);
}

void GameObject::Update(float time){
	if(IsDead()){
		remove = true;
		// for(auto i=Component::type::t_first+1;i!=Component::type::t_count;i++){
		FOR(i,Component::type::t_count){
			DEBUG(i);
			if(hasComponent[i]){
				DEBUG(components[i]);
				if(components[i]->Die(time))RemoveComponent((Component::type)i);
				else remove=false;
			}
		}
	}
	//reset move
	if(hasComponent[Component::type::t_movement])COMPMOVEp(this)->move=0.0f;
	//process input control and ai first
	if(hasComponent[Component::type::t_input_control])COMPINPUTCONTp(this)->Update(time);
	if(hasComponent[Component::type::t_ai])COMPAIp(this)->Update(time);
	//then set move
	if(hasComponent[Component::type::t_movement])COMPMOVEp(this)->move+=COMPMOVEp(this)->speed*time;
	//and then do the rest
	FOR2(i,Component::type::t__+1,Component::type::t_count){
		if(hasComponent[i])components[i]->Update(time);
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
	if(hasComponent[t])cerr << "Error, adding component " << t << " to a GameObject that already has it" << endl;
	hasComponent[t]=true;
	components[t]=component;
	component->Own(this);
}
void GameObject::ReplaceComponent(Component* component){
	auto t=component->GetType();
	if(!hasComponent[t]){
		cerr << "Error, replacing component " << t << " on a GameObject that doesnt have it" << endl;
		hasComponent[t]=true;
	}
	else delete components[t];
	components[t]=component;
	component->Own(this);
}
void GameObject::RemoveComponent(Component::type t){
	if(!hasComponent[t])cerr << "Error, removing component " << t << " on a GameObject that doesnt have it" << endl;
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
	return dead;
}
bool GameObject::Remove() const{
	return remove;
}

Rect GameObject::Box() const{
	Rect r{pos,size};
	r.x += curPos.x * r.w;
	r.y += curPos.y * r.h;
	r.w *= curSize.x;
	r.h *= curSize.y;
	return Rect{r.hotspot(hotspot),Vec2{r.w,r.h}};
}
Rect GameObject::Box(const Vec2& p,const Vec2 &sz) const{
	Rect r{pos,size};
	r.x += p.x * r.w;
	r.y += p.y * r.h;
	r.w *= sz.x;
	r.h *= sz.y;
	return Rect{r.hotspot(hotspot),Vec2{r.w,r.h}};
}
Rect GameObject::FullBox() const{
	Rect r{pos,size};
	return Rect{r.hotspot(hotspot),size};
}




template<int attackDist,int seeDist> void HostileAIfunc(CompAI* ai,float time){
	CompAnimControl *ac = COMPANIMCONTp(GO(ai->entity));
	CompMemory *mem = COMPMEMORYp(GO(ai->entity));

	int &state = mem->ints["state"];
	GameObject* target = GO(mem->ints["target"]);

	Timer &cd = mem->timers["cooldown"];
	Timer &al = mem->timers["alerted"];
	bool alerted = (al.Get() < 5);


	if(mem->ints["hit"]){
		mem->ints["hit"]=0;
		al.Restart();
		return;
	}
	if(state==CompAI::state::idling){
		if((alerted || cd.Get() > 3) && target != nullptr){
			state=CompAI::state::looking;
			cd.Restart();
			return;
		}
	}
	else if(target==nullptr){
		state=CompAI::state::idling;
		ac->ChangeCur("idle");
		cd.Restart();
		return;
	}
	else if(state==CompAI::state::looking){
		if(al.Get() > 10 && cd.Get() > 5) {
			state=CompAI::state::idling;
			cd.Restart();
			return;
		}
		//TODO: make line of sight component
		float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
		if((alerted && dist < (seeDist*2)) || dist < seeDist){
			if(dist <= attackDist)state=CompAI::state::attacking;
			else                  state=CompAI::state::walking,ac->ChangeCur("walk");
			cd.Restart();
			return;
		}
	}
	else if(state == CompAI::state::walking){
		CompMovement *movement = COMPMOVEp(GO(ai->entity));
		if(al.Get() > 10 && cd.Get() > 5){
			state=CompAI::state::looking;
			movement->speed.x = 0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
		}
		else{
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;

			//TODO: make line of sight component
			if(dist > (seeDist*2) || (!alerted && dist > seeDist)){
				state=CompAI::state::looking;
				movement->speed.x = 0;
				ac->ChangeCur("idle");
				cd.Restart();
				return;
			}
			else if(dist < attackDist+abs(movement->speed.x)*time){
				if(GO(ai->entity)->Box().x < target->Box().x)movement->move= dist-attackDist;
				else                                         movement->move=-dist+attackDist;

				state=CompAI::state::attacking;
				movement->speed.x=0;
				ac->ChangeCur("idle");
				cd.Restart();
				return;
			}
			else if(GO(ai->entity)->Box().x < target->Box().x){
				GO(ai->entity)->flipped=true;
				movement->speed.x = 100.0f;
			}
			else{
				GO(ai->entity)->flipped=false;
				movement->speed.x =-100.0f;
			}
		}
	}
	else if(state==CompAI::state::attacking){
		if(!alerted && mem->ints["attacked"]>3){
			state=CompAI::state::idling;
			mem->ints["attacked"]=0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
		}
		else if(ac->GetCurName() != "attack"){
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
			if(dist > attackDist){
				state=CompAI::state::looking;
				mem->ints["attacked"]=0;
				ac->ChangeCur("idle");
				cd.Restart();
				return;
			}
			else ac->ChangeCur("attack",false);
		}
		else{
			if(target->Box().x > GO(ai->entity)->Box().x && !GO(ai->entity)->flipped)GO(ai->entity)->flipped=true;
			if(target->Box().x < GO(ai->entity)->Box().x &&  GO(ai->entity)->flipped)GO(ai->entity)->flipped=false;
		}
	}
}
template<int posCount> void PassiveAIfunc(CompAI* ai,float time){
	CompAnimControl *ac = COMPANIMCONTp(GO(ai->entity));
	CompMemory *mem = COMPMEMORYp(GO(ai->entity));

	int &state = mem->ints["state"];
	int &next = mem->ints["nextPos"];

	Timer &cd = mem->timers["cooldown"];

	if(state==CompAI::state::idling){
		if(cd.Get()>0.5f){
			state=CompAI::state::walking;
			ac->ChangeCur("walk");
		}
	}
	else if(state==CompAI::state::walking){
		float pos = mem->floats["pos" + to_string(next)];
		float dist = pos - GO(ai->entity)->Box().x;
		CompMovement *movement = COMPMOVEp(GO(ai->entity));

		if(abs(dist)<abs(movement->speed.x)*time){
			movement->speed.x=0;
			movement->move=dist;

			state=CompAI::state::idling;
			cd.Restart();
			next=(next+1)%posCount;
			ac->ChangeCur("idle");
		}
		else if(dist>0){
			GO(ai->entity)->flipped=true;
			movement->speed.x= 100.0f;
		}
		else{
			GO(ai->entity)->flipped=false;
			movement->speed.x=-100.0f;
		}
	}
}
template<int atkDist,int seeDist,int stCD,int atkCount,int stompCount> void PumbaAiFunc(CompAI* ai,float time){
	CompAnimControl *ac = COMPANIMCONTp(GO(ai->entity));
	CompMemory *mem = COMPMEMORYp(GO(ai->entity));

	int &state = mem->ints["state"];
	int &attacked = mem->ints["attacked"];
	GameObject* target = GO(mem->ints["target"]);

	Timer &cd = mem->timers["cooldown"];
	Timer &stompCD = mem->timers["stompCooldown"];

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
		float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
		if(dist < seeDist){
			cd.Restart();
			if(dist < 2*atkDist && stompCD.Get()>stCD)
				state=CompAI::state::stomping, ac->ChangeCur("stomp");
			else if(dist < atkDist)
				state=CompAI::state::attacking,ac->ChangeCur("attack");
			else 
				state=CompAI::state::walking,  ac->ChangeCur("walk");
		}
	}
	else if(state == CompAI::state::walking){
		CompMovement *move = COMPMOVEp(GO(ai->entity));
		if(cd.Get() > 5){
			cd.Restart();
			state=CompAI::state::idling;
			move->speed = move->move = 0.0f;
			ac->ChangeCur("idle");
		}
		else{
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
			//TODO: make line of sight component
			if(dist > seeDist){
				cd.Restart();
				state=CompAI::state::looking;
				ac->ChangeCur("idle");
			}
			else if(dist < 2*atkDist+abs(move->speed.x)*time && cd.Get()<1.5 && stompCD.Get() > stCD){
				move->speed.x=0;
				if(GO(ai->entity)->Box().x < target->Box().x)move->move= dist - (2*atkDist);
				else                                             move->move=-dist + (2*atkDist);

				state=CompAI::state::stomping;
				cd.Restart();
				ac->ChangeCur("stomp");
			}
			else if(dist <   atkDist+abs(move->speed.x)*time){
				move->speed.x=0;
				if(GO(ai->entity)->Box().x < target->Box().x)move->move= dist-atkDist;
				else                                             move->move=-dist+atkDist;

				state=CompAI::state::charging;
				cd.Restart();
				ac->ChangeCur("charge");
			}
			else if(GO(ai->entity)->pos.x < target->pos.x){
				GO(ai->entity)->flipped=true;
				move->speed.x= 100.0f;
			}
			else{
				GO(ai->entity)->flipped=false;
				move->speed.x=-100.0f;
			}
		}
	}
	else if(state==CompAI::state::attacking){
		if(GO(ai->entity)->Box().distEdge(target->Box()).x > atkDist){
			attacked=0;
			cd.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
			return;
		}
		else{
			if(target->Box().x > GO(ai->entity)->Box().x && !GO(ai->entity)->flipped)GO(ai->entity)->flipped=true;
			if(target->Box().x < GO(ai->entity)->Box().x &&  GO(ai->entity)->flipped)GO(ai->entity)->flipped=false;
			if(attacked>=atkCount){
				attacked=0;
				cd.Restart();
				state=CompAI::state::idling;
				ac->ChangeCur("idle");
			}
		}
	}
	else if(state==CompAI::state::stomping){
		if(attacked>=stompCount){
			attacked=0;
			cd.Restart();
			stompCD.Restart();
			state=CompAI::state::looking;
			ac->ChangeCur("idle");
		}
	}
	else if(state==CompAI::state::charging){
		if(attacked>=1){
			attacked=0;
			cd.Restart();
			state=CompAI::state::idling;
			ac->ChangeCur("idle");
		}
	}
}

void PlayerControlFunc(GameObject* go, float time){
	CompAnimControl *ac = COMPANIMCONTp(go);
	CompMemory *mem = COMPMEMORYp(go);
	CompMovement *mv = COMPMOVEp(go);

	const string &curAnim = ac->GetCurName();
	int &arrowReady = mem->ints["arrowReady"];
	Vec2 &speed = mv->speed;


	if(curAnim == "kick")return;
	if(INPUT.KeyPress(KEY(s)) && curAnim == "idle")ac->ChangeCur("kick",false);
	else{
		if(INPUT.IsKeyDown(KEY(a)) && arrowReady)ac->ChangeCur("fire",false);

		if(!mem->ints["onAir"])mem->ints["doubleJump"]=0;
		if(INPUT.KeyPress(KEY_UP) && !mem->ints["doubleJump"]){
			speed.y=-1500.0f;
			mem->ints["doubleJump"]=mem->ints["onAir"];
			mem->ints["onAir"]=1;
			//ac->ChangeCur("jump");
		}

		if(INPUT.IsKeyDown(KEY_LEFT) && !INPUT.IsKeyDown(KEY_RIGHT))
			speed.x=max(-400.0f,min(0.0f,speed.x)-800*time);
		else if(INPUT.IsKeyDown(KEY_RIGHT) && !INPUT.IsKeyDown(KEY_LEFT))
			speed.x=min( 400.0f,max(0.0f,speed.x)+800*time);
		else if(speed.x>0.0f)
			speed.x=max(0.0f,speed.x-800*time);
		else if(speed.x<0.0f)
			speed.x=min(0.0f,speed.x+800*time);
	}


	if(equals(speed.x,0)){
		if(curAnim == "walk")ac->ChangeCur("idle");
	}
	else{
		if(curAnim == "idle")ac->ChangeCur("walk");
		if((speed.x < 0 && go->flipped) || (speed.x>0 && !go->flipped)){
			go->flipped = !go->flipped;
			// ac->ChangeCur("flipped",false);
		}
	}
}

void PlayerMonsterCollision(const CompCollider::Coll &a,const CompCollider::Coll &b){
	Vec2 &speed=COMPMOVEp(GO(a.entity))->speed;
	if(speed==Vec2{})return;

	Vec2 &totMove=COMPMOVEp(GO(a.entity))->move;
	Vec2 move=a.Collides(b,totMove);

	if(move!=totMove)COMPHPp(GO(a.entity))->Damage(1);
}

void PlayerBlockCollision(const CompCollider::Coll &a,const CompCollider::Coll &b){
	CompMovement *compMove = COMPMOVEp(GO(a.entity));
	CompMemory *mem = COMPMEMORYp(GO(a.entity));

	Vec2 &speed=compMove->speed;
	Vec2 &totMove=compMove->move;
	Vec2 move;

	if(totMove==Vec2{})return;

	move.x = a.Collides(b,{totMove.x,0.0f},move).x;
	if(move.x != totMove.x)speed.x=0.0f;

	move.y = a.Collides(b,{0.0f,totMove.y},move).y;
	if(move.y != totMove.y){
		speed.y=0.0f;
		if(totMove.y>0)mem->ints["onAir"]=0;
	}
	else if(equals(totMove.y,0.0f) && equals(a.Collides(b,{0.0f,10},move).y,0.0f))mem->ints["onAir"]=0;

	totMove=move;
}

void EmptyCollision(const CompCollider::Coll &a,const CompCollider::Coll &b){UNUSED(a);UNUSED(b);}

uint GameObject::MakePlayer(const Vec2 &pos){
	GameObject* player = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider coll{CompCollider::collType::t_player};
	coll.colls[0].useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_monster]=PlayerMonsterCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_any]=PlayerBlockCollision;

	CompAnimControl* animControl = new CompAnimControl{"player",&coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	player->AddComponent(animControl);


	player->AddComponent(new CompInputControl{PlayerControlFunc});
	player->AddComponent(new CompMovement{});

	player->AddComponent(new CompGravity{2500.0f});
	player->AddComponent(new CompHP{100,100,true,false,0.25f});

	CompMemory *mem = new CompMemory;
	mem->ints["arrowReady"] = 1;
	mem->ints["onAir"] = 1;
	mem->ints["doubleJump"] = 0;
	player->AddComponent(mem);

	player->flipped = true;
	player->team = Team::player;
	player->size = size;

	return player->uid;
}

uint GameObject::MakeTarget(const Vec2 &pos){
	GameObject* target = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompStaticRender* img = new CompStaticRender{Sprite{"img/target.png"},Vec2{}};
	Vec2 size{(float)img->sp.GetWidth(),(float)img->sp.GetHeight()};
	target->AddComponent(img);

	target->AddComponent(new CompCollider{CompCollider::collType::t_solid});
	target->AddComponent(new CompHP{100,100,true,false});

	target->team=Team::other;
	target->size = size;
	return target->uid;
}

uint GameObject::MakeMike(const Vec2 &pos){
	GameObject* mike = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider coll{CompCollider::collType::t_monster};
	coll.colls[0].useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"mike",&coll};
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

	return mike->uid;
}

uint GameObject::MakeBanshee(const Vec2 &pos,const Vec2 &pos2){
	GameObject* banshee = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider coll{CompCollider::collType::t_monster};
	coll.colls[0].useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"banshee",&coll};
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

	return banshee->uid;
}

uint GameObject::MakeMask(const Vec2 &pos){
	GameObject* mask = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider coll{CompCollider::collType::t_monster};
	coll.colls[0].useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"mascara",&coll};
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

	return mask->uid;
}

uint GameObject::MakePorco(const Vec2 &pos){
	GameObject* pumba = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	CompCollider coll{CompCollider::collType::t_monster};
	coll.colls[0].useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"porco",&coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	pumba->AddComponent(animControl);

	pumba->AddComponent(new CompMovement{});
	pumba->AddComponent(new CompGravity{2500.0f});
	pumba->AddComponent(new CompHP{100,100,true,false});
	pumba->AddComponent(new CompAI{PumbaAiFunc<5,500,2,3,1>});

	CompMemory *memory = new CompMemory{};
	memory->ints["state"]=CompAI::state::idling;
	memory->ints["target"]=PLAYER_UID;
	pumba->AddComponent(memory);

	pumba->team = Team::enemy;
	pumba->size = size;

	return pumba->uid;
}

