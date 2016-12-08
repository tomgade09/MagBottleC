#ifndef BFIELD_H
#define BFIELD_H
#include <string>
#include <vector>
#include "WireCoil.h"//#include "BObject_c.h"
#include "Particle_c.h"
#include "vectortools.h"

class BField
{
private:
	std::string name_m;
	int* window_m; //change the type once you know how and have one defined
	std::vector</*BObject*/WireCoil*> bObjPtrList_m; //temporary fix until WireCoil can be abstracted by making it a subclass of BObject, don't forget to change below too (constructor, and addBObj)
	std::vector<Particle*> partPtrList_m;
	double time_m;
	double dt_m;
	
	void incTime() { time_m += dt_m; }

public:
	BField(std::vector</*BObject**/WireCoil*> bObjPtrList = {}, std::vector<Particle*> partPtrList = {}, int* window = nullptr, std::string name = "")
		: bObjPtrList_m{ bObjPtrList }, partPtrList_m{ partPtrList }, window_m{ window }, name_m{ name }, time_m{0} {}

	//access functions - one liners
	void setActiveWindow(int* windobj) { window_m = windobj; }
	void changeName(std::string name) { name_m = name; }
	std::string getName() { return name_m; }
	double getTime() { return time_m; }
	double getdt() { return dt_m; }
	void forceIncTime() { incTime(); } //want to provide a way for users to increment time by dt if necessary, but totalBatP will do this too

	//defined in .cpp
	void addBObj(/*BObject**/WireCoil* bobjptr); //add pointer to instance of BObject to bObjPtrList_m
	void addPart(Particle* partptr);//add pointer to instance of Particle to partPtrList_m
	dblArray3_t totalBatP(const dblArray3_t& p, bool WarnFlag = 0);
};

#endif