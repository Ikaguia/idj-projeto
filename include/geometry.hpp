#ifndef GEOMETRYHPP
#define GEOMETRYHPP

#include <cmath>

#include <common.hpp>

#define PI 3.141592653589793
#define RAD(x)     (((x)*PI)/180.0)
#define DEGREES(x) (((x)*180.0)/PI)

class Vec2{
public:
	float x;
	float y;

	Vec2();
	Vec2(const float &a,const float &b);
	Vec2(const Vec2 &b);

	void operator= (const Vec2& b);
	Vec2 operator+ (const Vec2& b)const;
	void operator+=(const Vec2& b);
	Vec2 operator- (const Vec2& b)const;
	void operator-=(const Vec2& b);
	Vec2 operator* (const float& r)const;
	void operator*=(const float& r);
	Vec2 operator/ (const float& r)const;
	void operator/=(const float& r);

	bool operator==(const Vec2& b);

	float len() const;//magnitude do vetor
	float angle() const;//angulo entre this e o eixo x

	float dist(const Vec2& b) const;//distancia entre this e b
	float angle(const Vec2& b) const;//inclinacao da reta definida por this e b

	Vec2 unit() const;//vetor unitario

	Vec2 rotate(float a);//rotaciona o vetor em a graus
};

class Rect{
public:
	float x;
	float y;
	float w;
	float h;

	Rect();
	Rect(const float &a,const float &b,const float &c,const float &d);
	Rect(const Rect &b);

	void operator= (const Rect& b);
	Rect operator+ (const Vec2& b)const;
	void operator+=(const Vec2& b);
	Rect operator- (const Vec2& b)const;
	void operator-=(const Vec2& b);

	bool operator==(const Rect& b);

	Vec2 center() const;//retorna o centro do retangulo

	float dist(const Rect& b) const;//retorna a distancia entre os centros dos retangulos

	bool contains(const float &i,const float &j) const;//retorna se o ponto pertence ao retangulo
	bool contains(const Vec2& b) const;//retorna se o ponto pertence ao retangulo
};

class Circle{
public:
	float x;
	float y;
	float r;
	Circle(float x=0,float y=0,float r=1);

	bool contains(const Vec2 &p)const;//retorna se o ponto pertence ào circulo
	bool contains(const float &x,const float &y)const;//retorna se o ponto pertence ào circulo
};

#endif//GEOMETRYHPP
