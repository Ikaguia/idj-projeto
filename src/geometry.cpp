#include <geometry.hpp>

Vec2::Vec2(){}
Vec2::Vec2(const float &a,const float &b):x{a},y{b}{}
Vec2::Vec2(const Vec2 &b):x{b.x},y{b.y}{}


void Vec2::operator=(const Vec2& b){
	x=b.x;
	y=b.y;
}

Vec2 Vec2::operator+ (const Vec2& b)const{
	return Vec2(x+b.x,y+b.y);
}
void Vec2::operator+=(const Vec2& b){
	x+=b.x;
	y+=b.y;
}

Vec2 Vec2::operator- (const Vec2& b)const{
	return Vec2(x-b.x,y-b.y);
}
void Vec2::operator-=(const Vec2& b){
	x-=b.x;
	y-=b.y;
}

Vec2 Vec2::operator* (const float& r)const{
	return Vec2(x*r,y*r);
}
void Vec2::operator*=(const float& r){
	x*=r;
	y*=r;
}

Vec2 Vec2::operator/ (const float& r)const{
	return Vec2(x/r,y/r);
}
void Vec2::operator/=(const float& r){
	x/=r;
	y/=r;
}


float Vec2::len() const{
	return hypot(x,y);
}

float Vec2::angle() const{
	float f = DEGREES(atan(y / x) + (x < 0 ? PI : 0));
	return ((f<0)?(360+f):(f));
}


float Vec2::dist(const Vec2 &b) const{
	return ((*this)-b).len();
}

float Vec2::angle(const Vec2 &b) const{
	return (b-(*this)).angle();
}


Vec2 Vec2::unit() const{
	return (*this)/len();
}


Vec2 Vec2::rotate(float a){
	Vec2 v;
	a = RAD(a);//converte a para radianos, que é usado pelas funções sin e cos
	v.x = x*cos(a) - y*sin(a);
	v.y = y*cos(a) + x*sin(a);
	return v;
}










Rect::Rect(){}
Rect::Rect(const float &a,const float &b,const float &c,const float &d):x{a},y{b},w{c},h{d}{}
Rect::Rect(const Rect &b):x{b.x},y{b.y},w{b.w},h{b.h}{}



void Rect::operator=(const Rect& b){
	x=b.x;
	y=b.y;
	w=b.w;
	h=b.h;
}

Rect Rect::operator+ (const Vec2& b)const{
	Rect r(x+b.x,y+b.y,w,h);
}
void Rect::operator+=(const Vec2& b){
	x+=b.x;
	y+=b.y;
}

Rect Rect::operator- (const Vec2& b)const{
	Rect r(x-b.x,y-b.y,w,h);
}
void Rect::operator-=(const Vec2& b){
	x-=b.x;
	y-=b.y;
}


Vec2 Rect::center() const{
	return Vec2(x+(w/2),y+(h/2));
}


float Rect::dist(const Rect& b) const{
	return center().dist(b.center());
}


bool Rect::contains(const float &i,const float &j) const{
	if(i<x)return false;
	if(i>x+w)return false;
	if(j<y)return false;
	if(j>y+h)return false;
	return true;
}
bool Rect::contains(const Vec2& b) const{
	if(b.x<x)return false;
	if(b.x>x+w)return false;
	if(b.y<y)return false;
	if(b.y>y+h)return false;
	return true;
}




Circle::Circle(float xx,float yy,float rr):x{xx},y{yy},r{rr}{}

bool Circle::contains(const Vec2 &p)const{
	Vec2 center(x,y);
	return (center-p).len()<=r;
}
bool Circle::contains(const float &px,const float &py)const{
	Vec2 center(x,y);
	Vec2 p(px,py);
	return (center-p).len()<=r;
}
