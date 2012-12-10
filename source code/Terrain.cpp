/*
 *  terrain.cpp
 *  cacti
 *
 *  Created by le van nghia on 10/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
	if (sandTexture.texID)
		glDeleteTextures(1, &sandTexture.texID);
	if (treeTexture.texID) 
		glDeleteTextures(1, &treeTexture.texID);
}

void Terrain::loadTexture(char *filename, Texture &texture)
{
	glGenTextures(1, &texture.texID);
	glBindTexture(GL_TEXTURE_2D, texture.texID);
	
	texture.LoadTGA(filename);
	
	//set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0,texture.type , texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, texture.type, texture.width, texture.height, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
	
	free(texture.imageData);
	printf("width  %i  height %i\n",texture.width,texture.height);
	
}

void Terrain::initializeTerrain(char *sandFileName, char *treeFileName)  //initialize vertex array
{
	int index=0;
	int currentVertex;
	int x,z;
	
	//load texture to memory
	loadTexture(sandFileName, sandTexture);
	loadTexture(treeFileName, treeTexture);
	
	for (z=0; z<mapZ; z++) {
		for (x=0; x<mapX; x++) {
			
			terrain[x+z*mapX][0] = float(x)*mapScale;
			terrain[x+z*mapX][1] = 5.0 + FRAND *3.0;
			terrain[x+z*mapX][2] = -float(z)*mapScale;
			
			currentVertex = z*mapX +x;
			
			//printf(" %f   %f   %f\n",terrain[currentVertex][0],terrain[currentVertex][1],terrain[currentVertex][2]);
			
			colorArray[currentVertex][0] = colorArray[currentVertex][1]=colorArray[currentVertex][2]=terrain[x+z*mapX][1]/20.0 +0.5;
			
			texcoordArray[currentVertex][0]=(float)x;
			texcoordArray[currentVertex][1]=(float)z;
			
		}
	}
	
	//loop over all vertices in the terrain map
	for (z=0; z<mapZ-1; z++) {
		for (x=0; x<mapX; x++) {
			currentVertex=z*mapX +x;
			indexArray[index++] = currentVertex +mapX;
			indexArray[index++] = currentVertex;
		}
	}
}

void Terrain::drawSand()
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, terrain);
	glColorPointer(3, GL_FLOAT, 0, colorArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoordArray);

	//select the sand texture
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, sandTexture.texID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// loop through all the triangle strips
	for (int z = 0; z < mapZ-1; z++)
	{
		// draw the triangles in this strip
		glDrawElements(GL_TRIANGLE_STRIP, mapX * 2, GL_UNSIGNED_INT, &indexArray[z * mapX * 2]);
	}

	glDisable(GL_TEXTURE_2D);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Terrain::drawTree()
{
	glEnable(GL_TEXTURE_2D);
	
	//make sure the random numbers we generate are the same every time
	srand(100);
	//make sure the transparent part of the texture isn't drawn
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	
	//get the modelview matrix
	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	
	//get the right and up vectors
	TVector right(mat[0],mat[4],mat[8]);
	TVector up(mat[1],mat[5],mat[9]);
	
	
	//select the cactus texture
	glBindTexture(GL_TEXTURE_2D, treeTexture.texID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	//draw all cacti
	glBegin(GL_QUADS);
	for (int n=0; n<numTree; n++) {
		//randomly size the cactus
		float size = 20.0 + FRAND*5.0;
		
		//pick a random position on the map
		TVector pos(RAND_COORD((mapX-1)*mapScale),0.0,-RAND_COORD((mapZ-1)*mapScale));
		pos.setY(GetHeight(pos.X(), pos.Z()) + size -0.5);
		
		TVector tmp= pos + (right + up) * (-size);
		// bottom left corner
		glTexCoord2f(0.0, 0.0); glVertex3f(tmp.X(), tmp.Y(), tmp.Z());
		
		// bottom right corner
		tmp = pos + (right - up) * size;
		glTexCoord2f(1.0, 0.0); glVertex3f(tmp.X(), tmp.Y(), tmp.Z());
		
		// top right corner
		tmp = pos + (right + up) * size;
		glTexCoord2f(1.0, 1.0); glVertex3f(tmp.X(), tmp.Y(), tmp.Z());
		
		// top left corner
		tmp = pos + (up - right) * size;
		glTexCoord2f(0.0, 1.0); glVertex3f(tmp.X(), tmp.Y(), tmp.Z());
	}
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}

void Terrain::drawTerrain()
{
	drawSand();
	drawTree();
}

float Terrain::GetHeight(float x, float z)
{
	// divide by the grid-spacing if it is not 1
	float projCameraX = x / mapScale;
	float projCameraZ = -z / mapScale;
	
	// compute the height field coordinates (hflCol0, hflRow0) and
	// (hflCol1, hflRow1) that identify the height field cell directly below the camera.
	int hflCol0 = int(projCameraX);
	int hflRow0 = int(projCameraZ);
	int hflCol1 = hflCol0 + 1;
	int hflRow1 = hflRow0 + 1;
	
	// get the four corner heights of the cell from the height field
	float h00 = terrain[hflCol0 + hflRow0*mapX][1];
	float h01 = terrain[hflCol1 + hflRow0*mapX][1];
	float h11 = terrain[hflCol1 + hflRow1*mapX][1];
	float h10 = terrain[hflCol0 + hflRow1*mapX][1];
	
	// calculate the position of the camera relative to the cell.
	// note, that 0 <= tx, ty <= 1.
	float tx = projCameraX - float(hflCol0);
	float ty = projCameraZ - float(hflRow0);
	
	// the next step is to perform a bilinear interpolation to compute the height
	// of the terrain directly below the object.
	float txty = tx * ty;
	
	return h00 * (1.0f - ty - tx + txty)
	+ h01 * (tx - txty)
	+ h11 * txty
	+ h10 * (ty - txty);
} // end GetHeight()