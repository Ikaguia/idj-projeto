#ifndef TILEMAPHPP
#define TILEMAPHPP

#include <fstream>

#include <common.hpp>
#include <geometry.hpp>

#ifndef TILESETHPP
class TileSet;
#endif//TILESETHPP

class TileMap{
	vector<int> tileMatrix;
	vector<int> tileMatrixMeta;
	TileSet* tileSet;
	int mapWidth;
	int mapHeight;
	int mapDepth;
public:
	TileMap(string file,TileSet* ts);
	void Load(string file);
	void SetTileSet(TileSet* ts);
	int& At(int x,int y,int z);
	const int& At(int x,int y,int z) const;
	int& AtMeta(int x,int y);
	const int& AtMeta(int x,int y) const;
	void Render(int cameraX=0,int cameraY=0) const;
	void RenderLayer(int layer,int cameraX=0,int cameraY=0) const;
	int GetWidth() const;
	int GetHeight() const;
	int GetDepth() const;
	void GetIndAtPos(const int x,const int y,int &outX,int &outY) const;
	bool Collides(const int &x1,const int &x2,const int &y1,const int &y2,const ConvexPolygon& pol,ii& ret)const;
};

#endif//TILEMAPHPP