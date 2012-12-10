/*
 *  ControlWarObject.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ControlWarObject.h"

ControlWarObject::ControlWarObject()
{
	position = TVector(0.0, 0.0, 0.0);
	angleWithX = 0.0;
	life = true;
	bombArray = NULL;
	collisionBoxArray = NULL;
}

ControlWarObject::~ControlWarObject()
{
}

void ControlWarObject::setPosition(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void ControlWarObject::setParameters(TVector position, float angleWithX, BombArray *bombArray, CollisionBoxArray *collisionBoxArray)
{
	this->position = position;
	this->angleWithX = angleWithX;
	this->bombArray = bombArray;
	this->collisionBoxArray = collisionBoxArray;
}

void ControlWarObject::setLifeStatus(bool lifeStatus)
{
	this->life = lifeStatus;
}

void ControlWarObject::plusPosition(float deltaX, float deltaY, float deltaZ)
{
	position += TVector(deltaX, deltaY, deltaZ);
}