#include <glew.h>
#include "constsandtypes.h"
#include "vectortools.h"
#include "ItemDraw_c.h"

void drawSphere(double r, int ndiv = 100, double colorR = 0, double colorG = 1, double colorB = 0)
{
	GLUquadric* quadGLObj = gluNewQuadric();
	gluQuadricNormals(quadGLObj, GLU_SMOOTH);
	glColor3d(colorR,colorG,colorB);
	gluSphere(quadGLObj, r, ndiv, ndiv);
}

void drawTorus(int numc, int numt, double radius, double thickness = 0.01, dblArray3_t color = { 0.75, 0.75, 0.75 })
{
	int sign[3]{ 1, -1, -1 };
	double s{ 0.0 };
	double t{ 0.0 };
	double x{ 0.0 };
	double y{ 0.0 };
	double z{ 0.0 };
	double twopi{ 2 * M_PI };
	for (int iii = 0; iii < numc; iii++)
	{
		glBegin(GL_QUAD_STRIP);
		glColor3d(color[0], color[1], color[2]);
		for (int jjj = 0; jjj <= numt; jjj++)
		{
			for (int kkk = 0; kkk < 2; kkk++)
			{
				s = (iii + sign[kkk]) % numc + 0.5;
				t = jjj % numt;
				x = (radius + thickness*cos(s * twopi / numc))*cos(t * twopi / numt);
				y = (radius + thickness*cos(s * twopi / numc))*sin(t * twopi / numt);
				z = thickness * sin(s * twopi / numc);
				glVertex3d(x, y, z);
			}
		}
		glEnd();
	}
	//glEnd(); //in python, this is nested in the top loop - doesn't make sense to call glEnd() multiple times...maybe a mistake
}			   //but just in case, if it torus doesn't draw right, move up a level

/*void renderScene() //needs to be made a callback function passed to MinVR - the example nests this in a class
{
	glClear(GL_COLOR_BUFFER_BIT);
	int width = renderState.getValue("WindowWidth", "/");
	int height = renderState.getValue("WindowHeight", "/");
	double ratio = width / height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, width / height, 0.1, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -15);
}*/

//private
void ParticlePic::incrementInd()
{
	//circular buffer
	if (!maketrail_m)
		return;
	if (trailind_m < (retain_m * interval_m))
	{
		trailind_m++;
		return;
	}
	trailind_m = 0;
}

void ParticlePic::setPos(const dblArray3_t& P)
{
	//add element to array if make trail is true
	trail_m[trailind_m] = P;
}

//public
void ParticlePic::draw(const dblArray3_t& P)
{
	setPos(P);
	for (int iii = 0; iii < trail_m.size(); iii++)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(trail_m[iii][0], trail_m[iii][1], trail_m[iii][2]);
		drawSphere(radius_m);
		glPopMatrix();
	}
	incrementInd();
}

void WireCoilPic::draw(const dblArray3_t& dummy)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	dblArray3_t xyz = { 0.0, 0.0, -1.0 };
	dblArray3_t c = cross3dVectorsAndMultByConst(xyz, axis_m);
	glTranslated(cent_m[0], cent_m[1], cent_m[2]);
	glRotatef(90, c[0], c[1], c[2]);
	drawTorus(5, 30, radius_m, thickness_m);
	glPopMatrix();
}