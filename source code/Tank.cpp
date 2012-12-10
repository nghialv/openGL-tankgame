/*
 *  tank.cpp
 *  GlMetaseq
 *
 *  Created by le van nghia on 12/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tank.h"

Tank::Tank()
{
	gunLong =1.0;
	gunAngle=15.0;
	autoBombVelocityMag = 1.0;
	radius = 14.0;
}

Tank::~Tank()
{
}

void Tank::setParameters(const char *bodyFile, const char *gunFile, TVector position, float angleWithX, Terrain *terrain, float bombVelocityMag,  BombArray *bombArray, CollisionBoxArray *collisionBoxArray)
{
	bodyModel.setParameters(bodyFile, 0.2);
	gunModel.setParameters(gunFile, 0.2);
	this->position = position;
	this->angleWithX = angleWithX;
	this->terrain = terrain;
	this->bombVelocityMag = bombVelocityMag;
	this->bombArray = bombArray;
	this->collisionBoxArray = collisionBoxArray;
}

void Tank::setGunLong(float gunLong)
{
	this->gunLong = gunLong;
}

float Tank::getGunLong()
{
	return gunLong;
}

void Tank::setAutoBombVelocityMag(float autoBombVelocityMag)
{
	this->autoBombVelocityMag = autoBombVelocityMag;
}

void Tank::plusGunAngle(float deltaAngle)
{
	if(!life)
		return;

	gunAngle += deltaAngle;
	if(gunAngle<0 || gunAngle >120)
		gunAngle-=deltaAngle;
}

void Tank::plusBombVelocityMag(float deltaVelocity)
{
	if(bombVelocityMag <10.0 && bombVelocityMag > -deltaVelocity)
		bombVelocityMag += deltaVelocity;
	printf("bomb velocity Mag is %2f\n", bombVelocityMag);
}

void Tank::plusAngleWithX(float deltaAngle)
{
	if(!life)
		return;

	angleWithX += deltaAngle;
	if(angleWithX>= 360)
		angleWithX = 0;
	if(angleWithX < 0)
		angleWithX = 360;
	collisionBox.updateCollisionBoxAngle(angleWithX);
}

void Tank::move(float deltaTranslate)
{
	if(!life)
		return;

	position += TVector(deltaTranslate*cos(M_PI*angleWithX/180.0), 0.0, -deltaTranslate*sin(M_PI*angleWithX/180.0));
	position.setY(terrain->GetHeight((float)position.X(), (float)position.Z()) +9.0);

	radTankAngle = M_PI*angleWithX/180.0;
	collisionBox.updateCollisionBoxPos(TVector(position.X()-2.2*cos(radTankAngle), position.Y()-2.1, position.Z()));
}

void Tank::initTank()
{
	bodyModel.initModel();
	gunModel.initModel();

	radTankAngle = M_PI*angleWithX/180.0;
	collisionBox.setParameters(TVector(position.X()-2.5*cos(radTankAngle), position.Y()-2.1, position.Z()), angleWithX, 17.0, 4.5, 8.0);

	collisionBoxArray->addCollisionBox(&collisionBox);
}

void Tank::startFight()
{
	life = true;
	collisionBox.setTimeLife(30);
}

void Tank::shot()
{
	if(!life)
		return;

	radGunAngle = M_PI*gunAngle/180.0;
	radTankAngle = M_PI*angleWithX/180.0;

	bombVelocity.setX(bombVelocityMag*cos(radGunAngle)*cos(radTankAngle));
	bombVelocity.setY(bombVelocityMag*sin(radGunAngle));
	bombVelocity.setZ(-bombVelocityMag*cos(radGunAngle)*sin(radTankAngle));
	
	bombStartPos.setX(position.X()+ gunLong*cos(radGunAngle)*cos(radTankAngle));
	bombStartPos.setY(position.Y()+ gunLong*sin(radGunAngle) +0.1);
	bombStartPos.setZ(position.Z()- gunLong*cos(radGunAngle)*sin(radTankAngle));
	
	bombArray->addTankBomb(bombStartPos, bombVelocity);
}

void Tank::shotM(AutoWarObject *targetObject)
{	
	if(!life)
		return;

	bombStartPos.setX(position.X());
	bombStartPos.setY(position.Y() +0.1);
	bombStartPos.setZ(position.Z());

	bombArray->addAutoBomb(bombStartPos, autoBombVelocityMag, targetObject);
}

void Tank::draw()
{
	if(!life)
		return;

	//printf("Angle with X when draw tank is %2f\n", angleWithX);
	glPushMatrix();
	glTranslatef(position.X(), position.Y(), position.Z());
	glRotatef(angleWithX, 0.0, 1.0, 0.0);
	bodyModel.display();
	glRotatef(gunAngle, 0.0, 0.0, 1.0);
	gunModel.display();
	glPopMatrix();

	if(!collisionBox.isLife())
	{
		life = false;
		printf("player's Tank die\n");
	}
}