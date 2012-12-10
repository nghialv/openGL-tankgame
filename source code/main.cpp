#include <iostream>
#include <cstdlib>
#include <time.h>
#include "IncludeS.h"

#include "Camera.h"
#include "Texture.h"
#include "Terrain.h"
#include "Tank.h"
#include "AirPlane.h"
#include "AutoTank.h"
#include "AutowarControl.h"

#include "BombArray.h"
#include "CollisionSquare.h"
#include "CollisionBox.h"
#include "CollisionBoxArray.h"

//global variables
static int gWinWidth = 1200;
static int gWinHeight = 600;
static unsigned int gViewMode = 0;
static float gTimeStep = 0.6;
static float gMoveDelta = 2.0;
static float gAngleTankDelta = 3.0;
static float gAngleGunDelta = 5.0;
static float gBombVelocityMagDelta = 0.4;
static float gBombVelocityMag = 2.0;
static float gAutoBombVelocityMag = 1.8;
static float gAirPlaneVelocityMag = 1.0;

static float gFogDensity = 0.0025;

static TVector gAcceleBomb(0.0, -0.02, 0.0);
static float gRadViewAngle;
static TVector gCameraPosition;
static TVector gCenterPoint(mapX*mapScale*0.5, 5.0,  -mapZ * mapScale*0.5);

static float tankHeightPos;


AutoBomb autoBomb;

//object
BombArray bombArray;
CollisionSquare groundCollSquare;
CollisionBoxArray collisionBoxArray;
Terrain myTerrain;
Tank myTank;
AirPlane airPlane;
AutoTank autoTank;
AutoTank autoTank2;
AutoWarControl autoWarControl;

Camera camera(gCenterPoint.X(), 30.0, gCenterPoint.Z(), gCenterPoint.X()+100.0, 10.0, gCenterPoint.Z(),0.0,1.0,0.0);


//prototype funciton
void draw();
void myReshape(int width, int height);
void mySetLight();
void quit();
void caculateCameraView(unsigned int viewMode);

//=======================================
void draw()
{
	glEnable(GL_DEPTH_TEST);
	
	//draw terrain
	glDisable(GL_LIGHTING);
	myTerrain.drawTerrain();
	glEnable(GL_LIGHTING);

	//draw bomb array
	glDisable(GL_FOG);
	bombArray.draw();
	glEnable(GL_FOG);
	
	//draw tank
	myTank.draw();

	//draw autoTank
	autoTank.draw();
	autoTank2.draw();

	//draw airplane
	airPlane.draw();

	//collisionBoxArray.draw();

	glDisable(GL_DEPTH_TEST);
}

void myDisplay()
{	
	if(gViewMode == 2)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//viewport1
		glViewport(0, 0, 2*gWinWidth/3, gWinHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)(2*gWinWidth)/(double)(3*gWinHeight), 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		caculateCameraView(0);
		camera.view();
		draw();

		//viewport2
		glViewport(2*gWinWidth/3, gWinHeight/2, gWinWidth/3, gWinHeight/2);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)(2*gWinWidth)/(double)(3*gWinHeight), 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		caculateCameraView(2);
		camera.view();
		draw();

		//viewport3
		glViewport(2*gWinWidth/3, 0, gWinWidth/3, gWinHeight/2);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)(2*gWinWidth)/(double)(3*gWinHeight), 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		caculateCameraView(3);
		camera.view();
		draw();
	}
	else
	{
		glViewport(0, 0, gWinWidth, gWinHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)gWinWidth/(double)gWinHeight, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		caculateCameraView(gViewMode);
		camera.view();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw();
	}

	glutSwapBuffers();
}

void myIdle()
{
	//caculateCameraView(gViewMode);
	airPlane.update(gTimeStep);
	bombArray.checkCollision(gTimeStep);
	autoTank.update(gTimeStep);
	autoTank2.update(gTimeStep);
	autoWarControl.update(gTimeStep);
	glutPostRedisplay();
}

