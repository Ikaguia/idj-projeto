#include <gameObject.hpp>
#include <game.hpp>
#include <componentMovement.hpp>
#include <camera.hpp>
#include <complib.hpp>

GameObject::GameObject(){}
GameObject::GameObject(const Rect &rec,float r,bool a):box{rec},rotation{r},anchored{a}{}
GameObject::~GameObject(){
	UnAttach();
	for(GameObject* obj:attachedObjs)obj->dead=true;

	FOR(i,Component::type::t_count)if(hasComponent[i])delete components[i];
	if(Camera::focus==this)Camera::Unfollow();
}

void GameObject::Update(float time){
	if(hasComponent[Component::type::t_input_control])components[Component::type::t_input_control]->Update(time);

	if(hasComponent[Component::type::t_movement]){
		CompMovement *compM = (CompMovement*) components[Component::type::t_movement];
		compM->move=compM->speed*time;
	}

	FOR2(i,Component::type::t_collider,Component::type::t_count)if(hasComponent[i])components[i]->Update(time);
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
	component->entity=this;
}

void GameObject::ReplaceComponent(Component* component){
	auto t=component->GetType();
	if(!hasComponent[t]){
		cout << "Error, replacing component " << t << " on a GameObject that doesnt have it" << endl;
		hasComponent[t]=true;
	}
	else delete components[t];
	components[t]=component;
	component->entity=this;
}

void GameObject::RemoveComponent(Component::type t){
	if(!hasComponent[t])cout << "Error, removing component " << t << " on a GameObject that doesnt have it" << endl;
	else{
		delete components[t];
		components[t]=nullptr;
		hasComponent[t]=false;
	}
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
	if     (INPUTMAN.KeyPress(KEY_UP))speed.y=-1500.0f;
	if     (INPUTMAN.IsKeyDown(KEY_LEFT) && !INPUTMAN.IsKeyDown(KEY_RIGHT))speed.x=max(-400.0f,speed.x-800*time);
	else if(INPUTMAN.IsKeyDown(KEY_RIGHT) && !INPUTMAN.IsKeyDown(KEY_LEFT))speed.x=min( 400.0f,speed.x+800*time);
	else if(speed.x>0.0f)speed.x=max(0.0f,speed.x-800*time);
	else if(speed.x<0.0f)speed.x=min(0.0f,speed.x+800*time);


	if(INPUTMAN.KeyPress(KEY(z))){
		Vec2 pos{go->box.x+go->box.w+20,go->box.y};
		float force=1000 + (1000-rand()%2000)/10.0f;
		float angle=       (1000-rand()%2000)/100.0f;
		GAMESTATE.AddObject(GameObject::MakeBullet(pos,"img/arrow.png",force,angle,true));
	}
}
GameObject* GameObject::MakePlayer(const Vec2 &pos){
	CompStaticRender* img = new CompStaticRender{Sprite{"img/player_static.jpg"},Vec2{}};
	GameObject* player = new GameObject{Rect{pos.x,pos.x,(float)img->sp.GetWidth(),(float)img->sp.GetHeight()}};
	player->AddComponent(new CompInputControl{PlayerControlFunc});
	player->AddComponent(img);
	player->AddComponent(new CompMovement{});
	player->AddComponent(new CompCollider{CompCollider::collType::t_player});
	player->AddComponent(new CompGravity{2500.0f});
	player->AddComponent(new CompHP{100,100,true,false});
	return player;
}


void BulletBulletCollision(const CompCollider* a,const CompCollider* b){}
void BulletAnyCollision1(const CompCollider* a,const CompCollider* b){
	Vec2 &speed=((CompMovement*)a->entity->components[Component::type::t_movement])->speed;

	if(speed==Vec2{})return;

	Vec2 &totMove=((CompMovement*)a->entity->components[Component::type::t_movement])->move;
	Vec2 move=a->collides(b,totMove,a->entity->box+move);

	if(move!=totMove){
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

	GameObject *arrow = new GameObject{{pos.x,pos.y,(float)img->sp.GetWidth(),(float)img->sp.GetHeight()}};
	arrow->AddComponent(img);
	arrow->AddComponent(new CompMovement{Vec2::makeVec2(force,angle),CompMovement::moveType::t_bullet});

	CompCollider *collider = new CompCollider{CompCollider::collType::t_bullet};
	collider->useDefault[CompCollider::collType::t_bullet] = BulletBulletCollision;
	if(stick)collider->useDefault[CompCollider::collType::t_any] = BulletAnyCollision1;
	else     collider->useDefault[CompCollider::collType::t_any] = BulletAnyCollision2;
	arrow->AddComponent(collider);

	arrow->AddComponent(new CompGravity{500.0f});
	return arrow;
}
