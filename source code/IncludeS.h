/*
 *  untitled.h
 *  myGame
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>


#define mapX 48
#define mapZ 48
#define mapScale 50.0
#define numTree 40


#define tankBomb_Type 0
#define airPlaneBomb_Type 1
#define autoBomb_Type 2

/*************************** Macros and constants ***************************/
// returns a number ranging from -1.0 to 1.0
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max);

#endif
