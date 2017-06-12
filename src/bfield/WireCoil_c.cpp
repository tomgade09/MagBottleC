#include "bfield/WireCoil_c.h"
#include "integrate/gauss_legendre.h"
#include "tools/vectortools.h"
#include <cmath>

//private functions

void WireCoil::Init()
{
	dblArray3_t a{ { 1.0, M_PI_2, 0.0 } };
	dblArray3_t b(cartesianToSpherical(coilAxis_m));
	axisRot_m[0] = 1.0;

	for (unsigned int iii = 1; iii < 3; iii++)
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
	c1_m = { -constant_m * R_m * P[2] };
	c2_m = { -constant_m * R_m * P[1] };
	c3_m = { constant_m * pow(R_m,2) };
	a4_m = { pow(P[0],2) + pow(P[1],2) + pow(P[2],2) };
	c4_m = { a4_m + 2 * P[0] * d_m + pow(d_m, 2) + pow(R_m, 2) };
	c5_m = { -2 * P[1] * R_m };
	c6_m = { -2 * P[2] * R_m };
	c7_m = { constant_m * R_m * (P[0] + d_m) };
}

//public functions

double WireCoil::dBx(double x, void* data, double var[6]) //for gauss_legendre
{
	//return ((c1_m * sin(x) + c2_m * cos(x) + c3_m) / pow((c4_m + c5_m * cos(x) + c6_m * sin(x)), 1.5));
	return ((var[0] * sin(x) + var[1] * cos(x) + var[2]) / pow((var[3] + var[4] * cos(x) + var[5] * sin(x)), 1.5));
}

double WireCoil::dBy(double x, void* data, double var[6]) //for gauss_legendre
{
	//return (c7_m * cos(x) / pow((c4_m + c5_m * cos(x) + c6_m * sin(x)), 1.5));
	return (var[0] * cos(x) / pow((var[1] + var[2] * cos(x) + var[3] * sin(x)), 1.5));
}

double WireCoil::dBz(double x, void* data, double var[6]) //for gauss_legendre
{
	//return (c7_m * sin(x) / pow((c4_m + c5_m * cos(x) + c6_m * sin(x)), 1.5));
	return (var[0] * sin(x) / pow((var[1] + var[2] * cos(x) + var[3] * sin(x)), 1.5));
}

dblArray3_t WireCoil::calcBatP(const dblArray3_t& P, int norder)
{
	setIntegConst(P);

	dblArray3_t B{ 0.0, 0.0, 0.0 };

	typedef double(*SFP)(double, void*, double[]); //for gauss_legendre
	SFP FP[3] = { dBx, dBy, dBz };
	double dBxCst[6]{ c1_m, c2_m, c3_m, c4_m, c5_m, c6_m };
	double dByCst[6]{ c7_m, c4_m, c5_m, c6_m, 0.0, 0.0 };
	double *dBCst[] { dBxCst, dByCst, dByCst };
	
	for (unsigned int jjj = 0; jjj < 3; jjj++)
		B[jjj] = gauss_legendre(norder, FP[jjj], NULL, 0, 2 * M_PI, dBCst[jjj]); //for gauss_legendre, n=5 to match default SciPy quadpack value
	//32-37 yields some pretty close values...around this range, the numbers don't change up to 14 digits and match python almost perfectly
	//around the 40s the numbers start fluctuating again

	return B;
}

