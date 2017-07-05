#include <common.hpp>

int debugCount=0;
pair<float,float> HotspotPos[] = {
	// TOP_LEFT,
	{0.0,0.0},
	// TOP,
	{0.5,0.0},
	// TOP_RIGHT,
	{1.0,0.0},
	// LEFT,
	{0.0,0.5},
	// CENTER,
	{0.5,0.5},
	// RIGHT,
	{1.0,0.5},
	// BOTTOM_LEFT,
	{0.0,1.0},
	// BOTTOM,
	{0.5,1.0},
	// BOTTOM_RIGHT
	{1.0,1.0}
};

SDL_Color MakeColor(int r,int g,int b,int a){
	SDL_Color color;
	color.r=r;
	color.g=g;
	color.b=b;
	color.a=a;
	return color;
}


bool equals(const float &a,const float &b){
	return (std::fabs((a-b))<=PRECISION);
}

float closeDist(const float &from,const float &to,const float &change){
	if(abs(from-to)<change)return to;
	if(from>to)return from - change;
	return from + change;
}

string FloatToStr(float f) {
	char s[15];
	sprintf(s,"%.2f",f);
	return s;
}
