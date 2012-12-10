/*
 *  Bomb.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Bomb.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}
void Bomb::setParameters(TVector accle, char *explosionFile, const char *soundFileName)
{
	this->accele = accle;
	explosion.initialExplosion(explosionFile, soundFileName);
}

void Bomb::moveToCollisionPoint(float lamda)
{
	currentPosition = oldPosition + velocity*lamda;
	startExplosion();
	setLifeStatus(false);
}

void Bomb::startExplosion()
{
	explosion.startExplosion(currentPosition, 1.0, 10.0);
}

bool Bomb::getExplosionLifeStatus()
{
	return explosion.getLifeStatus();
}