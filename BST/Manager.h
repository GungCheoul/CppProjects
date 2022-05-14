#pragma once
#include "PatientNode.h"
#include "LocationBST.h"
#include "LocationHeap.h"

class Manager
{
private:
	queue <PatientNode *> * ds_queue;
	LocationBST * ds_bst;
	LocationHeap * ds_heap;
	ifstream fin;
	ofstream flog;

public:
	Manager();
	~Manager();

	void run(const char * command);
	bool LOAD();
	bool ADD();
	bool QPOP();
	bool SEARCH();
	bool PRINT();
	bool BPOP();

	void PrintErrorCode(int num);
	void PrintSuccess(char * act);
};
