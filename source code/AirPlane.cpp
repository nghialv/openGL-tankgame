#include "AirPlane.h"

AirPlane::AirPlane()
{
	angleWithX = 0.0;
	this->accele = TVector(0.0, 0.0, 0.0);
}

void AirPlane::setParameters(const char *modelFile, TVector bombInitVelocity, BombArray *bombArray, CollisionBoxArray *collisionBoxArray)
{
	model.setParameters(modelFile,0.2);
	this->bombInitVelocity = bombInitVelocity;
	this->bombArray = bombArray;
	this->collisionBoxArray = collisionBoxArray;
}

void AirPlane::initAirPlane()
{
	model.initModel();
	collisionBox.setParameters(currentPosition, 0.0, 20.0, 5.0, 10.0);
	collisionBoxArray->addCollisionBox(&collisionBox);
}

void AirPlane::startMove(TVector initPosition, float velocityMag, float angleWithX)
{
	this->angleWithX = angleWithX;
	this->velocityMag = velocityMag;
	float radAngle = M_PI*angleWithX/180.0;
	collisionBox.setTimeLife(2);
	AutoWarObject::startMove(initPosition, TVector(velocityMag*cos(radAngle), 0.0, -velocityMag*sin(radAngle)));
	collisionBox.updateCollisionBoxAngle(angleWithX);
}

void AirPlane::changeVelocityDirection(float newAngleWithX)
{
	this->angleWithX = newAngleWithX;
	float radAngle = M_PI*angleWithX/180.0;
	collisionBox.updateCollisionBoxAngle(angleWithX);
	this->velocity = TVector(velocityMag*cos(radAngle), 0.0, -velocityMag*sin(radAngle));
	collisionBox.updateCollisionBoxAngle(angleWithX);
}

void AirPlane::update(float restTime)
{
	if(life)
	{
		updateVelocity(restTime);
		updatePosition(restTime);
		collisionBox.updateCollisionBoxPos(currentPosition);
		if(currentPosition.X()<0 || currentPosition.X() >mapX*mapScale || currentPosition.Z()>0 || currentPosition.Z()<(-mapZ*mapScale))
			life = false;
		if(!collisionBox.isLife())
		{
			life = false;
			printf("airPlane die\n");
		}
	}
}

void AirPlane::shot()
{
	if(life)
	{
		bombArray->addAirPlaneBomb(currentPosition, bombInitVelocity, angleWithX);
		PlaySound("Data/Sound/bombAP.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
}

void AirPlane::draw()
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