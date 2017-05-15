#ifndef TILEMAPHPP
#define TILEMAPHPP

#include <fstream>

#include <common.hpp>
#include <geometry.hpp>

class TileSet;

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
	int  At(int x,int y,int z) const;
	void Render(int cameraX=0,int cameraY=0) const;
	void RenderLayer(int layer,int cameraX=0,int cameraY=0) const;
	int GetWidth() const;
	int GetHeight() const;
	int GetDepth() const;
};

#endif//TILEMAPHPP