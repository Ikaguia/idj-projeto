#include <game.hpp>
#include <state.hpp>
#include <inputManager.hpp>

Game* Game::instance=NULL;


Game::Game(string title,int width,int height):frameStart{0},dt{0}{
	srand(time(NULL));
	if(instance){
		cout << "Erro, mais de uma instancia de 'Game' instanciada, o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
	instance=this;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)){
		cout << "Erro SDL encontrado:\n" << SDL_GetError() << endl << "o programa ira encerrar agora" << endl;
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
	state = new State;
	if(!state){
		cout << "Erro ao inicializar o estado de jogo, o programa ira encerrar agora" << endl;
		exit(EXIT_FAILURE);
	}
};

Game::~Game(){
	delete state;
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}


Game& Game::getInstance(){
	return (*instance);
}

State& Game::getState(){
	return (*state);
}

SDL_Renderer* Game::getRenderer(){
	return renderer;
}

void Game::run(){
	while(!(state->QuitRequested())){
		calculateDeltaTime();
		INPUTMAN.Update();
		state->update();
		state->render();
		SDL_RenderPresent(renderer);
		SDL_Delay(33);
	}
}

float Game::GetDeltaTime(){
	return dt;
}



void Game::calculateDeltaTime(){
	int time = SDL_GetTicks();
	dt = (time - (frameStart))/1000.0f;
	frameStart = time;
}
