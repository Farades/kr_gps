#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <vector>
#include <cmath>

using namespace std;

class Vector
{
private:
    vector<double> v;

public:
    Vector() : v(0) {}
	Vector(int n ) : v(n) {}
	~Vector() {}

    Vector operator+ (Vector) const;
	Vector operator- (Vector) const;
	Vector operator* (double) const;
	double& operator[] (int);

    void add(double);
    void display() const;
	int size() const;
	double getValue( int i ) const;
	void resize( int );
	double getModule() const;

};

#endif // MYVECTOR_H
