/*
 *  ControlWarObject.h
 *  myGame
 *
 *  Created by le van nghia on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CONTROLWAROBJECT_H_
#define _CONTROLWAROBJECT_H_

#include "IncludeS.h"
#include "Tvector.h"
#include "BombArray.h"
#include "CollisionBox.h"
#include "CollisionBoxArray.h"

class ControlWarObject
{
protected:
	TVector	position;
	float angleWithX;
	bool life;
	BombArray *bombArray;
	CollisionBox collisionBox;
	CollisionBoxArray *collisionBoxArray;
	float radius;
	
public:
	ControlWarObject();
	~ControlWarObject();
	
	void setParameters(TVector position, float angleWithX, BombArray *bombArray, CollisionBoxArray *collisionBoxArray);
	void setPosition(float x, float y, float z);
	float getRadius(){return radius;}
	TVector getPosition(){ return position;}
	float getAngleWithX(){return angleWithX;}
	void setLifeStatus(bool lifeStatus);
	bool getLifeStatus(){return life;}
	void plusPosition(float deltaX, float deltaY, float deltaZ);
	virtual void draw(){}
};

#endif