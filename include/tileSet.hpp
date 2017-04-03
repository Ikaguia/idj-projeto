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
	TileSet(int  w,int h,string file);
	void render(unsigned int index,float x,float y);
	int getWidth();
	int getHeight();
};

#endif//TILESETHPP