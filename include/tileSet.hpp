#ifndef TILESETHPP
#define TILESETHPP

#include <common.hpp>

#include <sprite.hpp>

class TileSet{
	Sprite tileSet;

	int rows;
	int columns;

	int tileWidth;
	int tileHeight;
public:
	TileSet();
	TileSet(int  w,int h,string file);
	
	void Load(int w, int h, string file);
	
	void Render(unsigned int index,float x,float y, float extScale = 1.0f);
	
	int GetWidth();
	int GetHeight();
	int GetTileCount();
};

#endif//TILESETHPP
