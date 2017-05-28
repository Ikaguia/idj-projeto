#include <tileMap.hpp>
#include <tileSet.hpp>
#include <gameObject.hpp>
#include <componentCollider.hpp>
#include <componentStaticRender.hpp>

TileMap::TileMap(string file,TileSet* ts,set<unique_ptr<GameObject>> *entities){
	SetTileSet(ts);
	Load(file,entities);
}


void TileMap::Load(string file,set<unique_ptr<GameObject>> *entities){
	int x,y;
	float f;
	string line;
	std::ifstream in;
	in.open(file);
	if(!in.is_open()){
		cout << "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	getline(in,line);
	sscanf(line.c_str()," %d,%d,%d,",&mapWidth,&mapHeight,&mapDepth);

	tileMatrix.reserve(mapWidth*mapHeight*mapDepth);

	FOR(d,mapDepth){
		FOR(h,mapHeight){
			FOR(w,mapWidth){
				in >> x;
				in.ignore(1);
				At(w,h,d) = x-1;
			}
		}
	}

	in >> f;
	in.ignore(1);
	int tileW=tileSet->GetWidth();
	int tileH=tileSet->GetHeight();
	map<ii,Rect> mp;
	FOR(h,mapHeight){
		FOR(w,mapWidth){
			in >> x;
			in.ignore(1);
			if(x==1 || x==2){
				in >> y;
				in.ignore(1);
				if(!mp.count(ii{x,y}))mp[ii{x,y}]=Rect{(float)mapWidth+1,(float)mapHeight+1,(float)-1,(float)-1};//default vals to make min and max work
				else{
					mp[ii{x,y}].x=min(mp[ii{x,y}].x,(float)w);
					mp[ii{x,y}].y=min(mp[ii{x,y}].y,(float)h);
					mp[ii{x,y}].w=max(mp[ii{x,y}].w,(float)w);
					mp[ii{x,y}].h=max(mp[ii{x,y}].h,(float)h);
				}
			}
		}
	}
	for(auto &it:mp){
		Rect r = it.second;
		r.w-=r.x;r.w++;
		r.h-=r.y;r.h++;
		if(it.first.first==2){
			r.y+=f;
			r.h-=f;
		}
		r.x*=tileW;
		r.w*=tileW;
		r.y*=tileH;
		r.h*=tileH;
		GameObject *tile = new GameObject{r};
		if(it.first.first==1 || it.first.first==2)tile->AddComponent(new CompCollider{CompCollider::collType::t_ground});
		else if                (it.first.first==3)tile->AddComponent(new CompCollider{CompCollider::collType::t_h_ground});
		tile->AddComponent(new CompStaticRender{Sprite{"img/point_yellow.jpg"},Vec2{0,0}});
		entities->insert(unique_ptr<GameObject>(tile));
		cout << "new rect at " << r << endl;
	}
	in.close();
}

void TileMap::SetTileSet(TileSet* ts){
	tileSet = ts;
}

int& TileMap::At(int x,int y,int z){
	return tileMatrix[x+(y*mapWidth)+(z*(mapWidth*mapHeight))];
}

int TileMap::At(int x,int y,int z) const{
	return tileMatrix[x+(y*mapWidth)+(z*(mapWidth*mapHeight))];
}

void TileMap::Render(int cameraX,int cameraY) const{
	FOR(i,mapDepth){
		RenderLayer(i,cameraX,cameraY);
	}
}

void TileMap::RenderLayer(int layer,int cameraX,int cameraY) const{
	int w=tileSet->GetWidth();
	int h=tileSet->GetHeight();
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			tileSet->Render(At(x,y,layer),(x*w)-cameraX,(y*h)-cameraY);
		}
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
