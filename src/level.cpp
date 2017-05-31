#include <level.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <complib.hpp>

#define DEFAULT_BACKGROUND "img/mountain_bg.jpg"
#define DEFAULT_TILE_SIZE 64
#define DEFAULT_TILESET "img/tileset.png"
#define DEFAULT_MAP_WIDTH 50
#define DEFAULT_MAP_HEIGHT 50

Level::Level() : background{Sprite(DEFAULT_BACKGROUND)}, tileSet{TileSet(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE, DEFAULT_TILESET)}, tileMap{TileMap(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, &tileSet)} {
	collisionLayer.clear();
	collisionLayer.resize(DEFAULT_MAP_WIDTH*DEFAULT_MAP_HEIGHT);
	FOR(y,DEFAULT_MAP_HEIGHT){
		FOR(x,DEFAULT_MAP_WIDTH){
			collisionLayer[(y*DEFAULT_MAP_WIDTH)+x] = EMPTY_TILE;
		}
	}
}

Level::Level(string file,set<unique_ptr<GameObject>>* entities) : tileSet{TileSet()}, tileMap{TileMap(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, &tileSet)} {
	//TODO: Remove this when the collision layer is read from the file
	collisionLayer.clear();
	collisionLayer.resize(DEFAULT_MAP_WIDTH*DEFAULT_MAP_HEIGHT);
	
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
	background.StretchToFit(WINSIZE);
	
	int tileWidth, tileHeight;
	getline(in, tileSetFilename);
	getline(in, parameters);
	sscanf(parameters.c_str(), " %d,%d", &tileWidth, &tileHeight);
	tileSet.Load(tileWidth, tileHeight, tileSetFilename);
	in.ignore(1);
	
	tileMap.Load(in);
	
	int mapWidth = tileMap.GetWidth();
	int mapHeight = tileMap.GetHeight();
	collisionLayer.clear();
	collisionLayer.reserve(mapWidth*mapHeight);

	int t;
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			in >> t;
			in.ignore(1);
			collisionLayer[(y*mapWidth)+x] = t-1;
		}
	}
	//in.ignore(1);
	
	if(entities!=nullptr) {
		FOR(y,mapHeight) {
			FOR(x,mapWidth) {
				t = collisionLayer[(y*mapWidth)+x];
				if(t!=EMPTY_TILE) {
					GameObject *tile = new GameObject{Rect((x*tileWidth),(y*tileHeight),tileWidth,tileHeight)};
					tile->AddComponent(new CompCollider{CompCollider::collType::t_ground});
					//tile->AddComponent(new CompStaticRender{Sprite{"img/point_yellow.jpg"},Vec2{0,0}});
					entities->insert(unique_ptr<GameObject>(tile));
				
			
				}
			}
		}
	}

	//TODO: Add collision and remove this
	/*char c;
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
	}*/
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
	
	int mapWidth = tileMap.GetWidth();
	int mapHeight = tileMap.GetHeight();
	
	//Collision
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			out<<collisionLayer[(y*mapWidth)+x]+1<<",\t";
		}
		out<<endl;
	}
	out<<endl;
	
	//TODO: Add back collision and remove this
	//out << remaining;

	out.close();
}

