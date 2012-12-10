/*
 *  Bomb.h
 *  myGame
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BOMB_H_
#define _BOMB_H_

#include "IncludeS.h"
#include "AutoWarObject.h"
#include "Explosion.h"

class Bomb : public AutoWarObject
{
protected:
	unsigned int bombType;		//bomb of myTank = 1;  bomb of airplane = 2;
	unsigned int minusLifeDelta;
	Explosion explosion;
public:
	Bomb();
	~Bomb();
	
	void setParameters(TVector accle, char *explosionFile, const char *soundFileName);
	void moveToCollisionPoint(float lamda);
	void startExplosion();
	bool getExplosionLifeStatus();
	unsigned int getMinusLifeDelta(){return minusLifeDelta;}
	unsigned int getBombType(){ return bombType;}
	virtual void setAngleWithX(float angleWithX){}
	virtual void updateBombDirection(){}
	virtual void setTargetObject(AutoWarObject *autoWarObject){}
	virtual void setVelocityMag(float velocityMag){}
};

#endif