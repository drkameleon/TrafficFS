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

float Utils::metersTravelled(float speed, int interval)
{
	float m_per_sec = (float)((speed*1000)/3600);

	return (float)(interval * m_per_sec);
}

directionType Utils::oppositeDirection(directionType dir)
{
	if (dir==ALPHA) return BETA;
	if (dir==BETA) return ALPHA;
}

directionType Utils::charToDir(char ch)
{
	if (toupper(ch)=='A') return ALPHA;
	if (toupper(ch)=='B') return BETA;
}

char Utils::dirToChar(directionType dir)
{
	if (dir==ALPHA) return 'A';
	if (dir==BETA) return 'B';
}

bool Utils::getYesNo(string msg)
{
	char answer = 'a';

	while ((toupper(answer)!='Y')&&(toupper(answer)!='N'))
	{
		showWarning(msg);
		cin >> answer;
	}

	if (toupper(answer)=='Y') 
		return true;
	else 
		return false;
}

void Utils::showWarning(string msg)
{
	if (msg!="")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout << "!! " << msg;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}

void Utils::showInfo(string msg)
{
	if (msg!="")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		cout << "** " << msg;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}
