#ifndef LEVELHPP
#define LEVELHPP

#include <common.hpp>

#include <sprite.hpp>
#include <tileMap.hpp>
#include <tileSet.hpp>

class Level {
	string tileSetFilename;
	string backgroundFilename;
	vector<string> objectList;
	vector<int> collisionGroups;
public:
	Sprite background;
	TileSet tileSet;
	TileMap tileMap;
	vector<int> collisionLayer;
	
	Level();
	Level(string file);
	~Level();
	
	void Load(const string& file);
	string Save(const string& file="");
	
	void LoadObjects(bool collisors=true);
	void SaveObjects(const vector<pair<ii,ii>>& grouped);
	
	bool operator==(Level& level);
};
#endif //LEVELHPP
