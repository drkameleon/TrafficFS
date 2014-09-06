/******************************************
*  TrafficFS
*  v0.2
*  ----------------------------
*  Traffic Flow Simulator
*  
*  Coded in C++
*  Copyright (c) 2010 by Dr.Kameleon
*******************************************
*  file : tfs_dllist.cpp
*******************************************/

#include "trafficfs.h"


dlList::dlList()
{
	this->head = NULL;
	this->tail = NULL;               
}

void dlList::insertNode(Vehicle *newNode, positionType pos)
{    
	if (pos==HEAD)
	{
		if (this->head==NULL)
		{                                           
			this->head = newNode;
			this->tail = newNode;

			newNode->left  = NULL;
			newNode->right = NULL;        
		}
		else
			return insertNode(newNode, BEFORE, this->head);
	}
	else if (pos==TAIL)
	{
		if (this->tail==NULL)
			return insertNode(newNode, HEAD);
		else
			return insertNode(newNode, AFTER, this->tail);        
	}
}

void dlList::insertNode(Vehicle *newNode, relationType rel, Vehicle *node)
{   
	if (rel==BEFORE)
	{
		newNode->left = node->left;
		newNode->right = node;

		if (node->left==NULL)
			this->head = newNode;

		node->left = newNode;              
	}
	else if (rel==AFTER)
	{
		newNode->right = node->right;
		newNode->left = node;

		if (node->right==NULL)
			this->tail = newNode;

		node->right = newNode;
	}
}

void dlList::removeNode(positionType pos)
{
	if (pos==HEAD)
		removeNode(this->head);         
	else if (pos==TAIL)
		removeNode(this->tail);
}

void dlList::removeNode(Vehicle *node)
{
	if (node==this->head)
	{
		this->head = (Vehicle *)this->head->right;
		this->head->left = NULL;                     
	}
	else if (node==this->tail)
	{
		this->tail = (Vehicle *)this->tail->left;
		this->tail->right = NULL;         
	}
	else
	{
		node->right->left = node->left;
		node->left->right = node->right;
	}                          
}

void dlList::removeNode(relationType rel, Vehicle *node)
{
	if (rel==BEFORE)
	{
		if (node->left == this->head)          
		{
			this->head = node;
			this->head->left = NULL;
		}
		else
			removeNode((Vehicle *)node->left);              
	}
	else if (rel==AFTER)
	{
		if (node->right == this->tail)          
		{
			this->tail = node;
			this->tail->right = NULL;
		}
		else
			removeNode((Vehicle *)node->right); 
	}
}
