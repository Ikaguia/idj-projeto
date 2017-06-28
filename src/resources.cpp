#include <resources.hpp>
#include <game.hpp>

unordered_map<string,shared_ptr<SDL_Texture>> Resources::imageTable;
unordered_map<string,shared_ptr<Mix_Music>> Resources::musicTable;
unordered_map<string,shared_ptr<Mix_Chunk>> Resources::soundTable;
unordered_map<string,shared_ptr<TTF_Font>> Resources::fontTable;


shared_ptr<SDL_Texture> Resources::GetImage(const string& file){
	if(imageTable.count(file))return imageTable[file];
	SDL_Texture* texture = IMG_LoadTexture(GAMERENDER,file.c_str());
	if(!texture){
		cout << "Erro ao carregar textura \"" << file << "\":" << endl;
		string s=SDL_GetError();
		cout << s << endl << "o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	auto func = [](SDL_Texture* texture){SDL_DestroyTexture(texture);};
	return imageTable[file] = shared_ptr<SDL_Texture>(texture,func);
}
void Resources::ClearImages(){
	for(auto i=imageTable.begin();i!=imageTable.end();){
		if(i->second.unique())i=imageTable.erase(i);
		else i++;
	}
}

shared_ptr<Mix_Music> Resources::GetMusic(const string& file){
	if(musicTable.count(file))return musicTable[file];
	Mix_Music* music = Mix_LoadMUS(file.c_str());
	if(!music){
		cout << "Erro ao carregar musica \"" << file << "\":" << endl;
		string s=SDL_GetError();
		cout << s << endl << "o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	auto func = [](Mix_Music* music){Mix_FreeMusic(music);};
	return musicTable[file] = shared_ptr<Mix_Music>(music,func);
}
void Resources::ClearMusics(){
	for(auto i=musicTable.begin();i!=musicTable.end();){
		if(i->second.use_count()==1)i=musicTable.erase(i);
		else i++;
	}
}

shared_ptr<Mix_Chunk> Resources::GetSound(const string& file){
	if(soundTable.count(file))return soundTable[file];
	Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
	if(!sound){
		cout << "Erro ao carregar som \"" << file << "\":" << endl;
		string s=SDL_GetError();
		cout << s << endl << "o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	auto func = [](Mix_Chunk* sound){Mix_FreeChunk(sound);};
	return soundTable[file] = shared_ptr<Mix_Chunk>(sound,func);
}
void Resources::ClearSounds(){
	for(auto i=soundTable.begin();i!=soundTable.end();){
		if(i->second.use_count()==1)i=soundTable.erase(i);
		else i++;
	}
}

shared_ptr<TTF_Font> Resources::GetFont(const string& file,int ptsize){
	string key = file+std::to_string(ptsize);
	if(fontTable.count(key)){
		//cout << "found font " << key << " wih use_count = " << fontTable[key].use_count() << endl;
		return fontTable[key];
	}
	TTF_Font* font = TTF_OpenFont(file.c_str(),ptsize);
	if(!font){
		cout << "Erro ao carregar fonte \"" << file << "\":" << endl;
		string s=SDL_GetError();
		cout << s << endl << "o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	//return fontTable[key] = shared_ptr<TTF_Font>(font,[](TTF_Font* font){TTF_CloseFont(font);});
	auto func = [](TTF_Font* font){TTF_CloseFont(font);};
	return fontTable[key] = shared_ptr<TTF_Font>(font,func);
}
void Resources::ClearFonts(){
	for(auto i=fontTable.begin();i!=fontTable.end();){
		if(i->second.use_count()==1)i=fontTable.erase(i);
		else i++;
	}
}
