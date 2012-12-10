#include "AutoBomb.h"

AutoBomb::AutoBomb()
{
	bombType = autoBomb_Type;
	minusLifeDelta = 4;
	angleWithX = 0.0;
	angleWithY = 0.0;
	velocityMag = 0.5;
	autoWarObject = NULL;
}

void AutoBomb::setTargetObject(AutoWarObject *autoWarObject)
{
	this->autoWarObject = autoWarObject;
}

void AutoBomb::setMqoFile(const char *mqoFile)
{
	model.setParameters(mqoFile, 0.2);
}

void AutoBomb::setVelocityMag(float velocityMag)
{
	this->velocityMag = velocityMag;
}

void AutoBomb::initAutoBomb()
{
	model.initModel();
}


void AutoBomb::updateBombDirection()
{
	if(autoWarObject->getLifeStatus())
	{
		//printf("current position of autoWarObject is %2f	%2f		%2f\n", autoWarObject->getCurrentPosition().X(),  autoWarObject->getCurrentPosition().Y(),  autoWarObject->getCurrentPosition().Z());
		TVector newDirection = autoWarObject->getCurrentPosition() - currentPosition;
		TVector::unit(newDirection, uVeloc);
		velocity = uVeloc * velocityMag;
		
		if(uVeloc.X() == 0.0)
			angleWithX = M_PI/2.0;
		else
		{
			angleWithX = atan(-uVeloc.Z()/uVeloc.X());	//0->180.0
			if(angleWithX<0.0)
				angleWithX += M_PI;
		}

		angleWithY = acos(uVeloc.dot(TVector(cos(angleWithX), 0.0, -sin(angleWithX))))*180.0/M_PI;

		if(angleWithY <0.0)
			angleWithY += 180.0;
		angleWithX = angleWithX*180.0/M_PI;
	}
}

void AutoBomb::draw()
{
	if(getLifeStatus())
	{
		glPushMatrix();
		glTranslated(currentPosition.X(), currentPosition.Y(), currentPosition.Z());
		glRotatef(angleWithX, 0.0, 1.0, 0.0);
		glRotatef(angleWithY, 0.0, 0.0, 1.0);
		model.display();
		glPopMatrix();
	}
	explosion.draw();
}

