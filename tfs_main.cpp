/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_main.cpp
*******************************************/

#include "trafficfs.h"

Circulation *circulation;
Menu *menu;

Location  *Entries[5];
Location  *Exits[5];

list<InOut> InsAndOuts;

bool keepGoing;

void changeVehicleSpeed(void)
{
	string license;
	float newSpeed;
	directionType direction;
	float distance;

	cout << TFS_CHANGEVEHICLESPEED;
	cout << TFS_LICENSE;
	cin >> license;
	cout << TFS_SPEED;
	cin >> newSpeed;

	if (circulation->vehicleExists(license))
		circulation->changeVehicleSpeed(newSpeed, license);
	else
		Utils::showWarning("The requested Vehicle does NOT exist.");
}

int getEntryNo(string nm)
{
	for (int i=0; i<5; i++)
		if (Entries[i]->name==nm)
			return i;

	return -1;
}

int getExitNo(string nm)
{
	for (int i=0; i<5; i++)
		if (Exits[i]->name==nm)
			return i;

	return -1;
}

void forceVehicleIn(void)
{
	string name;

	cout << TFS_GIVEANENTRY;
	cout << TFS_EENAME;
	cin >> name;

	int entryNo = getEntryNo(name);

	if (entryNo!=-1)
	{
		string license;
		float speed;
		directionType direction;
		float distance;

		cout << TFS_NEWVEHICLE;
		cout << TFS_LICENSE;
		cin >> license;
		cout << TFS_SPEED;
		cin >> speed;   

		direction = Entries[entryNo]->direction;
		distance  = Entries[entryNo]->distance;
		
		Vehicle *ve = new Vehicle(license,speed,direction,distance);

		if (!circulation->performVehicleEntry(ve,Entries[entryNo]))  
		{
            InOut IO;
            
            IO.vehicle = ve;
            IO.location = Entries[entryNo];
            
            InsAndOuts.push_back(IO);
        }
	}
	else
		Utils::showWarning("The requested Entry does NOT exist");
}

void forceVehicleOut(void)
{
	string name;

	cout << TFS_GIVEANEXIT;
	cout << TFS_EENAME;
	cin >> name;

	int exitNo = getExitNo(name);

	if (exitNo!=-1)
	{
		string license;

		cout << TFS_LICENSE;
		cin >> license;
		
		Vehicle *ve = circulation->getVehicleByLicense(license);

		if (circulation->vehicleExists(license))  
		{                                                  
			if (!circulation->performVehicleExit(ve, Exits[exitNo]))
            {
                InOut IO;
                
                IO.vehicle = ve;
                IO.location = Exits[exitNo];
                
                InsAndOuts.push_back(IO);
            }                                                                                  
        }
		else
			Utils::showWarning("The requested Vehicle does NOT exist.");
	}
	else
		Utils::showWarning("The requested Exit does NOT exist");
}

void updateInsAndOuts(void)
{
    if (!InsAndOuts.empty())
    {
        list<InOut>::iterator i;
        
        for (i=InsAndOuts.begin(); i != InsAndOuts.end(); ++i)
        {
            if ((*i).location->location == ENTRY)
            {
                if (circulation->performVehicleEntry((*i).vehicle, (*i).location))
                   InsAndOuts.erase(i);                      
            }
            else if ((*i).location->location == EXIT)
            {
                if (circulation->performVehicleExit((*i).vehicle, (*i).location))
                   InsAndOuts.erase(i);
            }
        }                   
    }
}

void getMeanSpeed(void)
{
	float pointA, pointB;

	cout << TFS_GETMEANSPEED;
	cout << TFS_POINTA;
	cin >> pointA;
	cout << TFS_POINTB;
	cin >> pointB;

	cout << TFS_MEANSPEED << circulation->getMeanSpeed(pointA,pointB) << endl;
	cout << TFS_NOOFVEHICLES << circulation->getNoOfVehicles(pointA,pointB) << endl << endl;
}

void showAllVehicles(void)
{
	circulation->showAll();
}

void showCirculationDiagram(void)
{
	circulation->printChart();
}

void showAllEntriesExits(void)
{
	for (int i=0; i<5; i++)
	{
		cout << "Entry [" << i+1 << "]\n=================\n";
		cout << TFS_EENAME << Entries[i]->name << endl;
		cout << TFS_EEDISTANCE << Entries[i]->distance << endl;
		cout << TFS_EEDIRECTION << Utils::dirToChar(Entries[i]->direction) << endl << endl;
	}

	for (int i=0; i<5; i++)
	{
		cout << "Exit [" << i+1 << "]\n=================\n";
		cout << TFS_EENAME << Exits[i]->name << endl;
		cout << TFS_EEDISTANCE << Exits[i]->distance << endl;
		cout << TFS_EEDIRECTION << Utils::dirToChar(Exits[i]->direction) << endl << endl;
	}
}

