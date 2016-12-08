#include "Particle_c.h"
#include <cmath>

void Particle::updV(const dblArray3_t& b, double dt)
{
	dblArray3_t a(cross3dVectorsAndMultByConst(velocity_m, b, qOVERm_m * dt));
	for (int iii = 0; iii < 3; iii++)
		velocity_m[iii] += a[iii];
}

dblArray3_t Particle::updP(const dblArray3_t& b, double dt)
{
	updV(b, dt);
	for (int iii = 0; iii < 3; iii++)
		position_m[iii] += (velocity_m[iii] * dt);
}

dblArray3_t Particle::calcBatP(const dblArray3_t p)
{
	dblArray3_t r{ p[0] - position_m[0], p[1] - position_m[1], p[2] - position_m[2] };
	bool boo = 1;
	for (int iii = 0; iii < 3; iii++)
		boo &= (r[iii] < 1e-14);
	if (boo)
		return{ 0.0, 0.0, 0.0 }; //no mag field "inside" point charge
	return cross3dVectorsAndMultByConst(velocity_m, r, (SCALE * charge_m / pow(dA3len(r), 3)));
}