/*
 *  tank.h
 *  GlMetaseq
 *
 *  Created by le van nghia on 12/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _TANK_H_
#define _TANK_H_
#include "MqoObject.h"
#include "ControlWarObject.h"
#include "AutoWarObject.h"
#include "Terrain.h"
#include "TVector.h"

class Tank: public ControlWarObject
{
private:
	MqoObject bodyModel;
	MqoObject gunModel;
	float gunAngle;
	float gunLong;
	float radGunAngle;
	float radTankAngle;
	float bombVelocityMag;
	float autoBombVelocityMag;
	TVector bombVelocity;
	TVector bombStartPos;
	Terrain *terrain;
	
public:
	Tank();
	~Tank();
	
	void setParameters(const char *bodyFile, const char *gunFile, TVector position, float angleWithX, Terrain *terrain, float bombVelocityMag,  BombArray *bombArray, CollisionBoxArray *collisionBoxArray);
	void setGunLong(float gunLong);
	float getGunLong();
	void setAutoBombVelocityMag(float autoBombVelocityMag);
	void plusGunAngle(float deltaAngle);
	void plusAngleWithX(float deltaAngle);
	void plusBombVelocityMag(float deltaVelocity);
	void move(float deltaTranslate);
	void initTank();
	void startFight();
	void shot();
	void shotM(AutoWarObject *targetObject);
	void draw();
};

#endif