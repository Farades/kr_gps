#ifndef USER_H
#define USER_H

#include "myVector.h"
#include <cmath>
#include "OscularToDecart.h"

class User
{
public:
	Vector position;
	vector<Vector> AllPos;
	Vector DistanceToSatellite[27];
	vector<Vector> VectorToVisibleSatellite[27];
	vector<Vector> Phase_Vector_Visible_Satellite[27];
	vector<Vector> LSMRes;
	double alphatemp;

	User(Vector UserPos, double angle) : position(UserPos), alphatemp(angle)
	{
	}

	void VisibleSatellite(Vector aVectorToSatellite, double t, int number, Vector aPhaseVectorSatellite);
	double cosinus(Vector VectorToSatellite);
	void Run ( double step, double t );

};


double User::cosinus(Vector VectorToSatellite)
{
	double x1 = position.getValue(0);
	double y1 = position.getValue(1);
	double z1 = position.getValue(2);

	double x2 = VectorToSatellite.getValue(0);
	double y2 = VectorToSatellite.getValue(1);
	double z2 = VectorToSatellite.getValue(2);

	double modulePosition = position.getModule();
	double moduleToSatellite = VectorToSatellite.getModule();

	double numerator = x1 * x2 + y1 * y2 + z1 * z2;
	double denominator = modulePosition * moduleToSatellite;

	return numerator / denominator;

}


void User::VisibleSatellite(Vector aVectorToSatellite, double t, int number, Vector aPhaseVectorSatellite)
{
	double visible = 0;
	double alpha = GradToRad(alphatemp);
	Vector VectorToSatellite = aVectorToSatellite;
	Vector PhaseVectorSatellite = aPhaseVectorSatellite;
	double angle = acos(cosinus(VectorToSatellite));
	VectorToSatellite.add(number);
	VectorToSatellite.add(t);
	if (abs(angle) <= GradToRad(90) - alpha  )
	{
		visible = 1;
	}
	else
	{
		visible = 0;
	}
	VectorToSatellite.add(visible);
	VectorToVisibleSatellite[number-1].push_back(VectorToSatellite);
	PhaseVectorSatellite.add(number);
	PhaseVectorSatellite.add(t);
	PhaseVectorSatellite.add(visible);
	Phase_Vector_Visible_Satellite[number-1].push_back(PhaseVectorSatellite);
}

void User::Run( double step, double t )
{
	double alphaInHour = GradToRad(15.04);
	double alphaInStep = alphaInHour * step / 3600.;

	double x = position.getValue(0);
	double y = position.getValue(1);
	double z = position.getValue(2);

	position[0] = cos(alphaInStep) * x - sin(alphaInStep) * y;
	position[1] = sin(alphaInStep) * x + cos(alphaInStep) * y;
	position[2] = z;

	AllPos.push_back(position);

}



#endif // USER_H




