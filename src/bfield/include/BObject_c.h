#ifndef BFIELD_BOBJECT_H
#define BFIELD_BOBJECT_H
#include "constsandtypes.h"

class BObject
{
public:
	virtual ~BObject() {}
	virtual dblArray3_t calcBatP(const dblArray3_t& P) = 0;
};

#endif