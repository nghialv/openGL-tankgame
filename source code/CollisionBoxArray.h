/*
 *  CollisionObjectArray.h
 *  myGame
 *
 *  Created by le van nghia on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _COLLISIONBOXARRAY_H_
#define _COLLISIONBOXARRAY_H_

#include "IncludeS.h"
#include <vector>
#include "CollisionBox.h"

class CollisionBoxArray
{
private:
	std::vector<CollisionBox *> collisionBoxArray;
public:
	CollisionBoxArray();
	~CollisionBoxArray();
	
	void addCollisionBox(CollisionBox *collisionBox);
	int getCollisionBoxNumber();
	void minusTimeLifeCollBox(unsigned int collBoxNum, int minusTimeDelta);
	CollisionBox *getCollisionBox(int number);
	void draw();
};

#endif