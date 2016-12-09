#include "WireCoil_c.h"
#include <cmath>
#include "gauss_legendre.h"
#include "vectortools.h"
#include <iostream>

static double dBx_wrap(double x, void* data, WireCoil* wc);
static double dBy_wrap(double x, void* data, WireCoil* wc);
static double dBz_wrap(double x, void* data, WireCoil* wc);

//private functions

void WireCoil::Init()
{
	dblArray3_t a{ {1.0, M_PI_2, 0.0 } };
	dblArray3_t b(cartesianToSpherical(coilAxis_m));
	axisRot_m[0] = 1.0;

	for (int iii = 1; iii < 3; iii++)
		axisRot_m[iii] = (a[iii] - b[iii]);
	//somehow account for loop pair centers not being at {0,0,0}
	//not sure if I should allow a dblArray3_t to be passed, and let the user worry about figuring out the right value
	//or perhaps passing in a pointer to the paired loop and a T/F to allow the user to pair or not with another loop
	//but for now, just going to leave it and ensure symmetry in the examples
	d_m = dA3len(coilCenter_m); //coilCenter represents a point at the center of the loop, but here, is also the vector from origin to the center (cx-0, cy-0, cz-0)
	if (!rightLoop_m)			//need to be careful here...BatP point P simply adds/subtracts d to Px...need to translate, then rotate before calcing B
		d_m *= -1;
}

void WireCoil::setIntegConst(const dblArray3_t& P)
{
	double c1_m = { -constant_m * R_m * P[2] };
	double c2_m = { -constant_m * R_m * P[1] };
	double c3_m = { constant_m * pow(R_m,2) };
	double a4_m = { pow(P[0],2) + pow(P[1],2) + pow(P[2],2) };
	double c4_m = { a4_m + 2 * P[0] * d_m + pow(d_m, 2) + pow(R_m, 2) };
	double c5_m = { -2 * P[1] * R_m };
	double c6_m = { -2 * P[2] * R_m };
	double c7_m = { constant_m * R_m * (P[0] + d_m) };
}

/*double WireCoil::gauss_legendre_m(int n, double(*f)(double, void*), void* data, double a, double b)
{
	return gauss_legendre(n, f, data, 0, 2 * M_PI);
}*/

//public functions

double WireCoil::dBx(double x, void* data)
{
	return ((c1_m * sin(x) + c2_m * cos(x) + c3_m) / pow((c4_m + c5_m * cos(x) + c6_m * sin(x)), 1.5));
}

double WireCoil::dBy(double x, void* data)
{
	return (c7_m * cos(x) / pow((c4_m + c5_m * cos(x) + c6_m * sin(x)), 1.5));
}

double WireCoil::dBz(double x, void* data)
{
	return (c7_m * sin(x) / pow((c4_m + c5_m * cos(x) + c6_m * sin(x)), 1.5));
}

dblArray3_t WireCoil::calcBatP(const dblArray3_t& P)
{
	std::cout << "WireCoil::calcBatP: " << name_m << std::endl;
	setIntegConst(P);

	dblArray3_t B{ 0.0, 0.0, 0.0 };

	typedef double(*FP)(double, void*, WireCoil*);
	FP FP1[3]{ &dBx_wrap, &dBy_wrap, &dBz_wrap };

	for (int jjj = 0; jjj < 3; jjj++)
		for (int iii = 2; iii <= 128; iii++)
			B[0] = gauss_legendre(iii, FP1[jjj], NULL, 0, 2 * M_PI, this);

	return B;
}

static double dBx_wrap(double x, void* data, WireCoil* wc)
{
	return wc->dBx(x, data);
}

static double dBy_wrap(double x, void* data, WireCoil* wc)
{
	return wc->dBy(x, data);
}

static double dBz_wrap(double x, void* data, WireCoil* wc)
{
	return wc->dBy(x, data);
}