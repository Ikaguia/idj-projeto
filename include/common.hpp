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
#include <cmath>
#include <utility>
#include <algorithm>

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
using std::pair;
using std::min;
using std::max;
using std::next;

using ii=pair<int,int>;

#define FOR(  cont,    max) for(int (cont)=0;           (cont)<(int)(max); (cont)++)
#define FOR2( cont,min,max) for(int (cont)=(int)(min);  (cont)<(int)(max); (cont)++)
#define FORR( cont,    max) for(int (cont)=(int)(max-1);(cont)>=0;         (cont)--)
#define FOR2R(cont,min,max) for(int (cont)=(int)(max-1);(cont)>=(int)(min);(cont)--)
#define ABS(x) (((x)<0)?-(x):(x))
#define BETWEEN(x,a,b) ((x>=a) && (x<b))

#define RESOURCESFOLDER (string)""

#define SDL_COLOR_BLACK MakeColor(0,0,0)
#define SDL_COLOR_RED   MakeColor(255,0,0)
#define SDL_COLOR_GREEN MakeColor(0,255,0)
#define SDL_COLOR_BLUE  MakeColor(0,0,255)
#define SDL_COLOR_WHITE MakeColor(255,255,255)

#define WALK_SPEED 1.4//m/s
#define METER 100//pixels
//#define GRAVITY 9.80665//m/s
#define GRAVITY 15.0//m/s

#define NOMEPROJETO "IDJ - Projeto"

SDL_Color MakeColor(int r,int g,int b,int a=255);

bool equals(const float &a,const float &b);

#endif//COMMONHPP