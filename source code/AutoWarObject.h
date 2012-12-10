/*
 *  AutoWarObject.h
 *  myGame
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AUTOWAROBJECT_
#define _AUTOWAROBJECT_

#include "IncludeS.h"
#include "Tvector.h"

class AutoWarObject
{
protected:
	TVector accele;				//accelerate of object
	TVector velocity;			//velocity of object
	TVector currentPosition;	//current position of object
	TVector oldPosition;		//old position of object
	bool life;
	
public:
	AutoWarObject();
	~AutoWarObject();
	
	void setParameters(TVector initAccele);
	void setPosition(float x, float y, float z);
	void setLifeStatus(bool lifeStatus);
	void startMove(TVector initPosition, TVector initVelocity);
	bool getLifeStatus(){return life;}
	TVector getVelocity(){return velocity;}
	TVector getCurrentPosition(){return currentPosition;}
	TVector getOldPosition(){return oldPosition;}
	void updateVelocity(float restTime);
	void updatePosition(float restTime);
	virtual void draw()=0;
};

#endif
