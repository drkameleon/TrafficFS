/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_utils.cpp
*******************************************/

#include "trafficfs.h"

Random::Random()
{
	unsigned seed = time (0);
	srand (seed);
}

string Random::genLicense()
{
	const char* const plateLetters = "ABEHIKMNOPTXYZ";
	string output = "";

	for (int i=0; i<3; i++)
	{
		int r = rand()%14;
		output += plateLetters[r];
	}

	for (int i=0; i<4; i++)
	{
		int r = (rand()%9)+1;

		stringstream out;
		out << r;

		output += out.str();
	}

	return output;
}

string Random::genName()
{
	const char* const Letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string output = "";

	for (int i=0; i<5; i++)
	{
		int r = rand()%26;
		output += Letters[r];
	}

	return output;
}

float Random::genSpeed()
{
	return (rand()%100)+40;
}

directionType Random::genDirection()
{
	int r = rand()%2;

	if (r==1) 
		return ALPHA;
	else 
		return BETA;
}

float Random::genDistance()
{
	return ((rand()%TFS_MAXDISTANCE)+5)*1000;
}
