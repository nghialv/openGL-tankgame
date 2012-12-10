/*
 *  CollisionBoxArray.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CollisionBoxArray.h"

CollisionBoxArray::CollisionBoxArray()
{
}

CollisionBoxArray::~CollisionBoxArray()
{
	//khong can tai vi day chi la tap hop con tro tro toi cac doi tuong da co san ham destructor
	//for (unsigned int i=0; i<collisionBoxArray.size(); i++) {
	//	delete collisionBoxArray[i];
	//}
}

void CollisionBoxArray::addCollisionBox(CollisionBox *collisionBox)
{
	collisionBoxArray.push_back(collisionBox);
	printf("Number of collisionBox is %d\n", getCollisionBoxNumber());
}

int CollisionBoxArray::getCollisionBoxNumber()
{
	return collisionBoxArray.size();
}

CollisionBox* CollisionBoxArray::getCollisionBox(int number)
{
	if(number>=0 && number < collisionBoxArray.size())
		return collisionBoxArray[number];
	return NULL;
}
void CollisionBoxArray::minusTimeLifeCollBox(unsigned int collBoxNum, int minusTimeDelta)
{
	collisionBoxArray[collBoxNum]->minusTimeLife(minusTimeDelta);
}

void CollisionBoxArray::draw()
{
	for (unsigned int i=0; i<collisionBoxArray.size(); i++) {
		collisionBoxArray[i]->draw();
	}
}