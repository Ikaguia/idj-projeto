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
	
	Level();
	Level(string file);
	~Level();
	
	void Load(string file);
	void Save(string file);
	
};
#endif //LEVELHPP
