#include "myVector.h"
#include <cmath>

const double PI = 3.141592653589793;
const double mu = 3.98603E14;

//----------------------------------------------------

double GradToRad(double grad)
{
	return  grad * PI / 180;
}

//----------------------------------------------------

Vector OscularToDecart( Vector InOscular )
{
	const int RE  = 6371000;

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

