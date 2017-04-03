#include <game.hpp>
#include <titleState.hpp>
#include <stageState.hpp>

Game game("Cristiano Krug Brust - 15/0008058",1024,600);
//coloquei game como global pois quando era local de main, o programa dava seg fault ao encerrar, pois o destrutor de main chamava TTF_Quit antes de todas as fontes serem liberadas

int main(){
	GAMEINST.Push(new TitleState);
	GAMEINST.Run();
	return 0;
}

