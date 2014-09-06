/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_menu.cpp
*******************************************/

#include "trafficfs.h"

Menu::Menu()
{
}

void Menu::addMenuItem(MenuItem *mi)
{
	MenuItem mt(mi->name, mi->proc);

	this->items.push_back(mt);
}
 
void Menu::show()
{
	cout << endl << endl << "==< MENU >==========================" << endl;

	for (int i=0; i<this->items.size(); i++)
	{
		cout << i+1 << " : " << this->items[i].name << endl;
	}

	int choice = -1;

	while ((choice<1)||(choice>this->items.size()))
	{
		cout << endl << ">> Pick an option [1-" << this->items.size() << "] : ";
		cin >> choice;
	}

	this->items[choice-1].proc();
}
