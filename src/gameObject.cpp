#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>
#include <camera.hpp>
#include <complib.hpp>
#include <stateStage.hpp>
#include <txtFuncs.hpp>
#include <music.hpp>

uint GameObject::goCount=0;
map<uint, unique_ptr<GameObject>> GameObject::entities;

GameObject::GameObject():uid{goCount++}{
	components.fill(nullptr);
	entities[uid]=unique_ptr<GameObject>(this);
}
GameObject::GameObject(const Vec2 &pos_,float r,Hotspot hs,bool a):
						uid{goCount++},
						pos{pos_},
						rotation{r},
						hotspot{hs},
						anchored{a}{
	components.fill(nullptr);
	entities[uid]=unique_ptr<GameObject>(this);
}
GameObject::GameObject(const Rect &rect,float r,Hotspot hs,bool a):
						uid{goCount++},
						pos{rect.x,rect.y},
						size{rect.w,rect.h},
						rotation{r},
						hotspot{hs},
						anchored{a}{
	components.fill(nullptr);
	entities[uid]=unique_ptr<GameObject>(this);
}
GameObject::~GameObject(){
	UnAttach();
	for(GameObject* obj:attachedObjs)obj->dead=true;

	FOR(i,Component::type::t_count)if(HasComponent(i))delete components[i];

	if(Camera::GetFocus()==uid)Camera::Unfollow();

	entities.erase(uid);
}

void GameObject::Update(float time){
	if(IsDead()){
		remove = true;
		// for(auto i=Component::type::t_first+1;i!=Component::type::t_count;i++){
		FOR(i,Component::type::t_count){
			if(HasComponent(i)){
				if(components[i]->Die(time))RemoveComponent((Component::type)i);
				else remove=false;
			}
		}
	}
	//reset move
	if(HasComponent(Component::type::t_movement))COMPMOVEp(this)->move=0.0f;
	//process input control and ai first
	if(HasComponent(Component::type::t_input_control))COMPINPUTCONTp(this)->Update(time);
	if(HasComponent(Component::type::t_ai))COMPAIp(this)->Update(time);
	//then set move
	if(HasComponent(Component::type::t_movement))COMPMOVEp(this)->move+=COMPMOVEp(this)->speed*time;
	//and then do the rest
	FOR2(i,Component::type::t__+1,Component::type::t_count){
		if(HasComponent(i))components[i]->Update(time);
	}
}
void GameObject::Render(){
	FOR(i,Component::type::t_count){
		if(HasComponent(i)){
			components[i]->Render();
		}
	}
}

