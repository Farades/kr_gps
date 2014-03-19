#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "RightSides.h"
#include "User.h"
#include "LSM.h"
#include <cmath>
#include "myVector.h"

class Simulator
{
public:
	double step;
	double t0, tk;
	Satellite* SatPtr[27];
	User* user;
	LSM* lsm;
public:
	Simulator( double ah, double at0, double atk, Vector UserPos, double alpha ) :
	  step(ah), t0(at0), tk(atk)
	  {
		  user = new User(UserPos, alpha);
	  }
	  ~Simulator() {}

	  Vector OscularToDecart( Vector InOscular );
	  double GradToRad(double grad);
	  void Run();
};


void Simulator::Run()
{
	Vector InitOscul(6);
	Vector InitDecart(6);

	int multiplier_omega  = 0;
	int multiplier_anomal = 0;

	for (int i = 0; i < 27; i++)
	{
		InitOscul[0]  = 56.;
		InitOscul[1]  = 120. * multiplier_omega;
		InitOscul[2]  = 0;
		InitOscul[3]  = 29378.137;
		InitOscul[4]  = 29378.137;
		InitOscul[5]  = 40. * multiplier_anomal;

		InitDecart = OscularToDecart(InitOscul);
		SatPtr[i] = new Satellite(InitDecart, step, t0, tk, i+1 );
		SatPtr[i]->move();

		if (i == 8)
			multiplier_omega = 1;
		if (i == 17)
			multiplier_omega = 2;

		multiplier_anomal++;

		if ((i+1) % 9 == 0)
			multiplier_anomal = 0;
	}


	double t = t0;
	int i = 0;

	int iteration = static_cast<int>((tk - t0) / step) + 1;

	lsm = new LSM(user, iteration);


	while ( t <= tk)
	{
		for (int j = 0; j < 27; j++)
		{
			user->VisibleSatellite(SatPtr[j]->GetResult(i) - user->position, t, j+1, SatPtr[j]->GetResult(i));
			user->DistanceToSatellite[j].add(user->VectorToVisibleSatellite[j][i].getModule()) ;
		}

        lsm->Run(t, i);


		user->Run(step, t);

		t += step;
		i++;

	}

   	lsm->getResult();

}


double Simulator::GradToRad(double grad)
{
	const double PIz = 3.141592653589793;
	const double mu = 3.98603E14;
	return  grad * PIz / 180;
}

//----------------------------------------------------

Vector Simulator::OscularToDecart( Vector InOscular )
{
	const int RE  = 6371000;
	const double PIz = 3.141592653589793;
	const double mu = 3.98603E14;

	double i      = GradToRad(InOscular[0]);
	double omega  = GradToRad(InOscular[1]);
	double w      = GradToRad(InOscular[2]);
	double ha     = InOscular[3] * 1000;
	double hp     = InOscular[4] * 1000;
	double anomal = GradToRad(InOscular[5]);

	double Ra = RE + ha;
	double Rp = RE + hp;

	double sinU = sin(anomal) * cos(w) + cos(anomal) * sin(w);
	double CosU = cos(anomal) * cos(w) - sin(anomal) * sin(w);

	double a  = (Ra+Rp) / 2;
	double ex = (Ra-Rp) / (Ra+Rp);
	double p  = a * (1 - pow(ex, 2));
	double r  = p / (1+ex*cos(anomal));

	double Vr = sqrt(mu/p) * ex * sin(anomal);
	double Vn = sqrt(mu/p) * (1 + ex * cos(anomal));
	double u  = anomal + w;

	double x = r * ( CosU * cos(omega) - sinU * sin(omega) * cos(i) );
	double y = r * ( CosU * sin(omega) + sinU * cos(omega) * cos(i) );
	double z = r * sinU * sin(i);

	double Vx = x * Vr / r + Vn * ( -sinU * cos(omega) - CosU * sin(omega) * cos(i) );
	double Vy = y * Vr / r + Vn * ( -sinU * sin(omega) + CosU * cos(omega) * cos(i) );
	double Vz = z * Vr / r + Vn * CosU * sin(i);

	Vector temp;
	temp.add(x);
	temp.add(y);
	temp.add(z);
	temp.add(Vx);
	temp.add(Vy);
	temp.add(Vz);

	return temp;
}

#endif // SIMULATOR_H


