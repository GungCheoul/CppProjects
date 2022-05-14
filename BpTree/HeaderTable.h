#pragma once
#include "FPNode.h"
#include <list>

class HeaderTable
{
private:
	list<pair<int, string> > indexTable; // item이랑 frequency에 대한 list. table
	map<string, FPNode*> dataTable; // item이랑 FPNode(Next로 node들 연결)
public:
	HeaderTable() { }
	~HeaderTable();
	void insertTable(char* item, int frequency);
	list<pair<int, string>> getindexTable() { return indexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); }
	void ascendingIndexTable() { indexTable.sort(); }
	int find_frequency(string item);
};


