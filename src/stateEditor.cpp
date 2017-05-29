#include <stateEditor.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <complib.hpp>
#include <game.hpp>
#include <resources.hpp>

#define HELP_TEXT "Press [H] for help"
#define HELP_TEXT_OPENED 	"S - Save\nG - Toggle grid\nLMB - Place tile\nRMB - Erase tile\nA - Previous tile\nD - Next tile"

StateEditor::StateEditor():level{"level/level_0.txt"},tileIndex{0},showGrid{true},showHelp{true},helpText{Text(HELP_TEXT,16)},statusText{Text("test",16)} {
	LoadAssets();
	CAMERA = {-100, -100};
	CAMERAZOOM = 1.0f;
	
	helpText.SetPos(0,0);
	statusText.SetPos(WINSIZE);
	statusText.SetAlignment(Text::Align::RIGHT);
	statusText.SetHotspot(Hotspot::BOTTOM_RIGHT);
}

StateEditor::~StateEditor(){}

void StateEditor::LoadAssets(){
}

void StateEditor::Update(float time){
	Camera::Update(time);
	if(INPUTMAN.QuitRequested())quitRequested=true;
	if(INPUTMAN.KeyPress(KEY_ESC))popRequested=true;
	
	int tileCount = level.tileSet.GetTileCount();
	if(INPUTMAN.KeyPress(KEY(a))) tileIndex = (tileIndex + tileCount-1)%tileCount;
	if(INPUTMAN.KeyPress(KEY(d))) tileIndex = (tileIndex + 1)%tileCount;
	
	if(INPUTMAN.IsMouseDown(MBUTTON_LEFT)) {
		Vec2 cursor = GetCurrentTile();
		Rect canvas(0, 0, level.tileMap.GetWidth()-1, level.tileMap.GetHeight()-1);
		if(canvas.contains(cursor)) level.tileMap.At(cursor.x, cursor.y, 0) = tileIndex;
	}
	if(INPUTMAN.IsMouseDown(MBUTTON_RIGHT)) {
		Vec2 cursor = GetCurrentTile();
		Rect canvas(0, 0, level.tileMap.GetWidth()-1, level.tileMap.GetHeight()-1);
		if(canvas.contains(cursor)) level.tileMap.At(cursor.x, cursor.y, 0) = -1;
	}
	
	if(INPUTMAN.KeyPress(KEY(g))) showGrid = (!showGrid);
	if(INPUTMAN.KeyPress(KEY(h))) {
		showHelp = (!showHelp);
		if(showHelp) helpText.SetText(HELP_TEXT);
		else helpText.SetText(HELP_TEXT_OPENED);
	}
	
	if(INPUTMAN.KeyPress(KEY(s))) level.Save("level/level_0.txt");
	
	UpdateArray(time);
	
	statusText.SetText("Mouse:("+to_string(INPUTMAN.GetMouseX())+","+to_string(INPUTMAN.GetMouseY())+")  Zoom:"+FloatToStr(100*CAMERAZOOM)+"%");
}
void StateEditor::Render(){
	RenderBackground();
	level.background.Render(RENDERPOSX(0), RENDERPOSY(0), 0, CAMERAZOOM);
	level.tileMap.Render();
	RenderCursor();
	RenderArray();
	if(showGrid) RenderGrid(0, 0, 64, 64);
	RenderBorder();
	
	helpText.Render();
	statusText.Render();
}

void StateEditor::Pause(){
}

void StateEditor::Resume(){
}

void StateEditor::RenderBackground() {
	SDL_SetRenderDrawColor(GAMERENDER, 127, 127, 127, 255);
	SDL_RenderClear(GAMERENDER);
}

void StateEditor::RenderGrid(int x, int y, int w, int h) {
	SDL_SetRenderDrawColor(GAMERENDER, 255, 255, 255, 63);
	SDL_SetRenderDrawBlendMode(GAMERENDER, SDL_BLENDMODE_BLEND);
	
	if(w > 0) {
		int first = floor(CAMERA.x/w)*w;
		if(first>0) first+=w;
		int lim = CAMERA.x+(WINSIZE.x/CAMERAZOOM);
		for(int i=first;i<=lim;i+=w)
			SDL_RenderDrawLine(GAMERENDER, RENDERPOSX(i), 0, RENDERPOSX(i), WINSIZE.y);
	}	
	if(h > 0) {
		int first = floor(CAMERA.y/h)*h;
		if(first>0) first+=h;
		int lim = CAMERA.y+(WINSIZE.y/CAMERAZOOM);
		for(int i=first;i<=lim;i+=h)
			SDL_RenderDrawLine(GAMERENDER, 0, RENDERPOSY(i), WINSIZE.x, RENDERPOSY(i));
	}
}

void StateEditor::RenderBorder() {
	SDL_SetRenderDrawColor(GAMERENDER, 255, 255, 255, 255);
	
	SDL_Rect rect;
	rect.x = RENDERPOSX(0);
	rect.y = RENDERPOSY(0);
	rect.w = (level.tileMap.GetWidth()*level.tileSet.GetWidth()*CAMERAZOOM)+1;
	rect.h = (level.tileMap.GetHeight()*level.tileSet.GetHeight()*CAMERAZOOM)+1;
	
	SDL_RenderDrawRect(GAMERENDER, &rect);
}

void StateEditor::RenderCursor() {
	Vec2 pos = GetCurrentTile();
	int w = level.tileSet.GetWidth();
	int h = level.tileSet.GetHeight();
	level.tileSet.Render(tileIndex, RENDERPOSX(pos.x*w), RENDERPOSY(pos.y*h), CAMERAZOOM);
	
	SDL_SetRenderDrawColor(GAMERENDER, 255, 255, 255, 255);
	
	SDL_Rect rect;
	rect.x = RENDERPOSX(0);
	rect.y = RENDERPOSY(0);
	rect.w = (level.tileMap.GetWidth()*level.tileSet.GetWidth()*CAMERAZOOM)+1;
	rect.h = (level.tileMap.GetHeight()*level.tileSet.GetHeight()*CAMERAZOOM)+1;
	
	SDL_RenderDrawRect(GAMERENDER, &rect);
}

Vec2 StateEditor::GetCurrentTile() {
	Vec2 pos = CAMERA+(INPUTMAN.GetMouse()/CAMERAZOOM);
	int tileWidth = level.tileSet.GetWidth();
	int tileHeight = level.tileSet.GetHeight();
	
	//if(pos.x<0) pos.x-=tileWidth;
	pos.x/=tileWidth;
	//if(pos.y<0) pos.y-=tileHeight;
	pos.y/=tileHeight;
	pos.floor();
	//cout<<"mouse "<<INPUTMAN.GetMouseX()<<" "<<INPUTMAN.GetMouseY()<<endl;
	//cout<<"tile "<<pos.x<<" "<<pos.y<<endl;
	return pos;
}
