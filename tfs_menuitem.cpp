/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_menuitem.cpp
*******************************************/

#include "trafficfs.h"

MenuItem::MenuItem(string nm, Proc *pr)
{
	this->name = nm;
	this->proc = pr;
}
