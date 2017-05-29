#include <tileSet.hpp>

TileSet::TileSet() {
}

TileSet::TileSet(int w, int h, string file) {
	Load(w, h, file);
}

void TileSet::Load(int  w, int h, string file) {
	tileWidth = w;
	tileHeight = h;
	tileSet.Open(file);
	rows = tileSet.GetHeight()/tileHeight;
	columns = tileSet.GetWidth()/tileWidth;
}

void TileSet::Render(unsigned int index,float x,float y, float extScale){
	if((int)index<(rows*columns)){
		tileSet.SetClip(tileWidth*(index%columns),(tileHeight*(index/columns)),tileWidth,tileHeight);
		tileSet.Render(x,y,0,extScale);
	}
}

int TileSet::GetWidth(){
	return tileWidth;
}

int TileSet::GetHeight(){
	return tileHeight;
}

int TileSet::GetTileCount() {
	return rows*columns;
}
