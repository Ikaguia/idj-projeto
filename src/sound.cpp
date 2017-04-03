#include <sound.hpp>
#include <resources.hpp>

Sound::Sound():channel{-1},chunk{shared_ptr<Mix_Chunk>(nullptr)}{}
Sound::Sound(string file){
	Open(file);
}

void Sound::Play(int times){
	channel = Mix_PlayChannel(-1,chunk.get(),times);
}
void Sound::Stop(){
	if(channel!=-1){
		Mix_HaltChannel(channel);
		channel=-1;
	}
}
void Sound::Open(string file){
	chunk=Resources::GetSound(file);
}
bool Sound::IsOpen(){
	return (chunk.get()!=nullptr);
}
