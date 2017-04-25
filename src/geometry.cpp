#include <geometry.hpp>

Vec2::Vec2(const float &a,const float &b):x{a},y{b}{}
Vec2::Vec2(const Vec2 &b):x{b.x},y{b.y}{}


void Vec2::operator=(const Vec2& b){
	x=b.x;
	y=b.y;
}

Vec2 Vec2::operator+ (const Vec2& b)const{
	return {x+b.x,y+b.y};
}
void Vec2::operator+=(const Vec2& b){
	x+=b.x;
	y+=b.y;
}

Vec2 Vec2::operator- (const Vec2& b)const{
	return {x-b.x,y-b.y};
}
void Vec2::operator-=(const Vec2& b){
	x-=b.x;
	y-=b.y;
}

Vec2 Vec2::operator* (const float& r)const{
	return {x*r,y*r};
}
void Vec2::operator*=(const float& r){
	x*=r;
	y*=r;
}

Vec2 Vec2::operator/ (const float& r)const{
	return {x/r,y/r};
}
void Vec2::operator/=(const float& r){
	x/=r;
	y/=r;
}
bool Vec2::operator==(const Vec2& b)const{
	return (equals(x,b.x) && equals(y,b.y));
}
bool Vec2::operator!=(const Vec2& b)const{
	return !((*this)==b);
}
bool Vec2::operator< (const Vec2& b)const{
	if(x==b.x)return (y<b.y);
	return x<b.x;
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

std::ostream& operator<<(std::ostream& os, const Vec2& obj){
	os << "(" << obj.x << "," << obj.y << ")";
	return os;
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
	return {x+b.x,y+b.y,w,h};
}
void Rect::operator+=(const Vec2& b){
	x+=b.x;
	y+=b.y;
}

Rect Rect::operator- (const Vec2& b)const{
	return Rect(x-b.x,y-b.y,w,h);
}
void Rect::operator-=(const Vec2& b){
	x-=b.x;
	y-=b.y;
}
bool Rect::operator==(const Rect& b){
	return (equals(x,b.x) && equals(y,b.y) && equals(w,b.w) && equals(h,b.h));
}
bool Rect::operator!=(const Rect& b){
	return !((*this)==b);
}


Vec2 Rect::corner() const{
	return {x,y};
}

Vec2 Rect::center() const{
	return {x+(w/2),y+(h/2)};
}

ConvexPolygon Rect::polygon(const float &r) const{
	ConvexPolygon pol;
	Vec2 v=corner();
	pol.AddPoint(v);
	pol.AddPoint(v+Vec2(w,0.0f).rotate(r));
	pol.AddPoint(v+Vec2(0.0f,h).rotate(r));
	pol.AddPoint(v+Vec2(w,h).rotate(r));
	return pol;
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



ConvexPolygon::ConvexPolygon():count{0}{}

bool ConvexPolygon::AddPoint(const Vec2 &p){
	if(!count){//first point
		Source()=p;
		points.push_back(Vec2());
		pointsAng[Vec2()]=0.0f;
		count++;
		return true;
	}
	Vec2 point = p-Source();//calculate point relative to source

	if(std::find(points.begin(),points.end(),point)!=points.end())return false;//duplicate

	if(count>1 && !IsConvex(point))return false;

	points.push_back(point);
	pointsAng[point]=point.angle();

	count++;

	if(point.y>0.0f || (equals(point.y,0.0f) && point.x>0))SetSource(point);
	else{
		std::sort(points.begin(),points.end(),
			[this](const Vec2 &a,const Vec2 &b){
				if(equals(pointsAng[a],0))return true;
				if(equals(pointsAng[b],0))return false;
				return pointsAng[a]>pointsAng[b];
			}
		);
	}
	return true;
}
bool ConvexPolygon::RemovePoint(const Vec2 &p){
	if(p==Vec2() && count>1){
		if(count>1){
			points.clear();
			pointsAng.clear();
			count=0;
			Source()=Vec2();
			return true;
		}
		return false;//cant remove source if its not the last one
	}

	auto it=std::find(points.begin(),points.end(),p);
	if(it!=points.end())return false;//polygon doesnt contain p

	points.erase(it);
	pointsAng.erase(p);

	count--;
	return true;
}
bool ConvexPolygon::SetSource(const Vec2 &p){
	if(count<=0)return false;
	if(p==Vec2())return true;//p is already the source

	auto it=std::find(points.begin(),points.end(),p);
	if(it==points.end())return false;//polygon doesnt contain p

	for(auto &point:points)point+=Source();

	Source()=p+Source();

	pointsAng.clear();
	for(auto &point:points){
		point-=Source();
		pointsAng[point]=point.angle();
	}
	pointsAng[Vec2()]=0.0f;

	std::sort(points.begin(),points.end(),
		[this](const Vec2 &a,const Vec2 &b){
			if(equals(pointsAng[a],0))return true;
			if(equals(pointsAng[b],0))return false;
			return pointsAng[a]>pointsAng[b];
		}
	);
	return true;
}
bool ConvexPolygon::IsConvex(const Vec2 &p){
	vector<Vec2> v=points;
	map<Vec2,float> vAng=pointsAng;
	v.push_back(p);
	v.push_back(v[0]);//insert first point at the end to make the list "ciclic"
	vAng[p]=p.angle();
	std::sort(points.begin(),points.end(),
		[this](const Vec2 &a,const Vec2 &b){
			if(equals(pointsAng.at(a),0))return true;
			if(equals(pointsAng.at(b),0))return false;
			return pointsAng.at(a)>pointsAng.at(b);
		}
	);

	auto prev=points.begin();
	bool ang1=((*prev).angle(*(std::next(prev)))==0.0f);//check the angle between the 1st and 2nd point
	bool ang2=((*prev).angle(*(std::next(prev)))< 0.0f);
	for(auto point:points){
		if(point==(*points.begin()))continue;
		if(ang1){//if direction has not been determined yet, calculate it
			ang1=((*prev).angle(*(std::next(prev)))==0.0f);
			ang2=((*prev).angle(*(std::next(prev)))< 0.0f);
		}
		else{//if it has, check if it changes
			if(ang2 != ((*prev).angle(*(std::next(prev)))< 0.0f))return false;
		}
	}
	return true;
}

bool ConvexPolygon::Contains(const Vec2 &p)const{
	if(count<3)return false;
	float ang=0.0f;
	vector<Vec2> v=GetPoints();
	v.push_back(v[0]);
	FOR(i,v.size()-1){
		ang+=p.angle(v[i+1]);
		ang-=p.angle(v[i]);
	}
	return equals(ang,0.0f);
}

bool ConvexPolygon::Collides(const ConvexPolygon& other)const{
	if(count<3 || other.GetCount()<3)return false;//degenerated polygons dont collide
	Rect a=GetBoundingRectangle();
	Rect b=other.GetBoundingRectangle();
	if(a.x+a.w <= b.x || b.x+b.w<=a.x)return false;//if bounding boxes dont collide, no need
	if(a.y+a.h <= b.y || b.y+b.h<=a.y)return false;// to do more complex collision check

	ConvexPolygon ms = MinkowskySum(other*-1.0f);
	return ms.Contains(Center());
}

int ConvexPolygon::GetCount()const{
	return count;
}

Vec2 ConvexPolygon::Center()const{
	if(count<=1)return ConstSource();
	Vec2 center;
	auto pts=GetPoints();
	for(auto &p:pts)center+=p;
	center/=pts.size();
	return center;
}

Vec2& ConvexPolygon::Source(){
	return source;
}

const Vec2& ConvexPolygon::ConstSource()const{
	return source;
}

Rect ConvexPolygon::GetBoundingRectangle()const{
	if(count<=1)return Rect(ConstSource().x,ConstSource().y,0.0f,0.0f);
	float minX,minY,maxX,maxY;
	for(auto &p:points){
		minX=min(minX,p.x);
		minY=min(minY,p.y);
		maxX=max(maxX,p.x);
		maxY=max(maxY,p.y);
	}
	return {minX,minY,maxX-minX,maxY-minY};
}

vector<Vec2> ConvexPolygon::GetPoints()const{
	vector<Vec2> vPol=points;
	for(auto &p:vPol){
		p+=ConstSource();
	}
	return vPol;
}

float ConvexPolygon::GetPointAng(const Vec2& p)const{
	if(pointsAng.count(p-ConstSource())){
		cout << "Error, checking angle of invalid point '" << p-ConstSource() << "'" << endl;
		return 0.0f;
	}
	return pointsAng.at(p-ConstSource());
}


ConvexPolygon ConvexPolygon::operator* (const float &x)const{
	ConvexPolygon pol;
	auto pts=GetPoints();
	for(auto &p:pts)pol.AddPoint(p*x);
	return pol;
}


ConvexPolygon ConvexPolygon::MinkowskySum(const ConvexPolygon& pol)const{
	if(count<=0 || pol.count<=0)return ConvexPolygon();
	if(count==1){
		ConvexPolygon ans=pol;
		ans.Source()+=ConstSource();
		return ans;
	}
	if(pol.GetCount()==1){
		ConvexPolygon ans=*this;
		ans.Source()+=pol.ConstSource();
		return ans;
	}

	vector<Vec2> points1=GetPoints();
	vector<Vec2> points2=pol.GetPoints();
	auto func = [](const Vec2& a,const Vec2& b){
					if(a.y==b.y)return a.x>b.x;
					return a.y>b.y;
				};
	auto itMin1=std::min_element(points1.begin(),points1.end(),func);
	auto itMin2=std::min_element(points2.begin(),points2.end(),func);

	vector<Vec2> vPol1;
	vPol1.insert(vPol1.end(),itMin1,points1.end());
	vPol1.insert(vPol1.end(),points1.begin(),itMin1);
	vector<Vec2> vPol2;
	vPol2.insert(vPol2.end(),itMin2,points2.end());
	vPol2.insert(vPol2.end(),points2.begin(),itMin2);

	vector<Vec2> vPol3;
	vPol3.push_back(vPol1[0]+vPol2[0]);

	auto it1=vPol1.begin();
	auto it2=vPol2.begin();
	float ang1,ang2;

	while(next(it1)!=vPol1.end() && next(it2)!=vPol2.end()){
		ang1=it1->angle(*next(it1));
		ang2=it2->angle(*next(it2));

		if(ang1==ang2){//if both are lined up, add them both together
			Vec2 v=(*next(it1))-(*it1) + (*next(it2))-(*it2);
			vPol3.push_back(vPol3.back()+v);
			it1++;
			it2++;
		}
		else if(ang1>ang2){
			Vec2 v=(*next(it1))-(*it1);
			vPol3.push_back(vPol3.back()+v);
			it1++;
		}
		else{
			Vec2 v=(*next(it2))-(*it2);
			vPol3.push_back(vPol3.back()+v);
			it2++;
		}
	}
	while(next(it1)!=vPol1.end()){
		Vec2 v=(*next(it1))-(*it1);
		vPol3.push_back(vPol3.back()+v);
		it1++;
	}
	while(next(it2)!=vPol2.end()){
		Vec2 v=(*next(it2))-(*it2);
		vPol3.push_back(vPol3.back()+v);
		it2++;
	}

	ConvexPolygon minkSum;
	for(auto &p:vPol3)minkSum.AddPoint(p);
	return minkSum;
}

std::ostream& operator<<(std::ostream& os, const ConvexPolygon& obj){
	auto pts=obj.GetPoints();
	if(pts.size()==0)return os << "{}";
	os << "{" << pts[0];
	FOR2(i,1,pts.size())os << "," << pts[i];
	os << "}";
	return os;
	// auto pts=obj.GetPoints();
	// if(pts.size()==0){
	// 	os << "{}";
	// 	return os;
	// }
	// os << "{" << pts[0] << "[" << obj.GetPointAng(pts[0]) << "]";
	// FOR2(i,1,pts.size())os << "," << pts[i] << "[" << obj.GetPointAng(pts[i]) << "]";
	// os << "} -> " << obj.ConstSource();
	// return os;
}
