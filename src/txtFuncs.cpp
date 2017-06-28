#include <txtFuncs.hpp>
#include <gameObject.hpp>
#include <complib.hpp>
#include <game.hpp>
//#include <camera.hpp>
//#include <inputManager.hpp>

#define FUNC(N,T) {#N, N<T>}
#define MAP(N,T) map<string,txtFuncType2(T)> N = {\
	/*FUNC(AddParticle,T),*/\
	FUNC(AddSprite,T),\
	FUNC(AddVar,T),\
	FUNC(ChangeVar,T),\
	FUNC(Damage,T),\
	FUNC(DamageArea,T),\
	FUNC(DamageAreaFixed,T),\
	FUNC(FireProjectile,T),\
	FUNC(Remove,T),\
}

MAP(txtFuncsF,ifstream);
MAP(txtFuncsS,istringstream);

// template<class T> txtFuncType1 AddParticle(T& in){}
template<class T> txtFuncType1 AddSprite(T& in){
	Vec2 pos;
	in >> pos.x >> pos.y;
	return [pos](GameObject* self){
		GameObject *spr = new GameObject{pos};
		spr->flipped=true;
		spr->rotation=self->rotation;
		Sprite sp;
		if     (self->hasComponent[Component::type::t_animation])        sp = COMPANIMp(self)->sp;
		else if(self->hasComponent[Component::type::t_animation_control])sp = COMPANIMCONTp(self)->GetCur().sp;
		else if(self->hasComponent[Component::type::t_static_render])    sp = COMPSTATICRENDERp(self)->sp;
		sp.SetFrameTime(-1.0f);
		spr->AddComponent(new CompStaticRender{sp,Vec2{}});
		GAMESTATE.AddObject(spr);
	};
}
template<class T> txtFuncType1 AddVar(T& in){
	string type,name;
	in >> type >> name;
	if(type=="int"){
		int val;
		in >> val;
		return [name,val](GameObject* self){
			if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
			COMPMEMORYp(self)->ints[name]+=val;
		};
	}
	if(type=="float"){
		float val;
		in >> val;
		return [name,val](GameObject* self){
			if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
			COMPMEMORYp(self)->floats[name]+=val;
		};
	}

	cout << "AddVar called with invalid type '" << type << "'" << endl;
	return txtFuncType1{};
}
template<class T> txtFuncType1 ChangeVar(T& in){
	string type,name;
	in >> type >> name;
	if(type=="string"){
		string val;
		in >> val;
		return [name,val](GameObject* self){
			if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
			COMPMEMORYp(self)->strings[name]=val;
		};
	}
	if(type=="int"){
		int val;
		in >> val;
		return [name,val](GameObject* self){
			if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
			COMPMEMORYp(self)->ints[name]=val;
		};
	}
	if(type=="float"){
		float val;
		in >> val;
		return [name,val](GameObject* self){
			if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
			COMPMEMORYp(self)->floats[name]=val;
		};
	}
	if(type=="timer"){
		return [name](GameObject* self){
			if(!self->hasComponent[Component::type::t_memory])self->AddComponent(new CompMemory{});
			COMPMEMORYp(self)->timers[name].Restart();
		};
	}

	cout << "ChangeVar called with invalid type '" << type << "'" << endl;
	return txtFuncType1{};
}
template<class T> txtFuncType1 Damage(T& in){
	int dmgLow,dmgHigh;
	in >> dmgLow >> dmgHigh;
	dmgHigh=max(dmgHigh,dmgLow+1);
	return [dmgLow,dmgHigh](GameObject* self){
		int dmg = dmgLow + (rand()%(dmgHigh-dmgLow));
		if(self->hasComponent[Component::type::t_hp])COMPHPp(self)->Damage(dmg);
	};
}
template<class T> txtFuncType1 DamageArea(T& in){
	bool dmgSelf;
	int dmgLow,dmgHigh;
	float x,y,w,h,r;
	in >> x >> y >> w >> h >> r >> dmgLow >> dmgHigh >> dmgSelf;
	Rect rect{x,y,w,h};
	dmgHigh=max(dmgHigh,dmgLow+1);
	return [rect,r,dmgLow,dmgHigh,dmgSelf](GameObject* self){
		Rect area = self->Box();
		area.w *= rect.w;
		area.h *= rect.h;
		if(self->flipped)area.x += self->Box().w * rect.x;
		else area.x -= (self->Box().w * rect.x) + (area.w - self->Box().w);
		area.y += self->Box().h * rect.y;

		SET_COLOR4(255,255,255,200);
		SDL_Rect r = (area.renderBox().sdlRect());
		FILL_RECT(&r);

		set<uint> gos = GAMESTATE.GetEntitiesInRange(rect.x,rect.x2());
		for(uint go:gos){
			if(dmgSelf || ENTITY(go)->team != self->team){
				//TODO: change collision to work with rotation
				if(ENTITY(go)->hasComponent[Component::type::t_hp] && area.collides(ENTITY(go)->Box())){
					COMPHPp(ENTITY(go))->Damage(dmgLow+(rand()%(dmgHigh-dmgLow)));
				}
			}
		}
	};
}
template<class T> txtFuncType1 DamageAreaFixed(T& in){
	bool dmgSelf;
	int dmgLow,dmgHigh;
	float x,y,w,h,r;
	in >> x >> y >> w >> h >> r >> dmgLow >> dmgHigh >> dmgSelf;
	Rect rect{x,y,w,h};
	dmgHigh=max(dmgHigh,dmgLow+1);
	return [rect,r,dmgLow,dmgHigh,dmgSelf](GameObject* self){
		Vec2 source = self->pos + Vec2::makeVec2(self->size.x,self->rotation);
		Rect area = {0,0,rect.w,rect.h};
		area.setCenter(source);
		area += rect.corner();
		if(self->flipped)area.x += self->Box().w * rect.x;
		else area.x -= (self->Box().w * rect.x) + (area.w - self->Box().w);
		area.y += self->Box().h * rect.y;

		// SET_COLOR4(255,255,255,200);
		// SDL_Rect r = (area.renderBox().sdlRect());
		// FILL_RECT(&r);

		set<uint> gos = GAMESTATE.GetEntitiesInRange(rect.x,rect.x2());
		for(uint go:gos){
			if(dmgSelf || ENTITY(go)->team != self->team){
				//TODO: change collision to work with rotation
				if(ENTITY(go)->hasComponent[Component::type::t_hp] && area.collides(ENTITY(go)->Box())){
					COMPHPp(ENTITY(go))->Damage(dmgLow+(rand()%(dmgHigh-dmgLow)));
				}
			}
		}
	};
}
template<class T> txtFuncType1 FireProjectile(T& in){
	int count;
	float x,y,f,r;
	string projFile,animFile,cur,target,funcN;
	in >> x >> y >> f >> r >> projFile;

	map<string,int> vars;
	vector<pair<string,txtFuncType1>> start,hitEnemy,hitAlly,hitBlock;

	ifstream file(PROJECTILE_PATH + projFile + ".txt");
	if(!file.is_open()){
		cerr << "Erro ao abrir arquivo " << projFile << endl;
		exit(EXIT_FAILURE);
	}
	file >> animFile;
	while(file >> cur >> count){
		if(cur=="start" || cur=="hit_enemy" || cur=="hit_ally" || cur=="hit_block"){
			FOR(i,count){
				file >> target >> funcN;
				if(cur == "start")start.push_back(make_pair(target,txtFuncsF[funcN](file)));
				else if(cur == "hit_enemy")hitEnemy.push_back(make_pair(target,txtFuncsF[funcN](file)));
				else if(cur == "hit_ally")hitAlly.push_back(make_pair(target,txtFuncsF[funcN](file)));
				else if(cur == "hit_block")hitBlock.push_back(make_pair(target,txtFuncsF[funcN](file)));
			}
		}
		else vars[cur] = count;
	}
	file.close();

	return [x,y,f,r,animFile,vars,start,hitAlly,hitEnemy,hitBlock](GameObject* owner){
		Vec2 pos = owner->Box().relativePos({x,y},owner->flipped);
		float ang=-r;
		if(!owner->flipped)ang = 180-ang;

		GameObject* bullet = new GameObject{pos,ang,Hotspot::LEFT};

		CompCollider collider{CompCollider::collType::t_bullet};
		collider.colls[0].useDefault[CompCollider::collType::t_bullet] = []
			(const CompCollider::Coll &a,const CompCollider::Coll &b){UNUSED(a);UNUSED(b);};

		auto foo1 = [owner,vars,hitAlly,hitEnemy](const CompCollider::Coll &a,const CompCollider::Coll &b){
			bool isAlly = ENTITY(a.entity)->team == ENTITY(b.entity)->team;


			Vec2 &totMove=COMPMOVEp(ENTITY(a.entity).get())->move;
			if(totMove==Vec2{})return;

			Vec2 move=a.Collides(b,totMove);

			if(move!=totMove){
				if(isAlly){
					for(auto &pfunc:hitAlly){
						if(pfunc.first=="owner")pfunc.second(owner);
						if(pfunc.first=="self")pfunc.second(ENTITY(a.entity).get());
						if(pfunc.first=="target")pfunc.second(ENTITY(b.entity).get());
					}
				}
				else{
					for(auto &pfunc:hitEnemy){
						if(pfunc.first=="owner")pfunc.second(owner);
						if(pfunc.first=="self")pfunc.second(ENTITY(a.entity).get());
						if(pfunc.first=="target")pfunc.second(ENTITY(b.entity).get());
					}
				}
				if(!ENTITY(b.entity)->hasComponent[Component::type::t_movement]){
					if((isAlly && vars.count("stick_ally")==0) || (!isAlly && vars.count("stick_enemy")==0))return;
					Vec2 pos = ENTITY(a.entity)->Box().corner() + move + totMove/4.0f;
					auto &func = txtFuncsS["AddSprite"];
					istringstream iss(to_string(pos.x) + to_string(pos.y));
					func(iss)(ENTITY(a.entity).get());
					GAMESTATE.GetLastObject()->AttachTo(ENTITY(b.entity).get());
				}
			}
		};
		collider.colls[0].useDefault[CompCollider::collType::t_any] = foo1;

		auto foo2 = [owner,vars,hitBlock](const CompCollider::Coll &a,const CompCollider::Coll &b){
			Vec2 &totMove=COMPMOVEp(ENTITY(a.entity).get())->move;
			if(totMove==Vec2{})return;

			Vec2 move=a.Collides(b,totMove);

			if(move!=totMove){
				for(auto &pfunc:hitBlock){
					if(pfunc.first=="owner")pfunc.second(owner);
					if(pfunc.first=="self")pfunc.second(ENTITY(a.entity).get());
					if(pfunc.first=="target")pfunc.second(ENTITY(b.entity).get());
				}
				if(vars.count("stick_block")==1){
					Vec2 pos = ENTITY(a.entity)->pos + move + totMove/4.0f;
					auto &func = txtFuncsS["AddSprite"];
					istringstream iss(to_string(pos.x) + " " + to_string(pos.y));
					func(iss)(ENTITY(a.entity).get());
					GAMESTATE.GetLastObject()->AttachTo(ENTITY(b.entity).get());
				}
			}
		};
		collider.colls[0].useDefault[CompCollider::collType::t_ground] = foo2;
		collider.colls[0].useDefault[CompCollider::collType::t_h_ground] = foo2;

		CompAnim* anim = new CompAnim{animFile,&collider};
		Vec2 size{(float)anim->sp.GetWidth(),(float)anim->sp.GetHeight()};
		bullet->AddComponent(anim);

		Vec2 mv;
		if(owner->hasComponent[Component::type::t_movement])mv=COMPMOVEp(owner)->move;
		bullet->AddComponent(new CompMovement{Vec2::makeVec2(f,ang)+mv,CompMovement::moveType::t_bullet});

		bullet->AddComponent(new CompGravity{500.0f});

		bullet->team = owner->team;
		bullet->flipped = true;
		bullet->rotation = ang;
		bullet->size = size;

		if(owner->flipped)bullet->pos.x -= size.x;

		GAMESTATE.AddObject(bullet);

		for(auto &pfunc:start){
			if(pfunc.first=="owner")pfunc.second(owner);
			if(pfunc.first=="self")pfunc.second(bullet);
		}
	};
}
template<class T> txtFuncType1 Remove(T& in){
	UNUSED(in);
	return [](GameObject* self){
		self->dead = true;
	};
}
