#ifndef LSM_H
#define LSM_H

#include "matrix.h"
#include "myVector.h"
#include "White_noise.h"
#include <cstdlib>
#include <time.h>
#include "User.h"
#include <cmath>
#include <iostream>
#include <fstream>

class LSM
{
private:
	White_noise WN;

	User* user;
	int iteration;
	Matrix K_inverse;
	Matrix PsevdoRangeMeas;
	Matrix X[350];
	Matrix PsevdoRange;
	Matrix DiscrepancyVector;
	Matrix DirectionalCosines;
	Matrix DirectionalCosines_transpose;
	Matrix temp;
	Matrix temp1;
	Matrix temp1_inverse;
	Matrix temp2;
	Matrix Right;

public:
	vector<Vector> RESULT;
	vector<Vector> Array_Optimal_Galaxy_Full[4];
	vector<Matrix> RESULT_MATRIX;
	LSM(User* user_temp, int iter) : WN(), iteration(iter)
	{
		DirectionalCosines = Matrix(iteration * 4, 5, 0);
		DirectionalCosines_transpose = Matrix(5, iteration * 4, 0);
		K_inverse = Matrix(iteration * 4, iteration * 4, 0);
		PsevdoRangeMeas = Matrix(iteration * 4, 1, 0);

		temp = Matrix(5, iteration * 4, 0);
		temp1 = Matrix(5, 5, 0);

		X[0] = Matrix(5, 1, 0);


		for (int i = 0; i < iteration * 4; i++)
		{
			for (int j = 0; j < iteration * 4; j++)
			{
				if (i == j)
					K_inverse[i][j] = 0.01;
			}
		}

		srand(time(NULL));
		user = user_temp;
	}

	void Optimal_Galaxy(int iter);
	void Run(double t, int iter);
	void Psevdo_Range_Meas(double t, int iter);
	Matrix Psevdo_Range(Matrix Xk);
	Matrix Discrepancy(Matrix PsevdoRangeMeas, Matrix PsevdoRange);
	Matrix Directional_Cosines(Matrix Xk);
	Vector getResult();
	Matrix Inverse(Matrix m);
};



//#include "LSM.h"

const double C = 300000000.0;

const double betta1 = 0.00000001;
const double betta0 = 0.001;


void LSM::Optimal_Galaxy(int iter)       // выбор оптимального созвездия (4 спутник)
{

	int counter = 0;

	Vector Array_Optimal_Galaxy[4];

	for (int i = 0; i < 27; i++)
	{
		if (user->Phase_Vector_Visible_Satellite[i][iter].getValue(8) == 1)  // Если спутник видим для потребителя в данный момент времени
		{
			Array_Optimal_Galaxy_Full[counter].push_back(user->Phase_Vector_Visible_Satellite[i][iter]);
			counter++;
		}
		if (counter > 3) break;
	}
}

//------------------------------------------------------------------

void LSM::Psevdo_Range_Meas(double t, int iterat)    // расчет измереных псевдодальностей
{

	int i1 = iterat;

	double xp = user->position.getValue(0);
	double yp = user->position.getValue(1);
	double zp = user->position.getValue(2);


	for (int i = 0; i < 4; i++)
	{
		double xs = Array_Optimal_Galaxy_Full[i][iterat].getValue(0);
		double ys = Array_Optimal_Galaxy_Full[i][iterat].getValue(1);
		double zs = Array_Optimal_Galaxy_Full[i][iterat].getValue(2);

		double r = sqrt( (xp - xs)*(xp - xs) + (yp - ys)*(yp - ys) + (zp - zs)*(zp - zs) )
		+ C * (betta0 + betta1 * t) + WN.get(0, 10) ;

		PsevdoRangeMeas[iterat * 4 + i][0] = r;
	}
}



