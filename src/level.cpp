#include <level.hpp>

#include <complib.hpp>
#include <game.hpp>
#include <gameObject.hpp>
#include <state.hpp>

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

Level::Level(string file) : tileSet{TileSet()}, tileMap{TileMap(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, &tileSet)} {	
	Load(file);
}

Level::~Level() {
}

void Level::Load(const string& file){
	ifstream in;
	
	in.open(LEVEL_PATH + file + ".txt");
	if(!in.is_open()){
		cerr << "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	
	string parameters;
	//Loading the background:
	getline(in,backgroundFilename);
	if(!backgroundFilename.empty()) background.Open(backgroundFilename);
	in.ignore(1);
	background.StretchToFit(WINSIZE);
	
	//Loading the tileset:
	int tileWidth, tileHeight;
	getline(in, tileSetFilename);
	getline(in, parameters);
	sscanf(parameters.c_str(), " %d,%d", &tileWidth, &tileHeight);
	tileSet.Load(tileWidth, tileHeight, tileSetFilename);
	in.ignore(1);
	
	//Loading the tilemap:
	tileMap.Load(in);
	
	//Loading the collision layer:
	int mapWidth = tileMap.GetWidth();
	int mapHeight = tileMap.GetHeight();
	collisionLayer.clear();
	collisionLayer.resize(mapWidth*mapHeight);
	
	int t,g;
	collisionGroups.clear();
	collisionGroups.reserve(mapWidth*mapHeight);
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			in >> t;
			in.ignore(1);
			collisionLayer[(y*mapWidth)+x] = t-1;
			if(t == EMPTY_TILE) {
				collisionGroups[(y*mapWidth)+x] = 0;
			}
			else {
				in >> g;
				in.ignore(1);
				collisionGroups[(y*mapWidth)+x] = g;
			}
		}
	}
	in.ignore(1);

	//Loading the object list:
	objectList.clear();
	for(string object;getline(in, object);)
		if(!object.empty()) objectList.push_back(object);

	in.close();
}

string Level::Save(const string& file){
	stringstream out;
	ofstream output;
	if(file != ""){ 
		output.open(LEVEL_PATH + file + ".txt");
		if(!output.is_open()) {
			cerr << "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	//Saving the background:
	out<<backgroundFilename<<endl<<endl;
	
	//Saving the tileset:
	out<<tileSetFilename<<endl;
	out<<tileSet.GetWidth()<<","<<tileSet.GetHeight()<<endl<<endl;
	
	//Saving the tilemap:
	tileMap.Save(out);
	
	//Saving the collision layer:
	int mapWidth = tileMap.GetWidth();
	int mapHeight = tileMap.GetHeight();
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			char s[200];
			sprintf(s,"%02d-%03d, ",collisionLayer[(y*mapWidth)+x]+1,collisionGroups[(y*mapWidth)+x]);
			string str(s);
			out << str;
		}
		out << endl;
	}
	out << endl;
	
	if(file == "")
		return out.str();
	output<<out.str();
	output.close();
	return "";
}

void Level::LoadObjects(bool collisors){	
	//Creating the objects:
	char objType[50];
	Vec2 objPos;
	int layer;
	uint uid;
	for(auto& i:objectList){
		if(i.empty()) continue;
		sscanf(i.c_str(), " %s %f %f %d", objType, &objPos.x, &objPos.y, &layer);
		DEBUG(objType);
		DEBUG(objPos);
		uid = GameObject::Create(objType, objPos);
		GAMESTATE.AddObject(uid,layer);
	}
	
	//Setting the collision boxes:
	if(!collisors) return;
	
	int tileWidth = tileSet.GetWidth();
	int tileHeight = tileSet.GetHeight();
	int mapWidth = tileMap.GetWidth();
	int mapHeight = tileMap.GetHeight();
	map<int,pair<Rect,int>> mp;
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			int t = collisionLayer[(y*mapWidth)+x]+1;
			int g = collisionGroups[(y*mapWidth)+x];
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
		int t=it.second.second;

		r.w-=r.x-1;
		r.h-=r.y-1;
		r.x*=tileWidth;
		r.w*=tileWidth;
		r.y*=tileHeight;
		r.h*=tileHeight;

		if(t){
			GameObject *tile = new GameObject{r};
			tile->AddComponent(new CompCollider{CompCollider::collType::t_ground});
			GAMESTATE.AddObject(tile->uid);
		}
	}
}

void Level::SaveObjects(const vector<pair<ii,ii>>& grouped){
	//Saving the collision groups:
	int mapWidth = tileMap.GetWidth();
	int mapHeight = tileMap.GetHeight();
	int id=1;
	map<ii,int> ids;
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			if(collisionLayer[(y*mapWidth)+x]==EMPTY_TILE) collisionGroups[(y*mapWidth)+x] = 0;
			else{
				auto &group = grouped[(mapWidth*y)+x];
				if(group.first.first==x && group.first.second==y)ids[group.first]=id++;
				collisionGroups[(y*mapWidth)+x] = ids[group.first];
			}
		}
	}
}

bool Level::operator==(Level& level){
	if(Save() == level.Save()) return true;
	return false;
}
