#ifndef BFIELD_WIRECOIL_H
#define BFIELD_WIRECOIL_H
#include "vectortools.h"

#define SCALE 10e-5 //replace with global const in some header someday

class WireCoil
{
private:
	std::string name_m;
	int* window_m; //change the type once you know how and have one defined
	int* pic_m; //change the type once you know how and have one defined
	dblArray3_t coilCenter_m; //center point of wire loop
	dblArray3_t coilAxis_m; //normal vector from center of loop perpendicular to loop plane along positive B (use rhr to relate to I flow direction)
	dblArray3_t axisRot_m; //theta, phi to rotate coord system, abbreviated CS: (CS)' - (CS) = axisRot_m
	int N_m; //number of wire wraps
	double I_m; //current flowing through loop
	double R_m; //loop radius
	double d_m; //distance along x axis...define this way?  is needed if each loop defined separately?
	double constant_m; //constant that is easier to calculate once
	bool rightLoop_m; //is this the left or right loop? right loop = 1, left loop = 0 - of course left/right are relative - B flows from left to right loop
	//what other variables needed?
	double c1_m;
	double c2_m;
	double c3_m;
	double a4_m;
	double c4_m;
	double c5_m;
	double c6_m;
	double c7_m;

	double dBx(double x);
	double dBy(double x);
	double dBz(double x);

public:
	WireCoil(const dblArray3_t& coilCenter, const dblArray3_t& coilAxis, int N, double I, double R, bool rightLoop, std::string name = "", int* window = nullptr, int* pic = nullptr) 
		: coilCenter_m(coilCenter), coilAxis_m(coilAxis), N_m{ N }, I_m{ I }, R_m{ R }, rightLoop_m{ rightLoop }, name_m { name	}, window_m{ window }, pic_m{ pic }, 
		  constant_m{ N * I * SCALE }, d_m{}/*Is this variable needed?*/ { Init(); }

	void Init();

	void setIntegConst(const dblArray3_t& P);

	dblArray3_t calcBatP(const dblArray3_t& P);
};

#endif