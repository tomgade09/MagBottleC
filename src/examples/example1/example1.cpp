#include "WireCoil_c.h"
#include "Particle_c.h"
#include "BField_c.h"
#include <iostream>
#include "vectortools.h"

int main()
{
	BField B = BField(1e-10);
	
	std::cout << "BField creation complete." << std::endl;
	
	dblArray3_t WC1cent{ -5.0, 0.0, 0.0 };
	dblArray3_t WC2cent{  5.0, 0.0, 0.0 };
	dblArray3_t WCaxis{ 1.0, 0.0, 0.0 };
	WireCoil WC1 = WireCoil(WC1cent, WCaxis, 1, 1.0, 5.0, 0, "Left");
	WireCoil WC2 = WireCoil(WC2cent, WCaxis, 1, 1.0, 5.0, 1, "Right");
	
	std::cout << "WireCoils created." << std::endl;
	
	dblArray3_t e1posinit{ -4.75, 0.0, 0.0 };
	dblArray3_t e1velinit{ 1000.0, 1000.0, 1000.0 };
	Particle electron1 = Particle(-1.60217657e-19, 9.10938356e-31, e1posinit, e1velinit);
	
	std::cout << "Electron created." << std::endl;
	
	B.addBObj(&WC1);
	B.addBObj(&WC2);
	
	std::cout << "addBObj complete." << std::endl;
	
	long long int loopind{ 0 };

	while (B.getTime() < 1e-6)
	{
		electron1.updP(B.totalBatP(electron1.getP()), B.getdt());
		
		if (loopind % 1000 == 0)
		{
			std::cout << "Index: " << std::to_string(loopind) << " | Location: " << dA3ToStr(electron1.getP()) << std::endl;
			std::cout << "Velocity: " << dA3ToStr(electron1.getV()) << " | Time: " << std::to_string(B.getTime()) << std::endl;
		}
		loopind++;
	}
}