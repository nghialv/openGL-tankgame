/*
 *  BombAirPlane.h
 *  myGame
 *
 *  Created by le van nghia on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BOMBAIRPLANE_H_
#define _BOMBAITPLANE_H_

#include "IncludeS.h"
#include "Bomb.h"
#include "MqoObject.h"

class BombAirPlane : public Bomb
{
private:
	MqoObject model;
	float angleWithX;
public:
	BombAirPlane();
	
	void setMqoFile(const char *mqoFile);
	void initBombAirPlane();
	void setAngleWithX(float angleWithX);
	void draw();
};

#endif