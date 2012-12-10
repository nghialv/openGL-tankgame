/*
 *  BombTank.h
 *  myGame
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BOMBTANK_H_
#define _BOMBTANK_H_

#include "IncludeS.h"
#include "Bomb.h"

class BombTank : public Bomb
{
public:
	BombTank();
	
	void draw();
};

#endif