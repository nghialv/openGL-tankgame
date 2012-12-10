/*
 *  BombArray.h
 *  myGame
 *
 *  Created by le van nghia on 12/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BOMBARRAY_H_
#define _BOMBARRAY_H_

#include <vector>
#include "IncludeS.h"
#include "Tvector.h"
#include "Bomb.h"
#include "BombTank.h"
#include "AutoBomb.h"
#include "BombAirPlane.h"
#include "AutoWarObject.h"
#include "CollisionSquare.h"
#include "CollisionBoxArray.h"


class BombArray
{
private:

	TVector bombAccele[3];
	char bombExplosionFile[3][100];
	char bombSoundFile[3][100];

	std::vector<Bomb *> bombArray;
	int leastNotLifeBomb;
	CollisionSquare *groundCollSquare;
	CollisionBoxArray *collisionBoxArray;
	
	void caculateLeastNotLifeBomb(unsigned int bombType);
	bool checkCollisionWithBoxArray(const TVector &position, const TVector &direction, double &lamda, unsigned int &collisionBoxNum);
public:
	BombArray();
	~BombArray();
	
	void setParForBombType(unsigned int bombType, TVector accele, const char *explosionFile, const char *soundFileName);
	void setParForCollision(CollisionSquare *groundCollSquare, CollisionBoxArray *collisionBoxArray);
	
	void addTankBomb(TVector startPosition, TVector startVelocity);
	void addAirPlaneBomb(TVector startPosition,TVector startVelocity, float angleWithX);
	void addAutoBomb(TVector startPosition,  float velocityMag, AutoWarObject *targetObject);
	
	
	int getTotalNumberBomb();
	void updateBombArray(float restTime);
	void checkCollision(float restTime);
	void draw();
};

#endif