#include "MinVR.h"

using namespace MinVR;

	// Callback for event handling, inherited from VREventHandler
void MyVRApp::onVREvent(const std::string &eventName, VRDataIndex *eventData) {
	//std::cout << "Event: " << eventName << std::endl;
	if (eventName == "/KbdEsc_Down") {
		_quit = true;
	}
	else if (eventName == "/MouseBtnLeft_Down") {
		_radius += 5.0 * _incAngle;
	}
	else if (eventName == "/MouseBtnRight_Down") {
		_radius -= 5.0 * _incAngle;
	}
	else if ((eventName == "/KbdLeft_Down") || (eventName == "/KbdLeft_Repeat")) {
		_horizAngle -= _incAngle;
	}
	else if ((eventName == "/KbdRight_Down") || (eventName == "/KbdRight_Repeat")) {
		_horizAngle += _incAngle;
	}
	else if ((eventName == "/KbdUp_Down") || (eventName == "/KbdUp_Repeat")) {
		_vertAngle -= _incAngle;
	}
	else if ((eventName == "/KbdDown_Down") || (eventName == "/KbdDown_Repeat")) {
		_vertAngle += _incAngle;
	}

	if (_horizAngle > 6.283185) _horizAngle -= 6.283185;
	if (_horizAngle < 0.0) _horizAngle += 6.283185;

	if (_vertAngle > 6.283185) _vertAngle -= 6.283185;
	if (_vertAngle < 0.0) _vertAngle += 6.283185;
}


void MyVRApp::onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode) {
	if (!renderState->exists("IsConsole", "/")) {
	}
}

// Callback for rendering, inherited from VRRenderHandler
void MyVRApp::onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode) {
	if (renderState->exists("IsConsole", "/")) {
		VRConsoleNode *console = dynamic_cast<VRConsoleNode*>(callingNode);
		console->println("Console output...");
	}
	else {
		dblArray3_t elecP = partBVP(partList_m[0]);
		glClear(GL_COLOR_BUFFER_BIT);
		int width = renderState->getValue("WindowWidth", "/");
		int height = renderState->getValue("WindowHeight", "/");
		double ratio = width / height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, width / height, 0.1, 50.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -15);
		drawPics(elecP);
	}
}

void MyVRApp::run() {
	while (!_quit) {
		_vrMain->mainloop();
	}
}

void MyVRApp::drawPics(const dblArray3_t& P)
{
	for (unsigned int iii = 0; iii < picList_m.size(); iii++)
	{
		if (!(picList_m[iii]))
			continue;
		picList_m[iii]->draw(P);
	}
}