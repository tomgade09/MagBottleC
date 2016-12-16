#include <iostream>
#include <cmath>
#include "testsuite/testsuitetools.h"

bool isFloatEql(const dblArray3_t& x, const dblArray3_t& y)
{
	bool fpEql = 1;
	dblArray3_t diff( x - y );

	for (unsigned int ii = 0; ii < 3; ii++)
	{
		if (y[ii] == 0.0)
		{
			fpEql &= (abs(diff[ii]) < 1e-14);
			continue;
		}

		fpEql &= ((abs(diff[ii]) / y[ii]) < 1e-14); //need to scale error with magnitude of input array elements, assumes arrays approx equal in scale
	}

	return fpEql;
}

bool isFloatEql(double x, double y)
{
	double diff = x - y;
	
	if (x == 0.0)
		return (diff < 1e-14);
	
	return ((abs(diff) / x) < 1e-14);
}

int testErrCode(const dblArray3_t& testval, const dblArray3_t& result, const dblArray3_t& hcresult)
{ //assigns error code based on test value and result value vs hard coded result
	bool largeDiff = 0;
	for (unsigned int iii = 0; iii < 3; iii++)
	{
		if (std::isnan(result[iii]) || std::isinf(result[iii]))
			return -2;
		if (hcresult[iii] == 0.0)
		{ //when result value = 0, can't put it in the denominator - check for large error by just comparing result with threshold
			largeDiff |= (result[iii] > 1e-14);
			continue;
		}
		largeDiff |= (abs(1 - (result[iii] / hcresult[iii])) > 1e-14);
	}
	
	if (largeDiff)
		return -1;

	return -3;
}

int testErrHandler(const dblArray3_t& testval, const dblArray3_t& result, const dblArray3_t& hcresult, std::string name, const dblArray3_t& otherVar)
{ //handles errors: finds error code using testErrCode and prints relevant info about the error to cout
	std::cout << "Func causing error: " << name << std::endl;
	std::cout << "Test Value:       " << testval << std::endl;
	std::cout << "Other vars:       " << otherVar << std::endl;
	std::cout << "Result:           " << result << std::endl;
	std::cout << "Hard Coded Value: " << hcresult << std::endl;
	dblArray3_t errordblA3 = (result - hcresult);
	for (unsigned int lll = 0; lll < 3; lll++)
	{
		errordblA3[lll] *= 1e14;
	}
	if ((errordblA3[0] < 100.0) && (errordblA3[1] < 100.0) && (errordblA3[2] < 100.0)) //why is this not working right?
		std::cout << "Error * 1e14:     " << errordblA3 << std::endl; //will include this line no matter what
	int ret = testErrCode(testval, result, hcresult);

	return ret;
}

std::string errCodeToStr(int errCode)
{
	switch (errCode)
	{
	case 0:
		return "Pass";
	case -1:
		return "Fail: Large difference from hard-coded result values";
	case -2:
		return "Fail: NaN or Inf result";
	default:
		return "Fail: Unknown Error";
	}
}
