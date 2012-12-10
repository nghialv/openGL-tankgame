#ifndef _AUTOTANK_H_
#define _AUTOTANK_H_

#include "IncludeS.h"
#include "AutoWarObject.h"
#include "CollisionBox.h"
#include "CollisionBoxArray.h"
#include "BombArray.h"
#include "MqoObject.h"
#include "TVector.h"
#include "ControlWarObject.h"
#include "Terrain.h"
#include "time.h"

class AutoTank: public AutoWarObject
{
private:
	MqoObject model;
	float velocityMag;
	float bombVelocityMag;
	float angleWithX;
	BombArray *bombArray;
	CollisionBox collisionBox;
	CollisionBoxArray *collisionBoxArray;
	ControlWarObject *targetObject;
	Terrain *terrain;

	TVector bombVelocity;
	TVector bombStartPos;
	float bombAcceleMag;
	float radius;

	bool runAwayStatus;
	bool shotted;

	int shotTime;

	int currentSystemTime;
	int runAwayStartTime;
	int deltaTimeRunAway;

public:
	AutoTank();

	void setParameters(const char *modelFile, TVector position, float velocityMag, Terrain *terrain, BombArray *bombArray, CollisionBoxArray *collisionBoxArray);
	void setTargetObject(ControlWarObject *targetObject);
	float getRadius(){return radius;}
	void updateLifeStatus();
	void initAutoTank();
	void startFight(TVector startPositionNoNeedY, float angleWithX);
	void changeDirection(TVector targetPosition);
	void shot();
	void runAway();
	void moveTo(float plusAngle);
	void moveTo(TVector newPosition);
	void update(float restTime);
	void draw();
};

#endif