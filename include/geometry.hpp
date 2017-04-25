#ifndef GEOMETRYHPP
#define GEOMETRYHPP

#include <cmath>

#include <common.hpp>

#define PI 3.141592653589793
#define RAD(x)     (((x)*PI)/180.0)
#define DEGREES(x) (((x)*180.0)/PI)

class Vec2;
class Rect;
class ConvexPolygon;

class Vec2{
public:
	float x;
	float y;

	Vec2(const float &a=0.0f,const float &b=0.0f);
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

	bool operator==(const Vec2& b)const;
	bool operator!=(const Vec2& b)const;
	bool operator< (const Vec2& b)const;

	float len() const;//magnitude do vetor
	float angle() const;//angulo entre this e o eixo x

	float dist(const Vec2& b) const;//distancia entre this e b
	float angle(const Vec2& b) const;//inclinacao da reta definida por this e b

	Vec2 unit() const;//vetor unitario

	Vec2 rotate(float a);//rotaciona o vetor em a graus
};
std::ostream& operator<<(std::ostream& os, const Vec2& obj);

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
	bool operator!=(const Rect& b);

	Vec2 corner() const;//retorna o canto do retangulo
	Vec2 center() const;//retorna o centro do retangulo

	ConvexPolygon polygon(const float &r=0.0f) const;

	float dist(const Rect& b) const;//retorna a distancia entre os centros dos retangulos

	bool contains(const float &i,const float &j) const;//retorna se o ponto pertence ao retangulo
	bool contains(const Vec2& b) const;//retorna se o ponto pertence ao retangulo
};

class Circle{
public:
	float x;
	float y;
	float r;
	Circle(float x=0.0f,float y=0.0f,float r=float(1000));

	bool contains(const Vec2 &p)const;//retorna se o ponto pertence ào circulo
	bool contains(const float &x,const float &y)const;//retorna se o ponto pertence ào circulo
};

class ConvexPolygon{
	vector<Vec2> points;//all points are saved in a relative position to the source point(first inserted)
	map<Vec2,float> pointsAng;
	Vec2 source;
	int count;
public:
	ConvexPolygon();

	bool AddPoint(const Vec2 &p);
	bool RemovePoint(const Vec2 &p);//doesnt work on non-existing points or the source
	bool SetSource(const Vec2 &p);//returns false if p doesnt belong to the polygon
	bool IsConvex(const Vec2 &p);//tests if the polygon will remain convex if you add point p
	bool Contains(const Vec2 &p)const;
	bool Collides(const ConvexPolygon& other)const;
	int GetCount()const;
	Vec2 Center()const;
	Vec2& Source();
	const Vec2& ConstSource()const;
	Rect GetBoundingRectangle()const;
	vector<Vec2> GetPoints()const;
	float GetPointAng(const Vec2& p)const;

	ConvexPolygon operator* (const float &x)const;

	ConvexPolygon MinkowskySum(const ConvexPolygon& pol)const;
};
std::ostream& operator<<(std::ostream& os, const ConvexPolygon& obj);

#endif//GEOMETRYHPP
