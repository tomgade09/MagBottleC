#include "bfield/BField_c.h"
#include "bfield/BObject_c.h"
#include "bfield/WireCoil_c.h"
#include "particle/Particle_c.h"
#include "tools/vectortools.h"
#include "vis/ItemDraw_c.h"

#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

int main()
{
	BField B(5e-7);

	std::cout << "BField creation complete." << std::endl;

	dblArray3_t WC1cent{ -5.0, 0.0, 0.0 };
	dblArray3_t WC2cent{ 5.0, 0.0, 0.0 };
	dblArray3_t WCaxis{ 1.0, 0.0, 0.0 };
	double WCrad{ 5.0 };
	WireCoil WC1 = WireCoil(WC1cent, WCaxis, 1, 1.0, WCrad, 0, "Left");
	WireCoil WC2 = WireCoil(WC2cent, WCaxis, 1, 1.0, WCrad, 1, "Right");

	std::cout << "WireCoils created." << std::endl;

	dblArray3_t e1posinit{ -4.0, 3.0, 0.0 };
	dblArray3_t e1velinit{ 500.0, 4000.0, 4000.0 };
	Particle electron1 = Particle(-1.60217657e-19, 9.10938356e-31, e1posinit, e1velinit);

	std::cout << "Electron created." << std::endl;

	B.addBObj(&WC1);
	B.addBObj(&WC2);

	std::cout << "addBObj complete." << std::endl;

	std::cout << "MinVR init." << std::endl;
	//pics
	ParticlePic e1Pic;
	WireCoilPic wc1Pic(WC1cent, WCaxis, WCrad);
	WireCoilPic	wc2Pic(WC2cent, WCaxis, WCrad);
	std::vector<Pic*> picptr;
	picptr.reserve(3);
	picptr.push_back(&e1Pic);
	picptr.push_back(&wc1Pic);
	picptr.push_back(&wc2Pic);

	//GLFW
	GLFWwindow* window;
	
	if (!(glfwInit()))
		return -1;
	
	int width = 1920;
	int height = 1080;
	double ratio = static_cast<double>(width) / height;
	window = glfwCreateWindow(width, height, "MagBottleC", NULL, NULL);
	
	if (!(window))
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetTime(0.0);

	dblArray3_t elecP = electron1.getP();

	int indind{ 0 };

	while (!glfwWindowShouldClose(window))
	{
		//electron1.updP(B.totalBatP(electron1.getP(), 37), B.getdt()); //old method
		B.updateParticleP_V(&electron1, 37);
		elecP = electron1.getP();
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, width / height, 0.1, 50.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -15);
		for (unsigned int iii = 0; iii < picptr.size(); iii++)
			picptr[iii]->draw(elecP);
		
		if (indind % 1000 == 0)
			std::cout << indind << " : " << elecP << std::endl;
		indind++;
		glfwSwapBuffers(window);
		glEnd();
	}

	glfwTerminate();

	return 0;

}