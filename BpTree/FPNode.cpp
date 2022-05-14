#include "FPNode.h"

FPNode::FPNode()	//constructor
{
	parent = NULL;
	next = NULL;
	item = new char[40]; memset(item, 0, 40);
	frequency = 0;
}

FPNode::~FPNode()//destructor
{
	parent = NULL;
	next = NULL;
	delete[] item;
	frequency = 0;
	children.clear();
}

FPNode* FPNode::getChildrenNode(string item) {
	map<string, FPNode*>::iterator it = children.find(item); // item이 들어있는 fpnode 찾기
	if (it == children.end()) return NULL;
	FPNode* findNode = it->second;
	return findNode; // 찾은 fpnode 가져오기
}
