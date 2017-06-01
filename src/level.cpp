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

	int t,g;
	int grouped[mapWidth][mapHeight];
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			in >> t;
			in.ignore(1);
			in >> g;
			in.ignore(1);
			collisionLayer[(y*mapWidth)+x] = t-1;
			grouped[x][y]=g;
		}
	}
	//in.ignore(1);
	
	if(entities!=nullptr){
		map<int,pair<Rect,int>> mp;
		FOR(y,mapHeight){
			FOR(x,mapWidth){
				t = collisionLayer[x+(y*mapWidth)]+1;
				g = grouped[x][y];
				if(t){
					if(!mp.count(g)){
						mp[g]=make_pair(Rect{(float)mapWidth+1,(float)mapHeight+1,(float)-1,(float)-1},t);//default vals to make min and max work
					}
					mp[g].first.x=min(mp[g].first.x,(float)x);
					mp[g].first.y=min(mp[g].first.y,(float)y);
					mp[g].first.w=max(mp[g].first.w,(float)x);
					mp[g].first.h=max(mp[g].first.h,(float)y);
				}
			}
		}
		for(auto &it:mp){
			Rect r = it.second.first;
			t=it.second.second;

			r.w-=r.x-1;
			r.h-=r.y-1;
			r.x*=tileWidth;
			r.w*=tileWidth;
			r.y*=tileHeight;
			r.h*=tileHeight;

			if(t){
				GameObject *tile = new GameObject{r};
				tile->AddComponent(new CompCollider{CompCollider::collType::t_ground});
				tile->AddComponent(new CompStaticRender{Sprite{"img/point_yellow.jpg"},Vec2{0,0}});
				entities->insert(unique_ptr<GameObject>(tile));
			}
		}
	}

	in.close();
}

void Level::Save(string file,vector<pair<ii,ii>> grouped) {
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
	int id=1;
	map<ii,int> ids;
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			char s[200];
			if(collisionLayer[(y*mapWidth)+x]==-1)sprintf(s,"0-000, ");
			else{
				auto &group = grouped[x+mapWidth*y];
				if(group.first.first==x && group.first.second==y)ids[group.first]=id++;
				sprintf(s,"%d-%03d, ",collisionLayer[(y*mapWidth)+x]+1,ids[group.first]);
			}
			string str(s);
			out << str;

			// out<<collisionLayer[(y*mapWidth)+x]+1 << "-" << grouped[x][y] <<",\t";
		}
		out << endl;
	}
	out << endl;

	out.close();
}

