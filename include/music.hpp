#ifndef MUSICHPP
#define MUSICHPP

#include <SDL2/SDL_mixer.h>

#include <common.hpp>

class Music{
	shared_ptr<Mix_Music> music;
public:
	Music();
	Music(string file);

	void Play(int times);
	void Stop();
	void Open(string file);
	bool IsOpen();
};

#endif//MUSICHPP
