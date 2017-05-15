#include <tileMap.hpp>
#include <tileSet.hpp>

TileMap::TileMap(string file,TileSet* ts){
	Load(file);
	SetTileSet(ts);
}


void TileMap::Load(string file){
	int x;
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
	FOR(h,mapHeight){
		FOR(w,mapWidth){
			in >> x;
			in.ignore(1);
			//TODO: create entities with collision
		}
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
