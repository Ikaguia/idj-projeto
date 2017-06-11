#include <geometry.hpp>

Vec2::Vec2(const float &a,const float &b):x{a},y{b}{}
Vec2::Vec2(const Vec2 &b):x{b.x},y{b.y}{}


Vec2 Vec2::makeVec2(const float &len,const float &ang){
	Vec2 v{len,0.0f};
	return v.rotate(ang);
}


Vec2 Vec2::operator=(const Vec2& b){
	x=b.x;
	y=b.y;
	return *this;
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

void Vec2::floor(){
	x=std::floor(x);
	y=std::floor(y);
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
	if(*this==b)return 0.0f;
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

ConvexPolygon Vec2::polygon(){
	ConvexPolygon pol;
	pol.AddPoint({});
	pol.AddPoint(*this);
	return pol;
}

std::ostream& operator<<(std::ostream& os, const Vec2& obj){
	os << "(" << obj.x << "," << obj.y << ")";
	return os;
}










Rect::Rect(const float &a,const float &b,const float &c,const float &d):x{a},y{b},w{c},h{d}{}
Rect::Rect(const Rect &b):x{b.x},y{b.y},w{b.w},h{b.h}{}



Rect Rect::operator=(const Rect& b){
	x=b.x;
	y=b.y;
	w=b.w;
	h=b.h;
	return *this;
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

void Rect::floor(){
	x=std::floor(x);
	y=std::floor(y);
	w=std::floor(w);
	h=std::floor(h);
}

Vec2 Rect::corner() const{
	return {x,y};
}
Vec2 Rect::center() const{
	return {x+(w/2),y+(h/2)};
}

void Rect::setPos(const Vec2& b) {
	x = b.x;
	y = b.y;
}

void Rect::setCenter(const Vec2& b) {
	x = b.x-(w/2);
	y = b.y-(h/2);
}

ConvexPolygon Rect::polygon(const float &r) const{
	ConvexPolygon pol;
	pol.AddPoint(Vec2{});
	pol.AddPoint(Vec2{w,0.0f}.rotate(r));
	pol.AddPoint(Vec2{0.0f,h}.rotate(r));
	pol.AddPoint(Vec2{w,h}.rotate(r));
	pol.MoveSource(corner());
	return pol;
}

SDL_Rect Rect::sdlRect()const{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
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
bool Rect::collides(const Rect& b) const{
	// if(BETWEEN(x,b.x,b.x+b.w))return true;
	// if(BETWEEN(b.x,x,x+w))return true;
	// if(BETWEEN(y,b.y,b.y+b.h))return true;
	// if(BETWEEN(b.y,y,y+h))return true;
	// return false;

	if(x>(b.x+b.w))return false;
	if(y>(b.y+b.h))return false;
	if(b.x>(x+w))return false;
	if(b.y>(y+h))return false;
	return true;
}
std::ostream& operator<<(std::ostream& os, const Rect& obj){
	os << "(" << obj.x << "," << obj.y << "," << obj.w << "," << obj.h << ")";
	return os;
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

void Circle::floor(){
	x=std::floor(x);
	y=std::floor(y);
	r=std::floor(r);
}



ConvexPolygon::ConvexPolygon():count{0}{}
ConvexPolygon::ConvexPolygon(vector<Vec2> v,bool all):count{0}{
	AddPoints(v,all);
}

bool ConvexPolygon::AddPoint(Vec2 p){
	p-=GetSource();

	if(!count)boundingRect += p;
	else{
		if(boundingRect.x>p.x)boundingRect.x = p.x;
		else if(boundingRect.x+boundingRect.w<p.x)boundingRect.w = p.x-boundingRect.x;
		if(boundingRect.y>p.y)boundingRect.y = p.y;
		else if(boundingRect.y+boundingRect.h<p.y)boundingRect.h = p.y-boundingRect.y;
	}

	if(find(points.begin(),points.end(),p-GetSource())!=points.end())return false;
	else if(!IsConvex(p))return false;
	points.push_back(p);
	ReorderPoints();
	pointsAng[p]=points[0].angle(p);
	count++;
	return true;
}
bool ConvexPolygon::AddPoints(const vector<Vec2> &pts,bool all){
	if(all){
		ConvexPolygon pol=*this;
		if(!pol.AddPoints(pts))return false;
		*this=pol;
	}
	else{
		for(auto p:pts){
			p-=GetSource();
			AddPoint(p);
		}
	}
	return true;
}
bool ConvexPolygon::RemovePoint(Vec2 p){
	auto it=find(points.begin(), points.end(),p);
	return RemovePoint(it-points.begin());
}
bool ConvexPolygon::RemovePoint(int ind){
	if(ind<0 || ind>=count)return false;
	pointsAng.erase(points[ind]);
	points.erase(points.begin()+ind);
	if(ind==0)ReorderPoints();

	float x1=0,x2=0,y1=0,y2=0;
	for(auto &p:points){
		x1=min(x1,p.x);
		x2=max(x2,p.x);
		y1=min(y1,p.y);
		y2=max(y2,p.y);
	}
	boundingRect=Rect{x1,y1,x2-x1,y2-y1};

	return true;
}

void ConvexPolygon::SetSource(const Vec2 &p){
	Vec2 move=GetSource()-p;
	for(auto &i:points)i+=move;
	source=p;
}
void ConvexPolygon::MoveSource(const Vec2 &p){
	source=p;
}
void ConvexPolygon::ReorderPoints(){
	ReorderPoints(points,pointsAng);
}
void ConvexPolygon::ReorderPoints(vector<Vec2> &pts,map<Vec2,float> &ptsAng)const{
	const auto &it=min_element(pts.begin(), pts.end(),[](const Vec2& a,const Vec2& b){
		if(equals(a.y,b.y))return a.x>b.x;
		return a.y>b.y;
	});
	std::swap(*pts.begin(),*it);
	ptsAng.clear();
	for(auto &p:pts)ptsAng[p]=pts[0].angle(p);
	ptsAng[pts[0]]=400;
	sort(pts.begin(), pts.end(),[&pts,&ptsAng](const Vec2 &a,const Vec2 &b){return ptsAng[a]>ptsAng[b];});
	ptsAng[pts[0]]=0;
}

vector<Vec2> ConvexPolygon::GetPoints()const{
	vector<Vec2> v;
	for(auto p:points)v.push_back(p+GetSource());
	return v;
}
const Vec2& ConvexPolygon::GetSource()const{
	return source;
}
Vec2 ConvexPolygon::GetCenter()const{
	Vec2 sum;
	for(auto p:points)sum+=p;
	sum/=points.size();
	return sum+GetSource();
}

int ConvexPolygon::GetCount()const{
	return count;
}
float ConvexPolygon::GetPointAng(const Vec2& p)const{
	if(pointsAng.count(p))return pointsAng.at(p);
	return 0.0f;
}
float ConvexPolygon::GetPointAng(int ind)const{
	return pointsAng.at(points[ind]);
}

bool ConvexPolygon::IsConvex(const Vec2 &p)const{
	if(count<3)return true;
	vector<Vec2> v=points;
	map<Vec2,float> vAng=pointsAng;
	v.push_back(p-GetSource());
	ReorderPoints(v,vAng);
	v.push_back(v[0]);

	float prevAng=v[0].angle(v[1]),ang;
	for(int i=1;i<=count;i++){
		ang=v[i].angle(v[i+1]);
		if(ang>prevAng)return false;
		prevAng=ang;
	}
	return true;
}
bool ConvexPolygon::IsConvex(const vector<Vec2> &pts)const{
	vector<Vec2> v=points;
	map<Vec2,float> vAng=pointsAng;
	for(auto p:pts)v.push_back(p-GetSource());
	ReorderPoints(v,vAng);
	v.push_back(v[0]);

	float prevAng=v[1].angle(v[0]),ang;
	for(int i=1;i<=count;i++){
		ang=v[i+1].angle(v[i]);
		if(ang<prevAng)return false;
		prevAng=ang;
	}
	return true;
}
bool ConvexPolygon::Contains(Vec2 p)const{
	if(count<3)return false;
	p-=GetSource();
	float ang=0.0f;
	vector<Vec2> v=points;
	v.push_back(v[0]);
	FOR(i,v.size()-1){
		ang+=p.angle(v[i+1]);
		ang-=p.angle(v[i]);
	}
	return equals(ang,360.0f) || equals(ang,0.0f);
}
bool ConvexPolygon::Collides(const ConvexPolygon& other)const{
	if(count<3 || other.GetCount()<3)return false;//degenerated polygons dont collide
	Rect a=BoundingRect();
	Rect b=other.BoundingRect();
	if((a.x+a.w <= b.x) || (b.x+b.w<=a.x))return false;//if bounding boxes dont collide, no need
	if((a.y+a.h <= b.y) || (b.y+b.h<=a.y))return false;// to do more complex collision check

	ConvexPolygon ms = (AtOrigin()*-1.0f).MinkowskySum(other);

	//if(ms.Contains(GetSource()))cout << *this << " collides with " << other << endl;

	return ms.Contains(GetSource());
}

ConvexPolygon ConvexPolygon::operator+(const Vec2& p)const{
	ConvexPolygon pol=*this;
	pol.AddPoint(p);
	return pol;
}
ConvexPolygon ConvexPolygon::operator*(const float& f)const{
	ConvexPolygon pol;
	for(auto p:points)pol.AddPoint(p*-f);
	pol.MoveSource(GetSource());
	return pol;
}

void ConvexPolygon::Floor(){
	for(auto &p:points)p.floor();
	source.floor();
}

Rect ConvexPolygon::BoundingRect()const{
	return boundingRect+GetSource();
}
ConvexPolygon ConvexPolygon::AtOrigin()const{
	ConvexPolygon pol=*this;
	pol.MoveSource(Vec2{});
	return pol;
}
ConvexPolygon ConvexPolygon::MinkowskySum(const ConvexPolygon& pol)const{
	if(count<=0 || pol.count<=0)return ConvexPolygon();
	if(count==1)return pol+GetPoints()[0];
	if(pol.GetCount()==1)return (*this)+pol.GetPoints()[0];

	vector<Vec2> vPol1=GetPoints();
	vector<Vec2> vPol2=pol.GetPoints();
	vPol1.push_back(vPol1[0]);
	vPol2.push_back(vPol2[0]);

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

	//cout << *this << " + " << pol << " = " << minkSum << endl;

	return minkSum;
}

std::ostream& operator<<(std::ostream& os, const ConvexPolygon& obj){
	// auto pts=obj.GetPoints();
	// if(pts.size()==0)return os << "{}";
	// os << "{(" << (int)pts[0].x << "," << (int)pts[0].y << ")";
	// FOR2(i,1,pts.size())os << ",(" << (int)pts[i].x << "," << (int)pts[i].y << ")";
	// os << "}";
	// return os;

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
	// os << "} -> " << obj.GetSource();
	// return os;
}
