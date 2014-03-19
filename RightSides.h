#ifndef RIGHTSIDES_H
#define RIGHTSIDES_H

#include "myVector.h"

using namespace std;

//----------------------------------------------------

class Satellite
{
private:
	Vector InitState;
	Vector CurrentState;
	vector<Vector> ResultMove;
	double step;
	double t0, tk;
	int number;

public:
	Satellite() {}
	Satellite(Vector InSt, double ah, double at0, double atk, int i) : InitState(InSt), CurrentState(InSt),
		step(ah), t0(at0), tk(atk), number(i) {}
	~Satellite() {}

	Vector Get(Vector);
	Vector Integration( Vector Xi );
	void move();
	Vector GetResult( int i );

};

#endif // RIGHTSIDES_H
