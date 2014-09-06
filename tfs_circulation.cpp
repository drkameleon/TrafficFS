/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_circulation.cpp
*******************************************/

#include "trafficfs.h"

Circulation::Circulation()
{
	this->Vehicles = new dlList();
	this->first = Vehicles->head;
	this->last = Vehicles->tail;
}

void Circulation::addVehicle(Vehicle *ve)
{   
	Vehicle *current = this->first;
	Vehicle *newVehicle; 

	bool done = false;

	if (current==NULL)
		this->Vehicles->insertNode(ve,HEAD); 
	else
	{ 
		while ((current!=NULL)&&(!done))
		{
			if ((current->distance < ve->distance)&&(current->right==NULL))
			{ 
				this->Vehicles->insertNode(ve,AFTER,current);
				done = true;
			}
			else if (current->distance > ve->distance)
			{ 
				this->Vehicles->insertNode(ve,BEFORE,current);
				done = true;
			}
			else 
			{
				if (current->right!=NULL)
				{
					Vehicle *Right = (Vehicle *)current->right;
									
					if ((current->distance < ve->distance)&&(Right->distance > ve->distance))
					{
						this->Vehicles->insertNode(ve,AFTER,current);    
						done = true;              
					}
				}
			}
			current = (Vehicle *)current->right;
		}         
	}       

	this->first = Vehicles->head;
	this->last = Vehicles->tail;          
}

void Circulation::removeVehicle(Vehicle *ve)
{
	this->Vehicles->removeNode(ve);
}

void Circulation::updateVehiclesPosition()
{
	Vehicle *current = this->first;

	#ifdef __TFS_DEBUG
	cout << "@ In updateVehiclesPosition : " << endl;
	#endif

	while (current!=NULL)
	{
		#ifdef __TFS_DEBUG
		cout << "@ \tIn the Loop" << endl;
		#endif
		
		current->speed = current->oldSpeed;

		float distanceCovered = Utils::metersTravelled(current->speed, TFS_UPDATEINTERVAL);

		current->distance = current->distance - (current->direction * distanceCovered);

		int Overtake = this->canOvertake(current);

		#ifdef __TFS_DEBUG
		cout << "@ \t\t% Overtake = " << Overtake << endl;
		#endif

		if (Overtake == 1)
		{
			if (current->direction==ALPHA)
				this->Vehicles->insertNode((Vehicle *)current,BEFORE,(Vehicle *)current->left);
			else if (current->direction==BETA)
				this->Vehicles->insertNode((Vehicle *)current,AFTER,(Vehicle *)current->right);
		}
		else if (Overtake == -1)
		{
			current->oldSpeed = current->speed;

			if (current->direction==ALPHA)
				current->speed = ((Vehicle *)current->left)->speed;
			else if (current->direction==BETA)
				current->speed = ((Vehicle *)current->right)->speed;
		}

		current = (Vehicle *)current->right;

		#ifdef __TFS_DEBUG
		cout << "@ \t\tMoving on to the next item" << endl;
		#endif
	}
}

bool Circulation::performVehicleEntry(Vehicle *ve, Location *entryPoint)
{
	if (ve->direction==ALPHA)
	{
		if (this->getVehiclesBetween(ve->distance,ve->distance+30,ALPHA)==NULL)
			this->addVehicle(ve);
		else
		    return false;
	}
	else if (ve->direction==BETA)
	{
		if (this->getVehiclesBetween(ve->distance-30,ve->distance,BETA)==NULL)
			this->addVehicle(ve);
		else
		    return false;
	}
	
	return true;
}

bool Circulation::performVehicleExit(Vehicle *ve, Location *exitPoint)
{
	if (abs(ve->distance - exitPoint->distance)<5)
	{
		this->Vehicles->removeNode(ve);
		return true;
    }
    else
        return false;
}

bool Circulation::vehicleExists(string lc)
{
	Vehicle *current = this->first;

	while (current!=NULL)
	{
		if (current->license == lc) 
			return true;

		current = (Vehicle *)current->right;
	}

	return false;
}

void Circulation::showAll()
{
	Vehicle *current = this->first;
	int counter = 1;

	while (current!=NULL)
	{
		cout << "Vehicle [" << counter << "]\n=================\n";
		cout << TFS_LICENSE << current->license << endl;
		cout << TFS_SPEED << current->speed << endl;
		cout << TFS_DIRECTION << Utils::dirToChar(current->direction) << endl;
		cout << TFS_DISTANCE << current->distance << endl << endl;

		counter++;
		current = (Vehicle *)current->right;
	}
}

