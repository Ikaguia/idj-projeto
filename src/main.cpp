#include <common.hpp>

#include <game.hpp>
#include <stateTitle.hpp>

Game game(NOMEPROJETO,1024,768);

int main(){
	GAMEINST.Push(new StateTitle);
	GAMEINST.Run();
	return 0;
}