void GameObject::AddComponent(Component* component){
	auto t=component->GetType();
	if(HasComponent(t))cerr << "Error, adding component " << t << " to a GameObject that already has it" << endl;
	components[t]=component;
	component->Own(this);
}
void GameObject::ReplaceComponent(Component* component){
	auto t=component->GetType();
	if(!HasComponent(t))cerr << "Error, replacing component " << t << " on a GameObject that doesnt have it" << endl;
	else delete components[t];
	components[t]=component;
	component->Own(this);
}
void GameObject::RemoveComponent(Component::type t){
	if(!HasComponent(t))cerr << "Error, removing component " << t << " on a GameObject that doesnt have it" << endl;
	else{
		delete components[t];
		components[t]=nullptr;
	}
}
void GameObject::SetComponent(Component::type t,Component* component){
	components[t]=component;
	component->Own(this);
}
bool GameObject::HasComponent(size_t t) const{
	return (components[t] != nullptr);
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




template<int atkDist,int seeDist, int id> void HostileAIfunc(CompAI* ai,float time){
	Sound music;
	CompAnimControl *ac = COMPANIMCONTp(GO(ai->entity));
	CompMemory *mem = COMPMEMORYp(GO(ai->entity));

	int &state = mem->ints["state"];
	int &attacked = mem->ints["attacked"];
	GameObject* target = GO(mem->ints["target"]);

	Timer &cd = mem->timers["cooldown"];
	Timer &al = mem->timers["alerted"];

	if(mem->ints["hit"]){
		mem->ints["hit"]=0;
		al.Restart();
	}

	bool alerted = (al.Get() < 5);

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
			if(dist < atkDist) {
                state=CompAI::state::attacking;
                if(id == 1) { // Mike
                    music.Open("audio/mike-hit-chao.wav");
                    music.Play(1);
                } else { // Mask
                    music.Open("audio/alma-firebal.wav");
                    music.Play(1);
                }
			}
			else {
                state=CompAI::state::walking;
                ac->ChangeCur("walk");
                if(id == 1) { // Mike
                    music.Open("audio/mike-arrastando-clava.wav");
                    music.Play(1);
                }
			}
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
			else if(dist < atkDist+abs(movement->speed.x)*time){
				if(GO(ai->entity)->Box().x < target->Box().x)movement->move= dist-atkDist;
				else                                         movement->move=-dist+atkDist;

				state=CompAI::state::attacking;
				if(id == 1) { // Mike
                    music.Open("audio/mike-hit-chao.wav");
                    music.Play(1);
                } else { // Mask
                    music.Open("audio/alma-firebal.wav");
                    music.Play(1);
                }
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
		if(!alerted && attacked>3){
			state=CompAI::state::idling;
			attacked=0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
		}
		else if(ac->GetCurName() != "attack"){
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
			if(dist > atkDist){
				state=CompAI::state::looking;
				attacked=0;
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
void PassiveAIfunc(CompAI* ai,float time){
	Sound music;
	CompAnimControl *ac = COMPANIMCONTp(GO(ai->entity));
	CompMemory *mem = COMPMEMORYp(GO(ai->entity));

	int &state = mem->ints["state"];
	int &next = mem->ints["nextPos"];
	int &posCount = mem->ints["posCount"];

	Timer &cd = mem->timers["cooldown"];

	if(state==CompAI::state::idling){
		if(cd.Get()>0.5f){
			state=CompAI::state::walking;
			ac->ChangeCur("walk");
			music.Open("audio/banshee-vozes-1.wav");
			music.Play(1);
		}
	}
	else if(state==CompAI::state::walking){
		Vec2 pos{mem->floats["pos" + to_string(next) + "x"],mem->floats["pos" + to_string(next) + "y"]};
		Vec2 dist = pos - GO(ai->entity)->pos;
		CompMovement *movement = COMPMOVEp(GO(ai->entity));

		if(dist.len() < movement->speed.len() * time){
			movement->speed=Vec2{};
			movement->move=dist;

			state=CompAI::state::idling;
			cd.Restart();
			next=(next+1)%posCount;
			ac->ChangeCur("idle");
		}
		else{
			movement->speed = dist.unit() * 100.0f;
			GO(ai->entity)->flipped = (movement->speed.x>0);
		}
	}
}
template<int atkDist,int seeDist,int stCD,int atkCount,int stompCount> void PumbaAiFunc(CompAI* ai,float time){
	Sound music;
	Music music2;
	CompAnimControl *ac = COMPANIMCONTp(GO(ai->entity));
	CompMemory *mem = COMPMEMORYp(GO(ai->entity));

	int &state = mem->ints["state"];
	int &attacked = mem->ints["attacked"];
	GameObject* target = GO(mem->ints["target"]);

	Timer &cd = mem->timers["cooldown"];
	Timer &al = mem->timers["alerted"];
	Timer &stompCD = mem->timers["stomp"];
	music2.Open("audio/batalha-miniboss.ogg");
	music2.Play(-1);

	string &curAnim = ac->GetCurName();
	if(curAnim.substr(curAnim.size()-2)=="_r"){
		if(!GO(ai->entity)->flipped){
			int frame = ac->GetCur().GetCurFrame();
			curAnim=curAnim.substr(0,curAnim.size()-2);
			ac->GetCur().SetCurFrame(frame);
		}
	}
	else if(GO(ai->entity)->flipped){
		int frame = ac->GetCur().GetCurFrame();
		curAnim=curAnim+"_r";
		ac->GetCur().SetCurFrame(frame);
	}

	if(mem->ints["hit"]){
		mem->ints["hit"]=0;
		al.Restart();
	}

	bool alerted = (al.Get() < 5);

	if(state==CompAI::state::idling){
		if((alerted || cd.Get() > 3) && target != nullptr){
			state=CompAI::state::looking;
			music.Open("audio/porco-grunhido-3.wav");
			music.Play(1);
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
			if(dist < 2*atkDist && stompCD.Get()>stCD) {
				state=CompAI::state::stomping;
				ac->ChangeCur("stomp");
				music.Open("audio/porco-pisada.wav");
				music.Play(1);
			}
			else if(dist < atkDist)state=CompAI::state::attacking;
			else  {
                state=CompAI::state::walking;
                ac->ChangeCur("walk");
                music.Open("audio/porco-walking-grunhido.wav");
                music.Play(1);
			}
			cd.Restart();
			return;
		}
	}
	else if(state == CompAI::state::walking){
		CompMovement *move = COMPMOVEp(GO(ai->entity));
		if(al.Get() > 10 && cd.Get() > 5){
			state=CompAI::state::looking;
			move->speed.x = 0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
		}
		else{
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;

			//TODO: make line of sight component
			if(dist > (seeDist*2) || (!alerted && dist > seeDist)){
				state=CompAI::state::looking;
				move->speed.x = 0;
				ac->ChangeCur("idle");
				cd.Restart();
				return;
			}
			else if(dist < 2*atkDist+abs(move->speed.x)*time && cd.Get()<1.5 && stompCD.Get() > stCD){
				if(GO(ai->entity)->Box().x < target->Box().x)move->move= dist - (2*atkDist);
				else	move->move=-dist + (2*atkDist);

				state=CompAI::state::stomping;
				music.Open("audio/porco-pisada.wav");
				music.Play(1);
				move->speed.x=0;
				ac->ChangeCur("stomp");
				cd.Restart();
			}
			else if(dist < atkDist+abs(move->speed.x)*time){
				if(GO(ai->entity)->Box().x < target->Box().x)move->move= dist-atkDist;
				else	move->move=-dist+atkDist;

				if(cd.Get()<1.5)state=CompAI::state::attacking,ac->ChangeCur("idle");
				else            state=CompAI::state::charging, ac->ChangeCur("charge",false);
				move->speed.x=0;
				cd.Restart();
				return;
			}
			else if(GO(ai->entity)->Box().x < target->Box().x){
				GO(ai->entity)->flipped=true;
				move->speed.x = 350.0f;
			}
			else{
				GO(ai->entity)->flipped=false;
				move->speed.x =-350.0f;
			}
		}
	}
	else if(state==CompAI::state::attacking){
		if(!alerted && attacked>3){
			state=CompAI::state::idling;
			attacked=0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
		}
		else if(ac->GetCurName() != "attack" && ac->GetCurName() != "attack_r"){
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
			if(dist > atkDist){
				state=CompAI::state::looking;
				music.Open("audio/porco-grunhido-3.wav");
                music.Play(1);
				attacked=0;
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
	else if(state==CompAI::state::stomping){
		if(attacked>stompCount){
			state=CompAI::state::attacking;
			music.Open("audio/porco-investida-1.wav");
			music.Play(1);
			attacked=0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
		}
		else if(ac->GetCurName() != "stomp" && ac->GetCurName() != "stomp_r"){
			float dist = GO(ai->entity)->Box().distEdge(target->Box()).x;
			if(dist > atkDist*2){
				state=CompAI::state::looking;
				music.Open("audio/porco-grunhido-3.wav");
                music.Play(1);
				attacked=0;
				ac->ChangeCur("idle");
				cd.Restart();
				return;
			}
			else ac->ChangeCur("stomp",false);
		}
		else{
			if(target->Box().x > GO(ai->entity)->Box().x && !GO(ai->entity)->flipped)GO(ai->entity)->flipped=true;
			if(target->Box().x < GO(ai->entity)->Box().x &&  GO(ai->entity)->flipped)GO(ai->entity)->flipped=false;
		}
	}
	else if(state==CompAI::state::charging){
		if(ac->GetCurName() != "charge" && ac->GetCurName() != "charge_r"){
			state=CompAI::state::looking;
			attacked=0;
			ac->ChangeCur("idle");
			cd.Restart();
			return;
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
				music.Open("audio/porco-grunhido-3.wav");
                music.Play(1);
				ac->ChangeCur("idle");
			}
			else if(dist < 2*atkDist+abs(move->speed.x)*time && cd.Get()<1.5 && stompCD.Get() > stCD){
				move->speed.x=0;
				if(GO(ai->entity)->Box().x < target->Box().x)move->move= dist - (2*atkDist);
				else	move->move=-dist + (2*atkDist);

				state=CompAI::state::stomping;
				music.Open("audio/porco-pisada.wav");
				music.Play(1);
				cd.Restart();
				ac->ChangeCur("stomp");
			}
			else if(dist <   atkDist+abs(move->speed.x)*time){
				move->speed.x=0;
				if(GO(ai->entity)->Box().x < target->Box().x)move->move= dist-atkDist;
				else                                         move->move=-dist+atkDist;

				cd.Restart();
				if(cd.Get()<1.5)state=CompAI::state::attacking,ac->ChangeCur("idle");
				else            state=CompAI::state::charging, ac->ChangeCur("charge");
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
			music.Open("audio/porco-grunhido-3.wav");
            music.Play(1);
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
			music.Open("audio/porco-grunhido-3.wav");
            music.Play(1);
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
	UNUSED(time);

	CompAnimControl *ac = COMPANIMCONTp(go);
	CompMemory *mem = COMPMEMORYp(go);
	CompMovement *mv = COMPMOVEp(go);

	const string &curAnim = ac->GetCurName();
	int &arrowReady = mem->ints["arrowReady"];
	Vec2 &speed = mv->speed;

	//TODO: stun
	// if(mem->ints["hit"]){
	// 	mem->timers["stunned"].Restart();
	// 	mem->ints["hit"]=0;
	// }
	// if(mem->timers["stunned"].Get() < 0.5)return;

	if(curAnim == "kick" || curAnim == "fire")return;
	if(arrowReady && INPUT.IsKeyDown(KEY(a)) && curAnim == "idle")ac->ChangeCur("fire",false);
	else if(         INPUT.KeyPress (KEY(s)) && curAnim == "idle")ac->ChangeCur("kick",false);
	else{
		if(!mem->ints["onAir"])mem->ints["doubleJump"]=0;
		if(INPUT.KeyPress(KEY_UP) && !mem->ints["doubleJump"]){
			if(!mem->ints["onAir"])speed.y=-1500.0f;
			else speed.y=-1000.0f;
			mem->ints["doubleJump"]=mem->ints["onAir"];
			mem->ints["onAir"]=1;
			//ac->ChangeCur("jump");
		}

		if(INPUT.IsKeyDown(KEY_LEFT) && !INPUT.IsKeyDown(KEY_RIGHT))
			speed.x=-400.0f;
		else if(INPUT.IsKeyDown(KEY_RIGHT) && !INPUT.IsKeyDown(KEY_LEFT))
			speed.x= 400.0f;
		else speed.x=0.0f;
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
	player->AddComponent(new CompHP{100,100,true,false,0.75f});

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

uint GameObject::Create(const string& blueprint, const Vec2& pos, const Vec2& aux){
	if(blueprint == "mike")		return MakeMike(pos);
	if(blueprint == "banshee")	return MakeBanshee(pos,aux);
	if(blueprint == "mask")		return MakeMask(pos);
	if(blueprint == "porco")	return MakePorco(pos);

	GameObject* obj = new GameObject{pos};
	obj->type = blueprint;
	CompStaticRender* img = new CompStaticRender{Sprite{"img/"+blueprint+".png"}};
	Vec2 size{(float)img->sp.GetWidth(),(float)img->sp.GetHeight()};
	obj->AddComponent(img);
	obj->size = size;

	return obj->uid;
	/*vector<string> components = Resources::GetBlueprint(blueprint);
	stringstream comp;
	string compType;
	for(auto& i:components){
		comp.str(i);
		comp >> compType;
		if(compType == "movement")
	}*/
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
	
	mike->type = "mike";

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
	mike->AddComponent(new CompAI{HostileAIfunc<5,500,1>});

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

	banshee->type = "banshee";

	CompCollider coll{CompCollider::collType::t_monster};
	coll.colls[0].useDefault[CompCollider::collType::t_ground]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_bullet]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_player]=EmptyCollision;
	coll.colls[0].useDefault[CompCollider::collType::t_monster]=EmptyCollision;

	CompAnimControl* animControl = new CompAnimControl{"banshee",&coll};
	Vec2 size{(float)animControl->GetCur().sp.GetWidth(),(float)animControl->GetCur().sp.GetHeight()};
	banshee->AddComponent(animControl);

	banshee->AddComponent(new CompMovement{});
	banshee->AddComponent(new CompGravity{250.0f});
	// banshee->AddComponent(new CompHP{100,100,true,false});

	banshee->AddComponent(new CompAI{PassiveAIfunc});

	CompMemory *memory = new CompMemory{};
	memory->floats["pos0x"]=pos.x;
	memory->floats["pos0y"]=pos.y;
	memory->floats["pos1x"]=pos2.x;
	memory->floats["pos1y"]=pos2.y;
	memory->ints["state"]=CompAI::state::idling;
	memory->ints["nextPos"]=0;
	memory->ints["posCount"]=2;
	banshee->AddComponent(memory);

	banshee->team = Team::enemy;
	banshee->size = size;

	return banshee->uid;
}

uint GameObject::MakeMask(const Vec2 &pos){
	GameObject* mask = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	mask->type = "mask";

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
	mask->AddComponent(new CompAI{HostileAIfunc<500,1000,2>});

	CompMemory *memory = new CompMemory{};
	memory->ints["target"]=PLAYER_UID;
	mask->AddComponent(memory);

	mask->team = Team::enemy;
	mask->size = size;

	return mask->uid;
}

uint GameObject::MakePorco(const Vec2 &pos){
	GameObject* pumba = new GameObject{pos,0.0f,Hotspot::BOTTOM};

	pumba->type = "porco";

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
	pumba->AddComponent(new CompAI{PumbaAiFunc<8,800,2,3,1>});

	CompMemory *memory = new CompMemory{};
	memory->ints["state"]=CompAI::state::idling;
	memory->ints["target"]=PLAYER_UID;
	pumba->AddComponent(memory);

	pumba->team = Team::enemy;
	pumba->size = size;

	return pumba->uid;
}

