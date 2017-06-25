#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include <game.hpp>

#include <gameException.hpp>
#include <inputManager.hpp>
#include <resources.hpp>
#include <state.hpp>


Game* Game::instance=NULL;


Game::Game(string title,int width,int height):frameStart{0},dt{0},winSize{(float)width,(float)height}{
	srand(time(NULL));

	if(instance){
		cerr << "Erro, mais de uma instancia de 'Game' instanciada, o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	instance=this;

	bool success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0;
	if (!success) {
		string error_msg(error_msg = SDL_GetError());
		error_msg = "Could not initialize SDL:\n" + error_msg;
		throw GameException(error_msg);
	}

	//initialize image module and check if process went OK
	map<int, string> code_name_map = {{IMG_INIT_TIF, "tif"},
									  {IMG_INIT_JPG, "jpg"},
									  {IMG_INIT_PNG, "png"}};
	vector<int> image_formats{IMG_INIT_TIF, IMG_INIT_JPG, IMG_INIT_PNG};
	//or between all desired formats
	int image_settings = accumulate(image_formats.begin(),
									image_formats.end(),
									0,
									[](const int &a, const int &b) {
										return a | b;
									}
	);

	int res = IMG_Init(image_settings);
	if (image_settings != res) {
		string error_msg_main = SDL_GetError();
		string error_msg = "Could not initiazlie image libary for type:";
		for (auto format : image_formats)
			if ((format & res) == 0) {
				error_msg += code_name_map[format];
			}
		error_msg += "\n";
		error_msg = error_msg_main + error_msg;
		throw GameException(error_msg);
	}

	int audio_modules = MIX_INIT_OGG;
	res = Mix_Init(audio_modules);
	if (res != audio_modules) {
		if ((MIX_INIT_OGG & res ) == 0 )cerr << "OGG flag not in res!" << endl;
		if ((MIX_INIT_MP3 & res ) == 0 )cerr << "MP3 flag not in res!" << endl;
		throw GameException("Problem when initiating SDL audio!");
	}

	res = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	if(res != 0)throw GameException("Problem when initiating SDL audio!");

	res = TTF_Init();
	if(res != 0)cerr << "Could not initialize TTF module!" << endl;

	//window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_FULLSCREEN);
	window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
	if(!window)throw GameException("Window nao foi carregada)!");

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	if(!renderer)throw GameException("Erro ao instanciar renderizador da SDL!");

	storedState = nullptr;
	SDL_SetRenderDrawBlendMode(GAMERENDER, SDL_BLENDMODE_BLEND);
};

Game::~Game(){
	while(stateStack.size()){
		delete stateStack.top().get();
		stateStack.pop();
	}
	if(storedState)delete storedState;
	Resources::ClearImages();
	Resources::ClearMusics();
	Resources::ClearFonts();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


Game& Game::GetInstance(){
	return (*instance);
}

State& Game::GetCurrentState(){
	return (*stateStack.top());
}

SDL_Renderer* Game::GetRenderer(){
	return renderer;
}

void Game::Push(State* state){
	if(storedState)delete storedState;
	storedState=state;
}

void Game::Run(){
	//SDL_Rect r;
	//SDL_GetDisplayBounds(0, &r);
	//cout<<r<<endl;
	if(storedState){
		stateStack.push(unique_ptr<State>(storedState));
		storedState=nullptr;
		GetCurrentState().Begin();
	}
	while(stateStack.size() && !(GetCurrentState().QuitRequested())){
		CalculateDeltaTime();
		INPUT.Update(dt);
		//if(INPUT.KeyPress(KEY_F(11))) SwitchWindowMode();
		GetCurrentState().Update(dt);
		GetCurrentState().Render();
		SDL_RenderPresent(renderer);

		if(GetCurrentState().PopRequested()){
			GetCurrentState().Pause();
			GetCurrentState().End();
			stateStack.pop();
			Resources::ClearImages();
			Resources::ClearMusics();
			Resources::ClearFonts();
			if(stateStack.size())GetCurrentState().Resume();
		}
		if(storedState){
			GetCurrentState().Pause();
			stateStack.push(unique_ptr<State>(storedState));
			storedState=nullptr;
			GetCurrentState().Begin();
		}

		SDL_Delay(33);
	}
	while(stateStack.size()){
		GetCurrentState().End();
		stateStack.pop();
	}
}

float Game::GetDeltaTime(){
	return dt;
}

void Game::CalculateDeltaTime(){
	unsigned int tmp = frameStart;
	frameStart = SDL_GetTicks();
	dt = max((frameStart - tmp) / 1000.0, 0.001);
}

void Game::SwitchWindowMode() {
}
