#ifndef GEOMETRYHPP
#define GEOMETRYHPP

#include <common.hpp>

#define PI 3.141592653589793
#define RAD(x)     (((x)*PI)/180.0)
#define DEGREES(x) (((x)*180.0)/PI)

class Vec2;
class Rect;
class Circle;
class ConvexPolygon;

class Vec2{
public:
	float x;
	float y;

	Vec2(const float &a=0.0f,const float &b=0.0f);
	Vec2(const Vec2 &b);

	Vec2 operator= (const Vec2& b);
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

	void floor();

	float len() const;//magnitude do vetor
	float angle() const;//angulo entre this e o eixo x

	float dist(const Vec2& b) const;//distancia entre this e b
	float angle(const Vec2& b) const;//inclinacao da reta definida por this e b

	Vec2 unit() const;//vetor unitario

	Vec2 rotate(float a);//rotaciona o vetor em a graus

	ConvexPolygon polygon();//retorna a semireta definida pelo vetor e pela origem
};
std::ostream& operator<<(std::ostream& os, const Vec2& obj);

class Rect{
public:
	float x;
	float y;
	float w;
	float h;

	Rect(const float &a=0,const float &b=0,const float &c=0,const float &d=0);
	Rect(const Rect &b);

	Rect operator= (const Rect& b);
	Rect operator+ (const Vec2& b)const;
	void operator+=(const Vec2& b);
	Rect operator- (const Vec2& b)const;
	void operator-=(const Vec2& b);

	bool operator==(const Rect& b);
	bool operator!=(const Rect& b);

	void floor();

	Vec2 corner() const;//retorna o canto do retangulo
	Vec2 center() const;//retorna o centro do retangulo
	
	void setPos(const Vec2& b);
	void setCenter(const Vec2& b); 

	ConvexPolygon polygon(const float &r=0.0f) const;

	float dist(const Rect& b) const;//retorna a distancia entre os centros dos retangulos

	bool contains(const float &i,const float &j) const;//retorna se o ponto pertence ao retangulo
	bool contains(const Vec2& b) const;//retorna se o ponto pertence ao retangulo

	bool collides(const Rect& b) const;//retorna se o retangulo tem interseção com b
};
ostream& operator<<(ostream& os, const Rect& obj);

class Circle{
public:
	float x;
	float y;
	float r;
	Circle(float x=0.0f,float y=0.0f,float r=float(1000));

	void floor();

	bool contains(const Vec2 &p)const;//retorna se o ponto pertence ào circulo
	bool contains(const float &x,const float &y)const;//retorna se o ponto pertence ào circulo
};

class ConvexPolygon{
	vector<Vec2> points;
	map<Vec2,float> pointsAng;//angle point-points[0] to the origin
	Vec2 source;
	int count;
	Rect boundingRect;
public:
	ConvexPolygon();
	ConvexPolygon(vector<Vec2> v,bool all=false);

	bool AddPoint(Vec2 p=Vec2{0.0f,0.0f});//doesnt work if the resulting polygon is not convex
	bool AddPoints(const vector<Vec2> &pts,bool all=false);//if all==true, doesnt add any if adding all wont make a convex polygon
	bool RemovePoint(Vec2 p);//doesnt work on non-existing points
	bool RemovePoint(int ind);//doesnt work on non-existing points

	void SetSource(const Vec2 &p);//changes the source and updates all points
	void MoveSource(const Vec2 &p);//changes the source and keeps all points in the same relative position
	void ReorderPoints();//reorders the points with the lowest rightmost becoming the new points[0]
	void ReorderPoints(vector<Vec2> &pts,map<Vec2,float> &ptsAng)const;

	vector<Vec2> GetPoints()const;
	const Vec2& GetSource()const;
	Vec2 GetCenter()const;

	int GetCount()const;
	float GetPointAng(const Vec2& p)const;
	float GetPointAng(int ind)const;

	bool IsConvex(const vector<Vec2> &pts)const;//tests if the polygon will remain convex if you add points pts
	bool IsConvex(const Vec2 &p)const;//tests if the polygon will remain convex if you add point p
	bool Contains(Vec2 p)const;
	bool Collides(const ConvexPolygon& other)const;

	ConvexPolygon operator+(const Vec2& p)const;
	ConvexPolygon operator* (const float &x)const;

	void Floor();

	Rect BoundingRect()const;
	ConvexPolygon AtOrigin()const;
	ConvexPolygon MinkowskySum(const ConvexPolygon& pol)const;
};
std::ostream& operator<<(std::ostream& os, const ConvexPolygon& obj);

#endif//GEOMETRYHPP
