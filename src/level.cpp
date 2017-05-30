#include <level.hpp>
#include <gameObject.hpp>
#include <complib.hpp>

Level::Level() : background{Sprite("img/stage_bg.jpg")}, tileSet{TileSet(64, 64, "img/tileset.png")}, tileMap{TileMap(50, 50, &tileSet)} {

}

Level::Level(string file,set<unique_ptr<GameObject>>* entities) : tileSet{TileSet()}, tileMap{TileMap(50, 50, &tileSet)} {
	Load(file,entities);
}

Level::~Level() {
}

void Level::Load(string file,set<unique_ptr<GameObject>>* entities) {
	ifstream in;
	
	in.open(file);
	if(!in.is_open()){
		cout << "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	
	string parameters;
	
	getline(in,backgroundFilename);
	if(!backgroundFilename.empty()) background.Open(backgroundFilename);
	in.ignore(1);
	
	int tileWidth, tileHeight;
	getline(in, tileSetFilename);
	getline(in, parameters);
	sscanf(parameters.c_str(), " %d,%d", &tileWidth, &tileHeight);
	tileSet.Load(tileWidth, tileHeight, tileSetFilename);
	in.ignore(1);
	
	tileMap.Load(in);


	//TODO: Add collision and remove this
	char c;
	while(in >> c)remaining+=c;

	if(entities!=nullptr){
		int x,y;
		float f;
		stringstream ss(remaining);

		ss >> f;
		ss.ignore(1);
		int mapWidth =tileMap.GetWidth();
		int mapHeight=tileMap.GetHeight();
		map<ii,Rect> mp;
		FOR(h,mapHeight){
			FOR(w,mapWidth){
				ss >> x;
				ss.ignore(1);
				if(x==1 || x==2){
					ss >> y;
					ss.ignore(1);
					if(!mp.count(ii{x,y}))mp[ii{x,y}]=Rect{(float)mapWidth+1,(float)mapHeight+1,(float)-1,(float)-1};//default vals to make min and max work
					else{
						mp[ii{x,y}].x=min(mp[ii{x,y}].x,(float)w);
						mp[ii{x,y}].y=min(mp[ii{x,y}].y,(float)h);
						mp[ii{x,y}].w=max(mp[ii{x,y}].w,(float)w);
						mp[ii{x,y}].h=max(mp[ii{x,y}].h,(float)h);
					}
				}
			}
		}
		for(auto &it:mp){
			Rect r = it.second;
			r.w-=r.x-1;
			r.h-=r.y-1;
			if(it.first.first==2){
				r.y+=f;
				r.h-=f;
			}
			r.x*=tileWidth;
			r.w*=tileWidth;
			r.y*=tileHeight;
			r.h*=tileHeight;
			GameObject *tile = new GameObject{r};
			if(it.first.first==1 || it.first.first==2)tile->AddComponent(new CompCollider{CompCollider::collType::t_ground});
			else if                (it.first.first==3)tile->AddComponent(new CompCollider{CompCollider::collType::t_h_ground});
			tile->AddComponent(new CompStaticRender{Sprite{"img/point_yellow.jpg"},Vec2{0,0}});
			entities->insert(unique_ptr<GameObject>(tile));
		}
	}
	in.close();
}

void Level::Save(string file,set<unique_ptr<GameObject>>* entities) {
	ofstream out;
	
	out.open(file);
	if(!out.is_open()) {
		cout<< "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	
	out<<backgroundFilename<<endl<<endl;
	
	out<<tileSetFilename<<endl;
	out<<tileSet.GetWidth()<<","<<tileSet.GetHeight()<<endl<<endl;
	
	tileMap.Save(out);
	
	//TODO: Add back collision and remove this
	out << remaining;

	out.close();
}
