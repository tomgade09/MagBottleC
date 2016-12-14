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
	
	dblArray3_t e1posinit{ -4.75, 1.0, 1.0 };
	dblArray3_t e1velinit{ 1000.0, 1000.0, 1000.0 };
	Particle electron1 = Particle(-1.60217657e-19, 9.10938356e-31, e1posinit, e1velinit);
	
	std::cout << "Electron created." << std::endl;
	
	B.addBObj(&WC1);
	B.addBObj(&WC2);
	
	std::cout << "addBObj complete." << std::endl;
	long long int loopind{ 0 };
	dblArray3_t Bstr{ nulldA3_t };
	while (B.getTime() <= 5.0001e-6)
	{
		std::cout.precision(13);
		//for (int iii = 2; iii <= 128; iii++) //commented code for testing for appropriate n order
		//{
			//Bstr = B.totalBatP(electron1.getP(), iii);
			//std::cout << "n: " << iii;
			//std::cout << "  B: ";
			//dA3coutSci(Bstr);
		//}
		//exit(0);
		Bstr = B.totalBatP(electron1.getP(), 37);
		electron1.updP(Bstr, B.getdt());
		
		if (loopind % 1000 == 0)
		{
			std::cout << "Location: " << electron1.getP() << " | Index: " << loopind << /*" | dT: " << B.getdt() << */std::endl;
			std::cout << "Velocity: " << electron1.getV() << " | Time: " << B.getTime() << std::endl;
			std::cout << std::endl;
		}
		
		loopind++;
	}
}