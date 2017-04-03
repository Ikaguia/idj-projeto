#ifndef COMMONHPP
#define COMMONHPP

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <array>
#include <bitset>
#include <queue>


using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::unordered_map;
using std::array;
using std::bitset;
using std::queue;

#define FOR(  cont,    max) for(int (cont)=0;      (cont)<(max); (cont)++)
#define FOR2( cont,min,max) for(int (cont)=(min);  (cont)<(max); (cont)++)
#define FORR( cont,    max) for(int (cont)=(max-1);(cont)>=0;    (cont)--)
#define FOR2R(cont,min,max) for(int (cont)=(max-1);(cont)>=(min);(cont)--)
#define ABS(x) (((x)<0)?-(x):(x))


#endif//COMMONHPP