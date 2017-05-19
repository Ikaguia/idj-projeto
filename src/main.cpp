#include <common.hpp>

#include <game.hpp>
#include <stateTitle.hpp>

Game game(NOMEPROJETO,1366,768);

int main(){
	GAMEINST.Push(new StateTitle);
	GAMEINST.Run();
	return 0;
}

