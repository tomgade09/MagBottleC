#ifndef TESTSUITE_H
#define TESTSUITE_H
#include "vectortools.h"
#include <string>

bool isFloatEql(const dblArray3_t& x, const dblArray3_t& y);
bool isFloatEql(double x, double y);
int testErrCode(const dblArray3_t& testval, const dblArray3_t& result, const dblArray3_t& hcresult);
int testErrHandler(const dblArray3_t& testval, const dblArray3_t& result, const dblArray3_t& hcresult,
				   std::string name, const dblArray3_t& otherVar = { 0.0, 0.0, 0.0 });
std::string errCodeToStr(int errCode);

#endif