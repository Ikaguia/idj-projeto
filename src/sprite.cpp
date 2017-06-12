#include <sprite.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <game.hpp>
#include <resources.hpp>

Sprite::Sprite():texture{nullptr},scaleX{1.0f},scaleY{1.0f},flipH{false},flipV{false},timeElapsed{0.0f}{}
Sprite::Sprite(string file,int fCount,float fTime):texture{nullptr},scaleX{1.0f},scaleY{1.0f},flipH{false},flipV{false},timeElapsed{0.0f}{
	Open(file,fCount,fTime);
}
Sprite::~Sprite(){}

void Sprite::Open(string file,int fCount,float fTime){
	texture = Resources::GetImage(file);
	if(SDL_QueryTexture(texture.get(),nullptr,nullptr,&width,&height)){
		cout << "Erro ao carregar as dimensões da textura \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}

	SetFrameCount(fCount);
	SetFrameTime(fTime);
	SetFrame(0);
}
void Sprite::SetClip(int x,int y,int w,int h){
	clipRect.x=x;
	clipRect.y=y;
	clipRect.w=w;
	clipRect.h=h;
}

void Sprite::Render(int x,int y,float angle, float extScale){
	SDL_Rect dest;
	
	dest.x=(x);
	dest.y=(y);
	dest.w=ceil(clipRect.w * scaleX * extScale);
	dest.h=ceil(clipRect.h * scaleY * extScale);
	
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if(flipH && flipV)angle += (angle<180?180:-180);
	else if(flipH)flip = SDL_FLIP_HORIZONTAL;
	else if(flipV)flip = SDL_FLIP_VERTICAL;
	//cout << "rendering with size " << dest.w << "," << dest.h << " fCount = " << frameCount << endl;
	//SDL_RenderCopyEx(GAMERENDER,texture,nullptr,nullptr,angle,nullptr,SDL_FLIP_NONE);
	SDL_RenderCopyEx(GAMERENDER,texture.get(),&clipRect,&dest,angle,nullptr,flip);
}

void Sprite::Render(Vec2 v, float angle, float extScale) {
	Render(v.x, v.y, angle, extScale);
}

void Sprite::Update(float time){
	if(frameCount==1 || frameTime<0)return;
	timeElapsed+=time;
	if(timeElapsed>frameTime){
		timeElapsed-=frameTime;
		SetFrame((currentFrame+1)%frameCount);
	}
}
void Sprite::SetFrame(int frame){
	currentFrame = frame;
	SetClip(frame*GetWidth(),0,GetWidth(),GetHeight());
}
void Sprite::SetFrameCount(int fCount){
	frameCount=fCount;
}
void Sprite::SetFrameTime(float fTime){
	frameTime=fTime;
}

int Sprite::GetWidth(){
	return (width*scaleX)/frameCount;
}

int Sprite::GetHeight(){
	return (height*scaleY);
}

int Sprite::GetCurFrame(){
	return currentFrame;
}

int Sprite::GetFrameCount(){
	return frameCount;
}

bool Sprite::IsOpen(){
	return (texture!=nullptr);
}

void Sprite::SetScale(float scale) {
	scaleX = scaleY = scale;
}

void Sprite::SetScaleX(float scale){
	scaleX = scale;
}

void Sprite::SetScaleY(float scale){
	scaleY = scale;
}

void Sprite::SetScaleToFit(float w, float h) {
	if(w > h)
		scaleX = scaleY = (w/width);
	else
		scaleX = scaleY = (h/height); 
}

void Sprite::SetScaleToFit(Vec2 v) {
	SetScaleToFit(v.x, v.y);
}

void Sprite::StretchToFit(float w, float h) {
	scaleX = w/width;
	scaleY = h/height;
}

void Sprite::StretchToFit(Vec2 v) {
	StretchToFit(v.x, v.y);
}

void Sprite::FlipH() {
	flipH = !flipH;
}

void Sprite::FlipV() {
	flipV = !flipV;
}

void Sprite::SetFlipH(bool f) {
	flipH = f;
}

void Sprite::SetFlipV(bool f) {
	flipV = f;
}
