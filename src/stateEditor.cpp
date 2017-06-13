#include <stateEditor.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <resources.hpp>

#define HELP_TEXT "Press [H] for help"
#define HELP_TEXT_OPEN \
"S - Save\n\
G - Toggle grid\n\
LMB - Place tile\n\
RMB - Erase tile\n\
A - Previous tile\n\
D - Next tile\n\
C - Show Collision \n\
ARROW KEYS, MMB - Move Camera\n\
Z - Zoom In\n\
X - Zoom Out"

#define EDITOR_BG_COLOR 127,127,127,255
#define GRID_COLOR 255,255,255,63
#define LEVEL_BORDER_COLOR 255,255,255,255
#define TILE_CURSOR_COLOR 255,255,0,255
#define COLLISION_COLOR 255,0,0,255

//TODO: Remove placeholder index
#define COLLISION_BLOCK 0

StateEditor::StateEditor():helpText{HELP_TEXT,16},statusText{"test",16} {
	LoadAssets();
}

StateEditor::~StateEditor(){}

void StateEditor::LoadAssets(){
}

void StateEditor::Begin(){
	level.Load("data/level/level_0.txt", false);
	CAMERA = {-100, -100};
	CAMERAZOOM = 1.0f;
	
	helpText.SetPos(0,0);
	statusText.SetPos(WINSIZE);
	statusText.SetAlignment(Text::Align::RIGHT);
	statusText.SetHotspot(Hotspot::BOTTOM_RIGHT);
}

void StateEditor::Update(float time){
	Camera::Update(time);
	if(INPUT.QuitRequested())quitRequested=true;
	if(INPUT.KeyPress(KEY_ESC))popRequested=true;
	
	int tileCount = level.tileSet.GetTileCount();
	//Select next tile
	if(INPUT.KeyPress(KEY(a))) tileIndex = (tileIndex + tileCount-1)%tileCount;
	//Select previous tile
	if(INPUT.KeyPress(KEY(d))) tileIndex = (tileIndex + 1)%tileCount;
	
	//Place a tile
	if(INPUT.IsMouseDown(MBUTTON_LEFT)) {
		Vec2 cursor = GetCurrentTile();
		Rect canvas(0, 0, level.tileMap.GetWidth()-1, level.tileMap.GetHeight()-1);
		if(canvas.contains(cursor)) {
			level.tileMap.At(cursor.x, cursor.y, 0) = tileIndex;
			level.collisionLayer[(cursor.y*level.tileMap.GetWidth())+cursor.x] = COLLISION_BLOCK;
		}
	}
	//Erase a tile
	if(INPUT.IsMouseDown(MBUTTON_RIGHT)) {
		Vec2 cursor = GetCurrentTile();
		Rect canvas(0, 0, level.tileMap.GetWidth()-1, level.tileMap.GetHeight()-1);
		if(canvas.contains(cursor)) {
			level.tileMap.At(cursor.x, cursor.y, 0) = -1;
			level.collisionLayer[(cursor.y*level.tileMap.GetWidth())+cursor.x] = EMPTY_TILE;
		}
	}
	
	
	//Toggle grid
	if(INPUT.KeyPress(KEY(g))) showGrid = (!showGrid);
	//Toggle instructions menu
	if(INPUT.KeyPress(KEY(h))) {
		showHelp = (!showHelp);
		if(showHelp) helpText.SetText(HELP_TEXT);
		else helpText.SetText(HELP_TEXT_OPEN);
	}
	//Toggle collision boxes
	if(INPUT.KeyPress(KEY(c))) showCollision = (!showCollision);
	
	//Save
	if(INPUT.KeyPress(KEY(s))) {
		RecomputeCollisionRectangles();
		level.Save("data/level/level_0.txt",grouped);
	}
	
	//Pan view
	if(INPUT.MousePress(MBUTTON_MIDDLE)) {
		clickPos = INPUT.GetMouse();
		camPos = CAMERA;
		CAMERALOCK = true;
	}
	else if(INPUT.IsMouseDown(MBUTTON_MIDDLE)) {
		CAMERA = camPos - ((INPUT.GetMouse()-clickPos)/CAMERAZOOM);
	}
	if(INPUT.MouseRelease(MBUTTON_MIDDLE)) {
		CAMERALOCK = false;
	}
	
	UpdateArray(time);
	
	statusText.SetText("Mouse:("+to_string(INPUT.GetMouseX())+","+to_string(INPUT.GetMouseY())+")  Zoom:"+FloatToStr(100*CAMERAZOOM)+"%");
}
void StateEditor::Render(){
	RenderBackground();

	//Tirei background daqui
	//level.background.Render(RENDERPOSX(0), RENDERPOSY(0), 0, CAMERAZOOM);
	level.tileMap.Render();
	RenderArray();
	if(showGrid) RenderGrid(64, 64);
	RenderBorder();
	if(showCollision) RenderCollision();
	RenderCursor();
	
	helpText.Render();
	statusText.Render();
}

void StateEditor::Pause(){
}

void StateEditor::Resume(){
}

void StateEditor::RenderBackground() {
	SET_COLOR(EDITOR_BG_COLOR);
	CLEAR_SCREEN();
}

