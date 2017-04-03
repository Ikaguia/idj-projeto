#ifndef COMMONHPP
#define COMMONHPP

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <map>
#include <unordered_map>
#include <array>
#include <bitset>
#include <queue>
#include <stack>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::map;
using std::unordered_map;
using std::array;
using std::bitset;
using std::queue;
using std::stack;

#define FOR(  cont,    max) for(int (cont)=0;           (cont)<(int)(max); (cont)++)
#define FOR2( cont,min,max) for(int (cont)=(int)(min);  (cont)<(int)(max); (cont)++)
#define FORR( cont,    max) for(int (cont)=(int)(max-1);(cont)>=0;         (cont)--)
#define FOR2R(cont,min,max) for(int (cont)=(int)(max-1);(cont)>=(int)(min);(cont)--)
#define ABS(x) (((x)<0)?-(x):(x))

#define RESOURCESFOLDER (string)""

SDL_Color MakeColor(int r,int g,int b,int a=255);

#define SDL_COLOR_BLACK MakeColor(0,0,0)
#define SDL_COLOR_RED   MakeColor(255,0,0)
#define SDL_COLOR_GREEN MakeColor(0,255,0)
#define SDL_COLOR_BLUE  MakeColor(0,0,255)
#define SDL_COLOR_WHITE MakeColor(255,255,255)

#endif//COMMONHPP