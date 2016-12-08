#include <iostream>
#include <cmath>
#include <string>
#include "vectortools.h"
#include "testsuitetools.h"

int cToSTest()
{
	double xyzvalues[]{ 0.0, 5.6286, -5.6286 }; //arbitrary pos and neg numbers, symmetry makes testing easier
	int hcrhoind[]    { 0, 1, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 3, 3, 2, 3, 3, 1, 2, 2, 2, 3, 3, 2, 3, 3 }; //index values for rho, theta, phi hardcoded arrays
	int hcthetaind[]  { 0, 0, 4, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 5, 6, 2, 5, 6, 2, 1, 3, 2, 5, 6, 2, 5, 6 }; //generates a result appropriate to testval
	int hcphiind[]    { 0, 0, 0, 2, 2, 2, 6, 6, 6, 0, 0, 0, 1, 1, 1, 7, 7, 7, 4, 4, 4, 3, 3, 3, 5, 5, 5 }; //elminates a 27 case switch (yuck)

	double hcrho[]	  { 0.0, sqrt(pow(5.6286, 2)), sqrt(2 * pow(5.6286,2)), sqrt(3 * pow(5.6286, 2)) }; //hard coded values
	double hctheta[]  { 0.0, (M_PI_2 / 2.0), M_PI_2, (3 * M_PI_2 / 2.0), M_PI, acos(hcrho[1] / hcrho[3]), acos(-hcrho[1] / hcrho[3]) };
	double hcphi[]    { 0.0, (M_PI_2 / 2.0), M_PI_2, (3 * M_PI_2 / 2.0), M_PI, (5 * M_PI_2 / 2.0), (3 * M_PI_2), (7 * M_PI_2 / 2.0) };
	
	int indind{ 0 }; //index index - the index of the total number of iterations in the below 3 deep nested loop structure

	for (int iii = 0; iii < 3; iii++)
	{
		for (int jjj = 0; jjj < 3; jjj++)
		{
			for (int kkk = 0; kkk < 3; kkk++)
			{
				dblArray3_t testval{ xyzvalues[iii], xyzvalues[jjj], xyzvalues[kkk] };
				dblArray3_t result = cartesianToSpherical(testval);
				dblArray3_t hcresult{ hcrho[hcrhoind[indind]], hctheta[hcthetaind[indind]], hcphi[hcphiind[indind]] };
				
				if (!isFloatEql(result, hcresult))
					return testErrHandler(testval, result, hcresult, "cartesianToSpherical", { static_cast<double>(indind), 0.0, 0.0 });
				indind++;
			}
		}
	}

	return 0; //no errors
}

int sToCTest()
{
	double hcxyzvalues[]{ 0.0, 5.6286, -5.6286 }; //arbitrary pos and neg numbers, symmetry makes testing easier
	int hcrhoind[]		{ 0, 1, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 3, 3, 2, 3, 3, 1, 2, 2, 2, 3, 3, 2, 3, 3 }; //index values for rho, theta, phi arrays
	int hcthetaind[]    { 0, 0, 4, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 5, 6, 2, 5, 6, 2, 1, 3, 2, 5, 6, 2, 5, 6 }; //generates a testval that corresponds with result
	int hcphiind[]      { 0, 0, 0, 2, 2, 2, 6, 6, 6, 0, 0, 0, 1, 1, 1, 7, 7, 7, 4, 4, 4, 3, 3, 3, 5, 5, 5 }; //elminates a 27 case switch (yuck)

	double rho[]		{ 0.0, sqrt(pow(5.6286, 2)), sqrt(2 * pow(5.6286,2)), sqrt(3 * pow(5.6286, 2)) };
	double theta[]      { 0.0, (M_PI_2 / 2.0), M_PI_2, (3 * M_PI_2 / 2.0), M_PI, acos(rho[1] / rho[3]), acos(-rho[1] / rho[3]) };
	double phi[]		{ 0.0, (M_PI_2 / 2.0), M_PI_2, (3 * M_PI_2 / 2.0), M_PI, (5 * M_PI_2 / 2.0), (3 * M_PI_2), (7 * M_PI_2 / 2.0) };
	
	int indind{ 0 }; //index index - the index of the total number of iterations in the below 3 deep nested loop structure

	for (int iii = 0; iii < 3; iii++)
	{
		for (int jjj = 0; jjj < 3; jjj++)
		{
			for (int kkk = 0; kkk < 3; kkk++)
			{
				dblArray3_t testval{ rho[hcrhoind[indind]], theta[hcthetaind[indind]], phi[hcphiind[indind]] };
				dblArray3_t result = sphericalToCartesian(testval);
				dblArray3_t hcresult{ hcxyzvalues[iii], hcxyzvalues[jjj], hcxyzvalues[kkk] };
				
				if (!isFloatEql(result, hcresult))
					return testErrHandler(testval, result, hcresult, "sphericalToCartesian", { static_cast<double>(indind), 0.0, 0.0 });
				indind++;
			}
		}
	}

	return 0; //no errors
}