void myInit()
{
	//make the background look like the sky
	float blue[4] = {0.5,0.5,1.0,0.0};
	glClearColor(0.5, 0.5, 1.0, 0.0);
		
	glShadeModel(GL_SMOOTH);
	
	mqoInit();

	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR,blue);
	glFogf(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_START, 200);
	glFogf(GL_FOG_END, 1000);
	glFogf(GL_FOG_DENSITY, gFogDensity);

	//initial terrain
	myTerrain.initializeTerrain("Data/Texture/Terrain/sand.tga", "Data/Texture/Terrain/cactus.tga");

	//initial ground collision square
	groundCollSquare.setParameters(TVector(0.0, 1.0, 0.0), gCenterPoint);

	//initial bomb array 
	bombArray.setParForBombType(tankBomb_Type, gAcceleBomb, "Data/Texture/Explosion/tankBombEx.tga", "Data/Sound/bombTank.wav");
	bombArray.setParForBombType(airPlaneBomb_Type, gAcceleBomb, "Data/Texture/Explosion/tankBombEx.tga", "Data/Sound/bombAirPlane.wav");
	bombArray.setParForBombType(autoBomb_Type, gAcceleBomb, "Data/Texture/Explosion/tankBombEx.tga", "Data/Sound/bombAirPlane.wav");
	
	bombArray.setParForCollision(&groundCollSquare, &collisionBoxArray);

	//initial tank
	tankHeightPos = myTerrain.GetHeight(gCenterPoint.X()+100.0, gCenterPoint.Z());

	myTank.setParameters("Data/Mqo/Tank/bodyTank.mqo", "Data/Mqo/Tank/gunTank.mqo", TVector(gCenterPoint.X()+100.0, tankHeightPos + 9.0, gCenterPoint.Z()), 0.0, &myTerrain, gBombVelocityMag,  &bombArray, &collisionBoxArray);
	myTank.setGunLong(20.0);
	myTank.setAutoBombVelocityMag(gAutoBombVelocityMag);
	myTank.initTank();	

	//initial autoTank
	autoTank.setParameters("Data/Mqo/AutoTank/t70.mqo", TVector(gCenterPoint.X()+100.0, tankHeightPos + 6.4, gCenterPoint.Z()), 0.08, &myTerrain, &bombArray, &collisionBoxArray);
	autoTank.initAutoTank();
	autoTank.setTargetObject(&myTank);

	autoTank2.setParameters("Data/Mqo/AutoTank/t70.mqo", TVector(gCenterPoint.X()+100.0, tankHeightPos + 6.4, gCenterPoint.Z()), 0.08, &myTerrain, &bombArray, &collisionBoxArray);
	autoTank2.initAutoTank();
	autoTank2.setTargetObject(&myTank);

	//initial airPlane
	airPlane.setParameters("Data/Mqo/AirPlane/jet.mqo", TVector(0.0, 0.0, 0.0), &bombArray, &collisionBoxArray);
	airPlane.initAirPlane();

	//initial autoWarControl
	autoWarControl.setParameters(&myTank);
	autoWarControl.setAirPlane(&airPlane,  gAirPlaneVelocityMag);
	autoWarControl.addAutoTank(&autoTank);
	autoWarControl.addAutoTank(&autoTank2);
	autoWarControl.init();

	myTank.startFight();
	mySetLight();
}



void caculateCameraView(unsigned int viewMode)
{
	switch(viewMode)
	{
	case 0:
		gRadViewAngle = M_PI*myTank.getAngleWithX()/180.0 - M_PI;
		gCameraPosition.setX(myTank.getPosition().X() + 150*cos(gRadViewAngle));
		gCameraPosition.setY(myTank.getPosition().Y() + 30.0);
		gCameraPosition.setZ(myTank.getPosition().Z() - 150*sin(gRadViewAngle));

		camera.changePosition(gCameraPosition);
		camera.changeTarget(gCameraPosition.X() - 150.0*cos(gRadViewAngle), gCameraPosition.Y(), gCameraPosition.Z() + 150.0*sin(gRadViewAngle));
		break;
	case 1:
		gRadViewAngle = M_PI*myTank.getAngleWithX()/180.0  - M_PI;
		gCameraPosition.setX(myTank.getPosition().X() + 250*cos(gRadViewAngle));
		gCameraPosition.setY(myTank.getPosition().Y() + 50.0);
		gCameraPosition.setZ(myTank.getPosition().Z() - 250*sin(gRadViewAngle));

		camera.changePosition(gCameraPosition);
		camera.changeTarget(gCameraPosition.X() - 150.0*cos(gRadViewAngle), gCameraPosition.Y(), gCameraPosition.Z() + 150.0*sin(gRadViewAngle));
		break;
	case 2:
		gRadViewAngle = M_PI*myTank.getAngleWithX()/180.0  - M_PI*0.5;
		gCameraPosition.setX(myTank.getPosition().X() + 250*cos(gRadViewAngle));
		gCameraPosition.setY(myTank.getPosition().Y() + 50.0);
		gCameraPosition.setZ(myTank.getPosition().Z() - 250*sin(gRadViewAngle));

		camera.changePosition(gCameraPosition);
		camera.changeTarget(gCameraPosition.X() - 150.0*cos(gRadViewAngle), gCameraPosition.Y(), gCameraPosition.Z() + 150.0*sin(gRadViewAngle));
		break;
	case 3:
		gRadViewAngle = M_PI*myTank.getAngleWithX()/180.0;
		gCameraPosition.setX(myTank.getPosition().X() + 250*cos(gRadViewAngle));
		gCameraPosition.setY(myTank.getPosition().Y() + 50.0);
		gCameraPosition.setZ(myTank.getPosition().Z() - 250*sin(gRadViewAngle));

		camera.changePosition(gCameraPosition);
		camera.changeTarget(gCameraPosition.X() - 150.0*cos(gRadViewAngle), gCameraPosition.Y(), gCameraPosition.Z() + 150.0*sin(gRadViewAngle));
		break;
	}
}


