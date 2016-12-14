#include "WireCoil_c.h"
#include "Particle_c.h"
#include "BField_c.h"
#include <iostream>
#include "vectortools.h"
#include "MinVR.h"
#include "ItemDraw_c.h"
#include "BField_c.h"
#include "BObject_c.h"
#include "Particle_c.h"
#include "WireCoil_c.h"

int main(int argc, char **argv)
{
	BField B = BField(1e-10);

	std::cout << "BField creation complete." << std::endl;

	dblArray3_t WC1cent{ -5.0, 0.0, 0.0 };
	dblArray3_t WC2cent{ 5.0, 0.0, 0.0 };
	dblArray3_t WCaxis{ 1.0, 0.0, 0.0 };
	double WCrad{ 5.0 };
	WireCoil WC1 = WireCoil(WC1cent, WCaxis, 1, 1.0, WCrad, 0, "Left");
	WireCoil WC2 = WireCoil(WC2cent, WCaxis, 1, 1.0, WCrad, 1, "Right");

	std::cout << "WireCoils created." << std::endl;

	dblArray3_t e1posinit{ -4.75, 1.0, 1.0 };
	dblArray3_t e1velinit{ 1000.0, 1000.0, 1000.0 };
	Particle electron1 = Particle(-1.60217657e-19, 9.10938356e-31, e1posinit, e1velinit);

	std::cout << "Electron created." << std::endl;

	B.addBObj(&WC1);
	B.addBObj(&WC2);

	std::cout << "addBObj complete." << std::endl;

	std::cout << "MinVR init." << std::endl;
	//pics
	MyVRApp app(argc, argv);
	app.setBF(&B);
	app.addParticle(&electron1);
	ParticlePic e1Pic;
	WireCoilPic wc1Pic(WC1cent, WCaxis, WCrad);
	WireCoilPic	wc2Pic(WC2cent, WCaxis, WCrad);
	app.addPic(&e1Pic);
	app.addPic(&wc1Pic);
	app.addPic(&wc2Pic);

	app.run();

	return 0;

}