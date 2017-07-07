#include <sprite.hpp>

#include <common.hpp>

#include <camera.hpp>
#include <game.hpp>
#include <resources.hpp>

Sprite::Sprite():texture{nullptr}{}
Sprite::Sprite(const string& file,int fCountX,int fCountY,float fTime,int fCount):texture{nullptr}{
	if(fCount==-1)fCount=fCountX*fCountY;
	Open(file,fCountX,fCountY,fTime,fCount);
}
Sprite::Sprite(const string& file,int fCountX,float fTime,int fCount):texture{nullptr}{
	if(fCount==-1)fCount=fCountX;
	Open(file,fCountX,1,fTime,fCount);
}
Sprite::~Sprite(){}

void Sprite::Open(const string& file,int fCountX,int fCountY,float fTime,int fCount){
	if(fCount==-1)fCount=fCountX*fCountY;

	texture = Resources::GetImage(file);
	if(SDL_QueryTexture(texture.get(),nullptr,nullptr,&width,&height)){
		cerr << "Erro ao carregar as dimensões da textura \"" << file << "\", o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}

	SetFrameCount(fCountX,fCountY,fCount);
	SetFrameTime(fTime);
	SetFrame(0);
}
void Sprite::SetClip(int x,int y,int w,int h){
	clipRect.x=x;
	clipRect.y=y;
	clipRect.w=w;
	clipRect.h=h;
}

void Sprite::Render(float x,float y,float angle, float extScale){
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

void Sprite::Render(const Vec2& v, float angle, float extScale) {
	Render(v.x, v.y, angle, extScale);
}

void Sprite::Update(float time){
	if(GetFrameCount()==1 || frameTime<0)return;
	timeElapsed+=time;
	if(timeElapsed>frameTime){
		timeElapsed-=frameTime;
		SetFrame((currentFrame+1)%GetFrameCount());
		looped = (currentFrame==0);
	}
}
void Sprite::SetFrame(int frame){
	currentFrame = frame;
	SetClip((frame%frameCountX)*GetWidth(),(frame/frameCountX)*GetHeight(),GetWidth(),GetHeight());
}
void Sprite::SetFrameCount(int fCountX,int fCountY,int fCount){
	if(fCount==-1)fCount=fCountX*fCountY;
	frameCountX=fCountX;
	frameCountY=fCountY;
	frameCount=fCount;
}
void Sprite::SetFrameTime(float fTime){
	frameTime=fTime;
}

int Sprite::GetWidth()const{
	return (width*scaleX)/frameCountX;
}

int Sprite::GetHeight()const{
	return (height*scaleY)/frameCountY;
}

int Sprite::GetCurFrame()const{
	return currentFrame;
}

int Sprite::GetFrameCount()const{
	return frameCount;
}

Vec2 Sprite::GetFrameCount(bool b)const{
	UNUSED(b);
	return {(float)frameCountX,(float)frameCountY};
}

bool Sprite::IsOpen()const{
	return (texture!=nullptr);
}
bool Sprite::Looped()const{
	return looped;
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

void Sprite::SetBlend(bool b){
	if(b) SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
	else  SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_NONE);
}
