#include <tileSet.hpp>

TileSet::TileSet(int  w,int h,string file):tileWidth{w},tileHeight{h}{
	tileSet.Open(file);
	rows = tileSet.GetHeight()/tileHeight;
	columns = tileSet.GetWidth()/tileWidth;
}

void TileSet::Render(unsigned int index,float x,float y){
	if(/*index>=0 && */(int)index<(rows*columns)){
		tileSet.SetClip(tileWidth*(index%columns),(tileHeight*(index/columns)),tileWidth,tileHeight);
		tileSet.Render(x,y);
	}
}

int TileSet::GetWidth(){
	return tileWidth;
}
int TileSet::GetHeight(){
	return tileHeight;
}