Matrix LSM::Directional_Cosines(Matrix Xk)     // Расчет матрицы H
{
	Matrix Result(iteration * 4, 5);

	double xpgrin = Xk[0][0];
	double ypgrin = Xk[1][0];
	double zpgrin = Xk[2][0];


	double alphaInHour = GradToRad(15.04);
	double alphaInStep = alphaInHour * 100 / 3600.;

	for (int iterat = 0; iterat < iteration; iterat++)
	{
		double t = Array_Optimal_Galaxy_Full[0][iterat].getValue(7);
		for (int i = 0; i < 4; i++)
		{
			double xp = cos(alphaInStep * iterat) * xpgrin - sin(alphaInStep * iterat) * ypgrin;
			double yp = sin(alphaInStep * iterat) * xpgrin + cos(alphaInStep * iterat) * ypgrin;
			double zp = zpgrin;


			double xs = Array_Optimal_Galaxy_Full[i][iterat].getValue(0);
			double ys = Array_Optimal_Galaxy_Full[i][iterat].getValue(1);
			double zs = Array_Optimal_Galaxy_Full[i][iterat].getValue(2);

			Result[iterat * 4 + i][0] = (xp - xs) / sqrt((xp - xs)*(xp - xs) + (yp - ys)*(yp - ys) + (zp - zs)*(zp - zs));
			Result[iterat * 4 + i][1] = (yp - ys) / sqrt((xp - xs)*(xp - xs) + (yp - ys)*(yp - ys) + (zp - zs)*(zp - zs));
			Result[iterat * 4 + i][2] = (zp - zs) / sqrt((xp - xs)*(xp - xs) + (yp - ys)*(yp - ys) + (zp - zs)*(zp - zs));
			Result[iterat * 4 + i][3] = C;
			Result[iterat * 4 + i][4] = C*t;
		}
	}

	return Result;
}


void LSM::Run(double t, int i)
{
	Optimal_Galaxy(i);
	Psevdo_Range_Meas(t,i);
}

//------------------------------------------------------------------

Matrix LSM::Psevdo_Range(Matrix Xk)  // расчет псевдодальностей от Xk
{
	Matrix Result(iteration * 4, 1);

	double xpgrin = Xk[0][0];
	double ypgrin = Xk[1][0];
	double zpgrin = Xk[2][0];
	double b0 = Xk[3][0];
	double b1 = Xk[4][0];

	double alphaInHour = GradToRad(15.04);
	double alphaInStep = alphaInHour * 100 / 3600.;

	for (int iterat = 0; iterat < iteration; iterat++)
	{
		double t = Array_Optimal_Galaxy_Full[0][iterat].getValue(7);
		for (int i = 0; i < 4; i++)
		{


        	double xp = cos(alphaInStep * iterat) * xpgrin - sin(alphaInStep * iterat) * ypgrin;
			double yp = sin(alphaInStep * iterat) * xpgrin + cos(alphaInStep * iterat) * ypgrin;
			double zp = zpgrin;

			double xs = Array_Optimal_Galaxy_Full[i][iterat].getValue(0);
			double ys = Array_Optimal_Galaxy_Full[i][iterat].getValue(1);
			double zs = Array_Optimal_Galaxy_Full[i][iterat].getValue(2);

			double r = sqrt( (xp - xs)*(xp - xs) + (yp - ys)*(yp - ys) + (zp - zs)*(zp - zs) )
			+ C * (betta0 + betta1*t);
			Result[iterat * 4 + i][0] = r;
		}
	}
	return Result;
}

//------------------------------------------------------------------

Matrix LSM::Discrepancy(Matrix PsevdoRangeMeas, Matrix PsevdoRange) // расчет невязок
{
	return PsevdoRangeMeas - PsevdoRange;
}

//------------------------------------------------------------------


//------------------------------------------------------------------

Vector LSM::getResult()	// Расчет оценок координат потребителя
{
	double bias_of_estimator = 1010;
	Vector Result(5);

	int k = 1;

	while ( k < 50)
	{
		X[k] = Matrix(5,1);

		DirectionalCosines = Directional_Cosines(X[k-1]);
		DirectionalCosines_transpose = DirectionalCosines.transpose();
		PsevdoRange = Psevdo_Range(X[k-1]);
		Matrix DiscrepancyVector = Discrepancy(PsevdoRangeMeas, PsevdoRange);


		temp = DirectionalCosines_transpose * K_inverse;
		temp1 = temp * DirectionalCosines;
		Matrix temp1_inverse = temp1.inverse();
		Matrix temp2 = temp1_inverse * DirectionalCosines_transpose;
		Matrix temp3 = temp2 * K_inverse;
		Matrix Right = temp3 * DiscrepancyVector;

		RESULT_MATRIX.push_back(temp1_inverse);




		X[k] = X[k-1] + Right;

		bias_of_estimator = sqrt( (X[k][0][0] - X[k-1][0][0]) * (X[k][0][0] - X[k-1][0][0])
								+ (X[k][1][0] - X[k-1][1][0]) * (X[k][1][0] - X[k-1][1][0])
								+ (X[k][2][0] - X[k-1][2][0]) * (X[k][2][0] - X[k-1][2][0]) );





		Result[0] = (X[k-1][0][0]);
		Result[1] = (X[k-1][1][0]);
		Result[2] = (X[k-1][2][0]);
		Result[3] = (X[k-1][3][0]);
		Result[4] = (X[k-1][4][0]);

		RESULT.push_back(Result);

		k++;
	}





    X[k-1].input_file();

	return Result;
}


#endif // LSM_H

