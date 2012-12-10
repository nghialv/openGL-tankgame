/*
 *  camera.cpp
 *  RunningCarAnimation
 *
 *  Created by le van nghia on 10/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "Camera.h"

Camera::Camera()
	:eye(0.0,0.0,0.0),
	 target(0.0,0.0,-100.0),
	 up(0.0,1.0,0.0)
{
}

Camera::Camera(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz)
	:eye(ex,ey,ez),
	 target(cx,cy,cz),
	 up(ux,uy,uz)
{
}

void Camera::changePosition(const TVector &newEye)
{
	eye=newEye;
}

void Camera::changePosition(float eye_x, float eye_y, float eye_z)
{
	changePosition(TVector(eye_x,eye_y,eye_z));
}

void Camera::changeTarget(const TVector &newTarget)
{
	target=newTarget;
}

void Camera::changeTarget(float target_x, float target_y, float target_z)
{
	changeTarget(TVector(target_x,target_y,target_z));
}

void Camera::changeUpDirection(const TVector &newUp)
{
	up=newUp;
}
void Camera::changeUpDirection(float up_x, float up_y, float up_z)
{
	changeUpDirection(TVector(up_x,up_y,up_z));
}

void Camera::view()
{
	gluLookAt(eye.X(), eye.Y(), eye.Z(), target.X(), target.Y(), target.Z(), up.X(), up.Y(), up.Z());
}
