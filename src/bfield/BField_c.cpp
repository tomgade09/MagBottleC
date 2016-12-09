#include "BField_c.h"
#include <iostream>
#include "vectortools.h"

dblArray3_t BField::totalBatP(const dblArray3_t& P, bool WarnFlag)
{
	dblArray3_t b{ 0.0, 0.0, 0.0 };
	
	if (!bObjPtrList_m[0])
	{
		if (WarnFlag) { std::cout << "Warning (totalBatP): No B Objects or charged particles added to this BField instance.  Returning B = {0,0,0}." << std::endl; }
		return nulldA3_t;
	}

	for (unsigned int iii = 0; iii < bObjPtrList_m.size(); iii++)
	{
		dblArray3_t c{nulldA3_t};
		if (!(bObjPtrList_m[iii]))
		{
			if (WarnFlag) { std::cout << "Warning (totalBatP): nullptr in bObjPtrList_m at index " << std::to_string(iii) << ".  Skipping this index." << std::endl; }
			continue;
		}
		c = bObjPtrList_m[iii]->calcBatP(P);
		std::cout << dA3ToStr(c) << std::endl;
		for (int jjj = 0; jjj < 3; jjj++)
			b[jjj] += c[jjj];
	}

	incTime();

	return b;
}

void BField::addBObj(BObject* bobjptr)
{
	if (!bObjPtrList_m[0] && (bObjPtrList_m.size() == 1))
		std::cout << "Warning: Nullptr at bObjPtrList[0].  Popping off." << std::endl;
		bObjPtrList_m.pop_back();
	bObjPtrList_m.push_back(bobjptr);
	std::cout << "Added BObj" << std::endl;
}