/*
 *  Explosion.h
 *  myGame
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "windows.h"
#include "IncludeS.h"
#include "Texture.h"
#include "Tvector.h"

class Explosion
{
private:
	TVector position;
	float alpha;
	float scale;
	GLint dList;
	Texture texture;
	char soundFile[100];
	bool life;

	void createTexture(char *fileName);
	void createDList();
	void resetExplosion();
	void updateExplosion();
	void playSoundFile();
	
public:
	Explosion();
	Explosion(char *explosionFile, const char *soundFileName);
	~Explosion();
	
	void initialExplosion(char *explosionFile, const char *soundFileName);		//use this function if used first contructor 
	bool getLifeStatus();
	
	void startExplosion(TVector position, float alpha, float scale);	//when start collision
	void draw();														//to update and draw explosion
};

#endif
