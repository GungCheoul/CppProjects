#include "LocationHeapNode.h"

LocationHeapNode::LocationHeapNode()
{
	Count = 0;
	Location = NULL;
}

LocationHeapNode::~LocationHeapNode()
{
	Count = 0;
	Location = NULL;
}

int LocationHeapNode::GetCount()
{
	return Count;
}

char * LocationHeapNode::GetLoc()
{
	return Location;
}

void LocationHeapNode::SetCount(int count)
{
	Count = count;
}

void LocationHeapNode::SetLoc(char * location)
{
	Location = location;
}
