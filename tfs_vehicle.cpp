/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_vehicle.cpp
*******************************************/

#include "trafficfs.h"

Vehicle::Vehicle()
{
}

Vehicle::Vehicle(string lc, float sp, directionType dir, float ds)
{
	this->license   = lc;
	this->speed     = sp;
	this->direction = dir;
	this->distance  = ds;

	this->oldSpeed  = sp;

	this->left  = NULL;
	this->right = NULL;
}

void Vehicle::assign(string lc, float sp, directionType dir, float ds)
{
	this->license   = lc;
	this->speed     = sp;
	this->direction = dir;
	this->distance  = ds;

	this->oldSpeed  = sp;
}
