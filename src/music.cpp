#include <music.hpp>
#include <resources.hpp>

#define FADETIME 1000//ms

Music::Music():music{shared_ptr<Mix_Music>(nullptr)}{}
Music::Music(string file){
	Open(file);
}

void Music::Play(int times){
	Mix_PlayMusic(music.get(),times);
}
void Music::Stop(){
	Mix_FadeOutMusic(FADETIME);
}
void Music::Open(string file){
	music=Resources::GetMusic(file);
}
bool Music::IsOpen(){
	return (music.get()!=nullptr);
}
