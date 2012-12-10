/*
 *  AutoWarObject.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AutoWarObject.h"

AutoWarObject::AutoWarObject()
{
	life = false;
}

AutoWarObject::~AutoWarObject()
{
}

void AutoWarObject::setParameters(TVector initAccele)
{
	accele = initAccele;
}

void AutoWarObject::setPosition(float x, float y, float z)
{
	currentPosition(x, y, z);
}

void AutoWarObject::setLifeStatus(bool lifeStatus)
{
	this->life = lifeStatus;
}

void AutoWarObject::startMove(TVector initPosition, TVector initVelocity)
{
	life = true;
	currentPosition = initPosition;
	velocity = initVelocity;
}

void AutoWarObject::updateVelocity(float restTime)
{
	velocity += accele*restTime;
}

void AutoWarObject::updatePosition(float restTime)
{
	oldPosition = currentPosition;
	currentPosition = oldPosition + velocity* restTime;
}
