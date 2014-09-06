/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_location.cpp
*******************************************/

#include "trafficfs.h"

Location::Location(string nm, float ds, directionType dir, locationType loc)
{
    this->name = nm;
    this->distance = ds;
    this->direction = dir;
	this->location = loc;
}
