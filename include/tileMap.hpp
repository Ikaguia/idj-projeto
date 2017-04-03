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
	void Load(string file);
	void SetTileSet(TileSet* ts);
	int& At(int x,int y,int z);
	void Render(int cameraX=0,int cameraY=0);
	void RenderLayer(int layer,int cameraX=0,int cameraY=0);
	int GetWidth();
	int GetHeight();
	int GetDepth();
};

#endif//TILEMAPHPP