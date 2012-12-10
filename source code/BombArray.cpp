/*
 *  BombArray.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BombArray.h"

BombArray::BombArray()
{
	leastNotLifeBomb = -1;
	groundCollSquare = NULL;
	collisionBoxArray=NULL;
}

BombArray::~BombArray()
{
	for(unsigned int i=0; i< bombArray.size(); i++)
	{
		delete bombArray[i];
	}
}

void BombArray::setParForBombType(unsigned int bombType, TVector accele, const char *explosionFile, const char *soundFileName)
{
	strcpy(this->bombExplosionFile[bombType], explosionFile);
	strcpy(this->bombSoundFile[bombType], soundFileName);
	this->bombAccele[bombType] = accele;
}

void BombArray::setParForCollision(CollisionSquare *groundCollSquare, CollisionBoxArray *collisionBoxArray)
{
	this->groundCollSquare = groundCollSquare;
	this->collisionBoxArray = collisionBoxArray;
}

void BombArray::caculateLeastNotLifeBomb(unsigned int bombType)
{
	leastNotLifeBomb = -1;
	for (unsigned int i=0; i< bombArray.size(); i++) {
		if ((!bombArray[i]->getLifeStatus()) && (!bombArray[i]->getExplosionLifeStatus()) )
		{
			if(bombArray[i]->getBombType() == bombType)
			{
				leastNotLifeBomb = i;
				return;
			}
		}
	}
}

void BombArray::addTankBomb(TVector startPosition, TVector startVelocity)
{
	printf("total numbet bomb is %d\n", bombArray.size());
	caculateLeastNotLifeBomb(tankBomb_Type);
	if (leastNotLifeBomb>=0)		//has bomb not life
	{
		bombArray[leastNotLifeBomb]->setParameters(bombAccele[tankBomb_Type], bombExplosionFile[tankBomb_Type], bombSoundFile[tankBomb_Type]);
		bombArray[leastNotLifeBomb]->startMove(startPosition, startVelocity);
	}
	else {							//has not bomb not life
		printf("Them bomb moi \n");
		BombTank *newBomb = new BombTank;
		newBomb->setParameters(bombAccele[tankBomb_Type], bombExplosionFile[tankBomb_Type], bombSoundFile[tankBomb_Type]);
		newBomb->startMove(startPosition, startVelocity);
		bombArray.push_back(newBomb);
	}
}


void BombArray::addAirPlaneBomb(TVector startPosition, TVector startVelocity, float angleWithX)
{
	printf("total numbet bomb is %d\n", bombArray.size());
	caculateLeastNotLifeBomb(airPlaneBomb_Type);
	if (leastNotLifeBomb>=0)		//has bomb not life
	{
		bombArray[leastNotLifeBomb]->setAngleWithX(angleWithX);
		bombArray[leastNotLifeBomb]->setParameters(bombAccele[airPlaneBomb_Type], bombExplosionFile[airPlaneBomb_Type], bombSoundFile[airPlaneBomb_Type]);
		bombArray[leastNotLifeBomb]->startMove(startPosition, startVelocity);
	}
	else {							//has not bomb not life
		printf("Them bomb moi \n");
		BombAirPlane *newBomb = new BombAirPlane;
		newBomb->setParameters(bombAccele[airPlaneBomb_Type], bombExplosionFile[airPlaneBomb_Type], bombSoundFile[airPlaneBomb_Type]);
		newBomb->setMqoFile("Data/Mqo/AirPlaneBomb/airPlaneBomb.mqo");
		newBomb->setAngleWithX(angleWithX);
		newBomb->initBombAirPlane();
		newBomb->startMove(startPosition, startVelocity);
		bombArray.push_back(newBomb);
	}
}


void BombArray::addAutoBomb(TVector startPosition, float velocityMag, AutoWarObject *targetObject)
{
	printf("total numbet bomb is %d\n", bombArray.size());
	caculateLeastNotLifeBomb(autoBomb_Type);
	if (leastNotLifeBomb>=0)		//has bomb not life
	{
		bombArray[leastNotLifeBomb]->setTargetObject(targetObject);
		bombArray[leastNotLifeBomb]->setVelocityMag(velocityMag);
		bombArray[leastNotLifeBomb]->setParameters(bombAccele[autoBomb_Type], bombExplosionFile[autoBomb_Type], bombSoundFile[autoBomb_Type]);
		bombArray[leastNotLifeBomb]->startMove(startPosition, TVector(1.0, 1.0,0.0));
	}
	else {							//has not bomb not life
		printf("Them bomb moi \n");
		AutoBomb *newBomb = new AutoBomb;
		newBomb->setTargetObject(targetObject);
		newBomb->setVelocityMag(velocityMag);
		newBomb->setParameters(bombAccele[autoBomb_Type], bombExplosionFile[autoBomb_Type], bombSoundFile[autoBomb_Type]);
		newBomb->setMqoFile("Data/Mqo/AutoBomb/autoBomb.mqo");
		newBomb->initAutoBomb();
		newBomb->startMove(startPosition, TVector(1.0, 1.0,0.0));
		bombArray.push_back(newBomb);
	}
}

int BombArray::getTotalNumberBomb()
{
	return bombArray.size();
}

void BombArray::updateBombArray(float restTime)
{
	for (unsigned int i=0; i<bombArray.size(); i++ )
	{
		if(bombArray[i]->getLifeStatus())
		{
			if(bombArray[i]->getBombType() == autoBomb_Type)
				bombArray[i]->updateBombDirection();
			bombArray[i]->updateVelocity(restTime);
			bombArray[i]->updatePosition(restTime);
		}
	}
}

void BombArray::checkCollision(float restTime)
{
	TVector norm,uveloc;
	TVector normal,point;  //point is point collision 
	//normal is normal of plane at point collision
	
	double lamda;       //smallest time from current to collision
	double RestTime;
	double rt,rt2,rt4;
	unsigned int bombNumber=-1;
	unsigned int collisionBoxNum =-1, collBNum;
	Bomb *curentBomb;
	RestTime=restTime;
	
	//bombTank.updateVelocity(RestTime);   //update new velocity for ball after RestTime
	while (RestTime > ZERO)
	{
		lamda = 10000.0;
		updateBombArray(RestTime);
		
		for (unsigned int i=0; i<getTotalNumberBomb(); i++)
		{
			curentBomb = bombArray[i];
			if (curentBomb->getLifeStatus())
			{
				rt2 = curentBomb->getOldPosition().dist(curentBomb->getCurrentPosition());
				TVector::unit(curentBomb->getVelocity(), uveloc);
				
				if(checkCollisionWithBoxArray(curentBomb->getOldPosition(), uveloc, rt, collBNum))
				{
					rt4 = rt*RestTime/rt2;
					if (rt4<= lamda)
					{
						if (rt4<= RestTime + ZERO)
							if (! ((rt<=ZERO)&&(uveloc.dot(norm)>ZERO)) )
							{
								normal = norm;
								point = curentBomb->getOldPosition() + uveloc*rt;
								lamda = rt4;
								bombNumber = i;
								collisionBoxNum = collBNum;
							}
					}
				}
			}
		}
		
		if (lamda != 10000.0)
		{
			RestTime -= lamda;
			bombArray[bombNumber]->moveToCollisionPoint(lamda);
			if (collisionBoxNum == -1) {
				printf("ban roi xuong dat\n");
			}
			else {
				printf("ban trung doi tuong %d\n", collisionBoxNum);
				collisionBoxArray->minusTimeLifeCollBox(collisionBoxNum, bombArray[bombNumber]->getMinusLifeDelta());
			}
			for (unsigned int i=0; i<getTotalNumberBomb(); i++) {
				if (i!=bombNumber)
					if (bombArray[i]->getLifeStatus()) {
						bombArray[i]->updateVelocity(lamda);
						bombArray[i]->updatePosition(lamda);
					}
			}
			
		}
		else RestTime = 0;
	}
}

bool BombArray::checkCollisionWithBoxArray(const TVector &position, const TVector &direction, double &lamda, unsigned int &collisionBoxNum)
{
	double lamda1= 1000.0;
	double tmplamda = 1000.0;
	collisionBoxNum = -1;
	CollisionBox *currentColBox;
	
	for (unsigned int i=0; i< collisionBoxArray->getCollisionBoxNumber(); i++) {
		currentColBox = collisionBoxArray->getCollisionBox(i);
		if(currentColBox->isLife())
			if(currentColBox->testIntersection(position, direction, tmplamda))
			{
				if (tmplamda< lamda1) {
					lamda1= tmplamda;
					collisionBoxNum = i;
				}
			}
	}
	
	if(groundCollSquare->testIntersection(position, direction, tmplamda))
	{
		if (tmplamda< lamda1) {
			lamda1 = tmplamda;
			collisionBoxNum = -1;
		}
	}
	
	if (lamda1 != 1000.0) {
		lamda = lamda1;
		return true;
	}
	return false;
}

void BombArray::draw()
{
	for (unsigned int i=0; i< bombArray.size(); i++)
	{
		bombArray[i]->draw();
	}
}