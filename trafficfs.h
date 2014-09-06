/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : trafficfs.h
*******************************************/

#ifndef __TRAFFICFS_H_

#define __TRAFFICFS_H_

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cctype>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <sstream> 
#include <list>
#include "windows.h"

using namespace std;

typedef void Proc (void);

#define TFS_UPDATEINTERVAL 5

#define TFS_MAXDISTANCE 800

#define TFS_LOGO "\
********************************************************************************\
*                                   TrafficFS                                  *\
*                                Flow Simulator                                *\
********************************************************************************\
* version 0.2                                                                  *\
*                                                                              *\
* Coded in C++                                                                 *\
* Copyright (c) by Dr.Kameleon                                                 *\
********************************************************************************\n"

#define TFS_NOVEHICLES      "No Vehicles found! Auto-generate them? [Y/N] "
#define TFS_NOENTRYEXITS    "No Entries/Exits found! Auto-generate them? [Y/N] "
#define TFS_CONTINUEADDING  "Continue adding? [Y/N] "

#define TFS_VEHICLESAUTOGENERATION "Generating random circulation data... [  OK  ]\n\n"
#define TFS_ENTRYEXITSAUTOGENERATION "Generating random Entries/Exits...    [  OK  ]\n\n"

#define TFS_NEWVEHICLE "\nNew Vehicle\n===================\n"
#define TFS_NEWENTRY   "\nNew Entry\n===================\n"
#define TFS_NEWEXIT    "\nNew Exit\n===================\n"

#define TFS_GIVEANENTRY   "\nGive an Entry\n========================\n"
#define TFS_GIVEANEXIT    "\nGive an Exit\n========================\n"

#define TFS_CHANGEVEHICLESPEED "\nChange Vehicle Speed\n================================\n"

#define TFS_LICENSE   "License Plate   : "
#define TFS_SPEED     "Speed [km/h]    : "
#define TFS_DIRECTION "Direction [A/B] : "
#define TFS_DISTANCE  "Distance [m]    : "

#define TFS_EENAME      "Name : "
#define TFS_EEDISTANCE  "Distance [m] : "
#define TFS_EEDIRECTION "Direction [A/B] : "
#define TFS_EEWHAT      "Is that an Entry? [Y/N - type 'N' for an Exit] : "

#define TFS_NEW       "*NEW* "

#define TFS_GETMEANSPEED "\nGet Mean Speed\n============================\n"

#define TFS_POINTA "Point A [m]  : "
#define TFS_POINTB "Point B [m]  : "

#define TFS_MEANSPEED    ">> Mean Speed = "
#define TFS_NOOFVEHICLES ">> No of Vehicles = "

#define TFS_SIMULATING "-- Simulating"

enum directionType {
	ALPHA   = 1, 
	BETA    = -1
};

enum positionType {
	HEAD = 0,
	TAIL = 1
};

enum relationType {
	BEFORE = 0,
	AFTER  = 1
};

class dlNode {
	public:  
		dlNode *left;
		dlNode *right;

		dlNode();
};

class Vehicle : public dlNode
{
	public:
		string license;
		float speed;
		directionType direction;
		float distance;

		float oldSpeed;

		Vehicle();
		Vehicle(string lc, float sp, directionType dir, float ds);
		void assign(string lc, float sp, directionType dir, float ds);
};

class dlList {
	public:
		Vehicle *head;
		Vehicle *tail;

		dlList();

		void insertNode(Vehicle *newNode, positionType pos);
		void insertNode(Vehicle *newNode, relationType rel, Vehicle *node);

		void removeNode(positionType pos);
		void removeNode(Vehicle *node);
		void removeNode(relationType rel, Vehicle *node);      
};

enum locationType {
	ENTRY = 0,
	EXIT = 1
};

class Location
{
	public :
		string name;
		float distance;
		directionType direction;
		locationType location;

		Location(string nm, float ds, directionType dir, locationType loc);
};

class Circulation
{
	public:
		Vehicle *first;
		Vehicle *last;

		dlList *Vehicles;

		Circulation();
		
		void addVehicle(Vehicle *ve);
		void removeVehicle(Vehicle *ve);
		void updateVehiclesPosition();
		void showAll();

		void changeVehicleSpeed(float newSpeed, string lc);
		Vehicle* getVehicleByLicense(string lc);

		int canOvertake(Vehicle *ve);
		bool onTheSameLane(Vehicle *veA, Vehicle *veB);
		float metersApart(Vehicle *veA, Vehicle *veB);
		bool noVehicleOnTheOppositeLane(Vehicle *ve);
		bool noVehicleInFront(Vehicle *ve);
		bool vehicleExists(string lc);

		dlList* getVehiclesBetween(float pointA, float pointB);
		dlList* getVehiclesBetween(float pointA, float pointB, directionType dir);

		void changeVehicleSpeed(Vehicle *ve, float newSpeed);
		bool performVehicleEntry(Vehicle *ve, Location *entryPoint);
		bool performVehicleExit(Vehicle *ve, Location *exitPoint);
		int getNoOfVehicles(float pointA, float pointB);
		float getMeanSpeed(float pointA, float pointB);

		void printChart(void);
};

class InOut
{
      public:
             Vehicle *vehicle;
             Location *location;
};

class MenuItem
{
	public:
		string name;
		Proc* proc;
		
		MenuItem(string nm, Proc* pr);		
};

class Menu
{
	public:
		vector<MenuItem> items;
		
		Menu();
		
		void addMenuItem(MenuItem *mi);
		void show();
};

class Random
{
	public :
		Random();

		string genLicense();
		string genName();
		float genSpeed();
		directionType genDirection();
		float genDistance();
};

class Utils
{
	public:
		static float metersTravelled(float speed, int interval);

		static directionType charToDir(char ch);
		static directionType oppositeDirection(directionType dir);
		static char dirToChar(directionType dir);

		static bool getYesNo(string msg);
		static void showWarning(string msg);
		static void showInfo(string msg);
};

#endif
      
