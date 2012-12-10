
/*
 *  AirPlane.h
 *  myGame
 *
 *  Created by le van nghia on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

#include "IncludeS.h"
#include "AutoWarObject.h"
#include "CollisionBox.h"
#include "CollisionBoxArray.h"
#include "BombArray.h"
#include "MqoObject.h"
#include "TVector.h"

class AirPlane: public AutoWarObject
{
private:
	MqoObject model;
	TVector bombInitVelocity;
	float angleWithX;
	float velocityMag;
	BombArray *bombArray;
	CollisionBox collisionBox;
	CollisionBoxArray *collisionBoxArray;

public:
	AirPlane();
	~AirPlane(){}

	void setParameters(const char *modelFile, TVector bombInitVelocity, BombArray *bombArray, CollisionBoxArray *collisionBoxArray);
	void initAirPlane();
	void startMove(TVector initPosition, float velocityMag, float angleWithX);
	void changeVelocityDirection(float newAngleWithX);
	void update(float restTime);
	void shot();
	void draw();
};

#endif