#include <tileMap.hpp>
#include <tileSet.hpp>

TileMap::TileMap(string file,TileSet* ts){
	load(file);
	setTileSet(ts);
}


void TileMap::load(string file){
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
				at(w,h,d) = x-1;
			}
		}
	}
	in.close();
}

void TileMap::setTileSet(TileSet* ts){
	tileSet = ts;
}

int& TileMap::at(int x,int y,int z){
	return tileMatrix[x+(y*mapWidth)+(z*(mapWidth*mapHeight))];
}

void TileMap::render(int cameraX,int cameraY){
	FOR(i,mapDepth){
		renderLayer(i,cameraX,cameraY);
	}
}

void TileMap::renderLayer(int layer,int cameraX,int cameraY){
	int w=tileSet->getWidth();
	int h=tileSet->getHeight();
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			tileSet->render(at(x,y,layer),(x*w)-cameraX,(y*h)-cameraY);
		}
	}
}

int TileMap::getWidth(){
	return mapWidth;
}
int TileMap::getHeight(){
	return mapHeight;
}
int TileMap::getDepth(){
	return mapDepth;
}
