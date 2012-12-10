/*
 *  CollisionObject.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CollisionBox.h"

CollisionBox::CollisionBox()
{
	totalTimeLife = 0;
	remainTimeLife =0;
	xNormal = TVector(1.0, 0.0, 0.0);
	yNormal = TVector(0.0, 1.0, 0.0);
	zNormal = TVector(0.0, 0.0, 1.0);
	angleWithX = 0.0;
	life = false;
}

CollisionBox::~CollisionBox()
{
}

void CollisionBox::setTimeLife(int totalTime)
{
	remainTimeLife = totalTimeLife = totalTime;
	life = true;
}

void CollisionBox::setParameters(TVector center, float angleWithX, float xLong, float yHeight, float zWidth)
{
	this->center = center;
	this->angleWithX = angleWithX;
	this->xLong = xLong;
	this->yHeight = yHeight;
	this->zWidth = zWidth;
	
	radAngle = M_PI*angleWithX/180.0;

	xNormal = TVector(cos(radAngle), 0.0, -sin(radAngle));
	yNormal = TVector(0.0, 1.0, 0.0);
	zNormal = TVector(sin(radAngle), 0.0, cos(radAngle));


	collisionSquare[0].setParameters(yNormal,  center + yNormal*yHeight );
	collisionSquare[1].setParameters(-yNormal, center - yNormal*yHeight);

	collisionSquare[2].setParameters(-xNormal, center - xNormal*xLong);
	collisionSquare[3].setParameters(xNormal, center + xNormal*xLong);

	collisionSquare[4].setParameters(-zNormal, center - zNormal*zWidth);
	collisionSquare[5].setParameters(zNormal, center + zNormal*zWidth);	
}

void CollisionBox::updateCollisionBoxPos(TVector center)
{
	if(life)
	{
		this->center = center;
		
		collisionSquare[0].updateCollisionSquare(yNormal, center + yNormal*yHeight);
		collisionSquare[1].updateCollisionSquare(-yNormal, center - yNormal*yHeight);

		collisionSquare[2].updateCollisionSquare(-xNormal, center - xNormal*xLong);
		collisionSquare[3].updateCollisionSquare(xNormal, center + xNormal*xLong);

		collisionSquare[4].updateCollisionSquare(-zNormal, center - zNormal*zWidth);
		collisionSquare[5].updateCollisionSquare(zNormal, center + zNormal*zWidth);	
	}
}

void CollisionBox::updateCollisionBoxAngle(float angleWithX)
{
	if(life)
	{
		this->angleWithX = angleWithX;
		
		radAngle =  M_PI*angleWithX/180.0;

		xNormal = TVector(cos(radAngle), 0.0, -sin(radAngle));
		yNormal = TVector(0.0, 1.0, 0.0);
		zNormal = TVector(sin(radAngle), 0.0, cos(radAngle));
		
		collisionSquare[0].updateCollisionSquare(yNormal, center + yNormal*yHeight);
		collisionSquare[1].updateCollisionSquare(-yNormal, center - yNormal*yHeight);

		collisionSquare[2].updateCollisionSquare(-xNormal, center - xNormal*xLong);
		collisionSquare[3].updateCollisionSquare(xNormal, center + xNormal*xLong);

		collisionSquare[4].updateCollisionSquare(-zNormal, center - zNormal*zWidth);
		collisionSquare[5].updateCollisionSquare(zNormal, center + zNormal*zWidth);	
	}
}

void CollisionBox::draw()
{	
	glEnable(GL_NORMALIZE);
	glPushMatrix();
	glTranslatef(center.X(), center.Y(), center.Z());
	glColor3f(0.8, 0.8, 0.8);
	TVector tmp1, tmp2, tmp3, tmp4;
	
	tmp1 = xNormal*xLong + yNormal*yHeight - zNormal*zWidth;
	tmp2 = xNormal*xLong + yNormal*yHeight + zNormal*zWidth;
	tmp3 = xNormal*xLong - yNormal*yHeight + zNormal*zWidth;
	tmp4 = xNormal*xLong - yNormal*yHeight - zNormal*zWidth;

	glBegin(GL_QUADS);

	glNormal3f(xNormal.X(), xNormal.Y(), xNormal.Z());
	glVertex3f(tmp1.X(), tmp1.Y(), tmp1.Z());
	glVertex3f(tmp2.X(), tmp2.Y(), tmp2.Z());
	glVertex3f(tmp3.X(), tmp3.Y(), tmp3.Z());
	glVertex3f(tmp4.X(), tmp4.Y(), tmp4.Z());

	tmp1 = -xNormal*xLong + yNormal*yHeight + zNormal*zWidth;
	tmp2 = -xNormal*xLong + yNormal*yHeight - zNormal*zWidth;
	tmp3 = -xNormal*xLong - yNormal*yHeight - zNormal*zWidth;
	tmp4 = -xNormal*xLong - yNormal*yHeight + zNormal*zWidth;

	glNormal3f(-xNormal.X(), -xNormal.Y(), -xNormal.Z());
	glVertex3f(tmp1.X(), tmp1.Y(), tmp1.Z());
	glVertex3f(tmp2.X(), tmp2.Y(), tmp2.Z());
	glVertex3f(tmp3.X(), tmp3.Y(), tmp3.Z());
	glVertex3f(tmp4.X(), tmp4.Y(), tmp4.Z());

	tmp1 = xNormal*xLong + yNormal*yHeight - zNormal*zWidth;
	tmp2 = -xNormal*xLong + yNormal*yHeight - zNormal*zWidth;
	tmp3 = -xNormal*xLong + yNormal*yHeight + zNormal*zWidth;
	tmp4 = xNormal*xLong + yNormal*yHeight + zNormal*zWidth;

	glNormal3f(yNormal.X(), yNormal.Y(), yNormal.Z());
	glVertex3f(tmp1.X(), tmp1.Y(), tmp1.Z());
	glVertex3f(tmp2.X(), tmp2.Y(), tmp2.Z());
	glVertex3f(tmp3.X(), tmp3.Y(), tmp3.Z());
	glVertex3f(tmp4.X(), tmp4.Y(), tmp4.Z());

	tmp1 = xNormal*xLong - yNormal*yHeight - zNormal*zWidth;
	tmp2 = xNormal*xLong - yNormal*yHeight + zNormal*zWidth;
	tmp3 = -xNormal*xLong - yNormal*yHeight + zNormal*zWidth;
	tmp4 = -xNormal*xLong - yNormal*yHeight - zNormal*zWidth;

	glNormal3f(-yNormal.X(), -yNormal.Y(), -yNormal.Z());
	glVertex3f(tmp1.X(), tmp1.Y(), tmp1.Z());
	glVertex3f(tmp2.X(), tmp2.Y(), tmp2.Z());
	glVertex3f(tmp3.X(), tmp3.Y(), tmp3.Z());
	glVertex3f(tmp4.X(), tmp4.Y(), tmp4.Z());
	
	tmp1 = xNormal*xLong - yNormal*yHeight + zNormal*zWidth;
	tmp2 = xNormal*xLong + yNormal*yHeight + zNormal*zWidth;
	tmp3 = -xNormal*xLong + yNormal*yHeight + zNormal*zWidth;
	tmp4 = -xNormal*xLong - yNormal*yHeight + zNormal*zWidth;

	glNormal3f(zNormal.X(), zNormal.Y(), zNormal.Z());
	glVertex3f(tmp1.X(), tmp1.Y(), tmp1.Z());
	glVertex3f(tmp2.X(), tmp2.Y(), tmp2.Z());
	glVertex3f(tmp3.X(), tmp3.Y(), tmp3.Z());
	glVertex3f(tmp4.X(), tmp4.Y(), tmp4.Z());

	tmp1 = xNormal*xLong + yNormal*yHeight - zNormal*zWidth;
	tmp2 = xNormal*xLong - yNormal*yHeight - zNormal*zWidth;
	tmp3 = -xNormal*xLong - yNormal*yHeight - zNormal*zWidth;
	tmp4 = -xNormal*xLong + yNormal*yHeight - zNormal*zWidth;

	glNormal3f(-zNormal.X(), -zNormal.Y(), -zNormal.Z());
	glVertex3f(tmp1.X(), tmp1.Y(), tmp1.Z());
	glVertex3f(tmp2.X(), tmp2.Y(), tmp2.Z());
	glVertex3f(tmp3.X(), tmp3.Y(), tmp3.Z());
	glVertex3f(tmp4.X(), tmp4.Y(), tmp4.Z());

	glEnd();

	glPopMatrix();
	glDisable(GL_NORMALIZE);
}

void CollisionBox::minusTimeLife(int minusTime)
{
	remainTimeLife -= minusTime;
	if(remainTimeLife <=0)
		life = false;
}

bool CollisionBox::testIntersection(const TVector &position, const TVector &direction, double &lamda)
{
	double lamda1=1000.0;
	double tmpLamda=1000.0;
	for (unsigned int i=0; i<6; i++) {
		//if(collisionSquare[i].testIntersection(position, direction, tmpLamda))
		if(testSquareIntersection(position, direction, i, tmpLamda))
		{
			if(tmpLamda < lamda1)
				lamda1 = tmpLamda;
		}
	}
	
	if(lamda1 != 1000.0)
	{
		lamda = lamda1;
		return true;
	}
	return false;
}

bool CollisionBox::testSquareIntersection(const TVector &position, const TVector &direction, unsigned int squareNumber, double &lamda)
{
	double DotProduct=direction.dot(collisionSquare[squareNumber].normal);
	double l2;
	if(DotProduct >= 0.0)			//chi xet nhung vat di tu huong doi dien mat phang toi
		return false;
	
    //determine if ray paralle to plane
    if ((DotProduct<ZERO)&&(DotProduct>-ZERO)) 
		return false;
	
    l2=(collisionSquare[squareNumber].normal.dot(collisionSquare[squareNumber].center-position))/DotProduct;		//if direction is unit vector then l2 is distant from current position to collision
														//if direction is velocity then l2 is time from currunt position to collision
	
    if (l2<-ZERO)		
		return false;
	if(!checkPointInSquare(squareNumber, position + direction*l2))
		return false;
	lamda=l2;
    return true;
}

bool CollisionBox::checkPointInSquare(unsigned int squareNumber, TVector point)
{
	TVector tmp = point - collisionSquare[squareNumber].center;

	if(tmp.dot(xNormal) > xLong)
		return false;
	if(tmp.dot(xNormal) < -xLong)
		return false;
	if(tmp.dot(yNormal) > yHeight)
		return false;
	if(tmp.dot(yNormal) <-yHeight)
		return false;
	if(tmp.dot(zNormal) > zWidth)
		return false;
	if(tmp.dot(zNormal) <-zWidth)
		return false;
	return true;
}
