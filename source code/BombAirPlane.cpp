/*
 *  BombAirPlane.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BombAirPlane.h"

BombAirPlane::BombAirPlane()
{
	bombType = airPlaneBomb_Type;
	minusLifeDelta = 4;
	angleWithX =0.0;
}

void BombAirPlane::setMqoFile(const char *mqoFile)
{
	model.setParameters(mqoFile, 0.2);
}

void BombAirPlane::initBombAirPlane()
{
	model.initModel();
}

void BombAirPlane::setAngleWithX(float angleWithX)
{
	this->angleWithX = angleWithX;
}

void BombAirPlane::draw()
{
	if(getLifeStatus())
	{
		glPushMatrix();
		glTranslated(currentPosition.X(), currentPosition.Y(), currentPosition.Z());
		glRotatef(angleWithX, 0.0, 1.0, 0.0);
		model.display();
		glPopMatrix();
	}
	explosion.draw();
}