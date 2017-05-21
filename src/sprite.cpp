#include <sprite.hpp>
#include <game.hpp>
#include <resources.hpp>

Sprite::Sprite():texture{nullptr},scaleX{1.0f},scaleY{1.0f},flipX{false},flipY{false},timeElapsed{0.0f}{}
Sprite::Sprite(string file,int fCount,float fTime):texture{nullptr},scaleX{1.0f},scaleY{1.0f},timeElapsed{0.0f}{
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

void Sprite::Render(int x,int y,float angle){
	SDL_Rect dest;
	dest.x=x;
	dest.y=y;
	dest.w=clipRect.w;
	dest.h=clipRect.h;
	
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	float ang = angle;
	if(flipX && flipY)
		ang += (angle<180?180:-180);
	else if(flipX && !flipY)
		flip = SDL_FLIP_HORIZONTAL;
	else if(!flipX && flipY)
		flip = SDL_FLIP_VERTICAL;
	//cout << "rendering with size " << dest.w << "," << dest.h << " fCount = " << frameCount << endl;
	//SDL_RenderCopyEx(GAMERENDER,texture,nullptr,nullptr,angle,nullptr,SDL_FLIP_NONE);
	SDL_RenderCopyEx(GAMERENDER,texture.get(),&clipRect,&dest,ang,nullptr,flip);
}

void Sprite::Render(Vec2 v, float angle) {
	Render(v.x, v.y, angle);
}

void Sprite::Update(float time){
	if(frameCount==1)return;
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

bool Sprite::IsOpen(){
	return (texture!=nullptr);
}

void Sprite::SetScale(float scale) {
	scaleX=scaleY=scale;
}

void Sprite::SetScaleX(float scale){
	scaleX=scale;
}

void Sprite::SetScaleY(float scale){
	scaleY=scale;
}

void Sprite::FlipX() {
	flipX = !flipX;
}

void Sprite::FlipY() {
	flipY = !flipY;
}

void Sprite::SetFlipX(bool f) {
	flipX = f;
}

void Sprite::SetFlipY(bool f) {
	flipY = f;
}
