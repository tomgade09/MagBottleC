#include "bfield/BField_c.h"
#include "tools/vectortools.h"
#include "particle/Particle_c.h"
#include <iostream>

dblArray3_t BField::totalBatP(const dblArray3_t& P, int norder, bool WarnFlag)
{
	dblArray3_t B{ 0.0, 0.0, 0.0 };
	
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
		c = bObjPtrList_m[iii]->calcBatP(P,norder);

		B += c;
	}

	incTime();

	return B;
}

void BField::addBObj(BObject* bobjptr)
{
	if (!bObjPtrList_m[0] && (bObjPtrList_m.size() == 1))
	{
		std::cout << "Warning: Nullptr at bObjPtrList[0].  Popping off." << std::endl;
		bObjPtrList_m.pop_back();
	}
	bObjPtrList_m.push_back(bobjptr);
	std::cout << "Added BObj.  Size: " << std::to_string(bObjPtrList_m.size()) << std::endl;
}

/*dblArray3_t BField::totalFonParticle(Particle* partobj, int norder)
{//obsolete - eventually remove
	//dblArray3_t F{ 0.0, 0.0, 0.0 };

	for (unsigned int iii = 0; iii < bObjPtrList_m.size(); iii++)
	{
		dblArray3_t c{ nulldA3_t };
		if (!(bObjPtrList_m[iii]))
		{
			//if (WarnFlag) { std::cout << "Warning (totalBatP): nullptr in bObjPtrList_m at index " << std::to_string(iii) << ".  Skipping this index." << std::endl; }
			continue;
		}

		c = bObjPtrList_m[iii]->totalFatP(partobj, dt_m, norder);

		F += c;
	}

	incTime();

	return totalFatP(partobj, dt_m, norder);
}*/

dblArray3_t BField::totalFatP(Particle* partobj, double dt, int norder)
{
	dblArray3_t fLorentz, fMirror;
	fLorentz = foRKLorentz(partobj, dt, norder);
	fMirror = calcMirrorF(partobj, norder);
	//std::cout << "fLorentz (old): " << fLorentz << "\nfMirror: " << fMirror << "\n";
	return  fLorentz + fMirror;
}

void BField::updateParticleP_V(Particle* partobj, int norder)
{
	partobj->updP(totalFatP(partobj, dt_m, norder), dt_m, true);
	incTime();//an issue if more than one particle in sim - change later
}

dblArray3_t BField::foRKLorentz(Particle* partobj, double dt, int norder)
{
	dblArray3_t B1 = totalBatP(partobj->getP(), norder);
	dblArray3_t a11 = cross3dVectorsAndMultByConst(partobj->getV(), B1, partobj->getq_m());//acceleration

	dblArray3_t B2 = totalBatP(partobj->getP() + a11 * dt * dt / 4, norder);
	dblArray3_t a22 = cross3dVectorsAndMultByConst(partobj->getV() + a11 * dt / 2, B2, partobj->getq_m());

	dblArray3_t B3 = totalBatP(partobj->getP() + a22 * dt * dt / 4, norder);
	dblArray3_t a33 = cross3dVectorsAndMultByConst(partobj->getV() + a22 * dt / 2, B3, partobj->getq_m());

	dblArray3_t B4 = totalBatP(partobj->getP() + a33 * dt * dt, norder);
	dblArray3_t a44 = cross3dVectorsAndMultByConst(partobj->getV() + a33 * dt, B4, partobj->getq_m());

	return ((a11 + (a22 + a33) * 2. + a44) / 6.) * partobj->getM();
}

dblArray3_t BField::calcMirrorF(Particle* partobj, int norder)
{
	dblArray3_t Bp = totalBatP(partobj->getP(), norder);
	double Bplen = dA3len(Bp);
	dblArray3_t v = partobj->getV();
	double BpdotV = Bp[0] * v[0] + Bp[1] * v[1] + Bp[2] * v[2];

	double scaledLength = 1e-8; //total length of vector in the direction of B that is used to calculate B(p + ds) and B(p - ds)
	dblArray3_t halfds = Bp * (scaledLength / Bplen);

	dblArray3_t Bpminus = totalBatP(partobj->getP() - halfds, norder);
	dblArray3_t Bpplus = totalBatP(partobj->getP() + halfds, norder);

	double vperp2 = pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2) - pow(BpdotV / Bplen, 2);
	double mu = partobj->getM() * vperp2 / (2 * Bplen);
	dblArray3_t FgradB = (Bpplus - Bpminus) * -mu;

	for (unsigned int jjj = 0; jjj < 3; jjj++)
	{
		if (abs(Bp[jjj]) < 1e-20)
		{
			FgradB[jjj] = 0;
			continue;
		}
		FgradB[jjj] /= (2 * scaledLength);
	}

	return FgradB;
}