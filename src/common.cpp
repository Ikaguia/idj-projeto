#include <common.hpp>

int debugCount=0;

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