void StateEditor::RenderGrid(int w, int h) {
	SET_COLOR(GRID_COLOR);
	
	if(w > 0) {
		int first = floor(CAMERA.x/w)*w;
		if(first>0) first+=w;
		int lim = CAMERA.x+(WINSIZE.x/CAMERAZOOM);
		for(int i=first;i<=lim;i+=w)
			DRAW_LINE(RENDERPOSX(i), 0, RENDERPOSX(i), WINSIZE.y);
	}	
	if(h > 0) {
		int first = floor(CAMERA.y/h)*h;
		if(first>0) first+=h;
		int lim = CAMERA.y+(WINSIZE.y/CAMERAZOOM);
		for(int i=first;i<=lim;i+=h)
			DRAW_LINE(0, RENDERPOSY(i), WINSIZE.x, RENDERPOSY(i));
	}
}

void StateEditor::RenderBorder() {
	SET_COLOR(LEVEL_BORDER_COLOR);
	
	SDL_Rect rect;
	rect.x = RENDERPOSX(0);
	rect.y = RENDERPOSY(0);
	rect.w = (level.tileMap.GetWidth()*level.tileSet.GetWidth()*CAMERAZOOM)+1;
	rect.h = (level.tileMap.GetHeight()*level.tileSet.GetHeight()*CAMERAZOOM)+1;
	
	DRAW_RECT(&rect);
}

void StateEditor::RenderCursor() {
	Vec2 cursor = GetCurrentTile();
	int tileWidth = level.tileSet.GetWidth();
	int tileHeight = level.tileSet.GetHeight();
	int mapWidth = level.tileMap.GetWidth();
	int mapHeight = level.tileMap.GetHeight();
	
	Rect canvas(0, 0, mapWidth-1, mapHeight-1);
	if(canvas.contains(cursor)) {
		level.tileSet.Render(tileIndex, RENDERPOSX(cursor.x*tileWidth), RENDERPOSY(cursor.y*tileHeight), CAMERAZOOM);
	
		SET_COLOR(TILE_CURSOR_COLOR);
	
		SDL_Rect rect;
		rect.x = RENDERPOSX(cursor.x*tileWidth);
		rect.y = RENDERPOSY(cursor.y*tileHeight);
		rect.w = (tileWidth*CAMERAZOOM)+1;
		rect.h = (tileHeight*CAMERAZOOM)+1;
	
		DRAW_RECT(&rect);
	}
}

void StateEditor::RenderCollision() {
	SET_COLOR(COLLISION_COLOR);
	
	int mapWidth = level.tileMap.GetWidth();
	int mapHeight = level.tileMap.GetHeight();
	int tileWidth = level.tileSet.GetWidth();
	int tileHeight = level.tileSet.GetHeight();
	SDL_Rect rect;
	rect.w = (tileWidth*CAMERAZOOM)+1;
	rect.h = (tileHeight*CAMERAZOOM)+1;
	FOR(y,mapHeight) {
		FOR(x,mapWidth) {
			if(level.collisionLayer[(y*mapWidth)+x] == COLLISION_BLOCK) {
				rect.x = RENDERPOSX(x*tileWidth);
				rect.y = RENDERPOSY(y*tileWidth);
				DRAW_RECT(&rect);
			}
		}
	}
}

Vec2 StateEditor::GetCurrentTile() {
	Vec2 pos = CAMERA+(INPUT.GetMouse()/CAMERAZOOM);
	int tileWidth = level.tileSet.GetWidth();
	int tileHeight = level.tileSet.GetHeight();

	//if(pos.x<0) pos.x-=tileWidth;
	pos.x/=tileWidth;
	//if(pos.y<0) pos.y-=tileHeight;
	pos.y/=tileHeight;
	pos.floor();
	//cout<<"mouse "<<INPUT.GetMouseX()<<" "<<INPUT.GetMouseY()<<endl;
	//cout<<"tile "<<pos.x<<" "<<pos.y<<endl;
	return pos;
}


void StateEditor::RecomputeCollisionRectangles(){
	TileMap &tm = level.tileMap;
	vector<int> &coll=level.collisionLayer;
	int mapWidth=tm.GetWidth();
	int mapHeight=tm.GetHeight();
	
	grouped.clear();
	grouped.resize(mapWidth*mapHeight);
	FOR(y,mapHeight){
		FOR(x,mapWidth){
			int ind = x+(y*mapWidth);
			grouped[ind]=make_pair(ii{x,y},ii{x,y});
		}
	}

	auto join = [this,mapWidth,mapHeight](int ind1,int ind2){
		ii beg=min(grouped[ind1].first, grouped[ind2].first);
		ii end=max(grouped[ind1].second,grouped[ind2].second);

		int ind3=beg.first+(beg.second*mapWidth);
		int ind4=end.first+(end.second*mapWidth);

		grouped[ind1]=grouped[ind2]=grouped[ind3]=grouped[ind4]=make_pair(beg,end);
	};

	FOR(y,mapHeight){
		FOR(x,mapWidth){
			int ind = x+(y*mapWidth);
			if(coll[ind]==EMPTY_TILE)grouped[ind]=make_pair(ii{0,0},ii{0,0});
			else if(x+1 < mapWidth && coll[ind] == coll[1+ind]){
				join(ind,ind+1);
			}
		}
	}

	FOR(y,mapHeight-1){
		FOR(x,mapWidth){
			int ind1 = x+( y   *mapWidth);
			int ind2 = x+((y+1)*mapWidth);
			if(grouped[ind1].first.first == grouped[ind2].first.first &&
				grouped[ind1].second.first == grouped[ind2].second.first){
				join(ind1,ind2);
			}
		}
	}
}
