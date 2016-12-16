#pragma once
#include <display/VRConsoleNode.h>
#include <main/VRMain.h>
#include <main/VREventHandler.h>
#include <main/VRRenderHandler.h>
#include <math/VRMath.h>
#include <main/impl/VRDefaultAppLauncher.h>

#include "bfield/BObject_c.h"
#include "bfield/BField_c.h"
#include "particle/Particle_c.h"
#include "vis/ItemDraw_c.h"

#define NOMINMAX
#include <windows.h>
//#include <glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <iostream>
#include <math.h>
#include <vector>

using namespace MinVR;

class MyVRApp : public VREventHandler, public VRRenderHandler {
public:
	MyVRApp(int argc, char** argv) : _vrMain(NULL), _quit(false) {
		_vrMain = new VRMain();
		_vrMain->initialize(argc, argv);
		_vrMain->addEventHandler(this);
		_vrMain->addRenderHandler(this);
		_horizAngle = 0.0;
		_vertAngle = 0.0;
		_radius = 0.0;
		_incAngle = -0.1f;
	}

	virtual ~MyVRApp() {
		_vrMain->shutdown();
		delete _vrMain;
	}

	// Callback for event handling, inherited from VREventHandler
	virtual void onVREvent(const std::string &eventName, VRDataIndex *eventData);


	virtual void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode);

	int count = 0;

	// Callback for rendering, inherited from VRRenderHandler
	virtual void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode);

	void run();

	void addPic(Pic* picptr) { picList_m.push_back(picptr); }

	void addParticle(Particle* partptr) { partList_m.push_back(partptr); }

	dblArray3_t partBVP(Particle* particle)
	{
		particle->updP(bfieldp_m->totalBatP(particle->getP(), 37), bfieldp_m->getdt());
		return particle->getP();
	}

	void setBF(BField* bfp) { bfieldp_m = bfp; }

protected:
	VRMain *_vrMain;
	bool _quit;
	double _horizAngle, _vertAngle, _radius, _incAngle;
	std::vector<Pic*> picList_m;
	std::vector<Particle*> partList_m;
	BField* bfieldp_m{ nullptr };

	void drawPics(const dblArray3_t& P);
};