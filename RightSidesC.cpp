#include "RightSides.h"
#include <cmath>

using namespace std;

const double mu = 3.98603E14;

//----------------------------------------------------

Vector Satellite::Get(Vector StateVector)
{
    int raz = StateVector.size();

    Vector temp(raz);

    for (int j = 0; j < 3; j++)
        temp[j] = StateVector[j + 3];

    double r = pow(StateVector[0] * StateVector[0] + StateVector[1] * StateVector[1] + StateVector[2] * StateVector[2], 0.5);

    for (int i = 3; i < 6; i++)
		temp[i]=-(mu*StateVector[i-3])/(r*r*r);

    return temp;

}

//----------------------------------------------------

Vector Satellite::Integration(Vector Xi )
{
    Vector k, k1, k2, k3, k4;

	k1 = Get(Xi);
	k2 = Get(Xi +  k1 * (0.5 * step));
	k3 = Get(Xi +  k2 * (0.5 * step));
	k4 = Get(Xi + k3 * step);

	k = k1 + k2 * 2 + k3 * 2 + k4;

	return Xi + k * (step / 6);
}

//----------------------------------------------------

void Satellite::move()
{
	double t = t0;
	int i = 0;

	ResultMove.push_back(CurrentState);

	while (t <= tk)
	{
		t += step;
		i++;
		CurrentState = Integration(CurrentState);
		ResultMove.push_back(CurrentState);
	}
}

//----------------------------------------------------

Vector Satellite::GetResult( int i )
{
    return ResultMove[i];
}
