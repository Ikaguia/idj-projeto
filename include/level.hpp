#ifndef LEVELHPP
#define LEVELHPP

#include <common.hpp>

#include <sprite.hpp>
#include <tileMap.hpp>
#include <tileSet.hpp>

class Level {
	string backgroundFilename;
	string tileSetFilename;
public:
	Sprite background;
	TileSet tileSet;
	TileMap tileMap;
	vector<int> collisionLayer;
	
	Level();
	Level(string file,bool collisors=true);
	~Level();
	
	void Load(string file,bool collisors=true);
	void Save(string file,vector<pair<ii,ii>> grouped);
	
};
#endif //LEVELHPP
