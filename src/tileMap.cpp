#include <tileMap.hpp>

#include <camera.hpp>
#include <componentCollider.hpp>
#include <componentStaticRender.hpp>
#include <gameObject.hpp>
#include <tileSet.hpp>

TileMap::TileMap(int width, int height, TileSet* ts) : mapWidth{width},mapHeight{height},tileSet{ts},mapDepth{1} {
	tileMatrix.reserve(mapWidth*mapHeight);
	FOR(h, mapHeight)
		FOR(w, mapWidth)
			At(w, h, 0) = EMPTY_TILE;
}

TileMap::TileMap(string file,TileSet* ts,set<unique_ptr<GameObject>> *entities):tileSet{ts}{
}

void TileMap::Load(ifstream& in){
	string line;
	
	getline(in,line);
	sscanf(line.c_str()," %d,%d,%d",&mapWidth,&mapHeight,&mapDepth);
	
	tileMatrix.clear();
	tileMatrix.reserve(mapWidth*mapHeight*mapDepth);

	int t;
	FOR(d,mapDepth){
		FOR(h,mapHeight){
			FOR(w,mapWidth){
				in >> t;
				in.ignore(1);
				At(w,h,d) = t-1;
			}
		}
	}
}

void TileMap::Save(ofstream& out) {
	out<<mapWidth<<","<<mapHeight<<","<<mapDepth<<endl<<endl;
	
	FOR(d,mapDepth){
		FOR(h,mapHeight){
			FOR(w,mapWidth){
				out<<At(w,h,d)+1<<",\t";
			}
			out<<endl;
		}
		out<<endl;
	}
}

void TileMap::SetTileSet(TileSet* ts){
	tileSet = ts;
}

int& TileMap::At(int x,int y,int z){
	return tileMatrix[x+(y*mapWidth)+(z*mapWidth*mapHeight)];
}

int TileMap::At(int x,int y,int z) const{
	return tileMatrix[x+(y*mapWidth)+(z*mapWidth*mapHeight)];
}

void TileMap::RenderLayer(int layer,int posX ,int posY){
	int w=tileSet->GetWidth();
	int h=tileSet->GetHeight();
	int tile;
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			tile = At(x, y, layer);
			if(tile != EMPTY_TILE)
				tileSet->Render(tile, RENDERPOSX(posX+(x*w)), RENDERPOSY(posY+(y*h)), CAMERAZOOM);
		}
	}
}

void TileMap::Render(Vec2 pos){
	FOR(i,mapDepth){
		RenderLayer(i,pos.x,pos.y);
	}
}

int TileMap::GetWidth() const{
	return mapWidth;
}

int TileMap::GetHeight() const{
	return mapHeight;
}

int TileMap::GetDepth() const{
	return mapDepth;
}

void TileMap::SetWidth(const int& w) {
	mapWidth = w;
}

void TileMap::SetHeight(const int& h) {
	mapHeight = h;
}

void TileMap::SetDepth(const int& d) {
	mapDepth = d;
}

