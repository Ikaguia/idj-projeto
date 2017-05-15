#include <common.hpp>

#include <game.hpp>
#include <titleState.hpp>

Game game(NOMEPROJETO,1024,600);

int main(){
	GAMEINST.Push(new TitleState);
	GAMEINST.Run();
	return 0;
}