Vehicle* Circulation::getVehicleByLicense(string lc)
{
	Vehicle *current = this->first;

	while (current!=NULL)
	{
		if (current->license==lc)
			return current;

		current = (Vehicle *)current->right;
	}

	return NULL;
}

void Circulation::changeVehicleSpeed(float newSpeed, string lc)
{
	Vehicle *ve = this->getVehicleByLicense(lc);

	ve->speed = newSpeed;
}

float Circulation::metersApart(Vehicle *veA, Vehicle *veB)
{
	return (float)abs(veA->distance - veB->distance);
}

int Circulation::canOvertake(Vehicle *ve)
{
	if (ve->direction==ALPHA)
	{
		if (ve->left!=NULL)
		{
			Vehicle *Left = (Vehicle *)ve->left;

			if ((ve->speed > Left->speed)&&(this->metersApart(ve,Left) >= 4)&&(this->metersApart(ve,Left) <= 10))
			{   
				if ((this->noVehicleOnTheOppositeLane(ve))&&(this->noVehicleInFront(Left)))
					return 1;
				else
					return -1;
			}
			else
				return 0;
		}
		else 
			return 0;
	}
	else if (ve->direction==BETA)
	{
		if (ve->right!=NULL)
		{
			Vehicle *Right = (Vehicle *)ve->right;

			if ((ve->speed > Right->speed)&&(this->metersApart(ve,Right) >= 4)&&(this->metersApart(ve,Right) <= 10))
			{   
				if ((this->noVehicleOnTheOppositeLane(ve))&&(this->noVehicleInFront(Right)))
					return 1;
				else
					return -1;
			}
			else
				return 0;
		}
		else
			return 0;
	}
}

bool Circulation::noVehicleOnTheOppositeLane(Vehicle *ve)
{
	dlList *veh = this->getVehiclesBetween(ve->distance,ve->distance-(ve->direction*15),ve->direction);

	if (veh==NULL)
		return true;
	else
		return false;
}

bool Circulation::noVehicleInFront(Vehicle *ve)
{
	dlList *veh = this->getVehiclesBetween(ve->distance,ve->distance-(ve->direction*50),Utils::oppositeDirection(ve->direction));

	if (veh==NULL)
		return true;
	else
		return false;
}

bool Circulation::onTheSameLane(Vehicle *veA, Vehicle *veB)
{
	if (veA->direction==veB->direction) 
		return true;
	else 
		return false;
}

dlList* Circulation::getVehiclesBetween(float pointA, float pointB)
{
	dlList *lst = new dlList();

	Vehicle *current = this->first;

	while (current!=NULL)
	{
		if ((current->distance>=pointA)&&(current->distance<=pointB))
		{
			Vehicle *ve = new Vehicle(current->license,current->speed,current->direction,current->distance);
			lst->insertNode(ve,TAIL);
		}

		current = (Vehicle *)current->right;
	}

	return lst;
}

dlList* Circulation::getVehiclesBetween(float pointA, float pointB, directionType dir)
{
	dlList *lst = new dlList();

	Vehicle *current = this->first;

	while (current!=NULL)
	{
		if ((current->distance>=pointA)&&(current->distance<=pointB)&&(current->direction==dir))
		{
			Vehicle *ve = new Vehicle(current->license,current->speed,current->direction,current->distance);
			lst->insertNode(ve,TAIL);
		}

		current = (Vehicle *)current->right;
	}

	return lst;
}

float Circulation::getMeanSpeed(float pointA, float pointB)
{
	dlList *veh = this->getVehiclesBetween(pointA,pointB);

	Vehicle *current = veh->head;
	int cnt = 0;
	float speedTotal = 0;

	while (current!=NULL)
	{
		speedTotal += current->speed;

		cnt++;
		current = (Vehicle *)current->right;
	}

	return (float)(speedTotal/cnt);
}

int Circulation::getNoOfVehicles(float pointA, float pointB)
{
	dlList *veh = this->getVehiclesBetween(pointA,pointB);

	Vehicle *current = veh->head;
	int cnt = 0;

	while (current!=NULL)
	{          
		cnt++;
		current = (Vehicle *)current->right;
	}

	return cnt;
}

void Circulation::printChart(void)
{   
	Vehicle *current = this->first;

	string toA = "================================================================================";
	string toB = "================================================================================";

	while (current!=NULL)
	{
		int idx = (int) floor(current->distance / 10000);

		#ifdef __TFS_DEBUG
		cout << "IDX = " << idx << endl;
		#endif

		if (current->direction == ALPHA)
			toA[idx] = '<';
		else if (current->direction == BETA)
			toB[idx] = '>';

		current = (Vehicle *)current->right;
	}

	cout << toA << endl << toB << endl << endl;
}
