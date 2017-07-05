#ifndef COMMONHPP
#define COMMONHPP

#include <algorithm>
#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

using std::array;
using std::bitset;
using std::ceil;
using std::cout;
using std::cerr;
using std::endl;
using std::find;
using std::floor;
using std::function;
using std::getline;
using std::ifstream;
using std::isdigit;
using std::map;
using std::max;
using std::make_unique;
using std::min;
using std::next;
using std::ofstream;
using std::ostream;
using std::pair;
using std::queue;
using std::round;
using std::set;
using std::shared_ptr;
using std::stack;
using std::string;
using std::stringstream;
using std::istringstream;
using std::to_string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;
using std::make_pair;

using ii=pair<int,int>;
using uint=unsigned int;

#define FOR(  cont,    max) for(int (cont)=0 	       ,_max_=(max);(cont)< _max_;(cont)++)
#define FOR2( cont,min,max) for(int (cont)=(int)(min)  ,_max_=(max);(cont)< _max_;(cont)++)
#define FORR( cont,    max) for(int (cont)=(int)(max-1)            ;(cont)>=0;    (cont)--)
#define FOR2R(cont,min,max) for(int (cont)=(int)(max-1),_min_=(min);(cont)>=_min_;(cont)--)
#define ABS(x) (((x)<0)?-(x):(x))
#define BETWEEN(x,a,b) ((x>=a) && (x<b))
#define DEBUG(x) cerr << #x << " = " << x << endl;
#define UNUSED(x) ((void)(x))
#define COUT(x) cout << #x << " "
#define COUTL(x) cout << #x << endl
#define DEBUG_ cout << "DEBUG " << debugCount++ << endl
#define RAND(x) (rand()%(x))
#define RANDR(low,high) ((low) + (rand()%((high)-(low))))

extern int debugCount;

#define SET_COLOR(color) SDL_SetRenderDrawColor(GAMERENDER,color)
#define SET_COLOR4(r,g,b,a) SDL_SetRenderDrawColor(GAMERENDER,(r),(g),(b),(a))
#define CLEAR_SCREEN() SDL_RenderClear(GAMERENDER)
#define DRAW_RECT(r) SDL_RenderDrawRect(GAMERENDER, (r))
#define FILL_RECT(r) SDL_RenderFillRect(GAMERENDER, (r))
#define DRAW_LINE(x0,y0,x,y) SDL_RenderDrawLine(GAMERENDER,(x0),(y0),(x),(y))

#define SDL_COLOR_BLACK MakeColor(0,0,0)
#define SDL_COLOR_RED   MakeColor(255,0,0)
#define SDL_COLOR_GREEN MakeColor(0,255,0)
#define SDL_COLOR_BLUE  MakeColor(0,0,255)
#define SDL_COLOR_WHITE MakeColor(255,255,255)

#define PRECISION 0.0001
#define PLAYER_JUMP 8.1//m/s
#define WALK_SPEED 2.4//m/s
#define METER 100//pixels
//#define GRAVITY 9.80665//m/s
#define GRAVITY 15.0//m/s

#define DATA_PATH (string)"data/"
#define ANIMATION_PATH DATA_PATH+"animation/"
#define BLUEPRINT_PATH DATA_PATH+"blueprint/"
#define PROJECTILE_PATH DATA_PATH+"projectile/"
#define LEVEL_PATH DATA_PATH+"level/"

#define NOMEPROJETO "IDJ - Projeto"
#define RENDERCOLLISION


typedef enum {TOP_LEFT,TOP,TOP_RIGHT,LEFT,CENTER,RIGHT,BOTTOM_LEFT,BOTTOM,BOTTOM_RIGHT} Hotspot;
extern pair<float,float> HotspotPos[];

SDL_Color MakeColor(int r,int g,int b,int a=255);

bool equals(const float &a,const float &b);
float closeDist(const float &from,const float &to,const float &change);

string FloatToStr(float f);

#endif//COMMONHPP
