/*
 *  camera.h
 *  myGame
 *
 *  Created by le van nghia on 10/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "IncludeS.h"
#include "Tvector.h"

class Camera
{
private:
	TVector eye,target,up;
public:
	Camera();
	Camera(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz);
	~Camera(){}
	
	//change eye point
	void changePosition(const TVector &newEye);
	void changePosition(float eye_x, float eye_y, float eye_z);
	
	//change target point
	void changeTarget(const TVector &newTarget);
	void changeTarget(float target_x, float target_y, float target_z);
	
	//change up direction point
	void changeUpDirection(const TVector &newUp);
	void changeUpDirection(float up_x, float up_y, float up_z);
	
	float getEyeY(){return eye.Y();}
	float getEyeX(){return eye.X();}
	float getEyeZ(){return eye.Z();}
	void view();
};

#endif