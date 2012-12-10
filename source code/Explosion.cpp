/*
 *  Explosion.cpp
 *  cacti
 *
 *  Created by le van nghia on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Explosion.h"

Explosion::Explosion()
{
	life = false;
}

Explosion::Explosion(char *explosionFile, const char *soundFileName)
{
	strcpy(soundFile, soundFileName);
	resetExplosion();
	createTexture(explosionFile);
	createDList();
}

Explosion::~Explosion()
{
	glDeleteTextures(1, &texture.texID);
}

void Explosion::createTexture(char *fileName)
{
	texture.LoadTGA(fileName);
	
	glGenTextures(1, &texture.texID);					// Generate OpenGL texture IDs
	
	glBindTexture(GL_TEXTURE_2D,texture.texID);			// Bind Our Texture       Ket not voi texture
	printf("dang doc\n");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	printf("bpp = %i\n",texture.width);
	
	glTexImage2D(GL_TEXTURE_2D, 0, texture.type, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
	
	free(texture.imageData);
	
}

void Explosion::createDList()
{
	float t=1.0;
	
	glNewList(dList=glGenLists(1), GL_COMPILE);
    glBegin(GL_QUADS);
	glRotatef(-45,0,1,0);
	
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-t, -t, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(t, -t, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(t, t, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-t, t, 0.0);
	
    glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-t, t, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(t, t, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(t, -t,0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-t,-t,0.0);
	
	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0,-t,t);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0,-t,-t);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0,t,-t);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0,t,t);
    
	glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0,t,t);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0,t,-t);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0,-t,-t);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0,-t,t);
	
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(t,0.05,t);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(t,0.05,-t);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-t,0.05,-t);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-t,0.05,t);
	
	glEnd();
    glEndList();	
}

void Explosion::resetExplosion()
{
	alpha = 0.0;
	scale = 1.0;
	life = false;
}

void Explosion::updateExplosion()
{
	if(alpha >= 0.8)
	{
		alpha -= 0.001;
		scale += 0.09;
	}
	else
		if (alpha >= 0.6)
		{
			alpha -= 0.001;
			scale += 0.003;
		}
		else
			if (alpha >= 0.4)
				alpha -= 0.001;
			else
				if (alpha >= 0.0)
				{
					alpha -= 0.005;
					scale -= 0.01;
				}
				else
					life = false;
}

void Explosion::playSoundFile()
{
	//playsound Function
	//PlaySound(soundFile, NULL, SND_FILENAME | SND_NOSTOP | SND_ASYNC);
	PlaySound(soundFile, NULL, SND_FILENAME | SND_ASYNC);
}

void Explosion::initialExplosion(char *explosionFile, const char *soundFileName)
{
	strcpy(soundFile, soundFileName);
	resetExplosion();
	createTexture(explosionFile);
	createDList();
}

bool Explosion::getLifeStatus()
{
	return life;
}

void Explosion::startExplosion(TVector position, float alpha, float scale)
{
	this->position = position;
	this->alpha = alpha;
	this->scale = scale;
	life = true;
	playSoundFile();
}

void Explosion::draw()
{
	if(life)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, texture.texID);
		
		updateExplosion();
		
		glPushMatrix();
		glColor4f(1.0, 1.0, 0.0, alpha);
		glScalef(scale, scale, scale);
		glTranslatef((float)position.X()/scale, (float)position.Y()/scale, (float)position.Z()/scale);
		glCallList(dList);
		glPopMatrix();
		
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}

