#include "AutoTank.h"

AutoTank::AutoTank()
{
	bombVelocityMag = 2.0;
	bombAcceleMag = 0.02;
	radius = 8.0;
	angleWithX = 0.0;
	bombArray = NULL;
	collisionBoxArray = NULL;
	targetObject = NULL;
	terrain = NULL;
	shotted = false;
	deltaTimeRunAway = 5000;
}

void AutoTank::setParameters(const char *modelFile, TVector position, float velocityMag, Terrain *terrain, BombArray *bombArray, CollisionBoxArray *collisionBoxArray)
{
	model.setParameters(modelFile, 0.2);
	this->currentPosition = position;
	this->velocityMag = velocityMag;
	this->terrain = terrain;
	this->bombArray = bombArray;
	this->collisionBoxArray = collisionBoxArray;
}

void AutoTank::setTargetObject(ControlWarObject *targetObject)
{
	this->targetObject = targetObject;
}

void AutoTank::initAutoTank()
{
	model.initModel();
	collisionBox.setParameters(currentPosition, 0.0, 14.0, 3.0, 7.0);
	collisionBoxArray->addCollisionBox(&collisionBox);
}

void AutoTank::updateLifeStatus()
{
	if(!collisionBox.isLife())
			life = false;
}

void AutoTank::startFight(TVector startPositionNoNeedY, float angleWithX)
{
	this->angleWithX = angleWithX;
	currentPosition = startPositionNoNeedY;
	currentPosition.setY(terrain->GetHeight((float)currentPosition.X(), (float)currentPosition.Z()) + 6.4);
	collisionBox.setTimeLife(2);
	collisionBox.updateCollisionBoxPos(currentPosition);
	life = true;
	runAwayStatus = false;
	shotted = false;
}

void AutoTank::changeDirection(TVector targetPosition)
{
	TVector newDirection = targetPosition - currentPosition;
	if(newDirection.X() == 0.0)
		angleWithX = 90.0;
	else
		angleWithX = atan(-newDirection.Z()/newDirection.X()) * 180.0/M_PI;
	if(angleWithX < 0.0)
		angleWithX += 180.0;
	if(newDirection.Z() >0)
		angleWithX += 180.0;
	
	collisionBox.updateCollisionBoxAngle(angleWithX);
}

void AutoTank::shot()
{
	if(!life)
		return;

	changeDirection(targetObject->getPosition());

	float radGunAngle = M_PI*15.0/180.0;
	float radTankAngle = M_PI*angleWithX/180.0;
	
	bombStartPos.setX(currentPosition.X()+ 14.0*cos(radGunAngle)*cos(radTankAngle));
	bombStartPos.setY(targetObject->getPosition().Y());
	bombStartPos.setZ(currentPosition.Z()- 14.0*cos(radGunAngle)*sin(radTankAngle));

	float s = bombStartPos.dist(targetObject->getPosition());
	bombStartPos.setY(currentPosition.Y()+ 14.0*sin(radGunAngle) +2.0);
	float h = bombStartPos.Y() - targetObject->getPosition().Y();

	float x = bombAcceleMag /(0.5*s + 1.9318*h);
	bombVelocityMag = s*sqrt(x);

	bombVelocity.setX(bombVelocityMag*cos(radGunAngle)*cos(radTankAngle));
	bombVelocity.setY(bombVelocityMag*sin(radGunAngle));
	bombVelocity.setZ(-bombVelocityMag*cos(radGunAngle)*sin(radTankAngle));
	
	bombArray->addTankBomb(bombStartPos, bombVelocity);

	shotted = true;
	shotTime = glutGet(GLUT_ELAPSED_TIME);
	//runAway();
}

void AutoTank::runAway()
{

	runAwayStatus = true;
	srand(time(NULL));
	int plusAngle =-90 + rand()%181;

	angleWithX += (float)plusAngle;
	if(angleWithX > 360.0)
		angleWithX -= 360.0;
	if(angleWithX <0.0)
		angleWithX += 360.0;

	float radAngle = M_PI*angleWithX/180.0;
	velocity.setX(-velocityMag*cos(radAngle));
	velocity.setY(0.0);
	velocity.setZ(velocityMag*sin(radAngle));

	runAwayStartTime = glutGet(GLUT_ELAPSED_TIME);
}

void AutoTank::moveTo(float plusAngle)
{
	angleWithX += plusAngle;
	if(angleWithX > 360.0)
		angleWithX -= 360.0;
	if(angleWithX <0.0)
		angleWithX += 360.0;

	float radAngle = M_PI*angleWithX/180.0;
	velocity.setX(velocityMag*cos(radAngle));
	velocity.setY(0.0);
	velocity.setZ(-velocityMag*sin(radAngle));
}

void AutoTank::moveTo(TVector newPosition)
{
	setPosition(newPosition.X(), newPosition.Y(), newPosition.Z());
	currentPosition.setY(terrain->GetHeight((float)currentPosition.X(), (float)currentPosition.Z()) + 6.4);
}

void AutoTank::update(float restTime)
{
	updateLifeStatus();

	currentSystemTime = glutGet(GLUT_ELAPSED_TIME);
	if(shotted)
		if(currentSystemTime - shotTime > 2000)
		{
			runAway();
			shotted = false;
		}

	if(runAwayStatus)
	{
		if(currentSystemTime - runAwayStartTime < deltaTimeRunAway)
		{
			updatePosition(restTime);
			currentPosition.setY(terrain->GetHeight((float)currentPosition.X(), (float)currentPosition.Z()) + 6.4);
			collisionBox.updateCollisionBoxPos(currentPosition);
		}
		else runAwayStatus = false;
	}
}

void AutoTank::draw()
{
	if(life)
	{
		glPushMatrix();
		glTranslatef(currentPosition.X(), currentPosition.Y(), currentPosition.Z());
		glRotatef(angleWithX, 0.0, 1.0, 0.0);
		model.display();
		glPopMatrix();
	}
}