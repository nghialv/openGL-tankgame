/*
 *  CollisionSquare.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CollisionSquare.h"

CollisionSquare::CollisionSquare()
{
}

CollisionSquare::CollisionSquare(TVector normal, TVector center)
{
	this->normal = normal;
	this->center = center;
}

void CollisionSquare::updateCollisionSquare(TVector normal, TVector center)
{
	this->center = center;
	this->normal = normal;
}

void CollisionSquare::setParameters(TVector normal, TVector center)
{
	this->normal = normal;
	this->center = center;
}

bool CollisionSquare::testIntersection(const TVector &position, const TVector &direction, double &lamda)
{
	double DotProduct=direction.dot(normal);
	double l2;
	if(DotProduct >= 0.0)			//chi xet nhung vat di tu huong doi dien mat phang toi
		return false;
	
    //determine if ray paralle to plane
    if ((DotProduct<ZERO)&&(DotProduct>-ZERO)) 
		return false;
	
    l2=(normal.dot(center-position))/DotProduct;		//if direction is unit vector then l2 is distant from current position to collision
														//if direction is velocity then l2 is time from currunt position to collision
	
    if (l2<-ZERO)		
		return false;
	
	lamda=l2;
    return true;
}