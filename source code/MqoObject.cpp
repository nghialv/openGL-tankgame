/*
 *  Mqoobject.cpp
 *  GlMetaseq
 *
 *  Created by le van nghia on 12/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MqoObject.h"

MqoObject::MqoObject()
{
}

MqoObject::MqoObject(const char *filename, float scale)
{
	strcpy(mqoFileName, filename);
	this->scale = scale;
}

MqoObject::~MqoObject()
{
	mqoDeleteModel(mqoModel);
}

void MqoObject::setParameters(const char *filename, float scale)
{
	strcpy(mqoFileName, filename);
	this->scale = scale;
}

void MqoObject::initModel()
{
	mqoModel=mqoCreateModel(mqoFileName, scale);
}

void MqoObject::display()
{
	mqoCallModel(mqoModel);
}

void MqoObject::deleteModel()
{
	mqoDeleteModel(mqoModel);
}

