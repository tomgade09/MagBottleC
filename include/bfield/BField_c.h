#ifndef BFIELD_H
#define BFIELD_H
#include <string>
#include <vector>
#include "bfield/BObject_c.h"
#include "tools/constsandtypes.h"

class BField
{
private:
	std::string name_m;
	int* window_m; //change the type once you know how and have one defined - remove visualization stuff
	std::vector<BObject*> bObjPtrList_m; //temporary fix until WireCoil can be abstracted by making it a subclass of BObject, don't forget to change below too (constructor, and addBObj)
	double time_m;
	double dt_m;
	
	void incTime() { time_m += dt_m; }

public:
	BField(double dt, std::vector<BObject*> bObjPtrList = { nullptr }, int* window = nullptr, std::string name = "")
		: bObjPtrList_m{ bObjPtrList }, dt_m{ dt }, window_m { window }, name_m{ name }, time_m{ 0 } {}

	//~BField()-make a destructor to delete pointers in vectors

	//access functions - one liners
	void setActiveWindow(int* windobj) { window_m = windobj; }
	void changeName(std::string name) { name_m = name; }
	std::string getName() { return name_m; }
	double getTime() { return time_m; }
	double getdt() { return dt_m; }
	void forceIncTime() { incTime(); } //want to provide a way for users to increment time by dt if necessary, but totalBatP will do this too

	//defined in .cpp
	void addBObj(BObject* bobjptr); //add pointer to instance of BObject to bObjPtrList_m
	dblArray3_t totalBatP(const dblArray3_t& p, int norder, bool WarnFlag = 0);
};

#endif