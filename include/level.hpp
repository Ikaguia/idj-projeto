#ifndef LEVELHPP
#define LEVELHPP

#include <common.hpp>

#include <sprite.hpp>
#include <tileMap.hpp>
#include <tileSet.hpp>

class State;

class Level {
	string backgroundFilename;
	string tileSetFilename;
public:
	Sprite background;
	TileSet tileSet;
	TileMap tileMap;
	vector<int> collisionLayer;
	
	Level();
	Level(string file,State* state=nullptr);
	~Level();
	
	void Load(string file,State* state=nullptr);
	void Save(string file,vector<pair<ii,ii>> grouped);
	
};
#endif //LEVELHPP