/* Python code below
def calcGuidingCenter(self, PartObj, dt) :
# The method :
# 1. Calculate B at the position of the particle
# 2. Calculate q * (v x B) (Lorentz F), perpendicular velocity, and Larmour radius
#       These will be used to calculate a Guiding Center location, They won't be exact but should be really close
#       FLorentz / (length of F lorentz) - gives a unit vector in the direction of the Lorentz force - toward the GC
#       Multiply this vector by Larmour radius and add to the particle position to get the approx GC
# 3. (optional)Calculate B at guiding center, if desired, or use GC for other purposes
#       - Use this Bgc to calculate the mirror force on the particle for increased accuracy

# Note: subscript "p" denotes quantities calculated at a point P / with the B calculated at point P, while "gc" is used to
# denote the same quantities at the guiding center.
# This method is used to overcome a difficulty related to the fact that we need B at the guiding center in order to calculate
# vperp(which is needed for Larmour radius rL).We can't get rL without B at GC and we can't know where GC is without rL.
# This method uses vperp from B at the particle's position (instead of B at GC), but it should be close enough to get the approx. location of GC.
# At any rate, it will be closer than calculating from the particle's position P.

	Bp = np.array(self.calcBatP(PartObj.p))
	Bplen = np.sqrt(Bp[0] * *2 + Bp[1] * *2 + Bp[2] * *2)

	FLorentz = self.foRKLorentz(PartObj, dt)
	FLlen = np.sqrt(FLorentz[0] * *2 + FLorentz[1] * *2 + FLorentz[2] * *2)

	vp_perp = np.sqrt((PartObj.v[0] * *2 + PartObj.v[1] * *2 + PartObj.v[2] * *2) - (np.dot(Bp, PartObj.v) / Bplen)**2)
	rLp = PartObj.mass * vp_perp / (np.abs(PartObj.q) * Bplen)

	return np.array(PartObj.p + (rLp / FLlen) * FLorentz)

////////////////////////def drawBfromGC(self, PartObj, FieldObj, numiter = 5500, multlng = 500) :
							j = rotateVector(self.calcGuidingCenter(PartObj, FieldObj.dt), self.axiscf_theta, self.axiscf_phi) + self.Cpair
							FieldObj.drawBlines(j, numiter = numiter, multlng = multlng)

////////////////////////def calcDriftsF(self, PartObj) :
							return self.calcMirrorF(PartObj)

DONE!!  def calcMirrorF(self, PartObj) :
			Bp = np.array(self.calcBatP(PartObj.p))
			Bplen = np.sqrt(Bp[0] * *2 + Bp[1] * *2 + Bp[2] * *2)
			BpdotV = np.dot(Bp, PartObj.v)

			scaledLength = 1e-8 #total length of vector in the direction of B that is used to calculate B(p + ds) and B(p - ds)
			halfds = (scaledLength / Bplen) * Bp
			
			Bpminus = np.array(self.calcBatP(PartObj.p - halfds)) #using scaled Bp vector as an element, ds, along which B is calculated - dB / ds = (B(p + ds) - B(p - ds)) / ds
			Bpplus = np.array(self.calcBatP(PartObj.p + halfds))

			vperp2 = (PartObj.v[0] * *2 + PartObj.v[1] * *2 + PartObj.v[2] * *2) - (BpdotV / Bplen)**2 #length of vperp squared
			mu = PartObj.mass * vperp2 / (2 * Bplen) # should be invariant - so do I need to calculate every time ? Check for invariance
			FgradB = -mu * (Bpplus - Bpminus)

			for i in range(3) :
				if abs(Bp[i]) < 1e-20 : #Not sure, think it should work for the examples, but probably not general enough - maybe a ratio to other elements ?
					FgradB[i] = 0. #More for the first calculation than anything.Only used once->when exactly on axis
					#print("zeroed a FgradB element ", i)
					continue
				FgradB[i] = FgradB[i] / (2 * scaledLength)

			return FgradB[0], FgradB[1], FgradB[2]

DONE!!  def foRKLorentz(self, PartObj, h) :
			B1 = self.calcBatP(PartObj.p[:])
			k11 = PartObj.eom * np.cross(PartObj.v, B1) #accleration

			B2 = self.calcBatP(PartObj.p + k11 * h * h / 4)
			k22 = PartObj.eom * np.cross(PartObj.v + k11 * h / 2, B2)

			B3 = self.calcBatP(PartObj.p + k22 * h * h / 4)
			k33 = PartObj.eom * np.cross(PartObj.v + k22 * h / 2, B3)

			B4 = self.calcBatP(PartObj.p + k33 * h * h)
			k44 = PartObj.eom * np.cross(PartObj.v + k33 * h, B4)

			return (PartObj.mass * (k11 + 2 * (k22 + k33) + k44) / 6) #mass * accel = Force

DONE!!  def calcTotalF(self, PartObj, dt) :
			Fl = np.array(self.foRKLorentz(PartObj, dt))
			Fd = np.array(self.calcDriftsF(PartObj))
			return (Fl + Fd)
*/