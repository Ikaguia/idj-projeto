#ifndef SPRITEHPP
#define SPRITEHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <common.hpp>


class Sprite{
	SDL_Texture* texture;
	int width;
	int height;
	SDL_Rect clipRect;
	float scaleX;
	float scaleY;

	int frameCount;
	int currentFrame;
	float timeElapsed;
	float frameTime;
public:
	Sprite();
	Sprite(string file,int fCount=1,float fTime=1.0f);
	~Sprite();

	void Open(string file,int fCount=1,float fTime=1.0f);
	void SetClip(int x,int y,int w,int h);
	void render(int x,int y,float angle=0.0f);

	void Update(float time);
	void SetFrame(int frame);
	void SetFrameCount(int fCount);
	void SetFrameTime(float fTime);

	int GetWidth();
	int GetHeight();

	bool isOpen();

	void SetScaleX(float scale);
	void SetScaleY(float scale);
};

#endif//SPRITEHPP