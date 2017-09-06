#ifndef MBC_API_H
#define MBC_API_H

#include "bfield/WireCoil_c.h"
#include "particle/Particle_c.h"
#include "bfield/BField_c.h"
#include "tools/vectortools.h"

#define DLLEXPORT extern "C" __declspec(dllexport)

DLLEXPORT BField* initSimulation(double dt, double WC1x, double WC1y, double WC1z, double WC2x, double WC2y, double WC2z, int N, double I, double R);
DLLEXPORT Particle** createAnElectron();
DLLEXPORT void updateParticlePosition(Particle** particleArray, int particleIndex, BField* B);
DLLEXPORT double returnSimTime(BField* B);
DLLEXPORT double* returnParticlePosition(Particle** particleArray, int particleIndex);
DLLEXPORT double* returnParticleVelocity(Particle** particleArray, int particleIndex);
DLLEXPORT void deleteObjects(BObject* B, Particle** particleArray, int numberOfParticles);

#endif