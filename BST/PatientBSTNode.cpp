#include "PatientBSTNode.h"

PatientBSTNode::PatientBSTNode()
{
	Name = NULL;
	Disease = NULL;
	pLeft = NULL;
	pRight = NULL;
}

PatientBSTNode::~PatientBSTNode()
{
	Name = NULL;
	Disease = NULL;
	pLeft = NULL;
	pRight = NULL;
}

char * PatientBSTNode::GetName()
{
	return Name;
}

char PatientBSTNode::GetDisease()
{
	return Disease;
}

PatientBSTNode * PatientBSTNode::GetLeft()
{
	return pLeft;
}

PatientBSTNode * PatientBSTNode::GetRight()
{
	return pRight;
}

void PatientBSTNode::SetName(char * name)
{
	Name = name;
}

void PatientBSTNode::SetDisease(char disease)
{
	Disease = disease;
}

void PatientBSTNode::SetLeft(PatientBSTNode * node)
{
	pLeft = node;
}

void PatientBSTNode::SetRight(PatientBSTNode * node)
{
	pRight = node;
}
