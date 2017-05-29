#include <common.hpp>

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

string FloatToStr(float f) {
	char s[15];
	sprintf(s,"%.2f",f);
	return s;
}	
