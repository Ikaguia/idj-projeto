#ifndef RESOURCESHPP
#define RESOURCESHPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <common.hpp>

class Resources{
	static unordered_map<string,SDL_Texture*> imageTable;
public:
	static SDL_Texture* getImage(string file);
	static void clearImages();
};

#endif//RESOURCESHPP