#ifndef TILEMAPHPP
#define TILEMAPHPP

#include <common.hpp>

#include <geometry.hpp>

#define EMPTY_TILE -1

class TileSet;
class GameObject;

class TileMap{
	vector<int> tileMatrix;
	TileSet* tileSet;
	int mapWidth;
	int mapHeight;
	int mapDepth;
public:
	TileMap(int width, int height, TileSet* ts);
	TileMap(TileSet* ts);
	
	void Load(ifstream& in);
	void Save(ofstream& out);
	
	void SetTileSet(TileSet* ts);
	
	int& At(int x,int y,int z);
	int  At(int x,int y,int z) const;
	
	void Render(Vec2 pos = {0,0});
	void RenderLayer(int layer,int posX=0,int posY=0);
	
	int GetWidth() const;
	int GetHeight() const;
	int GetDepth() const;
	void SetSize(int width,int height);
};

#endif//TILEMAPHPP
