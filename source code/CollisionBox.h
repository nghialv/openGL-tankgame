/*
 *  CollisionObject.h
 *  myGame
 *
 *  Created by le van nghia on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _COLLISIONBOX_H_
#define _COLLISIONBOX_H_

#include "IncludeS.h"
#include "CollisionSquare.h"


class CollisionBox
{
private:
	int remainTimeLife;
	int totalTimeLife;
	bool life;
	TVector center;

	TVector xNormal;
	TVector yNormal;
	TVector zNormal;

	float angleWithX;
	float radAngle;

	float xLong;
	float yHeight;
	float zWidth;

	CollisionSquare collisionSquare[6];
	
	bool checkPointInSquare(unsigned int squareNumber, TVector point);
	bool testSquareIntersection(const TVector &position, const TVector &direction, unsigned int squareNumber, double &lamda);
public:
	CollisionBox();
	~CollisionBox();				
	
	void setTimeLife(int totalTime);
	void updateCollisionBoxPos(TVector center);
	void updateCollisionBoxAngle(float angleWithX);
	void setParameters(TVector center, float angleWithX, float xLong, float yHeight, float zWidth);
	void minusTimeLife(int minusTime);
	bool  isLife(){return life;}
	bool testIntersection(const TVector &position, const TVector &direction, double &lamda);
	void draw();
};

#endif