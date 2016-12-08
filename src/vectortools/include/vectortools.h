#ifndef VECTORTOOLS_H
#define VECTORTOOLS_H

#include <array>
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4 ^^ from <math.h>
using dblArray3_t = std::array<double, 3>;
#define nulldA3_t { 0.0, 0.0, 0.0 }

dblArray3_t cartesianToSpherical(const dblArray3_t& cart);
dblArray3_t sphericalToCartesian(const dblArray3_t& sphere, int warnFlag=0);
dblArray3_t rotateCartesianVector(const dblArray3_t& cart, double rotTheta, double rotPhi, int warnFlag=0);
dblArray3_t cross3dVectorsAndMultByConst(const dblArray3_t& a, const dblArray3_t& b, double c);
dblArray3_t dA3add(const dblArray3_t& a, const dblArray3_t& b);
double dA3len(const dblArray3_t& a);

#endif