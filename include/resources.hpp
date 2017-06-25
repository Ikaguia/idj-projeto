#ifndef RESOURCESHPP
#define RESOURCESHPP

#include <common.hpp>

class Resources{
	static unordered_map<string,shared_ptr<SDL_Texture>> imageTable;
	static unordered_map<string,shared_ptr<Mix_Music>> musicTable;
	static unordered_map<string,shared_ptr<Mix_Chunk>> soundTable;
	static unordered_map<string,shared_ptr<TTF_Font>> fontTable;
public:
	static shared_ptr<SDL_Texture> GetImage(const string& file);
	static void ClearImages();

	static shared_ptr<Mix_Music> GetMusic(const string& file);
	static void ClearMusics();

	static shared_ptr<Mix_Chunk> GetSound(const string& file);
	static void ClearSounds();

	static shared_ptr<TTF_Font> GetFont(const string& file,int ptsize);
	static void ClearFonts();
};

#endif//RESOURCESHPP
