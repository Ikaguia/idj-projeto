#ifndef TILEMAPHPP
#define TILEMAPHPP

#include <common.hpp>

#include <geometry.hpp>

class TileSet;
class GameObject;

class TileMap{
	vector<int> tileMatrix;
	TileSet* tileSet;
	int mapWidth;
	int mapHeight;
	int mapDepth;
public:
	TileMap(string file,TileSet* ts,set<unique_ptr<GameObject>> *entities);
	void Load(string file,set<unique_ptr<GameObject>> *entities);
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
