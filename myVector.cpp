#include "myVector.h"
#include <iostream>

using namespace std;


Vector Vector::operator+ ( Vector vArg ) const
{
    Vector temp(v.size());

	for (int j = 0; j < v.size(); j ++)
    {
		temp[j] = v[j] + vArg.v[j];
    }

    return temp;

}

//----------------------------------------------------

Vector Vector::operator- ( Vector vArg ) const
{
    Vector temp(v.size());

	for (int j = 0; j < v.size(); j ++)
    {
        temp.v[j] = v[j] - vArg.v[j];
    }

    return temp;
}

//----------------------------------------------------

Vector Vector::operator* (double n) const
{
    Vector temp(v.size());

	for (int j = 0; j < v.size(); j++)
        temp[j] = v[j] * n;

    return temp;
}

//----------------------------------------------------

double& Vector::operator[] (int n)
{
    return v[n];
}

//----------------------------------------------------

void Vector::add(double temp)
{
    v.push_back(temp);
}

//----------------------------------------------------

void Vector::display() const
{
	for (int j = 0; j < v.size(); j++)
        cout << v[j] << endl;

}

//----------------------------------------------------

int Vector::size() const
{
    return v.size();
}

//----------------------------------------------------

double Vector::getValue( int i ) const
{
	return v[i];
}

//----------------------------------------------------

void Vector::resize( int n )
{
	v.resize(n);
}

//----------------------------------------------------

double Vector::getModule() const
{
	return sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

