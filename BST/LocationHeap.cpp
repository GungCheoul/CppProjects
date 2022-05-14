#include "LocationHeap.h"

LocationHeap::LocationHeap()
{
	int capacity = 1;
	Heap = new LocationHeapNode*[8]();
	for (int i = 0; i < 9; i++)
		*(Heap + i) = new LocationHeapNode;
	size = 0;
}

LocationHeap::~LocationHeap()
{
	delete Heap;
	size = 0;
}

int LocationHeap::GetSize()
{
	return size;
}

bool LocationHeap::Insert(char * location)
{
    //BPOP
	if (location == NULL)
		return false;
	int check = 0;
	int current = size;
	while (current != 0) {
		if (strcmp(Heap[current]->GetLoc(), location) == 0) {
			check = 1;
			break;
		}
		current = current - 1;
	}
	if (size == 0 || check == 0) {
		Heap[++size]->SetLoc(location);
		Heap[size]->SetCount(1);
	}
	else
		Heap[current]->SetCount(Heap[current]->GetCount() + 1);

	int child = size;
	int parent = child / 2;
	while (child > 1 && Heap[parent]->GetCount() < Heap[child]->GetCount()) {
		int tmp = Heap[parent]->GetCount();
		Heap[parent]->SetCount(Heap[child]->GetCount());
		Heap[child]->SetCount(tmp);
		child = parent;
		parent = child / 2;
	}
	return true;
}

void LocationHeap::Print()
{
    //PRINT H
	ofstream flog;
	flog.open("log.txt", ios::app);
	int current = 1;
	queue <LocationHeapNode* > lv_queue;
	lv_queue.push(Heap[1]);
	lv_queue.push(NULL);
	LocationHeapNode* heap = new LocationHeapNode();
	while (lv_queue.size() > 1) {
		heap = lv_queue.front();
		lv_queue.pop();
		if (heap == NULL)
			lv_queue.push(NULL);
		else {
			if (Heap[++current]->GetLoc())
				lv_queue.push(Heap[current]);
			flog << heap->GetCount() << "/" << heap->GetLoc() << endl;
		}
	}
}
