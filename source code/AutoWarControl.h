#ifndef _AUTOWARCONTROL_H_
#define _AUTOWARCONTROL_H_

#include "IncludeS.h"
#include "ControlWarObject.h"
#include "AutoTank.h"
#include "AirPlane.h"
#include "Terrain.h"
#include "time.h"

class AutoWarControl
{
private:
	ControlWarObject *targetObject;
	AirPlane *airPlane;
	float airPlaneVelocityMag;
	unsigned int airPlaneShotNumber;

	std::vector<AutoTank *> autoTankArray;

	int currentTime;
	int lastTimeAirPlaneAppear;
	int deltaTimeAirPlaneAppear;

	int lastTimeAutoTankShot;
	int deltaTimeAutoTankShot;
	float distantMax;
	
	int lastTimeCreateAutoTank;
	int deltaTimeCreateAutoTank;

public:
	AutoWarControl();

	void setParameters(ControlWarObject *targetObject);
	void setAirPlane(AirPlane *airPlane, float airPlaneVelocityMag);
	void addAutoTank(AutoTank *autoTank);
	
	void init();

	void airPlaneStart();
	void autoTankStart(int tankNumber);
	void autoTankShot(int tankNumber);

	void autoTankArrayStart();
	void autoTankArrayShot();
	
	void checkCollisionWithTank();

	void update(float restTime);
};

#endif