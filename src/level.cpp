#include <level.hpp>

Level::Level() : background{Sprite("img/stage_bg.jpg")}, tileSet{TileSet(64, 64, "img/tileset.png")}, tileMap{TileMap(50, 50, &tileSet)} {

}

Level::Level(string file) : tileSet{TileSet()}, tileMap{TileMap(50, 50, &tileSet)} {
	Load(file);
}

Level::~Level() {
}

void Level::Load(string file) {
	ifstream in;
	
	in.open(file);
	if(!in.is_open()){
		cout << "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	
	string parameters;
	
	getline(in,backgroundFilename);
	if(backgroundFilename != "") background.Open(backgroundFilename);
	in.ignore(1);
	
	int tileWidth, tileHeight;
	getline(in, tileSetFilename);
	getline(in, parameters);
	sscanf(parameters.c_str(), " %d,%d", &tileWidth, &tileHeight);
	tileSet.Load(tileWidth, tileHeight, tileSetFilename);
	in.ignore(1);
	
	tileMap.Load(in);
/*	
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
		r.w-=r.x;
		r.h-=r.y;
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
	}	*/
	in.close();
}

void Level::Save(string file) {
	ofstream out;
	
	out.open(file);
	if(!out.is_open()) {
		cout<< "Erro ao abrir o arquivo \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	
	out<<backgroundFilename<<endl<<endl;
	
	out<<tileSetFilename<<endl;
	out<<tileSet.GetWidth()<<","<<tileSet.GetHeight()<<endl<<endl;
	
	tileMap.Save(out);
	
	out.close();
}
