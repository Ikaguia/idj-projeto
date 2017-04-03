#include <tileSet.hpp>

TileSet::TileSet(int  w,int h,string file):tileWidth{w},tileHeight{h}{
	tileSet.Open(file);
	rows = tileSet.GetHeight()/tileHeight;
	columns = tileSet.GetWidth()/tileWidth;
}

void TileSet::render(unsigned int index,float x,float y){
	if(/*index>=0 && */index<(rows*columns)){
		tileSet.SetClip(tileWidth*(index%columns),(tileHeight*(index/columns)),tileWidth,tileHeight);
		tileSet.render(x,y);
	}
}

int TileSet::getWidth(){
	return tileWidth;
}
int TileSet::getHeight(){
	return tileHeight;
}