void doNothing(void)
{
}

void addAVehicle(void)
{
	string license;
	float speed;
	directionType direction;
	float distance;

	cout << TFS_NEWVEHICLE;
	cout << TFS_LICENSE;
	cin >> license;
	cout << TFS_SPEED;
	cin >> speed;

	char dir;
	cout << TFS_DIRECTION;
	cin >> dir;
	direction = Utils::charToDir(dir);

	cout << TFS_DISTANCE;
	cin >> distance;

	circulation->addVehicle(new Vehicle(license, speed, direction, distance));
}

void addAnEntryExit(void)
{
	string name;
	float distance;
	directionType direction;

	for (int i=0; i<5; i++)
	{        
		cout << TFS_NEWENTRY;
		cout << TFS_EENAME;
		cin >> name;
		cout << TFS_EEDISTANCE;
		cin >> distance;

		char dir;
		cout << TFS_EEDIRECTION;
		cin >> dir;
		direction = Utils::charToDir(dir);

		Entries[i] = new Location(name, distance, direction, ENTRY);
	}

	for (int i=0; i<5; i++)
	{        
		cout << TFS_NEWEXIT;
		cout << TFS_EENAME;
		cin >> name;
		cout << TFS_EEDISTANCE;
		cin >> distance;

		char dir;
		cout << TFS_EEDIRECTION;
		cin >> dir;
		direction = Utils::charToDir(dir);

		Exits[i] = new Location(name, distance, direction, EXIT);
	}
}

void autoGenVehicles(void)
{
	Random *r = new Random();

	for (int i=0; i<10; i++)
	{
		circulation->addVehicle(
			new Vehicle(
				r->genLicense(),
				r->genSpeed(),
				r->genDirection(),
				r->genDistance()
			)
		);                   
	}
}

void autoGenEntryExits(void)
{
	Random *r = new Random();

	for (int i=0; i<5; i++)
	{        
		Entries[i] = new Location(
			r->genName(), 
			r->genDistance(), 
			r->genDirection(), 
			ENTRY
		);
	}

	for (int i=0; i<5; i++)
	{               
		Exits[i] = new Location(
			r->genName(), 
			r->genDistance(), 
			r->genDirection(), 
			EXIT
		);
	}
}

void terminateApp(void)
{
	keepGoing = false;
}

int main(int argc, char *argv[])
{
	circulation = new Circulation();

	menu =  new Menu();
	menu->addMenuItem(new MenuItem("Change Vehicle Speed",(Proc *)changeVehicleSpeed));
	menu->addMenuItem(new MenuItem("Force a Vehicle IN",(Proc *)forceVehicleIn));
	menu->addMenuItem(new MenuItem("Force a Vehicle OUT",(Proc *)forceVehicleOut));
	menu->addMenuItem(new MenuItem("Get mean speed / No of Vehicles",(Proc *)getMeanSpeed));
	menu->addMenuItem(new MenuItem("Show all Vehicles",(Proc *)showAllVehicles));
	menu->addMenuItem(new MenuItem("Show all Entries/Exits",(Proc *)showAllEntriesExits));
	menu->addMenuItem(new MenuItem("Show Circulation Diagram",(Proc *)showCirculationDiagram));
	menu->addMenuItem(new MenuItem("- Continue Simulation -",(Proc *)doNothing));
	menu->addMenuItem(new MenuItem("- Exit -",(Proc *)terminateApp));

	cout << TFS_LOGO;

	bool continueAdding;

	if (!Utils::getYesNo(TFS_NOVEHICLES))
	{
		continueAdding = true;

		while (continueAdding)
		{
			addAVehicle();

			if (!Utils::getYesNo(TFS_CONTINUEADDING))
				continueAdding = false;
		}        
	}
	else
	{
		autoGenVehicles();
		Utils::showInfo(TFS_VEHICLESAUTOGENERATION);
	}

	if (!Utils::getYesNo(TFS_NOENTRYEXITS))
	{
		continueAdding = true;

		while (continueAdding)
		{
			addAnEntryExit();

			if (!Utils::getYesNo(TFS_CONTINUEADDING))
				continueAdding = false;
		}        
	}
	else
	{
		autoGenEntryExits();
		Utils::showInfo(TFS_ENTRYEXITSAUTOGENERATION);
	}

	int secs=0;

	cout << TFS_SIMULATING;

	keepGoing = true;

	while (keepGoing)
	{
		cout << ".";
		Sleep(1000);

		if (++secs%TFS_UPDATEINTERVAL==0)
		{
			circulation->updateVehiclesPosition();
			updateInsAndOuts();
			
			menu->show();

			if (keepGoing)
				cout << TFS_SIMULATING;
		}
	}

	system("PAUSE");
	return EXIT_SUCCESS;
}
