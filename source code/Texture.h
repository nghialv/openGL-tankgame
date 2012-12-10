/*
 *  texture.h
 *  myGame
 *
 *  Created by le van nghia on 10/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdio.h>
#include <string>
#include "IncludeS.h"

class Texture
{
public:
	GLubyte	*imageData;		// Image Data (Up To 32 Bits)
	GLuint	bpp;			// Image Color Depth In Bits Per Pixel.
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLuint	texID;			// Texture ID Used To Select A Texture
	GLuint  type;           // Type of image
	
	Texture();
	bool LoadTGA(char *filename);
};

#endif
