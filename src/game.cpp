#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <ctime>

#include <game.hpp>
#include <state.hpp>
#include <inputManager.hpp>
#include <resources.hpp>


Game* Game::instance=NULL;


Game::Game(string title,int width,int height):frameStart{0},dt{0},winSize{(float)width,(float)height}{
	srand(time(NULL));

	if(instance){
		cout << "Erro, mais de uma instancia de 'Game' instanciada, o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	instance=this;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)){
		cout << "Erro SDL encontrado:\n" << (string)SDL_GetError() << endl << "o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}

	// window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_FULLSCREEN);
	window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
	if(!window){
		cout << "Erro ao instanciar janela da SDL, o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	if(!window){
		cout << "Erro ao instanciar renderizador da SDL, o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}

	int img_init = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if(img_init != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)){
		cout << "Erro na inicialização da IMG_Init:" << endl;
		if(!(img_init & IMG_INIT_JPG))cout << "erro ao inicializar JPG" << endl;
		if(!(img_init & IMG_INIT_PNG))cout << "erro ao inicializar PNG" << endl;
		if(!(img_init & IMG_INIT_TIF))cout << "erro ao inicializar TIF" << endl;
		cout << "o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}

	int mix_init = Mix_Init(MIX_INIT_OGG);
	if(!(mix_init & MIX_INIT_OGG)){
		cout << "Erro na inicialização da Mix_Init, erro ao inicializar OGG, o programa ira encerrar agora" << endl;
	}

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024)){
		cout << "Erro na inicialização da Mix_OpenAudio​, o programa ira encerrar agora" << endl;
	}

	if(TTF_Init()){
		cout << "Erro na inicialização da TTF_Init, erro ao inicializar OGG, o programa ira encerrar agora" << endl;
	}

	storedState = nullptr;
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
	if(storedState){
		stateStack.push(unique_ptr<State>(storedState));
		storedState=nullptr;
		GetCurrentState().Resume();
	}
	while(stateStack.size() && !(GetCurrentState().QuitRequested())){
		CalculateDeltaTime();
		INPUTMAN.Update();
		GetCurrentState().Update(GetDeltaTime());
		GetCurrentState().Render();
		SDL_RenderPresent(renderer);

		if(GetCurrentState().PopRequested()){
			GetCurrentState().Pause();
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
			GetCurrentState().Resume();
		}

		SDL_Delay(33);
	}
	while(stateStack.size())stateStack.pop();
}

float Game::GetDeltaTime(){
	return dt;
}



void Game::CalculateDeltaTime(){
	int time = SDL_GetTicks();
	dt = (time - (frameStart))/1000.0f;
	frameStart = time;
}
