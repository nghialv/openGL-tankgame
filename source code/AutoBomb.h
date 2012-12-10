#ifndef _AUTOBOMB_H_
#define _AUTOBOMB_H_


#include "IncludeS.h"
#include "Bomb.h"
#include "MqoObject.h"
#include "AutoWarObject.h"

class AutoBomb : public Bomb
{
private:
	MqoObject model;
	float angleWithX;
	float angleWithY;
	float velocityMag;
	TVector uVeloc;
	AutoWarObject *autoWarObject;
	
public:
	AutoBomb();

	void setTargetObject(AutoWarObject *autoWarObject);
	void setVelocityMag(float velocityMag);
	void setMqoFile(const char *mqoFile);
	void initAutoBomb();
	void updateBombDirection();
	void draw();
};

#endif