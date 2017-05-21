#ifndef FIXEDPOINTHPP
#define FIXEDPOINTHPP

class FixedPoint {
	int val;
	int precision;
public:
	FixedPoint(int v,int p=1000);

	operator float()const;
	int& V();//get val reference
	int& P();//ge precision reference
	int _V()const;
	int _P()const;
	FixedPoint get(int p)const;

	bool operator==(const FixedPoint& other)const;
	bool operator< (const FixedPoint& other)const;
	bool operator<=(const FixedPoint& other)const;
	bool operator> (const FixedPoint& other)const;
	bool operator>=(const FixedPoint& other)const;

	void operator =(const FixedPoint& other);
	void operator+=(const FixedPoint& other);
	void operator-=(const FixedPoint& other);
	void operator*=(const FixedPoint& other);
	void operator/=(const FixedPoint& other);

	FixedPoint operator+(const FixedPoint& other)const;
	FixedPoint operator-(const FixedPoint& other)const;
	FixedPoint operator*(const FixedPoint& other)const;
	FixedPoint operator/(const FixedPoint& other)const;
};

#endif//FIXEDPOINTHPP