int rotCVectorTest()
{
	
	dblArray3_t testval{ 1, 1, 1 };
	double hcxsign[]   { -1.0, -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,  1.0/*,        cos(M_PI_4), -1.0, -1.0,  1.0,    1.0, -1.0, -1.0,  1.0*/ }; //hard coded vector values
	double hcysign[]   {  1.0, -1.0, -1.0,  1.0,    1.0, -1.0, -1.0,  1.0/*,        cos(M_PI_4), -1.0, -1.0,  1.0,   -1.0,  1.0,  1.0, -1.0*/ };
	double hczsign[]   {  1.0,  1.0,  1.0,  1.0,   -1.0, -1.0, -1.0, -1.0/*,   -2 * cos(M_PI_4), -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,  1.0*/ };
	int hcind[]     { 0, 1, 2, 3, 2, 1, 0, 3,    4, 5, 6, 7, 6, 5, 4, 7/*,    8, 9, 10, 11, 10, 9, 8, 11,    12, 13, 14, 15, 14, 13, 12, 15*/ }; //indexes to traverse sign values above
	
	double rotPhi[]    { M_PI_2, -M_PI_2, 0.0, -M_PI_2 };
	double rotTheta[]  { M_PI_2, -M_PI_2, 0.0,  M_PI_2 };

	dblArray3_t result = rotateCartesianVector(testval, 0.0, 0.0, 1); //no rotation case
	if (result != testval)
		return testErrHandler(testval, result, testval, "rotateCartesianVector");

	int indind{ 0 };
	dblArray3_t hcresult{ 0.0, 0.0, 0.0 };

	for (int iii = 2; iii < 4; iii++) //rotates theta
	{
		for (int jjj = 0; jjj < 2; jjj++)
		{
			for (int kkk = 0; kkk < 4; kkk++)
			{
				result = rotateCartesianVector(testval, 0.0, rotPhi[jjj]);
				hcresult = { hcxsign[hcind[indind]], hcysign[hcind[indind]], hczsign[hcind[indind]] };

				if (!isFloatEql(result, hcresult))
					return testErrHandler(testval, result, hcresult, "rotateCartesianVector", { rotTheta[jjj], 0.0, static_cast<double>(indind) });

				testval = result;
				indind++;
			}
			if (jjj == 1) { testval = { 1.0, 1.0, -1.0 }; }
		}
		if (iii == 3) { testval = { 1.0, 1.0, 1.0 }; }
	}

	testval = { 1.0, 1.0, 1.0 };

/*	for (int iii = 2; iii < 4; iii++) //rotates phi, not working as expected due to sign issues and the like
	{
		//testval = rotateCartesianVector(testval, rotTheta[iii], 0.0);
		for (int jjj = 0; jjj < 2; jjj++)
		{
			for (int kkk = 0; kkk < 4; kkk++)
			{
				result = rotateCartesianVector(testval, 0.0, rotPhi[jjj], 1);
				hcresult = { hcxsign[hcind[indind]], hcysign[hcind[indind]], hczsign[hcind[indind]] };

				if (!isFloatEql(result, hcresult))
					return testErrHandler(testval, result, hcresult, "rotateCartesianVector", { 0.0, rotPhi[jjj], static_cast<double>(indind) });

				testval = result;
				indind++;
			}
			if (jjj == 1) { testval = { 1.0, -1.0, 1.0 }; }
		}
	}
	*/

	return 0; //no errors
}

int crossVectorTest()
{	
	double xval[]{ 4.0,  1.0,  1.0,    4.0,  0.0,  4.0 }; //hard coded vectors: a a a a b b b b
	double yval[]{ 5.0, -1.0,  1.0,    5.0,  0.0,  4.0 }; //1. a x a, 2. a x 0
	double zval[]{ 6.0,  1.0,  1.0,    6.0,  0.0, -4.0 }; //3. a x b (perpen.) * 0
	double cnst[]{ 1.0,  1.0,  0.0 };
	int len      { 3 }; // match to length of cnst or x|y|zval / 2

	double hc[]  { 0.0,  0.0,  0.0 };

	dblArray3_t a;
	dblArray3_t b;
	double c{ 0.0 };

	dblArray3_t result;
	dblArray3_t hcresult;

	for (int iii = 0; iii < len; iii++)
	{
		a = { xval[iii], yval[iii], zval[iii] };
		b = { xval[iii + len], yval[iii + len], zval[iii + len] };
		c = cnst[iii];

		result = cross3dVectorsAndMultByConst(a, b, c);
		hcresult = { hc[iii], hc[iii], hc[iii] };

		if (!isFloatEql(result, hcresult))
			return testErrHandler(a, result, hcresult, std::to_string(iii) + "  cross3dVectorsAndMultiply", b );
	}

	return 0; //no errors
}

int vtTestMain()
{
	//count and name tests here
	const int tests(4);
	int errCode[tests];
	std::string resultStrings[tests];
	std::string testNameStr[tests] = { "Cartesian to Spherical Test:            ", 
									   "Spherical to Cartesian Test:            ", 
									   "Rotate Cartesian Vector Test:           ",
									   "Cross Product and Scalar Multiply Test: " };
	
	//call tests here, assign results to testResults
	errCode[0] = cToSTest();
	errCode[1] = sToCTest();
	errCode[2] = rotCVectorTest();
	errCode[3] = crossVectorTest();

	//prints results to cout
	for (int ii = 0; ii < tests; ii++)
	{
		resultStrings[ii] = errCodeToStr(errCode[ii]);
		std::cout << testNameStr[ii] << resultStrings[ii] << std::endl;
	}

	return 0;
}