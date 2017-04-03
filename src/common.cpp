#include <common.hpp>

SDL_Color MakeColor(int r,int g,int b,int a){
	SDL_Color color;
	color.r=r;
	color.g=g;
	color.b=b;
	color.a=a;
	return color;
}
