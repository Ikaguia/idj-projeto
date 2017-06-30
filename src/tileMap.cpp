#include <tileMap.hpp>

#include <camera.hpp>
#include <componentCollider.hpp>
#include <componentStaticRender.hpp>
#include <gameObject.hpp>
#include <tileSet.hpp>

TileMap::TileMap(int width, int height, TileSet* ts) : tileSet{ts},mapWidth{width},mapHeight{height},mapDepth{1} {
	tileMatrix.reserve(mapWidth*mapHeight);
	FOR(h, mapHeight)
		FOR(w, mapWidth)
			At(w, h, 0) = EMPTY_TILE;
}
TileMap::TileMap(TileSet* ts):tileSet{ts}{
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
void TileMap::Save(stringstream& out) {
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

void TileMap::SetSize(int newWidth,int newHeight) {
	vector<int> newMatrix(newWidth*newHeight*mapDepth, EMPTY_TILE);
	int maxX = min(newWidth, mapWidth);
	int maxY = min(newHeight, mapHeight);
	
	FOR(z,mapDepth)
		FOR(y,maxY)
			FOR(x,maxX)
				newMatrix[x+(y*newWidth)+(z*newWidth*newHeight)] = At(x,y,z);
	
	mapWidth = newWidth;
	mapHeight = newHeight;
	tileMatrix.clear();
	tileMatrix = newMatrix;
}