void myKeyboard(unsigned char key,int x,int y)
{
	switch (key)
	{
		case 'g':					//decrease gun angle
			myTank.plusGunAngle(-gAngleGunDelta);
			break;
		case 'G':					//increase gun angle
			myTank.plusGunAngle(gAngleGunDelta);
			break;

		case 's':					//shot generally bomb
			myTank.shot();
			break;
		case 'm':					//shot auto bomb
			myTank.shotM(&airPlane);
			printf("%d\n", tankBomb_Type);
			break;
		case 'v':					//decrease generally bomb velocity
			myTank.plusBombVelocityMag(-gBombVelocityMagDelta);
			break;
		case 'V':					//increase generally bomb velocity
			myTank.plusBombVelocityMag(gBombVelocityMagDelta);
			break;
	
		case 'f':					//decrease fog density
			if(gFogDensity > 0.0005)
			{
				gFogDensity -= 0.0005;
				glFogf(GL_FOG_DENSITY, gFogDensity);
			}		
			break;
		case 'F':					//increase fog density
			if(gFogDensity < 0.99)
			{
				gFogDensity += 0.001;
				glFogf(GL_FOG_DENSITY, gFogDensity);
			}
			break;
		
		case 'r':					//reset game
			myTank.startFight();
			break;
		case 27:					//exit game
			exit(0);
			break;
		
		default:
			break;

		/*case 'a':
			autoWarControl.airPlaneStart();
			break;
		case 'S':
			airPlane.shot();
			break;
		case 'l':
			autoWarControl.autoTankStart(0);
			break;
		case 'k':
			autoTank.shot();
			break;*/
	}
}

void mySpecialKeyboard(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT:				//move to left
			myTank.plusAngleWithX(gAngleTankDelta);
			break;
		case GLUT_KEY_RIGHT:			//move to right
			myTank.plusAngleWithX(-gAngleTankDelta);
			break;
		case GLUT_KEY_UP:				//move to up
			myTank.move(gMoveDelta);
			break;
		case GLUT_KEY_DOWN:				//move to right
			myTank.move(-gMoveDelta);
			break;
		case GLUT_KEY_F1:				//view 1
			gViewMode = 0;
			break;
		case GLUT_KEY_F2:				//view 2
			gViewMode = 1;
			break;
		case GLUT_KEY_F3:				//view 3
			gViewMode = 2;
			break;
	}
}

//============================================  OK =========================================================
int main (int argc, char ** const argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH |GLUT_DOUBLE);
	glutInitWindowSize(gWinWidth, gWinHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("MyGame");
	
	myInit();
		 
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutIdleFunc(myIdle);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKeyboard);
	glutMainLoop();
	
	quit();
	return 0;	
}

void mySetLight()
{
	GLfloat diffuse[]={0.9, 0.9, 0.9, 1.0};
	GLfloat specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat ambient[]={0.3, 0.3, 0.3, 0.1};
	GLfloat position[]= {100.0, 200.0, 200.0, 0.0};
	
	glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void quit()
{
	mqoCleanup();
}

void myReshape(int width, int height)
{
	gWinWidth=width;
	gWinHeight=height;
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}