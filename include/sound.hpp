#ifndef SOUNDHPP
#define SOUNDHPP

#include <SDL2/SDL_mixer.h>

#include <common.hpp>

class Sound{
	int channel;
	shared_ptr<Mix_Chunk> chunk;
public:
	Sound();
	Sound(string file);

	void Play(int times);
	void Stop();
	void Open(string file);
	bool IsOpen();
};

#endif//SOUNDHPP
