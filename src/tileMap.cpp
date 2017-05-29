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

	// for(auto &it:mp){
	// 	Rect r = it.second;
	// 	r.w-=r.x;r.w++;
	// 	r.h-=r.y;r.h++;
	// 	if(it.first.first==2){
	// 		r.y+=f;
	// 		r.h-=f;
	// 	}
	// 	r.x*=tileW;
	// 	r.w*=tileW;
	// 	r.y*=tileH;
	// 	r.h*=tileH;
	// 	GameObject *tile = new GameObject{r};
	// 	if(it.first.first==1 || it.first.first==2)tile->AddComponent(new CompCollider{CompCollider::collType::t_ground});
	// 	else if                (it.first.first==3)tile->AddComponent(new CompCollider{CompCollider::collType::t_h_ground});
	// 	tile->AddComponent(new CompStaticRender{Sprite{"img/point_yellow.jpg"},Vec2{0,0}});
	// 	entities->insert(unique_ptr<GameObject>(tile));
	// 	cout << "new rect at " << r << endl;
	// }
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

