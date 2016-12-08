#include "BField_c.h"
#include <iostream>

dblArray3_t BField::totalBatP(const dblArray3_t& p, bool WarnFlag)
{
	dblArray3_t b{ 0.0, 0.0, 0.0 };
	
	if (!bObjPtrList_m[0] && !partPtrList_m[0])
	{
		if (WarnFlag) { std::cout << "Warning (totalBatP): No B Objects or charged particles added to this BField instance.  Returning B = {0,0,0}." << std::endl; }
		return nulldA3_t;
	}

	for (int iii = 0; iii < bObjPtrList_m.size(); iii++)
	{
		dblArray3_t c;
		if (!(bObjPtrList_m[iii]))
		{
			if (WarnFlag) { std::cout << "Warning (totalBatP): nullptr in bObjPtrList_m at index " << std::to_string(iii) << ".  Skipping this index." << std::endl; }
			continue;
		}
		c = bObjPtrList_m[iii]->calcBatP(p);
		for (int jjj = 0; jjj < 3; jjj++)
			b[jjj] += c[jjj];
	}

	for (int iii = 0; iii < partPtrList_m.size(); iii++) //somehow need a superset iterator for { bObjPtrList_m, partPtrList_m } - to eliminate duplicated code
	{
		dblArray3_t c;
		if (!(partPtrList_m[iii]))
		{
			if (WarnFlag) { std::cout << "Warning (totalBatP): nullptr in partPtrList_m at index " << std::to_string(iii) << ".  Skipping this index." << std::endl; }
			continue;
		}
		c = partPtrList_m[iii]->calcBatP(p);
		for (int jjj = 0; jjj < 3; jjj++)
			b[jjj] += c[jjj];
	}

	incTime();

	return b;
}

void BField::addBObj(/*BObject**/WireCoil* bobjptr) //Need to make sure if nullptr is initialized to these vectors, that it is removed once the first element is added
{
	if (!bObjPtrList_m[0] && (bObjPtrList_m.size() == 1))
		bObjPtrList_m.pop_back();
	bObjPtrList_m.push_back(bobjptr);
}

void BField::addPart(Particle* partptr)
{
	if (!partPtrList_m[0] && (partPtrList_m.size() == 1))
		partPtrList_m.pop_back();
	partPtrList_m.push_back(partptr);
}

//void BField::add(const BObject* bobjptr = nullptr, const Particle* partptr = nullptr) Maybe a way to make one single add function?