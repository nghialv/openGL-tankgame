#include "AutoWarControl.h"

AutoWarControl::AutoWarControl()
{
	airPlaneShotNumber = 1;
	deltaTimeAirPlaneAppear = 30000;
	lastTimeAirPlaneAppear = glutGet(GLUT_ELAPSED_TIME);
	lastTimeAutoTankShot = lastTimeAirPlaneAppear;
	deltaTimeAutoTankShot = 9000;
	lastTimeCreateAutoTank = lastTimeAirPlaneAppear;
	deltaTimeCreateAutoTank = 19000;

	distantMax = 500.0;
}

void AutoWarControl::setParameters(ControlWarObject *targetObject)
{
	this->targetObject = targetObject;
}

void AutoWarControl::setAirPlane(AirPlane *airPlane, float airPlaneVelocityMag)
{
	this->airPlane = airPlane;
	this->airPlaneVelocityMag = airPlaneVelocityMag;
}

void AutoWarControl::addAutoTank(AutoTank *autoTank)
{
	autoTankArray.push_back(autoTank);
}

void AutoWarControl::airPlaneStart()
{
	printf("AirPlane start\n");
	airPlaneShotNumber = 1;
	srand(time(NULL));
	int angle = rand()%360;
	float angleWithX = ((float)angle)*M_PI/180.0;
	TVector startPosition = targetObject->getPosition() + TVector(300*cos(angleWithX), 120.0, -300*sin(angleWithX));
	PlaySound("Data/Sound/airPlane.wav", NULL, SND_FILENAME | SND_ASYNC);
	airPlane->startMove(startPosition, airPlaneVelocityMag, angleWithX*180.0/M_PI + 180.0);
}

void AutoWarControl::autoTankStart(int tankNumber)
{
	srand(time(NULL));
	int angle = rand()%360;
	float angleWithX = ((float)angle)*M_PI/180.0;
	TVector startPosition = targetObject->getPosition() + TVector(350*cos(angleWithX), 0.0, -350*sin(angleWithX));
	angleWithX = angleWithX*180.0/M_PI;
	angleWithX += 180.0;
	if(angleWithX >360.0)
		angleWithX -= 360.0;
	autoTankArray[tankNumber]->startFight(startPosition, angleWithX);
}

void AutoWarControl::autoTankShot(int tankNumber)
{
	autoTankArray[tankNumber]->shot();
}

void AutoWarControl::init()
{
	if(!autoTankArray[0]->getLifeStatus())
	{
		autoTankStart(0);
	}
}

void AutoWarControl::autoTankArrayStart()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	if(currentTime - lastTimeCreateAutoTank > deltaTimeCreateAutoTank)
	{
		for(unsigned int i=0; i<autoTankArray.size(); i++)
		{
			if(!autoTankArray[i]->getLifeStatus())
			{
				autoTankStart(i);
				lastTimeCreateAutoTank = glutGet(GLUT_ELAPSED_TIME);
				break;
			}
		}
	}
}

void AutoWarControl::autoTankArrayShot()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	if(currentTime - lastTimeAutoTankShot > deltaTimeAutoTankShot)
	{
		srand(time(NULL));
		int shotNumber = rand()%8;

		if(shotNumber ==0)	//2 shot
		{
			printf("2 shot\n");
			shotNumber = 2;
			for(unsigned int i=0; i<autoTankArray.size() && shotNumber >0; i++)
			{
				if(autoTankArray[i]->getLifeStatus())
				{
					if(autoTankArray[i]->getCurrentPosition().dist(targetObject->getPosition()) < distantMax)
					{
						autoTankShot(i);
						printf("shot autoTankNumbet is %d\n", i);
						shotNumber --;
					}
				}
			}
		}
		else		//1 shot
		{
			printf("1 shot\n");
			float minDistant = distantMax;
			unsigned int shotNum =-1;
			for(unsigned int i=0; i<autoTankArray.size(); i++)
			{
				if(autoTankArray[i]->getLifeStatus())
				{
					if(autoTankArray[i]->getCurrentPosition().dist(targetObject->getPosition()) < minDistant)
					{
						shotNum = i;
						minDistant = autoTankArray[i]->getCurrentPosition().dist(targetObject->getPosition());
					}
				}
			}
			if(shotNum != -1)
			{
				autoTankShot(shotNum);
				printf("shot autoTankNumbet is %d\n", shotNum);
			}
		}

		lastTimeAutoTankShot =  glutGet(GLUT_ELAPSED_TIME);
		srand(time(NULL));
		deltaTimeAutoTankShot = 6000 + 30*(rand()%100);
	}
}

void AutoWarControl::checkCollisionWithTank()
{
	float distant;
	TVector uDirection;
	for(unsigned int i=0; i<autoTankArray.size(); i++)
	{
		if(autoTankArray[i]->getLifeStatus())
		{
			distant = autoTankArray[i]->getRadius() + targetObject->getRadius();
			uDirection = autoTankArray[i]->getCurrentPosition() - targetObject->getPosition();
			if(uDirection.mag() < distant)
			{
				uDirection.unit();
				TVector newPosition = targetObject->getPosition() + uDirection*distant;
				autoTankArray[i]->moveTo(newPosition);
			}
		}
	}
}

void AutoWarControl::update(float restTime)
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	//update for airPlane
	if(currentTime - lastTimeAirPlaneAppear > deltaTimeAirPlaneAppear)		//start appear airPlane
	{
		if(!airPlane->getLifeStatus())
		{
			airPlaneStart();
			lastTimeAirPlaneAppear = currentTime;
			srand(time(NULL));
			deltaTimeAirPlaneAppear = 25000 + 50*(rand()%100);
		}
	}
	
	if(targetObject->getLifeStatus())
		if(airPlane->getLifeStatus())			//check start bomb airPlane
		{
			TVector direction = airPlane->getCurrentPosition() - targetObject->getPosition();
			direction.unit();
			float angle = acos(direction.dot(TVector(0.0, 1.0, 0.0))) * 180.0/M_PI;
			if(angle <4.0 && airPlaneShotNumber >0)
			{
				airPlaneShotNumber --;
				airPlane->shot();
			}
		}


	//update for autoTank
	if(targetObject->getLifeStatus())
	{
		autoTankArrayStart();
		autoTankArrayShot();
		checkCollisionWithTank();
	}
}
