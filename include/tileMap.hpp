#ifndef TILEMAPHPP
#define TILEMAPHPP

#include <fstream>

#include <common.hpp>

#ifndef TILESETHPP
class TileSet;
#endif//TILESETHPP

class TileMap{
	vector<int> tileMatrix;
	TileSet* tileSet;
	int mapWidth;
	int mapHeight;
	int mapDepth;
public:
	TileMap(string file,TileSet* ts);
	void load(string file);
	void setTileSet(TileSet* ts);
	int& at(int x,int y,int z);
	void render(int cameraX=0,int cameraY=0);
	void renderLayer(int layer,int cameraX=0,int cameraY=0);
	int getWidth();
	int getHeight();
	int getDepth();
};

#endif//TILEMAPHPP