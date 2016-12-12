#ifndef VECTORTOOLS_H
#define VECTORTOOLS_H
#include "constsandtypes.h"
#include <string>

dblArray3_t cartesianToSpherical(const dblArray3_t& cart);
dblArray3_t sphericalToCartesian(const dblArray3_t& sphere, int warnFlag=0);
dblArray3_t rotateCartesianVector(const dblArray3_t& cart, double rotTheta, double rotPhi, int warnFlag=0);
dblArray3_t cross3dVectorsAndMultByConst(const dblArray3_t& a, const dblArray3_t& b, double c);
dblArray3_t dA3add(const dblArray3_t& a, const dblArray3_t& b);
dblArray3_t dA3sub(const dblArray3_t& x, const dblArray3_t& y);
double dA3len(const dblArray3_t& a);
std::string dA3ToStr(const dblArray3_t& var);
void dA3coutSci(const dblArray3_t& var);

#endif