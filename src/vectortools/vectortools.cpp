#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include "vectortools.h"

//Physics convention for spherical used: phi = angle from z axis, theta = angle from x axis
//All angles in radians unless otherwise noted
//From here out, the word "vector" refers to a 1 x 3 array (represented by std::array<double, 3>), NOT std::vector

dblArray3_t cartesianToSpherical(dblArray3_t& cart)
{ //convert a cartesian array {x, y, z} to a spherical array {rho, theta, phi}
	if ((cart[0] == 0.0) && (cart[1] == 0.0) && (cart[2] > 0.0)) //pos z-axis vector case
		return{ cart[2], 0.0, 0.0 };
	else if ((cart[0] == 0.0) && (cart[1] == 0.0) && (cart[2] < 0.0)) //neg z-axis vector case
		return{ -cart[2], 0.0, M_PI };
	else if ((cart[0] == 0.0) && (cart[1] == 0.0) && (cart[2] == 0.0)) //zero vector case
		return{ 0.0, 0.0, 0.0 };

	double rho = sqrt(pow(cart[0], 2.0) + pow(cart[1], 2.0) + pow(cart[2], 2.0));
	double theta = atan2(cart[1], cart[0]);
	double phi = acos((cart[2] / rho));

	if (theta < 0.0)
		theta += (2 * M_PI);

	dblArray3_t retarray{ rho, theta, phi };

	return retarray;
}

dblArray3_t sphericalToCartesian(dblArray3_t& sphere, int warnFlag) //warnFlag default = 0 (in fwd declaration)
{ //convert a spherical vector {rho, theta, phi} to a cartesian vector {x, y, z}
	if (sphere[1] == 0.0 && sphere[2] == 0.0) //z-axis vector case
		return{ 0.0, 0.0, sphere[0] };
	else if (sphere[1] == 0.0 && (abs(1 - (sphere[2] / M_PI)) < 1e-14))
		return{ 0.0, 0.0, -sphere[0] };
	else if (sphere[0] < 0.0)
	{
		if (warnFlag == 1) { std::cout << "WARNING: Negative rho passed to sphericalToCartesian.  You can't do that!  Returning 0 vector." << std::endl; }
		return{ 0.0, 0.0, 0.0 }; //could also do a Pi/2 +/- Phi (depending on quadrant) and Pi + Theta...but shouldn't be passing in a negative rho
	}

	double x = (sphere[0] * cos(sphere[1]) * sin(sphere[2]));
	double y = (sphere[0] * sin(sphere[1]) * sin(sphere[2]));
	double z = (sphere[0] * cos(sphere[2]));

	dblArray3_t retarray{ x, y, z };

	return retarray;
}

dblArray3_t rotateCartesianVector(dblArray3_t& cart, double rotTheta, double rotPhi, int warnFlag) //warnFlag default = 0 (in fwd declaration)
{ //rotate a cartesian vector cart by Theta and Phi in spherical, return 3d cartesian vector
	/*if (abs(M_PI_2 + rotTheta) < 1e-14)			//difference will likely not be exactly zero, so accounting for floating point error
		return{ -cart[2], cart[1], cart[0] };   //rotTheta ~= -PI/2
	else */if ((rotTheta == 0.0) && (rotPhi == 0.0)) //no rotation
		return cart;

	dblArray3_t sphere = cartesianToSpherical(cart);
	sphere[1] += rotTheta;
	sphere[2] += rotPhi;

	if (sphere[2] < 0.0) //negative phi value - flip to positive, rotate phi 180 degrees
	{
		if (warnFlag == 1) { std::cout << "Warning: Negative phi.  Converting to equivalent vector with phi within 0 <= theta < 180." << std::endl; }
		sphere[1] += M_PI;
		sphere[2] = -sphere[2];
	}
	else if (sphere[2] > M_PI) //phi greater than 180 - subtract from 360 to get offset from 180 degrees, rotate phi 180 degrees
	{
		if (warnFlag == 1) { std::cout << "Warning: phi > 180.  Converting to equivalent vector with phi within 0 <= theta < 180." << std::endl; }
		sphere[1] = (2 * M_PI - sphere[1]);
		sphere[2] += M_PI;
	}

	if (sphere[1] < 0.0) //correct theta less than 0, greater than 360 - add 2PI, subtract 2PI respectively
	{
		if (warnFlag == 1) { std::cout << "Warning: Negative theta.  Converting to equivalent vector with theta within 0 <= theta < 360." << std::endl; }
		sphere[1] += (2 * M_PI);
	}
	else if (sphere[1] > (2 * M_PI))
	{
		if (warnFlag == 1) { std::cout << "Warning: Theta > 360.  Converting to equivalent vector with theta within 0 <= theta < 360." << std::endl; }
		sphere[1] -= (2 * M_PI);
	}

	return sphericalToCartesian(sphere);
} 

dblArray3_t cross3dVectorsAndMultByConst(dblArray3_t& a, dblArray3_t& b, double c)
{ //return 3D vector v = a(3D vector) x b (3D vector) * c (double)
	return{ (a[1] * b[2] - a[2] * b[1]) * c, (a[2] * b[0] - a[0] * b[2]) * c, (a[0] * b[1] - a[1] * b[0]) * c };
}