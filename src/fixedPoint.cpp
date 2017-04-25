#include <fixedPoint.hpp>
#include <common.hpp>

FixedPoint::FixedPoint(int v,int p):val(v),precision{p}{};

FixedPoint::operator float()const{
	return _V()/(float)_P();
}
int& FixedPoint::V(){
	return val;
}
int& FixedPoint::P(){
	return precision;
}
int FixedPoint::_V()const{
	return val;
}
int FixedPoint::_P()const{
	return precision;
}
FixedPoint FixedPoint::get(int p)const{
	return FixedPoint((val*p)/_P(),p);
}

bool FixedPoint::operator==(const FixedPoint& other)const{
	if(_P()==other._P())return _V()==other._V();
	int prec=max(_P(),other._P());
	return get(prec)==other.get(prec);
}
bool FixedPoint::operator< (const FixedPoint& other)const{
	if(_P()==other._P())return _V()< other._V();
	int prec=max(_P(),other._P());
	return get(prec)< other.get(prec);
}
bool FixedPoint::operator<=(const FixedPoint& other)const{
	if(_P()==other._P())return _V()<=other._V();
	int prec=max(_P(),other._P());
	return get(prec)<=other.get(prec);
}
bool FixedPoint::operator> (const FixedPoint& other)const{
	if(_P()==other._P())return _V()> other._V();
	int prec=max(_P(),other._P());
	return get(prec)> other.get(prec);
}
bool FixedPoint::operator>=(const FixedPoint& other)const{
	if(_P()==other._P())return _V()>=other._V();
	int prec=max(_P(),other._P());
	return get(prec)>=other.get(prec);
}

void FixedPoint::operator =(const FixedPoint& other){
	V()=other._V();
	P()=other._P();
}
void FixedPoint::operator+=(const FixedPoint& other){
	if(_P()==other._P())V()=other._V();
	int prec=max(_P(),other._P());
	*this = get(prec);
	V()+=other.get(prec)._V();
}
void FixedPoint::operator-=(const FixedPoint& other){
	if(_P()==other._P())V()-=other._V();
	int prec=max(_P(),other._P());
	*this = get(prec);
	V()-=other.get(prec)._V();
}
void FixedPoint::operator*=(const FixedPoint& other){
	if(_P()==other._P())V()*=other._V();
	int prec=max(_P(),other._P());
	*this = get(prec);
	V()*=other.get(prec)._V();
}
void FixedPoint::operator/=(const FixedPoint& other){
	if(_P()==other._P())V()*=other._V();
	int prec=max(_P(),other._P());
	*this = get(prec);
	V()/=other.get(prec)._V();
}

FixedPoint FixedPoint::operator+(const FixedPoint& other)const{
	if(_P()==other._P())return FixedPoint(_V()+other._V(),_P());
	int prec=max(_P(),other._P());
	return FixedPoint(get(prec)._V()+other.get(prec)._V(),prec);
}
FixedPoint FixedPoint::operator-(const FixedPoint& other)const{
	if(_P()==other._P())return FixedPoint(_V()-other._V(),_P());
	int prec=max(_P(),other._P());
	return FixedPoint(get(prec)._V()-other.get(prec)._V(),prec);
}
FixedPoint FixedPoint::operator*(const FixedPoint& other)const{
	if(_P()==other._P())return FixedPoint(_V()*other._V(),_P());
	int prec=max(_P(),other._P());
	return FixedPoint(get(prec)._V()*other.get(prec)._V(),prec);
}
FixedPoint FixedPoint::operator/(const FixedPoint& other)const{
	if(_P()==other._P())return FixedPoint(_V()/other._V(),_P());
	int prec=max(_P(),other._P());
	return FixedPoint(get(prec)._V()/other.get(prec)._V(),prec);
}
