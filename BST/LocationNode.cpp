#include "LocationNode.h"

LocationNode::LocationNode()
{
	Location = NULL;
	BST = new PatientBST();
	pLeft = NULL;
	pRight = NULL;
}

LocationNode::~LocationNode()
{
	Location = NULL;
	BST = new PatientBST();
	pLeft = NULL;
	pRight = NULL;
}

char * LocationNode::GetLoc()
{
	return Location;
}

PatientBST * LocationNode::GetBST()
{
	return BST;
}

LocationNode * LocationNode::GetLeft()
{
	return pLeft;
}

LocationNode * LocationNode::GetRight()
{
	return pRight;
}

void LocationNode::SetLoc(char * location)
{
	Location = location;
}

void LocationNode::SetLeft(LocationNode * node)
{
	pLeft = node;
}

void LocationNode::SetRight(LocationNode * node)
{
	pRight = node;
}
